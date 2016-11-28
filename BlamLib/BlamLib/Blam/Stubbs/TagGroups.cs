/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using BlamLib.TagInterface;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Stubbs
{
	public class TagGroups
	{
		#region Tag Groups
		/// <summary>image_effect</summary>
		public static TagGroup imef = new TagGroup("imef", "image_effect");

		/// <summary>I DON'T FUCKING KNOW</summary>
		public static TagGroup terr = new TagGroup("terr", "UNKNOWN_terr");

		/// <summary>vegetation</summary>
		public static TagGroup vege = new TagGroup("vege", "vegetation");
		#endregion

		#region Tags Group Collection
		/// <summary>
		/// All tag groups in Stubbs The Zombie
		/// </summary>
		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			Groups = new TagGroupCollection(false,
				TagGroup.Null,
				Halo1.TagGroups.actr,
				Halo1.TagGroups.actv,
				Halo1.TagGroups.ant_,
				Halo1.TagGroups.bipd,
				Halo1.TagGroups.bitm,
				Halo1.TagGroups.trak,
				Halo1.TagGroups.colo,
				Halo1.TagGroups.cdmg,
				Halo1.TagGroups.cont,
				Halo1.TagGroups.jpt_,
				Halo1.TagGroups.deca,
				Halo1.TagGroups.dobc,
				Halo1.TagGroups.devi,
				Halo1.TagGroups.ctrl,
				Halo1.TagGroups.lifi,
				Halo1.TagGroups.mach,
				Halo1.TagGroups.udlg,
				Halo1.TagGroups.effe,
				Halo1.TagGroups.eqip,
				Halo1.TagGroups.flag,
				Halo1.TagGroups.fog_,
				Halo1.TagGroups.font,
				Halo1.TagGroups.garb,
				Halo1.TagGroups.matg,
				Halo1.TagGroups.glw_,
				Halo1.TagGroups.grhi,
				Halo1.TagGroups.hudg,
				Halo1.TagGroups.hmt_,
				Halo1.TagGroups.hud_,
				Halo1.TagGroups.item,
				Halo1.TagGroups.itmc,
				Halo1.TagGroups.lens,
				Halo1.TagGroups.ligh,
				Halo1.TagGroups.mgs2,
				Halo1.TagGroups.elec,
				Halo1.TagGroups.foot,
				Halo1.TagGroups.metr,
				Halo1.TagGroups.mode,
				Halo1.TagGroups.antr,
				Halo1.TagGroups.coll,
				Halo1.TagGroups.mply,
				Halo1.TagGroups.obje,
				Halo1.TagGroups.part,
				Halo1.TagGroups.pctl,
				Halo1.TagGroups.phys,
				Halo1.TagGroups.plac,
				Halo1.TagGroups.pphy,
				Halo1.TagGroups.ngpr,
				Halo1.TagGroups.proj,
				Halo1.TagGroups.scnr,
				Halo1.TagGroups.sbsp,
				Halo1.TagGroups.scen,
				Halo1.TagGroups.shdr,
				Halo1.TagGroups.senv,
				Halo1.TagGroups.soso,
				Halo1.TagGroups.schi,
				Halo1.TagGroups.scex,
				Halo1.TagGroups.sotr,
				Halo1.TagGroups.sgla,
				Halo1.TagGroups.smet,
				Halo1.TagGroups.spla,
				Halo1.TagGroups.swat,
				Halo1.TagGroups.sky_,
				Halo1.TagGroups.snd_,
				Halo1.TagGroups.snde,
				Halo1.TagGroups.lsnd,
				Halo1.TagGroups.ssce,
				Halo1.TagGroups.boom,
				Halo1.TagGroups.str_,
				Halo1.TagGroups.Soul,
				Halo1.TagGroups.DeLa,
				Halo1.TagGroups.ustr,
				Halo1.TagGroups.unit,
				Halo1.TagGroups.unhi,
				Halo1.TagGroups.vehi,
				Halo1.TagGroups.vcky,
				Halo1.TagGroups.weap,
				Halo1.TagGroups.wphi,
				Halo1.TagGroups.rain,
				Halo1.TagGroups.wind,
				Halo1.TagGroups.tag_,

				imef,
				terr,
				vege
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>any tag</summary>
			null_,
			/// <summary>actor</summary>
			actr,
			/// <summary>actor_variant</summary>
			actv,
			/// <summary>antenna</summary>
			ant_,
			/// <summary>biped</summary>
			bipd,
			/// <summary>bitmap</summary>
			bitm,
			/// <summary>camera_track</summary>
			trak,
			/// <summary>color_table</summary>
			colo,
			/// <summary>continuous_damage_effect</summary>
			cdmg,
			/// <summary>contrail</summary>
			cont,
			/// <summary>damage_effect</summary>
			jpt_,
			/// <summary>decal</summary>
			deca,
			/// <summary>detail_object_collection</summary>
			dobc,
			/// <summary>device</summary>
			devi,
			/// <summary>device_control</summary>
			ctrl,
			/// <summary>device_light_fixture</summary>
			lifi,
			/// <summary>device_machine</summary>
			mach,
			/// <summary>dialogue</summary>
			udlg,
			/// <summary>effect</summary>
			effe,
			/// <summary>equipment</summary>
			eqip,
			/// <summary>flag</summary>
			flag,
			/// <summary>fog</summary>
			fog_,
			/// <summary>font</summary>
			font,
			/// <summary>garbage</summary>
			garb,
			/// <summary>gbxmodel</summary>
			mod2,
			/// <summary>globals</summary>
			matg,
			/// <summary>glow</summary>
			glw_,
			/// <summary>grenade_hud_interface</summary>
			grhi,
			/// <summary>hud_globals</summary>
			hudg,
			/// <summary>hud_message_text</summary>
			hmt_,
			/// <summary>hud_number</summary>
			hud_,
			/// <summary>input_device_defaults</summary>
			devc,
			/// <summary>item</summary>
			item,
			/// <summary>item_collection</summary>
			itmc,
			/// <summary>lens_flare</summary>
			lens,
			/// <summary>light</summary>
			ligh,
			/// <summary>light_volume</summary>
			mgs2,
			/// <summary>lightning</summary>
			elec,
			/// <summary>material_effects</summary>
			foot,
			/// <summary>meter</summary>
			metr,
			/// <summary>model</summary>
			mode,
			/// <summary>model_animations</summary>
			antr,
			/// <summary>model_collision_geometry</summary>
			coll,
			/// <summary>multiplayer_scenario_description</summary>
			mply,
			/// <summary>object</summary>
			obje,
			/// <summary>particle</summary>
			part,
			/// <summary>particle_system</summary>
			pctl,
			/// <summary>physics</summary>
			phys,
			/// <summary>placeholder</summary>
			plac,
			/// <summary>point_physics</summary>
			pphy,
			/// <summary>preferences_network_game</summary>
			ngpr,
			/// <summary>projectile</summary>
			proj,
			/// <summary>scenario</summary>
			scnr,
			/// <summary>scenario_structure_bsp</summary>
			sbsp,
			/// <summary>scenery</summary>
			scen,
			/// <summary>shader</summary>
			shdr,
			/// <summary>shader_environment</summary>
			senv,
			/// <summary>shader_model</summary>
			soso,
			/// <summary>shader_transparent_chicago</summary>
			schi,
			/// <summary>shader_transparent_chicago_extended</summary>
			scex,
			/// <summary>shader_transparent_generic</summary>
			sotr,
			/// <summary>shader_transparent_glass</summary>
			sgla,
			/// <summary>shader_transparent_meter</summary>
			smet,
			/// <summary>shader_transparent_plasma</summary>
			spla,
			/// <summary>shader_transparent_water</summary>
			swat,
			/// <summary>sky</summary>
			sky_,
			/// <summary>sound</summary>
			snd_,
			/// <summary>sound_environment</summary>
			snde,
			/// <summary>sound_looping</summary>
			lsnd,
			/// <summary>sound_scenery</summary>
			ssce,
			/// <summary>spheroid</summary>
			boom,
			/// <summary>string_list</summary>
			str_,
			/// <summary>tag_collection</summary>
			tagc,
			/// <summary>ui_widget_collection</summary>
			Soul,
			/// <summary>ui_widget_definition</summary>
			DeLa,
			/// <summary>unicode_string_list</summary>
			ustr,
			/// <summary>unit</summary>
			unit,
			/// <summary>unit_hud_interface</summary>
			unhi,
			/// <summary>vehicle</summary>
			vehi,
			/// <summary>virtual_keyboard</summary>
			vcky,
			/// <summary>weapon</summary>
			weap,
			/// <summary>weapon_hud_interface</summary>
			wphi,
			/// <summary>weather_particle_system</summary>
			rain,
			/// <summary>wind</summary>
			wind,
			/// <summary>tag_database</summary>
			tag_,

			/// <summary>image_effect</summary>
			imef,
			/// <summary>I DON'T FUCKING KNOW</summary>
			terr,
			/// <summary>vegetation</summary>
			vege,
		};
		#endregion

		#region Static Ctor
		static TagGroups()
		{
			GroupsInitialize();
			imef.Definition = new Tags.image_effect_group().State;
			terr.Definition = new Tags.terr_group().State;
			vege.Definition = new Tags.vegetation_group().State;

			for (int x = Halo1.TagGroups.Groups.Count; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Stubbs, x, false);
		}
		#endregion
	};
};

