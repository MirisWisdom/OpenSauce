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
				bool no_errors = true;

				var v = (BlamLib.BlamVersion)cbEngineVersion.SelectedItem;//Enum.Parse(typeof(BlamLib.BlamVersion), cbEngineVersion.SelectedText);
				switch(v)
				{
					case BlamLib.BlamVersion.Halo1_CE:
						try
						{
							var h1 = new CheApeInterface.UnlockH1(txtPathOutput.Text, txtPathGuerilla.Text, txtPathTool.Text, txtPathSapien.Text);
							h1.Unlock(false);
							h1.Close();
						}
						catch (Exception ex)
						{
							BlamLib.Debug.LogFile.WriteLine("CheApe Apply failed in {0}. Reason:{1}{2}", v.ToString(), BlamLib.Program.NewLine, ex);
							no_errors = false;
						}
						break;

					case BlamLib.BlamVersion.Halo2_PC:
						try
						{
							var h2 = new CheApeInterface.UnlockH2(txtPathOutput.Text, txtPathGuerilla.Text, txtPathTool.Text, txtPathSapien.Text);
							h2.Unlock(false);
							h2.Close();
						}
						catch (Exception ex)
						{
							BlamLib.Debug.LogFile.WriteLine("CheApe Apply failed in {0}. Reason:{1}{2}", v.ToString(), BlamLib.Program.NewLine, ex);
							no_errors = false;
						}
						break;

					default: MessageBox.Show(this, string.Format("the quarter ({0}) isn't under any of the cups you fuckin cheater i kill you", cbEngineVersion.SelectedText),
								"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
				}

				string msg = !no_errors ? 
					"CheApe successfully applied!" :
					"There was an error while trying to apply CheApe. Validate that you selected copies of the original tools and try again.";

				MessageBox.Show(this, msg,
					!no_errors ? "Success!" : "Error", 
					MessageBoxButtons.OK,
					!no_errors ? MessageBoxIcon.Information : MessageBoxIcon.Error);
			}
		}
	};
}