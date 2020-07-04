/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

namespace OpenSauceIDE.CheApeInterface
{
	partial class UnlockH1
	{
		static class Constants
		{
			public static readonly byte[] LoadSymbolTableErrorMsgNullCode = {
				0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
			};

			#region Guerilla Fixes
			public static string[] fix_inertial_matrix_block_fields = new string[] {
				"-xy     zz+xx    -yz",
				"-zx      -yz    xx+yy",
			};

			public static PlatformData[] FixReadonlyFieldCode = new PlatformData[] {
				new PlatformData(
				BlamLib.Util.ByteStringToArray("B000C20400"), null, null
				),
				new PlatformData(
				BlamLib.Util.ByteStringToArray("B000C3"), null, null
				),
			};

			#region ReadonlyFieldNames
			/// <summary>
			/// Strings to replace the strings found at the addresses represented in the respected 
			/// indexes of <see cref="AddressOf.ReadonlyFieldNames"/>
			/// </summary>
			public static string[] ReadonlyFieldNames = new string[] {
				"flags",
				"node list checksum",
				"super-high detail node count:nodes",
				"high detail node count:nodes",
				"medium detail node count:nodes",
				"low detail node count:nodes",
				"super-low detail node count:nodes",
				"base map u-scale#0 defaults to 1",
				"base map v-scale#0 defaults to 1",
				"markers",
				"name^",
				"magic identifier",
				"instances",
				"region index",
				"permutation index",
				"node index",
				"translation",
				"rotation",
				"nodes",
				"next sibling node index",
				"first child node index",
				"parent node index",
				"default translation",
				"default rotation",
				"node distance from parent",
				"regions",
				"permutations",
				"super low",
				"low",
				"medium",
				"high",
				"super high",
				"geometries",
				"parts",
				"shader index",
				"centroid primary node",
				"centroid secondary node",
				"centroid primary weight",
				"centroid secondary weight",
				"centroid",
				"uncompressed vertices",
				"position",
				"normal",
				"binormal",
				"tangent",
				"texture coords",
				"node0 index",
				"node1 index",
				"node0 weight",
				"node1 weight",
				"compressed vertices",
				"normal[11.11.10-bit]",
				"binormal[11.11.10-bit]",
				"tangent[11.11.10-bit]",
				"texture coordinate u[16-bit]",
				"texture coordinate v[16-bit]",
				"node0 index(x3)",
				"node1 index(x3)",
				"node0 weight[16-bit]",
				"triangles",
				"vertex0 index",
				"vertex1 index",
				"vertex2 index",
				"shaders",
				"base vector",
				"vector range",
				"animations",
				"name^",
				"type",
				"frame count",
				"frame size",
				"frame info type",
				"node count",
				"next animation",
//				"frame info*",
//				"node trans. flag data*",

//				"node rotation flag data*",

//				"node scale flag data*",

//				"offset to compressed data:bytes*",
//				"default data*",
//				"frame data*",

//				"color plate width*:pixels",
//				"color plate height*:pixels",
//				"compressed color plate data*",
//				"processed pixel data*",
				"sprite spacing",
				"sequences",
				"first bitmap index",
				"bitmap count",
				"sprites",
				"bitmap index",
				"left",
				"right",
				"top",
				"bottom",
				"registration point",
				"bitmaps",
//				"signature*",
				"width:pixels",
				"height:pixels",
				"depth:pixels#depth is 1 for 2D textures and cube maps",
				"type#determines bitmap 'geometry'",
				"format#determines how pixels are represented internally",
				"mipmap count",
				"pixels offset",
				"character",
				"bitmap width",
				"bitmap height",
				"hardware character index",
				"center of mass",
				"inertial matrix and inverse",
				"model node",
				"mass",
				"density",
				"predicted resources",
				"sample rate",
				"encoding",
				"compression",
				"name^#the name of the imported pitch range directory",
				"actual permutation count",
				"name^#name of the file from which this sample was imported",
				"next permutation index",

				"materials",
				"region",
				"parent node",
				"next sibling node",
				"first child node",
				"bsps",
				"bsp3d nodes",
				"plane",
				"back child",
				"front child",
				"planes",
				"leaves",
				"bsp2d reference count",
				"first bsp2d reference",
				"bsp2d references",
				"bsp2d node",
				"bsp2d nodes",
				"left child",
				"right child",
				"surfaces",
				"first edge",
				"breakable surface",
				"material",
				"edges",
				"start vertex",
				"end vertex",
				"forward edge",
				"reverse edge",
				"left surface",
				"right surface",
				"vertices",
				"point",
				"collision materials",
				"shader",
				"collision bsp",
				"world bounds x",
				"world bounds y",
				"world bounds z",
				"cluster",
				"surface reference count",
				"surface references",
				"leaf surfaces",
				"surface",
				"node",
				"lightmaps",
				"bitmap",
				"shader permutation",
				"surface count",
				"ambient color",
				"distant light count",
				"distant light 0 color",
				"distant light 0 direction",
				"distant light 1 color",
				"distant light 1 direction",
				"reflection tint",
				"shadow vector",
				"shadow color",
				"vertex buffers",
				"count",
				"offset",
				"lens flares",
				"lens flare markers",
				"direction i-component",
				"direction j-component",
				"direction k-component",
				"lens flare index",
				"clusters",
				"sky",
				"fog",
				"background sound",
				"sound environment",
				"weather",
				"subclusters",
				"surface indices",
				"index",
				"first lens flare marker index",
				"lens flare marker count",
				"mirrors",
				"portals",
				"portal",
//				"cluster data*",
				"cluster portals",
				"front cluster",
				"back cluster",
				"plane index",
				"bounding radius",
				"breakable surfaces",
				"radius",
				"collision surface index",
				"fog planes",
				"front region",
				"fog regions",
				"fog palette",
				"weather palette",
				"weather polyhedra",
				"bounding sphere center",
				"bounding sphere radius",
				"pathfinding surfaces",
				"data",
				"pathfinding edges",
				"midpoint",
				"background sound palette",
				"sound environment palette",
//				"sound PAS data*",
				//"name*",
//				"detail objects*",
				//"*",
//				"runtime decals*",
				"faces",
				"vertex",
				"portal indices",
				"portal index",
				"back leaf index",
				"front leaf index",
				"object names",
				"DON'T TOUCH THIS",

				"version",
//				"raw animation data*",
				"unit control data version",
				"length of animation:ticks",
//				"recorded animation event stream*",
				"yaw[-127,127]",
				"pitch[-127,127]",
				"surface index",

				"scripts",
				"script type",
				"return type",
//				"root expression index*",
				"globals",
//				"initialization expression index*",
				"references",
				"reference^",
				"source files",
				"maximum sprite extent:pixels",

				"motion sensor scale [DON'T TOUCH EVER]",
//				"text data*",
				"message elements",
				"messages",
				"start index into text blob",
				"start index of message block",
				"panel count",
				"device type",
				"device id",
				"profile",
			};
			#endregion
			#endregion

