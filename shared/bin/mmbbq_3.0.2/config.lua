--- mmBBQ configuration for all targets
--
-- It contains LUA syntax, so you may also
-- add additional code directly in this file.
--
-- This file basically defines TARGETS configuration and an optional APIKEY.
-- It may also be used to perform any operation.
--
module("config", package.seeall);


----------------------
-- TARGET CONFIGURATION
----------------------
-- A host process where mmBBQ injects is called a `target`. You can define any injection target you like. 
-- When running `START.bat` the dll starts a temporary Lua VM to determine which targets are defined.
-- Targets defined here are available for the injector. Has to be gobally defined like that:
--	_G.TARGETS = {
--		{	                                                   
--          ["name"]  = "np",                                  -- the targets shorthandle name. used as global `TARGET` and `PREFIX`.
--			["exe"]   = "notepad.exe",                         -- the executables filename
--			["title"] = "Notepad",	                           -- OPTIONAL arbitrary, but should be the main windows title
--			["ver"]   = "6.1",                                 -- OPTIONAL an arbritrary string describing the targets version number
--			["md5"]   = "0123456789abcdef0123456789abcdef",    -- OPTIONAL md5sum of process executable. checked before attaching
--			["lua"]   = "notepad.lua",                         -- OPTIONAL target bootsrap file - called with `dofile()`
--			["cfg"]   = "config_np",                           -- OPTIONAL target config module - called with `require()`
--		},
--		
--		-- ...                                                 -- add as many targets as you like
--	}
_G.TARGETS = {	
	{
		["name"]  = "haloce",
		["exe"]   = "haloce.exe",
		["title"] = "Halo Custom Edition",
		["lua"]   = "HaloCE/haloce_target.lua",
		["cfg"]   = "HaloCE/config_haloce",
	}
}

-- only continue if a TARGET is known (already injected -> called from mmbbq.lua)
if not _G.TARGET then return end

--[[ PLACE YOUR LAZY STUFF HERE 
all stuff below this line is executed for any target. 
If you are doing a quick hack for only one target anything can be added here.
You dont need a special config.lua or target.lua ]]--

--require("common/asmcall");
--asmcall.cdecl(getProcAddress("user32", "MessageBoxA"), 0, "Hello World!", "Title", 0)

