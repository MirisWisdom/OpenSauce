/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;
using System.ComponentModel;
using System.Windows.Forms;

// Great news everyone! I can hack together code with one hand tied around my back
// (V) (°,,,,°) (V)

// We need to patch XnaNative.dll to support the correct window size used for 
// compressing (LZX) .s3dpak files and HA10 .map files. XnaNative is actually 
// built on the XDK's xcompress.lib, but the XNA framework is freely available

namespace OpenSauceIDE.OpenSauce
{
	public partial class XnaPatcherForm : Form
	{
		public XnaPatcherForm()
		{
			InitializeComponent();
		}

		void OnPathFind(out string path)
		{
			path = string.Empty;

			if (dlgOpenFile.ShowDialog(this) == DialogResult.OK)
				path = dlgOpenFile.FileName;
		}

		void OnPathXnaNative(object sender, EventArgs e)
		{
			string path; OnPathFind(out path); txtPathXnaNative.Text = path;
		}

		void OnPathOutput(object sender, EventArgs e)
		{
			if (dlgFolderBrowser.ShowDialog(this) == DialogResult.OK)
				txtPathOutput.Text = dlgFolderBrowser.SelectedPath;
		}

		void OnApplyException(Exception ex)
		{
			BlamLib.Debug.LogFile.WriteLine("XnaPatcher Apply failed in. Reason:{0}{1}", BlamLib.Program.NewLine, ex);
		}
		void OnApplyMsg(bool is_error, string format, params string[] args)
		{
			string msg = string.Format(format, args);
			string title = is_error ? "Error" : "Success!";

			MessageBox.Show(this, msg, title, MessageBoxButtons.OK,
				is_error ? MessageBoxIcon.Error : MessageBoxIcon.Information);
		}
		void OnApply()
		{
			Exception exception = null;
			OpenSauceIDE.GameInterface.UnlockXna patcher = null;
			try					{ patcher = new OpenSauceIDE.GameInterface.UnlockXna(txtPathOutput.Text, txtPathXnaNative.Text); }
			catch (Exception ex){ exception = ex; }

			if (patcher != null) // If the patcher api was initialized OK, run the unlocking operations
			{
				try
				{
					patcher.Unlock(false);
					patcher.Close();
				}
				catch (Exception ex)	{ exception = ex; }
			}

			if (exception != null)
				OnApplyException(exception);

			string msg = exception == null ?
				"XNA successfully patcher!" :
				"There was an error while trying to patch the XNA. Validate that you selected copies of the original exes and try again.";

			if (patcher.EncounteredInvalidExe)
				OnApplyMsg(true, "Couldn't patch some or all of the exes. Check the debug log for more details");
			else
				OnApplyMsg(exception != null, msg);
		}
		void OnApply(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(txtPathOutput.Text))
				OnApplyMsg(true, "Select output directory first!");
			else if (string.IsNullOrEmpty(txtPathXnaNative.Text))
				OnApplyMsg(true, "At least one executable must be selected!");
			else
				OnApply();
		}
	};
}

namespace OpenSauceIDE.GameInterface
{
	class UnlockXna : UnlockExeBase
	{
		#region XnaNative
		class XnaNative : PlatformInterfaceGenericBase
		{
			static readonly uint[] kWindowSizeFixupOffsets_v40 = { 0x42D, 0x4CA };

			protected override bool ValidateExe(string path)
			{
				// v4.0
				return UnlockExeBase.ValidateExe(path, 0x128, 0x4C72CCEA);
			}

			public XnaNative(string output_dir, string path)
			{
				if (!ValidateExe(path))
					throw new BlamLib.Debug.ExceptionLog("{0} is not a supported {1} dll", path, "XnaNative");

				string dir = string.IsNullOrEmpty(output_dir) ? Path.GetDirectoryName(path) : output_dir;
				string name = Path.GetFileName(path);
				name = Path.Combine(dir, "OS_" + name);
				File.Copy(path, name, true);
				output = new BlamLib.IO.EndianWriter(name);
				output.BaseAddress = Program.PeAddressMask;
			}

			public override void Unlock(bool debug)
			{
				foreach (uint offset in kWindowSizeFixupOffsets_v40)
				{
					output.Seek(offset);
					output.Write((byte)0x2); // Window size needs to be 1<<17
				}
			}

		}; XnaNative XnaNativeInterface = null;
		#endregion

		public UnlockXna(string output_path, string xna_native_path)
		{
			xna_native_path += "XnaNative.dll";

			if (!string.IsNullOrEmpty(xna_native_path))
				try { XnaNativeInterface = new XnaNative(output_path, xna_native_path); }
				catch (BlamLib.Debug.ExceptionLog) { EncounteredInvalidExe = true; }
		}

		public override void Close()
		{
			if (XnaNativeInterface != null) XnaNativeInterface.Close();
		}

		public override void Unlock(bool debug)
		{
			if (XnaNativeInterface != null) XnaNativeInterface.Unlock(debug);
		}
	};
}

namespace BlamLib
{
	partial class Tool
	{
		static ExitCode PatchXna(params string[] args)
		{
			const string kModifcationName = "OpenSauce-XNA";
			const string kSupportedExeVersion = "4.0";

			if (args.Length < 1)
			{
				Console.WriteLine("error: invalid command argument count");
				return ExitCode.InvalidArgsCount;
			}

			string output_path = args[0];
			string hint_path = "";
			if (args.Length >= 2) hint_path = args[1];
			else
			{
				hint_path = Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles);
				hint_path = Path.Combine(hint_path, @"Microsoft Shared\XNA\Framework\v" + kSupportedExeVersion + @"\");
			}

			if (!Directory.Exists(output_path))
			{
				Console.WriteLine("Output path does not exist: {0}", output_path);
				return ExitCode.InvalidArgs;
			}
			if (!Directory.Exists(hint_path))
			{
				Console.WriteLine("XnaNative path does not exist: {0}", hint_path);
				return ExitCode.InvalidArgs;
			}

			Console.WriteLine("Applying {0} modifications...", kModifcationName);
			DumpArguments(0, args);

			Exception exception = null;
			OpenSauceIDE.GameInterface.UnlockXna patcher = null;
			try					{ patcher = new OpenSauceIDE.GameInterface.UnlockXna(output_path, hint_path); }
			catch (Exception ex){ exception = ex; }

			if (patcher != null) // If the patcher api was initialized OK, run the unlocking operations
			{
				try
				{
					patcher.Unlock(false);
					patcher.Close();
				}
				catch (Exception ex)	{ exception = ex; }
			}

			DumpModificationException(kModifcationName, exception);

			ExitCode exit_code = ExitCode.Success;
			string msg;
			if (exception == null)
				msg = kModifcationName + " successfully applied!";
			else if (patcher.EncounteredInvalidExe)
			{
				msg = kModifcationName + " couldn't be applied to some or all of the exes. Check the debug log for more details";
				exit_code = ExitCode.InvalidInput;
			}
			else
			{
				msg = "There was an error while trying to apply " + kModifcationName + ". Validate that you selected original copies of the " +
					"v" + kSupportedExeVersion + " exe(s) and try again.";
				exit_code = ExitCode.Failure;
			}

			Console.WriteLine(msg);

			return exit_code;
		}
	};
}