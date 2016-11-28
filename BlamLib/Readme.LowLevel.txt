
With the inclusion of xma_parse code into LowLevel, LowLevel now uses some std C++ library code.

This is an issue with Release builds due to a *cough*stupid fucking asshole never-got-fixed*cough* compiler bug on MS's part.

If you try to build LowLevel as release now, you get the following:

1>LINK : error LNK2034: metadata inconsistent with COFF symbol table: symbol '?c_str@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$FQBEPBDXZ' (060000C9) has inconsistent metadata with (0A000130) in Precompile.obj
1>Precompile.obj : error LNK2020: unresolved token (0A000130) "public: char const * __thiscall std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >::c_str(void)const " (?c_str@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$FQBEPBDXZ)

I've tried multiple code setups and project configurations to try and work around this. Nothing works. Internet doesn't provide any solutions either.

So with that said, LowLevel now builds as Debug, even when Release is selected.

Fuck you and your compiler "support" MS.

Also note that LowLevel has some harcoded paths for some compiler actions (i.e., nothing bound at runtime). See Readme.HardCodedPaths.txt for more.