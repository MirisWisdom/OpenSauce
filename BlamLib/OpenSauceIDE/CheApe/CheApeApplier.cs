/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	public partial class CheApeApplier : Form
	{
		public CheApeApplier(BlamLib.BlamVersion version)
		{
			InitializeComponent();

			cbEngineVersion.Items.Add(BlamLib.BlamVersion.Halo1_CE);
			cbEngineVersion.Items.Add(BlamLib.BlamVersion.Halo2_PC);

			// NOTE: this isn't the best control logic for this. Will probably want to change this later
				 if (version == BlamLib.BlamVersion.Halo1_CE) cbEngineVersion.SelectedIndex = 0;
			else if (version == BlamLib.BlamVersion.Halo2_PC) cbEngineVersion.SelectedIndex = 1;

// 			txtPathOutput.Text = @"C:\Mount\B\Kornner\Projects\bin\_Halo1\";
// 			txtPathGuerilla.Text = @"C:\Program Files (x86)\Microsoft Games\Halo Custom Edition\guerilla.exe";
// 			txtPathTool.Text = @"C:\Program Files (x86)\Microsoft Games\Halo Custom Edition\tool.exe";
// 			txtPathSapien.Text = @"C:\Program Files (x86)\Microsoft Games\Halo Custom Edition\sapien.exe";

// 			txtPathOutput.Text = @"C:\Mount\B\Kornner\Projects\bin\_Halo2\";
// 			txtPathGuerilla.Text = @"C:\Program Files (x86)\Microsoft Games\Halo 2 Map Editor\H2Guerilla.exe";
// 			txtPathTool.Text = @"C:\Program Files (x86)\Microsoft Games\Halo 2 Map Editor\H2Tool.exe";
// 			txtPathSapien.Text = @"C:\Program Files (x86)\Microsoft Games\Halo 2 Map Editor\H2Sapien.exe";
		}

		void OnPathFind(out string path)
		{
			path = string.Empty;

			if(dlgOpenFile.ShowDialog(this) == DialogResult.OK)
				path = dlgOpenFile.FileName;
		}
		void OnPathGuerilla(object sender, EventArgs e)
		{
			string path;	OnPathFind(out path);	txtPathGuerilla.Text = path;
		}
		void OnPathSapien(object sender, EventArgs e)
		{
			string path;	OnPathFind(out path);	txtPathSapien.Text = path;
		}
		void OnPathTool(object sender, EventArgs e)
		{
			string path;	OnPathFind(out path);	txtPathTool.Text = path;
		}

		void OnPathOutput(object sender, EventArgs e)
		{
			if (dlgFolderBrowser.ShowDialog(this) == DialogResult.OK)
				txtPathOutput.Text = dlgFolderBrowser.SelectedPath;
		}

		void OnApplyException(BlamLib.BlamVersion v, Exception ex)
		{
			BlamLib.Debug.LogFile.WriteLine("CheApe Apply failed in {0}. Reason:{1}{2}", v.ToString(), BlamLib.Program.NewLine, ex);
		}
		void OnApplyMsg(bool is_error, string format, params string[] args)
		{
			string msg = string.Format(format, args);
			string title = is_error ? "Error" : "Success!";

			MessageBox.Show(this, msg, title, MessageBoxButtons.OK,
				is_error ? MessageBoxIcon.Error : MessageBoxIcon.Information);
		}
		void OnApply(BlamLib.BlamVersion v)
		{
			Exception exception = null;
			CheApeInterface.UnlockToolsBase unlocker = null;

			switch(v)
			{
				case BlamLib.BlamVersion.Halo1_CE:
					try						{ unlocker = new CheApeInterface.UnlockH1(txtPathOutput.Text, txtPathGuerilla.Text, txtPathTool.Text, txtPathSapien.Text); }
					catch (Exception ex)	{ exception = ex; }
					break;

				case BlamLib.BlamVersion.Halo2_PC:
					try						{ unlocker = new CheApeInterface.UnlockH2(txtPathOutput.Text, txtPathGuerilla.Text, txtPathTool.Text, txtPathSapien.Text); }
					catch (Exception ex)	{ exception = ex; }
					break;

				default: MessageBox.Show(this, string.Format("the quarter ({0}) isn't under any of the cups you fuckin cheater i kill you", cbEngineVersion.SelectedText),
							"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
			}

			if (unlocker != null) // If the unlocker api was initialized OK, run the unlocking operations
			{
				try
				{
					unlocker.Unlock(false);
					unlocker.Close();
				}
				catch (Exception ex)	{ exception = ex; }
			}

			if (exception != null)
				OnApplyException(v, exception);

			string msg = exception == null ? 
				"CheApe successfully applied!" :
				"There was an error while trying to apply CheApe. Validate that you selected copies of the original tools and try again.";

			if (unlocker.EncounteredInvalidExe)
				OnApplyMsg(true, "CheApe couldn't be applied to some or all of the exes. Check the debug log for more details");
			else
				OnApplyMsg(exception != null, msg);
		}
		void OnApply(object sender, EventArgs e)
		{
			if(cbEngineVersion.SelectedItem == null)
				OnApplyMsg(true, "Select an engine version first!");
			else if (string.IsNullOrEmpty(txtPathOutput.Text))
				OnApplyMsg(true, "Select output directory first!");
			else if (string.IsNullOrEmpty(txtPathGuerilla.Text) && string.IsNullOrEmpty(txtPathSapien.Text) && string.IsNullOrEmpty(txtPathTool.Text))
				OnApplyMsg(true, "At least one executable must be selected!");
			else
			{
				var v = (BlamLib.BlamVersion)cbEngineVersion.SelectedItem;
				OnApply(v);
			}
		}
	};
}

