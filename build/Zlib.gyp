{
  "includes": [
    "Common.gypi",
    "Zlib.generated.gypi"
  ],
  "targets": [  
    {
      "target_name": "Zlib",
      "type": "static_library",
      "include_dirs": [
        "../external"
      ],
      "direct_dependent_settings": {
        "include_dirs": [
          "../external"
        ]
      },
      "defines": [
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE"
      ],
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
        "<@(sourceFilesZlib)",
      ]
    }
  ]
}