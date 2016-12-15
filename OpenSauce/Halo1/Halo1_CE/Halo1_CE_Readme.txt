= OpenSauce for Halo Custom Edition v1.10 =

== Change Log ==
=== Version 2015 ===
  * REMOVE		scenario_faux_zones have been depreceated
  * PATCH		Non-multiplayer maps will no longer even show up in the map list
  * CHANGE		OS is now targeted at 1.10
  * CHANGE		Installer no-longer locks installation to Runtime/HEK registry keys, if the selected directory isn't right it will tell the user
  * CHANGE		OpenSauce is now compiled with VS2013, instead of VS2008
  * CHANGE		Settings system is entirely new. Old settings files will be ignored and won't be automatically upgraded.
  * CHANGE		mod-sets now Just Work. There's no more need for a ModSets.xml, so long as the mod's data files exist in a proper maps folder, OS will load them
  * CHANGE		OS will now process files in the "maps\" folder in the user's profile directory
  * CHANGE		HUD scaling now respects screen anchors and scale multiplayer elements correctly
  * CHANGE		Installer now needs .NET 4 and Windows Installer 4.0
  * CHANGE		Installer now checks for CPU support of SSE2
  * CHANGE		Installer now forces the user to manually uninstall builds older than 4.0.0
  * CHANGE		Map downloading will now try to save the maps to your My Games/Halo CE/maps directory
  * ADD			Added actor variant transforms
  * ADD			Added settings to ignore FOV changes on the main menu and in cutscenes
  * ADD			Added GUI for changing OpenSauce settings (Uses GWEN)
  * ADD			Added soft particles to the engine
  * ADD			Shadows can now be enabled on scenery and devices
  * ADD			Added diffuse directional lightmaps to the engine
  * ADD			Added experimental specular directional lightmaps to the engine
  * ADD			Added bsp modifiers for changing lightmaps and skies using HS (replaces scenario_faux_zones)
  * ADD			Added rasterizer globals for toggling shader extensions
  * ADD			Added new script functions: weapon_data_magazine_get\set_integer
  * ADD			Added functionality to allow full crash dumps to be created
  * ADD			Added crash report creation with a minidump, screenshot, core dump, etc. (Uses CrashRpt)
  * ADD			Installer now includes the VS120 runtime
  * ADD			Added unit boarding
  * ADD			Added entering biped unit seats
  * ADD			disableVehicleRemapper setting, along with a vehicle_remapper_enabled script function, to turn off the PC's vehicle respawning nonsense
  * FIX			Fixed incorrect object data real mapping of "maximum/current_vitality", replaced with "maximum/current_health" and "maximum/current_shield"
  * FIX			Fixed map download client not trying master servers if the dedicated server can't serve the map
  * FIX			Fixed map download server not handling certain map names
  * FIX [69]	Object_data_get_real logic will no longer cause exceptions
  * FIX			Fixed detail objects not being drawn when using os shader extensions
  * FIX			Fixed crash when downloading encrypted maps
  * FIX			Fixed yelo maps not being added to the map list
  * FIX [72]	Fixed >2 grenade types breaking tool lightmaps
  * FIX [90]	Fixed damage anchors not scaling correctly
  * FIX [80]	Fixed Ctrl+C being seen as a crash in tool

