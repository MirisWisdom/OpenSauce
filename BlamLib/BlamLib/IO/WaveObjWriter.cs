/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using TI = BlamLib.TagInterface;

namespace BlamLib.IO
{
	public class WaveFrontWriterBase : IDisposable
	{
		protected StreamWriter m_stream;

		public WaveFrontWriterBase(string path, string name, string ext)
		{
			string full_path = string.Format("{0}.{1}", Path.Combine(path, name), ext);

			m_stream = new StreamWriter(full_path, false);
		}

		#region IDisposable Members
		public void Dispose()
		{
			if (m_stream != null)
			{
				m_stream.Dispose();
				m_stream = null;
			}
		}
		#endregion

		protected static string ToStr(float v)							{ return v.ToString("R"); }

		protected void WriteMono(string type, float x)					{ m_stream.WriteLine("{0} {1} {2}", type, ToStr(x)); }
		protected void WriteBi(string type, float x, float y)			{ m_stream.WriteLine("{0} {1} {2}", type, ToStr(x), ToStr(y)); }
		protected void WriteTri(string type, float x, float y, float z)	{ m_stream.WriteLine("{0} {1} {2} {3}", type, ToStr(x), ToStr(y), ToStr(z)); }
	};

	// http://www.martinreddy.net/gfx/3d/OBJ.spec
	public class WaveObjWriter : WaveFrontWriterBase
	{
		public WaveObjWriter(string path, string name) : base(path, name, "obj")
		{
		}

		#region Vertex data
		#region Vertex
		public void WriteVertex(float x, float y, float z)		{ WriteTri("v", x, y, z); }
		public void WriteVertex(LowLevel.Math.real_quaternion v){ WriteVertex(v.Vector.I, v.Vector.J, v.Vector.K); }
		internal void WriteVertex(Render.DeclarationTypes.IDeclType decl)
		{
			if (decl != null)
			{
				LowLevel.Math.real_quaternion v;
				decl.Denormalize(out v);
				WriteVertex(v);
			}
		}
		#endregion

		#region UV
		public void WriteUv(float x, float y)					{ WriteBi("vt", x, y); }
		public void WriteUv(LowLevel.Math.real_quaternion v)	{ WriteUv(v.Vector.I, v.Vector.J); }
		internal void WriteUv(Render.DeclarationTypes.IDeclType decl)
		{
			if (decl != null)
			{
				LowLevel.Math.real_quaternion v;
				decl.Denormalize(out v);
				WriteUv(v);
			}
		}

		public void WriteUvW(float x, float y, float z)			{ WriteTri("vt", x, y, z); }
		public void WriteUvW(LowLevel.Math.real_quaternion v)	{ WriteUvW(v.Vector.I, v.Vector.J, v.Vector.K); }
		internal void WriteUvW(Render.DeclarationTypes.IDeclType decl)
		{
			if (decl != null)
			{
				LowLevel.Math.real_quaternion v;
				decl.Denormalize(out v);
				WriteUvW(v);
			}
		}
		#endregion

		#region Normal
		public void WriteNormal(float x, float y, float z)		{ WriteTri("vn", x, y, z); }
		public void WriteNormal(LowLevel.Math.real_quaternion v){ WriteNormal(v.Vector.I, v.Vector.J, v.Vector.K); }
		internal void WriteNormal(Render.DeclarationTypes.IDeclType decl)
		{
			if (decl != null)
			{
				LowLevel.Math.real_quaternion v;
				decl.Denormalize(out v);
				WriteNormal(v);
			}
		}
		#endregion
		#endregion