namespace BlamLib.Blam.Stubbs.Tags
{
	#region image_effect
	[TI.TagGroup((int)TagGroups.Enumerated.imef, 1, 428)]
	public class image_effect_group : TI.Definition
	{
		#region effects_block
		[TI.Definition(1, 164)]
		public class effects_block : TI.Definition
		{
			#region Fields
			public TI.Struct<Halo1.Tags.shader_animation_struct> Animation;
			#endregion

			#region Ctor
			public effects_block() : base(14)
			{
				//////////////////////////////////////////////////////////////////////////
				// array of 8 dwords. padding?
				Add(new TI.Skip(32));
				//////////////////////////////////////////////////////////////////////////
				Add(new TI.ShortInteger()); // unknown 16 bits 20
				Add(new TI.ShortInteger()); // unknown 16 bits 22
				Add(new TI.ShortInteger()); // unknown 16 bits 24
				Add(new TI.ShortInteger()); // unknown 16 bits 26
				Add(new TI.LongInteger()); // unknown 32 bits 28
				Add(new TI.LongInteger()); // unknown 32 bits 2C
				Add(new TI.LongInteger()); // unknown 32 bits 30
				Add(new TI.LongInteger()); // unknown 32 bits 34
				Add(new TI.LongInteger()); // unknown 32 bits 38
				Add(new TI.TagReference(this, Halo1.TagGroups.bitm));
				Add(new TI.LongInteger()); // unknown 32 bits 4C
				Add(new TI.Pad(28));
				Add(Animation = new TI.Struct<Halo1.Tags.shader_animation_struct>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public image_effect_group() : base(48)
		{
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			//////////////////////////////////////////////////////////////////////////
			// array of 4 dwords. padding?
			Add(new TI.Skip(16));
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.ShortInteger()); // unknown 16 bits 1C
			Add(new TI.ShortInteger()); // unknown 16 bits 1E
			Add(new TI.LongInteger()); // unknown 32 bits 20
			Add(new TI.LongInteger()); // unknown 32 bits 24
			Add(new TI.LongInteger()); // unknown 32 bits 28
			Add(new TI.LongInteger()); // unknown 32 bits 2C
			Add(new TI.LongInteger()); // unknown 32 bits 30
			Add(new TI.LongInteger()); // unknown 32 bits 34
			Add(new TI.LongInteger()); // unknown 32 bits 38
			Add(new TI.ShortInteger()); // unknown 16 bits 3C
			Add(new TI.ShortInteger()); // unknown 16 bits 3E
			Add(new TI.LongInteger()); // unknown 32 bits 40
			//////////////////////////////////////////////////////////////////////////
			// array of 3 dwords. padding?
			Add(new TI.Skip(12));
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.LongInteger()); // unknown 32 bits 50
			Add(new TI.LongInteger()); // unknown 32 bits 54
			Add(new TI.LongInteger()); // unknown 32 bits 58
			Add(new TI.LongInteger()); // unknown 32 bits 5C
			Add(new TI.LongInteger()); // unknown 32 bits 60
			//////////////////////////////////////////////////////////////////////////
			// array of 8 dwords. padding?
			Add(new TI.Skip(32));
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.ShortInteger()); // unknown 16 bits 84
			Add(new TI.ShortInteger()); // unknown 16 bits 86
			Add(new TI.LongInteger()); // unknown 32 bits 88
			Add(new TI.LongInteger()); // unknown 32 bits 8C
			Add(new TI.LongInteger()); // unknown 32 bits 90
			Add(new TI.LongInteger()); // unknown 32 bits 94
			Add(new TI.LongInteger()); // unknown 32 bits 9C
			Add(new TI.LongInteger()); // unknown 32 bits A0
			Add(new TI.LongInteger()); // unknown 32 bits A4
			//////////////////////////////////////////////////////////////////////////
			// array of 7 dwords. padding?
			Add(new TI.Skip(28));
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.ShortInteger()); // unknown 16 bits C4
			Add(new TI.ShortInteger()); // unknown 16 bits C6
			Add(new TI.ShortInteger()); // unknown 16 bits C8
			Add(new TI.ShortInteger()); // unknown 16 bits CA
			Add(new TI.LongInteger()); // unknown 32 bits CC
			Add(new TI.LongInteger()); // unknown 32 bits D0
			//////////////////////////////////////////////////////////////////////////
			// array of 8 dwords. padding?
			Add(new TI.Skip(32));
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.ShortInteger()); // unknown 16 bits F4
			Add(new TI.ShortInteger()); // unknown 16 bits F6
			Add(new TI.ShortInteger()); // unknown 16 bits F8
			Add(new TI.ShortInteger()); // unknown 16 bits FA
			Add(new TI.LongInteger()); // unknown 32 bits FC
			//////////////////////////////////////////////////////////////////////////
			// array of 8 dwords. padding?
			Add(new TI.Skip(32));
			//////////////////////////////////////////////////////////////////////////
			// array of 32 dwords. padding?
			Add(new TI.Skip(128));
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.Block<effects_block>(this, 0));
		}
		#endregion
	};
	#endregion

	#region terr_group
	[TI.TagGroup((int)TagGroups.Enumerated.terr, 1, 32)]
	public class terr_group : TI.Definition
	{
		#region terr_14_block
		[TI.Definition(1, 84)]
		public class terr_14_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public terr_14_block() : base(9)
			{
				Add(new TI.LongInteger()); // unknown 32 bits
				Add(new TI.LongInteger()); // unknown 32 bits
				Add(new TI.LongInteger()); // unknown 32 bits
				Add(new TI.LongInteger()); // unknown 32 bits
				Add(new TI.LongInteger()); // unknown 32 bits
				Add(new TI.LongInteger()); // unknown 32 bits
				Add(new TI.Pad(28));
				Add(new TI.TagReference(this)); // 34
				Add(new TI.TagReference(this)); // 44
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public terr_group() : base(3)
		{
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.TagReference(this));
			Add(new TI.Block<terr_14_block>(this, 0));
		}
		#endregion
	};
	#endregion

	// only used in farm house
	#region vegetation
	[TI.TagGroup((int)TagGroups.Enumerated.vege, 1, 148)]
	public class vegetation_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public vegetation_group() : base(28)
		{
			Add(new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.ShortInteger()); // unknown 16bits
			Add(new TI.RealVector3D()); // looks like it may be a vector, three reals at least
			Add(new TI.TagReference(this, Halo1.TagGroups.shdr)); // front view
			Add(new TI.TagReference(this, Halo1.TagGroups.shdr)); // side view
			Add(new TI.Real());
			Add(new TI.Real());
			Add(new TI.Real());
			Add(new TI.Real());
			Add(new TI.TagReference(this, Halo1.TagGroups.mode)); // model
			Add(new TI.Real());
			Add(new TI.Real());
			//////////////////////////////////////////////////////////////////////////
			// wasn't used in corn
			// pretty sure this is a vector3d
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			//////////////////////////////////////////////////////////////////////////
			// i believe this may be an angles2d
			Add(new TI.Real(TI.FieldType.Angle));
			Add(new TI.Real(TI.FieldType.Angle));
			//////////////////////////////////////////////////////////////////////////
			// wasn't used in corn
			// i believe this may be a vector3d
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			//////////////////////////////////////////////////////////////////////////
			// wasn't used in corn
			// i believe this may be a vector3d
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			Add(new TI.LongInteger()); // unknown 32 bits
			//////////////////////////////////////////////////////////////////////////
			Add(new TI.LongInteger()); // unknown 32 bits. wasn't used in corn
			Add(new TI.LongInteger()); // unknown 32 bits. wasn't used in corn
			Add(new TI.ShortInteger()); // unknown 16bits
			Add(new TI.ShortInteger()); // unknown 16bits
			Add(new TI.Real());
		}
		#endregion
	};
	#endregion

	public class CacheTagDatabase : Managers.CacheTagDatabase
	{
		CacheFile cacheFile;

		System.Collections.Generic.Dictionary<DatumIndex, Halo1.Tags.tag_database_entry_block> lookup = 
			new System.Collections.Generic.Dictionary<DatumIndex, Halo1.Tags.tag_database_entry_block>(DatumIndex.kEqualityComparer);

		#region Definition
		Halo1.Tags.tag_database_group db = new Halo1.Tags.tag_database_group();
		/// <summary>
		/// Database tag definition
		/// </summary>
		public override TI.Definition Definition { get { return db; } }
		#endregion

		/// <summary>
		/// Get a entry in the database
		/// </summary>
		/// <param name="hash">Hash value for tag entry we wish to get</param>
		/// <returns></returns>
		public Halo1.Tags.tag_database_entry_block this[int hash] { get { return db.Entries[hash]; } }

		public CacheTagDatabase(CacheFile cf) : base() { cacheFile = cf; }
		public CacheTagDatabase(CacheFile cf, Blam.CacheIndex.Item root_tag) : base(root_tag) { cacheFile = cf; }

		public override int Add(Blam.CacheIndex.Item tag)
		{
			int index = -1;

			Halo1.Tags.tag_database_entry_block entry;
			if (!lookup.TryGetValue(tag.Datum, out entry)) // hey, not in the database
			{
				string name = cacheFile.GetReferenceName(tag);

				index = db.Entries.Count;				// count will be the index of our new entry
				db.Entries.Add(out entry);				// add a new entry
				entry.Name.Value =						// set the name
					System.Text.Encoding.ASCII.GetBytes(name);
				entry.GroupTag.Value = tag.GroupTag.Tag;// set the group tag
				entry.HandleDataHigh.Value = name.GetHashCode();

				lookup.Add(tag.Datum, entry);			// add entry to the dictionary
				return entry.Flags.Value = index;
			}

			return entry.Flags.Value;
		}

		public override int AddDependent(Blam.CacheIndex.Item tag, Blam.CacheIndex.Item dependent_tag)
		{
			int index = Add(dependent_tag); // add and\or get the index of the child id entry

			var tag_entry = lookup[tag.Datum];
			var dtag_entry = lookup[dependent_tag.Datum];

			Halo1.Tags.tag_database_entry_reference_block eref;
			dtag_entry.ReferenceIds.Add(out eref); // add a new entry reference to the dependent tag which points to this tag
			eref.EntryReference.Value = tag_entry.Flags.Value;

			tag_entry.ChildIds.Add(out eref); // add a new entry reference to this tag which points to the dependent tag

			return eref.EntryReference.Value = index;
		}
	};
};