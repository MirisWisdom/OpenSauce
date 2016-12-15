/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace OpenSauceIDE.Cache
{
	partial class CacheView
	{
		enum TagExtractionSender
		{
			/// <summary>Tag extraction was initiated by a tag tree operation</summary>
			TagTree,
			/// <summary>Tag extraction was initiated by a tag instance operation</summary>
			TagInstance,
		};

		BlamLib.Blam.CacheExtractionArguments GetTagInstanceExtractionArguments(string output_dir, bool with_dependents)
		{
			return new BlamLib.Blam.CacheExtractionArguments(output_dir,
				CacheToolsOutputTagDatabase.Checked,
				with_dependents,
				CacheToolsDontOverwrite.Checked,
				m_groupsInvalidForExtraction);
		}
		class TagInstanceExtractionInfo
		{
			TagExtractionSender m_sender;
			public bool Result;

			public TagInstanceExtractionInfo() { }
			/// <summary>Constructor for processing a tag hierarchy</summary>
			/// <param name="args"></param>
			/// <param name="instance"></param>
			public TagInstanceExtractionInfo(BlamLib.Blam.CacheExtractionArguments args, BlamLib.Blam.CacheIndex.Item instance)
			{
				InitializeStateForTagHierarchy(args, instance);
			}
			/// <summary>Constructor for processing a single tag</summary>
			/// <param name="instance"></param>
			/// <param name="base_dir"></param>
			/// <param name="name_override"></param>
			public TagInstanceExtractionInfo(BlamLib.Blam.CacheIndex.Item instance, string base_dir, string name_override)
			{
				InitializeStateForTagInstance(instance, base_dir, name_override);
			}

			#region Initialize State
			public BlamLib.Blam.CacheExtractionArguments Arguments;
			public BlamLib.Blam.CacheIndex.Item Instance;

			public string BaseDirectory, NameOverride;

			/// <summary>Initialize state for processing a single tag</summary>
			/// <param name="instance"></param>
			/// <param name="base_dir"></param>
			/// <param name="name_override"></param>
			public void InitializeStateForTagInstance(BlamLib.Blam.CacheIndex.Item instance, string base_dir, string name_override)
			{
				m_sender = TagExtractionSender.TagInstance;

				Instance = instance;
				BaseDirectory = base_dir;
				NameOverride = name_override;
			}
			/// <summary>Initialize state for processing a tag hierarchy</summary>
			/// <param name="args"></param>
			/// <param name="instance"></param>
			public void InitializeStateForTagHierarchy(BlamLib.Blam.CacheExtractionArguments args, BlamLib.Blam.CacheIndex.Item instance)
			{
				m_sender = TagExtractionSender.TagTree;

				Arguments = args;
				Instance = instance;
			}
			#endregion

			/// <summary>Are we extracting for a single tag only?</summary>
			public bool ExtractingSingleTag { get { return m_sender == TagExtractionSender.TagInstance; } }

			#region Background worker interface
			public System.ComponentModel.BackgroundWorker BgWorker;
			public System.ComponentModel.DoWorkEventArgs BgWorkerArgs;

			public void InitializeBackgroundWorker(object sender, System.ComponentModel.DoWorkEventArgs args)
			{
				BgWorker = sender as System.ComponentModel.BackgroundWorker;
				BgWorkerArgs = args;
			}
			/// <summary>Is this task waiting to be canceled?</summary>
			/// <returns></returns>
			public bool CancelRequested()
			{
				if (BgWorker != null && BgWorker.CancellationPending)
					return BgWorkerArgs.Cancel = true;

				return false;
			}

			int m_progressPercent = 0;
			public void InitializeProgress()
			{
				m_progressPercent = 0;
			}
			public void AdvanceProgress(int advancement_percent)
			{
				m_progressPercent += advancement_percent;

				if (BgWorker != null)
					BgWorker.ReportProgress(m_progressPercent, this);
			}
			#endregion

			#region Initialize Extract
			public void InitializeExtractionForTagInstance()
			{
			}

			System.Windows.Forms.TreeNodeCollection m_extractAllNodes;
			bool m_extractAll, m_extractAllChecked;
			public void InitializeExtractionForTagHierarchy(System.Windows.Forms.TreeNodeCollection nodes,
				bool all, bool r_checked)
			{
				m_extractAllNodes = nodes;
				m_extractAll = all;
				m_extractAllChecked = r_checked;
			}
			#endregion

			#region Extraction
			CacheView m_ownerView;

			void PerformExtractionTagInstance()
			{
				m_ownerView.OnTagInstanceExtractUpdateStatus(this);

				var ti = m_ownerView.m_cache.TagIndexManager;

				ti.ExtractionInitialize();
				this.Result = false;

				if (ExtractingSingleTag)
					this.Result = ti.Extract(this.Instance.Datum, this.BaseDirectory, this.NameOverride);
				else
				{
					var cei = ti.ExtractionBegin(this.Instance.Datum, this.Arguments);
					this.Result = ti.Extract(cei);
					ti.ExtractionEnd();
				}

				m_ownerView.OnTagInstanceExtractUpdateStatus(null);
			}

			void PerformExtractionTagHierarchyNode(System.Windows.Forms.TreeNode node, bool all, bool r_checked)
			{
				foreach (System.Windows.Forms.TreeNode n in node.Nodes)
				{
					if (!all)
						if ((r_checked && !n.Checked) || (!r_checked && n.Checked))
							continue; // skip tags not checked or checked

					this.Instance = n.Tag as BlamLib.Blam.CacheIndex.Item;
					PerformExtractionTagInstance();
				}
			}
			void PerformExtractionTagHierarchy()
			{
				foreach (System.Windows.Forms.TreeNode n in m_extractAllNodes)
					PerformExtractionTagHierarchyNode(n, m_extractAll, m_extractAllChecked);
			}

			public void PerformExtraction(CacheView owner_view)
			{
				m_ownerView = owner_view;

				switch (m_sender)
				{
					case TagExtractionSender.TagInstance:
						PerformExtractionTagInstance();
						break;
					case TagExtractionSender.TagTree:
						PerformExtractionTagHierarchy();
						break;
				}
			}
			#endregion
		};

		void TagInstanceExtraction(object state) // threaded
		{
			var info = state as TagInstanceExtractionInfo;
			var ti = m_cache.TagIndexManager;

			ti.ExtractionInitialize();
			info.Result = false;

			if (!info.ExtractingSingleTag)
			{
				var cei = ti.ExtractionBegin(info.Instance.Datum, info.Arguments);
				info.Result = ti.Extract(cei);
				ti.ExtractionEnd();
			}
			else
				info.Result = ti.Extract(info.Instance.Datum, info.BaseDirectory, info.NameOverride);

			this.BeginInvoke(new Action<TagInstanceExtractionInfo>(OnTagInstanceExtractUpdateStatus), null);
		}
	};
}