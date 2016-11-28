/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

namespace OpenSauceIDE.CheApeInterface
{
	partial class UnlockH1
	{
		static class AddressOf
		{
			/// <summary>Address of the null string in each game tool</summary>
			public static PlatformAddress NullString = new PlatformAddress
				(0x894B11, 0x604452, 0x8FD492);

			/// <summary>Address of the code which bitches about not having the memory map file (*beta.map)</summary>
			public static PlatformAddress LoadSymbolTableErrorMsg = new PlatformAddress
				(0x42127F, 0x42D44B, 0x41810B);

			#region Guerilla fixes
			public static PlatformAddress[] fix_inertial_matrix_block_fields = new PlatformAddress[] {
				new PlatformAddress(0x8D715C, 0, 0), // [1] " -xy    zz+xx    -yz"
				new PlatformAddress(0x8D7144, 0, 0), // [2] " -zx     -yz    xx+yy"
			};

			public static PlatformAddress[] FixReadonlyFieldCode = new PlatformAddress[] {
				new PlatformAddress(0x41DC00, 0, 0),
				new PlatformAddress(0x41DC40, 0, 0),
			};

			public static PlatformAddress[] FixReadonlyTags = new PlatformAddress[] { // Replace the dword at these addr with NullString
				new PlatformAddress(0x41CDEF, 0, 0), // ui_widget_definition
				new PlatformAddress(0x41CE01, 0, 0), // tag_collection
				new PlatformAddress(0x41CE13, 0, 0), // ui_widget_collection
			};

			/// <summary>Address of the "field" string in the game tool</summary>
			public static PlatformAddress FieldString = new PlatformAddress // "field"
				(0x89C528, 0, 0);

			/// <summary>Address of tag_fields which have blank names (and thus won't appear in the editor)</summary>
			public static PlatformAddress[] NullFields = new PlatformAddress[] {
				new PlatformAddress(0x9D6D24, 0, 0), // LongInteger
				new PlatformAddress(0x9D6D54, 0, 0), // LongInteger
				new PlatformAddress(0x9D6D84, 0, 0), // LongInteger
				new PlatformAddress(0x9CC2E4, 0, 0), // Real
			};

			#region ReadonlyBlankFields
			/// <summary>Address of tag_fields which have blank names (and thus won't appear in the editor) and are also readonly</summary>
			public static PlatformAddress[] ReadonlyBlankFields = new PlatformAddress[] { // *, all these are for the bsp's global detail object bs
				new PlatformAddress(0x9C2E88, 0, 0), // ShortInteger
				new PlatformAddress(0x9C2E94, 0, 0), // ShortInteger
				new PlatformAddress(0x9C2EA0, 0, 0), // ShortInteger
				new PlatformAddress(0x9C2EAC, 0, 0), // ShortInteger
				new PlatformAddress(0x9C2EB8, 0, 0), // LongInteger
				new PlatformAddress(0x9C2EC4, 0, 0), // LongInteger
				new PlatformAddress(0x9C2ED0, 0, 0), // LongInteger
				new PlatformAddress(0x9C2F20, 0, 0), // CharInteger
				new PlatformAddress(0x9C2F2C, 0, 0), // CharInteger
				new PlatformAddress(0x9C2F38, 0, 0), // CharInteger
				new PlatformAddress(0x9C2F44, 0, 0), // CharInteger
				new PlatformAddress(0x9C2F50, 0, 0), // ShortInteger
				new PlatformAddress(0x9C2F94, 0, 0), // ShortInteger
				new PlatformAddress(0x9C2FD8, 0, 0), // Real
				new PlatformAddress(0x9C2FE4, 0, 0), // Real
				new PlatformAddress(0x9C2FF0, 0, 0), // Real
				new PlatformAddress(0x9C2FFC, 0, 0), // Real
			};
			#endregion

