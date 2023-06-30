#ifndef TAG_EXIF_H
#define TAG_EXIF_H

#include <napi.h>
#include <map>
#include <string>
#include <taglib/fileref.h>

using namespace std;

class Exif
{
private:
    static map<string, string> __get_exif_data(string file_path);
    static bool __set_exif_data(string file_path, map<string, string> &new_exif);
    static void __set_exif(TagLib::Tag *tag, string key, string data);

public:
    static Napi::Value get_exif_data(const Napi::CallbackInfo &info);
    static Napi::Boolean set_exif_data(const Napi::CallbackInfo &info);
};

#endif
