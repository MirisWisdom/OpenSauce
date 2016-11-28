using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using BlamLib;

namespace OpenSauceIDE
{
	public partial class EngineSettingsForm : Form
	{
		/// <summary>The valid engines which the settings editor can be used with</summary>
		public const BlamLib.BlamVersion kAllowedPlatforms = 
			BlamLib.BlamVersion.Halo1 | /*BlamLib.BlamVersion.Stubbs |*/
			BlamLib.BlamVersion.Halo2 |
			BlamLib.BlamVersion.Halo3 | BlamLib.BlamVersion.HaloOdst | BlamLib.BlamVersion.HaloReach;

		const string kDlgName = "Engine Settings";
		void SetDialogName(BlamVersion version)
		{
			this.Text = string.Format("{0} - [{1}]", kDlgName, version.ToString());
		}

		public EngineSettingsForm(BlamVersion version)
		{
			InitializeComponent();

			SetDialogName(version);

			object settings = null;
			switch (version.ToBuild())
			{
				case BlamBuild.Halo1: settings = BlamLib.Program.Halo1.Manager.Settings; break;
				case BlamBuild.Halo2: settings = BlamLib.Program.Halo2.Manager.Settings; break;
				case BlamBuild.Halo3: settings = BlamLib.Program.Halo3.Manager.Settings; break;
				case BlamBuild.HaloOdst: settings = BlamLib.Program.HaloOdst.Manager.Settings; break;
				case BlamBuild.HaloReach: settings = BlamLib.Program.HaloReach.Manager.Settings; break;
				case BlamBuild.Stubbs: settings = BlamLib.Program.Stubbs.Manager.Settings; break;
			}

			PropGrid.SelectedObject = settings;
		}

		void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			var settings = PropGrid.SelectedObject as BlamLib.Managers.BlamDefinition.SettingsInterface;

			if (settings != null)
				e.Cancel = !settings.ValidateSettings();

			if (e.Cancel)
			{
				MessageBox.Show(this, "Provided settting(s) were invalid.\n" + 
					"Check to make sure all paths exist or set them to an empty string.\n" + 
					"If all else fails, learn to program and figure out the problem yourself.",
					"Invalid Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}
	};
}