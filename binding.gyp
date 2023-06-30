{
    "variables": {
        "taglib_dir": "<!@(pwd)/taglib-1.13/_install"
    },
    "targets": [
        {
            "target_name": "node_taglib",
            # "type": "static_library",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": [
                "./src/binding.cpp",
                "./src/sources/exif.cpp",
                "./src/sources/cover.cpp",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "<(taglib_dir)/include"
            ],
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
            "libraries": [
                "<(taglib_dir)/lib/libtag.a",
                "<(taglib_dir)/lib/libtag_c.a"
            ]
        }
    ]
}