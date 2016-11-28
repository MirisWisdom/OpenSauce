/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/GameSystemComponent.IterationRoundBegin.inl"

#if		BOOST_PP_ITERATION() == 1
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Rasterizer::DeviceHooks::Initialize
#		define __GS_COMPONENT_DISPOSE		Rasterizer::DeviceHooks::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 2
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE			DX9::Initialize
#		define __GS_COMPONENT_DISPOSE				DX9::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#		define __GS_COMPONENT_DX9_INITIALIZE		DX9::Initialize3D
#		define __GS_COMPONENT_DX9_ON_LOST_DEVICE	DX9::OnLostDevice
#		define __GS_COMPONENT_DX9_ON_RESET_DEVICE	DX9::OnResetDevice
#		define __GS_COMPONENT_DX9_RENDER			DX9::Render
#		define __GS_COMPONENT_DX9_RELEASE			DX9::Release
#	endif
#elif	BOOST_PP_ITERATION() == 3
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE			DX9::c_gbuffer_system::Initialize
#		define __GS_COMPONENT_DISPOSE				DX9::c_gbuffer_system::Dispose
#		define __GS_COMPONENT_UPDATE				DX9::c_gbuffer_system::Update
#	elif	__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#		define __GS_COMPONENT_DX9_INITIALIZE		DX9::c_gbuffer_system::Initialize3D
#		define __GS_COMPONENT_DX9_ON_LOST_DEVICE	DX9::c_gbuffer_system::OnLostDevice
#		define __GS_COMPONENT_DX9_ON_RESET_DEVICE	DX9::c_gbuffer_system::OnResetDevice
#		define __GS_COMPONENT_DX9_RENDER			DX9::c_gbuffer_system::Render
#		define __GS_COMPONENT_DX9_RELEASE			DX9::c_gbuffer_system::Release
#	endif
#elif	BOOST_PP_ITERATION() == 4
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Rasterizer::Initialize
#		define __GS_COMPONENT_DISPOSE		Rasterizer::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 5
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Rasterizer::ShaderDraw::Initialize
#		define __GS_COMPONENT_DISPOSE		Rasterizer::ShaderDraw::Dispose
#	else
#		define __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#elif	BOOST_PP_ITERATION() == 6
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE			Rasterizer::ShaderExtension::Initialize
#		define __GS_COMPONENT_DISPOSE				Rasterizer::ShaderExtension::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#		define __GS_COMPONENT_DX9_INITIALIZE		Rasterizer::ShaderExtension::Initialize3D
#		define __GS_COMPONENT_DX9_ON_LOST_DEVICE	Rasterizer::ShaderExtension::OnLostDevice
#		define __GS_COMPONENT_DX9_ON_RESET_DEVICE	Rasterizer::ShaderExtension::OnResetDevice
#		define __GS_COMPONENT_DX9_RENDER			Rasterizer::ShaderExtension::Render
#		define __GS_COMPONENT_DX9_RELEASE			Rasterizer::ShaderExtension::Release
#	endif
#elif	BOOST_PP_ITERATION() == 7
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE				Rasterizer::PostProcessing::Initialize
#		define __GS_COMPONENT_DISPOSE					Rasterizer::PostProcessing::Dispose
#		define __GS_COMPONENT_UPDATE					Rasterizer::PostProcessing::Update
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP	Rasterizer::PostProcessing::InitializeForNewMap
#		define __GS_COMPONENT_DISPOSE_FROM_OLD_MAP		Rasterizer::PostProcessing::DisposeFromOldMap
#	elif	__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#		define __GS_COMPONENT_DX9_INITIALIZE			Rasterizer::PostProcessing::Initialize3D
#		define __GS_COMPONENT_DX9_ON_LOST_DEVICE		Rasterizer::PostProcessing::OnLostDevice
#		define __GS_COMPONENT_DX9_ON_RESET_DEVICE		Rasterizer::PostProcessing::OnResetDevice
#		define __GS_COMPONENT_DX9_RENDER				Rasterizer::PostProcessing::Render
#		define __GS_COMPONENT_DX9_RELEASE				Rasterizer::PostProcessing::Release
#	endif
#elif	BOOST_PP_ITERATION() == 8
#	if		__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#		define __GS_COMPONENT_DX9_INITIALIZE		Hud::Initialize3D
#		define __GS_COMPONENT_DX9_ON_LOST_DEVICE	Hud::OnLostDevice
#		define __GS_COMPONENT_DX9_ON_RESET_DEVICE	Hud::OnResetDevice
#		define __GS_COMPONENT_DX9_RENDER			Hud::Render
#		define __GS_COMPONENT_DX9_RELEASE			Hud::Release
#	endif
#elif	BOOST_PP_ITERATION() == 9
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE					Render::Lightmaps::Initialize
#		define __GS_COMPONENT_DISPOSE						Render::Lightmaps::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 10
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE					Render::Sky::Initialize
#		define __GS_COMPONENT_DISPOSE						Render::Sky::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP		Render::Sky::InitializeForNewMap
#		define __GS_COMPONENT_DISPOSE_FROM_OLD_MAP			Render::Sky::DisposeFromOldMap
#	endif
#elif	BOOST_PP_ITERATION() == 11
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE					StructureBSP::Initialize
#		define __GS_COMPONENT_DISPOSE						StructureBSP::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP		StructureBSP::InitializeForNewMap
#		define __GS_COMPONENT_DISPOSE_FROM_OLD_MAP			StructureBSP::DisposeFromOldMap
#	elif	__GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE	StructureBSP::InitializeForNewGameState
#		define __GS_COMPONENT_HANDLE_GAME_STATE_LIFECYCLE	StructureBSP::HandleGameStateLifeCycle
#	elif	__GS_COMPONENT == __GS_COMPONENT_BSP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_BSP		StructureBSP::InitializeForNewBSP
#		define __GS_COMPONENT_DISPOSE_FROM_OLD_BSP			StructureBSP::DisposeFromOldBSP
#	endif
#endif

#include "Common/GameSystemComponent.IterationRoundEnd.inl"