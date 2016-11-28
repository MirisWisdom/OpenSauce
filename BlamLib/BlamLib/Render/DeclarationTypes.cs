/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Render
{
	internal static class DeclarationTypes
	{
		public interface IDeclType : IO.IStreamable
		{
			void Normalize(LowLevel.Math.real_quaternion values);
			void Denormalize(out LowLevel.Math.real_quaternion values);
			string ToString(LowLevel.Math.real_quaternion values);
		};

		public class Skip : IDeclType
		{
			public int ByteCount;
			byte[] nullBuffer;

			public override string ToString()
			{
				return string.Format("[Skip {0}]", ByteCount.ToString());
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Skip]");
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values) { }

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				s.Seek(ByteCount);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				if(nullBuffer == null)
					nullBuffer = new byte[ByteCount];

				s.Write(nullBuffer);
			}
			#endregion
		};


		#region Float#
		public class Float1 : IDeclType
		{
			float X;

			public override string ToString()
			{
				return string.Format("[Float1 {0}]", X.ToString());
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Float1 {0}]", values.Vector.I.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values) { X = values.Vector.I; }

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = X;
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadSingle();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
			}
			#endregion
		};

		public class Float2 : IDeclType
		{
			float X, Y;

			public override string ToString()
			{
				return string.Format("[Float2 {0}\t{1}]", X.ToString(), Y.ToString());
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Float2 {0}\t{1}]", values.Vector.I.ToString(), values.Vector.J.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)		{ X = values.Vector.I; Y = values.Vector.J; }

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = X;
				values.Vector.J = Y;
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadSingle();
				Y = s.ReadSingle();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		public class Float3 : IDeclType
		{
			float X, Y, Z;

			public override string ToString()
			{
				return string.Format("[Float3 {0}\t{1}\t{2}]", X, Y, Z);
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Float3 {0}\t{1}\t{2}]", values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)		{ X = values.Vector.I; Y = values.Vector.J; Z = values.Vector.K; }

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = X;
				values.Vector.J = Y;
				values.Vector.K = Z;
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadSingle();
				Y = s.ReadSingle();
				Z = s.ReadSingle();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
			}
			#endregion
		};

		public class Float4 : IDeclType
		{
			float X, Y, Z, W;

			public override string ToString()
			{
				return string.Format("[Float4 {0}\t{1}\t{2}\t{3}]", X, Y, Z, W);
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Float4 {0}\t{1}\t{2}\t{3}]", 
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)		{ X = values.Vector.I; Y = values.Vector.J; Z = values.Vector.K; W = values.W; }

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = X;
				values.Vector.J = Y;
				values.Vector.K = Z;
				values.W = W;
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadSingle();
				Y = s.ReadSingle();
				Z = s.ReadSingle();
				W = s.ReadSingle();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
				s.Write(W);
			}
			#endregion
		};
		#endregion

		#region Ubyte#
		public class Ubyte4 : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[Ubyte4 {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Ubyte4 {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUByte4(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUByte4(Value);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		public class Ubyte4N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[Ubyte4N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Ubyte4N {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUByte4N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUByte4N(Value);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};
		#endregion

		public class Color : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[Color {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Color {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Math.real_quaternion temp = new LowLevel.Math.real_quaternion();
				temp.W = values.W; // A
				temp.Vector.I = values.Vector.K; // B
				temp.Vector.J = values.Vector.J; // G
				temp.Vector.K = values.Vector.I; // R

				LowLevel.Xna.Math.ConvertColor(temp, out Value);
			}

			/// <summary>
			/// <see cref="LowLevel.Math.real_vector3d.I"/> = R, 
			/// <see cref="LowLevel.Math.real_vector3d.J"/> = G, 
			/// <see cref="LowLevel.Math.real_vector3d.K"/> = B, 
			/// <see cref="LowLevel.Math.real_quaternion.W"/> = A
			/// </summary>
			/// <param name="values"></param>
			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				LowLevel.Math.real_quaternion temp = LowLevel.Xna.Math.ConvertColor(Value);
				// I\X = B
				// J\Y = G
				// K\Z = R
				// W\W = A
				
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I =	temp.Vector.K; // R
				values.Vector.J =	temp.Vector.J; // G
				values.Vector.K =	temp.Vector.I; // B
				values.W =			temp.W; // A
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		#region UShort#(@)
		public class UShort2 : IDeclType
		{
			public ushort X, Y;

			public override string ToString()
			{
				return string.Format("[UShort2 {0}\t{1}]", X.ToString("X4"), Y.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UShort2 {0}\t{1}]", values.Vector.I.ToString(), values.Vector.J.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUShort2(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUShort2(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		public class UShort4 : IDeclType
		{
			public ushort X, Y, Z, W;

			public override string ToString()
			{
				return string.Format("[UShort4 {0}\t{1}\t{2}\t{3}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"), W.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UShort4 {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUShort4(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUShort4(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
				Z = s.ReadUInt16();
				W = s.ReadUInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
				s.Write(W);
			}
			#endregion
		};

		public class UShort2N : IDeclType
		{
			public ushort X, Y;

			public override string ToString()
			{
				return string.Format("[UShort2N {0}\t{1}]", X.ToString("X4"), Y.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UShort2N {0}\t{1}]", values.Vector.I.ToString(), values.Vector.J.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUShort2N(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUShort2N(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		public class UShort4N : IDeclType
		{
			public ushort X, Y, Z, W;

			public override string ToString()
			{
				return string.Format("[UShort4N {0}\t{1}\t{2}\t{3}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"), W.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UShort4N {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUShort4N(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUShort4N(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
				Z = s.ReadUInt16();
				W = s.ReadUInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
				s.Write(W);
			}
			#endregion
		};
		#endregion

		#region(U)DHen3N
		public class UDHen3N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[UDHen3N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UDHen3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUDHen3N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUDHen3N(Value);
				//values = new LowLevel.Math.real_quaternion();
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		public class DHen3N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[DHen3N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[DHen3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertDHen3N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertDHen3N(Value);
				//values = new LowLevel.Math.real_quaternion();
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};
		#endregion

		#region Float16#
		public class Float16Two : IDeclType
		{
			public ushort X, Y;

			public override string ToString()
			{
				return string.Format("[Float16Two {0}\t{1}]", X.ToString("X4"), Y.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Float16Two {0}\t{1}]", values.Vector.I.ToString(), values.Vector.J.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertFloat16Two(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertFloat16Two(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		public class Float16Four : IDeclType
		{
			public ushort X, Y, Z, W;

			public override string ToString()
			{
				return string.Format("[Float16Four {0}\t{1}\t{2}\t{3}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"), W.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Float16Four {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertFloat16Four(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertFloat16Four(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
				Z = s.ReadUInt16();
				W = s.ReadUInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
				s.Write(W);
			}
			#endregion
		};
		#endregion

		public class Dec3N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[Dec3N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Dec3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertDec3N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertDec3N(Value);
				//values = new LowLevel.Math.real_quaternion();
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		public class UHenD3N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[UHenD3N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UHenD3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUHenD3N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUHenD3N(Value);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		public class UDec4N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[UDec4N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UDec4N {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUDec4N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUDec4N(Value);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		// Only added for added support for a runtime only vertex format 
		// (so this shouldn't be found in tag data...)
		public class Byte4N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[Byte4N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Byte4N {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertByte4N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertByte4N(Value);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};

		#region Legacy
		public class UbyteN : IDeclType
		{
			public byte Value;

			public override string ToString()
			{
				return string.Format("[UbyteN {0}]", Value.ToString("X2"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UbyteN {0}\t0\t0\t0]",
					values.Vector.I.ToString()/*, values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString()*/);
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				Value = (byte)values.Vector.I;
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = (float)Value;
			}


			public void Read(BlamLib.IO.EndianReader s)		{ Value = s.ReadByte(); }

			public void Write(BlamLib.IO.EndianWriter s)	{ s.Write(Value); }
			#endregion
		};

		public class UbyteN2 : IDeclType
		{
			public byte X, Y;

			public override string ToString()
			{
				return string.Format("[UbyteN2 {0}]", X.ToString("X2"), Y.ToString("X2"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UbyteN2 {0}\t{1}\t0\t0]",
					values.Vector.I.ToString(), values.Vector.J.ToString()/*, values.Vector.K.ToString(), values.W.ToString()*/);
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				X = (byte)values.Vector.I;
				Y = (byte)values.Vector.J;
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = (float)X;
				values.Vector.J = (float)Y;
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadByte();
				Y = s.ReadByte();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		public class UbyteN3 : IDeclType
		{
			public byte X, Y, Z;

			public override string ToString()
			{
				return string.Format("[UbyteN3 {0}]", X.ToString("X2"), Y.ToString("X2"), Z.ToString("X2"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UbyteN3 {0}\t{1}\t{2}\t0]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString()/*, values.W.ToString()*/);
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				X = (byte)values.Vector.I;
				Y = (byte)values.Vector.J;
				Z = (byte)values.Vector.K;
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = new LowLevel.Math.real_quaternion();
				values.Vector.I = (float)X;
				values.Vector.J = (float)Y;
				values.Vector.K = (float)Z;
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadByte();
				Y = s.ReadByte();
				Z = s.ReadByte();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
			}
			#endregion
		};


		//HACK: Piggy backs on UShort2N since XM doesn't define a N type
		public class UShortN : IDeclType
		{
			public ushort X;
			private ushort Y;

			public override string ToString()
			{
				return string.Format("[UShortN {0}]", X.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UShortN {0}]", values.Vector.I.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUShort2N(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUShort2N(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = 0;
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
			}
			#endregion
		};

		//HACK: Piggy backs on UShort4N since XM doesn't define a 3N type
		public class UShort3N : IDeclType
		{
			public ushort X, Y, Z;
			private ushort W;

			public override string ToString()
			{
				return string.Format("[UShort3N {0}\t{1}\t{2}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[UShort3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertUShort4N(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertUShort4N(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadUInt16();
				Y = s.ReadUInt16();
				Z = s.ReadUInt16();
				W = 0;
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
			}
			#endregion
		};


		//HACK: Piggy backs on Short2N since XM doesn't define a N type
		public class ShortN : IDeclType
		{
			public short X;
			private short Y;

			public override string ToString()
			{
				return string.Format("[ShortN {0}]", X.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[ShortN {0}]", values.Vector.I.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertShort2N(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertShort2N(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadInt16();
				Y = 0;
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
			}
			#endregion
		};

		public class Short2N : IDeclType
		{
			public short X, Y;

			public override string ToString()
			{
				return string.Format("[Short2N {0}\t{1}]", X.ToString("X4"), Y.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Short2N {0}\t{1}]", values.Vector.I.ToString(), values.Vector.J.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertShort2N(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertShort2N(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadInt16();
				Y = s.ReadInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		//HACK: Piggy backs on Short4N since XM doesn't define a 3N type
		public class Short3N : IDeclType
		{
			public short X, Y, Z;
			private short W;

			public override string ToString()
			{
				return string.Format("[Short3N {0}\t{1}\t{2}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Short3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertShort4N(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertShort4N(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadInt16();
				Y = s.ReadInt16();
				Z = s.ReadInt16();
				W = 0;
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
			}
			#endregion
		};

		public class Short4N : IDeclType
		{
			public short X, Y, Z, W;

			public override string ToString()
			{
				return string.Format("[Short4N {0}\t{1}\t{2}\t{3}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"), W.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Short4N {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertShort4N(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertShort4N(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadInt16();
				Y = s.ReadInt16();
				Z = s.ReadInt16();
				W = s.ReadInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
				s.Write(W);
			}
			#endregion
		};


		public class HenD3N : IDeclType
		{
			public uint Value;

			public override string ToString()
			{
				return string.Format("[HenD3N {0}]", Value.ToString("X8"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[HenD3N {0}\t{1}\t{2}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertHenD3N(values, out Value);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertHenD3N(Value);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				Value = s.ReadUInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Value);
			}
			#endregion
		};
		#endregion


		// No games currently use the following...

		public class Short2 : IDeclType
		{
			public short X, Y;

			public override string ToString()
			{
				return string.Format("[Short2 {0}\t{1}]", X.ToString("X4"), Y.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Short2 {0}\t{1}]", values.Vector.I.ToString(), values.Vector.J.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertShort2(values, out X, out Y);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertShort2(X, Y);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadInt16();
				Y = s.ReadInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
			}
			#endregion
		};

		public class Short4 : IDeclType
		{
			public short X, Y, Z, W;

			public override string ToString()
			{
				return string.Format("[Short4 {0}\t{1}\t{2}\t{3}]", X.ToString("X4"), Y.ToString("X4"), Z.ToString("X4"), W.ToString("X4"));
			}

			string IDeclType.ToString(LowLevel.Math.real_quaternion values) { return ToString(values); }
			public static string ToString(LowLevel.Math.real_quaternion values)
			{
				return string.Format("[Short4 {0}\t{1}\t{2}\t{3}]",
					values.Vector.I.ToString(), values.Vector.J.ToString(), values.Vector.K.ToString(), values.W.ToString());
			}

			#region IDeclType Members
			public void Normalize(LowLevel.Math.real_quaternion values)
			{
				LowLevel.Xna.Math.ConvertShort4(values, out X, out Y, out Z, out W);
			}

			public void Denormalize(out LowLevel.Math.real_quaternion values)
			{
				values = LowLevel.Xna.Math.ConvertShort4(X, Y, Z, W);
			}


			public void Read(BlamLib.IO.EndianReader s)
			{
				X = s.ReadInt16();
				Y = s.ReadInt16();
				Z = s.ReadInt16();
				W = s.ReadInt16();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(X);
				s.Write(Y);
				s.Write(Z);
				s.Write(W);
			}
			#endregion
		};
	};
}