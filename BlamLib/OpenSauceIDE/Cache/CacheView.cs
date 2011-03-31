/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C)  Kornner Studios (http://kornner.com)

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

		const string kStatusBarText = "trololo";
		void SetStatusBarText(string text)
		{
			if (text == null) text = kStatusBarText;

			StatusProgressText.Text = text;
		}

		TagTreeEditorMode m_tagTreeEditorMode;
		BlamLib.BlamVersion m_engine;
		BlamLib.Blam.CacheFile m_cache;
		BlamLib.TagInterface.TagGroupCollection m_groupsInvalidForExtraction;
		BackgroundWorker m_currentTask;
		// Event signal for making sure we don't do anything crazy, like exit, while the cache is performing 
		// a thread pool task. Eg, opening, extracting, etc.
		System.Threading.AutoResetEvent m_waitEvent, m_extractAllWaitEvent;

		public CacheView(BlamLib.BlamVersion engine)
		{
			InitializeComponent();

			MainMenu.Renderer = MainForm.kOpenSauceIDEToolStripRenderer;
			MenuTagInstance.Renderer = MainForm.kOpenSauceIDEToolStripRenderer;

			m_tagTreeEditorMode = TagTreeEditorMode.TagExtraction;
			m_engine = engine;
			m_cache = null;
			m_waitEvent = new System.Threading.AutoResetEvent(true);
			m_extractAllWaitEvent = new System.Threading.AutoResetEvent(true);

			// We currently only allow check boxes for marking tags for extraction. If we ever change 
			// this behavior (eg, to mark for resource extract too), we'll need to update this logic
			var b = engine.ToBuild();
			bool tag_extraction_supported = b == BlamBuild.Halo1 || b == BlamBuild.Halo2 || b == BlamBuild.Stubbs;

			ViewUpdateExtractionSupportedState(tag_extraction_supported, false);
		}

		/// <summary>Update the UI state with the available extraction abilities that we support for the current engine</summary>
		/// <param name="tags_supported">Is tag extraction supported?</param>
		/// <param name="rsrc_supported">Is resource extraction supported?</param>
		void ViewUpdateExtractionSupportedState(bool tags_supported, bool rsrc_supported)
		{
			TagTreeView.CheckBoxes = tags_supported || rsrc_supported;
			CacheToolsDontOverwrite.Enabled = tags_supported || rsrc_supported;

			CacheToolsExtractAll.Enabled = tags_supported;
			CacheToolsExtractAllChecked.Enabled = tags_supported;
			CacheToolsExtractAllUnchecked.Enabled = tags_supported;
			CacheToolsOutputTagDatabase.Enabled = tags_supported;
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
				node.ContextMenuStrip = MenuTagInstance;
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
			var cv = state as CacheView;
			cv.m_waitEvent.Set();
			cv.BeginInvoke(new Action<TreeNode[]>(PopulateTagTreeView_FillNodes), args);
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
			{
				m_waitEvent.Reset();
				System.Threading.ThreadPool.QueueUserWorkItem(PopulateTagTreeView_GenerateNodes, this);
			}
			else
			{
				m_groupsInvalidForExtraction = null;
				TagTreeView.BeginUpdate();
				TagTreeView.Nodes.Clear();
				TagTreeView.EndUpdate();
			}
		}
		#endregion


		/// <summary>Ran when a cache has finished loading and now the UI needs to be updated</summary>
		/// <param name="cache_path">Full file path of the cache file we're viewing</param>
		void OnCacheLoaded(string cache_path)
		{
			FileClose.Enabled = true;
			CacheToolsExtractAll.Enabled = true;

			SetCacheViewerPath(cache_path);
			PopulateCacheProperties();
			PopulateTagTreeView();
		}
		/// <summary>Ran whenever the UI says it's time to close the cache</summary>
		void OnCacheClosed()
		{
			m_extractAllWaitEvent.WaitOne();
			m_waitEvent.WaitOne();

			PopulateTagInstanceProperties(null);
			m_cache = null;

			FileClose.Enabled = false;
			CacheToolsExtractAll.Enabled = false;
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
		/// <summary>Handles the results of <see cref="OnFileOpen_DoWork"/></summary>
		/// <param name="ex"></param>
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
		/// <summary>Performs the actual cache loading logic</summary>
		/// <param name="obj"></param>
		/// <remarks>Designed for being ran in a worker task (ie, threaded)</remarks>
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

			var cv = obj as CacheView;
			cv.m_waitEvent.Set();
			cv.BeginInvoke(new Action<Exception>(OnFileFinishedOpening), except);
		}
		void OnFileOpen(object sender, EventArgs e)
		{
			if (m_cache == null && OpenFileDlg.ShowDialog(this) == DialogResult.OK)
			{
				m_waitEvent.Reset();
				m_extractAllWaitEvent.Set();
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

		/// <summary>Enables us to force the file-open dialog for browsing to a cache file we want to view</summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		void OnFormShown(object sender, EventArgs e)				{ OnFileOpen(this, null); }
		void OnFormClosing(object sender, FormClosingEventArgs e)	{ OnFileClose(this, null); }

		/// <summary>For handling cases where we want to block a user from checking a tag instance</summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		void OnTagTreeBeforeCheck(object sender, TreeViewCancelEventArgs e)
		{
			var tag_instance = e.Node.Tag as BlamLib.Blam.CacheIndex.Item;

			if (tag_instance != null)
			{
				if (m_groupsInvalidForExtraction.Contains(tag_instance.GroupTag))
					e.Cancel = true;
			}
		}
		/// <summary>For handling the tag instance PropertyGrid view updating</summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		void OnTagTreeAfterSelect(object sender, TreeViewEventArgs e)
		{
			var tag = e.Node.Tag;

			if (tag is BlamLib.Blam.CacheIndex.Item || tag is BlamLib.TagInterface.TagGroup)
				PopulateTagInstanceProperties(tag);
		}

		#region MenuTagInstance event handlers
		void EnableTagExtractionContextMenus(bool enable)
		{
			MenuTagInstanceExtractAs.Enabled = enable;
			MenuTagInstanceExtractFolder.Enabled = enable;
			MenuTagInstanceExtractFolderAll.Enabled = enable;
		}
		void EnableTagRsrcExtractionContextMenus(bool enable)
		{
			MenuTagInstanceExtractRsrc.Enabled = enable;
		}
		void OnMenuTagInstanceOpening(object sender, CancelEventArgs e)
		{
			if (m_tagTreeEditorMode == TagTreeEditorMode.TagExtraction)
			{
				EnableTagExtractionContextMenus(true);
				EnableTagRsrcExtractionContextMenus(false);
			}
			else if (m_tagTreeEditorMode == TagTreeEditorMode.ResourceExtraction)
			{
				EnableTagExtractionContextMenus(false);
				EnableTagRsrcExtractionContextMenus(true);
			}
		}
		#endregion


		void bgw_DoWorkBase(object sender, DoWorkEventArgs e)
		{
			m_currentTask = sender as BackgroundWorker;
			this.BeginInvoke( new MethodInvoker( () => StatusProgressCancel.Enabled = true ));

			var info = e.Argument as TagInstanceExtractionInfo;
			info.InitializeBackgroundWorker(sender, e);
		}
		void bgw_ProgressChangedBase(object sender, ProgressChangedEventArgs e)
		{
			StatusProgressBar.Value = e.ProgressPercentage;
		}
		void bgw_RunWorkerCompletedBase(object sender, RunWorkerCompletedEventArgs e)
		{
			m_currentTask = null;
			StatusProgressCancel.Enabled = false;

			StatusProgressBar.Value = 0;

			var info = e.Result as TagInstanceExtractionInfo;
			if (!info.ExtractingSingleTag)
			{
				TagTreeView.Enabled = true;
				TagTreeView.UseWaitCursor = false;
			}
		}

		void bgwProcessTagTreeView_DoWork(object sender, DoWorkEventArgs e)
		{
			bgw_DoWorkBase(sender, e);
		}
		void bgwProcessTagTreeView_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			bgw_ProgressChangedBase(sender, e);
		}
		void bgwProcessTagTreeView_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			bgw_RunWorkerCompletedBase(sender, e);
		}

		void bgwTagExtract_DoWork(object sender, DoWorkEventArgs e)
		{
			bgw_DoWorkBase(sender, e);
		}
		void bgwTagExtract_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			bgw_ProgressChangedBase(sender, e);
		}
		void bgwTagExtract_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			bgw_RunWorkerCompletedBase(sender, e);
		}

		void OnCancelTask(object sender, EventArgs e)
		{
			if (m_currentTask != null)
				m_currentTask.CancelAsync();
		}
	};
}