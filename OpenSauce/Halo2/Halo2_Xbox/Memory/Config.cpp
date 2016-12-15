/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Memory/Config.hpp"

#include "Interface/Input.hpp"

namespace Yelo
{
	namespace Config
	{
		void s_definition::_View::ToDefaults()
		{
			Look.Speed = 1.0f;
			Look.SpeedBounds.lower = 0.1f;
			Look.SpeedBounds.upper = 5.0f;
			Look.Resistance = 0.15f;

			Move.Speed = 1.0f;
			Move.SpeedBounds.lower = 0.25f;
			Move.SpeedBounds.upper = 10.0f;
			Move.Resistance = 0.1f;
		}
		void s_definition::_Input::ToDefaults()
		{
			AdjustScale = 1.03f;
			Tolerance = 0.225f;
		}
		void s_definition::_Misc::ToDefaults()
		{
			JumpHeight = PlayerSize = GravityScale = 1.0f;
		}
		void s_definition::ToDefaults()
		{
			XboxLib::_memzero(this, sizeof(*this));

			View.ToDefaults();
			Input.ToDefaults();
			Misc.ToDefaults();
		}
		void s_definition::ToInputDefaults()
		{
			using namespace Yelo::Flags;

			//////////////////////////////////////////////////////////////////////////
			// Cheats
			Cheats.Input.InfAmmo =		_input_button_x | _input_button_dpad_left;
			Cheats.Input.InfGrenades =	_input_button_x | _input_button_dpad_right;
			Cheats.Input.Invincible =	_input_button_x | _input_button_dpad_up;
			Cheats.Input.Cloak =		_input_button_x | _input_button_dpad_down;
			Cheats.Input.AiEraseAll =									_input_button_undefined;
			Cheats.Input.AutoWin =		_input_button_b | _input_button_a;
			//////////////////////////////////////////////////////////////////////////
			// View
			View.Input.Wireframe =		_input_button_b | _input_button_dpad_down;
			View.Input.Letterbox =										_input_button_undefined;//_input_button_b | _input_button_dpad_left;
			View.Input.HudOpacity =										_input_button_undefined;//_input_button_b | _input_button_dpad_right;
			View.Input.Screenshot =	_input_button_white | _input_button_dpad_up;
			View.Input.DisplayPosition =_input_button_b | _input_button_y;
			View.Input.ToggleDof =										_input_button_undefined;
			View.Input.ToggleBloom =									_input_button_undefined;
			View.Input.ToggleCrossFade =								_input_button_undefined;
			View.InputCamera.FpCamera =			_input_button_back | _input_button_dpad_up;
			View.InputCamera.TpCamera =			_input_button_b | _input_button_dpad_left;//_input_button_back | _input_button_dpad_down;
			View.InputCamera.ChaseCamera =		_input_button_back | _input_button_dpad_left;
			View.InputCamera.ScriptedCamera =	_input_button_back | _input_button_right_thumb;
			View.InputCamera.EditorCamera =		_input_button_back | _input_button_dpad_right;
			View.InputCamera.AnchoredCamera =	_input_button_back | _input_button_left_thumb;
			View.InputSliders.Depth.Inc =	_input_button_right_thumb | _input_button_dpad_down;
			View.InputSliders.Depth.Dec =	_input_button_right_thumb | _input_button_dpad_up;
			View.InputSliders.Fov.Inc =		_input_button_right_thumb | _input_button_dpad_left;
			View.InputSliders.Fov.Dec =		_input_button_right_thumb | _input_button_dpad_right;
			View.Look.Input.IncSpeed = _input_button_a | _input_button_dpad_right;
			View.Look.Input.DecSpeed = _input_button_a | _input_button_dpad_left;
			View.Look.Input.IncHorizShift =	_input_button_left_thumb | _input_button_dpad_left;
			View.Look.Input.DecHorizShift =	_input_button_left_thumb | _input_button_dpad_right;
			View.Look.Input.IncVertShift =	_input_button_left_thumb | _input_button_dpad_up;
			View.Look.Input.DecVertShift =	_input_button_left_thumb | _input_button_dpad_down;
			View.Move.Input.IncSpeed = _input_button_a | _input_button_dpad_up;
			View.Move.Input.DecSpeed = _input_button_a | _input_button_dpad_down;
			View.Move.Input.IncHorizShift =	_input_button_black | _input_button_dpad_right;
			View.Move.Input.DecHorizShift =	_input_button_black | _input_button_dpad_left;
			View.Move.Input.IncVertShift =	_input_button_black | _input_button_dpad_up;
			View.Move.Input.DecVertShift =	_input_button_black | _input_button_dpad_down;
			//////////////////////////////////////////////////////////////////////////
			// Input
			Input.SaveGameState = _input_button_y | _input_button_dpad_left;
			Input.LoadGameState = _input_button_y | _input_button_dpad_right;
			Input.AiToggle =											_input_button_undefined;
			Input.GameSpeedToggle = _input_button_left_thumb | _input_button_right_thumb;
			Input.ThirdPersonOverride = _input_button_y | _input_button_dpad_up;
			Input.HighSpeedCapture =									_input_button_undefined;
			Input.TrackMovement = _input_button_black | _input_button_b;
			Input.ChangeActiveConfig =									_input_button_undefined;
			Input.GameSpeed.Inc =	_input_button_b | _input_button_dpad_right;									//_input_button_undefined;
			Input.GameSpeed.Dec =										_input_button_undefined;
			Input.Cinematic.On =	_input_button_black | _input_button_x;
			Input.Cinematic.Off =	_input_button_black | _input_button_y;


			View.InputCamera.LookControlThumb = _input_button_right_thumb;
		}

		void Initialize()
		{
			s_definition& def = Current();
			def.ToDefaults();
			def.ToInputDefaults();
			def.Initialized = true;
		}

		void Dispose()
		{
		}

		void InitializeForNewConfig()
		{
		}

		void DisposeFromOldConfig()
		{
		}

		static int32 CurrentDefinition = 0;
		int32& CurrentIndex() { return CurrentDefinition; }

		s_definition& Current()
		{
			static s_definition _Definitions[Enums::k_config_definition_count] ={
				{0},
				{0},
				{0}
			};
			return _Definitions[CurrentDefinition];
		}
	};
};