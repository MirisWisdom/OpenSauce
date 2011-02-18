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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using BlamLib;

namespace OpenSauceIDE
{
	/// <summary>Dialog used to force the user to select a specific engine (plus platform)</summary>
	/// <example>
	/// var sed = new SelectEngineDialog(BlamLib.BlamVersion.kAllEngines);
	///	if(sed.ShowDialog(this) == DialogResult.OK)
	///		sed.SelectedEngineVersion.ToString();
	/// </example>
	public partial class SelectEngineDialog : Form
	{
		/// <summary>The user's selected engine, or <see cref="BlamVersion.Unknown"/> if invalid or nothing was selected</summary>
		public BlamVersion SelectedEngineVersion { get; private set; }

		void cbEngineVersionPopulate(BlamVersion valid_vers)
		{
			cbEngineVersion.BeginUpdate();

			var platform = valid_vers.ToPlatform();
			bool has_platform = platform != BlamPlatform.Unknown;

			#region Halo1
			if (!has_platform)
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo1))		cbEngineVersion.Items.Add(BlamVersion.Halo1);
			else
			{
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo1_Xbox)) cbEngineVersion.Items.Add(BlamVersion.Halo1_Xbox);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo1_PC))	cbEngineVersion.Items.Add(BlamVersion.Halo1_PC);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo1_CE))	cbEngineVersion.Items.Add(BlamVersion.Halo1_CE);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo1_Mac))	cbEngineVersion.Items.Add(BlamVersion.Halo1_Mac);
			}
			#endregion

			#region Halo2
			if (!has_platform)
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo2))		cbEngineVersion.Items.Add(BlamVersion.Halo2);
			else
			{
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo2_Alpha))cbEngineVersion.Items.Add(BlamVersion.Halo2_Alpha);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo2_Xbox)) cbEngineVersion.Items.Add(BlamVersion.Halo2_Xbox);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo2_PC))	cbEngineVersion.Items.Add(BlamVersion.Halo2_PC);
			}
			#endregion

			#region Halo3
			if (!has_platform)
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo3))		cbEngineVersion.Items.Add(BlamVersion.Halo2);
			else
			{
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo3_Beta)) cbEngineVersion.Items.Add(BlamVersion.Halo3_Beta);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo3_Xbox)) cbEngineVersion.Items.Add(BlamVersion.Halo3_Xbox);
				//if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Halo3_PC))	cbEngineVersion.Items.Add(BlamVersion.Halo3_PC);
			}
			#endregion

			#region HaloOdst
			if (!has_platform)
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloOdst))		cbEngineVersion.Items.Add(BlamVersion.HaloOdst);
			else
			{
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloOdst_Xbox))	cbEngineVersion.Items.Add(BlamVersion.HaloOdst_Xbox);
				//if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloOdst_PC))	cbEngineVersion.Items.Add(BlamVersion.HaloOdst_PC);
			}
			#endregion

			#region HaloReach
			if (!has_platform)
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloReach))		cbEngineVersion.Items.Add(BlamVersion.HaloReach);
			else
			{
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloReach_Beta)) cbEngineVersion.Items.Add(BlamVersion.HaloReach_Beta);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloReach_Xbox)) cbEngineVersion.Items.Add(BlamVersion.HaloReach_Xbox);
				//if (BlamExtensions.HasFlag(valid_vers, BlamVersion.HaloReach_PC))	cbEngineVersion.Items.Add(BlamVersion.HaloReach_PC);
			}
			#endregion

			#region Stubbs
			if (!has_platform)
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Stubbs))		cbEngineVersion.Items.Add(BlamVersion.Stubbs);
			else
			{
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Stubbs_Xbox))cbEngineVersion.Items.Add(BlamVersion.Stubbs_Xbox);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Stubbs_PC))	cbEngineVersion.Items.Add(BlamVersion.Stubbs_PC);
				if (BlamExtensions.HasFlag(valid_vers, BlamVersion.Stubbs_Mac))	cbEngineVersion.Items.Add(BlamVersion.Stubbs_Mac);
			}
			#endregion

			cbEngineVersion.EndUpdate();
		}
		public SelectEngineDialog(BlamVersion valid_versions)
		{
			InitializeComponent();
			cbEngineVersionPopulate(valid_versions);

			SelectedEngineVersion = BlamVersion.Unknown;
		}
		public SelectEngineDialog() : this(BlamVersion.kAll) {}

		void OnOK(object sender, EventArgs e)
		{
			SelectedEngineVersion = cbEngineVersion.SelectedItem is BlamVersion ?
				(BlamVersion)cbEngineVersion.SelectedItem : BlamVersion.Unknown;
		}

		void OnCancel(object sender, EventArgs e)
		{
			SelectedEngineVersion = BlamVersion.Unknown;
		}
	}
}