			#region ReadonlyFieldNames
			/// <summary>Address of all field name strings which are read-only which we want to make editable</summary>
			public static PlatformAddress[] ReadonlyFieldNames = new PlatformAddress[] {
				new PlatformAddress(0x8A2B10, 0x0, 0x0), // flags*
				new PlatformAddress(0x8DBAA0, 0x0, 0x0), // node list checksum*
				new PlatformAddress(0x8DC014, 0x0, 0x0), // super-high detail node count*:nodes
				new PlatformAddress(0x8DBFF4, 0x0, 0x0), // high detail node count*:nodes
				new PlatformAddress(0x8DBFD4, 0x0, 0x0), // medium detail node count*:nodes
				new PlatformAddress(0x8DBFB4, 0x0, 0x0), // low detail node count*:nodes
				new PlatformAddress(0x8DBF90, 0x0, 0x0), // super-low detail node count*:nodes
				new PlatformAddress(0x8DBF6C, 0x0, 0x0), // base map u-scale*#0 defaults to 1
				new PlatformAddress(0x8DBF48, 0x0, 0x0), // base map v-scale*#0 defaults to 1
				new PlatformAddress(0x8C8DC4, 0x0, 0x0), // markers*
				new PlatformAddress(0x8CB0A8, 0x0, 0x0), // name^*
				new PlatformAddress(0x8DC110, 0x0, 0x0), // magic identifier*
				new PlatformAddress(0x8DC104, 0x0, 0x0), // instances*
				new PlatformAddress(0x8DC14C, 0x0, 0x0), // region index*
				new PlatformAddress(0x8DC138, 0x0, 0x0), // permutation index*
				new PlatformAddress(0x8DC2BC, 0x0, 0x0), // node index*
				new PlatformAddress(0x8DC244, 0x0, 0x0), // translation*
				new PlatformAddress(0x8DC254, 0x0, 0x0), // rotation*
				new PlatformAddress(0x8C8F00, 0x0, 0x0), // nodes*
				new PlatformAddress(0x8DB3FC, 0x0, 0x0), // next sibling node index*
				new PlatformAddress(0x8DB3E4, 0x0, 0x0), // first child node index*
				new PlatformAddress(0x8DB3D0, 0x0, 0x0), // parent node index*
				new PlatformAddress(0x8DC290, 0x0, 0x0), // default translation*
				new PlatformAddress(0x8DC27C, 0x0, 0x0), // default rotation*
				new PlatformAddress(0x8DC260, 0x0, 0x0), // node distance from parent*
				new PlatformAddress(0x8CA97C, 0x0, 0x0), // regions*
				new PlatformAddress(0x8DC190, 0x0, 0x0), // permutations*
				new PlatformAddress(0x8DC1FC, 0x0, 0x0), // super low*
				new PlatformAddress(0x8DC1F4, 0x0, 0x0), // low*
				new PlatformAddress(0x8DC1EC, 0x0, 0x0), // medium*
				new PlatformAddress(0x8DC1E4, 0x0, 0x0), // high*
				new PlatformAddress(0x8DC1D8, 0x0, 0x0), // super high*
				new PlatformAddress(0x8DBF3C, 0x0, 0x0), // geometries*
				new PlatformAddress(0x8DC308, 0x0, 0x0), // parts*
				new PlatformAddress(0x8DC454, 0x0, 0x0), // shader index*
				new PlatformAddress(0x8DC3D8, 0x0, 0x0), // centroid primary node*
				new PlatformAddress(0x8DC3BC, 0x0, 0x0), // centroid secondary node*
				new PlatformAddress(0x8DC3A0, 0x0, 0x0), // centroid primary weight*
				new PlatformAddress(0x8DC384, 0x0, 0x0), // centroid secondary weight*
				new PlatformAddress(0x8C99C0, 0x0, 0x0), // centroid*
				new PlatformAddress(0x8DC36C, 0x0, 0x0), // uncompressed vertices*
				new PlatformAddress(0x8C77C8, 0x0, 0x0), // position*
				new PlatformAddress(0x8DC64C, 0x0, 0x0), // normal*
				new PlatformAddress(0x8DC640, 0x0, 0x0), // binormal*
				new PlatformAddress(0x8DC634, 0x0, 0x0), // tangent*
				new PlatformAddress(0x8DC624, 0x0, 0x0), // texture coords*
				new PlatformAddress(0x8DC614, 0x0, 0x0), // node0 index*
				new PlatformAddress(0x8DC604, 0x0, 0x0), // node1 index*
				new PlatformAddress(0x8DC5F4, 0x0, 0x0), // node0 weight*
				new PlatformAddress(0x8DC5E4, 0x0, 0x0), // node1 weight*
				new PlatformAddress(0x8DC354, 0x0, 0x0), // compressed vertices*
				new PlatformAddress(0x8DC5AC, 0x0, 0x0), // normal[11.11.10-bit]*
				new PlatformAddress(0x8DC594, 0x0, 0x0), // binormal[11.11.10-bit]*
				new PlatformAddress(0x8DC57C, 0x0, 0x0), // tangent[11.11.10-bit]*
				new PlatformAddress(0x8DC55C, 0x0, 0x0), // texture coordinate u[16-bit]*
				new PlatformAddress(0x8DC53C, 0x0, 0x0), // texture coordinate v[16-bit]*
				new PlatformAddress(0x8DC528, 0x0, 0x0), // node0 index(x3)*
				new PlatformAddress(0x8DC514, 0x0, 0x0), // node1 index(x3)*
				new PlatformAddress(0x8DC4FC, 0x0, 0x0), // node0 weight[16-bit]*
				new PlatformAddress(0x8DC348, 0x0, 0x0), // triangles*
				new PlatformAddress(0x8DC4D4, 0x0, 0x0), // vertex0 index*
				new PlatformAddress(0x8DC4C4, 0x0, 0x0), // vertex1 index*
				new PlatformAddress(0x8DC4B4, 0x0, 0x0), // vertex2 index*
				new PlatformAddress(0x8DBF30, 0x0, 0x0), // shaders*
				new PlatformAddress(0x8DB3AC, 0x0, 0x0), // base vector*
				new PlatformAddress(0x8DB39C, 0x0, 0x0), // vector range*
				new PlatformAddress(0x8DB2BC, 0x0, 0x0), // animations*
				new PlatformAddress(0x8CB434, 0x0, 0x0), // name*^
				new PlatformAddress(0x8B4670, 0x0, 0x0), // type*
				new PlatformAddress(0x8DBAD4, 0x0, 0x0), // frame count*
				new PlatformAddress(0x8DBAC8, 0x0, 0x0), // frame size*
				new PlatformAddress(0x8DBAB4, 0x0, 0x0), // frame info type*
				new PlatformAddress(0x8DBA94, 0x0, 0x0), // node count*
				new PlatformAddress(0x8DBA48, 0x0, 0x0), // next animation*
//				new PlatformAddress(0x8DB9F8, 0x0, 0x0), // frame info*
//				new PlatformAddress(0x8DB9E0, 0x0, 0x0), // node trans. flag data*

//				new PlatformAddress(0x8DB9C4, 0x0, 0x0), // node rotation flag data*

//				new PlatformAddress(0x8DB9AC, 0x0, 0x0), // node scale flag data*

//				new PlatformAddress(0x8DB988, 0x0, 0x0), // offset to compressed data:bytes*
//				new PlatformAddress(0x8DB978, 0x0, 0x0), // default data*
//				new PlatformAddress(0x8DB96C, 0x0, 0x0), // frame data*

//				new PlatformAddress(0x8D96F0, 0x0, 0x0), // color plate width*:pixels
//				new PlatformAddress(0x8D96D4, 0x0, 0x0), // color plate height*:pixels
//				new PlatformAddress(0x8D96B4, 0x0, 0x0), // compressed color plate data*
//				new PlatformAddress(0x8D9654, 0x0, 0x0), // processed pixel data*
				new PlatformAddress(0x8D9524, 0x0, 0x0), // sprite spacing*
				new PlatformAddress(0x8D9518, 0x0, 0x0), // sequences*
				new PlatformAddress(0x8DA5A4, 0x0, 0x0), // first bitmap index*
				new PlatformAddress(0x8DA594, 0x0, 0x0), // bitmap count*
				new PlatformAddress(0x8DA588, 0x0, 0x0), // sprites*
				new PlatformAddress(0x8DA5F4, 0x0, 0x0), // bitmap index*
				new PlatformAddress(0x8DA5EC, 0x0, 0x0), // left*
				new PlatformAddress(0x8DA5E4, 0x0, 0x0), // right*
				new PlatformAddress(0x8DA5DC, 0x0, 0x0), // top*
				new PlatformAddress(0x8DA5D4, 0x0, 0x0), // bottom*
				new PlatformAddress(0x8DA630, 0x0, 0x0), // registration point*
				new PlatformAddress(0x8D950C, 0x0, 0x0), // bitmaps*
//				new PlatformAddress(0x8DA6FC, 0x0, 0x0), // signature*
				new PlatformAddress(0x8DA6EC, 0x0, 0x0), // width*:pixels
				new PlatformAddress(0x8DA6DC, 0x0, 0x0), // height*:pixels
				new PlatformAddress(0x8DA6A4, 0x0, 0x0), // depth*:pixels#depth is 1 for 2D textures and cube maps
				new PlatformAddress(0x8DA680, 0x0, 0x0), // type*#determines bitmap 'geometry'
				new PlatformAddress(0x8DA644, 0x0, 0x0), // format*#determines how pixels are represented internally
				new PlatformAddress(0x8DA620, 0x0, 0x0), // mipmap count*
				new PlatformAddress(0x8D9300, 0x0, 0x0), // pixels offset*
				new PlatformAddress(0x8D937C, 0x0, 0x0), // character*
				new PlatformAddress(0x8D935C, 0x0, 0x0), // bitmap width*
				new PlatformAddress(0x8D934C, 0x0, 0x0), // bitmap height*
				new PlatformAddress(0x8D9310, 0x0, 0x0), // hardware character index*
				new PlatformAddress(0x8D6F3C, 0x0, 0x0), // center of mass*
				new PlatformAddress(0x8D6E7C, 0x0, 0x0), // inertial matrix and inverse*
				new PlatformAddress(0x8D7038, 0x0, 0x0), // model node*
				new PlatformAddress(0x8D7020, 0x0, 0x0), // mass*
				new PlatformAddress(0x8D7000, 0x0, 0x0), // density*
				new PlatformAddress(0x8C73AC, 0x0, 0x0), // predicted resources*
				new PlatformAddress(0x8A1860, 0x0, 0x0), // sample rate*
				new PlatformAddress(0x8A135C, 0x0, 0x0), // encoding*
				new PlatformAddress(0x8A1B1C, 0x0, 0x0), // compression*
				new PlatformAddress(0x8A1A7C, 0x0, 0x0), // name*#the name of the imported pitch range directory
				new PlatformAddress(0x8A18F4, 0x0, 0x0), // actual permutation count*
				new PlatformAddress(0x8A1BD4, 0x0, 0x0), // name*^#name of the file from which this sample was imported
				new PlatformAddress(0x8A1B04, 0x0, 0x0), // next permutation index*

				new PlatformAddress(0x8C9860, 0x0, 0x0), // materials*
				new PlatformAddress(0x8CB0A0, 0x0, 0x0), // region*
				new PlatformAddress(0x8CB090, 0x0, 0x0), // parent node*
				new PlatformAddress(0x8CB07C, 0x0, 0x0), // next sibling node*
				new PlatformAddress(0x8CB068, 0x0, 0x0), // first child node*
				new PlatformAddress(0x8CB060, 0x0, 0x0), // bsps*
				new PlatformAddress(0x926DC0, 0x0, 0x0), // bsp3d nodes*
				new PlatformAddress(0x8C98F4, 0x0, 0x0), // plane*
				new PlatformAddress(0x926DA8, 0x0, 0x0), // back child*
				new PlatformAddress(0x926D98, 0x0, 0x0), // front child*
				new PlatformAddress(0x8C9400, 0x0, 0x0), // planes*
				new PlatformAddress(0x8C8EF8, 0x0, 0x0), // leaves*
				new PlatformAddress(0x926D58, 0x0, 0x0), // bsp2d reference count*
				new PlatformAddress(0x926D40, 0x0, 0x0), // first bsp2d reference*
				new PlatformAddress(0x926D2C, 0x0, 0x0), // bsp2d references*
				new PlatformAddress(0x926D10, 0x0, 0x0), // bsp2d node*
				new PlatformAddress(0x926CD8, 0x0, 0x0), // bsp2d nodes*
				new PlatformAddress(0x926CF8, 0x0, 0x0), // left child*
				new PlatformAddress(0x926CE8, 0x0, 0x0), // right child*
				new PlatformAddress(0x8C99DC, 0x0, 0x0), // surfaces*
				new PlatformAddress(0x926CC4, 0x0, 0x0), // first edge*
				new PlatformAddress(0x8C98E0, 0x0, 0x0), // breakable surface*
				new PlatformAddress(0x926C94, 0x0, 0x0), // material*
				new PlatformAddress(0x926C8C, 0x0, 0x0), // edges*
				new PlatformAddress(0x926C74, 0x0, 0x0), // start vertex*
				new PlatformAddress(0x926C68, 0x0, 0x0), // end vertex*
				new PlatformAddress(0x926C58, 0x0, 0x0), // forward edge*
				new PlatformAddress(0x926C48, 0x0, 0x0), // reverse edge*
				new PlatformAddress(0x926C38, 0x0, 0x0), // left surface*
				new PlatformAddress(0x926C28, 0x0, 0x0), // right surface*
				new PlatformAddress(0x8C9AD0, 0x0, 0x0), // vertices*
				new PlatformAddress(0x8C9834, 0x0, 0x0), // point*
				new PlatformAddress(0x8C8F18, 0x0, 0x0), // collision materials*
				new PlatformAddress(0x8C99FC, 0x0, 0x0), // shader*
				new PlatformAddress(0x8C8F08, 0x0, 0x0), // collision bsp*
				new PlatformAddress(0x8C9630, 0x0, 0x0), // world bounds x*
				new PlatformAddress(0x8C9620, 0x0, 0x0), // world bounds y*
				new PlatformAddress(0x8C9610, 0x0, 0x0), // world bounds z*
				new PlatformAddress(0x8C9A6C, 0x0, 0x0), // cluster*
				new PlatformAddress(0x8C9A50, 0x0, 0x0), // surface reference count*
				new PlatformAddress(0x8C9A3C, 0x0, 0x0), // surface references*
				new PlatformAddress(0x8C8EE8, 0x0, 0x0), // leaf surfaces*
				new PlatformAddress(0x8C9A9C, 0x0, 0x0), // surface*
				new PlatformAddress(0x8C9A94, 0x0, 0x0), // node*
				new PlatformAddress(0x8C8EDC, 0x0, 0x0), // lightmaps*
				new PlatformAddress(0x8C986C, 0x0, 0x0), // bitmap*
				new PlatformAddress(0x8C99E8, 0x0, 0x0), // shader permutation*
				new PlatformAddress(0x8C99CC, 0x0, 0x0), // surface count*
				new PlatformAddress(0x8C99B0, 0x0, 0x0), // ambient color*
				new PlatformAddress(0x8C9998, 0x0, 0x0), // distant light count*
				new PlatformAddress(0x8C9980, 0x0, 0x0), // distant light 0 color*
				new PlatformAddress(0x8C9964, 0x0, 0x0), // distant light 0 direction*
				new PlatformAddress(0x8C994C, 0x0, 0x0), // distant light 1 color*
				new PlatformAddress(0x8C9930, 0x0, 0x0), // distant light 1 direction*
				new PlatformAddress(0x8C991C, 0x0, 0x0), // reflection tint*
				new PlatformAddress(0x8C990C, 0x0, 0x0), // shadow vector*
				new PlatformAddress(0x8C98FC, 0x0, 0x0), // shadow color*
				new PlatformAddress(0x8C98D0, 0x0, 0x0), // vertex buffers*
				new PlatformAddress(0x8C98C8, 0x0, 0x0), // count*
				new PlatformAddress(0x8C98C0, 0x0, 0x0), // offset*
				new PlatformAddress(0x8C8ECC, 0x0, 0x0), // lens flares*
				new PlatformAddress(0x8C8EB8, 0x0, 0x0), // lens flare markers*
				new PlatformAddress(0x8C9224, 0x0, 0x0), // direction i-component*
				new PlatformAddress(0x8C920C, 0x0, 0x0), // direction j-component*
				new PlatformAddress(0x8C91F4, 0x0, 0x0), // direction k-component*
				new PlatformAddress(0x8C91E0, 0x0, 0x0), // lens flare index*
				new PlatformAddress(0x8C8EAC, 0x0, 0x0), // clusters*
				new PlatformAddress(0x8C97C0, 0x0, 0x0), // sky*
				new PlatformAddress(0x8C97B8, 0x0, 0x0), // fog*
				new PlatformAddress(0x8C97A4, 0x0, 0x0), // background sound*
				new PlatformAddress(0x8C9750, 0x0, 0x0), // sound environment*
				new PlatformAddress(0x8C9714, 0x0, 0x0), // weather*
				new PlatformAddress(0x8C9660, 0x0, 0x0), // subclusters*
				new PlatformAddress(0x8C95FC, 0x0, 0x0), // surface indices*
				new PlatformAddress(0x8C95C4, 0x0, 0x0), // index*
				new PlatformAddress(0x8C95A4, 0x0, 0x0), // first lens flare marker index*
				new PlatformAddress(0x8C9588, 0x0, 0x0), // lens flare marker count*
				new PlatformAddress(0x8C9550, 0x0, 0x0), // mirrors*
				new PlatformAddress(0x8C9544, 0x0, 0x0), // portals*
				new PlatformAddress(0x8C97E4, 0x0, 0x0), // portal*
//				new PlatformAddress(0x8C8E9C, 0x0, 0x0), // cluster data*
				new PlatformAddress(0x8C8E88, 0x0, 0x0), // cluster portals*
				new PlatformAddress(0x8C94C4, 0x0, 0x0), // front cluster*
				new PlatformAddress(0x8C94B4, 0x0, 0x0), // back cluster*
				new PlatformAddress(0x8C94A4, 0x0, 0x0), // plane index*
				new PlatformAddress(0x8C9490, 0x0, 0x0), // bounding radius*
				new PlatformAddress(0x8C8E74, 0x0, 0x0), // breakable surfaces*
				new PlatformAddress(0x8C92A0, 0x0, 0x0), // radius*
				new PlatformAddress(0x8C9284, 0x0, 0x0), // collision surface index*
				new PlatformAddress(0x8C8E68, 0x0, 0x0), // fog planes*
				new PlatformAddress(0x8C9330, 0x0, 0x0), // front region*
				new PlatformAddress(0x8C8E58, 0x0, 0x0), // fog regions*
				new PlatformAddress(0x8C9374, 0x0, 0x0), // fog palette*
				new PlatformAddress(0x8C9360, 0x0, 0x0), // weather palette*
				new PlatformAddress(0x8C8E44, 0x0, 0x0), // weather polyhedra*
				new PlatformAddress(0x8C9420, 0x0, 0x0), // bounding sphere center*
				new PlatformAddress(0x8C9408, 0x0, 0x0), // bounding sphere radius*
				new PlatformAddress(0x8C8E2C, 0x0, 0x0), // pathfinding surfaces*
				new PlatformAddress(0x8B4668, 0x0, 0x0), // data*
				new PlatformAddress(0x8C8E18, 0x0, 0x0), // pathfinding edges*
				new PlatformAddress(0x8C92D0, 0x0, 0x0), // midpoint*
				new PlatformAddress(0x8C8DFC, 0x0, 0x0), // background sound palette*
				new PlatformAddress(0x8C8DE0, 0x0, 0x0), // sound environment palette*
//				new PlatformAddress(0x8C8DD0, 0x0, 0x0), // sound PAS data*
//				new PlatformAddress(0x8B464C, 0x0, 0x0), // name*
//				new PlatformAddress(0x8C8DB4, 0x0, 0x0), // detail objects*
//				new PlatformAddress(0x8AF124, 0x0, 0x0), // *
//				new PlatformAddress(0x8C8DA4, 0x0, 0x0), // runtime decals*
				new PlatformAddress(0x9268C4, 0x0, 0x0), // faces*
				new PlatformAddress(0x926924, 0x0, 0x0), // vertex*
				new PlatformAddress(0x9268B4, 0x0, 0x0), // portal indices*
				new PlatformAddress(0x9268F8, 0x0, 0x0), // portal index*
				new PlatformAddress(0x926868, 0x0, 0x0), // back leaf index*
				new PlatformAddress(0x926854, 0x0, 0x0), // front leaf index*
				new PlatformAddress(0x8C736C, 0x0, 0x0), // object names*
				new PlatformAddress(0x8C7928, 0x0, 0x0), // *DON'T TOUCH THIS

				new PlatformAddress(0x926804, 0x0, 0x0), // version*
//				new PlatformAddress(0x9267F0, 0x0, 0x0), // raw animation data*
				new PlatformAddress(0x9267D4, 0x0, 0x0), // unit control data version*
				new PlatformAddress(0x9267B8, 0x0, 0x0), // length of animation*:ticks
//				new PlatformAddress(0x926794, 0x0, 0x0), // recorded animation event stream*
				new PlatformAddress(0x8C77E8, 0x0, 0x0), // yaw[-127,127]*
				new PlatformAddress(0x8C77D4, 0x0, 0x0), // pitch[-127,127]*
				new PlatformAddress(0x925B58, 0x0, 0x0), // surface index*

				new PlatformAddress(0x8C70AC, 0x0, 0x0), // scripts*
				new PlatformAddress(0x924AA0, 0x0, 0x0), // script type*
				new PlatformAddress(0x924A90, 0x0, 0x0), // return type*
//				new PlatformAddress(0x924A78, 0x0, 0x0), // root expression index*
				new PlatformAddress(0x8C70A0, 0x0, 0x0), // globals*
//				new PlatformAddress(0x924A40, 0x0, 0x0), // initialization expression index*
				new PlatformAddress(0x8C7094, 0x0, 0x0), // references*
				new PlatformAddress(0x924A20, 0x0, 0x0), // reference*^
				new PlatformAddress(0x8C7084, 0x0, 0x0), // source files*
				new PlatformAddress(0x8BB4DC, 0x0, 0x0), // maximum sprite extent:pixels*

				new PlatformAddress(0x8B53E0, 0x0, 0x0), // motion sensor scale [DON'T TOUCH EVER]*
//				new PlatformAddress(0x8B45E0, 0x0, 0x0), // text data*
				new PlatformAddress(0x8B45CC, 0x0, 0x0), // message elements*
				new PlatformAddress(0x8B45C0, 0x0, 0x0), // messages*
				new PlatformAddress(0x8B4630, 0x0, 0x0), // start index into text blob*
				new PlatformAddress(0x8B4610, 0x0, 0x0), // start index of message block*
				new PlatformAddress(0x8B4600, 0x0, 0x0), // panel count*
				new PlatformAddress(0x89A030, 0x0, 0x0), // device type*
				new PlatformAddress(0x89A020, 0x0, 0x0), // device id*
				new PlatformAddress(0x89A0F8, 0x0, 0x0), // profile*
			};
			#endregion
			#endregion