			#region Che Ape
			/// <summary>DLL name strings for each game tool</summary>
			public static PlatformData CheApeDllString = new PlatformData(
				// "\0CheApeDLLG.dll\0"
				BlamLib.Util.ByteStringToArray("00436865417065444C4C472E646C6C00"),
				// "\0CheApeDLLT.dll\0"
				BlamLib.Util.ByteStringToArray("00436865417065444C4C542E646C6C00"),
				// "\0CheApeDLLS.dll\0"
				BlamLib.Util.ByteStringToArray("00436865417065444C4C532E646C6C00")
				);

			/// <summary>Initializer code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.CheApeFunction1"/>
			/// push	<see cref="AddressOf.CheApeDllString"/>
			/// call	ds:[<see cref="AddressOf.CheApeLoadLibrary"/>]
			/// jmp		__SEH_prolog
			/// </remarks>
			public static PlatformData CheApeFunction1 = new PlatformData(
				// push	0x93A098		// Push our dll string
				// call	ds:[0xF5582C]	// call LoadLibrary
				// jmp	0x758534		// goto __SEH_prolog
				BlamLib.Util.ByteStringToArray("6898A09300" + "FF152C58F500" + "E9146A3300"),

				// push	0x6235B0		// Push our dll string
				// call	ds:[0x6040E8]	// call LoadLibrary
				// jmp	0x5F4BDC		// goto __SEH_prolog
				BlamLib.Util.ByteStringToArray("68B0356200" + "FF15E8406000" + "E91C691C00"),

				// push	0x910DBA		// Push our dll string
				// call	ds:[0x1082FCC]	// call LoadLibrary
				// jmp	0x406825		// goto __SEH_prolog
				BlamLib.Util.ByteStringToArray("68BA0D9100" + "FF15CC2F0801" + "E995C8F0FF")
				);

			/// <summary>Initializer's hook code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.CheApeFunction2"/>
			/// call	<see cref="AddressOf.CheApeFunction1"/>
			/// </remarks>
			public static PlatformData CheApeFunction2 = new PlatformData(
				// call	0x421B10		// Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E8A3F5CCFF"),

				// call	0x42E2B0		// Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E835C5E3FF"),

				// call	0x4F9F80		// Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E8E085DAFF")
				);
			#endregion
		};
	};
}