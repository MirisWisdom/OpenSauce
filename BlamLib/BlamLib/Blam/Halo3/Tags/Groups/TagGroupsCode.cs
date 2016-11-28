/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3
{
	public static partial class TagGroups
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
		/// render_method
		/// </summary>
		public static TagGroup rm__ = new TagGroup("rm  ", "render_method");
		


		/// <summary>
		/// shader_contrail
		/// </summary>
		public static TagGroup _rmc = new TagGroup("\0rmc", rm__, "shader_contrail");
		
		/// <summary>
		/// shader_particle
		/// </summary>
		public static TagGroup _rmp = new TagGroup("\0rmp", rm__, "shader_particle");
		
		/// <summary>
		/// cache_file_sound
		/// </summary>
		public static TagGroup shit = new TagGroup("$#!+", "cache_file_sound");

		/// <summary>
		/// scenario_scenery_resource
		/// </summary>
		public static TagGroup srscen = new TagGroup("*cen", "scenario_scenery_resource");

		/// <summary>
		/// scenario_weapons_resource
		/// </summary>
		public static TagGroup srweap = new TagGroup("*eap", "scenario_weapons_resource");
		
		/// <summary>
		/// scenario_vehicles_resource
		/// </summary>
		public static TagGroup srvehi = new TagGroup("*ehi", "scenario_vehicles_resource");

		/// <summary>
		/// scenario_effect_scenery_resource
		/// </summary>
		public static TagGroup srefsc = new TagGroup("*fsc", "scenario_effect_scenery_resource");

		/// <summary>
		/// scenario_lights_resource
		/// </summary>
		public static TagGroup srligh = new TagGroup("*igh", "scenario_lights_resource");

		/// <summary>
		/// scenario_bipeds_resource
		/// </summary>
		public static TagGroup srbipd = new TagGroup("*ipd", "scenario_bipeds_resource");

		/// <summary>
		/// scenario_equipment_resource
		/// </summary>
		public static TagGroup sreqip = new TagGroup("*qip", "scenario_equipment_resource");

		/// <summary>
		/// scenario_creature_resource
		/// </summary>
		public static TagGroup srcrea = new TagGroup("*rea", "scenario_creature_resource");

		/// <summary>
		/// scenario_sound_scenery_resource
		/// </summary>
		public static TagGroup srssce = new TagGroup("*sce", "scenario_sound_scenery_resource");

		/// <summary>
		/// scenario_comments_resource
		/// </summary>
		public static TagGroup srcmmt = new TagGroup("/**/", "scenario_comments_resource");

		/// <summary>
		/// sound_effect_template
		/// </summary>
		public static TagGroup _fx_ = new TagGroup("<fx>", "sound_effect_template");

		/// <summary>
		/// stereo_system
		/// </summary>
		public static TagGroup BooM = new TagGroup("BooM", "stereo_system");

		/// <summary>
		/// ai_dialogue_globals
		/// </summary>
		public static TagGroup adlg = new TagGroup("adlg", "ai_dialogue_globals");

		/// <summary>
		/// scenario_ai_resource
		/// </summary>
		public static TagGroup srai = new TagGroup("ai**", "scenario_ai_resource");

		/// <summary>
		/// antenna
		/// </summary>
		public static TagGroup ant_ = new TagGroup("ant!", "antenna");

		/// <summary>
		/// beam_system
		/// </summary>
		public static TagGroup beam = new TagGroup("beam", "beam_system");

		/// <summary>
		/// bink
		/// </summary>
		public static TagGroup bink = new TagGroup("bink", "bink");

		/// <summary>
		/// biped
		/// </summary>
		public static TagGroup bipd = new TagGroup("bipd", "biped");

		/// <summary>
		/// bitmap
		/// </summary>
		public static TagGroup bitm = new TagGroup("bitm", "bitmap");

		/// <summary>
		/// gui_button_key_definition
		/// </summary>
		public static TagGroup bkey = new TagGroup("bkey", "gui_button_key_definition");

		/// <summary>
		/// crate
		/// </summary>
		public static TagGroup bloc = new TagGroup("bloc", "crate");

		/// <summary>
		/// gui_bitmap_widget_definition
		/// </summary>
		public static TagGroup bmp3 = new TagGroup("bmp3", "gui_bitmap_widget_definition");

		/// <summary>
		/// breakable_surface
		/// </summary>
		public static TagGroup bsdt = new TagGroup("bsdt", "breakable_surface");

		/// <summary>
		/// collision_damage
		/// </summary>
		public static TagGroup cddf = new TagGroup("cddf", "collision_damage");

		/// <summary>
		/// camera_fx_settings
		/// </summary>
		public static TagGroup cfxs = new TagGroup("cfxs", "camera_fx_settings");

		/// <summary>
		/// chud_animation_definition
		/// </summary>
		public static TagGroup chad = new TagGroup("chad", "chud_animation_definition");

		/// <summary>
		/// character
		/// </summary>
		public static TagGroup char_ = new TagGroup("char", "character");

		/// <summary>
		/// chud_definition
		/// </summary>
		public static TagGroup chdt = new TagGroup("chdt", "chud_definition");

		/// <summary>
		/// chud_globals_definition
		/// </summary>
		public static TagGroup chgd = new TagGroup("chgd", "chud_globals_definition");

		/// <summary>
		/// chocolate_mountain_new
		/// </summary>
		public static TagGroup chmt = new TagGroup("chmt", "chocolate_mountain_new");

		/// <summary>
		/// scenario_cinematics_resource
		/// </summary>
		public static TagGroup srcine = new TagGroup("cin*", "scenario_cinematics_resource");

		/// <summary>
		/// cinematic
		/// </summary>
		public static TagGroup cine = new TagGroup("cine", "cinematic");

		/// <summary>
		/// cinematic_scene
		/// </summary>
		public static TagGroup cisc = new TagGroup("cisc", "cinematic_scene");

		/// <summary>
		/// scenario_cluster_data_resource
		/// </summary>
		public static TagGroup srclut = new TagGroup("clu*", "scenario_cluster_data_resource");

		/// <summary>
		/// cloth
		/// </summary>
		public static TagGroup clwd = new TagGroup("clwd", "cloth");

		/// <summary>
		/// contrail_system
		/// </summary>
		public static TagGroup cntl = new TagGroup("cntl", "contrail_system");

		/// <summary>
		/// collision_model
		/// </summary>
		public static TagGroup coll = new TagGroup("coll", "collision_model");

		/// <summary>
		/// color_table
		/// </summary>
		public static TagGroup colo = new TagGroup("colo", "color_table");

		/// <summary>
		/// creature
		/// </summary>
		public static TagGroup crea = new TagGroup("crea", obje, "creature");

		/// <summary>
		/// cortana_effect_definition
		/// </summary>
		public static TagGroup crte = new TagGroup("crte", "cortana_effect_definition");

		/// <summary>
		/// device_control
		/// </summary>
		public static TagGroup ctrl = new TagGroup("ctrl", devi, "device_control");

		/// <summary>
		/// scenario_cubemap_resource
		/// </summary>
		public static TagGroup srcube = new TagGroup("cub*", "scenario_cubemap_resource");

		/// <summary>
		/// scenario_decorators_resource
		/// </summary>
		public static TagGroup srdcrs = new TagGroup("dc*s", "scenario_decorators_resource");

		/// <summary>
		/// decorator_set
		/// </summary>
		public static TagGroup dctr = new TagGroup("dctr", "decorator_set");

		/// <summary>
		/// scenario_decals_resource
		/// </summary>
		public static TagGroup srdeca = new TagGroup("dec*", "scenario_decals_resource");

		/// <summary>
		/// decal_system
		/// </summary>
		public static TagGroup decs = new TagGroup("decs", "decal_system");

		/// <summary>
		/// cellular_automata
		/// </summary>
		public static TagGroup devo = new TagGroup("devo", "cellular_automata");

		/// <summary>
		/// scenario_devices_resource
		/// </summary>
		public static TagGroup srdgrp = new TagGroup("dgr*", "scenario_devices_resource");

		/// <summary>
		/// detail_object_collection
		/// </summary>
		public static TagGroup dobc = new TagGroup("dobc", "detail_object_collection");

		/// <summary>
		/// rasterizer_cache_file_globals
		/// </summary>
		public static TagGroup draw = new TagGroup("draw", "rasterizer_cache_file_globals");

		/// <summary>
		/// damage_response_definition
		/// </summary>
		public static TagGroup drdf = new TagGroup("drdf", "damage_response_definition");

		/// <summary>
		/// gui_datasource_definition
		/// </summary>
		public static TagGroup dsrc = new TagGroup("dsrc", "gui_datasource_definition");

		/// <summary>
		/// effect
		/// </summary>
		public static TagGroup effe = new TagGroup("effe", "effect");

		/// <summary>
		/// effect_globals
		/// </summary>
		public static TagGroup effg = new TagGroup("effg", "effect_globals");

		/// <summary>
		/// effect_scenery
		/// </summary>
		public static TagGroup efsc = new TagGroup("efsc", obje, "effect_scenery");

		/// <summary>
		/// screen_effect
		/// </summary>
		public static TagGroup egor = new TagGroup("egor", "screen_effect");

		/// <summary>
		/// equipment
		/// </summary>
		public static TagGroup eqip = new TagGroup("eqip", item, "equipment");

		/// <summary>
		/// flock
		/// </summary>
		public static TagGroup flck = new TagGroup("flck", "flock");

		/// <summary>
		/// fluid_dynamics
		/// </summary>
		public static TagGroup fldy = new TagGroup("fldy", "fluid_dynamics");

		/// <summary>
		/// planar_fog
		/// </summary>
		public static TagGroup fog_ = new TagGroup("fog ", "planar_fog");

		/// <summary>
		/// material_effects
		/// </summary>
		public static TagGroup foot = new TagGroup("foot", "material_effects");

		/// <summary>
		/// patchy_fog
		/// </summary>
		public static TagGroup fpch = new TagGroup("fpch", "patchy_fog");

		/// <summary>
		/// fragment
		/// </summary>
		public static TagGroup frag = new TagGroup("frag", "fragment");

		/// <summary>
		/// giant
		/// </summary>
		public static TagGroup gint = new TagGroup("gint", unit, "giant");

		/// <summary>
		/// global_pixel_shader
		/// </summary>
		public static TagGroup glps = new TagGroup("glps", "global_pixel_shader");

		/// <summary>
		/// global_vertex_shader
		/// </summary>
		public static TagGroup glvs = new TagGroup("glvs", "global_vertex_shader");

		/// <summary>
		/// multiplayer_variant_settings_interface_definition
		/// </summary>
		public static TagGroup goof = new TagGroup("goof", "multiplayer_variant_settings_interface_definition");

		/// <summary>
		/// gui_group_widget_definition
		/// </summary>
		public static TagGroup grup = new TagGroup("grup", "gui_group_widget_definition");

		/// <summary>
		/// model
		/// </summary>
		public static TagGroup hlmt = new TagGroup("hlmt", "model");

		/// <summary>
		/// hlsl_include
		/// </summary>
		public static TagGroup hlsl = new TagGroup("hlsl", "hlsl_include");

		/// <summary>
		/// scenario_hs_source_file
		/// </summary>
		public static TagGroup srhscf = new TagGroup("hsc*", "scenario_hs_source_file");

		/// <summary>
		/// item_collection
		/// </summary>
		public static TagGroup itmc = new TagGroup("itmc", "item_collection");

		/// <summary>
		/// model_animation_graph
		/// </summary>
		public static TagGroup jmad = new TagGroup("jmad", "model_animation_graph");

		/// <summary>
		/// sandbox_text_value_pair_definition
		/// </summary>
		public static TagGroup jmrq = new TagGroup("jmrq", "sandbox_text_value_pair_definition");

		/// <summary>
		/// damage_effect
		/// </summary>
		public static TagGroup jpt_ = new TagGroup("jpt!", "damage_effect");

		/// <summary>
		/// lens_flare
		/// </summary>
		public static TagGroup lens = new TagGroup("lens", "lens_flare");

		/// <summary>
		/// light
		/// </summary>
		public static TagGroup ligh = new TagGroup("ligh", "light");

		/// <summary>
		/// sound_looping
		/// </summary>
		public static TagGroup lsnd = new TagGroup("lsnd", "sound_looping");

		/// <summary>
		/// gui_list_widget_definition
		/// </summary>
		public static TagGroup lst3 = new TagGroup("lst3", "gui_list_widget_definition");

		/// <summary>
		/// leaf_system
		/// </summary>
		public static TagGroup lswd = new TagGroup("lswd", "leaf_system");

		/// <summary>
		/// light_volume_system
		/// </summary>
		public static TagGroup ltvl = new TagGroup("ltvl", "light_volume_system");

		/// <summary>
		/// device_machine
		/// </summary>
		public static TagGroup mach = new TagGroup("mach", devi, "device_machine");

		/// <summary>
		/// globals
		/// </summary>
		public static TagGroup matg = new TagGroup("matg", "globals");

		/// <summary>
		/// gui_model_widget_definition
		/// </summary>
		public static TagGroup mdl3 = new TagGroup("mdl3", "gui_model_widget_definition");

		/// <summary>
		/// ai_mission_dialogue
		/// </summary>
		public static TagGroup mdlg = new TagGroup("mdlg", "ai_mission_dialogue");

		/// <summary>
		/// meter
		/// </summary>
		public static TagGroup metr = new TagGroup("metr", "meter");

		/// <summary>
		/// muffin
		/// </summary>
		public static TagGroup mffn = new TagGroup("mffn", "muffin");

		/// <summary>
		/// render_model
		/// </summary>
		public static TagGroup mode = new TagGroup("mode", "render_model");

		/// <summary>
		/// multiplayer_scenario_description
		/// </summary>
		public static TagGroup mply = new TagGroup("mply", "multiplayer_scenario_description");

		/// <summary>
		/// multiplayer_globals
		/// </summary>
		public static TagGroup mulg = new TagGroup("mulg", "multiplayer_globals");

		/// <summary>
		/// new_cinematic_lighting
		/// </summary>
		public static TagGroup nclt = new TagGroup("nclt", "new_cinematic_lighting");

		/// <summary>
		/// performance_throttles
		/// </summary>
		public static TagGroup perf = new TagGroup("perf", "performance_throttles");

		/// <summary>
		/// physics_model
		/// </summary>
		public static TagGroup phmo = new TagGroup("phmo", "physics_model");

		/// <summary>
		/// pixel_shader
		/// </summary>
		public static TagGroup pixl = new TagGroup("pixl", "pixel_shader");

		/// <summary>
		/// cache_file_resource_layout_table
		/// </summary>
		public static TagGroup play = new TagGroup("play", "cache_file_resource_layout_table");

		/// <summary>
		/// particle_model
		/// </summary>
		public static TagGroup pmdf = new TagGroup("pmdf", "particle_model");

		/// <summary>
		/// particle_physics
		/// </summary>
		public static TagGroup pmov = new TagGroup("pmov", "particle_physics");

		/// <summary>
		/// point_physics
		/// </summary>
		public static TagGroup pphy = new TagGroup("pphy", "point_physics");

		/// <summary>
		/// projectile
		/// </summary>
		public static TagGroup proj = new TagGroup("proj", obje, "projectile");

		/// <summary>
		/// particle
		/// </summary>
		public static TagGroup prt3 = new TagGroup("prt3", "particle");

		/// <summary>
		/// rasterizer_globals
		/// </summary>
		public static TagGroup rasg = new TagGroup("rasg", "rasterizer_globals");

		/// <summary>
		/// shader_beam
		/// </summary>
		public static TagGroup rmb_ = new TagGroup("rmb ", rm__, "shader_beam");

		/// <summary>
		/// shader_custom
		/// </summary>
		public static TagGroup rmcs = new TagGroup("rmcs", rm__, "shader_custom");

		/// <summary>
		/// shader_cortana
		/// </summary>
		public static TagGroup rmct = new TagGroup("rmct", rm__, "shader_cortana");

		/// <summary>
		/// shader_decal
		/// </summary>
		public static TagGroup rmd_ = new TagGroup("rmd ", rm__, "shader_decal");

		/// <summary>
		/// render_method_definition
		/// </summary>
		public static TagGroup rmdf = new TagGroup("rmdf", "render_method_definition");

		/// <summary>
		/// shader_foliage
		/// </summary>
		public static TagGroup rmfl = new TagGroup("rmfl", rm__, "shader_foliage");

		/// <summary>
		/// shader_halogram
		/// </summary>
		public static TagGroup rmhg = new TagGroup("rmhg", rm__, "shader_halogram");

		/// <summary>
		/// shader_light_volume
		/// </summary>
		public static TagGroup rmlv = new TagGroup("rmlv", rm__, "shader_light_volume");

		/// <summary>
		/// render_method_option
		/// </summary>
		public static TagGroup rmop = new TagGroup("rmop", "render_method_option");

		/// <summary>
		/// shader
		/// </summary>
		public static TagGroup rmsh = new TagGroup("rmsh", rm__, "shader");

		/// <summary>
		/// shader_skin
		/// </summary>
		public static TagGroup rmsk = new TagGroup("rmsk", rm__, "shader_skin");

		/// <summary>
		/// render_method_template
		/// </summary>
		public static TagGroup rmt2 = new TagGroup("rmt2", "render_method_template");

		/// <summary>
		/// shader_terrain
		/// </summary>
		public static TagGroup rmtr = new TagGroup("rmtr", rm__, "shader_terrain");

		/// <summary>
		/// shader_water
		/// </summary>
		public static TagGroup rmw_ = new TagGroup("rmw ", rm__, "shader_water");

		/// <summary>
		/// render_water_ripple
		/// </summary>
		public static TagGroup rwrd = new TagGroup("rwrd", "render_water_ripple");

		/// <summary>
		/// scenario_faux_data
		/// </summary>
		public static TagGroup sFdT = new TagGroup("sFdT", "scenario_faux_data");

		/// <summary>
		/// scenario_lightmap
		/// </summary>
		public static TagGroup sLdT = new TagGroup("sLdT", "scenario_lightmap");

		/// <summary>
		/// scenario_structure_bsp
		/// </summary>
		public static TagGroup sbsp = new TagGroup("sbsp", "scenario_structure_bsp");

		/// <summary>
		/// scenery
		/// </summary>
		public static TagGroup scen = new TagGroup("scen", obje, "scenery");

		/// <summary>
		/// gui_screen_widget_definition
		/// </summary>
		public static TagGroup scn3 = new TagGroup("scn3", "gui_screen_widget_definition");

		/// <summary>
		/// scenario
		/// </summary>
		public static TagGroup scnr = new TagGroup("scnr", "scenario");

		/// <summary>
		/// structure_design
		/// </summary>
		public static TagGroup sddt = new TagGroup("sddt", "structure_design");

		/// <summary>
		/// area_screen_effect
		/// </summary>
		public static TagGroup sefc = new TagGroup("sefc", "area_screen_effect");

		/// <summary>
		/// sound_effect_collection
		/// </summary>
		public static TagGroup sfx_ = new TagGroup("sfx+", "sound_effect_collection");

		/// <summary>
		/// sound_global_propagation
		/// </summary>
		public static TagGroup sgp_ = new TagGroup("sgp!", "sound_global_propagation");

		/// <summary>
		/// shield_impact
		/// </summary>
		public static TagGroup shit2 = new TagGroup("shit", "shield_impact");

		/// <summary>
		/// text_value_pair_definition
		/// </summary>
		public static TagGroup sily = new TagGroup("sily", "text_value_pair_definition");

		/// <summary>
		/// gui_skin_definition
		/// </summary>
		public static TagGroup skn3 = new TagGroup("skn3", "gui_skin_definition");

		/// <summary>
		/// scenario_sky_references_resource
		/// </summary>
		public static TagGroup srsky_ = new TagGroup("sky*", "scenario_sky_references_resource");

		/// <summary>
		/// sky_atm_parameters
		/// </summary>
		public static TagGroup skya = new TagGroup("skya", "sky_atm_parameters");

		/// <summary>
		/// shared_cache_file_layout
		/// </summary>
		public static TagGroup smap = new TagGroup("smap", "shared_cache_file_layout");

		/// <summary>
		/// sound_classes
		/// </summary>
		public static TagGroup sncl = new TagGroup("sncl", "sound_classes");

		/// <summary>
		/// sound
		/// </summary>
		public static TagGroup snd_ = new TagGroup("snd!", "sound");

		/// <summary>
		/// sound_environment
		/// </summary>
		public static TagGroup snde = new TagGroup("snde", "sound_environment");

		/// <summary>
		/// sound_mix
		/// </summary>
		public static TagGroup snmx = new TagGroup("snmx", "sound_mix");

		/// <summary>
		/// sound_dialogue_constants
		/// </summary>
		public static TagGroup spk_ = new TagGroup("spk!", "sound_dialogue_constants");

		/// <summary>
		/// sound_scenery
		/// </summary>
		public static TagGroup ssce = new TagGroup("ssce", obje, "sound_scenery");

		/// <summary>
		/// scenario_structure_lighting_resource
		/// </summary>
		public static TagGroup srsslt = new TagGroup("sslt", "scenario_structure_lighting_resource");

		/// <summary>
		/// scenario_structure_lighting_info
		/// </summary>
		public static TagGroup stli = new TagGroup("stli", "scenario_structure_lighting_info");

		/// <summary>
		/// structure_seams
		/// </summary>
		public static TagGroup stse = new TagGroup("stse", "structure_seams");

		/// <summary>
		/// style
		/// </summary>
		public static TagGroup styl = new TagGroup("styl", "style");

		/// <summary>
		/// device_terminal
		/// </summary>
		public static TagGroup term = new TagGroup("term", devi, "device_terminal");

		/// <summary>
		/// camera_track
		/// </summary>
		public static TagGroup trak = new TagGroup("trak", "camera_track");

		/// <summary>
		/// scenario_trigger_volumes_resource
		/// </summary>
		public static TagGroup srtrgr = new TagGroup("trg*", "scenario_trigger_volumes_resource");

		/// <summary>
		/// gui_text_widget_definition
		/// </summary>
		public static TagGroup txt3 = new TagGroup("txt3", "gui_text_widget_definition");

		/// <summary>
		/// dialogue
		/// </summary>
		public static TagGroup udlg = new TagGroup("udlg", "dialogue");

		/// <summary>
		/// sound_cache_file_gestalt
		/// </summary>
		public static TagGroup ugh_ = new TagGroup("ugh!", "sound_cache_file_gestalt");

		/// <summary>
		/// user_interface_sounds_definition
		/// </summary>
		public static TagGroup uise = new TagGroup("uise", "user_interface_sounds_definition");

		/// <summary>
		/// multilingual_unicode_string_list
		/// </summary>
		public static TagGroup unic = new TagGroup("unic", "multilingual_unicode_string_list");

		/// <summary>
		/// vehicle_collection
		/// </summary>
		public static TagGroup vehc = new TagGroup("vehc", "vehicle_collection");

		/// <summary>
		/// vehicle
		/// </summary>
		public static TagGroup vehi = new TagGroup("vehi", unit, "vehicle");

		/// <summary>
		/// vertex_shader
		/// </summary>
		public static TagGroup vtsh = new TagGroup("vtsh", "vertex_shader");

		/// <summary>
		/// gui_widget_animation_collection_definition
		/// </summary>
		public static TagGroup wacd = new TagGroup("wacd", "gui_widget_animation_collection_definition");

		/// <summary>
		/// gui_widget_color_animation_definition
		/// </summary>
		public static TagGroup wclr = new TagGroup("wclr", "gui_widget_color_animation_definition");

		/// <summary>
		/// weapon
		/// </summary>
		public static TagGroup weap = new TagGroup("weap", item, "weapon");

		/// <summary>
		/// game_engine_settings_definition
		/// </summary>
		public static TagGroup wezr = new TagGroup("wezr", "game_engine_settings_definition");

		/// <summary>
		/// gui_widget_font_animation_definition
		/// </summary>
		public static TagGroup wfon = new TagGroup("wfon", "gui_widget_font_animation_definition");

		/// <summary>
		/// gui_widget_animation_definition
		/// </summary>
		public static TagGroup wgan = new TagGroup("wgan", "gui_widget_animation_definition");

		/// <summary>
		/// user_interface_globals_definition
		/// </summary>
		public static TagGroup wgtz = new TagGroup("wgtz", "user_interface_globals_definition");

		/// <summary>
		/// cellular_automata2d
		/// </summary>
		public static TagGroup whip = new TagGroup("whip", "cellular_automata2d");

		/// <summary>
		/// user_interface_shared_globals_definition
		/// </summary>
		public static TagGroup wigl = new TagGroup("wigl", "user_interface_shared_globals_definition");

		/// <summary>
		/// wind
		/// </summary>
		public static TagGroup wind = new TagGroup("wind", "wind");

		/// <summary>
		/// gui_widget_position_animation_definition
		/// </summary>
		public static TagGroup wpos = new TagGroup("wpos", "gui_widget_position_animation_definition");

		/// <summary>
		/// gui_widget_rotation_animation_definition
		/// </summary>
		public static TagGroup wrot = new TagGroup("wrot", "gui_widget_rotation_animation_definition");

		/// <summary>
		/// gui_widget_scale_animation_definition
		/// </summary>
		public static TagGroup wscl = new TagGroup("wscl", "gui_widget_scale_animation_definition");

		/// <summary>
		/// gui_widget_sprite_animation_definition
		/// </summary>
		public static TagGroup wspr = new TagGroup("wspr", "gui_widget_sprite_animation_definition");

		/// <summary>
		/// gui_widget_texture_coordinate_animation_definition
		/// </summary>
		public static TagGroup wtuv = new TagGroup("wtuv", "gui_widget_texture_coordinate_animation_definition");

		/// <summary>
		/// cache_file_resource_gestalt
		/// </summary>
		public static TagGroup zone = new TagGroup("zone", "cache_file_resource_gestalt");
	};
}