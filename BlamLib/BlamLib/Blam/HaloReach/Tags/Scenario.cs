/*
    BlamLib: .NET SDK for the Blam Engine

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
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using H3 = BlamLib.Blam.Halo3.Tags;

namespace BlamLib.Blam.HaloReach.Tags
{
	#region hs_scripts_block
	[TI.Definition(3, 24)]
	public class hs_scripts_block : Scripting.hs_scripts_block
	{
		public TI.Block<Blam.Halo3.Tags.hs_scripts_block.hs_scripts_parameters_block> Parameters;

		#region Ctor
		public hs_scripts_block() : base(5)
		{
			Add(Name = new TI.StringId());
			Add(ScriptType = new TI.Enum());
			Add(ReturnType = new TI.Enum());
			Add(RootExpressionIndex = new TI.LongInteger());
			Add(Parameters = new TI.Block<Blam.Halo3.Tags.hs_scripts_block.hs_scripts_parameters_block>(this, 0));
		}
		#endregion
	}
	#endregion


	#region hs_unit_seat_block
	[TI.Definition(2, 12)]
	public class hs_unit_seat_block : TI.Definition
	{
		public hs_unit_seat_block()
		{
			Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger());
		}
	}
	#endregion

	#region scenario
	[TI.TagGroup((int)TagGroups.Enumerated.scnr, 5, 2156)]
	public class scenario_group : TI.Definition
	{
		void version_construct_add_unnamed_array()
		{
			#region unknown
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			#endregion
		}
		public scenario_group()
		{
			// TOOD: finish up from here

			// 0x4A0
			Add(/*CustomObjectNames =*/ new TI.TagReference(this, TagGroups.unic));
			Add(/*ChapterTitleText =*/ new TI.TagReference(this, TagGroups.unic));
			Add(/*ScenarioResources =*/ new TI.Block<H3.scenario_resources_block>(this, 1));
			Add(/*HsUnitSeats =*/ new TI.Block<hs_unit_seat_block>(this, 65536));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x4]
				// short
				// short
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x30] this is physics related ata
				// dword
				// tag block [0x3C or 0x40] (I think this may get 16-byte aligned)
					// unknown 0x16 bytes
					// real[4] (the first three are definitely xyz)
					// dword?
					// dword
					// dword
					// dword?
					// tag block [0x1] (mopp?)
					// dword? (if this is just aligned to 16-byte pages, ignore this field)
				// dword
				// tag_reference?
				// real?
				// real
				// real
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(/*HsScriptDatums =*/ new TI.Block<H3.syntax_datum_block>(this, 0));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Orders
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Triggers
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] BackgroundSoundPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] SoundEnvironmentPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] WeatherPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x54] ScenarioClusterData
				// this is scenario_cluster_data_block but with an extra dword of data right after the tag reference
			version_construct_add_unnamed_array();
			Add(TI.UnknownPad.BlockHalo3); // SpawnData?
			// 0x604
			Add(/*SoundEffectCollection = */new TI.TagReference(this, TagGroups.sfx_));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xD8] Crates
			Add(/*CratesPalette = */new TI.Block<H3.scenario_crate_palette_block>(this, 256));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10] Flocks palette
				// tag reference
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Flocks
			Add(/*Subtitles = */new TI.TagReference(this, TagGroups.unic));

			// Creatures & CreaturesPalette should be in one of these blocks
			Add(TI.UnknownPad.BlockHalo3);
			Add(TI.UnknownPad.BlockHalo3);
			Add(TI.UnknownPad.BlockHalo3);
			Add(/*EditorFolders = */new TI.Block<H3.g_scenario_editor_folder_block>(this, 32767));
			Add(/*TerritoryLocationNames = */new TI.TagReference(this, TagGroups.unic));
			Add(new TI.Pad(8));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] MissionDialogue
			Add(/*Objectives = */new TI.TagReference(this, TagGroups.unic));
			Add(/*? = */new TI.TagReference(this, TagGroups.sirp));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(/*Camere fx settings = */new TI.TagReference(this, TagGroups.cfxs));
			Add(/*? = */new TI.TagReference(this, TagGroups.sefc));
			Add(/*? = */new TI.TagReference(this, TagGroups.ssao));
			Add(/*sky parameters= */new TI.TagReference(this, TagGroups.skya));
			Add(/*atmosphere globals = */new TI.TagReference(this, TagGroups.atgf));
			Add(/*GlobalLighting = */new TI.TagReference(this, TagGroups.chmt));
			Add(/*lightmap = */new TI.TagReference(this, TagGroups.sLdT));
			Add(/*performance throttles = */new TI.TagReference(this, TagGroups.perf));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.ReferenceHalo3);
		}
	};
	#endregion
}