/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_config_definition_count = 3,
		};
	};

	namespace Config
	{
		struct s_definition
		{
			bool Initialized;
			PAD24;

			struct _Cheats {
				// When true, means Yelo should update the game state with the config values
				struct _UpdateFlags {
					byte_flags Invincible : 1;
					byte_flags Cloak : 1;
				}UpdateFlags; BOOST_STATIC_ASSERT( sizeof(_UpdateFlags) == sizeof(byte_flags) );
				bool InfAmmo;
				bool InfGrenades;
				bool Invincible;
				bool Cloak;
				bool EnablePrintFunction;
				bool IceCreamFlavors[21 + 1];

				struct _Input {
					word_flags InfAmmo;
					word_flags InfGrenades;
					word_flags Invincible;
					word_flags Cloak;
					word_flags AiEraseAll;
					word_flags AutoWin;
				}Input;
			}Cheats;

			struct _View {
				struct _Input {
					word_flags Wireframe;
					word_flags Letterbox;
					word_flags HudOpacity;
					word_flags Screenshot;

					word_flags DisplayPosition;
					word_flags ToggleDof;
					word_flags ToggleBloom;
					word_flags ToggleCrossFade;
				}Input;

				struct _InputCamera {
					word_flags FpCamera;
					word_flags TpCamera;
					word_flags ChaseCamera;
					word_flags ScriptedCamera;
					word_flags EditorCamera;
					word_flags AnchoredCamera;
					word_flags LookControlThumb; // thumbstick to use for look control
					PAD16;
				}InputCamera;

				struct _InputSliders {
					struct _InputSlider {
						word_flags Inc;
						word_flags Dec;
					}	Depth,
						Fov;
				}InputSliders;

				struct _Blah {
					struct _Input {
						word_flags IncSpeed;
						word_flags DecSpeed;
						word_flags IncHorizShift;
						word_flags DecHorizShift;
						word_flags IncVertShift;
						word_flags DecVertShift;
					}Input;

					real Speed;
					real_bounds SpeedBounds;
					real Resistance;
				}Look, Move;

				void ToDefaults();
			}View;

			struct _Input {
				word_flags SaveGameState;
				word_flags LoadGameState;
				word_flags AiToggle;
				word_flags GameSpeedToggle;
				word_flags ThirdPersonOverride;
				word_flags HighSpeedCapture;
				word_flags TrackMovement;
				word_flags ChangeActiveConfig;

				struct _InputSlider {
					word_flags Inc;
					word_flags Dec;
				}	GameSpeed;

				struct _InputToggle {
					word_flags On;
					word_flags Off;
				}	Cinematic;

				real AdjustScale; // does this belong in this group?
				real Tolerance; // thumbstick debounce

				void ToDefaults();
			}Input;

			struct _Misc {
				// Update flags. When true, means Yelo should update the game state with the config values
				struct _UpdateFlags {
					byte_flags JumpHeight : 1;
					byte_flags PlayerSize : 1;
					byte_flags GravityScale : 1;
				}UpdateFlags; BOOST_STATIC_ASSERT( sizeof(_UpdateFlags) == sizeof(byte_flags) );
				bool SaveStats;
				bool HighSpeedCapture;
				bool CoordinateDisplay;

				real JumpHeight;
				real PlayerSize;
				real GravityScale;

				void ToDefaults();
			}Misc;

			void ToDefaults();
			void ToInputDefaults();
		};

		void Initialize();
		void Dispose();

		// Calls all of yelo's config init functions
		void InitializeForNewConfig();
		// Calls all of yelo's config dispose function
		void DisposeFromOldConfig();

		int32& CurrentIndex();
		s_definition& Current();
	};
};