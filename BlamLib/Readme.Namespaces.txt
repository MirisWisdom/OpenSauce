

-- Namspaces --

Blam
	Each game supported by the library has it's own namespace inside 'Blam'. Here is where all the code related to the game's specific engine version can typically be found. There are 
	only two major exceptions to this: scripting & the tag system. However, when it comes to cache files, tag definitions, library interface implementations, etc, you can find them here.

CheApe
	"CheApe" is the code name given to extensions written to work on top of official game tools. Eg, there are CheApe extensions for Halo Custom Edition and Halo 2 Vista. This namespace 
	contains all the generic interfaces and utilities used by implementing game systems (eg, Halo1 or Halo2).

Debug
	This shit is bananas. Old dirty nasty bananas. This namespace hasn't changed much from 2005. Back then I wasn't too savvy with .NET's System.Diag namespace or else I probably could 
	have came up with a far better and extensible Debug related system. Oh, and back then they didn't have Code Contracts! Three cheers for .NET 4.0! Boooo @ Assert.cs!

Documentation
	Before I implemented new code or whatnot, I'd typically drop a text file in here detailing my intentions with a new system or idea. Most of it didn't apply to the public release 
	of BlamLib so all you'll really find here are documentation on some of the XML formats used in BlamLib. During BlamLib development I've never bothered to get down and dirty with 
	XSD or else I probably could have created .xsd files to document formats and then also verify those formats using some XML tools.

Games
	I store game specific resource definitions here. Probably should have actually made this its own assembly. That or I could have just loaded resource files directly from disk...
	but I didn't like that idea back then and still don't. Tough cookies. Get some milk.

IO
	Stuff dealing with streams and endian orders can be found here. I wish I could have included a new EndianStream system I developed for some other library as it's far better than 
	this old junk but as with most new stuff I've moved onto using .NET 4.0 only...and I wanted BlamLib to be usable with VS2008 and .NET 2.0 as I know not all people have or can get 
	some of the newer flashy Visual Studios.
	There's also a somewhat nifty Xml stream reader I prototyped in here. This one only supports reading but that should satisfy most BlamLib usages. The final version supported more 
	xml streaming features and not to mention writing. I really do wish I had the time to do the 3rd or 4th rewrite of this library. Then it could probably even support future Bungie games too

Managers
	If something can be managed, it probably has a definition or an interface declaration here. GameManager.cs could probably be removed from BlamLib as it's not used by any code last 
	I checked but maybe someone will want to do something with it. Who knows. For the curious, I was going to use it for loading game resource definitions from various locations while 
	keeping the actual location (assembly, disk, user profile, etc) hidden from the resource code.

Render
	No, BlamLib doesn't support rendering. At one point this was going to be possible but I considered such a thing a "convenience" feature, not a needed feature or system. Instead, 
	this namespace now only houses the interface used for dealing with vertex formats of the various games. Most Blam based games store their vertex data/streams in a compressed format, 
	so Render provides utilities that work with LowLevel.XNA for de/compressing floating point data. Halo1, 2, 3, etc. I've taken the time to research their internal vertex stream 
	descriptions to try and create a "standard" format which I could express in XML.
	In the future we hope to add COLLADA file support, which will probably appear under this namespace.

Scripting
	Once upon a time...I meant to finish this system. Instead, I took the time I had at hand to work on standard interfaces for tag/cache data and various resource data, etc. In the end, 
	I didn't have the time to keep this up to date with changes and abstractions I needed to create in order to make this support any Blam engine's scripting system (and there have been 
	some pretty big changes since the days of Halo 1).

TagInterface
	Probably the oldest living Blam-specific system in BlamLib. The stuff in here has gone thru about 3 or 4 rewrites or designs. It still isn't pretty but it works. It can even handle 
	some simple tag versioning which was introduced in Halo 2. It can't handle tag versions which define tag-structs which were later added (see: Halo2.unit_boarding_melee_struct).
	Some will notice that I have an old system in place for defining editor related data via code attributes. I stopped working on this because I realized I did not want to fucking store 
	editor definitions with the code definitions.

Util
	As the name suggests, this namespace contains misc code which supports the rest of the library.