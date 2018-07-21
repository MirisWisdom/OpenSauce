{
  "includes": [
    "Common.gypi",
    "GameSpyOpen.generated.gypi"
  ],
  "targets": [  
    {
      "target_name": "GameSpyOpen",
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
        "COMMON_API="
      ],
      "msbuild_settings": {
        "ClCompile": {
          "AdditionalOptions": "/J",
          "CallingConvention": "StdCall"
        },
        "Lib": {
          "AdditionalDependencies": [
            "User32.lib"
          ],
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
        "<@(sourceFilesGameSpyOpen)",
      ]
    }
  ]
}