/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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

		/// <summary>The valid engines which CacheView can be used with</summary>
		public const BlamVersion kAllowedPlatforms = 
			BlamVersion.Halo1 | BlamVersion.Stubbs |
			BlamVersion.Halo2 |
			BlamVersion.Halo3 | BlamVersion.HaloOdst | BlamVersion.HaloReach;
		const BlamVersion kAllowPlatformsForTagExtraction =
			BlamVersion.Halo1 | BlamVersion.Stubbs |
			BlamVersion.Halo2;

		#region SetCacheViewerPath
		const string kDlgName = "Cache Viewer";
		void SetCacheViewerPath(string cache_path)
		{
			this.Text = string.Format("{0} - [{1}]", kDlgName, cache_path);
		}
		#endregion

		#region SetStatusBarText
		const string kStatusBarText = "trololo";
		Action<CacheView,string> kSetStatusBarTextAction = new Action<CacheView,string>( (cv,text) => cv.StatusProgressText.Text = text );
		void SetStatusBarText(string text)
		{
			if (text == null) text = kStatusBarText;

			this.BeginInvoke(kSetStatusBarTextAction, this, text);
		}
		#endregion

		TagTreeEditorMode m_tagTreeEditorMode;
		BlamLib.BlamVersion m_engine;
		BlamLib.Blam.CacheFile m_cache;
		BlamLib.TagInterface.TagGroupCollection m_groupsInvalidForExtraction;
		BackgroundWorker m_currentTask;

		public CacheView(BlamLib.BlamVersion engine)
		{
			InitializeComponent();

			MainMenu.Renderer = MainForm.kOpenSauceIDEToolStripRenderer;
			MenuTagInstance.Renderer = MainForm.kOpenSauceIDEToolStripRenderer;

			m_tagTreeEditorMode = TagTreeEditorMode.TagExtraction;
			m_engine = engine;
			m_cache = null;

			bool tag_extraction_supported = (engine & kAllowedPlatforms) != 0;
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

		void TagTreeViewFillNodes(TreeNode[] nodes)
		{
			if (m_cache == null) return;

			TagTreeView.BeginUpdate();
			TagTreeView.Nodes.AddRange(nodes);
			TagTreeView.EndUpdate();
		}
		void TagTreeViewClear()
		{
			m_groupsInvalidForExtraction = null;
			TagTreeView.BeginUpdate();
			TagTreeView.Nodes.Clear();
			TagTreeView.EndUpdate();
		}
		#endregion

		/// <summary>Update the UI state based on the state of the current task (or lack thereof)</summary>
		void ToggleUserInterfaceForOperatingTask()
		{
			bool performing_task = m_currentTask != null;

			FileOpen.Enabled = !performing_task;
			FileClose.Enabled = !performing_task;
			CacheToolsExtractAll.Enabled = !performing_task;
		}
		/// <summary>Update the UI state based on the state of the cache we're viewing</summary>
		/// <param name="cache_is_opened"></param>
		void ToggleUserInterfaceForCacheState(bool cache_is_opened)
		{
			FileClose.Enabled = cache_is_opened;
			CacheToolsExtractAll.Enabled = cache_is_opened;
		}


		/// <summary>Ran when a cache has finished loading and now the UI needs to be updated</summary>
		/// <param name="cache_path">Full file path of the cache file we're viewing</param>
		void OnCacheLoaded(string cache_path)
		{
			ToggleUserInterfaceForCacheState(true);

			SetCacheViewerPath(cache_path);
			PopulateCacheProperties();
		}
		/// <summary>Ran whenever the UI says it's time to close the cache</summary>
		void OnCacheClosed()
		{
			PopulateTagInstanceProperties(null);
			m_cache = null;

			ToggleUserInterfaceForCacheState(false);
			SetCacheViewerPath("No File Loaded");
			PopulateCacheProperties();
			TagTreeViewClear();
		}

		#region OnFileOpen
		void ShowOpenErrorMessage(string fmt, params object[] args)
		{
			string caption = string.Format("Error opening [{0}] cache", m_engine.ToString());

			MessageBox.Show(this, string.Format(fmt, args), caption, MessageBoxButtons.OK, MessageBoxIcon.Error);
		}
		/// <summary>Handles the exception results of <see cref="CacheOpenDoWorkLoad"/></summary>
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
		void OnFileOpen(object sender, EventArgs e)
		{
			if (OpenFileDlg.ShowDialog(this) == DialogResult.OK)
			{
				OnFileClose(this, null);

				SetCacheViewerPath("Loading...");
				bgwCacheOpen.RunWorkerAsync(this);
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

		#region OnTagTree event handlers
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
		#endregion

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


		#region BackgroundWorker event handlers
		#region Cache Opening
		class CacheOpenDoWorkResults
		{
			public Exception OpenException;
			public TreeNode[] TagTreeNodes;
		};

		static TreeNode[] CacheOpenDoWorkGenerateTagTreeNodes(CacheView cv, BlamLib.Blam.CacheFile cache)
		{
			List<TreeNode> tg_nodes;
			Dictionary<BlamLib.TagInterface.TagGroup, TreeNode> tg_dic;
			BlamLib.TagInterface.TagGroupCollection tg_coll, tg_invalid_for_cv;
			{
				var game_man = BlamLib.Program.GetManager(cache.EngineVersion);
				tg_dic = new Dictionary<BlamLib.TagInterface.TagGroup, TreeNode>(game_man.TagGroups.Count);
				tg_coll = game_man.TagGroups;
				tg_invalid_for_cv = game_man.TagGroupsInvalidForCacheViewer;
				cv.m_groupsInvalidForExtraction = game_man.TagGroupsInvalidForExtraction;

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
			foreach (var inst in cache.Index.Tags)
			{
				if (inst.IsEmpty) continue;

				TreeNode tg_node;
				if (!tg_dic.TryGetValue(inst.GroupTag, out tg_node)) continue;

				var node = new TreeNode(cache.GetReferenceName(inst.ReferenceName));
				node.Tag = inst;
				node.Checked = false;
				node.ContextMenuStrip = cv.MenuTagInstance;
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

			return tg_nodes.ToArray();
		}
		/// <summary>Performs the actual cache loading logic</summary>
		/// <param name="cv"></param>
		static CacheOpenDoWorkResults CacheOpenDoWorkLoad(CacheView cv)
		{
			var results = new CacheOpenDoWorkResults();
			var engine = cv.m_engine;
			BlamLib.Blam.CacheFile cache = null;

			var gd = BlamLib.Program.GetManager(engine);
			var cid = BlamLib.Blam.DatumIndex.Null;
			try
			{
				cid = gd.OpenCacheFile(engine, cv.OpenFileDlg.FileName);
				cache = gd.GetCacheFile(cid);
				cache.Read();
			}
			catch (Exception ex)
			{
				results.OpenException = ex;

				if (!cid.IsNull && cache != null)
					gd.CloseCacheFile(cache.CacheId);
				cache = null;
			}

			cv.m_cache = cache;

			if (cache != null)
				results.TagTreeNodes = CacheOpenDoWorkGenerateTagTreeNodes(cv, cache);

			return results;
		}

		void bgwCacheOpenDoWork(object sender, DoWorkEventArgs e)
		{
			m_currentTask = sender as BackgroundWorker;
			// Initialize the UI state first, before performing work
			this.BeginInvoke(new Action(ToggleUserInterfaceForOperatingTask));

			e.Result = CacheOpenDoWorkLoad(e.Argument as CacheView);
		}
		void bgwCacheOpenProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			StatusProgressBar.Value = e.ProgressPercentage;
		}
		void bgwCacheOpenRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			m_currentTask = null;
			// Reset and release the UI back to the user
			ToggleUserInterfaceForOperatingTask();
			StatusProgressBar.Value = 0;

			var results = e.Result as CacheOpenDoWorkResults;
			OnFileFinishedOpening(results.OpenException);
			if (results.TagTreeNodes != null)
				TagTreeViewFillNodes(results.TagTreeNodes);
		}
		#endregion

		#region Tag Extraction
		void bgwTagExtractDoWork(object sender, DoWorkEventArgs e)
		{
			m_currentTask = sender as BackgroundWorker;
			// Initialize the UI state first, before performing work
			this.BeginInvoke( new Action( () => { 
				StatusProgressCancel.Enabled = true;
				ToggleUserInterfaceForOperatingTask();
			}));

			var info = e.Argument as TagInstanceExtractionInfo;
			info.InitializeBackgroundWorker(sender, e);
			TagInstanceExtraction(info);
		}
		void bgwTagExtractProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			StatusProgressBar.Value = e.ProgressPercentage;
		}
		void bgwTagExtractRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			m_currentTask = null;
			// Reset and release the UI back to the user
			ToggleUserInterfaceForOperatingTask();
			StatusProgressCancel.Enabled = false;
			StatusProgressBar.Value = 0;

			var info = e.Result as TagInstanceExtractionInfo;
			if (!info.ExtractingSingleTag)
			{
				TagTreeView.Enabled = true;
				TagTreeView.UseWaitCursor = false;
			}
		}
		#endregion

		void OnCancelTask(object sender, EventArgs e)
		{
			if (m_currentTask != null && m_currentTask.WorkerSupportsCancellation)
				m_currentTask.CancelAsync();
		}
		#endregion
	};
}