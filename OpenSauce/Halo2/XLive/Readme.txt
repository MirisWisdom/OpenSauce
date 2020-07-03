OpenSauce XLive is a wrapper for xlive.dll which is distributed with G4WL games. The goal is to provide explicit overriding of some or all functions (eg, for removing debugger protection...incase a game "isn't working" right)

This library was inspired by the following works of code:
	GTA XLiveless:	http://www.gtamodding.com/index.php?title=XLiveLess
	H2V XLiveless:	http://www.modacity.net/forums/showthread.php?16074-Halo-2-Vista-XLiveless
	
The library allows specific parts to be stubbed out by defining macros. Eg:
	* XLIVE_NO_NETDLL: applies to functions in XLive/XLive.NetDll.inl and XLive/XLive.NetDll.XNet.inl
	* XLIVE_NO_XAPI: applies to functions in XLive/XLive.Xapi.inl
	* XLIVE_NO_XAM: applies to functions in XLive/XLive.Xam.inl
	* XLIVE_NO_XLIVE: applies to functions in XLive/XLive.inl

Currently, not all functions have full implementations avaiable (eg, XLiveUserGetName, where NetDll_WSAStartup does) so all build conditions will yield stubbed results for them