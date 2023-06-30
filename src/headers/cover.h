#ifndef TAG_COVER_H
#define TAG_COVER_H

#include <iostream>
#include <sys/stat.h>
#include <cstdlib>
#include <napi.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/attachedpictureframe.h>

using namespace std;

struct Cover_Info
{
    unsigned long size = 0;
    // can be "image/png" or "image/jpeg".
    string mime_type = "";
    char *cover_art_data = nullptr;

    ~Cover_Info()
    {
        if (cover_art_data != nullptr)
            delete cover_art_data;
    }
};

class Cover
{
private:
    // this method is not exposed to js world directly.
    static void __get_cover_data_byte(string file_path, Cover_Info *cover_info);
    static bool __save_cover_art(string file_path, string cover_art_dest);
    static bool __set_new_cover_art(string file_path, string mime_type, string cover_file_path);
    static bool __set_new_cover_art(string file_path, string mime_type, char *data, unsigned long size);
    inline static const char *picture_id = "APIC";

public:
    static Napi::Boolean save_cover_art(const Napi::CallbackInfo &info);
    static Napi::Boolean set_new_cover_art(const Napi::CallbackInfo &info);
    static void get_cover_art(const Napi::CallbackInfo &info);
};

#endif
