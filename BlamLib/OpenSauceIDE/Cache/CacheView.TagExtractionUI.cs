/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Windows.Forms;

namespace OpenSauceIDE.Cache
{
	partial class CacheView
	{
		void OnTagInstanceExtractUpdateStatus(TagInstanceExtractionInfo info)
		{
			if (info != null)
			{
				SetStatusBarText(string.Format("Extracting {0}", 
					m_cache.GetReferenceName(info.Instance.ReferenceName)));
			}
			else
				SetStatusBarText(null);
		}
		void OnTagInstanceExtract(TagInstanceExtractionInfo info)
		{
			OnTagInstanceExtractUpdateStatus(info);

			System.Threading.ThreadPool.QueueUserWorkItem(TagInstanceExtraction, info);
		}

		void OnTagInstanceExtractAs(BlamLib.Blam.CacheIndex.Item tag_instance)
		{
			SaveFileDlg.Filter = tag_instance.GroupTag.Filter;
			if (SaveFileDlg.ShowDialog(this) == DialogResult.OK)
			{
				string path = SaveFileDlg.FileName;
				var tiei = new TagInstanceExtractionInfo();

				tiei.InitializeStateForTagInstance(tag_instance,
					System.IO.Path.Combine(System.IO.Path.GetDirectoryName(path), @"\"),
					System.IO.Path.GetFileNameWithoutExtension(path));
				tiei.InitializeExtractionForTagInstance();

				bgwTagExtract.RunWorkerAsync(tiei);
			}
			SaveFileDlg.Filter = "";
		}

		bool OnTagInstanceExtractAskForPath(out string path)
		{
			path = null;

			if (FolderDlg.ShowDialog(this) == DialogResult.OK)
				path = System.IO.Path.Combine(FolderDlg.SelectedPath, @"\");

			return path != null;
		}
		void OnTagInstanceExtract(object sender, EventArgs e)
		{
			if (Program.WorkInProgress(this)) return;

			string path = null;
			if (sender != MenuTagInstanceExtractAs && // this has its own dialog for saving
				!OnTagInstanceExtractAskForPath(out path))
				return;

			BlamLib.Blam.CacheIndex.Item tag_instance;
			{
				var sel_node = TagTreeView.SelectedNode;
				if (sel_node == null) return;

				tag_instance = sel_node.Tag as BlamLib.Blam.CacheIndex.Item;
			};

			var tiei = new TagInstanceExtractionInfo();
			if (sender == MenuTagInstanceExtractFolder)
			{
				tiei.InitializeStateForTagInstance(tag_instance, path, null);
				tiei.InitializeExtractionForTagInstance();

				bgwTagExtract.RunWorkerAsync(tiei);
			}
			else if (sender == MenuTagInstanceExtractFolderAll)
			{
//				tiei.InitializeStateForTagHierarchy(
//					GetTagInstanceExtractionArguments(path, true), tag_instance);
//				tiei.InitializeExtractionForTagInstance();

				bgwTagExtract.RunWorkerAsync(tiei);
			}
			else if (sender == MenuTagInstanceExtractAs)
				OnTagInstanceExtractAs(tag_instance);
		}

		/// <summary></summary>
		/// <param name="tiei"></param>
		/// <param name="node">GroupTag node</param>
		/// <param name="all"></param>
		/// <param name="r_checked"></param>
		void OnTagInstanceExtractAll(TagInstanceExtractionInfo tiei, TreeNode node, bool all, bool r_checked) // threaded
		{
			foreach (TreeNode n in node.Nodes)
			{
				if(!all)
					if ((r_checked && !n.Checked) || (!r_checked && n.Checked))
						continue; // skip tags not checked or checked

				tiei.Instance = n.Tag as BlamLib.Blam.CacheIndex.Item;
				OnTagInstanceExtract(tiei);
				//m_waitEvent.WaitOne(); // wait until this tag finishes extracting before moving onto the next
			}
		}
		void OnTagInstanceExtractAll(string path, bool all, bool r_checked)
		{
			var tiei = new TagInstanceExtractionInfo(null, path, null);
			// Disable the tag tree, and set a worker task to process all checked 
			// tags. Once the task finishes, it releases the tag tree back to the user
			TagTreeView.Enabled = false;
			TagTreeView.UseWaitCursor = true;
			StatusProgressCancel.Enabled = true;

			System.Threading.ThreadPool.QueueUserWorkItem((obj) => {
				foreach (TreeNode n in TagTreeView.Nodes)
					OnTagInstanceExtractAll(tiei, n, all, r_checked);

				TagTreeView.Enabled = true;
				TagTreeView.UseWaitCursor = false;
				StatusProgressCancel.Enabled = false;
			}, null);
		}

		void OnTagInstanceExtractAll(object sender, EventArgs e)
		{
			if (Program.WorkInProgress(this)) return;

			string path;
			if (!OnTagInstanceExtractAskForPath(out path))
				return;

			if (sender == CacheToolsExtractAll)					OnTagInstanceExtractAll(path, true, false);
			else if (sender == CacheToolsExtractAllChecked)		OnTagInstanceExtractAll(path, true, true);
			else if (sender == CacheToolsExtractAllUnchecked)	OnTagInstanceExtractAll(path, false, false);
		}
	};
}