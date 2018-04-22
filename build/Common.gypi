{
  "target_defaults": {
    "variables": {
      "project_root": "<!(echo %ProjectRoot%)"
    },
    "msbuild_configuration_attributes": {
      "OutputDirectory": "$(SolutionDir)../../bin/$(ProjectName)/$(Configuration)/",
      "IntermediateDirectory": "$(SolutionDir)../../objects/$(ProjectName)/$(Configuration)/",
      "CharacterSet": "MultiByte"
    },
    "msbuild_settings": {
      "ClCompile": {
        "SDLCheck": "true",
        "MinimalRebuild": "false",
        "MultiProcessorCompilation": "true",
        "EnableEnhancedInstructionSet": "StreamingSIMDExtensions2",
        "LanguageStandard": "stdcpp17",
        "UseFullPaths": "false",
        "StringPooling": "true",
        "RuntimeTypeInfo": "false"
      },
      "Link": {
        "GenerateDebugInformation": "true",
        "LinkTimeCodeGeneration": "UseLinkTimeCodeGeneration",
        "AssemblyDebug": "true",
        "SubSystem": "Windows",
        "IgnoreSpecificDefaultLibraries": "LIBC",
        "EnableCOMDATFolding": "true",
      },
      "Lib": {
        "TargetMachine": "MachineX86",
        "LinkTimeCodeGeneration": "true"
      }
    },
    "msvs_configuration_platform": "Win32",
    "defines": [
      "WIN32",
      "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
      "_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING"
    ],
    "configurations":
    {
      "DebugBase": {
        "abstract": 1,
        "defines": [
          "_DEBUG"
        ],
        "msbuild_settings": {
          "ClCompile": {
            "Optimization": "Disabled",
            "RuntimeLibrary": "MultiThreadedDebugDLL",
            "WholeProgramOptimization": "false",
            "IntrinsicFunctions": "false",
            "BasicRuntimeChecks": "EnableFastChecks",
            "DebugInformationFormat": "EditAndContinue",
            "InlineFunctionExpansion": "OnlyExplicitInline"
          },
          "Link": {
            "ImageHasSafeExceptionHandlers": "false",
            "LinkIncremental": "true",
            "OptimizeReferences": "false"
          }
        }
      },
      "ReleaseBase": {
        "abstract": 1,
        "defines": [
          "NDEBUG"
        ],
        "msbuild_settings": {
          "ClCompile": {
            "Optimization": "MaxSpeed",
            "RuntimeLibrary": "MultiThreadedDLL",
            "WholeProgramOptimization": "true",
            "IntrinsicFunctions": "true",
            "BasicRuntimeChecks": "Default",
            "FunctionLevelLinking": "true",
            "DebugInformationFormat": "ProgramDatabase",
            "InlineFunctionExpansion": "AnySuitable",
            "FavorSizeOrSpeed": "Speed"
          },
          "Link": {
            "ImageHasSafeExceptionHandlers": "true",
            "LinkIncremental": "false",
            "OptimizeReferences": "true"
          }
        }
      }
    }
  }
}
