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
		void BuildCheApeMenusHalo1(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler Halo1CheApe_handler;
			ToolStripMenuItem Halo1CheApe = BlamLib.Forms.Util.CreateMenuItem("CheApe", Halo1CheApe_handler = delegate(object sender, EventArgs e)
			{
				AddMdiChild(new CheApe(BlamLib.BlamVersion.Halo1_CE));
			});
			Halo1CheApe.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			Halo1CheApe.ForeColor = System.Drawing.Color.LightGreen;

			ToolsHalo1.DropDownItems.Add(Halo1CheApe);
			command_dic.Add("Halo1:CheApe", Halo1CheApe_handler);
		}
		void BuildCheApeMenusHalo2(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler Halo2CheApe_handler;
			ToolStripMenuItem Halo2CheApe = BlamLib.Forms.Util.CreateMenuItem("CheApe", Halo2CheApe_handler = delegate(object sender, EventArgs e)
			{
				AddMdiChild(new CheApe(BlamLib.BlamVersion.Halo2_PC));
			});
			Halo2CheApe.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			Halo2CheApe.ForeColor = System.Drawing.Color.LightGreen;


			//ToolsHalo2.DropDownItems.Add(Halo2CheApe);
			//command_dic.Add("Halo2:CheApe", Halo2CheApe_handler);
		}
		void BuildCheApeMenus(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler CheApeApply_handler;
			ToolStripMenuItem CheApeApply = BlamLib.Forms.Util.CreateMenuItem("CheApe Applier", CheApeApply_handler = delegate(object sender, EventArgs e)
			{
				AddMdiChild(new CheApeApplier());
			});
			CheApeApply.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			CheApeApply.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Insert(0, CheApeApply);
			command_dic.Add("CheApeApplier", CheApeApply_handler);

			BuildCheApeMenusHalo1(command_dic);
			BuildCheApeMenusHalo2(command_dic);
		}
		public MainForm(string[] args)
		{
			InitializeComponent();

			MainMenu.Renderer = new OpenSauceIDEToolStripRenderer();

			//BlamLib.Program.Initialize();

			var dic = new Dictionary<string, EventHandler>();
			BuildCheApeMenus(dic);

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

		void OnFileExit(object sender, EventArgs e)
		{
			this.Close();
		}

		#region Custom Drawing
		private class OpenSauceIDEToolStripRenderer : ToolStripProfessionalRenderer
		{
			protected override void OnRenderSeparator(ToolStripSeparatorRenderEventArgs e)
			{
				if ((e.Item as ToolStripSeparator) == null)
					base.OnRenderSeparator(e);
				else
				{
					Rectangle seperator_bounds = new Rectangle(Point.Empty, e.Item.Size);

					Brush seperator_brush = new SolidBrush(e.Item.BackColor);
					e.Graphics.FillRectangle(seperator_brush, 0, 0, seperator_bounds.Width, seperator_bounds.Height);
					seperator_brush.Dispose();

					seperator_brush = new SolidBrush(e.Item.ForeColor);
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
					seperator_brush.Dispose();
				}
			}
		};
		#endregion
	};
}