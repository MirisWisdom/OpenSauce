/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.HaloReach
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

		//Reach
		/// <summary>
		/// lightmapper_globals
		/// </summary>
		public static TagGroup LMgS = new TagGroup("LMgS", "lightmapper_globals");

		//ODST
		/// <summary>
		/// scenario_lightmap_bsp_data
		/// </summary>
		public static TagGroup Lbsp = new TagGroup("Lbsp", "scenario_lightmap_bsp_data");

		//Reach
		/// <summary>
		/// lightning_system
		/// </summary>
		public static TagGroup ZZAP = new TagGroup("ZZAP", "lightning_system");

		//ODST
		/// <summary>
		/// achievements
		/// </summary>
		public static TagGroup achi = new TagGroup("achi", "achievements");

		/// <summary>
		/// ai_dialogue_globals
		/// </summary>
		public static TagGroup adlg = new TagGroup("adlg", "ai_dialogue_globals");

		/// <summary>
		/// scenario_ai_resource
		/// </summary>
		public static TagGroup srai = new TagGroup("ai**", "scenario_ai_resource");

		//ODST
		/// <summary>
		/// ai_globals
		/// </summary>
		public static TagGroup aigl = new TagGroup("aigl", "ai_globals");

		//Reach Retail
		/// <summary>
		/// airstrike
		/// </summary>
		public static TagGroup airs = new TagGroup("airs", "airstrike");

		/// <summary>
		/// antenna
		/// </summary>
		public static TagGroup ant_ = new TagGroup("ant!", "antenna");

		//Reach
		/// <summary>
		/// atmosphere_globals
		/// </summary>
		public static TagGroup atgf = new TagGroup("atgf", "atmosphere_globals");

		//Reach Retail
		/// <summary>
		/// avatar_awards
		/// </summary>
		public static TagGroup avat = new TagGroup("avat", "avatar_awards");

		//Reach Retail
		/// <summary>
		/// big_battle_creature
		/// </summary>
		public static TagGroup bbcr = new TagGroup("bbcr", "big_battle_creature");

		//Reach
		/// <summary>
		/// death_program_selector
		/// </summary>
		public static TagGroup bdpd = new TagGroup("bdpd", "death_program_selector");

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

		//Reach
		/// <summary>
		/// challenge_globals_definition
		/// </summary>
		public static TagGroup chdg = new TagGroup("chdg", "challenge_globals_definition");

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

		//Reach
		/// <summary>
		/// cinematic_transition
		/// </summary>
		public static TagGroup citr = new TagGroup("citr", "cinematic_transition");

		/// <summary>
		/// scenario_cluster_data_resource
		/// </summary>
		public static TagGroup srclut = new TagGroup("clu*", "scenario_cluster_data_resource");

		/// <summary>
		/// cloth
		/// </summary>
		public static TagGroup clwd = new TagGroup("clwd", "cloth");

		//Reach Retail
		/// <summary>
		/// camo
		/// </summary>
		public static TagGroup cmoe = new TagGroup("cmoe", "camo");

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

		//Reach
		/// <summary>
		/// commendation_globals_definition
		/// </summary>
		public static TagGroup comg = new TagGroup("comg", "commendation_globals_definition");

		//Reach
		/// <summary>
		/// communication_sounds
		/// </summary>
		public static TagGroup coms = new TagGroup("coms", "communication_sounds");

		//Reach
		/// <summary>
		/// cookie_globals_definition
		/// </summary>
		public static TagGroup cook = new TagGroup("cook", "cookie_globals_definition");

		//Reach
		/// <summary>
		/// coop_spawning_globals_definition
		/// </summary>
		public static TagGroup coop = new TagGroup("coop", "coop_spawning_globals_definition");

		//Reach
		/// <summary>
		/// cheap_particle_emitter
		/// </summary>
		public static TagGroup cpem = new TagGroup("cpem", "cheap_particle_emitter");

		//Reach
		/// <summary>
		/// cookie_purchase_globals
		/// </summary>
		public static TagGroup cpgd = new TagGroup("cpgd", "cookie_purchase_globals");

		//Reach
		/// <summary>
		/// cheap_particle_type_library
		/// </summary>
		public static TagGroup cptl = new TagGroup("cptl", "cheap_particle_type_library");

		/// <summary>
		/// creature
		/// </summary>
		public static TagGroup crea = new TagGroup("crea", obje, "creature");

		// Removed: crte	cortana_effect_definition
		//Reach
		/// <summary>
		/// camera_shake
		/// </summary>
		public static TagGroup csdt = new TagGroup("csdt", "camera_shake");

		/// <summary>
		/// device_control
		/// </summary>
		public static TagGroup ctrl = new TagGroup("ctrl", devi, "device_control");

		/// <summary>
		/// scenario_cubemap_resource
		/// </summary>
		public static TagGroup srcube = new TagGroup("cub*", "scenario_cubemap_resource");

		//Reach
		/// <summary>
		/// cui_screen
		/// </summary>
		public static TagGroup cusc = new TagGroup("cusc", "cui_screen");

		//Reach
		/// <summary>
		/// cui_static_data
		/// </summary>
		public static TagGroup cust = new TagGroup("cust", "cui_static_data");

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

		//Reach
		/// <summary>
		/// scenario_dumplings_resource
		/// </summary>
		public static TagGroup srdmpr = new TagGroup("dmp*", "scenario_dumplings_resource");

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

		//Reach
		/// <summary>
		/// particle_emitter_boat_hull_shape
		/// </summary>
		public static TagGroup ebhd = new TagGroup("ebhd", "particle_emitter_boat_hull_shape");

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

		//Removed: 'fog '	planar_fog
		//Reach
		/// <summary>
		/// atmosphere_fog
		/// </summary>
		public static TagGroup fogg = new TagGroup("fogg", "atmosphere_fog");

		/// <summary>
		/// material_effects
		/// </summary>
		public static TagGroup foot = new TagGroup("foot", "material_effects");

		//ODST
		/// <summary>
		/// formation
		/// </summary>
		public static TagGroup form = new TagGroup("form", "formation");

		//Removed: 'fpch'	patchy_fog
		//Removed: 'frag'	fragment

		//Reach
		/// <summary>
		/// frame_event_list
		/// </summary>
		public static TagGroup frms = new TagGroup("frms", "frame_event_list");

		//Reach
		/// <summary>
		/// fx_test
		/// </summary>
		public static TagGroup fxtt = new TagGroup("fxtt", "fx_test");

		//Reach
		/// <summary>
		/// game_completion_rewards_globals
		/// </summary>
		public static TagGroup gcrg = new TagGroup("gcrg", "game_completion_rewards_globals");

		//Reach
		/// <summary>
		/// game_engine_globals
		/// </summary>
		public static TagGroup gegl = new TagGroup("gegl", "game_engine_globals");

		/// <summary>
		/// giant
		/// </summary>
		public static TagGroup gint = new TagGroup("gint", unit, "giant");

		//Reach
		/// <summary>
		/// cheap_light
		/// </summary>
		/// <remarks>A la Halo 2's 'gldf' aka 'chocolate_mountain'</remarks>
		public static TagGroup gldf = new TagGroup("gldf", "cheap_light");

		/// <summary>
		/// global_pixel_shader
		/// </summary>
		public static TagGroup glps = new TagGroup("glps", "global_pixel_shader");

		/// <summary>
		/// global_vertex_shader
		/// </summary>
		public static TagGroup glvs = new TagGroup("glvs", "global_vertex_shader");

		//Reach
		/// <summary>
		/// game_medal_globals
		/// </summary>
		public static TagGroup gmeg = new TagGroup("gmeg", "game_medal_globals");

		/// <summary>
		/// multiplayer_variant_settings_interface_definition
		/// </summary>
		public static TagGroup goof = new TagGroup("goof", "multiplayer_variant_settings_interface_definition");

		//Reach
		/// <summary>
		/// global_cache_file_pixel_shaders
		/// </summary>
		public static TagGroup gpix = new TagGroup("gpix", "global_cache_file_pixel_shaders");

		//Reach
		/// <summary>
		/// game_performance_throttle
		/// </summary>
		public static TagGroup gptd = new TagGroup("gptd", "game_performance_throttle");

		//Reach
		/// <summary>
		/// grounded_friction
		/// </summary>
		public static TagGroup grfr = new TagGroup("grfr", "grounded_friction");

		/// <summary>
		/// gui_group_widget_definition
		/// </summary>
		public static TagGroup grup = new TagGroup("grup", "gui_group_widget_definition");

		//Reach
		/// <summary>
		/// havok_collision_filter
		/// </summary>
		public static TagGroup hcfd = new TagGroup("hcfd", "havok_collision_filter");

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

		//Reach
		/// <summary>
		/// incident_global_properties_definition
		/// </summary>
		public static TagGroup igpd = new TagGroup("igpd", "incident_global_properties_definition");

		//Reach
		/// <summary>
		/// instance_imposter_definition
		/// </summary>
		public static TagGroup iimz = new TagGroup("iimz", "instance_imposter_definition");

		//Reach
		/// <summary>
		/// imposter_model
		/// </summary>
		public static TagGroup impo = new TagGroup("impo", "imposter_model");

		//Reach
		/// <summary>
		/// incident_globals_definition
		/// </summary>
		public static TagGroup ingd = new TagGroup("ingd", "incident_globals_definition");

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

		//Reach Retail
		/// <summary>
		/// load_screen
		/// </summary>
		public static TagGroup ldsc = new TagGroup("ldsc", "load_screen");

		/// <summary>
		/// lens_flare
		/// </summary>
		public static TagGroup lens = new TagGroup("lens", "lens_flare");

		//Reach
		/// <summary>
		/// loadout_globals_definition
		/// </summary>
		public static TagGroup lgtd = new TagGroup("lgtd", "loadout_globals_definition");

		/// <summary>
		/// light
		/// </summary>
		public static TagGroup ligh = new TagGroup("ligh", "light");

		//Reach
		/// <summary>
		/// location_name_globals_definition
		/// </summary>
		public static TagGroup locs = new TagGroup("locs", "location_name_globals_definition");

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

		//Reach
		/// <summary>
		/// megalogamengine_sounds
		/// </summary>
		public static TagGroup mgls = new TagGroup("mgls", "megalogamengine_sounds");

		//Reach
		/// <summary>
		/// emblem_library
		/// </summary>
		public static TagGroup mlib = new TagGroup("mlib", "emblem_library");

		/// <summary>
		/// render_model
		/// </summary>
		public static TagGroup mode = new TagGroup("mode", "render_model");

		//Reach
		/// <summary>
		/// multiplayer_object_type_list
		/// </summary>
		public static TagGroup motl = new TagGroup("motl", "multiplayer_object_type_list");

		/// <summary>
		/// multiplayer_scenario_description
		/// </summary>
		public static TagGroup mply = new TagGroup("mply", "multiplayer_scenario_description");

		//Reach
		/// <summary>
		/// megalo_string_id_table
		/// </summary>
		public static TagGroup msit = new TagGroup("msit", "megalo_string_id_table");

		/// <summary>
		/// multiplayer_globals
		/// </summary>
		public static TagGroup mulg = new TagGroup("mulg", "multiplayer_globals");

		//Reach
		/// <summary>
		/// mux_generator
		/// </summary>
		public static TagGroup muxg = new TagGroup("muxg", "mux_generator");

		/// <summary>
		/// new_cinematic_lighting
		/// </summary>
		public static TagGroup nclt = new TagGroup("nclt", "new_cinematic_lighting");

		//Reach
		/// <summary>
		/// tag_package_manifest
		/// </summary>
		public static TagGroup pach = new TagGroup("pach", "tag_package_manifest");

		//Reach
		/// <summary>
		/// pgcr_enemy_to_category_mapping_definition
		/// </summary>
		public static TagGroup pcec = new TagGroup("pcec", "pgcr_enemy_to_category_mapping_definition");

		//ODST
		/// <summary>
		/// particle_emitter_custom_points
		/// </summary>
		public static TagGroup pecp = new TagGroup("pecp", "particle_emitter_custom_points");

		/// <summary>
		/// performance_throttles
		/// </summary>
		public static TagGroup perf = new TagGroup("perf", "performance_throttles");

		//Reach Retail
		/// <summary>
		/// performance_template
		/// </summary>
		public static TagGroup pfmc = new TagGroup("pfmc", "performance_template");

		//Reach
		/// <summary>
		/// planar_fog_parameters
		/// </summary>
		public static TagGroup pfpt = new TagGroup("pfpt", "planar_fog_parameters");

		//Reach
		/// <summary>
		/// player_grade_globals_definition
		/// </summary>
		public static TagGroup pggd = new TagGroup("pggd", "player_grade_globals_definition");

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

		//Reach
		/// <summary>
		/// player_model_customization_globals
		/// </summary>
		public static TagGroup pmcg = new TagGroup("pmcg", "player_model_customization_globals");

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

		//Reach
		/// <summary>
		/// scenario_performances_resource
		/// </summary>
		public static TagGroup srprfr = new TagGroup("prf*", "scenario_performances_resource");

		/// <summary>
		/// projectile
		/// </summary>
		public static TagGroup proj = new TagGroup("proj", obje, "projectile");

		/// <summary>
		/// particle
		/// </summary>
		public static TagGroup prt3 = new TagGroup("prt3", "particle");

		//Reach
		/// <summary>
		/// rain_definition
		/// </summary>
		public static TagGroup rain = new TagGroup("rain", "rain_definition");

		/// <summary>
		/// rasterizer_globals
		/// </summary>
		public static TagGroup rasg = new TagGroup("rasg", "rasterizer_globals");

		/// <summary>
		/// shader_beam
		/// </summary>
		public static TagGroup rmb_ = new TagGroup("rmb ", rm__, "shader_beam");

		//Reach
		/// <summary>
		/// rumble
		/// </summary>
		public static TagGroup rmbl = new TagGroup("rmbl", "rumble");

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

		//Reach
		/// <summary>
		/// shader_fur_stencil
		/// </summary>
		public static TagGroup rmfs = new TagGroup("rmfs", rm__, "shader_fur_stencil");

		//Reach
		/// <summary>
		/// shader_fur
		/// </summary>
		public static TagGroup rmfu = new TagGroup("rmfu", rm__, "shader_fur");

		//Reach
		/// <summary>
		/// shader_glass
		/// </summary>
		public static TagGroup rmgl = new TagGroup("rmgl", rm__, "shader_glass");

		/// <summary>
		/// shader_halogram
		/// </summary>
		public static TagGroup rmhg = new TagGroup("rmhg", rm__, "shader_halogram");

		/// <summary>
		/// shader_light_volume
		/// </summary>
		public static TagGroup rmlv = new TagGroup("rmlv", rm__, "shader_light_volume");

		//Reach
		/// <summary>
		/// shader_mux_material
		/// </summary>
		public static TagGroup rmmm = new TagGroup("rmmm", rm__, "shader_mux_material");

		//Reach
		/// <summary>
		/// shader_mux
		/// </summary>
		public static TagGroup rmmx = new TagGroup("rmmx", rm__, "shader_mux");

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

		//ODST
		/// <summary>
		/// shader_screen
		/// </summary>
		public static TagGroup rmss = new TagGroup("rmss", rm__, "shader_screen");

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

		//Reach
		/// <summary>
		/// spring_acceleration
		/// </summary>
		public static TagGroup sadt = new TagGroup("sadt", "spring_acceleration");

		/// <summary>
		/// scenario_structure_bsp
		/// </summary>
		public static TagGroup sbsp = new TagGroup("sbsp", "scenario_structure_bsp");

		/// <summary>
		/// scenery
		/// </summary>
		public static TagGroup scen = new TagGroup("scen", obje, "scenery");

		//Reach
		/// <summary>
		/// sound_combiner
		/// </summary>
		public static TagGroup scmb = new TagGroup("scmb", "sound_combiner");

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

		//Reach Retail
		/// <summary>
		/// scenario_required_resource
		/// </summary>
		public static TagGroup sdzg = new TagGroup("sdzg", "scenario_required_resource");

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

		//Reach
		/// <summary>
		/// simulated_input
		/// </summary>
		public static TagGroup sidt = new TagGroup("sidt", "simulated_input");

		//Reach
		/// <summary>
		/// simulation_interpolation
		/// </summary>
		public static TagGroup siin = new TagGroup("siin", "simulation_interpolation");

		/// <summary>
		/// text_value_pair_definition
		/// </summary>
		public static TagGroup sily = new TagGroup("sily", "text_value_pair_definition");

		//Reach Retail
		/// <summary>
		/// scenario_interpolator
		/// </summary>
		public static TagGroup sirp = new TagGroup("sirp", "scenario_interpolator");

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

		//ODST
		/// <summary>
		/// survival_mode_globals
		/// </summary>
		public static TagGroup smdt = new TagGroup("smdt", "survival_mode_globals");

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

		//ODST
		/// <summary>
		/// squad_template
		/// </summary>
		public static TagGroup sqtm = new TagGroup("sqtm", "squad_template");

		//Reach
		/// <summary>
		/// sound_radio_settings
		/// </summary>
		public static TagGroup srad = new TagGroup("srad", "sound_radio_settings");

		//Reach
		/// <summary>
		/// ssao_definition
		/// </summary>
		public static TagGroup ssao = new TagGroup("ssao", "ssao_definition");

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

		//ODST
		/// <summary>
		/// test_tag
		/// </summary>
		public static TagGroup ttag = new TagGroup("ttag", "test_tag");

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

		//ODST
		/// <summary>
		/// tag_template_unit_test
		/// </summary>
		public static TagGroup uttt = new TagGroup("uttt", "tag_template_unit_test");

		/// <summary>
		/// vehicle
		/// </summary>
		public static TagGroup vehi = new TagGroup("vehi", unit, "vehicle");

		//ODST
		/// <summary>
		/// vision_mode
		/// </summary>
		public static TagGroup vmdx = new TagGroup("vmdx", "vision_mode");

		//Reach Retail
		/// <summary>
		/// variant_globals
		/// </summary>
		public static TagGroup vtgl = new TagGroup("vtgl", "variant_globals");

		/// <summary>
		/// vertex_shader
		/// </summary>
		public static TagGroup vtsh = new TagGroup("vtsh", "vertex_shader");

		/// <summary>
		/// gui_widget_animation_collection_definition
		/// </summary>
		public static TagGroup wacd = new TagGroup("wacd", "gui_widget_animation_collection_definition");

		//Reach
		/// <summary>
		/// chud_widget_animation_data_template
		/// </summary>
		public static TagGroup wadt = new TagGroup("wadt", "chud_widget_animation_data_template");

		//Reach
		/// <summary>
		/// wave_template
		/// </summary>
		public static TagGroup wave = new TagGroup("wave", "wave_template");

		/// <summary>
		/// gui_widget_color_animation_definition
		/// </summary>
		public static TagGroup wclr = new TagGroup("wclr", "gui_widget_color_animation_definition");

		//Reach
		/// <summary>
		/// chud_widget_datasource_template
		/// </summary>
		public static TagGroup wdst = new TagGroup("wdst", "chud_widget_datasource_template");

		/// <summary>
		/// weapon
		/// </summary>
		public static TagGroup weap = new TagGroup("weap", item, "weapon");

		//Reach
		/// <summary>
		/// scenario_wetness_bsp_data
		/// </summary>
		public static TagGroup wetn = new TagGroup("wetn", "scenario_wetness_bsp_data");

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

		//Reach
		/// <summary>
		/// water_physics_drag_properties
		/// </summary>
		public static TagGroup wpdp = new TagGroup("wpdp", "water_physics_drag_properties");

		//Reach
		/// <summary>
		/// chud_widget_placement_data_template
		/// </summary>
		public static TagGroup wpdt = new TagGroup("wpdt", "chud_widget_placement_data_template");

		/// <summary>
		/// gui_widget_position_animation_definition
		/// </summary>
		public static TagGroup wpos = new TagGroup("wpos", "gui_widget_position_animation_definition");

		//Reach
		/// <summary>
		/// chud_widget_render_data_template
		/// </summary>
		public static TagGroup wrdt = new TagGroup("wrdt", "chud_widget_render_data_template");

		/// <summary>
		/// gui_widget_rotation_animation_definition
		/// </summary>
		public static TagGroup wrot = new TagGroup("wrot", "gui_widget_rotation_animation_definition");

		/// <summary>
		/// gui_widget_scale_animation_definition
		/// </summary>
		public static TagGroup wscl = new TagGroup("wscl", "gui_widget_scale_animation_definition");

		//Reach
		/// <summary>
		/// chud_widget_state_data_template
		/// </summary>
		public static TagGroup wsdt = new TagGroup("wsdt", "chud_widget_state_data_template");

		/// <summary>
		/// gui_widget_sprite_animation_definition
		/// </summary>
		public static TagGroup wspr = new TagGroup("wspr", "gui_widget_sprite_animation_definition");

		/// <summary>
		/// gui_widget_texture_coordinate_animation_definition
		/// </summary>
		public static TagGroup wtuv = new TagGroup("wtuv", "gui_widget_texture_coordinate_animation_definition");

		//Reach
		/// <summary>
		/// weather_globals
		/// </summary>
		public static TagGroup wxcg = new TagGroup("wxcg", "weather_globals");

		/// <summary>
		/// cache_file_resource_gestalt
		/// </summary>
		public static TagGroup zone = new TagGroup("zone", "cache_file_resource_gestalt");
	};
}