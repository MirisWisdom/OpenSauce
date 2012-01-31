= OpenSauce for Halo Custom Edition v1.09 =

NOTE: Always check our wiki FIRST! As documentation there is the most up-to-date
	http://code.google.com/p/open-sauce/w/list


== Overview ==

We have a FAQ that addresses questions and common mistakes users make. Check it first before reporting an issue.
	http://code.google.com/p/open-sauce/wiki/Doc_Halo1_FAQ

We have an Issues system that we use to track *problems* (nothing else; we're NOT taking feature requests). Use it to report problems. It's the only way we'll accept problem reports.
	ALL Issues	http://code.google.com/p/open-sauce/issues/list?can=1 (includes issues that have been fixed/closed)
	Open Issues	http://code.google.com/p/open-sauce/issues/list


== System Requirements ==


== Change Log ==
=== Version 3.0.1 ===
  * REMOVED		Vehicle Views from F7 menu (it's unimplemented)
  * ADD			Implemented project_yellow's gravity_scale override field
  * ADD			Prompts user with a MessageBox when a map contains mismatching yelo script defintiions with the current build of OS
  * FIX			Fixed potential issue when a proxy calling OS stores the device pointter in a local variable
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