=== Version 3.1 ===
  * PATCH		project_yellow_globals tag features new flag for forcing the game to use 'stun jumping penalty' instead of 'turning penalty' in the engine's biped jump code
  * PATCH		non-multiplayer maps will no longer be selectable from the MP map list
  * CHANGE		Game hooking is now done via dinput8.dll instead of d3d9.dll
  * CHANGE		Removed XFire restriction now that we use dinput8.dll
  * CHANGE		Changed the server list to list all servers regardless of their version
  * CHANGE		Made OpensauceIDE an optional installation
  * CHANGE		Made XP SP3 a minimum requirement when installing
  * CHANGE		Installation now requires .NET 3.5
  * CHANGE		Increased hardcoded 'k_game_state_allocation_maximum_size_for_yelo' by 0x970 bytes
  * CHANGE		Increased hardcoded 'k_runtime_data_max_values_count' to a total of 64 (up from 32)
  * CHANGE		Made the GBuffer require SM 3.0
  * CHANGE		Clients now use "initc.txt" for their init.txt file
  * ADD			Added flag to use specular colour map alpha as a specular lighitng exponent mask
  * ADD			Make screenshots get saved to unique directorys to stop them from being overwritten
  * ADD			Upgraded the max number of particles active/rendered at once
  * ADD			Upgraded the max number of objects rendered at once
  * ADD			Ability for mainmenu maps to have animating objects
  * ADD			Added an xml setting for completely turning off game update checks
  * ADD			Added an in-game map download system to the client
  * ADD			Added a HTTP server to the OS dedi
  * ADD			Added a map server component to the OS dedi
  * ADD			Added an installation validation component to the installer
  * ADD			Added file patching component to the installer
  * ADD			Added new setting for forcing OS to use .yelo files first when searching for .map files (off by default)
  * ADD			Script functions for bit operations (AND, OR, etc) and converting hex string to an integer
  * ADD			Script function for playing a .bik movie just like how the engine plays credits.bik, etc (local games only)
  * ADD			New tag/script system for displaying ui widgets to the user
  * ADD			Added system for adding custom dedicated server logging events
  * ADD			Runtime vector3d values (part of the game state)
  * ADD			Added a mini crashdump creator for easier debugging
  * ADD			Added command line argument to disable all OS graphics (-no_os_gfx)
  * ADD			Added command line argument to disable special OS game state allocations (-no_os_gamestate)
  * ADD			Added Visual C++ 9.0 runtime to the installer
  * FIX			Fixed Halo not loading when using -use*
  * FIX			Fixed crash in shader model settings when extension element is missing
  * FIX [39]	Fixed GBuffer issues with stretched models and incorrect velocity when using custom models with high bone counts
  * FIX			Fixed shader models' specular lighting being dulled by detail maps when using detail before reflection
  * FIX			Fixed an exception on load when the device doesn't support hardware rendering (Intel integrated)
  * FIX			Fixed a bug where a non-existant map (of any kind) would cause the 'missing <mapname>.yelo' exception, when the stock game already had code to handle such cases
  * FIX			project_yellow tag's "prohibit_multiteam_vehicles" flag should work as expected now
  * FIX			Multi-team vehicles should no longer allow 'double loading'
  * FIX			Fix for instant respawns when using the OS dedi
  * FIX			Fixed the -path argument being ignored by OS
  * FIX			Fixed the roaming/appdata folders made by OS not being removed on uninstall
  * FIX [56]	Fixed the team index offset for MP teams for the GBuffer. MP teams now start at 10, not 9. This is a breaking fix, but I'm not aware of any major MP map releases that used this specific part of postprocessing

=== Version 3.0.3 ===
  * N/A			Nothing you all need to concern yourselves with. Go play Halo.

=== Version 3.0.2 ===
  * PATCH		Made a work-around for the HaloPC engine bug where model nodes over 43 were rendering wrong.
  * ADD			Verbose script definition mismatch messages when developer_mode is >= 4
  * FIX			Script definition mismatch error message erroneously prompting
  * FIX			Fixed tag-based gravity settings problem

=== Version 3.0.1 ===
  * REMOVED		Vehicle Views from F7 menu (it's unimplemented)
  * ADD			Implemented project_yellow's gravity_scale override field
  * ADD			Prompts user with a MessageBox when a map contains mismatching yelo script defintiions with the current build of OS
  * ADD			Bloom toggle added to the users settings
  * ADD			Shader extension toggle added to the user settings to completely disable it if needed
  * CHANGE		Motion blur is now disabled by default
  * FIX			Fixed potential issue when a proxy calling OS stores the device pointer in a local variable
  * FIX			Fixed possible issue where _global_yelo could reference invalid memory
  * FIX [19]	Unregistered mod-sets crash game w/o warning		http://code.google.com/p/open-sauce/issues/detail?id=19
  * FIX [17]	External PP effects problem w/ single model mask	http://code.google.com/p/open-sauce/issues/detail?id=17
  * FIX [14]	Weapon Views not saving correctly					http://code.google.com/p/open-sauce/issues/detail?id=14 (Thanks jellymasterer!)
  * FIX [13]	OS_haloceded crashes on startup						http://code.google.com/p/open-sauce/issues/detail?id=13
  * FIX [11]	Black screen when DisableAlphaRenderTargets is set	http://code.google.com/p/open-sauce/issues/detail?id=11 (Thanks as7raios!)
  * FIX [8]		HUD scaling causes problems with UI					http://code.google.com/p/open-sauce/issues/detail?id=8 (Thanks SeanAero!)
  * FIX [5]		HUD scaling has chatbox problem w/ hi-res			http://code.google.com/p/open-sauce/issues/detail?id=5 (Thanks SeanAero!)


== Known Issues ==
  * Running Steam with Halo in general sometimes causes the Function keys to not work. We can't fix this, it's on Valve's end. http://code.google.com/p/open-sauce/issues/detail?id=6&can=1
