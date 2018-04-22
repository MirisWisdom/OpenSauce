{
  "includes": [
    "Common.gypi",
    "7zip.generated.gypi"
  ],
  "targets": [  
    {
      "target_name": "7zip",
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
        "<@(sevenZipSourceFiles)",
      ]
    }
  ]
}