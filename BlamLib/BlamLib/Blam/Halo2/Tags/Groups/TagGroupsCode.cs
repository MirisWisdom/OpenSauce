/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
	// bitmap			rasterizer\\invalid
	// scenery			incompetent\\default_object
	// shader			shaders\\invalid
	// point_physics	globals\\global_missing_point_physics
	// damage_effect	globals\\global_missing_damage_effect

	partial class TagGroups
	{
		/// <summary>
		/// object
		/// </summary>
		public static TagGroup obje = new TagGroup("obje", "object");

		/// <summary>
		/// device
		/// </summary>
		public static TagGroup devi = new TagGroup("devi", obje, "device");

		/// <summary>
		/// item
		/// </summary>
		public static TagGroup item = new TagGroup("item", obje, "item");

		/// <summary>
		/// unit
		/// </summary>
		public static TagGroup unit = new TagGroup("unit", obje, "unit");


		/// <summary>
		/// model
		/// </summary>
		public static TagGroup hlmt = new TagGroup("hlmt", "model");

		/// <summary>
		/// render_model
		/// </summary>
		public static TagGroup mode = new TagGroup("mode", "render_model");

		/// <summary>
		/// collision_model
		/// </summary>
		public static TagGroup coll = new TagGroup("coll", "collision_model");

		/// <summary>
		/// physics_model
		/// </summary>
		public static TagGroup phmo = new TagGroup("phmo", "physics_model");

		/// <summary>
		/// bitmap
		/// </summary>
		public static TagGroup bitm = new TagGroup("bitm", "bitmap");

		/// <summary>
		/// color_table
		/// </summary>
		public static TagGroup colo = new TagGroup("colo", "color_table");

		/// <summary>
		/// multilingual_unicode_string_list
		/// </summary>
		public static TagGroup unic = new TagGroup("unic", "multilingual_unicode_string_list");

		/// <summary>
		/// biped
		/// </summary>
		public static TagGroup bipd = new TagGroup("bipd", unit, "biped");

		/// <summary>
		/// vehicle
		/// </summary>
		public static TagGroup vehi = new TagGroup("vehi", unit, "vehicle");

		/// <summary>
		/// scenery
		/// </summary>
		public static TagGroup scen = new TagGroup("scen", obje, "scenery");

		/// <summary>
		/// crate
		/// </summary>
		public static TagGroup bloc = new TagGroup("bloc", obje, "crate");

		/// <summary>
		/// creature
		/// </summary>
		public static TagGroup crea = new TagGroup("crea", obje, "creature");

		/// <summary>
		/// physics
		/// </summary>
		public static TagGroup phys = new TagGroup("phys", "physics");

		/// <summary>
		/// contrail
		/// </summary>
		public static TagGroup cont = new TagGroup("cont", "contrail");

		/// <summary>
		/// weapon
		/// </summary>
		public static TagGroup weap = new TagGroup("weap", item, "weapon");

		/// <summary>
		/// light
		/// </summary>
		public static TagGroup ligh = new TagGroup("ligh", "light");

		/// <summary>
		/// effect
		/// </summary>
		public static TagGroup effe = new TagGroup("effe", "effect");

		/// <summary>
		/// particle
		/// </summary>
		public static TagGroup prt3 = new TagGroup("prt3", "particle");

		/// <summary>
		/// particle_model
		/// </summary>
		public static TagGroup PRTM = new TagGroup("PRTM", "particle_model");

		/// <summary>
		/// particle_physics
		/// </summary>
		public static TagGroup pmov = new TagGroup("pmov", "particle_physics");

		/// <summary>
		/// globals
		/// </summary>
		public static TagGroup matg = new TagGroup("matg", "globals");

		/// <summary>
		/// sound
		/// </summary>
		public static TagGroup snd_ = new TagGroup("snd!", "sound");

		/// <summary>
		/// sound_looping
		/// </summary>
		public static TagGroup lsnd = new TagGroup("lsnd", "sound_looping");

		/// <summary>
		/// equipment
		/// </summary>
		public static TagGroup eqip = new TagGroup("eqip", item, "equipment");

		/// <summary>
		/// antenna
		/// </summary>
		public static TagGroup ant_ = new TagGroup("ant!", "antenna");

		/// <summary>
		/// light_volume
		/// </summary>
		public static TagGroup MGS2 = new TagGroup("MGS2", "light_volume");

		/// <summary>
		/// liquid
		/// </summary>
		public static TagGroup tdtl = new TagGroup("tdtl", "liquid");

		/// <summary>
		/// cellular_automata
		/// </summary>
		public static TagGroup devo = new TagGroup("devo", "cellular_automata");

		/// <summary>
		/// cellular_automata2d
		/// </summary>
		public static TagGroup whip = new TagGroup("whip", "cellular_automata2d");

		/// <summary>
		/// stereo_system
		/// </summary>
		public static TagGroup BooM = new TagGroup("BooM", "stereo_system");

		/// <summary>
		/// camera_track
		/// </summary>
		public static TagGroup trak = new TagGroup("trak", "camera_track");

		/// <summary>
		/// projectile
		/// </summary>
		public static TagGroup proj = new TagGroup("proj", obje, "projectile");

		/// <summary>
		/// device_machine
		/// </summary>
		public static TagGroup mach = new TagGroup("mach", devi, "device_machine");

		/// <summary>
		/// device_control
		/// </summary>
		public static TagGroup ctrl = new TagGroup("ctrl", devi, "device_control");

		/// <summary>
		/// device_light_fixture
		/// </summary>
		public static TagGroup lifi = new TagGroup("lifi", devi, "device_light_fixture");

		/// <summary>
		/// point_physics
		/// </summary>
		public static TagGroup pphy = new TagGroup("pphy", "point_physics");

		/// <summary>
		/// scenario_structure_lightmap
		/// </summary>
		public static TagGroup ltmp = new TagGroup("ltmp", "scenario_structure_lightmap");

		/// <summary>
		/// scenario_structure_bsp
		/// </summary>
		public static TagGroup sbsp = new TagGroup("sbsp", "scenario_structure_bsp");

		/// <summary>
		/// scenario
		/// </summary>
		public static TagGroup scnr = new TagGroup("scnr", "scenario");

		/// <summary>
		/// shader
		/// </summary>
		public static TagGroup shad = new TagGroup("shad", "shader");

		/// <summary>
		/// shader_template
		/// </summary>
		public static TagGroup stem = new TagGroup("stem", "shader_template");

		/// <summary>
		/// shader_light_response
		/// </summary>
		public static TagGroup slit = new TagGroup("slit", "shader_light_response");

		/// <summary>
		/// shader_pass
		/// </summary>
		public static TagGroup spas = new TagGroup("spas", "shader_pass");

		/// <summary>
		/// vertex_shader
		/// </summary>
		public static TagGroup vrtx = new TagGroup("vrtx", "vertex_shader");

		/// <summary>
		/// pixel_shader
		/// </summary>
		public static TagGroup pixl = new TagGroup("pixl", "pixel_shader");

		/// <summary>
		/// decorator_set
		/// </summary>
		public static TagGroup DECR = new TagGroup("DECR", "decorator_set");

		/// <summary>
		/// decorators
		/// </summary>
		public static TagGroup DECP = new TagGroup("DECP", "decorators");

		/// <summary>
		/// sky
		/// </summary>
		public static TagGroup sky_ = new TagGroup("sky ", "sky");

		/// <summary>
		/// wind
		/// </summary>
		public static TagGroup wind = new TagGroup("wind", "wind");

		/// <summary>
		/// sound_environment
		/// </summary>
		public static TagGroup snde = new TagGroup("snde", "sound_environment");

		/// <summary>
		/// lens_flare
		/// </summary>
		public static TagGroup lens = new TagGroup("lens", "lens_flare");

		/// <summary>
		/// planar_fog
		/// </summary>
		public static TagGroup fog  = new TagGroup("fog ", "planar_fog");

		/// <summary>
		/// patchy_fog
		/// </summary>
		public static TagGroup fpch = new TagGroup("fpch", "patchy_fog");

		/// <summary>
		/// meter
		/// </summary>
		public static TagGroup metr = new TagGroup("metr", "meter");

		/// <summary>
		/// decal
		/// </summary>
		public static TagGroup deca = new TagGroup("deca", "decal");

		/// <summary>
		/// colony
		/// </summary>
		public static TagGroup coln = new TagGroup("coln", "colony");

		/// <summary>
		/// damage_effect
		/// </summary>
		public static TagGroup jpt_ = new TagGroup("jpt!", "damage_effect");

		/// <summary>
		/// dialogue
		/// </summary>
		public static TagGroup udlg = new TagGroup("udlg", "dialogue");

		/// <summary>
		/// item_collection
		/// </summary>
		public static TagGroup itmc = new TagGroup("itmc", "item_collection");

		/// <summary>
		/// vehicle_collection
		/// </summary>
		public static TagGroup vehc = new TagGroup("vehc", "vehicle_collection");

		/// <summary>
		/// weapon_hud_interface
		/// </summary>
		public static TagGroup wphi = new TagGroup("wphi", "weapon_hud_interface");

		/// <summary>
		/// grenade_hud_interface
		/// </summary>
		public static TagGroup grhi = new TagGroup("grhi", "grenade_hud_interface");

		/// <summary>
		/// unit_hud_interface
		/// </summary>
		public static TagGroup unhi = new TagGroup("unhi", "unit_hud_interface");

		/// <summary>
		/// new_hud_definition
		/// </summary>
		public static TagGroup nhdt = new TagGroup("nhdt", "new_hud_definition");

		/// <summary>
		/// hud_number
		/// </summary>
		public static TagGroup hud_ = new TagGroup("hud#", "hud_number");

		/// <summary>
		/// hud_globals
		/// </summary>
		public static TagGroup hudg = new TagGroup("hudg", "hud_globals");

		/// <summary>
		/// multiplayer_scenario_description
		/// </summary>
		public static TagGroup mply = new TagGroup("mply", "multiplayer_scenario_description");

		/// <summary>
		/// detail_object_collection
		/// </summary>
		public static TagGroup dobc = new TagGroup("dobc", "detail_object_collection");

		/// <summary>
		/// sound_scenery
		/// </summary>
		public static TagGroup ssce = new TagGroup("ssce", obje, "sound_scenery");

		/// <summary>
		/// hud_message_text
		/// </summary>
		public static TagGroup hmt_ = new TagGroup("hmt ", "hud_message_text");

		/// <summary>
		/// user_interface_screen_widget_definition
		/// </summary>
		public static TagGroup wgit = new TagGroup("wgit", "user_interface_screen_widget_definition");

		/// <summary>
		/// user_interface_list_skin_definition
		/// </summary>
		public static TagGroup skin = new TagGroup("skin", "user_interface_list_skin_definition");

		/// <summary>
		/// user_interface_globals_definition
		/// </summary>
		public static TagGroup wgtz = new TagGroup("wgtz", "user_interface_globals_definition");

		/// <summary>
		/// user_interface_shared_globals_definition
		/// </summary>
		public static TagGroup wigl = new TagGroup("wigl", "user_interface_shared_globals_definition");

		/// <summary>
		/// text_value_pair_definition
		/// </summary>
		public static TagGroup sily = new TagGroup("sily", "text_value_pair_definition");

		/// <summary>
		/// multiplayer_variant_settings_interface_definition
		/// </summary>
		public static TagGroup goof = new TagGroup("goof", "multiplayer_variant_settings_interface_definition");

		/// <summary>
		/// material_effects
		/// </summary>
		public static TagGroup foot = new TagGroup("foot", "material_effects");

		/// <summary>
		/// garbage
		/// </summary>
		public static TagGroup garb = new TagGroup("garb", item, "garbage");

		/// <summary>
		/// style
		/// </summary>
		public static TagGroup styl = new TagGroup("styl", "style");

		/// <summary>
		/// character
		/// </summary>
		public static TagGroup char_ = new TagGroup("char", "character");

		/// <summary>
		/// ai_dialogue_globals
		/// </summary>
		public static TagGroup adlg = new TagGroup("adlg", "ai_dialogue_globals");

		/// <summary>
		/// ai_mission_dialogue
		/// </summary>
		public static TagGroup mdlg = new TagGroup("mdlg", "ai_mission_dialogue");

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
		/// scenario_lights_resource
		/// </summary>
		public static TagGroup srligh = new TagGroup("*igh", "scenario_lights_resource");

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
		/// scenario_creature_resource
		/// </summary>
		public static TagGroup srcrea = new TagGroup("*rea", "scenario_creature_resource");

		/// <summary>
		/// scenario_decorators_resource
		/// </summary>
		public static TagGroup srdcrs = new TagGroup("dc*s", "scenario_decorators_resource");

		/// <summary>
		/// scenario_structure_lighting_resource
		/// </summary>
		public static TagGroup srsslt = new TagGroup("sslt", "scenario_structure_lighting_resource");

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
		/// breakable_surface
		/// </summary>
		public static TagGroup bsdt = new TagGroup("bsdt", "breakable_surface");

		/// <summary>
		/// material_physics
		/// </summary>
		public static TagGroup mpdt = new TagGroup("mpdt", "material_physics");

		/// <summary>
		/// sound_classes
		/// </summary>
		public static TagGroup sncl = new TagGroup("sncl", "sound_classes");

		/// <summary>
		/// multiplayer_globals
		/// </summary>
		public static TagGroup mulg = new TagGroup("mulg", "multiplayer_globals");

		/// <summary>
		/// sound_effect_template
		/// </summary>
		public static TagGroup _fx_ = new TagGroup("<fx>", "sound_effect_template");

		/// <summary>
		/// sound_effect_collection
		/// </summary>
		public static TagGroup sfx_ = new TagGroup("sfx+", "sound_effect_collection");

		/// <summary>
		/// chocolate_mountain
		/// </summary>
		public static TagGroup gldf = new TagGroup("gldf", "chocolate_mountain");

		/// <summary>
		/// model_animation_graph
		/// </summary>
		public static TagGroup jmad = new TagGroup("jmad", "model_animation_graph");

		/// <summary>
		/// cloth
		/// </summary>
		public static TagGroup clwd = new TagGroup("clwd", "cloth");

		/// <summary>
		/// screen_effect
		/// </summary>
		public static TagGroup egor = new TagGroup("egor", "screen_effect");

		/// <summary>
		/// weather_system
		/// </summary>
		public static TagGroup weat = new TagGroup("weat", "weather_system");

		/// <summary>
		/// sound_mix
		/// </summary>
		public static TagGroup snmx = new TagGroup("snmx", "sound_mix");

		/// <summary>
		/// sound_dialogue_constants
		/// </summary>
		public static TagGroup spk_ = new TagGroup("spk!", "sound_dialogue_constants");

		/// <summary>
		/// sound_cache_file_gestalt
		/// </summary>
		public static TagGroup ugh_ = new TagGroup("ugh!", "sound_cache_file_gestalt");

		/// <summary>
		/// cache_file_sound
		/// </summary>
		public static TagGroup shit = new TagGroup("$#!+", "cache_file_sound");

		/// <summary>
		/// mouse_cursor_definition
		/// </summary>
		public static TagGroup mcsr = new TagGroup("mcsr", "mouse_cursor_definition");

		/// <summary>
		/// tag_database
		/// </summary>
		public static TagGroup tag_ = new TagGroup("tag+", "tag_database");
	};
}