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
using Color = System.Drawing.Color;
using SystemColors = System.Drawing.SystemColors;
using BlamLib;

namespace OpenSauceIDE.Cache
{
	public partial class CacheView : Form
	{
		enum TagTreeEditorMode
		{
			TagExtraction,
			ResourceExtraction,
		};

		/// <summary>The valid engines which CheApe can be used with</summary>
		public const BlamLib.BlamVersion kAllowedPlatforms = 
			BlamLib.BlamVersion.Halo1 | BlamLib.BlamVersion.Stubbs |
			BlamLib.BlamVersion.Halo2 |
			BlamLib.BlamVersion.Halo3 | BlamLib.BlamVersion.HaloOdst | BlamLib.BlamVersion.HaloReach;
		
		const string kDlgName = "Cache Viewer";
		void SetCacheViewerPath(string cache_path)
		{
			this.Text = string.Format("{0} - [{1}]", kDlgName, cache_path);
		}

		TagTreeEditorMode m_tagTreeEditorMode;
		BlamLib.BlamVersion m_engine;
		BlamLib.Blam.CacheFile m_cache;
		BlamLib.TagInterface.TagGroupCollection m_groupsInvalidForExtraction;

		public CacheView(BlamLib.BlamVersion engine)
		{
			InitializeComponent();

			m_tagTreeEditorMode = TagTreeEditorMode.TagExtraction;
			m_engine = engine;
			m_cache = null;

			// We currently only allow check boxes for marking tags for extraction. If we ever change 
			// this behavior (eg, to mark for resource extract too), we'll need to update this logic
			var b = engine.ToBuild();
			if (b != BlamBuild.Halo1 && b != BlamBuild.Halo2 && b != BlamBuild.Stubbs)
				TagTreeView.CheckBoxes = false;
		}

		#region Information population
		void PopulateTagInstanceProperties(object tag_group_or_instance)
		{
			PropGrigTag.SelectedObject = tag_group_or_instance;
		}
		void PopulateCacheProperties()
		{
			PropGridCache.SelectedObject = m_cache;
		}

		void PopulateTagTreeView_GenerateNodes(object state) // threaded
		{
			List<TreeNode> tg_nodes;
			Dictionary<BlamLib.TagInterface.TagGroup, TreeNode> tg_dic;
			BlamLib.TagInterface.TagGroupCollection tg_coll, tg_invalid_for_cv;
			{
				var game_man = BlamLib.Program.GetManager(m_cache.EngineVersion);
				tg_dic = new Dictionary<BlamLib.TagInterface.TagGroup, TreeNode>(game_man.TagGroups.Count);
				tg_coll = game_man.TagGroups;
				tg_invalid_for_cv = game_man.TagGroupsInvalidForCacheViewer;
				m_groupsInvalidForExtraction = game_man.TagGroupsInvalidForExtraction;

				tg_nodes = new List<TreeNode>(game_man.TagGroups.Count);
			};

			#region Build tag group parent nodes
			foreach (var tg in tg_coll)
			{
				if (tg == BlamLib.TagInterface.TagGroup.Null || tg_invalid_for_cv.Contains(tg)) continue;

				var node = new TreeNode(tg.Name);
				node.Tag = tg;
				node.Checked = false;
				node.ForeColor = Color.LightGreen;
				node.BackColor = SystemColors.ControlDarkDark;
				tg_dic.Add(tg, node);
				tg_nodes.Add(node);
			}
			#endregion

			#region Build tag instances
			foreach (var inst in m_cache.Index.Tags)
			{
				if (inst.IsEmpty) continue;

				TreeNode tg_node;
				if (!tg_dic.TryGetValue(inst.GroupTag, out tg_node)) continue;

				var node = new TreeNode(m_cache.GetReferenceName(inst.ReferenceName));
				node.Tag = inst;
				node.Checked = false;
				//node.ContextMenuStrip = null; // TODO
				node.ForeColor = Color.LightGreen;
				node.BackColor = SystemColors.ControlDarkDark;
				tg_node.Nodes.Add(node);
			}
			#endregion

			#region Clean up the parent nodes
			tg_nodes.TrimExcess();
			for (int x = 0; x < tg_nodes.Count; x++)
				if (tg_nodes[x].Nodes.Count == 0)
					tg_nodes.RemoveAt(x--);
			#endregion

			object args = tg_nodes.ToArray();
			(state as CacheView).BeginInvoke(new Action<TreeNode[]>(PopulateTagTreeView_FillNodes), args);
		}
		void PopulateTagTreeView_FillNodes(TreeNode[] nodes)
		{
			if (m_cache == null) return;

			TagTreeView.BeginUpdate();
			TagTreeView.Nodes.AddRange(nodes);
			TagTreeView.EndUpdate();
		}
		void PopulateTagTreeView()
		{
			if (m_cache != null)
				System.Threading.ThreadPool.QueueUserWorkItem(PopulateTagTreeView_GenerateNodes, this);
			else
			{
				m_groupsInvalidForExtraction = null;
				TagTreeView.BeginUpdate();
				TagTreeView.Nodes.Clear();
				TagTreeView.EndUpdate();
			}
		}
		#endregion


