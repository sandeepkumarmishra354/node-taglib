#include "../headers/exif.h"

// private methods.
void Exif::__set_exif(TagLib::Tag *tag, string key, string data)
{
    if (key == "album")
        tag->setAlbum(data.c_str());
    else if (key == "artist")
        tag->setArtist(data.c_str());
    else if (key == "comment")
        tag->setComment(data.c_str());
    else if (key == "genre")
        tag->setGenre(data.c_str());
    else if (key == "title")
        tag->setTitle(data.c_str());
    else if (key == "track")
        tag->setTrack(stoi(data));
    else if (key == "year")
        tag->setYear(stoi(data));
}

map<string, string> Exif::__get_exif_data(string file_path)
{
    std::map<string, string> map;
    TagLib::FileRef f = TagLib::FileRef(file_path.c_str());
    auto audio_props = f.audioProperties();
    auto tag = f.tag();
    auto album = tag->album();
    auto artist = tag->artist();
    auto comment = tag->comment();
    auto genre = tag->genre();
    auto title = tag->title();
    map["album"] = album.isNull() ? "" : album.to8Bit();
    map["artist"] = artist.isNull() ? "" : artist.to8Bit();
    map["comment"] = comment.isNull() ? "" : comment.to8Bit();
    map["genre"] = genre.isNull() ? "" : genre.to8Bit();
    map["title"] = title.isNull() ? "" : title.to8Bit();
    map["track"] = to_string(tag->track());
    map["year"] = to_string(tag->year());
    map["bitrate"] = audio_props ? to_string(audio_props->bitrate()) : "";
    map["channels"] = audio_props ? to_string(audio_props->channels()) : "";
    map["sampleRate"] = audio_props ? to_string(audio_props->sampleRate()) : "";
    map["duration"] = audio_props ? to_string(audio_props->length()) : "";
    return map;
}

bool Exif::__set_exif_data(string file_path, map<string, string> &new_exif)
{
    TagLib::FileRef f = TagLib::FileRef(file_path.c_str());
    auto tag = f.tag();
    for (auto i = new_exif.begin(); i != new_exif.end(); i++)
    {
        Exif::__set_exif(tag, i->first, i->second);
    }
    return f.save();
}

// public methods.
// export this function to js-side.
Napi::Value Exif::get_exif_data(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        Napi::Error::New(env, "node-taglib - getExifData: expects one argument as filepath.").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsString())
    {
        Napi::Error::New(env, "node-taglib - getExifData: expects argument of type string.").ThrowAsJavaScriptException();
        return env.Null();
    }
    string file_path = info[0].ToString();
    auto map = Exif::__get_exif_data(file_path);
    Napi::Object ret_obj = Napi::Object::New(env);
    for (auto i = map.begin(); i != map.end(); i++)
    {
        ret_obj.Set(i->first, Napi::String::New(env, i->second));
    }
    return ret_obj;
}

Napi::Boolean Exif::set_exif_data(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2)
    {
        Napi::Error::New(env, "node-taglib - setExifData: expects two argument.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[0].IsString())
    {
        Napi::Error::New(env, "node-taglib - setExifData: expects 1st argument of type string.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[1].IsObject())
    {
        Napi::Error::New(env, "node-taglib - setExifData: expects 2nd argument of type object.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    string file_path = info[0].ToString();
    auto new_exif_data = info[1].ToObject();
    string exif_keys[] = {"album", "artist", "comment", "genre", "title", "track", "year"};
    map<string, string> map;
    for (string key : exif_keys)
    {
        if (!new_exif_data.Get(key).IsUndefined())
            map[key] = new_exif_data.Get(key).ToString();
    }
    return Napi::Boolean::New(env, Exif::__set_exif_data(file_path, map));
}
