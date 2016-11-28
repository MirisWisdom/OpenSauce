/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1
{
	// maybe do this just to be consistent? or maybe not because it really doesn't matter, you decide %-)
	// rename definition 'model_animation_group' to 'model_animations_group'
	// rename definition 'model_collision_group' to 'model_collision_geometry_group
	// rename definition 'structure_bsp_group' to 'scenario_structure_bsp_group'

	public static partial class TagGroups
	{
		/// <summary>
		/// Object
		/// </summary>
		public static TagGroup obje = new TagGroup("obje", "object");

		/// <summary>
		/// Device
		/// </summary>
		public static TagGroup devi = new TagGroup("devi", obje, "device");

		/// <summary>
		/// Item
		/// </summary>
		public static TagGroup item = new TagGroup("item", obje, "item");

		/// <summary>
		/// Unit
		/// </summary>
		public static TagGroup unit = new TagGroup("unit", obje, "unit");

		/// <summary>
		/// Shader
		/// </summary>
		public static TagGroup shdr = new TagGroup("shdr", "shader");


		/// <summary>
		/// Antenna
		/// </summary>
		public static TagGroup ant_ = new TagGroup("ant!", "antenna");

		/// <summary>
		/// Biped
		/// </summary>
		public static TagGroup bipd = new TagGroup("bipd", unit, "biped");

		/// <summary>
		/// Bitmap
		/// </summary>
		public static TagGroup bitm = new TagGroup("bitm", "bitmap");

		/// <summary>
		/// Camera Track
		/// </summary>
		public static TagGroup trak = new TagGroup("trak", "camera_track");

		/// <summary>
		/// Color Table
		/// </summary>
		public static TagGroup colo = new TagGroup("colo", "color_table");

		/// <summary>
		/// Contral
		/// </summary>
		public static TagGroup cont = new TagGroup("cont", "contrail");

		/// <summary>
		/// Damage Effect
		/// </summary>
		public static TagGroup jpt_ = new TagGroup("jpt!", "damage_effect");

		/// <summary>
		/// Decal
		/// </summary>
		public static TagGroup deca = new TagGroup("deca", "decal");

		/// <summary>
		/// Detail Object Collection
		/// </summary>
		public static TagGroup dobc = new TagGroup("dobc", "detail_object_collection");

		/// <summary>
		/// Device Control
		/// </summary>
		public static TagGroup ctrl = new TagGroup("ctrl", devi, "device_control");

		/// <summary>
		/// Device Light Fixture
		/// </summary>
		public static TagGroup lifi = new TagGroup("lifi", devi, "device_light_fixture");

		/// <summary>
		/// Device Machine
		/// </summary>
		public static TagGroup mach = new TagGroup("mach", devi, "device_machine");

		/// <summary>
		/// Dialogue
		/// </summary>
		public static TagGroup udlg = new TagGroup("udlg", "dialogue");

		/// <summary>
		/// Effect
		/// </summary>
		public static TagGroup effe = new TagGroup("effe", "effect");

		/// <summary>
		/// Equipment
		/// </summary>
		public static TagGroup eqip = new TagGroup("eqip", item, "equipment");

		/// <summary>
		/// Fog
		/// </summary>
		public static TagGroup fog_ = new TagGroup("fog ", "fog");

		/// <summary>
		/// Garbage
		/// </summary>
		public static TagGroup garb = new TagGroup("garb", item, "garbage");

		/// <summary>
		/// Globals
		/// </summary>
		public static TagGroup matg = new TagGroup("matg", "globals");

		/// <summary>
		/// Grenade HUD Interface
		/// </summary>
		public static TagGroup grhi = new TagGroup("grhi", "grenade_hud_interface");

		/// <summary>
		/// HUD Globals
		/// </summary>
		public static TagGroup hudg = new TagGroup("hudg", "hud_globals");

		/// <summary>
		/// HUD Message Text
		/// </summary>
		public static TagGroup hmt_ = new TagGroup("hmt ", "hud_message_text");

		/// <summary>
		/// HUD Number
		/// </summary>
		public static TagGroup hud_ = new TagGroup("hud#", "hud_number");

		/// <summary>
		/// Item Collection
		/// </summary>
		public static TagGroup itmc = new TagGroup("itmc", "item_collection");

		/// <summary>
		/// Lens Flare
		/// </summary>
		public static TagGroup lens = new TagGroup("lens", "lens_flare");

		/// <summary>
		/// Light
		/// </summary>
		public static TagGroup ligh = new TagGroup("ligh", "light");

		/// <summary>
		/// Material Effects
		/// </summary>
		public static TagGroup foot = new TagGroup("foot", "material_effects");

		/// <summary>
		/// Meter
		/// </summary>
		public static TagGroup metr = new TagGroup("metr", "meter");

		/// <summary>
		/// Model
		/// </summary>
		public static TagGroup mode = new TagGroup("mode", "model");

		/// <summary>
		/// Model Collision Geometry
		/// </summary>
		public static TagGroup coll = new TagGroup("coll", "model_collision_geometry");

		/// <summary>
		/// Physics
		/// </summary>
		public static TagGroup phys = new TagGroup("phys", "physics");

		/// <summary>
		/// Point Physics
		/// </summary>
		public static TagGroup pphy = new TagGroup("pphy", "point_physics");

		/// <summary>
		/// Projectile
		/// </summary>
		public static TagGroup proj = new TagGroup("proj", obje, "projectile");

		/// <summary>
		/// Scenario
		/// </summary>
		public static TagGroup scnr = new TagGroup("scnr", "scenario");

		/// <summary>
		/// Scenario Structure BSP
		/// </summary>
		public static TagGroup sbsp = new TagGroup("sbsp", "scenario_structure_bsp");

		/// <summary>
		/// Scenery
		/// </summary>
		public static TagGroup scen = new TagGroup("scen", obje, "scenery");

		/// <summary>
		/// Sky
		/// </summary>
		public static TagGroup sky_ = new TagGroup("sky ", "sky");

		/// <summary>
		/// Sound
		/// </summary>
		public static TagGroup snd_ = new TagGroup("snd!", "sound");

		/// <summary>
		/// Sound Environment
		/// </summary>
		public static TagGroup snde = new TagGroup("snde", "sound_environment");

		/// <summary>
		/// Sound Looping
		/// </summary>
		public static TagGroup lsnd = new TagGroup("lsnd", "sound_looping");

		/// <summary>
		/// Unit HUD Interface
		/// </summary>
		public static TagGroup unhi = new TagGroup("unhi", "unit_hud_interface");

		/// <summary>
		/// Vehicle
		/// </summary>
		public static TagGroup vehi = new TagGroup("vehi", unit, "vehicle");

		/// <summary>
		/// Weapon
		/// </summary>
		public static TagGroup weap = new TagGroup("weap", item, "weapon");

		/// <summary>
		/// Weapon HUD Interface
		/// </summary>
		public static TagGroup wphi = new TagGroup("wphi", "weapon_hud_interface");

		/// <summary>
		/// Wind
		/// </summary>
		public static TagGroup wind = new TagGroup("wind", "wind");

		/// <summary>
		/// Actor
		/// </summary>
		public static TagGroup actr = new TagGroup("actr", "actor");

		/// <summary>
		/// Actor Variant
		/// </summary>
		public static TagGroup actv = new TagGroup("actv", "actor_variant");

		/// <summary>
		/// Continuous Damage Effect
		/// </summary>
		public static TagGroup cdmg = new TagGroup("cdmg", "continuous_damage_effect");

		/// <summary>
		/// Flag
		/// </summary>
		public static TagGroup flag = new TagGroup("flag", "flag");

		/// <summary>
		/// Font
		/// </summary>
		public static TagGroup font = new TagGroup("font", "font");

		/// <summary>
		/// GBX Model
		/// </summary>
		public static TagGroup mod2 = new TagGroup("mod2", "gbxmodel");

		/// <summary>
		/// Glow
		/// </summary>
		public static TagGroup glw_ = new TagGroup("glw!", "glow");

		/// <summary>
		/// Input Device Defaults
		/// </summary>
		public static TagGroup devc = new TagGroup("devc", "input_device_defaults");

		/// <summary>
		/// Light Volume
		/// </summary>
		public static TagGroup mgs2 = new TagGroup("mgs2", "light_volume");

		/// <summary>
		/// Lightning
		/// </summary>
		public static TagGroup elec = new TagGroup("elec", "lightning");

		/// <summary>
		/// Model Animations
		/// </summary>
		public static TagGroup antr = new TagGroup("antr", "model_animations");

		/// <summary>
		/// Multiplayer Scenario Description
		/// </summary>
		public static TagGroup mply = new TagGroup("mply", "multiplayer_scenario_description");

		/// <summary>
		/// Particle
		/// </summary>
		public static TagGroup part = new TagGroup("part", "particle");

		/// <summary>
		/// Particle System
		/// </summary>
		public static TagGroup pctl = new TagGroup("pctl", "particle_system");

		/// <summary>
		/// Placeholder
		/// </summary>
		public static TagGroup plac = new TagGroup("plac", obje, "placeholder");

		/// <summary>
		/// Preferences Network Game
		/// </summary>
		public static TagGroup ngpr = new TagGroup("ngpr", "preferences_network_game");

		/// <summary>
		/// Shader Effect
		/// </summary>
		public static TagGroup seff = new TagGroup("seff", shdr, "shader_effect");

		/// <summary>
		/// Shader Environment
		/// </summary>
		public static TagGroup senv = new TagGroup("senv", shdr, "shader_environment");

		/// <summary>
		/// Shader Model
		/// </summary>
		public static TagGroup soso = new TagGroup("soso", shdr, "shader_model");

		/// <summary>
		/// Shader Transparent Chicago
		/// </summary>
		public static TagGroup schi = new TagGroup("schi", shdr, "shader_transparent_chicago");

		/// <summary>
		/// Shader Transparent Chicago Extended
		/// </summary>
		public static TagGroup scex = new TagGroup("scex", shdr, "shader_transparent_chicago_extended");

		/// <summary>
		/// Shader Transparent Generic
		/// </summary>
		public static TagGroup sotr = new TagGroup("sotr", shdr, "shader_transparent_generic");

		/// <summary>
		/// Shader Transparent Glass
		/// </summary>
		public static TagGroup sgla = new TagGroup("sgla", shdr, "shader_transparent_glass");

		/// <summary>
		/// Shader Transparent Meter
		/// </summary>
		public static TagGroup smet = new TagGroup("smet", shdr, "shader_transparent_meter");

		/// <summary>
		/// Shader Transparent Plasma
		/// </summary>
		public static TagGroup spla = new TagGroup("spla", shdr, "shader_transparent_plasma");

		/// <summary>
		/// Shader Transparent Water
		/// </summary>
		public static TagGroup swat = new TagGroup("swat", shdr, "shader_transparent_water");

		/// <summary>
		/// Sound Scenery
		/// </summary>
		public static TagGroup ssce = new TagGroup("ssce", obje, "sound_scenery");

		/// <summary>
		/// Spheriod
		/// </summary>
		public static TagGroup boom = new TagGroup("boom", "spheroid");

		/// <summary>
		/// String List
		/// </summary>
		public static TagGroup str_ = new TagGroup("str#", "string_list");

		/// <summary>
		/// Tag Collection
		/// </summary>
		public static TagGroup tagc = new TagGroup("tagc", "tag_collection");

		/// <summary>
		/// UI Widget Collection
		/// </summary>
		public static TagGroup Soul = new TagGroup("Soul", "ui_widget_collection");

		/// <summary>
		/// UI Widget Definition
		/// </summary>
		public static TagGroup DeLa = new TagGroup("DeLa", "ui_widget_definition");

		/// <summary>
		/// Unicode String List
		/// </summary>
		public static TagGroup ustr = new TagGroup("ustr", "unicode_string_list");

		/// <summary>
		/// Virtual Keyboard
		/// </summary>
		public static TagGroup vcky = new TagGroup("vcky", "virtual_keyboard");

		/// <summary>
		/// Weather Particle System
		/// </summary>
		public static TagGroup rain = new TagGroup("rain", "weather_particle_system");


		/// <summary>
		/// Tag Database
		/// </summary>
		public static TagGroup tag_ = new TagGroup("tag+", "tag_database");


		/// <summary>
		/// scenario_scenery_resource
		/// </summary>
		public static TagGroup srscen = new TagGroup("*cen", "scenario_scenery_resource");

		/// <summary>
		/// scenario_bipeds_resource
		/// </summary>
		public static TagGroup srbipd = new TagGroup("*ipd", "scenario_bipeds_resource");

		/// <summary>
		/// scenario_vehicles_resource
		/// </summary>
		public static TagGroup srvehi = new TagGroup("*ehi", "scenario_vehicles_resource");

		/// <summary>
		/// scenario_equipment_resource
		/// </summary>
		public static TagGroup sreqip = new TagGroup("*qip", "scenario_equipment_resource");

		/// <summary>
		/// scenario_weapons_resource
		/// </summary>
		public static TagGroup srweap = new TagGroup("*eap", "scenario_weapons_resource");

		/// <summary>
		/// scenario_sound_scenery_resource
		/// </summary>
		public static TagGroup srssce = new TagGroup("*sce", "scenario_sound_scenery_resource");

		/// <summary>
		/// scenario_devices_resource
		/// </summary>
		public static TagGroup srdgrp = new TagGroup("dgr*", "scenario_devices_resource");

		/// <summary>
		/// scenario_decals_resource
		/// </summary>
		public static TagGroup srdeca = new TagGroup("dec*", "scenario_decals_resource");

		/// <summary>
		/// scenario_cinematics_resource
		/// </summary>
		public static TagGroup srcine = new TagGroup("cin*", "scenario_cinematics_resource");

		/// <summary>
		/// scenario_trigger_volumes_resource
		/// </summary>
		public static TagGroup srtrgr = new TagGroup("trg*", "scenario_trigger_volumes_resource");

		/// <summary>
		/// scenario_cluster_data_resource
		/// </summary>
		public static TagGroup srclut = new TagGroup("clu*", "scenario_cluster_data_resource");

		/// <summary>
		/// scenario_hs_source_file
		/// </summary>
		public static TagGroup srhscf = new TagGroup("hsc*", "scenario_hs_source_file");

		/// <summary>
		/// scenario_ai_resource
		/// </summary>
		public static TagGroup srai = new TagGroup("ai**", "scenario_ai_resource");

		/// <summary>
		/// scenario_comments_resource
		/// </summary>
		public static TagGroup srcmmt = new TagGroup("/**/", "scenario_comments_resource");


		/// <summary>
		/// project_yellow_globals
		/// </summary>
		public static TagGroup gelo = new TagGroup("gelo", "project_yellow_globals");
		/// <summary>
		/// project_yellow
		/// </summary>
		public static TagGroup yelo = new TagGroup("yelo", "project_yellow");
	};
}