			#region Che Ape
			// we offset from one byte since we null-terminate the string memory we're hijacking
			public static PlatformAddress CheApeDllString = new PlatformAddress
				(0x93A098 - 1, 0x6235B0 - 1, 0x910DBA - 1);

			/// <summary>LoadLibraryA call in the game tool</summary>
			public static PlatformAddress CheApeLoadLibrary = new PlatformAddress
				(0xF5582C, 0x6040E8, 0x1082FCC);
			/// <summary>Address where we'll place our initializer code</summary>
			public static PlatformAddress CheApeFunction1 = new PlatformAddress
				(0x421B10, 0x42E2B0, 0x4F9F80);

			/// <summary>Address where we'll place our hook code which calls <see cref="CheApeFunction1"/></summary>
			public static PlatformAddress CheApeFunction2 = new PlatformAddress
				(0x752568, 0x5F1D76, 0x75199B);
			#endregion

			public static PlatformAddress LogHeaderVersion = new PlatformAddress
				(0x89CBDC, 0x60E568, 0x8FDA6A);

			//public static PlatformAddress LogHeaderDateStamp = new PlatformAddress
			//	(0x0, 0x0, 0x0);

			#region Blocks
			public static PlatformAddress BlockAnimation = new PlatformAddress
				(0x9D6DD8, 0x6BAAC0, 0xA4E5B0);

			public static PlatformAddress BlockModelShaderReference = new PlatformAddress
				(0x9D7E84, 0x6C6A0C, 0xA4F65C);

			public static PlatformAddress BlockDamageRegions = new PlatformAddress
				(0x9CF210, 0x6DC338, 0xA47190);
			#endregion
		};
	};
}