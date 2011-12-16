/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using DX = SlimDX;
using DECLT = BlamLib.Render.DeclarationTypes;

namespace BlamLib
{
	public static class TypeExtensions
	{
		// HACK: The new string id code was ported from some .NET 4 from a different project, which uses some .NET 4 constructs
		// This is just a workaround to continue to use those constructs (kinda)
		public static bool HasFlag(this Blam.StringID.GenerateIdMethod method, Blam.StringID.GenerateIdMethod flag)
		{
			return (method & flag) != 0;
		}

		public static DX.Vector2 To2D(this LowLevel.Math.real_quaternion values)
		{
			return new DX.Vector2(values.Vector.I, values.Vector.J);
		}

		public static DX.Vector3 To3D(this LowLevel.Math.real_quaternion values)
		{
			return new DX.Vector3(values.Vector.I, values.Vector.J, values.Vector.K);
		}

		public static DX.Vector4 To4D(this LowLevel.Math.real_quaternion values)
		{
			return new DX.Vector4(values.Vector.I, values.Vector.J, values.Vector.K, values.W);
		}

		#region Render
		internal static Render.DeclarationTypes.IDeclType AllocateDeclType(this Render.DeclarationType type)
		{
			switch(type)
			{
				case Render.DeclarationType.Float1:		return new DECLT.Float1();
				case Render.DeclarationType.Float2:		return new DECLT.Float2();
				case Render.DeclarationType.Float3:		return new DECLT.Float3();
				case Render.DeclarationType.Float4:		return new DECLT.Float4();
				case Render.DeclarationType.Ubyte4:		return new DECLT.Ubyte4();
				case Render.DeclarationType.UbyteN4:	return new DECLT.Ubyte4N();
				case Render.DeclarationType.Color:		return new DECLT.Color();
				case Render.DeclarationType.UShort2:	return new DECLT.UShort2();
				case Render.DeclarationType.UShort4:	return new DECLT.UShort4();
				case Render.DeclarationType.UShortN2:	return new DECLT.UShort2N();
				case Render.DeclarationType.UShortN4:	return new DECLT.UShort4N();
				case Render.DeclarationType.UShortN2_2:	return new DECLT.UShort2N();
				case Render.DeclarationType.UShortN4_2:	return new DECLT.UShort4N();
				case Render.DeclarationType.UDHen3N:	return new DECLT.UDHen3N();
				case Render.DeclarationType.DHen3N:		return new DECLT.DHen3N();
				case Render.DeclarationType.Float16Two:	return new DECLT.Float16Two();
				case Render.DeclarationType.Float16Four:return new DECLT.Float16Four();
				case Render.DeclarationType.Dec3N:		return new DECLT.Dec3N();
				case Render.DeclarationType.UHenD3N:	return new DECLT.UHenD3N();
				case Render.DeclarationType.UDec4N:		return new DECLT.UDec4N();

				case Render.DeclarationType.ByteN4:		return new DECLT.Byte4N();

				#region Legacy
 				case Render.DeclarationType.UbyteN:		return new DECLT.UbyteN();
				case Render.DeclarationType.UbyteN2:	return new DECLT.UbyteN2();
				case Render.DeclarationType.UbyteN3:	return new DECLT.UbyteN3();

				case Render.DeclarationType.UShortN:	return new DECLT.UShortN();
				case Render.DeclarationType.UShortN3:	return new DECLT.UShort3N();

				case Render.DeclarationType.ShortN:		return new DECLT.ShortN();
				case Render.DeclarationType.ShortN2:	return new DECLT.Short2N();
				case Render.DeclarationType.ShortN3:	return new DECLT.Short3N();
				case Render.DeclarationType.ShortN4:	return new DECLT.Short4N();

				case Render.DeclarationType.HenD3N:		return new DECLT.HenD3N();
				#endregion
				
				case Render.DeclarationType.Skip:		return new DECLT.Skip();

				default: throw new Debug.Exceptions.UnreachableException(type);
			}
		}

		public static int GetSize(this Render.DeclarationType type)
		{
			switch(type)
			{
				case Render.DeclarationType.Float1:		return 4;
				case Render.DeclarationType.Float2:		return 8;
				case Render.DeclarationType.Float3:		return 12;
				case Render.DeclarationType.Float4:		return 16;
				case Render.DeclarationType.Ubyte4:		return 4;
				case Render.DeclarationType.UbyteN4:	return 4;
				case Render.DeclarationType.Color:		return 4;
				case Render.DeclarationType.UShort2:	return 4;
				case Render.DeclarationType.UShort4:	return 8;
				case Render.DeclarationType.UShortN2:	return 4;
				case Render.DeclarationType.UShortN4:	return 8;
				case Render.DeclarationType.UShortN2_2:	return 4;
				case Render.DeclarationType.UShortN4_2:	return 8;
				case Render.DeclarationType.UDHen3N:	return 4;
				case Render.DeclarationType.DHen3N:		return 4;
				case Render.DeclarationType.Float16Two:	return 4;
				case Render.DeclarationType.Float16Four:return 8;
				case Render.DeclarationType.Dec3N:		return 4;
				case Render.DeclarationType.UHenD3N:	return 4;
				case Render.DeclarationType.UDec4N:		return 4;

				case Render.DeclarationType.ByteN4:		return 4;

				#region Legacy
				case Render.DeclarationType.UbyteN:		return 1;
				case Render.DeclarationType.UbyteN2:	return 2;
				case Render.DeclarationType.UbyteN3:	return 3;

				case Render.DeclarationType.UShortN:	return 2;
				case Render.DeclarationType.UShortN3:	return 6;

				case Render.DeclarationType.ShortN:		return 2;
				case Render.DeclarationType.ShortN2:	return 4;
				case Render.DeclarationType.ShortN3:	return 6;
				case Render.DeclarationType.ShortN4:	return 8;

				case Render.DeclarationType.HenD3N:		return 4;
				#endregion
				
				default: throw new Debug.Exceptions.UnreachableException(type);
			}
		}
		#endregion

		/// <summary>Get the inverse of this byte order</summary>
		/// <param name="ef"></param>
		/// <returns></returns>
		public static IO.EndianState Invert(this IO.EndianState ef)
		{
			return ef == IO.EndianState.Little ? IO.EndianState.Big : IO.EndianState.Little;
		}
	};
}