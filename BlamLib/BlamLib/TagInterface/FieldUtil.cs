/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.TagInterface
{
	/// <summary>
	/// Misc. field utility functions
	/// </summary>
	public static class FieldUtil
	{
		/// <summary>
		/// Calculate the size (in bytes) of a certain field depending on certain factors
		/// </summary>
		/// <param name="f">Field instance</param>
		/// <param name="game">Game the field is for</param>
		/// <param name="cache">Is the field in a cache file?</param>
		/// <param name="flags"><see cref="IO.ITagStreamFlags"/></param>
		/// <returns>Size in bytes of the field instance</returns>
		public static int Sizeof(Field f, BlamVersion game, bool cache, uint flags)
		{
			switch (f.FieldType)
			{
				case FieldType.String:
					return (f as String).GetFieldSize();

				#region StringId
				case FieldType.OldStringId:
					if (Util.Flags.Test(flags, IO.ITagStreamFlags.Halo2OldFormat_StringId))
					{
						return 32; // 28 bytes are preview string, 4 bytes for the string_id
					}
					goto case FieldType.StringId;

				case FieldType.StringId:
					if ((game & BlamVersion.Halo1) != 0 || (game & BlamVersion.Stubbs) != 0)
					{
						return 20; // we treat string ids as tag data in these games
					}
					else if (game == BlamVersion.Halo2_Alpha)
					{
						return 32; // 28 bytes are preview string, 4 bytes for the string_id
					}
					else if ((game & BlamVersion.Halo2) != 0 || // even echo used this format
							(game & BlamVersion.Halo3) != 0)
					{
						return 4;
					}
					throw new Debug.ExceptionLog("Failed to handle a string id: {0}", game);
				#endregion

				#region sizeof(byte)
				case FieldType.ByteInteger:
				case FieldType.ByteEnum:
				case FieldType.ByteFlags:
				case FieldType.ByteBlockIndex:
					return 1;
				#endregion

				#region sizeof(word)
				case FieldType.ShortInteger:
				case FieldType.Enum:
				case FieldType.WordFlags:
				case FieldType.ShortBlockIndex:
					return 2;
				#endregion

				#region sizeof(dword)
				case FieldType.LongInteger:
				case FieldType.Angle:
				case FieldType.Tag:
				case FieldType.LongEnum:
				case FieldType.Point2D:
				case FieldType.LongFlags:
				case FieldType.RgbColor:
				case FieldType.ArgbColor:
				case FieldType.Real:
				case FieldType.RealFraction:
				case FieldType.ShortBounds:
				case FieldType.LongBlockIndex:
					return 4;
				#endregion

				#region sizeof(qword)
				case FieldType.Rectangle2D:
				case FieldType.RealPoint2D:
				case FieldType.RealVector2D:
				case FieldType.RealEulerAngles2D:
				case FieldType.AngleBounds:
				case FieldType.RealBounds:
				case FieldType.RealFractionBounds:
					return 4 + 4;
				#endregion

				#region sizeof(real_vector3d)
				case FieldType.RealPoint3D:
				case FieldType.RealVector3D:
				case FieldType.RealPlane2D:
				case FieldType.RealRgbColor:
				case FieldType.RealEulerAngles3D:
					return 4 + 4 + 4;
				#endregion

				#region sizeof(real_vector4d)
				case FieldType.RealQuaternion:
				case FieldType.RealPlane3D:
				case FieldType.RealArgbColor:
					return 4 + 4 + 4 + 4;
				#endregion

				#region TagReference
				case FieldType.TagReference:
					if ((game & (BlamVersion.Halo1 | BlamVersion.Stubbs)) != 0)
					{
						return 16;
					}
					else if (game == BlamVersion.Halo2_Alpha || game == BlamVersion.Halo2_Epsilon)
					{
						return 16;
					}
					else if ((game & BlamVersion.Halo2) != 0)
					{
						if (cache)	return 8;
						else		return 16;
					}
					else if ((game & (BlamVersion.Halo3 | BlamVersion.HaloOdst)) != 0)
					{
						return 16;
					}
					else if ((game & BlamVersion.HaloReach) != 0)
					{
						return 16; // TODO: update upon release
					}
					throw new Debug.ExceptionLog("Failed to handle a tag reference: {0}", game);
				#endregion

				#region TagBlock
				case FieldType.Block:
					if ((game & (BlamVersion.Halo1 | BlamVersion.Stubbs)) != 0)
					{
						return 12;
					}
					else if (game == BlamVersion.Halo2_Alpha || game == BlamVersion.Halo2_Epsilon)
					{
						return 12;
					}
					else if ((game & BlamVersion.Halo2) != 0)
					{
						if (cache) return 8;
						else return 12;
					}
					else if ((game & (BlamVersion.Halo3 | BlamVersion.HaloOdst)) != 0)
					{
						return 12;
					}
					else if ((game & BlamVersion.HaloReach) != 0)
					{
						return 12; // TODO: update upon release
					}
					throw new Debug.ExceptionLog("Failed to handle a tag block: {0}", game);
				#endregion

				#region TagData
				case FieldType.Data:
					if ((game & (BlamVersion.Halo1 | BlamVersion.Stubbs)) != 0)
					{
						return 20;
					}
					else if (game == BlamVersion.Halo2_Alpha || game == BlamVersion.Halo2_Epsilon)
					{
						return 20;
					}
					else if ((game & BlamVersion.Halo2) != 0)
					{
						if (cache) return 8;
						else return 20;
					}
					else if ((game & (BlamVersion.Halo3 | BlamVersion.HaloOdst)) != 0)
					{
						return 20;
					}
					else if ((game & BlamVersion.HaloReach) != 0)
					{
						return 20; // TODO: update upon release
					}
					throw new Debug.ExceptionLog("Failed to handle tag data: {0}", game);
				#endregion

				case FieldType.VertexBuffer:
					if ((game & BlamVersion.Halo2) != 0) return 32;
					else return 0;

				case FieldType.Struct:
					if (cache && ((game & BlamVersion.Halo2) != 0))
						return (f as IStruct).GetDefinition().RuntimeSizes.RuntimeSize;
					return (f as IStruct).GetDefinition().Attribute.SizeOf;
				case FieldType.StructReference:
					return 12;

				#region Padding\Skip
				case FieldType.Pad:
					return (f as Pad).Value;

				case FieldType.UnknownPad:
					return (f as UnknownPad).Value;

				case FieldType.UselessPad:
					if ((game & (BlamVersion.Halo1 | BlamVersion.Stubbs)) != 0 ||
						game == BlamVersion.Halo2_Alpha || game == BlamVersion.Halo2_Epsilon)
					{
						return (f as UselessPad).Value;
					}
					else if ((game & (BlamVersion.Halo2 | BlamVersion.Halo3 | BlamVersion.HaloOdst | BlamVersion.HaloReach)) != 0)
					{
						if(Util.Flags.Test(flags, IO.ITagStreamFlags.Halo2OldFormat_UselessPadding))
							return (f as UselessPad).Value;

						return 0;
					}
					throw new Debug.ExceptionLog("Failed to handle useless pad: {0}", game);

				case FieldType.Skip:
					return (f as Skip).Value;
				#endregion

				#region CustomData
				case FieldType.CustomData:
					return (f as CustomDataField).Sizeof(game, cache);
				#endregion

				#region Zero
				case FieldType.ArrayStart:
				case FieldType.ArrayEnd:
				case FieldType.Custom:
				case FieldType.Terminator:
					return 0;
				#endregion

				case FieldType.None:	throw new Debug.ExceptionLog("Found a field that says it has no type: {0}", f.GetType());
				default:				throw new Debug.ExceptionLog("Sizeof failed to handle a field type: {0:X}", (int)f.FieldType);
			}
		}

		/// <summary>
		/// Recreate a field based on properties defined in a definition file's entry data
		/// </summary>
		/// <param name="engine">Engine this definition item belongs in</param>
		/// <param name="owner">Owner object to this field</param>
		/// <param name="item">Property data for the field</param>
		/// <returns></returns>
		public static Field CreateFromDefinitionItem(BlamVersion engine, IStructureOwner owner, DefinitionFile.Item item)
		{
			switch (item.Type)
			{
				case FieldType.String:				return new String((StringType)(item.Flags & 0x0000FFFF), (int)(item.Flags >> 16));
				case FieldType.StringId:			return new StringId(item.Type == FieldType.OldStringId);
				case FieldType.ByteInteger:			return new ByteInteger();
				case FieldType.ShortInteger:		return new ShortInteger();
				case FieldType.LongInteger:			return new LongInteger();
				case FieldType.Tag:					return new Tag();
				case FieldType.ByteEnum:
				case FieldType.Enum:
				case FieldType.LongEnum:
													return new Enum((FieldType)item.Flags);
				case FieldType.ByteFlags:
				case FieldType.WordFlags:
				case FieldType.LongFlags:
													return new Flags((FieldType)item.Flags);
				case FieldType.Point2D:				return new Point2D();
				case FieldType.Rectangle2D:			return new Rectangle2D();
				case FieldType.RgbColor:
				case FieldType.ArgbColor:
													return new Color((FieldType)item.Flags);
				case FieldType.Angle:
				case FieldType.Real:
				case FieldType.RealFraction:
													return new Real((FieldType)item.Flags);
				case FieldType.RealPoint2D:			return new RealPoint2D();
				case FieldType.RealPoint3D:			return new RealPoint3D();
				case FieldType.RealVector2D:		return new RealVector2D();
				case FieldType.RealVector3D:		return new RealVector3D();
				case FieldType.RealQuaternion:		return new RealQuaternion();
				case FieldType.RealEulerAngles2D:	return new RealEulerAngles2D();
				case FieldType.RealEulerAngles3D:	return new RealEulerAngles3D();
				case FieldType.RealPlane2D:			return new RealPlane2D();
				case FieldType.RealPlane3D:			return new RealPlane3D();
				case FieldType.RealRgbColor:
				case FieldType.RealArgbColor:
													return new RealColor((FieldType)item.Flags);
				case FieldType.ShortBounds:			return new ShortIntegerBounds();
				case FieldType.AngleBounds:
				case FieldType.RealBounds:
				case FieldType.RealFractionBounds:
													return new RealBounds((FieldType)item.Flags);
				case FieldType.TagReference:		return new TagReference(owner, Blam.MiscGroups.TagGroupFromHandle((Blam.DatumIndex)item.Flags));
				case FieldType.Block:				return new Block<Definition>(owner);
				case FieldType.ByteBlockIndex:
				case FieldType.ShortBlockIndex:
				case FieldType.LongBlockIndex:
													return new BlockIndex((FieldType)item.Flags);
				case FieldType.Data:				return new Data(owner, (DataType)item.Flags);
				case FieldType.Struct:				return new Struct<Definition>(owner);
				case FieldType.StructReference:		return new StructReference<Definition>(owner);
				case FieldType.Pad:					return new Pad(item.Flags);
				case FieldType.UnknownPad:			return new UnknownPad(item.Flags);
				case FieldType.UselessPad:			return new UselessPad(item.Flags);
				case FieldType.Skip:				return new Skip(item.Flags);
				//case FieldType.Custom:			return new Custom(TagGroup.FromUInt(item.Flags));
				//case FieldType.CustomData:			return null;

				default:							throw new Debug.Exceptions.UnreachableException(item.Type);
			}
		}
	};
}