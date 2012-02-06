= OpenSauce for Halo Custom Edition v1.09 =
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
Fault Module Name: d3d9.dll
Fault Module Version: 3.0.0.0
Fault Module Timestamp: 4f16444a
Exception Code: c0000005
Exception Offset: 000126f6
OS Version: 6.1.7601.2.1.0.256.49
Locale ID: 1033

It is also useful for us to know your system specifications such as your graphics card, OS version, etc.

NOTE: The issues system is for problems and bugs ONLY. If you are unsure of how to use OS, please consult our wiki and/or create a post on Halomods to get community assistance. We are also NOT taking feature requests in any way.

== Change Log ==
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
  * XFire crashes OS. We can't fix this, it's on XFire's end. http://code.google.com/p/open-sauce/issues/detail?id=4&can=1
  * Running Steam with Halo in general causes the Function keys to not work. We can't fix this, it's on Valve's end. http://code.google.com/p/open-sauce/issues/detail?id=6&can=1
  * [21] It has been reported that for some users Halo will inexplicably close when opened with no exception or other error dialog appearing. If this occurs for you, completely disable the shader extension in your user settings.