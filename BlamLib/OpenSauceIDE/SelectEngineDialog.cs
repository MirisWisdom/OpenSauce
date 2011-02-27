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

		void cbEngineVersionPopulate(BlamVersion valid_vers, BlamVersion[] invalid_versions)
		{
			cbEngineVersion.BeginUpdate();

			var platform = valid_vers.ToPlatform();
			bool has_platform = platform != BlamPlatform.Unknown;
			var items = cbEngineVersion.Items;

			#region Halo1
			if (!has_platform)
			{	if (valid_vers.HasFlag(BlamVersion.Halo1))		items.Add(BlamVersion.Halo1); }
			else
			{
				if (valid_vers.HasFlag(BlamVersion.Halo1_Xbox)) items.Add(BlamVersion.Halo1_Xbox);
				if (valid_vers.HasFlag(BlamVersion.Halo1_CE))	items.Add(BlamVersion.Halo1_CE); // CE is an extended version so it will always be added with Halo1_PC flags
				if (valid_vers.HasFlag(BlamVersion.Halo1_PC))	items.Add(BlamVersion.Halo1_PC);
				if (valid_vers.HasFlag(BlamVersion.Halo1_Mac))	items.Add(BlamVersion.Halo1_Mac);
			}
			#endregion

			#region Halo2
			if (!has_platform)
			{	if (valid_vers.HasFlag(BlamVersion.Halo2))		items.Add(BlamVersion.Halo2); }
			else
			{
				if (valid_vers.HasFlag(BlamVersion.Halo2_Alpha))items.Add(BlamVersion.Halo2_Alpha);
				if (valid_vers.HasFlag(BlamVersion.Halo2_Xbox)) items.Add(BlamVersion.Halo2_Xbox);
				if (valid_vers.HasFlag(BlamVersion.Halo2_PC))	items.Add(BlamVersion.Halo2_PC);
			}
			#endregion

			#region Halo3
			if (!has_platform)
			{	if (valid_vers.HasFlag(BlamVersion.Halo3))		items.Add(BlamVersion.Halo2); }
			else
			{
				if (valid_vers.HasFlag(BlamVersion.Halo3_Beta)) items.Add(BlamVersion.Halo3_Beta);
				if (valid_vers.HasFlag(BlamVersion.Halo3_Xbox)) items.Add(BlamVersion.Halo3_Xbox);
				//if (valid_vers.HasFlag(BlamVersion.Halo3_PC))	items.Add(BlamVersion.Halo3_PC);
			}
			#endregion

			#region HaloOdst
			if (!has_platform)
			{	if (valid_vers.HasFlag(BlamVersion.HaloOdst))		items.Add(BlamVersion.HaloOdst); }
			else
			{
				if (valid_vers.HasFlag(BlamVersion.HaloOdst_Xbox))	items.Add(BlamVersion.HaloOdst_Xbox);
				//if (valid_vers.HasFlag(BlamVersion.HaloOdst_PC))	items.Add(BlamVersion.HaloOdst_PC);
			}
			#endregion

			#region HaloReach
			if (!has_platform)
			{	if (valid_vers.HasFlag(BlamVersion.HaloReach))		items.Add(BlamVersion.HaloReach); }
			else
			{
				if (valid_vers.HasFlag(BlamVersion.HaloReach_Beta)) items.Add(BlamVersion.HaloReach_Beta);
				if (valid_vers.HasFlag(BlamVersion.HaloReach_Xbox)) items.Add(BlamVersion.HaloReach_Xbox);
				//if (valid_vers.HasFlag(BlamVersion.HaloReach_PC))	items.Add(BlamVersion.HaloReach_PC);
			}
			#endregion

			#region Stubbs
			if (!has_platform)
			{	if (valid_vers.HasFlag(BlamVersion.Stubbs))		items.Add(BlamVersion.Stubbs); }
			else
			{
				if (valid_vers.HasFlag(BlamVersion.Stubbs_Xbox))items.Add(BlamVersion.Stubbs_Xbox);
				if (valid_vers.HasFlag(BlamVersion.Stubbs_PC))	items.Add(BlamVersion.Stubbs_PC);
				if (valid_vers.HasFlag(BlamVersion.Stubbs_Mac))	items.Add(BlamVersion.Stubbs_Mac);
			}
			#endregion

			foreach (var ver in invalid_versions)
			{
				object verobj = ver;
				if (items.Contains(verobj))
					items.Remove(verobj);
			}

			cbEngineVersion.EndUpdate();
		}
		public SelectEngineDialog(BlamVersion valid_versions, params BlamVersion[] invalid_versions)
		{
			InitializeComponent();
			cbEngineVersionPopulate(valid_versions, invalid_versions);

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

		public DialogResult ShowDialogWithResult(IWin32Window owner, out BlamVersion version)
		{
			var dr = this.ShowDialog(owner);
			version = this.SelectedEngineVersion;

			return dr;
		}
	}
}