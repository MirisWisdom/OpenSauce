{
  "includes": [
    "Common.gypi"
  ],
  "target_defaults": {
    "variables": {
      "haloce_installation": "D:/Program Files (x86)/Microsoft Games/Halo Custom Edition"
    },
    "msbuild_settings": {
      "ClCompile": {
        "WarningLevel": "Level3",
        "StructMemberAlignment": "1Byte",
        "AdditionalOptions": "/J /Zc:trigraphs",
        "CallingConvention": "StdCall"
      },
      "Link": {
        "AdditionalLibraryDirectories": [
          ">(project_root)/external/boost_1_65_1/stage/lib"
        ]
      }
    },
    "defines": [
      "_CRT_SECURE_NO_WARNINGS",
      "BLAM_ARCHITECTURE_X86",
      "BLAM_PLATFORM_WINDOWS",         
      "PLATFORM_TARGET=PLATFORM_TARGET_PC"
    ],
    "configurations":
    {
      "Game":{
        "abstract": 1,
        "include_dirs": [
          "../external/boost_1_65_1",
          "$(ProgramFiles)/Microsoft DirectX SDK (June 2010)/Include"
        ],
        "defines": [
          "PLATFORM_IS_EDITOR=FALSE"
        ]
      },
      "Debug": {
        "abstract": 1,
        "inherit_from": ["DebugBase", "Game"],
        "include_dirs": [
          "$(ProgramFiles)/Visual Leak Detector/include"
        ]
      },
      "Release": {
        "abstract": 1,
        "inherit_from": ["ReleaseBase", "Game"]
      },
      "SDK": {
        "abstract": 1
      },
      "Tool": {
        "abstract": 1,
        "inherit_from": ["SDK"],
        "defines": [
          "BLAM_TARGET_APP_TYPE_TOOL"
        ]
      },
      "Editor": {
        "abstract": 1,
        "inherit_from": ["SDK"],
        "defines": [
          "PLATFORM_IS_EDITOR=TRUE",
          "BLAM_TARGET_APP_TYPE_EDITOR"
        ]
      },
      "Client": {
        "abstract": 1,
        "defines": [
          "PLATFORM_TYPE_CLIENT",
          "BLAM_TARGET_APP_TYPE_CLIENT",
          "BLAM_TARGET_APP_TOOL_TYPE_NONE",
          "GSI_NO_THREADS",
          "DX_WRAPPER",
          "HALO1_CE_EXPORTS",
          "YELO_USE_GAMESPY_OPEN"
        ],
      },
      "Server": {
        "abstract": 1,
        "defines": [
          "PLATFORM_TYPE_DEDI",
          "BLAM_TARGET_APP_TYPE_SERVER",
          "BLAM_TARGET_APP_TOOL_TYPE_NONE",
          "YELO_USE_GAMESPY_OPEN"
        ]
      }
    }
  }
}
