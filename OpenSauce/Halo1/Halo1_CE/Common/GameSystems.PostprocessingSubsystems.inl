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

#define DX_POSTPROCESSING_SUBSYSTEM(name)										\
	name::Initialize, name::OnLostDevice, name::OnResetDevice, name::Render,	\
	name::AllocateResources, name::ReleaseResources,							\
	name::LoadSettings, name::SaveSettings, name::SetDefaultSettings,			\
	name::Load, name::Unload,													\
	name::InitializeForNewMap, name::DisposeFromOldMap, name::Update,			\
	name::DoPostProcesses


DX_POSTPROCESSING_SUBSYSTEM(Postprocessing::Subsystem::MotionBlur::c_motionblur_subsystem),
DX_POSTPROCESSING_SUBSYSTEM(Postprocessing::Subsystem::Internal::c_internal_subsystem),
DX_POSTPROCESSING_SUBSYSTEM(Postprocessing::Subsystem::External::c_external_subsystem),
DX_POSTPROCESSING_SUBSYSTEM(Postprocessing::Subsystem::Bloom::c_bloom_subsystem),


#undef DX_POSTPROCESSING_SUBSYSTEM