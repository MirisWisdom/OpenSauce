{
  "includes": [
    "CommonOpenSauce.gypi",
    "Blam.generated.gypi",
    "Yelo.generated.gypi",
    "Halo.generated.gypi"
  ],
  "targets": [  
    {
      "target_name": "yelo",
      "type": "static_library",
      "include_dirs": [
        "../source",
        "../external"
      ],
      "direct_dependent_settings": {
        "include_dirs": [
          "../source"
        ]
      },
      "msbuild_settings": {
        "Lib": {
          "AdditionalDependencies": [
            "TinyXml.lib",
            "7zip.lib",
            "Zlib.lib",
            "User32.lib",
            "Shell32.lib",
          ],
        }
      },
      "configurations": {
        "Debug Client": {
          "inherit_from": [ "Debug", "Client" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Debug/lib",
                ">(project_root)/bin/7zip/Debug/lib",
                ">(project_root)/bin/Zlib/Debug/lib"
              ]
            }
          }
        },
        "Debug Server": {
          "inherit_from": [ "Debug", "Server" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Debug/lib",
                ">(project_root)/bin/7zip/Debug/lib",
                ">(project_root)/bin/Zlib/Debug/lib"
              ]
            }
          }
        },
        "Release Client": {
          "inherit_from": [ "Release", "Client" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Release/lib",
                ">(project_root)/bin/7zip/Release/lib",
                ">(project_root)/bin/Zlib/Release/lib"
              ]
            }
          }
        },
        "Release Server": {
          "inherit_from": [ "Release", "Server" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Release/lib",
                ">(project_root)/bin/7zip/Release/lib",
                ">(project_root)/bin/Zlib/Release/lib"
              ]
            }
          }
        }
      },
      "default_configuration": "Debug Client",
      "sources": [
        "<@(sourceFilesYelo)"
      ],
      "msvs_precompiled_header": "../source/yelo/yelo.h",
      "msvs_precompiled_source": "../source/yelo/yelo.cpp"
    },    
    {
      "target_name": "blam",
      "type": "static_library",
      "dependencies": [
        "yelo"
      ],
      "include_dirs": [
        "../source",
        "../external"
      ],
      "direct_dependent_settings": {
        "include_dirs": [
          "../source"
        ]
      },
      "msbuild_settings": {
        "Lib": {
          "AdditionalDependencies": [
            "TinyXml.lib"
          ],
        }
      },
      "configurations": {
        "Debug Client": {
          "inherit_from": [ "Debug", "Client" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Debug/lib"
              ]
            }
          }
        },
        "Debug Server": {
          "inherit_from": [ "Debug", "Server" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Debug/lib"
              ]
            }
          }
        },
        "Release Client": {
          "inherit_from": [ "Release", "Client" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Release/lib"
              ]
            }
          }
        },
        "Release Server": {
          "inherit_from": [ "Release", "Server" ],
          "msbuild_settings": {
            "Lib": {
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/TinyXml/Release/lib"
              ]
            }
          }
        }
      },
      "default_configuration": "Debug Client",
      "sources": [
        "<@(sourceFilesBlam)"
      ],
      "msvs_precompiled_header": "../source/blam/blam.h",
      "msvs_precompiled_source": "../source/blam/blam.cpp"
    },      
    {
      "target_name": "halo",
      "type": "shared_library",
      "include_dirs": [
        "../source",
        "../source/halo",
        "../external"
      ],
      "dependencies": [
        "blam",
        "yelo"
      ],
      "libraries": [
        "-lCrashRpt1401.lib",
        "-lGameSpyOpen.lib",
        "-lWs2_32.lib"
      ],
      "msbuild_settings": {
        "Link": {
          "OutputFile": "$(OutDir)$(TargetName)$(TargetExt)",
          "AdditionalLibraryDirectories": [
            ">(project_root)/external/CrashRpt/lib/x86"
          ]
        }
      },
      "msvs_postbuild": "cd /d $(SolutionDir)../../scripts\nset ProjectRoot=$(SolutionDir)../..\npowershell -ExecutionPolicy Bypass -File cake.ps1 -Script CakeScripts/PostBuild.cake -ScriptArgs '--BuildConfiguration=\"$(Configuration)\"'",
      "configurations": {
        "Debug Client": {
          "inherit_from": [ "Debug", "Client" ],
          "msbuild_configuration_attributes": {
            "TargetName": "dinput8"
          },
          "msbuild_settings": {
            "Link": {
              "AdditionalDependencies": [
                "D3dx9d.lib",
                "GWEN.lib"
              ],
              "AdditionalLibraryDirectories": [
                "$(ProgramFiles)/Microsoft DirectX SDK (June 2010)/Lib/x86",
                ">(project_root)/bin/GameSpyOpen/Debug/lib",
                ">(project_root)/bin/GWEN/Debug/lib"
              ]
            }
          }
        },
        "Debug Server": {
          "inherit_from": [ "Debug", "Server" ],
          "msbuild_configuration_attributes": {
            "TargetName": "OpenSauceDedi"
          },
          "msbuild_settings": {
            "Link": {
              "AdditionalDependencies": [
                "Mongoose.lib"
              ],
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/GameSpyOpen/Debug/lib",                
                ">(project_root)/bin/Mongoose/Debug/lib"
              ]
            }
          }
        },
        "Release Client": {
          "inherit_from": [ "Release", "Client" ],
          "msbuild_configuration_attributes": {
            "TargetName": "dinput8"
          },
          "msbuild_settings": {
            "Link": {
              "AdditionalDependencies": [
                "D3dx9.lib",
                "GWEN.lib"
              ],
              "AdditionalLibraryDirectories": [
                "$(ProgramFiles)/Microsoft DirectX SDK (June 2010)/Lib/x86",
                ">(project_root)/bin/GameSpyOpen/Release/lib",
                ">(project_root)/bin/GWEN/Release/lib"
              ]
            }
          }
        },
        "Release Server": {
          "inherit_from": [ "Release", "Server" ],
          "msbuild_configuration_attributes": {
            "TargetName": "OpenSauceDedi"
          },
          "msbuild_settings": {
            "Link": {
              "AdditionalDependencies": [
                "Mongoose.lib"
              ],
              "AdditionalLibraryDirectories": [
                ">(project_root)/bin/GameSpyOpen/Release/lib",                
                ">(project_root)/bin/Mongoose/Release/lib"
              ]
            }
          }
        }
      },
      "default_configuration": "Debug Client",
      "sources": [
        "<@(sourceFilesHalo)"
      ],
      "msvs_precompiled_header": "../source/halo/halo.h",
      "msvs_precompiled_source": "../source/halo/halo.cpp"
    }
  ]
}