using System;
//using Contracts = System.Diagnostics.Contracts;
//using Contract = System.Diagnostics.Contracts.Contract;
using Expr = System.Linq.Expressions.Expression;
using ExprParam = System.Linq.Expressions.ParameterExpression;

namespace BlamLib.Reflection//KSoft.Reflection
{
	internal static class EnumUtils
	{
		/// <summary>Name of the internal member used to represent an enum's integral value</summary>
		/// <remarks>.NET uses this for the name...not sure if Mono or other implementations do as well</remarks>
		public const string kMemberName = "value__";

		#region Underlying Type support utils
		/// <summary>TypeCodes for supported underlying types</summary>
		public static readonly TypeCode[] kSupportedTypeCodes = {
			TypeCode.SByte,	TypeCode.Byte,
			TypeCode.Int16,	TypeCode.UInt16,
			TypeCode.Int32, TypeCode.UInt32,
			TypeCode.Int64, TypeCode.UInt64,
		};
		/// <summary>Types for supported underlying types</summary>
		public static readonly Type[] kSupportedTypes = {
			typeof(SByte), typeof(Byte),
			typeof(Int16), typeof(UInt16),
			typeof(Int32), typeof(UInt32),
			typeof(Int64), typeof(UInt64),
		};

		/// <summary>Is the TypeCode a supported underlying type?</summary>
		/// <param name="c"></param>
		/// <returns></returns>
		public static bool TypeIsSupported(TypeCode c)
		{
			switch (c)
			{
				case TypeCode.SByte:	case TypeCode.Byte:
				case TypeCode.Int16:	case TypeCode.UInt16:
				case TypeCode.Int32:	case TypeCode.UInt32:
				case TypeCode.Int64:	case TypeCode.UInt64:
					return true;

				default: return false;
			}
		}

		/// <summary>Assert the properties of an enum type are valid for any Enum Utils code</summary>
		/// <param name="kEnumType">Enum type in question</param>
		/// <param name="kUnderlyingType">Optional. <paramref name="kEnumType"/>'s underlying type</param>
		/// <remarks>If <paramref name="kUnderlyingType"/> is null, we use the type info found in <paramref name="kEnumType"/></remarks>
		/// <exception cref="NotSupportedException">Thrown when the underlying type is unsupported</exception>
		public static void AssertUnderlyingTypeIsSupported(Type kEnumType, Type kUnderlyingType)
		{
			//Contract.Requires<ArgumentNullException>(kEnumType != null);

			if (kUnderlyingType == null) kUnderlyingType = kEnumType.GetEnumUnderlyingType();

			if (!TypeIsSupported(Type.GetTypeCode(kUnderlyingType)))
			{
				var message =
					string.Format("The underlying type of the type parameter {0} is {1}. " +
								  "Enum Utils only supports Enums with underlying type of " +
								  "SByte, Byte, Int16, UInt16, Int32, UInt32, Int64, or UInt64.",
								  kEnumType, kUnderlyingType);
				throw new NotSupportedException(message);
			}
		}
		#endregion

		/// <summary>Assert the properties of a type are that of an Enum</summary>
		/// <param name="the_type">Type in question</param>
		/// <exception cref="NotSupportedException">Thrown when <paramref name="the_type"/> is not an Enum</exception>
		public static void AssertTypeIsEnum(Type the_type)
		{
			//Contract.Requires<ArgumentNullException>(the_type != null);

			if (the_type.IsEnum) return;

			var message = string.Format("The type parameter {0} is not an Enum. Enum Utils supports Enums only.",
							  the_type);
			throw new NotSupportedException(message);
		}

		public static void AssertTypeIsFlagsEnum(Type the_type)
		{
			//Contract.Requires<ArgumentNullException>(the_type != null);

			AssertTypeIsEnum(the_type);

			if (the_type.GetCustomAttribute<FlagsAttribute>() != null) return;

			var message = string.Format("The Enum type parameter {0} is not annotated as being a Flags Enum (via FlagsAttribute).",
							  the_type);
			throw new NotSupportedException(message);
		}
	};

