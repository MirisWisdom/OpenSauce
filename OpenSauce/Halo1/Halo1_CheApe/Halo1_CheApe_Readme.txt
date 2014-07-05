= OpenSauce for the Halo Editing Kit =

== Change Log ==
=== Version 2014 ===
  * UPGRADE		We now use Visual Studio 2013 instead of 2008 for development
  * ADD			tag system was rewritten in OS code, providing better diagnostic messages and tag utilities. However, the engine's tag saving code is still used
  * ADD			tag system now supports tag fields of the _field_string kind with a custom string length (stock is fixed to 31-character strings)
  * ADD			Custom event handlers for Guerilla's Source Control and Xsync menu items, for use in a future system (they still don't do anything currently)
  * ADD			Introduced "tag-load-test" command to Tool.
  * CHANGE		Tool now supports specifying the editor profile (-editorProfile:) on the command line. The first string that doesn't begin with a dash, '-', is assumed to be the command verb
  * CHANGE		Settings system is entirely new. Old settings files will be ignored and won't be automatically upgraded.
  * CHANGE		unit boarding definitions moved from the project_yellow_globals_cv tag and put directly in the unit tag

=== Version 3.1.1 ===
  * ADD			project_yellow_globals_cv tag containing both boarding and unit infection tag blocks
  * ADD			model_animations tag features two new unit_seat_animation types for seat boarding and ejection

=== Version 3.1 ===
  * PATCH		Weapon's magazine's magazine-objects block will no longer crash when you try to add more than 2 elements. Uses the equipment field for the block name now too.
  * PATCH		"The %s shader %s cannot be used in the environment" will now output the correct shader type in all cases
  * PATCH		project_yellow_globals tag features new flag for forcing the game to use 'stun jumping penalty' instead of 'turning penalty' in the engine's biped jump code
  * PATCH		project_yellow tag features new flag for forcing game update logic to ignore if players can see various game entities (flag only acknowledged in mainmenu scenarios)
  * ADD			project_yellow_globals_cv tag; unit_infections data were also moved to here
  * ADD			Mini-dump support for tool and sapien builds (no guerilla support)
  * ADD			Increased the number of grenade types to 4
  * FIX			Increased the maximum buffer size for model vertex and index data. Tool crashes (buffer overflow) if you actually hit this limit (Gearbox's fault, not ours!)

=== Version 3.0.2 ===
  * PATCH		Made a work-around for the HaloPC engine bug where model nodes over 43 were rendering wrong.
  * ADD			Increased the maximum amount of files tool will process during an import (this means tool won't stop importing after 500 animations)
  * CHANGE		Increased maximum animation seat elements from 32 to 64 in light of the increased max animations
  * CHANGE		Increased maximum animations from 500 to 2048. Crazy shit. I dare someone to use all 2048. Can I get a Challenge Accepted?
  * FIX			Corrected the project_yellow_scripting.xml data to match the code (should no longer get script mismatch messages with new builds)
  * FIX			Patched some tool code that is ran when reimporting an animation tag to use the increased limit (wasn't patched before, resulting in buffer overflows)

=== Version 3.0.1 ===
  * ADD			More verbose debug output to compile-shader-postprocess
  * ADD			More sanity checks and messages to build-cache-fil-ex
  * CHANGE		Moved autogeneration of yelo script definitions to when building tags for runtime
  * FIX	[18]	Yelo scripting blocks add *all* definitions			http://code.google.com/p/open-sauce/issues/detail?id=18


== Known Issues ==
