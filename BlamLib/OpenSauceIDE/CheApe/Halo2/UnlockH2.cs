/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace OpenSauceIDE.CheApeInterface
{
	partial class UnlockH2 : UnlockToolsBase
	{
		abstract class PlatformInterface : PlatformInterfaceBase
		{
			public PlatformInterface(string output_dir, string path, Platform pv) : base(pv)
			{
				if (!ValidateExe(path))
					throw new BlamLib.Debug.ExceptionLog("{0} is not a supported {1} exe", path, pv.ToString());

				string dir = string.IsNullOrEmpty(output_dir) ? Path.GetDirectoryName(path) : output_dir;
				string name = Path.GetFileName(path);
				name = Path.Combine(dir, "OS_" + name);
				File.Copy(path, name, true);
				output = new BlamLib.IO.EndianWriter(name);
				output.BaseAddress = Program.PeAddressMask;
			}

			#region EnableCheApe
			void EnableCheApe()
			{
				output.Seek(AddressOf.CheApeDllString[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.CheApeDllString[PlatformVersion]);

				output.Seek(AddressOf.CheApeFunction1[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.CheApeFunction1[PlatformVersion]);

				output.Seek(AddressOf.CheApeFunction2[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.CheApeFunction2[PlatformVersion]);
			}
			#endregion

			#region UpdateLogHeader
			protected void UpdateLogHeader()
			{
				const string k_build_string = "1" + "20" + "81";

				output.Seek(AddressOf.LogHeaderVersion[PlatformVersion] - Program.PeAddressMask);
				output.Write(k_build_string, 6);
				output.Seek(-1, SeekOrigin.Current);
				output.Write((byte)' ');
			}
			#endregion

			public override void Unlock(bool debug)
			{
				//UpdateLogHeader();
				EnableCheApe();
			}
		};

		#region Guerilla
		partial class Guerilla : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockToolsBase.ValidateExe(path, 0x118, 0x46361B50);
			}

			// Add a menu id of 0xA85D to perform Tag Exports

			public Guerilla(string output_dir, string path) : base(output_dir, path, Platform.Guerilla) { }
		
			void FixCode()
			{
				// Fix a duplicate call to tag_group_get_next in the new_dialog code
				output.Seek(0x9FC1);
				output.Write((byte)0x90);
				output.Write((ushort)0x9090);
				output.Write((uint)0x90909090);
			}

			#region EnableBaseObjectCreation
			void EnableBaseObjectCreation()
			{
				// original code 66837B68000F8586000000
				output.Seek(0x9FE0);
				output.Write((uint)0x90909090);
				output.Write((uint)0x90909090);
				output.Write((ushort)0x9090);
				output.Write((byte)0x90);
			}
			#endregion

			#region AllowAllTags
			void AllowAllTags(bool debug)
			{
				// new_dialog
				if (!debug)
				{
					// Hack code which I added for allowing "disabled" tags for beta releases
					output.Seek(0x9FE3);
					// cmp     word ptr [ebx+22h], 0
					output.Write((byte)0x22);
				}
				output.Seek(0x9FEB);
				// jmp 40A011
				output.Write((ushort)0x24EB); // EB24

				// tag tree
				output.Seek(0x476E7);
				// jmp 44770A
				output.Write((ushort)0x24EB); // EB24

				output.Seek(0x7FB10);
				// jmp 47FB71
				output.Write((ushort)0x5FEB); // EB5F

				output.Seek(0x7FB81);
				// jmp 47FC5F
				output.Write((ushort)0xD9E9); // E9D9
				output.Write((ushort)0x0000);
				output.Write((byte)0x00);
			}
			#endregion

			#region DisallowTags
			bool IsDisallowedTag(string name)
			{
				foreach (string str in DisallowTagNames)
					if (name == str) return true;
				return false;
			}
			void DisallowTags()
			{
				BlamLib.IO.EndianReader input = new BlamLib.IO.EndianReader(output.FileName);
				input.BaseAddress = Program.PeAddressMask;
				input.Seek(AddressOf.TagGroupDefinitions[Platform.Guerilla] - Program.PeAddressMask);

				uint[] offsets = new uint[Constants.StockTagGroupCount];
				for (int x = 0; x < offsets.Length; x++)
					offsets[x] = input.ReadPointer();

				for (int x = 0; x < offsets.Length; x++)
				{
					input.Seek(offsets[x]);
					string name = Program.ReadCString(input.ReadPointer(), input);
					if (IsDisallowedTag(name))
					{
						input.Seek(0x20, System.IO.SeekOrigin.Current);
						output.Seek(input.Position - 2);
						output.Write((short)1);
						output.Seek(input.ReadPointer() + 0x14);
						output.Write(0);
						output.Write(0);
						output.Write(0);
					}
				}
				input.Close();
			}
			#endregion

			#region DisallowFields
			void DisallowFields()
			{
			}
			#endregion

			#region AllowAllFields
			void AllowAllFields()
			{
				output.Seek(0x7FEF2);
				// jmp 47FF2B
				output.Write((ushort)0x37EB); // EB37

				// enum limiter
				output.Seek(0x501928);
				output.Write((int)0);
			}
			#endregion

			#region FixFieldNames
			void FixFieldNames()
			{
				for (int x = 0; x < FixListNameIds.Length; x++)
				{
					output.Seek(FixListNameIds[x]);
					output.Write(AddressOf.BlamString[Platform.Guerilla]);
				}

				for (int x = 0; x < FixListNames.Length; x++)
				{
					output.Seek(FixListNames[x]);
					output.Write(AddressOf.BlamString[Platform.Guerilla]);
				}

				for(int x = 0; x < FixListHackNames.Length; x+=2)
				{
					output.Seek(FixListHackNames[x] + 4);
					output.Write(FixListHackNames[x + 1] + output.BaseAddress);
				}
			}
			#endregion

			#region EnableNewLangDll
			void EnableNewLangDll()
			{
				// write the new dll's name
				// L"h2lang++.dll"
				byte[] code = BlamLib.Util.ByteStringToArray("6800320061006C0061006E0067002B002B002E0064006C006C000000");
				output.Seek(0x36D634);
				output.Write(code);

				// write the string we overwrote with the data above to a new place
				// L"GUERILLA"
				code = BlamLib.Util.ByteStringToArray("470055004500520049004C004C0041000000");
				output.Seek(0x36D684);
				output.Write(code);

				// update the pointer to that string above
				code = BlamLib.Util.ByteStringToArray("84D67600");
				output.Seek(0x3850);
				output.Write(code);
			}
			#endregion

			#region DisableTagTemplateViews
			void DisableTagTemplateViews()
			{
				output.Seek(0x8E730);
				output.Write((byte)0x33);
				output.Write((byte)0xC0);
				output.Write((byte)0xC3);
			}
			#endregion

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);

				//FixCode();
				if (debug) EnableBaseObjectCreation();
				if (debug) AllowAllFields();
				if (debug) FixFieldNames();
				AllowAllTags(debug);
				if (!debug) DisallowTags();
				if (!debug) DisallowFields();
				EnableNewLangDll();
				if (!debug) DisableTagTemplateViews();
			}
		}; Guerilla GuerillaInterface = null;
		#endregion

		#region Tool
		class Tool : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockToolsBase.ValidateExe(path, 0x108, 0x46361B4D);
			}

			public Tool(string output_dir, string path) : base(output_dir, path, Platform.Tool) { }

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);
			}
		}; Tool ToolInterface = null;
		#endregion

		#region Sapien
		class Sapien : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockToolsBase.ValidateExe(path, 0x110, 0x46361B42);
			}

			public Sapien(string output_dir, string path) : base(output_dir, path, Platform.Sapien) { }

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);
			}

		}; Sapien SapienInterface = null;
		#endregion

		public UnlockH2(string output_dir, string g_path, string t_path, string s_path)
		{
			if (!string.IsNullOrEmpty(g_path))
				try { GuerillaInterface = new Guerilla(output_dir, g_path); }
				catch (BlamLib.Debug.ExceptionLog) { EncounteredInvalidExe = true; }

			if (!string.IsNullOrEmpty(t_path))
				try { ToolInterface = new Tool(output_dir, t_path); }
				catch (BlamLib.Debug.ExceptionLog) { EncounteredInvalidExe = true; }

			if (!string.IsNullOrEmpty(s_path))
				try { SapienInterface = new Sapien(output_dir, s_path); }
				catch (BlamLib.Debug.ExceptionLog) { EncounteredInvalidExe = true; }
		}

		public override void Close()
		{
			if (GuerillaInterface != null) GuerillaInterface.Close();
			if (ToolInterface != null) ToolInterface.Close();
			if (SapienInterface != null) SapienInterface.Close();
		}

		public override void Unlock(bool debug)
		{
			if (GuerillaInterface != null) GuerillaInterface.Unlock(debug);
			if (ToolInterface != null) ToolInterface.Unlock(debug);
			if (SapienInterface != null) SapienInterface.Unlock(debug);
		}
	};
}