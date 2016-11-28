/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;

namespace OpenSauceIDE
{
	abstract class UnlockExeBase
	{
		public bool EncounteredInvalidExe = false;
		protected static bool ValidateExe(string path, long time_stamp_offset, uint time_stamp)
		{
			using (var fs = new System.IO.FileStream(path, System.IO.FileMode.Open, System.IO.FileAccess.Read))
			using (var s = new System.IO.BinaryReader(fs))
			{
				if (fs.Length > (time_stamp_offset+4))
				{
					fs.Seek(time_stamp_offset, System.IO.SeekOrigin.Begin);
					uint ts = s.ReadUInt32();

					return ts == time_stamp;
				}
			}

			return false;
		}

		public abstract void Close();

		public abstract void Unlock(bool debug);

		protected abstract class PlatformInterfaceGenericBase
		{
			protected BlamLib.IO.EndianWriter output;

			public void Close() { output.Close(); }

			public abstract void Unlock(bool debug);

			protected abstract bool ValidateExe(string path);
		};
	};
}