		#region Element
		#region Faces
		static void GetFace(bool inc, short[] v, int index, out string s0, out string s1, out string s2)
		{
			if (v != null)
			{
				short v0 = v[index+0], v1 = v[index+1], v2 = v[index+2];
				s0 = (v0 + (inc ? 1 : 0)).ToString();
				s1 = (v1 + (inc ? 1 : 0)).ToString();
				s2 = (v2 + (inc ? 1 : 0)).ToString();
			}
			else
				s0 = s1 = s2 = "";
		}
		public void WriteFace(bool inc, short[] v, int index)
		{
			string s0, s1, s2;
			GetFace(inc, v, index, out s0, out s1, out s2);

			m_stream.WriteLine("f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}", s0, s1, s2);
		}
		public void WriteFace(bool inc, short[] v, short[] vt, short[] vn, int index)
		{
			//const string k_v =		"f {0}// {3}// {6}//";
			//const string k_v_vt =	"f {0}/{1}/ {3}/{4}/ {6}/{7}/";
			const string k_v_vt_vn ="f {0}/{1}/{2} {3}/{4}/{5} {6}/{7}/{8}";
			//const string k_v_vn =	"f {0}//{2} {3}//{5} {6}//{8}";

			string sv0, sv1, sv2,	svt0, svt1, svt2,	svn0, svn1, svn2;

			string fmt = k_v_vt_vn;

			GetFace(inc, v, index, out sv0, out sv1, out sv2);
			GetFace(inc, vt, index, out svt0, out svt1, out svt2);
			GetFace(inc, vn, index, out svn0, out svn1, out svn2);

			//	 if (vt != null && vn == null) fmt = k_v_vt;
			//else if (vt == null && vn != null) fmt = k_v_vn;
			//else if (vt != null && vn != null) fmt = k_v_vt_vn;

			m_stream.WriteLine(fmt, sv0, sv1, sv2,	svt0, svt1, svt2,	svn0, svn1, svn2);
		}

		static void GetFace(bool inc, TI.IElementArray ea, int fi, int index, out string s0, out string s1, out string s2)
		{
			if (ea == null)
			{
				s0 = s1 = s2 = "";
				return;
			}

			TI.Field f0 = ea[index+0, fi], f1 = ea[index+1,fi], f2 = ea[index+2,fi];
			var ft = f0.FieldType;

			switch (ft)
			{
				case TI.FieldType.ShortInteger:
					s0 = ((f0 as TI.ShortInteger).Value + (inc ? 1 : 0)).ToString();
					s1 = ((f1 as TI.ShortInteger).Value + (inc ? 1 : 0)).ToString();
					s2 = ((f2 as TI.ShortInteger).Value + (inc ? 1 : 0)).ToString();
					break;
				case TI.FieldType.LongInteger:
					s0 = ((f0 as TI.LongInteger).Value + (inc ? 1 : 0)).ToString();
					s1 = ((f1 as TI.LongInteger).Value + (inc ? 1 : 0)).ToString();
					s2 = ((f2 as TI.LongInteger).Value + (inc ? 1 : 0)).ToString();
					break;

				default: throw new Debug.Exceptions.UnreachableException(ft);
			}
		}
		/// <param name="fi">field index</param>
		public void WriteFace(bool inc, TI.IElementArray ea, int fi, int index)
		{
			string s0, s1, s2;
			GetFace(inc, ea, fi, index, out s0, out s1, out s2);

			m_stream.WriteLine("f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}", s0, s1, s2);
		}
		/// <param name="fi">field index</param>
		/// <param name="fi_t">field index</param>
		/// <param name="fi_n">field index</param>
		public void WriteFace(bool inc, TI.IElementArray ea, int fi, TI.IElementArray ea_t, int fi_t, TI.IElementArray ea_n, int fi_n, int index)
		{
			const string k_v_vt_vn = "f {0}/{1}/{2} {3}/{4}/{5} {6}/{7}/{8}";
			string fmt = k_v_vt_vn;

			string sv0, sv1, sv2,	svt0, svt1, svt2,	svn0, svn1, svn2;

			GetFace(inc, ea, fi, index, out sv0, out sv1, out sv2);
			GetFace(inc, ea_t, fi_t, index, out svt0, out svt1, out svt2);
			GetFace(inc, ea_n, fi_n, index, out svn0, out svn1, out svn2);

			m_stream.WriteLine(fmt, sv0, sv1, sv2,	svt0, svt1, svt2,	svn0, svn1, svn2);
		}
		#endregion
		#endregion

		#region Grouping
		public void BeginGroup(params string[] group_names)
		{
			bool has_names = group_names != null && group_names.Length > 0;

			m_stream.Write(has_names ? "g " : "g");
			if (has_names)
			{
				int x;
				for(x = 0; x < (group_names.Length-1); x++)
					m_stream.Write("{0} ", group_names[x]);
				m_stream.Write("{0}", group_names[x]);
			}
			m_stream.WriteLine();
		}

		public void EndGroup() { }
		#endregion

		public void WriteComment(string cmt) { m_stream.WriteLine("# {0}", cmt); }
		public void WriteComment(string fmt, params object[] arg) { m_stream.WriteLine("# " + fmt, arg); }
	};
}