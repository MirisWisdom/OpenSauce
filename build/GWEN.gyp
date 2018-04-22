{
  "includes": [
    "Common.gypi",
    "GWEN.generated.gypi"
  ],
  "targets": [  
    {
      "target_name": "GWEN",
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
        "_CRT_SECURE_NO_WARNINGS"
      ],
      "msbuild_settings": {
        "ClCompile": {
          "StructMemberAlignment": "1Byte",
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
        "<@(gwenSourceFiles)",
      ]
    }
  ]
}