namespace BlamLib
{
	partial class Tool
	{
		static ExitCode UnlockBlamTools(params string[] args)
		{
			if (args.Length < 5)
			{
				Console.WriteLine("error: invalid command argument count");
				return ExitCode.InvalidArgsCount;
			}

			BlamVersion version = BlamVersion.Unknown;
			switch (args[0])
			{
				case "Halo1_CE":	version = BlamVersion.Halo1_CE;	break;
                case "Halo2_PC":	version = BlamVersion.Halo2_PC;	break;
				default:
					Console.WriteLine("error: unsupported engine version - {0}", version.ToString());
					return ExitCode.InvalidArgs;
			};

			Console.WriteLine("Applying {0} modifications...", "CheApe");
			DumpArguments(1, args);

			Exception exception = null;
			OpenSauceIDE.CheApeInterface.UnlockToolsBase unlocker = null;
			switch (version)
			{
				case BlamVersion.Halo1_CE:
					try					{ unlocker = new OpenSauceIDE.CheApeInterface.UnlockH1(args[4], args[1], args[2], args[3]); }
					catch (Exception ex){ exception = ex; }
					break;
				case BlamVersion.Halo2_PC:
					try					{ unlocker = new OpenSauceIDE.CheApeInterface.UnlockH2(args[4], args[1], args[2], args[3]); }
					catch (Exception ex){ exception = ex; }
					break;
			}

			if (unlocker != null) // If the unlocker api was initialized OK, run the unlocking operations
			{
				try
				{
					unlocker.Unlock(false);
					unlocker.Close();
				}
				catch (Exception ex)	{ exception = ex; }
			}

			if (exception != null)
			{
				Console.WriteLine("error: an exception occurred when attempting to apply the {0} modifications", "CheApe");
				Console.WriteLine();
				Console.WriteLine("exception details:");
				if(exception.Message != null) Console.WriteLine(exception.Message);
				Console.WriteLine(exception.StackTrace);
				Console.WriteLine();
			}

			ExitCode exit_code = ExitCode.Success;
			string msg;
			if (exception == null)
				msg = "CheApe successfully applied!";
			else if (unlocker.EncounteredInvalidExe)
			{
				msg = "CheApe couldn't be applied to some or all of the exes. Check the debug log for more details";
				exit_code = ExitCode.InvalidInput;
			}
			else
			{
				msg = "There was an error while trying to apply CheApe. Validate that you selected copies of the original tools and try again.";
				exit_code = ExitCode.Failure;
			}

			Console.WriteLine(msg);

			return exit_code;
		}
	};
}