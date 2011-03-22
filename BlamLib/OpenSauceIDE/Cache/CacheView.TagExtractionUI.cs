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
using System.Windows.Forms;

namespace OpenSauceIDE.Cache
{
	partial class CacheView
	{
		void OnTagInstanceExtractUpdateStatus(TagInstanceExtractionInfo info)
		{
			if (info != null)
			{
				m_waitEvent.Reset();
				SetStatusBarText(string.Format("Extracting {0}", 
					m_cache.GetReferenceName(info.Instance.ReferenceName)));
			}
			else
			{
				m_waitEvent.Set();
				SetStatusBarText(null);
			}
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
				var tiei = new TagInstanceExtractionInfo(tag_instance,
					System.IO.Path.Combine(System.IO.Path.GetDirectoryName(path), @"\"),
					System.IO.Path.GetFileNameWithoutExtension(path));
				OnTagInstanceExtract(tiei);
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

			if (sender == MenuTagInstanceExtractFolder)
			{
				var tiei = new TagInstanceExtractionInfo(tag_instance, path, null);
				OnTagInstanceExtract(tiei);
			}
			else if (sender == MenuTagInstanceExtractFolderAll)
			{
				var tiei = new TagInstanceExtractionInfo(
					GetTagInstanceExtractionArguments(path, true), tag_instance);
				OnTagInstanceExtract(tiei);
			}
			else if (sender == MenuTagInstanceExtractAs)
				OnTagInstanceExtractAs(tag_instance);
		}

		/// <summary></summary>
		/// <param name="tiei"></param>
		/// <param name="node">GroupTag node</param>
		/// <param name="all"></param>
		/// <param name="r_checked"></param>
		void OnTagInstanceExtractAll(TagInstanceExtractionInfo tiei, TreeNode node, bool all, bool r_checked)
		{
			foreach (TreeNode n in node.Nodes)
			{
				if(!all)
					if ((r_checked && !n.Checked) || (!r_checked && n.Checked))
						continue; // skip tags not checked or checked

				tiei.Instance = n.Tag as BlamLib.Blam.CacheIndex.Item;
				OnTagInstanceExtract(tiei);
			}
		}
		void OnTagInstanceExtractAll(string path, bool all, bool r_checked)
		{
			var tiei = new TagInstanceExtractionInfo(null, path, null);

			foreach (TreeNode n in TagTreeView.Nodes)
				OnTagInstanceExtractAll(tiei, n, all, r_checked);
		}

		void OnTagInstanceExtractAll(object sender, EventArgs e)
		{
			string path;
			if (!OnTagInstanceExtractAskForPath(out path))
				return;

			if (sender == CacheToolsExtractAll)					OnTagInstanceExtractAll(path, true, false);
			else if (sender == CacheToolsExtractAllChecked)		OnTagInstanceExtractAll(path, true, true);
			else if (sender == CacheToolsExtractAllUnchecked)	OnTagInstanceExtractAll(path, false, false);
		}
	};
}