		void OnCacheLoaded(string cache_path)
		{
			FileClose.Enabled = true;

			SetCacheViewerPath(cache_path);
			PopulateCacheProperties();
			PopulateTagTreeView();
		}
		void OnCacheClosed()
		{
			PopulateTagInstanceProperties(null);
			m_cache = null;

			FileClose.Enabled = false;
			SetCacheViewerPath("No File Loaded");
			PopulateCacheProperties();
			PopulateTagTreeView();
		}

		#region OnFileOpen
		void ShowOpenErrorMessage(string fmt, params object[] args)
		{
			string caption = string.Format("Error opening [{0}] cache", m_engine.ToString());

			MessageBox.Show(this, string.Format(fmt, args), caption, MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		void OnFileFinishedOpening(Exception ex)
		{
			if (ex != null)
			{
				OnCacheClosed();

				if (ex is BlamLib.Blam.InvalidCacheFileException)
					ShowOpenErrorMessage("Selected cache file has an invalid format. Refer to debug logs for more information");
				else
					ShowOpenErrorMessage("Exception:{0}{1}", BlamLib.Program.NewLine, ex);
			}

			if (m_cache != null)
				OnCacheLoaded(OpenFileDlg.FileName);
		}
		void OnFileOpen_DoWork(object obj) // threaded
		{
			Exception except = null;
			var gd = BlamLib.Program.GetManager(m_engine);
			var cid = BlamLib.Blam.DatumIndex.Null;
			try
			{
				cid = gd.OpenCacheFile(m_engine, OpenFileDlg.FileName);
				m_cache = gd.GetCacheFile(cid);
				m_cache.Read();
			}
			catch (Exception ex)
			{
				except = ex;

				if (!cid.IsNull && m_cache != null)
					gd.CloseCacheFile(m_cache.CacheId);
				m_cache = null;
			}
			(obj as CacheView).BeginInvoke(new Action<Exception>(OnFileFinishedOpening), except);
		}
		void OnFileOpen(object sender, EventArgs e)
		{
			if (m_cache == null && OpenFileDlg.ShowDialog(this) == DialogResult.OK)
			{
				SetCacheViewerPath("Loading...");
				System.Threading.ThreadPool.QueueUserWorkItem(OnFileOpen_DoWork, this);
			}
		}

		void OnFileClose(object sender, EventArgs e)
		{
			if (m_cache != null)
			{
				var gd = BlamLib.Program.GetManager(m_engine);
				gd.CloseCacheFile(m_cache.CacheId);

				OnCacheClosed();
			}
		}
		#endregion

		void OnFormShown(object sender, EventArgs e)
		{
			OnFileOpen(this, null);
		}
		void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			OnFileClose(this, null);
		}

		void OnTagTreeBeforeCheck(object sender, TreeViewCancelEventArgs e)
		{
			var tag_instance = e.Node.Tag as BlamLib.Blam.CacheIndex.Item;

			if (tag_instance != null)
			{
				if (m_groupsInvalidForExtraction.Contains(tag_instance.GroupTag))
					e.Cancel = true;
			}
		}

		void OnTagTreeAfterSelect(object sender, TreeViewEventArgs e)
		{
			var tag = e.Node.Tag;

			if (tag is BlamLib.Blam.CacheIndex.Item || tag is BlamLib.TagInterface.TagGroup)
				PopulateTagInstanceProperties(tag);
		}
	};
}