/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// This must come first, many systems use the Report filing system
{Settings::Initialize,								Settings::Dispose},
{FileIO::Initialize,								FileIO::Dispose},
// ...like debugging for example
#ifdef API_DEBUG
	{DebugFile::Initialize,								DebugFile::Dispose},
#endif

{GameState::Initialize,								GameState::Dispose},
{Cache::Initialize,									Cache::Dispose},
{DataFiles::Initialize,								DataFiles::Dispose},
{TagGroups::Initialize,								TagGroups::Dispose,					NULL,	TagGroups::InitializeForNewMap},

{Scripting::Initialize,								Scripting::Dispose},	// No active code in Update game hook currently
#if PLATFORM_IS_USER
	{DX9::Initialize,								DX9::Dispose},
	{DX9::c_gbuffer_system::Initialize,				DX9::c_gbuffer_system::Dispose,		NULL, NULL, NULL, DX9::c_gbuffer_system::Update},
	{Rasterizer::Initialize,						Rasterizer::Dispose},
	{Rasterizer::ShaderExtension::Initialize,		Rasterizer::ShaderExtension::Dispose},
	{Postprocessing::Initialize,					Postprocessing::Dispose,			NULL, Postprocessing::InitializeForNewMap,	Postprocessing::DisposeFromOldMap,	Postprocessing::Update},
	{Camera::Initialize,							Camera::Dispose},
#endif
{Networking::VersionCheck::Initialize,				Networking::VersionCheck::Dispose,	NULL, Networking::VersionCheck::InitializeForNewMap, NULL, Networking::VersionCheck::Update},
//{Console::Initialize,								Console::Dispose},		// No active code currently

{GameEngine::Initialize,							GameEngine::Dispose},
{Networking::Initialize,							Networking::Dispose},
{Players::Initialize,								Players::Dispose},		// No active code in Update game hook currently

//{Effects::Initialize,								Effects::Dispose},		// No active code currently
//{AI::Initialize,									AI::Dispose},			// No active code currently
{Objects::Initialize,								Objects::Dispose,					NULL,		Objects::InitializeForNewMap},		// No active code in Update game hook currently

#if PLATFORM_IS_USER
	{GameUI::Initialize,							GameUI::Dispose},
	//{Input::Initialize,								Input::Dispose},	// No active code currently
	{Keystone::Initialize,							Keystone::Dispose},
#endif

{Memory::c_function_interface_system::Initialize,	Memory::c_function_interface_system::Dispose},