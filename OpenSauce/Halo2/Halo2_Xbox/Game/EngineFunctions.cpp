/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/EngineFunctions.hpp"

#include "Game/Globals.hpp"
#include "Game/Director.hpp"
#include "Game/Rasterizer.hpp"
#include "Sound/Sound.hpp"
#include "Interface/Hud.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

	namespace Engine
	{
		namespace AI
		{
			API_FUNC_NAKED void Erase(datum_index ai_index)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(AI_ERASE_ALL);

				API_FUNC_NAKED_START()

					mov		eax, ai_index
#if PLATFORM_ID != PLATFORM_H2_ALPHA
					push	0
#endif
					call	CALL_ADDR

				API_FUNC_NAKED_END(1);
			}
		};

		namespace Cheats
		{
			API_FUNC_NAKED void InfiniteAmmo(bool is_on)
			{
				static uint32 info_ammo_codes[] = { // TODO: is this code platform independent?
					0x008418966,
					0x086110FF3,
					0x000000184,
					0x085110FF3,
					0x000000180,
				};

				API_FUNC_NAKED_START()
					push	edi
					push	ebx
					push	esi

					CR0_BEGIN()
					mov		al, is_on
					test	al, al
					jz		turn_off
/*turn_on:*/
					mov		ecx, 90909090
					mov		edx, ecx
					mov		edi, edx
					mov		ebx, edi
					mov		esi, ebx
					jmp		toggle
turn_off:
					mov		eax, info_ammo_codes
					mov		ecx, [eax]
					mov		ebx, [eax+4]
					mov		edi, [eax+8]
					mov		edx, [eax+12]
					mov		esi, [eax+16]
toggle:
					mov		eax, GET_FUNC_PTR(INF_AMMO1)	// infinite human ammo
					mov		[eax], ecx
					mov		eax, GET_FUNC_PTR(INF_AMMO2)	// infinite alien ammo
					mov		[eax], ebx
					mov		[eax+4], edi
					mov		eax, GET_FUNC_PTR(INF_AMMO3)	// no overheat
					mov		[eax], edx
					mov		[eax+4], esi

					CR0_END()
					pop		esi
					pop		ebx
					pop		edi
				API_FUNC_NAKED_END(1);
			}

			void InfiniteGrenades(bool is_on)
			{
				static unsigned short inf_grenades_code[] = { // TODO: is this code platform independent?
					0x08FE,
					0x9090,
				};

				CR0_BEGIN()

				*CAST_PTR(unsigned short*, GET_FUNC_VPTR(UNIT_THROW_GRENADE_MOVE_TO_HAND)) =
					inf_grenades_code[is_on];

				CR0_END()
			}
		};

		namespace Cinematic
		{
			API_FUNC_NAKED void LightingSetPrimaryLight(angle horiz, angle vert, real_rgb_color* color)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(LIGHTING_SET_PRIMARY_LIGHT);

				API_FUNC_NAKED_START()

					mov		eax, color
					push	[eax+0x8]
					push	[eax+0x4]
					push	[eax]
					push	horiz
					push	vert
					call	CALL_ADDR

				API_FUNC_NAKED_END(3);
			}

			API_FUNC_NAKED void LightingSetSecondaryLight(angle horiz, angle vert, real_rgb_color* color)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(LIGHTING_SET_SECONDARY_LIGHT);

				API_FUNC_NAKED_START()

					mov		eax, color
					push	[eax+0x8]
					push	[eax+0x4]
					push	[eax]
					push	horiz
					push	vert
					call	CALL_ADDR

				API_FUNC_NAKED_END(3);
			}

			API_FUNC_NAKED void FadeIn(short time, real_rgb_color* color)
			{
				// TODO: ALPHA REQUIRES DIFF CODE!!
				static uint32 CALL_ADDR = GET_FUNC_PTR(FADE_IN);

				API_FUNC_NAKED_START()

					mov		eax, color
					push	[eax+0x8]
					push	[eax+0x4]
					movss	xmm0, [eax]
					movsx	ax, time
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void FadeOut(short time, real_rgb_color* color)
			{
				// TODO: ALPHA REQUIRES DIFF CODE!!
				static uint32 CALL_ADDR = GET_FUNC_PTR(FADE_OUT);

				API_FUNC_NAKED_START()

					mov		eax, color
					push	[eax+0x8]
					push	[eax+0x4]
					movss	xmm0, [eax]
					movsx	ax, time
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void Start()
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(CINEMATIC_START);

				API_FUNC_NAKED_START_()

					call	CALL_ADDR

				API_FUNC_NAKED_END_();
			}

			API_FUNC_NAKED void Stop()
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(CINEMATIC_STOP);

				API_FUNC_NAKED_START_()

					call	CALL_ADDR

				API_FUNC_NAKED_END_();
			}

			void SkipStartInternal()
			{
				GameState::_CinematicGlobals()->SkipStart = true;
			}

			void SkipStopInternal()
			{
				GameState::_CinematicGlobals()->SkipStart = false;
			}

			void ShowLetterbox(bool show)
			{
				GameState::_CinematicGlobals()->ShowLetterbox = show;
			}

			void ShowLetterboxImmediate(bool show)
			{
				GameState::_CinematicGlobals()->Time =
					(GameState::_CinematicGlobals()->ShowLetterbox = show)
					?
					XboxLib::Math::RealConstants.One : XboxLib::Math::RealConstants.Zero;
			}

			API_FUNC_NAKED void SetFilterBitmap(datum_index bitmap_index)
			{
				// Alpha has more args and resembles the function in halo1
				static uint32 CALL_ADDR = GET_FUNC_PTR(CINEMATIC_SCREEN_EFFECT_SET_FILTER_BITMAP);

				API_FUNC_NAKED_START()

					mov		edx, bitmap_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(1);
			}

			API_FUNC_NAKED void RenderLightsEnableCinematicShadow(real arg_value)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(RENDER_LIGHTS_ENABLE_CINEMATIC_SHADOW);

				API_FUNC_NAKED_START()
					push	esi // render light index?
					push	edi // ?
					push	ebx // some switch

					xor		bl, bl	// do this because there are other values it CAN use
					push	arg_value
					call	CALL_ADDR

					pop		ebx
					pop		edi
					pop		esi
				API_FUNC_NAKED_END(1);
			}

			void ScreenEffectStart(bool start_effect)
			{
				GameState::s_rasterizer_cinematic_globals* rcg = 
					GameState::_RasterizerCinematicGlobals();

				bool* i_dont_know = CAST_PTR(bool*, rcg)+0xAD;
				if(start_effect || !*i_dont_know)
				{
					// memset wasn't inlining and thus was giving a linker error
					XboxLib::_memzero(rcg, 0xAC);
					*i_dont_know = true;
				}
				*(i_dont_know-1) = true; // 0xAC = true
			}

			void ScreenEffectStop()
			{
				// memset wasn't inlining and thus was giving a linker error
				XboxLib::_memzero(GameState::_RasterizerCinematicGlobals(), sizeof(GameState::s_rasterizer_cinematic_globals));
				if( *CAST_PTR(long*, GET_DATA_VPTR(ScreenEffectStop_BULLSHIT1)) )
				{
					bool* bs2 = CAST_PTR(bool*, GET_DATA_VPTR(ScreenEffectStop_BULLSHIT2));
					if(!*bs2)
						*bs2 = true;
				}
			}

			void LightmapShadow(bool enable_shadow)
			{
				*CAST_PTR(bool*, GET_DATA_VPTR(CINEMATIC_LIGHTMAP_SHADOW_ENABLE)) = enable_shadow;
			}

			void AmbienceDetails(bool enable_details)
			{
				*GameState::_GameSoundGlobals()->GetAmbienceDetailsEnabled() = enable_details;
			}
		};

		namespace Game
		{
			API_FUNC_NAKED void GameRate(real one, real two, real three)
			{
				// TODO: ALPHA REQUIRES DIFF CODE!!
				static uint32 CALL_ADDR = GET_FUNC_PTR(GAME_RATE);

				API_FUNC_NAKED_START()

					movss	xmm0, two
					movss	xmm1, one
					push	three
					call	CALL_ADDR

				API_FUNC_NAKED_END(3);
			}

			API_FUNC_NAKED void Revert()
			{
#if PLATFORM_ID == PLATFORM_H2_ALPHA
				__asm	ret
#else
				API_FUNC_NAKED_START_()

					mov		al, 1
					mov		byte ptr ds:[GET_DATA_PTR(GAME_REVERT1)], al
					mov		byte ptr ds:[GET_DATA_PTR(GAME_REVERT2)], al
					mov		byte ptr ds:[GET_DATA_PTR(GAME_REVERT3)], al

				API_FUNC_NAKED_END_();
#endif
			}

			API_FUNC_NAKED void Save()
			{
				API_FUNC_NAKED_START_()

					mov		eax, 1
					mov		ecx, GET_DATA_PTR(GAME_SAVE)
					cmp		[ecx], eax
					jz		is_saving
					mov		[ecx], eax
					call	Yelo::GameState::_GameTimeGlobals
					mov		ecx, eax
					xor		eax, eax
					mov		dword ptr ds:[GET_DATA_PTR(GAME_SAVE) + 4], eax
					mov		ecx, [ecx+0x8]
					mov		dword ptr ds:[GET_DATA_PTR(GAME_SAVE) + 8], ecx
					mov		word ptr ds:[GET_DATA_PTR(GAME_SAVE) + 12], ax
is_saving:
				API_FUNC_NAKED_END_();
			}

			API_FUNC_NAKED void Won()
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(GAME_WON);

				API_FUNC_NAKED_START_()

#if PLATFORM_ID == PLATFORM_H2_ALPHA
					mov		dword ptr ds:[GET_FUNC_PTR(GAME_WON)], 1
#else
					call	CALL_ADDR
#endif

				API_FUNC_NAKED_END_();
			}

			API_FUNC_NAKED void PlaySound(datum_index sound_index, real volume)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(PLAY_SOUND);

				API_FUNC_NAKED_START()

					movss	xmm0, volume
					push	sound_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}
		};

		namespace Interface
		{
			API_FUNC_NAKED DWORD XInputGetState(HANDLE device, void* state)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(XINPUT_GET_STATE);

				API_FUNC_NAKED_START()

					push	state
					push	device
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void HudCinematicFade(real init_opcaity, real final_opcaity)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(HUD_CINEMATIC_FADE);

				API_FUNC_NAKED_START()

					mov		ecx, init_opcaity
					push	ecx
					push	final_opcaity
					call	CALL_ADDR // things that make you go... :wtc: (look at 0x13B341)

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void HudMessagePrint(wcstring string, int32 player_index)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(HUD_MESSAGE_PRINT);

				API_FUNC_NAKED_START()

					push	string
					mov		eax, player_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void HudAddItemMessage(wcstring string, int32 player_index)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(HUD_ADD_ITEM_MESSAGE);

				API_FUNC_NAKED_START()

					mov		ecx, string
					mov		eax, player_index
					push	XboxLib::Math::RealConstants.Zero // this is a fucking hack. I'm not sure how the game will react to this tbh, refer to the game's code for generating the argument
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void HudTimerSet(int16 minutes, int16 seconds)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(SCRIPTED_HUD_SET_TIMER_TIME);

				API_FUNC_NAKED_START()

					push	seconds
					movsx	ecx, minutes
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			void HudTimerSetWarningTime(int16 minutes, int16 seconds)
			{
				minutes = (minutes * 60) + seconds;
				minutes *= GameState::_GameTimeGlobals()->TickRate;
				*GameState::_HudMessaging()->GetTimerWarningTicks() = minutes;
			}

			void HudTimerSetPosition(int16 x, int16 y, _enum hud_corner)
			{
				GameState::s_hud_messaging* hm = GameState::_HudMessaging();
				hm->GetTimerPosition()->x = x;
				hm->GetTimerPosition()->y = y;
				*hm->GetTimerHudPosition() = hud_corner;
			}

			API_FUNC_NAKED void HudTimerPause(bool pause_timer)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(SCRIPTED_HUD_PAUSE_TIMER);

				API_FUNC_NAKED_START()

					mov		dl, pause_timer
					call	CALL_ADDR

				API_FUNC_NAKED_END(1);
			}


			API_FUNC_NAKED void HudSetStateText(wcstring state_text, int32 player_index, int32 up_time, datum_index optional_sound)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(HUD_SET_STATE_TEXT);

				API_FUNC_NAKED_START()

					push	optional_sound
					mov		eax, player_index
					push	up_time
					mov		ecx, state_text
					call	CALL_ADDR

				API_FUNC_NAKED_END(4);
			}
		};

		namespace Players
		{
			API_FUNC_NAKED datum_index LocalGetPlayerIndex(int32 local_player_index)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(LOCAL_PLAYER_GET_PLAYER_INDEX);

				API_FUNC_NAKED_START()

					mov		ecx, local_player_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(1);
			}

			API_FUNC_NAKED datum_index PlayerFromUnitIndex(datum_index unit_index)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX);

				API_FUNC_NAKED_START()

					mov		eax, unit_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(1);
			}
		};

		namespace Objects
		{
			API_FUNC_NAKED bool IsOrContainsPlayer(datum_index object_index)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(OBJECT_IS_OR_CONTAINS_PLAYER);

				API_FUNC_NAKED_START()

					mov		ecx, object_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(1);
			}

			API_FUNC_NAKED void Hide(datum_index object_index, bool hide_it)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(OBJECT_HIDE);

				API_FUNC_NAKED_START()

					movzx	eax, hide_it
					push	eax
					mov		eax, object_index
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void SetPosition(datum_index object_index, real_vector3d* pos, real_vector3d* forward, real_euler_angles3d* up)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(OBJECT_SET_POSITION);

				API_FUNC_NAKED_START()

					push	0
					push	0
					push	object_index
					mov		edx, up
					mov		ecx, forward
					mov		eax, pos
					call	CALL_ADDR

				API_FUNC_NAKED_END(4);
			}

			API_FUNC_NAKED void UnitLowerWeapon(datum_index unit_index, int32 duration_seconds)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(UNIT_SCRIPTING_LOWER_WEAPON);

				API_FUNC_NAKED_START()

					mov		eax, unit_index
					push	duration_seconds
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}

			API_FUNC_NAKED void UnitRaiseWeapon(datum_index unit_index, int32 duration_seconds)
			{
				static uint32 CALL_ADDR = GET_FUNC_PTR(UNIT_SCRIPTING_RAISE_WEAPON);

				API_FUNC_NAKED_START()

					mov		eax, unit_index
					push	duration_seconds
					call	CALL_ADDR

				API_FUNC_NAKED_END(2);
			}
		};
	};
};