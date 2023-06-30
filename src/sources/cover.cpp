#include "../headers/cover.h"

void Cover::__get_cover_data_byte(string file_path, Cover_Info *cover_info)
{
    TagLib::MPEG::File mpeg_file(file_path.c_str());
    TagLib::ID3v2::Tag *id3v2_tag = mpeg_file.ID3v2Tag();
    if (id3v2_tag)
    {
        TagLib::ID3v2::FrameList frame_list;
        TagLib::ID3v2::AttachedPictureFrame *pic_frame;
        frame_list = id3v2_tag->frameListMap()[Cover::picture_id];
        if (!frame_list.isEmpty())
        {
            for (auto it = frame_list.begin(); it != frame_list.end(); ++it)
            {
                pic_frame = (TagLib::ID3v2::AttachedPictureFrame *)(*it);
                if (pic_frame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
                {
                    if (pic_frame->picture().size() > 0)
                    {
                        cover_info->mime_type = pic_frame->mimeType().to8Bit();
                        cover_info->size = pic_frame->picture().size();
                        cover_info->cover_art_data = new char[cover_info->size]();
                        memcpy(cover_info->cover_art_data, pic_frame->picture().data(), cover_info->size);
                    }
                }
            }
        }
    }
}

bool Cover::__save_cover_art(string file_path, string cover_art_dest)
{
    Cover_Info info;
    __get_cover_data_byte(file_path, &info);
    const bool has_cover_art = info.cover_art_data != nullptr && info.size > 0;
    if (has_cover_art)
    {
        FILE *file = fopen(cover_art_dest.c_str(), "wb");
        fwrite(info.cover_art_data, info.size, 1, file);
        fclose(file);
    }
    return has_cover_art;
}

bool Cover::__set_new_cover_art(string file_path, string mime_type, string cover_file_path)
{
    struct stat stat_buf;
    int rc = stat(cover_file_path.c_str(), &stat_buf);
    if (rc == 0)
    {
        char buffer[stat_buf.st_size];
        FILE *file = fopen(cover_file_path.c_str(), "rb");
        fread(buffer, stat_buf.st_size, 1, file);
        fclose(file);
        return __set_new_cover_art(file_path, mime_type, buffer, stat_buf.st_size);
    }
    return false;
}

bool Cover::__set_new_cover_art(string file_path, string mime_type, char *data, unsigned long size)
{
    TagLib::ByteVector byte_vector(data, size);
    TagLib::MPEG::File mpeg_file(file_path.c_str());
    TagLib::ID3v2::Tag *id3v2_tag = mpeg_file.ID3v2Tag();
    if (id3v2_tag)
    {
        TagLib::ID3v2::FrameList frame_list;
        TagLib::ID3v2::AttachedPictureFrame *pic_frame;
        frame_list = id3v2_tag->frameListMap()[Cover::picture_id];
        if (!frame_list.isEmpty())
        {
            for (auto it = frame_list.begin(); it != frame_list.end(); ++it)
            {
                pic_frame = (TagLib::ID3v2::AttachedPictureFrame *)(*it);
                if (pic_frame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
                {
                    pic_frame->setMimeType(mime_type);
                    pic_frame->setPicture(byte_vector);
                    mpeg_file.save();
                    return true;
                }
            }
        }
    }
    return false;
}

Napi::Boolean Cover::save_cover_art(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2)
    {
        Napi::Error::New(env, "node-taglib - saveCoverArt: expects two argument.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[0].IsString())
    {
        Napi::Error::New(env, "node-taglib - saveCoverArt: expects 1st argument of type string.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[1].IsString())
    {
        Napi::Error::New(env, "node-taglib - saveCoverArt: expects 2nd argument of type string.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    const string file_path = info[0].ToString();
    const string cover_art_dest = info[1].ToString();
    return Napi::Boolean::New(env, Cover::__save_cover_art(file_path, cover_art_dest));
}

void Cover::get_cover_art(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    const Napi::Function callback = info[1].As<Napi::Function>();
    if (info.Length() < 2)
    {
        Napi::Error::New(env, "node-taglib - getCoverArt: expects 2 argument.").ThrowAsJavaScriptException();
        callback.Call({env.Null()});
    }
    if (!info[0].IsString())
    {
        Napi::Error::New(env, "node-taglib - getCoverArt: expects 1st argument of type string.").ThrowAsJavaScriptException();
        callback.Call({env.Null()});
    }
    if (!info[1].IsFunction())
    {
        Napi::Error::New(env, "node-taglib - getCoverArt: expects 2nd argument of type function.").ThrowAsJavaScriptException();
        callback.Call({env.Null()});
    }
    const string file_path = info[0].ToString();
    Cover_Info cover_info;
    __get_cover_data_byte(file_path, &cover_info);
    uint8_t *buffer = (uint8_t *)cover_info.cover_art_data;
    const auto uint8_buffer = Napi::Buffer<uint8_t>::New(env, buffer, cover_info.size);
    callback.Call({uint8_buffer});
}

Napi::Boolean Cover::set_new_cover_art(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 3)
    {
        Napi::Error::New(env, "node-taglib - setNewCoverArt: expects 3 argument.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[0].IsString())
    {
        Napi::Error::New(env, "node-taglib - setNewCoverArt: expects 1st argument of type string.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[1].IsString())
    {
        Napi::Error::New(env, "node-taglib - setNewCoverArt: expects 2nd argument of type string.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!(info[2].IsString() || info[2].IsBuffer()))
    {
        Napi::Error::New(env, "node-taglib - setNewCoverArt: expects 3rd argument of type string or buffer.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    bool is_success = false;
    const string mime_type = info[1].ToString();
    /*if (mime_type != "image/png" || mime_type != "image/jpeg")
    {
        Napi::Error::New(env, "node-taglib - setNewCoverArt: allowed mimeType = image/png or image/jpeg.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, is_success);
    }*/
    const string file_path = info[0].ToString();
    if (info[2].IsString())
    {
        string new_cover_file = info[2].ToString();
        is_success = Cover::__set_new_cover_art(file_path, mime_type, new_cover_file);
    }
    else
    {
        auto new_cover_buffer = info[2].As<Napi::Buffer<uint8_t>>();
        uint8_t *data = new_cover_buffer.Data();
        unsigned long size = new_cover_buffer.ByteLength();
        is_success = Cover::__set_new_cover_art(file_path, mime_type, (char *)data, size);
    }
    return Napi::Boolean::New(env, is_success);
}
