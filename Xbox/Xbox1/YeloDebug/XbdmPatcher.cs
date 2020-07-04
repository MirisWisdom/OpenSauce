/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;

namespace YeloDebug
{
	public class XbdmPatcher
	{
		public static string TryAndGetXbdmPath()
		{
			const string kXdkPath = @"Microsoft Xbox SDK\xbox\symbols\5455\";

			string path = Path.Combine(@"C:\Program Files\", kXdkPath);
			if (Directory.Exists(path))
				return path;

			path = Path.Combine(@"C:\Program Files (x86)\", kXdkPath);
			if (Directory.Exists(path))
				return path;

			return null;
		}

		/// <summary>Expected XOR hash value of a v5455 XBDM.dll file</summary>
		const uint kXbdmFileHash5455 = 0x3F579BF9;

		string m_xbdmFilePath;

		public XbdmPatcher(string xbdm_file_path)
		{
			m_xbdmFilePath = xbdm_file_path;
		}

		/// <summary>Verify the source XBDM.dll is valid for patching</summary>
		/// <returns></returns>
		public bool Verify()
		{
			if (File.Exists(m_xbdmFilePath))
			{
				uint hash_result = 0;
				using (var fs = new FileStream(m_xbdmFilePath, FileMode.Open, FileAccess.Read))
				using (var br = new BinaryReader(fs))
				{
					for (long pos = 0; pos < fs.Length && (fs.Length - pos) >= 4; pos += sizeof(uint) )
						hash_result ^= br.ReadUInt32();
				}

				return hash_result == kXbdmFileHash5455;
			}

			return false;
		}

		/// <summary>Copy the source XBDM.dll and patch it</summary>
		/// <param name="output_file_path">Output path and filename of the resulting XBDM.dll</param>
		/// <returns>False if this fails</returns>
		public bool ApplyPatch(string output_file_path)
		{
			try { File.Copy(m_xbdmFilePath, output_file_path, true); }
			catch (UnauthorizedAccessException /*ex*/)
			{
				return false;
			}

			using (var fs = new FileStream(output_file_path, FileMode.Open, FileAccess.Write))
			using (var bw = new BinaryWriter(fs))
			{
				// patch version info
				fs.Position = 0x1B0AC; // build1
				bw.Write((ushort)7887);
				fs.Position = 0x34356; // build2
				bw.Write((ushort)7887);
				fs.Position = 0x3435E; // build3
				bw.Write((ushort)7887);
				fs.Position = 0x1FFD;
				bw.Write(System.Text.ASCIIEncoding.ASCII.GetBytes("7887.1"));
				fs.Position = 0x3447E;
				bw.Write(System.Text.UnicodeEncoding.Unicode.GetBytes("7887.1"));
				fs.Position = 0x34626;
				bw.Write(System.Text.UnicodeEncoding.Unicode.GetBytes("7887.1"));
				//patched.Position = 0x34354; // revision1
				//pbw.Write((ushort)7887);

				// hook receive file functions
				fs.Position = 0xB3E0;
				byte[] hrReceiveFileDataHook = { 0x68, 0x70, 0x70, 0x03, 0xB0, 0xC3 };
				bw.Write(hrReceiveFileDataHook);
				fs.Position = 0xB664;
				byte[] hrReceivePartialFileHook = { 0x68, 0x70, 0x6F, 0x03, 0xB0, 0xC3 };
				bw.Write(hrReceivePartialFileHook);

				// add extra space
				fs.SetLength(0x38000);

				// store codecaves
				fs.Position = 0x36F70;
				byte[] hrReceivePartialFileCave = { 0x60, 0x0F, 0x20, 0xC0, 0x50, 0x25, 0xFF, 0xFF, 0xFE, 0xFF, 0x0F, 
                    0x22, 0xC0, 0xBB, 0x60, 0x6F, 0x03, 0xB0, 0x3E, 0x80, 0xBD, 0xA4, 0xFE, 0xFF, 0xFF, 0x7C, 0x75, 
                    0x07, 0xC6, 0x03, 0x01, 0xEB, 0x07, 0xEB, 0x03, 0xC6, 0x03, 0x00, 0xEB, 0x33, 0x3E, 0x8B, 0x45, 
                    0x14, 0x3E, 0xC7, 0x45, 0x14, 0x00, 0x00, 0x00, 0x00, 0x89, 0x43, 0x04, 0x03, 0x47, 0x14, 0x89, 
                    0x43, 0x08, 0x8D, 0xBD, 0xA4, 0xFE, 0xFF, 0xFF, 0xC7, 0x07, 0x45, 0x3A, 0x5C, 0x66, 0xC7, 0x47, 
                    0x04, 0x55, 0x6B, 0x4D, 0x24, 0xC7, 0x47, 0x08, 0x44, 0x65, 0x56, 0x73, 0xC6, 0x47, 0x0C, 0x00, 
                    0x58, 0x0F, 0x22, 0xC0, 0x61, 0x8B, 0x77, 0x10, 0x8B, 0x0B, 0x6A, 0x20, 0x68, 0x6B, 0xB6, 0x00, 
                    0xB0, 0xC3 };
				bw.Write(hrReceivePartialFileCave);

				fs.Position = 0x37070;
				byte[] hrReceiveFileDataCave = { 0x60, 0xBB, 0x60, 0x6F, 0x03, 0xB0, 0x80, 0x3B, 0x01, 0x75, 0x3E, 
                    0x0F, 0x20, 0xC0, 0x50, 0x25, 0xFF, 0xFF, 0xFE, 0xFF, 0x0F, 0x22, 0xC0, 0x8B, 0x56, 0x08, 0x8B, 
                    0xCA, 0xC1, 0xE9, 0x02, 0x8B, 0x76, 0x04, 0x8B, 0x7B, 0x04, 0xF3, 0xA5, 0x8B, 0xCA, 0x83, 0xE1, 
                    0x03, 0xF3, 0xA4, 0x3B, 0x7B, 0x08, 0x75, 0x03, 0xC6, 0x03, 0x00, 0x89, 0x7B, 0x04, 0x58, 0x0F, 
                    0x22, 0xC0, 0x61, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x68, 0xF5, 0xB3, 0x00, 0xB0, 0xC3, 0x61, 0x53, 
                    0xFF, 0x76, 0x08, 0x8D, 0x4D, 0xF8, 0x68, 0xE7, 0xB3, 0x00, 0xB0, 0xC3 };
				bw.Write(hrReceiveFileDataCave);

				fs.Position = 0x37800;
				byte[] hrNullSub = { 0xB8, 0x05, 0x40, 0x00, 0x80, 0xC2, 0x10 };
				bw.Write(hrNullSub);
			}

			return true;
		}
	};
}