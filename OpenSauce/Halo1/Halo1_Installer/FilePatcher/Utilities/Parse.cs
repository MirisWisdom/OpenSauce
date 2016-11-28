/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.ComponentModel;

namespace FilePatcher.Utilities
{
	public static class Parse
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attempts to parse hexadecimal from the given data. </summary>
		///
		/// <tparam name="T">	Generic type parameter. </tparam>
		/// <param name="value"> 	The hex value. </param>
		/// <param name="result">	[out] The result. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public static bool TryParseHex<T>(string value, out T result)
		{
			var converter = TypeDescriptor.GetConverter(typeof(T));

			try
			{
				// Force interpret the value as hex
				var valueString = value;
				if (!valueString.StartsWith("0x"))
				{
					valueString = valueString.Insert(0, "0x");
				}

				result = (T)converter.ConvertFromString(valueString);
			}
			catch (Exception)
			{
				result = default(T);

				return false;
			}

			return true;
		}
	}
}
