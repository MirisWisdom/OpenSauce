/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.IO;

namespace OpenSauceIDE.CheApeInterface
{
	partial class UnlockH1 : UnlockToolsBase
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

			#region FixMisc
			protected void ChangeBlockMaxElements(PlatformAddress address, int new_max)
			{
				output.Seek((address[PlatformVersion] - Program.PeAddressMask) + 0x8);
				output.Write(new_max);
			}

			protected void ChangeFieldName(PlatformAddress address, uint string_address)
			{
				output.Seek((address[PlatformVersion] - Program.PeAddressMask) + 0x4);
				output.Write(string_address);
			}

			protected virtual void FixMisc(bool debug)
			{
				//ChangeBlockMaxElements(AddressOf.BlockAnimation, 0x100 * 2); // allow for twice as many animations
				//ChangeBlockMaxElements(AddressOf.BlockModelShaderReference, 0x20 * 2); // allow for twice as many shaders
				//ChangeBlockMaxElements(AddressOf.BlockDamageRegions, 8 * 2); // allow for twice as many regions

				output.Seek(AddressOf.LoadSymbolTableErrorMsg[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.LoadSymbolTableErrorMsgNullCode);
			}
			#endregion

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
				const string k_build_string = "01." + "00." + "09." + "0620";

				output.Seek(AddressOf.LogHeaderVersion[PlatformVersion] - Program.PeAddressMask);
				output.Write(k_build_string, 14);
				output.Seek(-1, SeekOrigin.Current);
				output.Write((byte)' ');
			}
			#endregion

			public override void Unlock(bool debug)
			{
				FixMisc(debug);
				UpdateLogHeader();
				EnableCheApe();
			}
		};

		#region Guerilla
		class Guerilla : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockToolsBase.ValidateExe(path, 0x138, 0x40969651);
			}

			public Guerilla(string output_dir, string path) : base(output_dir, path, Platform.Guerilla) { }

			#region EnableBaseObjectCreation
			void EnableBaseObjectCreation()
			{
			}
			#endregion

			#region FixNullFields
			void FixNullFields()
			{
				for (int x = 0; x < AddressOf.NullFields.Length; x++)
					ChangeFieldName(AddressOf.NullFields[x], AddressOf.FieldString[Platform.Guerilla]);
			}
			#endregion

			#region FixReadonlyBlankFields
			void FixReadonlyBlankFields()
			{
			}
			#endregion

			#region FixReadonlyFieldNames
			void FixReadonlyFieldNames()
			{
				BlamLib.Debug.Assert.If(AddressOf.ReadonlyFieldNames.Length == Constants.ReadonlyFieldNames.Length);

				for(int x = 0; x < AddressOf.ReadonlyFieldNames.Length; x++)
				{
					output.Seek(AddressOf.ReadonlyFieldNames[x][Platform.Guerilla] - Program.PeAddressMask);
					output.Write(Constants.ReadonlyFieldNames[x], true);
				}
			}
			#endregion

			#region FixMisc
			protected override void FixMisc(bool debug)
			{
				base.FixMisc(debug);

				for(int x = 0; x < 2; x++)
				{
					output.Seek(AddressOf.fix_inertial_matrix_block_fields[x][Platform.Guerilla] - Program.PeAddressMask);
					output.Write(Constants.fix_inertial_matrix_block_fields[x], true);
				}

				for (int x = 0; x < 3; x++)
				{
					output.Seek(AddressOf.FixReadonlyTags[x][Platform.Guerilla] - Program.PeAddressMask);
					output.Write(AddressOf.NullString[Platform.Guerilla]);
				}
			}
			#endregion

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);

				if (debug) EnableBaseObjectCreation();
				FixNullFields();
				FixReadonlyBlankFields();
				FixReadonlyFieldNames();
			}

		}; Guerilla GuerillaInterface = null;
		#endregion

		#region Tool
		class Tool : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockToolsBase.ValidateExe(path, 0x130, 0x408EE21B);
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
				return UnlockToolsBase.ValidateExe(path, 0x138, 0x4096942C);
			}

			public Sapien(string output_dir, string path) : base(output_dir, path, Platform.Sapien) { }

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);
			}

		}; Sapien SapienInterface = null;
		#endregion

		public UnlockH1(string output_dir, string g_path, string t_path, string s_path)
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