	/// <summary>Base class for all Enum utilities (even if they're static utils)</summary>
	/// <typeparam name="TEnum">Enum type we're dealing with</typeparam>
	/// <remarks>Not used as a base for utils which have a non-generic core, eg. EnumBinaryStreamerBase</remarks>
	public abstract class EnumUtilBase<TEnum>
		where TEnum : struct
	{
		/// <summary>Enum type we're dealing with</summary>
		protected static readonly Type kEnumType;
		/// <summary>Enum (by-reference) type we're dealing with</summary>
		protected static readonly Type kEnumTypeByRef;
		/// <summary><see cref="kEnumType"/>'s integer type used to represent its raw value</summary>
		protected static readonly Type kUnderlyingType;

		/// <summary>Initializes the <see cref="EnumUtilBase{TEnum}"/> class by generating the base data</summary>
		static EnumUtilBase()
		{
			kEnumType = typeof(TEnum);
			kEnumTypeByRef = kEnumType.MakeByRefType();
			kUnderlyingType = Enum.GetUnderlyingType(kEnumType);

			// NOTE: Up to inheriting class to call AssertTypeIsEnum or AssertTypeIsFlagsEnum
		}
	};

	/// <summary>Utility for converting to and from a given Enum and integer types, without boxing operations but without the safeguards of reflection</summary>
	/// <typeparam name="TEnum"></typeparam>
	/// <remarks>'From' methods can be unforgiving. Make sure you know what you're doing</remarks>
	public sealed class EnumValue<TEnum> : EnumUtilBase<TEnum>
		where TEnum : struct
	{
		public static readonly Func<TEnum, sbyte> ToSByte;
		public static readonly Func<TEnum, byte> ToByte;
		public static readonly Func<sbyte, TEnum> FromSByte;
		public static readonly Func<byte, TEnum> FromByte;

		public static readonly Func<TEnum, short> ToInt16;
		public static readonly Func<TEnum, ushort> ToUInt16;
		public static readonly Func<short, TEnum> FromInt16;
		public static readonly Func<ushort, TEnum> FromUInt16;

		public static readonly Func<TEnum, int> ToInt32;
		public static readonly Func<TEnum, uint> ToUInt32;
		public static readonly Func<int, TEnum> FromInt32;
		public static readonly Func<uint, TEnum> FromUInt32;

		public static readonly Func<TEnum, long> ToInt64;
		public static readonly Func<TEnum, ulong> ToUInt64;
		public static readonly Func<long, TEnum> FromInt64;
		public static readonly Func<ulong, TEnum> FromUInt64;

		static EnumValue()
		{
			EnumUtils.AssertTypeIsEnum(kEnumType);

			GenerateToMethod(out ToSByte);
			GenerateToMethod(out ToByte);
			GenerateFromMethod(out FromSByte);
			GenerateFromMethod(out FromByte);

			GenerateToMethod(out ToInt16);
			GenerateToMethod(out ToUInt16);
			GenerateFromMethod(out FromInt16);
			GenerateFromMethod(out FromUInt16);

			GenerateToMethod(out ToInt32);
			GenerateToMethod(out ToUInt32);
			GenerateFromMethod(out FromInt32);
			GenerateFromMethod(out FromUInt32);

			GenerateToMethod(out ToInt64);
			GenerateToMethod(out ToUInt64);
			GenerateFromMethod(out FromInt64);
			GenerateFromMethod(out FromUInt64);
		}

		static ExprParam GenerateParamValue()
		{
			return Expr.Parameter(kEnumType, "value");	// TEnum value
		}
		static void GenerateToMethod<TInt>(out Func<TEnum, TInt> func)
		{
			// NOTE: Getting runtime errors in lambda.Compile() when we try to return value__ directly

			//////////////////////////////////////////////////////////////////////////
			// Define the generated method's parameters
			var param_v = GenerateParamValue();

			//////////////////////////////////////////////////////////////////////////
			// [result] = (TInt)value.value__
			var TIntType = typeof(TInt);
			var value_expr = Expr.Convert(param_v, TIntType);

			//////////////////////////////////////////////////////////////////////////
			// return (TInt)value.value__
			var ret = value_expr;

			var lambda = Expr.Lambda<Func<TEnum, TInt>>(ret, param_v);
			func = lambda.Compile();
		}

		static void GenerateFromMethod<TInt>(out Func<TInt, TEnum> func)
		{
			var TIntType = typeof(TInt);
			var param_v = Expr.Parameter(TIntType, "value");

			var ret = Expr.Convert(param_v, kEnumType);

			var lambda = Expr.Lambda<Func<TInt, TEnum>>(ret, param_v);
			func = lambda.Compile();
		}
	};
}