/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace BlamLib.Render
{
	namespace COLLADA
	{
		public static class ColladaUtilities
		{
			public enum ColladaRotationOrder
			{
				XYZ,
				YZX,
				ZXY,
				XZY,
				ZYX,
				YXZ,
				ZXZ,
				XYX,
				YZY,
				ZYZ,
				XZX,
				YXY
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Replace characters in a string according to string find-replace pairs. </summary>
			/// <exception cref="ColladaException">	Thrown when a Collada error condition occurs. </exception>
			/// <param name="name">		   	String to format. </param>
			/// <param name="replacements">	find-replace string pairs. </param>
			/// <returns>	String with unwanted characters replaced. </returns>
			///-------------------------------------------------------------------------------------------------
			public static string FormatName(string name, params string[] replacements)
			{
				// the replacements array must be a multiple of two
				if ((replacements.Length % 2) > 0)
					throw new ColladaException("COLLADA_UTILITIES : FormatName : replacements.Length is not a multiple of two");

				// loop through the passed string, replacing all unwanted characters
				for (int i = 0; i < replacements.Length; i += 2)
					name = name.Replace(replacements[i], replacements[i + 1]);
				return name;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Creates a string containing the values of a list. </summary>
			/// <typeparam name="T">	The value type of the source list. </typeparam>
			/// <param name="array">	The sources list. </param>
			/// <returns>	. </returns>
			///-------------------------------------------------------------------------------------------------
			public static string ListToString<T>(List<T> array)
			{
				if (array == null) return "";

				StringBuilder string_builder = new StringBuilder();
				for (int i = 0; i < array.Count; i++)
				{
					if (i > 0) string_builder.Append(' ');
					string_builder.Append(array[i]);
				}
				return string_builder.ToString();
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Parses a string of values into a list. </summary>
			/// <typeparam name="T">	The type of the values in the string. </typeparam>
			/// <param name="value">	The string to parse. </param>
			/// <returns>	A List containing the parsed values from the string. </returns>
			///-------------------------------------------------------------------------------------------------
			public static List<T> StringToList<T>(string value)
			{
				if (value == null) return null;

				// split the string into a list of substrings
				string[] string_array = value.Split(' ');
				List<T> return_array = new List<T>();

				TypeCode type_code = Type.GetTypeCode(typeof(T));
				for (int i = 0; i < string_array.Length; i++)
				{
					object element_object = null;

					// parse the string element into a generic element by typecode
					bool parse_success = false;
					switch (type_code)
					{
						case TypeCode.Boolean:
							bool bool_element;
							parse_success = bool.TryParse(string_array[i], out bool_element);
							element_object = (object)bool_element;
							break;
						case TypeCode.Byte:
							byte byte_element;
							parse_success = byte.TryParse(string_array[i], out byte_element);
							element_object = (object)byte_element;
							break;
						case TypeCode.Char:
							char char_element;
							parse_success = char.TryParse(string_array[i], out char_element);
							element_object = (object)char_element;
							break;
						case TypeCode.DateTime:
							DateTime datetime_element;
							parse_success = DateTime.TryParse(string_array[i], out datetime_element);
							element_object = (object)datetime_element;
							break;
						case TypeCode.Decimal:
							decimal decimal_element;
							parse_success = decimal.TryParse(string_array[i], out decimal_element);
							element_object = (object)decimal_element;
							break;
						case TypeCode.Double:
							double double_element;
							parse_success = double.TryParse(string_array[i], out double_element);
							element_object = (object)double_element;
							break;
						case TypeCode.Int16:
							Int16 int16_element;
							parse_success = Int16.TryParse(string_array[i], out int16_element);
							element_object = (object)int16_element;
							break;
						case TypeCode.Int32:
							Int32 int32_element;
							parse_success = Int32.TryParse(string_array[i], out int32_element);
							element_object = (object)int32_element;
							break;
						case TypeCode.Int64:
							Int64 int64_element;
							parse_success = Int64.TryParse(string_array[i], out int64_element);
							element_object = (object)int64_element;
							break;
						case TypeCode.SByte:
							sbyte sbyte_element;
							parse_success = sbyte.TryParse(string_array[i], out sbyte_element);
							element_object = (object)sbyte_element;
							break;
						case TypeCode.Single:
							float float_element;
							parse_success = float.TryParse(string_array[i], out float_element);
							element_object = (object)float_element;
							break;
						case TypeCode.UInt16:
							UInt16 uint16_element;
							parse_success = UInt16.TryParse(string_array[i], out uint16_element);
							element_object = (object)uint16_element;
							break;
						case TypeCode.UInt32:
							UInt32 uint32_element;
							parse_success = UInt32.TryParse(string_array[i], out uint32_element);
							element_object = (object)uint32_element;
							break;
						case TypeCode.UInt64:
							UInt64 uint64_element;
							parse_success = UInt64.TryParse(string_array[i], out uint64_element);
							element_object = (object)uint64_element;
							break;
						default:
							element_object = string_array[i];
							break;
					}
					// cast the generic element to the needed type and add it to the array
					T element = (T)element_object;
					return_array.Add(element);
				}
				return return_array;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Generates a Uri containing just a fragment. </summary>
			/// <param name="fragment">	. </param>
			/// <returns>	. </returns>
			///-------------------------------------------------------------------------------------------------
			public static string BuildUri(string fragment)
			{
				return String.Concat("#", fragment);
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Generates a Uri for a path using a specific scheme. </summary>
			/// <param name="scheme">	. </param>
			/// <param name="path">  	. </param>
			/// <returns>	. </returns>
			///-------------------------------------------------------------------------------------------------
			public static string BuildUri(string scheme, string path)
			{
				UriBuilder uri_builder = new UriBuilder();
				uri_builder.Scheme = scheme;
				uri_builder.Path = path;
				uri_builder.Host = "/";
				return uri_builder.Uri.AbsoluteUri;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Generates a Uri for a path and fragment using a specific scheme. </summary>
			/// <param name="scheme">  	. </param>
			/// <param name="path">	   	. </param>
			/// <param name="fragment">	. </param>
			/// <returns>	. </returns>
			///-------------------------------------------------------------------------------------------------
			public static string BuildUri(string scheme, string path, string fragment)
			{
				UriBuilder uri_builder = new UriBuilder();
				uri_builder.Scheme = scheme;
				uri_builder.Path = path;
				uri_builder.Fragment = fragment;
				uri_builder.Host = "/";
				return uri_builder.Uri.AbsoluteUri;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Builds external reference URI. </summary>
			/// <param name="externalReference">	The external reference source. </param>
			/// <param name="rootDirectory">		Pathname of the root directory. </param>
			/// <param name="id">					The identifier of the element to referenec. </param>
			/// <returns>	The absolute URI of the external element reference. </returns>
			///-------------------------------------------------------------------------------------------------
			public static string BuildExternalReference(IColladaExternalReference externalReference, string rootDirectory, string id)
			{
				return ColladaUtilities.BuildUri("file://", Path.Combine(rootDirectory, externalReference.GetRelativeURL()) + ".dae", id);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Creates a list of ColladaRotate elements describing a rotation defined by three floats
			/// 	(XYZ)
			/// </summary>
			///
			/// <param name="x">				The x angle. </param>
			/// <param name="y">				The y angle. </param>
			/// <param name="z">				The z angle. </param>
			/// <param name="xColumnVector">	The x column vector. </param>
			/// <param name="yColumnVector">	The y column vector. </param>
			/// <param name="zColumnVector">	The z column vector. </param>
			/// <param name="order">			The rotation order. </param>
			///
			/// <returns>	The new rotation set. </returns>
			public static List<ColladaElement> CreateRotationSet(float x,
				float y,
				float z,
				LowLevel.Math.real_vector3d xColumnVector,
				LowLevel.Math.real_vector3d yColumnVector,
				LowLevel.Math.real_vector3d zColumnVector,
				ColladaRotationOrder order)
			{
				List<ColladaElement> return_array = new List<ColladaElement>();

				Dictionary<ColladaRotationOrder, byte[]> rotation_indices = new Dictionary<ColladaRotationOrder,byte[]>()
				{
					{ ColladaRotationOrder.XYZ, new byte[]{ 0, 1, 2 }},
					{ ColladaRotationOrder.YZX, new byte[]{ 1, 2, 0 }},
					{ ColladaRotationOrder.ZXY, new byte[]{ 2, 0, 1 }},
					{ ColladaRotationOrder.XZY, new byte[]{ 0, 2, 1 }},
					{ ColladaRotationOrder.ZYX, new byte[]{ 2, 1, 0 }},
					{ ColladaRotationOrder.YXZ, new byte[]{ 1, 0, 2 }},
					{ ColladaRotationOrder.ZXZ, new byte[]{ 2, 0, 2 }},
					{ ColladaRotationOrder.XYX, new byte[]{ 0, 1, 0 }},
					{ ColladaRotationOrder.YZY, new byte[]{ 1, 2, 1 }},
					{ ColladaRotationOrder.ZYZ, new byte[]{ 2, 1, 2 }},
					{ ColladaRotationOrder.XZX, new byte[]{ 0, 2, 0 }},
					{ ColladaRotationOrder.YXY, new byte[]{ 1, 0, 1 }}
				};

				var values = new[] {
					new { SID = "rotateX", Column = xColumnVector, Value = x },
					new { SID = "rotateY", Column = yColumnVector, Value = y },
					new { SID = "rotateZ", Column = zColumnVector, Value = z }
				};

				foreach(var index in rotation_indices[order])
				{
					var axis = values[index];

					return_array.Add(
						new Core.ColladaRotate(axis.Column.I, axis.Column.J, axis.Column.K, axis.Value) { sID = axis.SID }
					);
				}

				return return_array;
			}
		}
	}
}