{
  "includes": [
    "Common.gypi",
    "Mongoose.generated.gypi"
  ],
  "targets": [  
    {
      "target_name": "Mongoose",
      "type": "static_library",
      "include_dirs": [
        "../external"
      ],
      "direct_dependent_settings": {
        "include_dirs": [
          "../external"
        ]
      },
      "msbuild_settings": {
        "ClCompile": {
          "AdditionalOptions": "/J",
          "CallingConvention": "StdCall"
        }
      },
      "configurations": {
        "Debug": {
          "inherit_from": [ "DebugBase" ]
        },
        "Release": {
          "inherit_from": [ "ReleaseBase" ]
        }
      },
      "default_configuration": "Debug",
      "sources": [
        "<@(mongooseSourceFiles)",
      ]
    }
  ]
}