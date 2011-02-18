/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
﻿using System;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	public partial class CheApeApplier : Form
	{
		public CheApeApplier()
		{
			InitializeComponent();

			cbEngineVersion.Items.Add(BlamLib.BlamVersion.Halo1_CE);
			cbEngineVersion.Items.Add(BlamLib.BlamVersion.Halo2_PC);

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

		void OnAppleException(BlamLib.BlamVersion v, Exception ex)
		{
			BlamLib.Debug.LogFile.WriteLine("CheApe Apply failed in {0}. Reason:{1}{2}", v.ToString(), BlamLib.Program.NewLine, ex);
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
				OnAppleException(v, exception);

			string msg = exception == null ? 
				"CheApe successfully applied!" :
				"There was an error while trying to apply CheApe. Validate that you selected copies of the original tools and try again.";

			MessageBox.Show(this, msg,
				exception==null ? "Success!" : "Error", 
				MessageBoxButtons.OK,
				exception==null ? MessageBoxIcon.Information : MessageBoxIcon.Error);
		}
		void OnApply(object sender, EventArgs e)
		{
			if(cbEngineVersion.SelectedItem == null)
				MessageBox.Show(this, "Select an engine version first!",
					"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			else if (string.IsNullOrEmpty(txtPathOutput.Text))
				MessageBox.Show(this, "Select output directory first!", 
					"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			else if (string.IsNullOrEmpty(txtPathGuerilla.Text) && string.IsNullOrEmpty(txtPathSapien.Text) && string.IsNullOrEmpty(txtPathTool.Text))
				MessageBox.Show(this, "At least one executable must be selected!",
					"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
		static void UnlockBlamTools(System.Collections.Generic.List<string> args)
		{
			if (args.Count < 5)
			{
				Console.WriteLine("error: invalid command argument count");
				return;
			}

			BlamVersion version = BlamVersion.Unknown;
			switch (args[0])
			{
				case "Halo1_CE":	version = BlamVersion.Halo1_CE;	break;
                case "Halo2_PC":	version = BlamVersion.Halo2_PC;	break;
				default:
					Console.WriteLine("error: unsupported engine version - {0}", version.ToString());
					break;
			};
			
			Console.WriteLine("Applying CheApe modifications...");
			Exception exception = null;
			OpenSauceIDE.CheApeInterface.UnlockToolsBase unlocker = null;
			switch (version)
			{
				case BlamVersion.Halo1_CE:
					try					{ unlocker = new OpenSauceIDE.CheApeInterface.UnlockH1(args[4], args[1], args[2], args[3]); }
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
				Console.WriteLine("error: an exception occurred when attempting to apply the CheApe modifications");
				Console.WriteLine();
				Console.WriteLine("exception details:");
				Console.WriteLine(exception.Message);
				Console.WriteLine();
			}

			string msg = exception==null ?
				"CheApe successfully applied!" :
				"There was an error while trying to apply CheApe. Validate that you selected copies of the original tools and try again.";
			Console.WriteLine(msg);
		}
	};
}