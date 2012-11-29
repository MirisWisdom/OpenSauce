= OpenSauce for the Halo Editing Kit =
== Overview ==
Thank you for installing OpenSauce for Halo Custom Edition! You now have access to a plethora of features and extensions added to Halo Custom Edition which you can read all about on our Google Code wiki:
	http://code.google.com/p/open-sauce/wiki/

We have a WIP FAQ available for some of the more common questions that have been asked:
	http://code.google.com/p/open-sauce/wiki/Doc_Halo1_FAQ

It is still early days for OpenSauce so the way ahead may have a few bumps for some, but we hope they are minimal and that you enjoy it all the same!

== System Requirements ==
These requirements are mainly guesswork as we do not have the resources for broad testing on multiple hardware configurations.

Operating System: Microsoft Windows XP SP3, Microsoft Windows Vista SP2, Microsoft Windows 7 SP1
Computer/Processor: 1GHz processor.
DirectX: DirectX® 9.0c or later.
Memory: 512 megabytes (MB) of RAM.
Video card: 256 MB with shader model 2.a or 2.b, shader model 3.0 recommended.

You should not expect your current hardware to run Halo CE with OS at the same level of performace you had before. What OS adds does not come for free. If you feel that you would rather sacrifice some features for better performance, you can turn off various additions in the user settings file (see our Google code wiki for details).

== Installation ==
If you have cut and pasted or have otherwise moved your Halo CE installation from it's original install location, you will not have the option to install the OS runtime. This is due to your registry entries for Halo CE having incorrect paths.

If you have problems with OpenSauce not running follow these steps first.

1. Install Halo Custom Edition using the official installer.
2. Update Halo CE to version 1.09 by either initialising the update in-game, or running haloupdate.exe in the Halo CE root directory.
3. Install OpenSauce for Halo CE.

== Having problems? ==
If you are having problems when running the game, such as an exception, a black screen or other game breaking issue please search our issues page on the OS Google code website to see if it has already been reported and if not, create a new issue to inform us of the problem. This is the ONLY bug reporting system we are using. We will not look at issues sent to us via forum posts, private messages, e-mails, etc. This is so that we can track the issues appropriately.
	ALL Issues	http://code.google.com/p/open-sauce/issues/list?can=1 (includes issues that have been fixed/closed)
	Open Issues	http://code.google.com/p/open-sauce/issues/list

In the event of you having an exception, do NOT assume that you are having the same issue as somebody else unless you know what you are looking for in the exception details to determine whether it is an already reported issue. If in doubt, create a new issue and we will see if it is a duplicate and handle it accordingly.

When adding any issue to the database, add as much information as you can to help us debug the problem. For exceptions, please click the View Details button on the Send Report window that appears and copy the exception data into the issues description. That data will look something like this:

Problem Event Name: APPCRASH
Application Name: haloce.exe
Application Version: 1.0.9.620
Application Timestamp: 4ac7faf1
Fault Module Name: dinput8.dll
Fault Module Version: 3.0.0.0
Fault Module Timestamp: 4f16444a
Exception Code: c0000005
Exception Offset: 000126f6
OS Version: 6.1.7601.2.1.0.256.49
Locale ID: 1033

It is also useful for us to know your system specifications such as your graphics card, OS version, etc.

NOTE: The issues system is for problems and bugs ONLY. If you are unsure of how to use OS, please consult our wiki and/or create a post on Halomods to get community assistance. We are also NOT taking feature requests in any way.

== Change Log ==
=== Version 3.1 (Unreleased) ===
  * PATCH		Weapon's magazine's magazine-objects block will no longer crash when you try to add more than 2 elements. Uses the equipment field for the block name now too.
  * PATCH		"The %s shader %s cannot be used in the environment" will now output the correct shader type in all cases
  * PATCH		project_yellow_globals tag features new flag for forcing the game to use 'stun jumping penalty' instead of 'turning penalty' in the engine's biped jump code
  * ADD			Mini-dump support for tool and sapien builds (no guerilla support)
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
