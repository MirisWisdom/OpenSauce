/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region render_geometry_api_resource_definition
	partial class render_geometry_api_resource_definition
	{
		[System.Diagnostics.Conditional("DEBUG")]
		void DebugVerticesOld(System.IO.StreamWriter s, Render.VertexBufferInterface.VertexBuffersHalo3 gr, bool denormalize,
			Render.VertexBufferInterface.VertexBuffersHalo3.Definition def,
			s_tag_d3d_vertex_buffer vb)
		{
			LowLevel.Math.real_quaternion quat;

			var elements = new Render.DeclarationTypes.IDeclType[def.Elements.Length];
			for (int x = 0; x < elements.Length; x++)
				elements[x] = def.Elements[x].DeclarationType.AllocateDeclType();

			using (var er = new IO.EndianReader(vb.VertexBuffer.Value, BlamLib.IO.EndianState.Big, this))
			{
				for (int x = 0; x < vb.VertexCount.Value; x++) // foreach vertex...
				{
					s.WriteLine("\tVertex\t{0}", x.ToString("X8"));
					foreach (Render.DeclarationTypes.IDeclType dt in elements) // and foreach type in the vertex...
					{
						dt.Read(er); // load the type data
						if (denormalize)
						{
							dt.Denormalize(out quat);
							s.WriteLine("\t\t{0}", dt.ToString(quat));
						}
						else s.WriteLine("\t\t{0}", dt.ToString());
						s.Flush();
					}
					s.WriteLine();
				}
				s.WriteLine(); s.WriteLine();
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		void DebugVertices(System.IO.StreamWriter s, Render.VertexBufferInterface.VertexBuffersHalo3 gr, bool denormalize,
			Render.VertexBufferInterface.VertexBuffersHalo3.Definition def,
			s_tag_d3d_vertex_buffer vb)
		{
			var stream_r = new Render.VertexBufferInterface.StreamReader(def);

			using (var er = new IO.EndianReader(vb.VertexBuffer.Value, BlamLib.IO.EndianState.Big, this))
			{
				for (int x = 0; x < vb.VertexCount.Value; x++) // foreach vertex...
				{
					s.WriteLine("\tVertex\t{0}", x.ToString("X8"));
					{
						stream_r.Read(er);
						if (denormalize)
						{
							foreach (string str in stream_r.GetDenormalizedStrings())
								s.WriteLine("\t\t{0}", str);
						}
						else
							foreach (string str in stream_r.GetNormalizedStrings())
								s.WriteLine("\t\t{0}", str);
					}
					s.WriteLine();
				}
				s.WriteLine(); s.WriteLine();
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		void DebugVertices(System.IO.StreamWriter s, Render.VertexBufferInterface.VertexBuffersHalo3 gr, bool denormalize)
		{
			int index = 0;
			foreach (s_tag_d3d_vertex_buffer_reference vbr in VertexBuffers)
			{
				var vb = vbr.Reference.Value;
				if (vb == null)
				{
					s.WriteLine("VertexBuffer\t{0}\tNull", index.ToString("X8"));
					s.WriteLine(); s.WriteLine();
					index++;
					continue; // wtf??
				}
				else s.WriteLine("VertexBuffer\t{0}", index.ToString("X8"));

				var def = gr.DefinitionFind((short)vb.VertxType.Value) as Render.VertexBufferInterface.VertexBuffersHalo3.Definition;
				s.WriteLine("\tSize\t{0}\tCount\t{1}\tType\t{2}",
					vb.VertexSize.Value.ToString("X4"), vb.VertexCount.Value.ToString("X8"), vb.VertxType.Value.ToString("X2"));
				s.WriteLine("\tDefinition Size\t{0}", def.Size.ToString("X4"));
				s.WriteLine();

				//DebugVerticesOld(s, gr, denormalize, def, vb); // from before StreamReader was written!
				DebugVertices(s, gr, denormalize, def, vb);

				index++;
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public void Debug(Managers.TagManager owner_tag, string file, bool denormalize)
		{
			using(var s = new System.IO.StreamWriter(file))
			{
				s.WriteLine("{0} {1}", owner_tag.GroupTag.NameToRightPaddedString(), owner_tag.Name);
				s.WriteLine();

				Program.Halo3.Manager.VertexBufferCacheOpen(BlamVersion.Halo3_Xbox);
				var gr = Program.Halo3.Manager[BlamVersion.Halo3_Xbox].
					GetResource<Render.VertexBufferInterface.VertexBuffersHalo3>(Managers.BlamDefinition.ResourceVertexBuffers);

				// vertex
				DebugVertices(s, gr, denormalize);

				#region index
				int index = 0;
				foreach (s_tag_d3d_index_buffer_reference ibr in IndexBuffers)
				{
					var ib = ibr.Reference.Value;
					if (ib == null)
					{
						s.WriteLine("IndexBuffer\t{0}\tNull", index.ToString("X8"));
						s.WriteLine(); s.WriteLine();
						index++;
						continue; // wtf??
					}
					else s.WriteLine("IndexBuffer\t{0}", index.ToString("X8"));

					s.WriteLine("\tSize\t{0}\tUnknown\t{1}",
						ib.IndexBuffer.Value.Length.ToString("X8"), ib.Unknown.Value.ToString("X8"));
				}
				#endregion

				Program.Halo3.Manager.VertexBufferCacheClose(BlamVersion.Halo3_Xbox);
			}
		}
	};
	#endregion
}