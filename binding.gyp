{
  "targets": [
    {
      "target_name": "nbiobsp",
      "sources": [ "src/nbiobsp.cpp" ],
      'include_dirs': [
        './src/include',
      ],
      'libraries': [
        "<(module_root_dir)/src/lib/NBioBSP.lib",
        "<(module_root_dir)/src/lib/NBioAPI_CheckValidity.lib",
        "<(module_root_dir)/src/lib/NImgConv.lib",
      ],
      "cflags": ["-Wall", "-std=c++11", "-lNBioBSP"],
    }
  ]
}