/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using System.Collections.Generic;

namespace YeloDebug
{
	/// <summary>
	/// Miscellaneous helper functions.
	/// </summary>
	public static class Util
	{
        /// <summary>
        /// Converts a timestamp to its local DateTime equivalent.
        /// </summary>
        /// <param name="timestamp"></param>
        /// <returns></returns>
        public static DateTime TimeStampToLocalDateTime(uint timestamp)
        {
            return new DateTime(1970, 1, 1, 0, 0, 0) + new TimeSpan(0, 0, (int)timestamp);
        }

        /// <summary>
        /// Converts a timestamp to its universal DateTime equivalent.
        /// </summary>
        /// <param name="timestamp"></param>
        /// <returns></returns>
        public static DateTime TimeStampToUniversalDateTime(uint timestamp)
        {
            return new DateTime(1970, 1, 1, 0, 0, 0).ToUniversalTime() + new TimeSpan(0, 0, (int)timestamp);
        }

		public static void DataToFile(byte[] data, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write)) fs.Write(data, 0, data.Length);
        }

		public static byte[] HexStringToBytes(string str)
		{
			byte[] Hex = new byte[str.Length / 2];

			for (int i = 0; i < str.Length / 2; i++)
				Hex[i] = Convert.ToByte(str.Substring(i * 2, 2), 16);

			return Hex;
		}

        public static string HexBytesToString(byte[] hexBytes)
        {
            string hexStr = string.Empty;
            for (int i = 0; i < hexBytes.Length; i++)
                hexStr += Convert.ToString(hexBytes[i], 16).PadLeft(2, '0').ToUpper();
            return hexStr;
        }


        // todo: rewrite shitty response info code

        // todo: xbe info path has space in it, need to rethink delimiters...use regex maybe? it's surrounded by quotes
        // methods to parse command responses


        // name=value
        public static object GetResponseInfo(string response, int index)
        {
            return ExtractResponseInformation(response)[index];
        }


        //public static List<object> ExtractResponseInformation(string responseLine)
        //{
        //    List<object> responses = new List<object>();
        //    char[] delimiters = {' ', '\r'};

        //    string[] values = responseLine.Substring(responseLine.IndexOf('=') + 1).Split('=');
        //    foreach (string value in values)
        //    {
        //        string val;
        //        if (value.IndexOfAny(delimiters) != -1)
        //            val = value.Remove(value.IndexOfAny(delimiters));
        //        else val = value;

        //        if (val[0] == '\"') responses.Add((string)val.Substring(1, val.Length - 2));
        //        else if (val.Length > 2 && val.Remove(2) == "0x") responses.Add((uint)Convert.ToUInt32(val.Substring(2), 16));
        //        else responses.Add((uint)Convert.ToUInt32(val));
        //    }
        //    return responses;
        //}


        public static List<object> ExtractResponseInformation(string responseLine)
        {
            List<object> responses = new List<object>();
            int current = 0;
            while (current < responseLine.Length)
            {
                int index = responseLine.IndexOf('=', current);
                if (index < current) break;
                current = index + 1;   // index after next '=' (beginning of value)
                
                // check if next char is a quote, if so...calculate indexof next quote + 2 for after
                int after;
                if (responseLine[current] == '"')
                {
                    after = responseLine.IndexOf('"', current + 1) + 1;
                }
                else
                {
                    after = responseLine.IndexOf(' ', current);     // index after value
                }

                if (after == -1) after = responseLine.Length;
                string value = responseLine.Substring(current, after - current);
                if (value[0] == '\"')
                {
                    responses.Add(value.Replace("\"", ""));
                }
                else if (value.Length > 2 && value.Remove(2) == "0x")
                {
                    responses.Add(Convert.ToUInt32(value.Substring(2), 16));
                }
                else responses.Add(Convert.ToInt32(value));

                current = after + 1;
            }


            //char[] delimiters = { ' ', '\r' };

            //string[] values = responseLine.Substring(responseLine.IndexOf('=') + 1).Split('=');
            //foreach (string value in values)
            //{
            //    string val;
            //    if (value.IndexOfAny(delimiters) != -1)
            //        val = value.Remove(value.IndexOfAny(delimiters));
            //    else val = value;

            //    if (val[0] == '\"') responses.Add((string)val.Substring(1, val.Length - 2));
            //    else if (val.Length > 2 && val.Remove(2) == "0x") responses.Add((uint)Convert.ToUInt32(val.Substring(2), 16));
            //    else responses.Add((uint)Convert.ToUInt32(val));
            //}
            return responses;
        }


        public static uint CelsiusToFahrenheit(uint degrees)
        {
            return (uint)(1.8f * degrees + 32);
        }

        /// <summary>
        /// Makes sure size is aligned to specs.  Unaligned sizes will be rounded up to the next 4kb page boundary.
        /// </summary>
        /// <param name="size"></param>
        /// <returns>Aligned size.</returns>
        public static uint GetAlignedPageBoundary(uint size)
        {
            // calculate actual size of allocation since we are working with 4kb pages
            size += 0xFFF;
            size &= 0xFFFFF000; //rounds up to next 4kb if needed
            return size;
        }

        public static byte[] ObjectToDwordBytes(object obj)
		{
			byte[] data = new byte[4];
			using (MemoryStream ms = new MemoryStream(data))
			using (BinaryWriter bw = new BinaryWriter(ms))
			{
				switch (Type.GetTypeCode(obj.GetType()))
				{
					case TypeCode.Boolean:
					case TypeCode.Byte:
					case TypeCode.Char:
					case TypeCode.Int16:
					case TypeCode.Int32:
					default:
						bw.Write(Convert.ToUInt32(obj));
						break;
					case TypeCode.Single:
						bw.Write(LowLevel.ByteSwap.SingleToUInt32((float)obj));
						break;
				}
			}
			return data;
		}
	};
}