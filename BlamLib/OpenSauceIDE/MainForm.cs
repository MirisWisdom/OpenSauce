/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	public partial class MainForm : Form
	{
		#region Build Menus
		static void BuildMenuSeparator(ToolStripMenuItem owner)
		{
			var tss = new ToolStripSeparator();
			tss.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			tss.ForeColor = System.Drawing.Color.LightGreen;

			owner.DropDownItems.Add(tss);
		}

		/// <summary>We currently don't have any real engine-specific tools, so remove them from the tools menu</summary>
		void RemoveUnusedMenus()
		{
			var ddi = ToolsMenu.DropDownItems;

			ddi.Remove(ToolsHalo1);
			ddi.Remove(ToolsHalo2);
			ddi.Remove(ToolsHalo3);
			ddi.Remove(ToolsStubbs);
		}

		#region BuildSettingsMenus
		/// <summary>Build the menu which when clicked, opens the folder where BlamLib shits out its local files (eg, debug.log)</summary>
		void BuildShowAppFolderMenu()
		{
			var Settings = BlamLib.Forms.Util.CreateMenuItem("Open App Folder", (sender, e) =>
			{
				var process_si = new System.Diagnostics.ProcessStartInfo("explorer", BlamLib.Program.BuildDocumentPath(""));
				var process = new System.Diagnostics.Process();
				process.StartInfo = process_si;
				process.Start();
			});
			Settings.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			Settings.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Insert(0, Settings);
		}
		void BuildSettingsMenus()
		{
			BuildShowAppFolderMenu();

			var Settings = BlamLib.Forms.Util.CreateMenuItem("Settings", (sender, e) =>
			{
				var sed = new SelectEngineDialog(EngineSettingsForm.kAllowedPlatforms);
				BlamLib.BlamVersion version;
				sed.ShowDialogWithResult(this, out version);

				if (version != BlamLib.BlamVersion.Unknown)
					new EngineSettingsForm(version).ShowDialog(this);
			});
			Settings.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			Settings.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(Settings);
		}
		#endregion

		#region CheApe menus
		void BuildCheApeMenusCheApeApply(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler CheApeApply_handler;
			var CheApeApply = BlamLib.Forms.Util.CreateMenuItem("CheApe Applier", CheApeApply_handler = (sender, e) =>
			{
                var sed = new SelectEngineDialog(CheApe.kTargetPlatforms, BlamLib.BlamVersion.Halo2_PC | BlamLib.BlamVersion.Halo1_PC);
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
			var CheApe = BlamLib.Forms.Util.CreateMenuItem("CheApe", CheApe_handler = (sender, e) =>
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
		void BuildCheApeMenusXnaPatcher(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler XnaPatcher_handler;
			var XnaPatcher = BlamLib.Forms.Util.CreateMenuItem("Xna Patcher", XnaPatcher_handler = (sender, e) =>
			{
				new OpenSauce.XnaPatcherForm().ShowDialog(this);
			});
			XnaPatcher.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			XnaPatcher.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(XnaPatcher);
			command_dic.Add("XnaPatcher", XnaPatcher_handler);
		}
		void BuildCheApeMenusServerTool(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler ServerTool_handler;
			var ServerTool = BlamLib.Forms.Util.CreateMenuItem("Server Map Download Tool", ServerTool_handler = (sender, e) =>
			{
				AddMdiChild(new ServerMapDownloadTool.MainForm());
			});
			ServerTool.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			ServerTool.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(ServerTool);
			command_dic.Add("ServerTool", ServerTool_handler);
		}
		void BuildCheApeMenusModelExtractor(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler ModelExtractor_handler;
			var ModelExtractor = BlamLib.Forms.Util.CreateMenuItem("Model Extractor", ModelExtractor_handler = (sender, e) =>
			{
				var sed = new SelectEngineDialog(OpenSauceIDE.CheApe.kTargetPlatforms, BlamLib.BlamVersion.Halo1_PC, BlamLib.BlamVersion.Halo2_PC);
				BlamLib.BlamVersion version;
				sed.ShowDialogWithResult(this, out version);

				if (version != BlamLib.BlamVersion.Unknown)
				{
					AddMdiChild(new ModelExtractor.ModelExtractor(version));
				}
			});
			ModelExtractor.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			ModelExtractor.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(ModelExtractor);
			command_dic.Add("ModelExtractor", ModelExtractor_handler);
		}
		void BuildCheApeMenusLightmapImporter(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler LightmapImporter_handler;
			var LightmapImporter = BlamLib.Forms.Util.CreateMenuItem("Lightmap Importer", LightmapImporter_handler = (sender, e) =>
			{
				var sed = new SelectEngineDialog(OpenSauceIDE.CheApe.kTargetPlatforms, BlamLib.BlamVersion.Halo1_PC, BlamLib.BlamVersion.Halo2_PC);
				BlamLib.BlamVersion version;
				sed.ShowDialogWithResult(this, out version);

				if (version != BlamLib.BlamVersion.Unknown)
				{
					AddMdiChild(new LightmapImporter.LightmapImporter(version));
				}
			});
			LightmapImporter.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			LightmapImporter.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(LightmapImporter);
			command_dic.Add("LightmapImporter", LightmapImporter_handler);
		}
		void BuildCheApeMenus(Dictionary<string, EventHandler> command_dic)
		{
			BuildMenuSeparator(ToolsMenu);
			BuildCheApeMenusCheApeApply(command_dic);
			BuildCheApeMenusCheApe(command_dic);
			BuildCheApeMenusXnaPatcher(command_dic);
			BuildCheApeMenusServerTool(command_dic);
			BuildCheApeMenusModelExtractor(command_dic);
			BuildCheApeMenusLightmapImporter(command_dic);
		}
		#endregion

		#region Cache menus
		void BuildCacheMenusOpenCache(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler OpenCache_handler;
			var OpenCache = BlamLib.Forms.Util.CreateMenuItem("Open Cache", OpenCache_handler = (sender, e) =>
			{
				var sed = new SelectEngineDialog(Cache.CacheView.kAllowedPlatforms);
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
		void BuildCacheMenusCachePacker(Dictionary<string, EventHandler> command_dic)
		{
			EventHandler CachePacker_handler;
			var CachePacker = BlamLib.Forms.Util.CreateMenuItem("Cache Packer", CachePacker_handler = (sender, e) =>
			{
				new Cache.CachePacker().ShowDialog(this);
			});
			CachePacker.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			CachePacker.ForeColor = System.Drawing.Color.LightGreen;

			ToolsMenu.DropDownItems.Add(CachePacker);
			command_dic.Add("CachePacker", CachePacker_handler);
		}
		void BuildCacheMenus(Dictionary<string, EventHandler> command_dic)
		{
			BuildMenuSeparator(ToolsMenu);
			BuildCacheMenusOpenCache(command_dic);
			BuildCacheMenusCachePacker(command_dic);
		}
		#endregion
		#endregion

		public MainForm(string[] args)
		{
			InitializeComponent();

			MainMenu.Renderer = kOpenSauceIDEToolStripRenderer;

			//BlamLib.Program.Initialize();

			RemoveUnusedMenus();
			var dic = new Dictionary<string, EventHandler>();
			BuildSettingsMenus();
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
		internal static readonly ToolStripProfessionalRenderer kOpenSauceIDEToolStripRenderer = new OpenSauceIDEToolStripRenderer();
		#endregion
	};
}