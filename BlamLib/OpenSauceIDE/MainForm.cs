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
using System.Drawing;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	public partial class MainForm : Form
	{
		#region CheApe menus
		void BuildCheApeMenusCheApeApply(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler CheApeApply_handler;
			var CheApeApply = BlamLib.Forms.Util.CreateMenuItem("CheApe Applier", CheApeApply_handler = delegate(object sender, EventArgs e)
			{
				var sed = new SelectEngineDialog(OpenSauceIDE.CheApe.kTargetPlatforms, BlamLib.BlamVersion.Halo1_PC);
				BlamLib.BlamVersion version;
				sed.ShowDialogWithResult(this, out version);

				if (version != BlamLib.BlamVersion.Unknown)
					new CheApeApplier(version).ShowDialog(this);
			});
			CheApeApply.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			CheApeApply.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(CheApeApply);
			command_dic.Add("CheApeApplier", CheApeApply_handler);
		}
		void BuildCheApeMenusCheApe(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler CheApe_handler;
			var CheApe = BlamLib.Forms.Util.CreateMenuItem("CheApe", CheApe_handler = delegate(object sender, EventArgs e)
			{
				var sed = new SelectEngineDialog(OpenSauceIDE.CheApe.kTargetPlatforms, BlamLib.BlamVersion.Halo1_PC,
					BlamLib.BlamVersion.Halo2_PC); // NOTE: H2PC still needs some work, so don't display it
				BlamLib.BlamVersion version;
				sed.ShowDialogWithResult(this, out version);

				if(version != BlamLib.BlamVersion.Unknown)
					AddMdiChild(new CheApe(version));
			});
			CheApe.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			CheApe.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(CheApe);
			command_dic.Add("CheApe", CheApe_handler);
		}
		void BuildCheApeMenus(Dictionary<string, EventHandler> command_dic)
		{
			BuildCheApeMenusCheApeApply(command_dic);
			BuildCheApeMenusCheApe(command_dic);
		}
		#endregion

		void BuildCacheMenus(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler OpenCache_handler;
			var OpenCache = BlamLib.Forms.Util.CreateMenuItem("Open Cache", OpenCache_handler = delegate(object sender, EventArgs e)
			{
				var sed = new SelectEngineDialog(OpenSauceIDE.Cache.CacheView.kAllowedPlatforms);
				BlamLib.BlamVersion version;
				sed.ShowDialogWithResult(this, out version);

				if (version != BlamLib.BlamVersion.Unknown)
					AddMdiChild(new Cache.CacheView(version), false);
			});
			OpenCache.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			OpenCache.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(OpenCache);
			command_dic.Add("OpenCache", OpenCache_handler);
		}

		public MainForm(string[] args)
		{
			InitializeComponent();

			MainMenu.Renderer = new OpenSauceIDEToolStripRenderer();

			//BlamLib.Program.Initialize();

			var dic = new Dictionary<string, EventHandler>();
			BuildCheApeMenus(dic);
			BuildCacheMenus(dic);

			const string k_startup_prefix = "-startup:";
			foreach(string s in args)
			{
				if (s.StartsWith(k_startup_prefix, true, null))
				{
					string command = s.Substring(k_startup_prefix.Length - 1);

					foreach (KeyValuePair<string, EventHandler> kv in dic)
						if (command.Equals(kv.Key, StringComparison.OrdinalIgnoreCase))
							kv.Value.Invoke(null, new EventArgs());
				}
			}
		}

		void OnMdiFormClosed(object sender, FormClosedEventArgs e)
		{
			Form f = sender as Form;
			foreach (ToolStripMenuItem i in f.MdiParent.MainMenuStrip.Items)
				i.Visible = true;
		}

		void AddMdiChild(Form form)
		{
			AddMdiChild(form, true);
		}
		void AddMdiChild(Form form, bool merge_menus)
		{
			if(merge_menus)
				foreach (ToolStripMenuItem i in MainMenuStrip.Items)
					i.Visible = false;

			form.MdiParent = this;
			form.Show();
			form.FormClosed += new FormClosedEventHandler(OnMdiFormClosed);
		}

		void OnFormLoad(object sender, EventArgs e)
		{
		}

		void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			if (ActiveMdiChild != null)
				ActiveMdiChild.Close();
			//BlamLib.Program.Close();
		}

		#region Drag - Drop
		void OnDragDrop(object sender, DragEventArgs e)
		{
		}

		void OnDragEnter(object sender, DragEventArgs e)
		{
		}

		void OnDragLeave(object sender, EventArgs e)
		{
		}

		void OnDragOver(object sender, DragEventArgs e)
		{
		}

		void OnDragGiveFeedback(object sender, GiveFeedbackEventArgs e)
		{
		}

		void OnDragQueryContinueDrag(object sender, QueryContinueDragEventArgs e)
		{
		}
		#endregion

		void OnFileExit(object sender, EventArgs e)	{ this.Close(); }

		#region Custom Drawing
		class OpenSauceIDEToolStripRenderer : ToolStripProfessionalRenderer
		{
			protected override void OnRenderSeparator(ToolStripSeparatorRenderEventArgs e)
			{
				if (!(e.Item is ToolStripSeparator))
					base.OnRenderSeparator(e);
				else
				{
					Rectangle seperator_bounds = new Rectangle(Point.Empty, e.Item.Size);

					using (var seperator_brush = new SolidBrush(e.Item.BackColor))
					{ e.Graphics.FillRectangle(seperator_brush, 0, 0, seperator_bounds.Width, seperator_bounds.Height); }

					using (var seperator_brush = new SolidBrush(e.Item.ForeColor))
					{
						if (e.Vertical)
							e.Graphics.FillRectangle(seperator_brush,
							seperator_bounds.Width / 2,
							2,
							1,
							seperator_bounds.Height - 4);
						else
							e.Graphics.FillRectangle(seperator_brush,
							23,
							seperator_bounds.Height / 2,
							seperator_bounds.Width - 23,
							1);
					}
				}
			}
		};
		#endregion
	};
}