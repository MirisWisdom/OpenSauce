/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo4
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
		/// entity
		/// </summary>
		public static TagGroup ents = new TagGroup("ents", obje, "entity");

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
		/// sound_effect_template
		/// </summary>
		public static TagGroup _fx_ = new TagGroup("<fx>", "sound_effect_template");

		/// <summary>
		/// stereo_system
		/// </summary>
		public static TagGroup BooM = new TagGroup("BooM", "stereo_system");

		//Halo4
		/// <summary>
		/// lightmap_model_globals
		/// </summary>
		public static TagGroup LMMg = new TagGroup("LMMg", "lightmap_model_globals");

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

		//Halo4
		/// <summary>
		/// StreamingZoneSet
		/// </summary>
		public static TagGroup SDzs = new TagGroup("SDzs", "StreamingZoneSet");

		//ODST
		/// <summary>
		/// achievements
		/// </summary>
		public static TagGroup achi = new TagGroup("achi", "achievements");

		/// <summary>
		/// ai_dialogue_globals
		/// </summary>
		public static TagGroup adlg = new TagGroup("adlg", "ai_dialogue_globals");

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

		//Halo4
		/// <summary>
		/// armormod_globals
		/// </summary>
		public static TagGroup armg = new TagGroup("armg", "armormod_globals");

		//Reach
		/// <summary>
		/// atmosphere_globals
		/// </summary>
		public static TagGroup atgf = new TagGroup("atgf", "atmosphere_globals");

		//Halo4
		/// <summary>
		/// authored_light_probe
		/// </summary>
		public static TagGroup aulp = new TagGroup("aulp", "authored_light_probe");

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
		/// crate
		/// </summary>
		public static TagGroup bloc = new TagGroup("bloc", "crate");

		/// <summary>
		/// breakable_surface
		/// </summary>
		public static TagGroup bsdt = new TagGroup("bsdt", "breakable_surface");

		//Halo4
		/// <summary>
		/// custom_app_globals
		/// </summary>
		public static TagGroup capg = new TagGroup("capg", "custom_app_globals");

		/// <summary>
		/// collision_damage
		/// </summary>
		public static TagGroup cddf = new TagGroup("cddf", "collision_damage");

		/// <summary>
		/// camera_fx_settings
		/// </summary>
		public static TagGroup cfxs = new TagGroup("cfxs", "camera_fx_settings");

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
		/// cinematic
		/// </summary>
		public static TagGroup cine = new TagGroup("cine", "cinematic");

		/// <summary>
		/// cinematic_scene
		/// </summary>
		public static TagGroup cisc = new TagGroup("cisc", "cinematic_scene");

		//Halo4
		/// <summary>
		/// cinematic_scene_data
		/// </summary>
		public static TagGroup cisd = new TagGroup("cisd", "cinematic_scene_data");

		//Reach
		/// <summary>
		/// cinematic_transition
		/// </summary>
		public static TagGroup citr = new TagGroup("citr", "cinematic_transition");

		/// <summary>
		/// cloth
		/// </summary>
		public static TagGroup clwd = new TagGroup("clwd", "cloth");

		//Reach Retail
		/// <summary>
		/// camo
		/// </summary>
		public static TagGroup cmoe = new TagGroup("cmoe", "camo");

		//Halo4
		/// <summary>
		/// controller_mapping
		/// </summary>
		public static TagGroup cnmp = new TagGroup("cnmp", "controller_mapping");

		//Halo4
		/// <summary>
		/// commendation_aggregator_list
		/// </summary>
		public static TagGroup coag = new TagGroup("coag", "commendation_aggregator_list");

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

		//Halo4
		/// <summary>
		/// curve_scalar
		/// </summary>
		public static TagGroup crvs = new TagGroup("crvs", "curve_scalar");

		//Reach
		/// <summary>
		/// camera_shake
		/// </summary>
		public static TagGroup csdt = new TagGroup("csdt", "camera_shake");

		/// <summary>
		/// device_control
		/// </summary>
		public static TagGroup ctrl = new TagGroup("ctrl", devi, "device_control");

		//Halo4
		/// <summary>
		/// cui_logic
		/// </summary>
		public static TagGroup culo = new TagGroup("culo", "cui_logic");

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
		/// decorator_set
		/// </summary>
		public static TagGroup dctr = new TagGroup("dctr", "decorator_set");

		/// <summary>
		/// decal_system
		/// </summary>
		public static TagGroup decs = new TagGroup("decs", "decal_system");

		/// <summary>
		/// cellular_automata
		/// </summary>
		public static TagGroup devo = new TagGroup("devo", "cellular_automata");

		/// <summary>
		/// detail_object_collection
		/// </summary>
		public static TagGroup dobc = new TagGroup("dobc", "detail_object_collection");

		//Halo4
		/// <summary>
		/// dependency
		/// </summary>
		public static TagGroup dpnd = new TagGroup("dpnd", "dependency");

		/// <summary>
		/// rasterizer_cache_file_globals
		/// </summary>
		public static TagGroup draw = new TagGroup("draw", "rasterizer_cache_file_globals");

		/// <summary>
		/// damage_response_definition
		/// </summary>
		public static TagGroup drdf = new TagGroup("drdf", "damage_response_definition");

		//Halo4
		/// <summary>
		/// device_dispenser
		/// </summary>
		public static TagGroup dspn = new TagGroup("dspn", devi, "device_dispenser");

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

		//Halo4
		/// <summary>
		/// effect_global_force
		/// </summary>
		public static TagGroup egfd = new TagGroup("egfd", "effect_global_force");

		/// <summary>
		/// equipment
		/// </summary>
		public static TagGroup eqip = new TagGroup("eqip", item, "equipment");

		//Halo4
		/// <summary>
		/// firefight_globals
		/// </summary>
		public static TagGroup ffgd = new TagGroup("ffgd", "firefight_globals");

		//Halo4
		/// <summary>
		/// GameEngineFirefightVariantTag
		/// </summary>
		public static TagGroup ffgt = new TagGroup("ffgt", "GameEngineFirefightVariantTag");

		/// <summary>
		/// flock
		/// </summary>
		public static TagGroup flck = new TagGroup("flck", "flock");

		/// <summary>
		/// fluid_dynamics
		/// </summary>
		public static TagGroup fldy = new TagGroup("fldy", "fluid_dynamics");

		//Reach
		/// <summary>
		/// atmosphere_fog
		/// </summary>
		public static TagGroup fogg = new TagGroup("fogg", "atmosphere_fog");

		/// <summary>
		/// material_effects
		/// </summary>
		public static TagGroup foot = new TagGroup("foot", "material_effects");

		//Halo4
		/// <summary>
		/// forge_globals
		/// </summary>
		public static TagGroup forg = new TagGroup("forg", "forge_globals");

		//ODST
		/// <summary>
		/// formation
		/// </summary>
		public static TagGroup form = new TagGroup("form", "formation");

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

		//Halo4
		/// <summary>
		/// game_globals_grenade_list
		/// </summary>
		public static TagGroup gggl = new TagGroup("gggl", "game_globals_grenade_list");

		//Halo4
		/// <summary>
		/// game_globals_ordnance_list
		/// </summary>
		public static TagGroup ggol = new TagGroup("ggol", "game_globals_ordnance_list");

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
		/// hsc
		/// </summary>
		/// <remarks>scenario_hs_source_file used to use this group tag</remarks>
		public static TagGroup hsc_ = new TagGroup("hsc*", "hsc");

		//Halo4
		/// <summary>
		/// script_container
		/// </summary>
		public static TagGroup hscn = new TagGroup("hscn", "script_container");

		//Halo4
		/// <summary>
		/// script
		/// </summary>
		public static TagGroup hsdt = new TagGroup("hsdt", "script");

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

		//Halo4
		/// <summary>
		/// InfinityUIImages
		/// </summary>
		public static TagGroup iuii = new TagGroup("iuii", "InfinityUIImages");

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

		//Halo4
		/// <summary>
		/// KillCamCameraParamter
		/// </summary>
		public static TagGroup kccd = new TagGroup("kccd", "KillCamCameraParamter");

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

		//Halo4
		/// <summary>
		/// light_cone
		/// </summary>
		public static TagGroup licn = new TagGroup("licn", "light_cone");

		/// <summary>
		/// light
		/// </summary>
		public static TagGroup ligh = new TagGroup("ligh", "light");

		//Reach
		/// <summary>
		/// location_name_globals_definition
		/// </summary>
		public static TagGroup locs = new TagGroup("locs", "location_name_globals_definition");

		//Halo4
		/// <summary>
		/// light_rig
		/// </summary>
		public static TagGroup lrig = new TagGroup("lrig", "light_rig");

		/// <summary>
		/// sound_looping
		/// </summary>
		public static TagGroup lsnd = new TagGroup("lsnd", "sound_looping");

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

		//Halo4
		/// <summary>
		/// material
		/// </summary>
		public static TagGroup mat_ = new TagGroup("mat ", "material");

		/// <summary>
		/// globals
		/// </summary>
		public static TagGroup matg = new TagGroup("matg", "globals");

		//Halo4
		/// <summary>
		/// material_shader
		/// </summary>
		public static TagGroup mats = new TagGroup("mats", "material_shader");

		/// <summary>
		/// ai_mission_dialogue
		/// </summary>
		public static TagGroup mdlg = new TagGroup("mdlg", "ai_mission_dialogue");

		//Halo4
		/// <summary>
		/// model_dissolve_definition
		/// </summary>
		public static TagGroup mdsv = new TagGroup("mdsv", "model_dissolve_definition");

		//Halo4
		/// <summary>
		/// medal_challenge_aggregator_list
		/// </summary>
		public static TagGroup mech = new TagGroup("mech", "medal_challenge_aggregator_list");

		//Halo4
		/// <summary>
		/// medal_commendation_aggregator_list
		/// </summary>
		public static TagGroup meco = new TagGroup("meco", "medal_commendation_aggregator_list");

		/// <summary>
		/// meter
		/// </summary>
		public static TagGroup metr = new TagGroup("metr", "meter");

		/// <summary>
		/// muffin
		/// </summary>
		public static TagGroup mffn = new TagGroup("mffn", "muffin");

		//Halo4
		/// <summary>
		/// MultiplayerEffects
		/// </summary>
		public static TagGroup mgee = new TagGroup("mgee", "MultiplayerEffects");

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

		//Halo4
		/// <summary>
		/// main_menu_voiceover
		/// </summary>
		public static TagGroup mmvo = new TagGroup("mmvo", "main_menu_voiceover");

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

		//Halo4
		/// <summary>
		/// material_shader_bank
		/// </summary>
		public static TagGroup mtsb = new TagGroup("mtsb", "material_shader_bank");

		/// <summary>
		/// multiplayer_globals
		/// </summary>
		public static TagGroup mulg = new TagGroup("mulg", "multiplayer_globals");

		//Reach
		/// <summary>
		/// mux_generator
		/// </summary>
		public static TagGroup muxg = new TagGroup("muxg", "mux_generator");

		//Halo4
		/// <summary>
		/// NarrativeGlobals
		/// </summary>
		public static TagGroup narg = new TagGroup("narg", "NarrativeGlobals");

		/// <summary>
		/// new_cinematic_lighting
		/// </summary>
		public static TagGroup nclt = new TagGroup("nclt", "new_cinematic_lighting");

		//Halo4
		/// <summary>
		/// polyart_asset
		/// </summary>
		public static TagGroup paas = new TagGroup("paas", "polyart_asset");

		//Reach
		/// <summary>
		/// tag_package_manifest
		/// </summary>
		public static TagGroup pach = new TagGroup("pach", "tag_package_manifest");

		//Halo4
		/// <summary>
		/// patch_globals
		/// </summary>
		public static TagGroup patg = new TagGroup("patg", "patch_globals");

		//Halo4
		/// <summary>
		/// pca_animation
		/// </summary>
		public static TagGroup pcaa = new TagGroup("pcaa", "pca_animation");

		//Reach
		/// <summary>
		/// pgcr_enemy_to_category_mapping_definition
		/// </summary>
		public static TagGroup pcec = new TagGroup("pcec", "pgcr_enemy_to_category_mapping_definition");

		//Halo4
		/// <summary>
		/// pgcr_damage_type_image_mapping_definition
		/// </summary>
		public static TagGroup pdti = new TagGroup("pdti", "pgcr_damage_type_image_mapping_definition");

		//ODST
		/// <summary>
		/// particle_emitter_custom_points
		/// </summary>
		public static TagGroup pecp = new TagGroup("pecp", "particle_emitter_custom_points");

		//Halo4
		/// <summary>
		/// player_enlistment_globals_definition
		/// </summary>
		public static TagGroup pegd = new TagGroup("pegd", "player_enlistment_globals_definition");

		/// <summary>
		/// performance_throttles
		/// </summary>
		public static TagGroup perf = new TagGroup("perf", "performance_throttles");

		//Reach Retail
		/// <summary>
		/// performance_template
		/// </summary>
		public static TagGroup pfmc = new TagGroup("pfmc", "performance_template");

		//Halo4
		/// <summary>
		/// pathfinding
		/// </summary>
		public static TagGroup pfnd = new TagGroup("pfnd", "pathfinding");

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

		//Halo4
		/// <summary>
		/// ParticleMan
		/// </summary>
		public static TagGroup pman = new TagGroup("pman", "ParticleMan");

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

		//Halo4
		/// <summary>
		/// portrait_poses_definition
		/// </summary>
		public static TagGroup ppod = new TagGroup("ppod", "portrait_poses_definition");

		//Halo4
		/// <summary>
		/// prefab
		/// </summary>
		public static TagGroup prfb = new TagGroup("prfb", "prefab");

		//Halo4
		/// <summary>
		/// progression
		/// </summary>
		public static TagGroup prog = new TagGroup("prog", "progression");

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

		//Halo4
		/// <summary>
		/// render_model_lightmap_atlas
		/// </summary>
		public static TagGroup rmla = new TagGroup("rmla", rm__, "render_model_lightmap_atlas");

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

		//Halo4
		/// <summary>
		/// shader_waterfall
		/// </summary>
		public static TagGroup rmwf = new TagGroup("rmwf", rm__, "shader_waterfall");

		/// <summary>
		/// render_water_ripple
		/// </summary>
		public static TagGroup rwrd = new TagGroup("rwrd", "render_water_ripple");

		/// <summary>
		/// scenario_lightmap
		/// </summary>
		public static TagGroup sLdT = new TagGroup("sLdT", "scenario_lightmap");

		//Reach
		/// <summary>
		/// spring_acceleration
		/// </summary>
		public static TagGroup sadt = new TagGroup("sadt", "spring_acceleration");

		//Halo4
		/// <summary>
		/// SoundBank
		/// </summary>
		public static TagGroup sbnk = new TagGroup("sbnk", "SoundBank");

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
		/// scenario
		/// </summary>
		public static TagGroup scnr = new TagGroup("scnr", "scenario");

		//Halo4
		/// <summary>
		/// scenario_ordnance_list
		/// </summary>
		public static TagGroup scol = new TagGroup("scol", "scenario_ordnance_list");

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

		//Halo4
		/// <summary>
		/// sound_response
		/// </summary>
		public static TagGroup sgrp = new TagGroup("sgrp", "sound_response");

		/// <summary>
		/// shield_impact
		/// </summary>
		public static TagGroup shit2 = new TagGroup("shit", "shield_impact");

		//Halo4
		/// <summary>
		/// shared_variables
		/// </summary>
		public static TagGroup shvr = new TagGroup("shvr", "shared_variables");

		//Halo4
		/// <summary>
		/// self_illumination
		/// </summary>
		public static TagGroup sict = new TagGroup("sict", "self_illumination");

		//Reach
		/// <summary>
		/// simulated_input
		/// </summary>
		public static TagGroup sidt = new TagGroup("sidt", "simulated_input");

		//Halo4
		/// <summary>
		/// SuppressedIncident
		/// </summary>
		public static TagGroup sigd = new TagGroup("sigd", "SuppressedIncident");

		//Reach
		/// <summary>
		/// simulation_interpolation
		/// </summary>
		public static TagGroup siin = new TagGroup("siin", "simulation_interpolation");

		/// <summary>
		/// text_value_pair_definition
		/// </summary>
		public static TagGroup sily = new TagGroup("sily", "text_value_pair_definition");

		//Halo4
		/// <summary>
		/// sound_incident_response
		/// </summary>
		public static TagGroup sirg = new TagGroup("sirg", "sound_incident_response");

		//Reach Retail
		/// <summary>
		/// scenario_interpolator
		/// </summary>
		public static TagGroup sirp = new TagGroup("sirp", "scenario_interpolator");

		//Halo4
		/// <summary>
		/// silent_assist_globals
		/// </summary>
		public static TagGroup slag = new TagGroup("slag", "silent_assist_globals");

		/// <summary>
		/// shared_cache_file_layout
		/// </summary>
		public static TagGroup smap = new TagGroup("smap", "shared_cache_file_layout");

		//ODST
		/// <summary>
		/// survival_mode_globals
		/// </summary>
		public static TagGroup smdt = new TagGroup("smdt", "survival_mode_globals");

		//Halo4
		/// <summary>
		/// structure_meta
		/// </summary>
		public static TagGroup smet = new TagGroup("smet", "structure_meta");

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

		//Halo4
		/// <summary>
		/// sound_old
		/// </summary>
		public static TagGroup sndo = new TagGroup("sndo", "sound_old");

		//Halo4
		/// <summary>
		/// sound2
		/// </summary>
		public static TagGroup sndx = new TagGroup("sndx", "sound2");

		/// <summary>
		/// sound_mix
		/// </summary>
		public static TagGroup snmx = new TagGroup("snmx", "sound_mix");

		/// <summary>
		/// sound_dialogue_constants
		/// </summary>
		public static TagGroup spk_ = new TagGroup("spk!", "sound_dialogue_constants");

		//Halo4
		/// <summary>
		/// spawner
		/// </summary>
		public static TagGroup spnr = new TagGroup("spnr", ents, "spawner");

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

		//Halo4
		/// <summary>
		/// SpawnSettings
		/// </summary>
		public static TagGroup ssdf = new TagGroup("ssdf", "SpawnSettings");

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

		//Halo4
		/// <summary>
		/// tracer_system
		/// </summary>
		public static TagGroup trac = new TagGroup("trac", "tracer_system");

		/// <summary>
		/// camera_track
		/// </summary>
		public static TagGroup trak = new TagGroup("trak", "camera_track");

		//ODST
		/// <summary>
		/// test_tag
		/// </summary>
		public static TagGroup ttag = new TagGroup("ttag", "test_tag");

		/// <summary>
		/// dialogue
		/// </summary>
		public static TagGroup udlg = new TagGroup("udlg", "dialogue");

		/// <summary>
		/// sound_cache_file_gestalt
		/// </summary>
		public static TagGroup ugh_ = new TagGroup("ugh!", "sound_cache_file_gestalt");

		//Halo4
		/// <summary>
		/// user_interface_hud_globals_definition
		/// </summary>
		public static TagGroup uihg = new TagGroup("uihg", "user_interface_hud_globals_definition");

		/// <summary>
		/// user_interface_sounds_definition
		/// </summary>
		public static TagGroup uise = new TagGroup("uise", "user_interface_sounds_definition");

		//Halo4
		/// <summary>
		/// style_sheet_list
		/// </summary>
		public static TagGroup uiss = new TagGroup("uiss", "style_sheet_list");

		/// <summary>
		/// multilingual_unicode_string_list
		/// </summary>
		public static TagGroup unic = new TagGroup("unic", "multilingual_unicode_string_list");

		//ODST
		/// <summary>
		/// tag_template_unit_test
		/// </summary>
		public static TagGroup uttt = new TagGroup("uttt", "tag_template_unit_test");

		//Halo4
		/// <summary>
		/// vectorart_asset
		/// </summary>
		public static TagGroup vaas = new TagGroup("vaas", "vectorart_asset");

		//Halo4
		/// <summary>
		/// vector_hud_definition
		/// </summary>
		public static TagGroup vchd = new TagGroup("vchd", "vector_hud_definition");

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

		//Reach
		/// <summary>
		/// wave_template
		/// </summary>
		public static TagGroup wave = new TagGroup("wave", "wave_template");

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
		/// weather_globals
		/// </summary>
		public static TagGroup wxcg = new TagGroup("wxcg", "weather_globals");

		/// <summary>
		/// cache_file_resource_gestalt
		/// </summary>
		public static TagGroup zone = new TagGroup("zone", "cache_file_resource_gestalt");
	};
}