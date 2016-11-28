/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	public partial class CheApe : Form
	{
		/// <summary>The valid engines which CheApe can be used with</summary>
		public const BlamLib.BlamVersion kTargetPlatforms = BlamLib.BlamVersion.Halo1_CE | BlamLib.BlamVersion.Halo2_PC;

		const string kFileExt = "xml";
		const string kFileFilter = "XML Files (*.xml)|*.xml";

		BlamLib.CheApe.Project Project = null;
		BlamLib.CheApe.Project.Interface ProjectInterface = null;

		System.Windows.Forms.TreeNode FileViewRoot;

		public CheApe(BlamLib.BlamVersion engine)
		{
			InitializeComponent();

			MainMenu.Renderer = MainForm.kOpenSauceIDEToolStripRenderer;
			FileViewMenu.Renderer = MainForm.kOpenSauceIDEToolStripRenderer;

			SetState(false);

			#region FileViewRoot
			this.FileViewRoot = new System.Windows.Forms.TreeNode("Project Files");
			FileViewRoot.ContextMenuStrip = this.FileViewMenu;
			FileViewRoot.Name = "FileViewRoot";
			this.FileView.Nodes.Add(FileViewRoot);
			#endregion

			OpenDialog.DefaultExt = kFileExt;
			OpenDialog.Filter = kFileFilter;

			SaveDialog.DefaultExt = kFileExt;
			SaveDialog.Filter = kFileFilter;

			OpenProjDialog.Multiselect = false;
			OpenProjDialog.Filter = BlamLib.IO.TagGroups.CheApeProject.Filter;

			SaveProjDialog.Filter = BlamLib.IO.TagGroups.CheApeProject.Filter;

			ProjectInterface = BlamLib.CheApe.Project.GetInterface(engine);
		}

		void SetState(bool active)
		{
			BuildMenu.Enabled = active;
			FileViewAdd.Enabled = active;
			FileViewRemove.Enabled = active;
			FileClose.Enabled = active;
			FileSave.Enabled = active;
			FileSaveAs.Enabled = active;
		}


		#region Closing Time
		DialogResult PromptSaveDialog()
		{
			DialogResult result;

			result = MessageBox.Show(null, "Do you wish to save the project?", "Che Ape", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);

			return result;
		}

		void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			if (Project == null) return;

			DialogResult result = PromptSaveDialog();

			if (result == DialogResult.Yes)
			{
				ProjectInterface.Close(true, Project);
				Project = null;
			}
			else if (result == DialogResult.Cancel)
				e.Cancel = true;
			else
			{
				ProjectInterface.Close(false, Project);
				Project = null;
			}
		}

		bool FileClosed()
		{
			if (Project == null) return true;

			DialogResult result = PromptSaveDialog();

			if (result != DialogResult.Cancel)
			{
				ProjectInterface.Close(result == DialogResult.Yes ? true : false, Project);
				FileViewRoot.Nodes.Clear();
				Project = null;
			}

			this.Text = "Che Ape";
			SetState(false);

			return result != DialogResult.Cancel;
		}
		#endregion

		#region File Menu
		void OnFileNew(object sender, EventArgs e)
		{
			if (FileClosed() && SaveProjDialog.ShowDialog(this) == DialogResult.OK)
			{
				Project = ProjectInterface.Create(SaveProjDialog.FileName);
				this.Text = string.Format("Che Ape [{0}]", Project.FileName);
				SetState(true);
			}
		}

		void OnFileOpen(object sender, EventArgs e)
		{
			if (FileClosed() && OpenProjDialog.ShowDialog(this) == DialogResult.OK)
			{
				this.Text = string.Format("Che Ape [{0}]", "Opening...");
				Project = ProjectInterface.Open(OpenProjDialog.FileName);
				this.Text = string.Format("Che Ape [{0}]", Project.FileName);
				SetState(true);

				TreeNode node = null;
				FileView.SuspendLayout();
				foreach (string s in Project.Files)
				{
					node = new TreeNode(System.IO.Path.GetFileNameWithoutExtension(s));
					node.Tag = s;
					node.BackColor = System.Drawing.SystemColors.ControlDarkDark;
					node.ForeColor = System.Drawing.Color.LightGreen;
					node.ContextMenuStrip = this.FileViewMenu;
					FileViewRoot.Nodes.Add(node);
				}
				FileView.ResumeLayout();
			}
		}

		void OnFileClose(object sender, EventArgs e)
		{
			FileClosed();
		}

		void OnFileSave(object sender, EventArgs e)
		{
			ProjectInterface.Save(null, Project);
		}

		void OnFileSaveAs(object sender, EventArgs e)
		{
			if (SaveProjDialog.ShowDialog(this) == DialogResult.OK)
			{
				ProjectInterface.Save(SaveProjDialog.FileName, Project);
				this.Text = string.Format("Che Ape [{0}]", Project.FileName);
			}
		}
		#endregion

		void OnBuild(object sender, EventArgs e)
		{
			// This should happen faster than a user can decide 'oh I want to close CheApe' 
			// so for now I'll just ignore the fact that we need to add a wait event for while 
			// the project is building
			System.Threading.ThreadPool.QueueUserWorkItem(Project.Build, this);
		}

		#region File View Menu
		void FileViewAddEnabled(bool enabled)
		{
			FileViewAdd.Enabled = enabled;
			FileViewAddNew.Enabled = enabled;
			FileViewAddExisting.Enabled = enabled;
		}
		void FileViewRemoveEnabled(bool enabled)
		{
			FileViewRemove.Enabled = enabled;
			FileViewRemoveFromProject.Enabled = enabled;
			FileViewRemoveFromDisk.Enabled = enabled;
		}
		void OnFileViewMenuOpening(object sender, CancelEventArgs e)
		{
			if (Project == null)
			{
				FileViewAddEnabled(false);
				FileViewRemoveEnabled(false);
			}
			else if (FileView.SelectedNode.Handle == FileViewRoot.Handle)
			{
				FileViewAddEnabled(true);
				FileViewRemoveEnabled(false);
			}
			else
			{
				FileViewAddEnabled(false);
				FileViewRemoveEnabled(true);
			}
		}

		void FileAdd(string[] file_names, bool create)
		{
			string tag = string.Empty;
			TreeNode node = null;

			FileView.SuspendLayout();
			string error;
			foreach (string path in OpenDialog.FileNames)
			{
				if (!Project.ValidFile(path, out error))
				{
					MessageBox.Show(this, error, "File not added!", MessageBoxButtons.OK, MessageBoxIcon.Error);
					continue;
				}

				if ((tag = Project.Add(path, create)) != null)
				{
					node = new TreeNode(System.IO.Path.GetFileNameWithoutExtension(path));
					node.Tag = tag;
					node.ContextMenuStrip = this.FileViewMenu;
					FileViewRoot.Nodes.Add(node);
				}
			}
			FileView.ResumeLayout();
		}

		void OnFileViewAdd(object sender, EventArgs e)
		{
			if (OpenDialog.ShowDialog(this) == DialogResult.OK)
				FileAdd(OpenDialog.FileNames, false);
		}

		void OnFileViewAddNew(object sender, EventArgs e)
		{
			if (SaveDialog.ShowDialog(this) == DialogResult.OK)
				FileAdd(OpenDialog.FileNames, true);
		}

		void OnFileViewRemove(object sender, EventArgs e)
		{
			TreeNode node = FileView.SelectedNode;
			Project.Remove(node.Tag as string, false);
			node.Remove();
		}

		void OnFileViewRemovePermently(object sender, EventArgs e)
		{
			TreeNode node = FileView.SelectedNode;
			Project.Remove(node.Tag as string, true);
			node.Remove();
		}
		#endregion

		void OnFileViewKeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)Keys.Add) // File View Add
				OnFileViewAdd(null, null);
			else if (e.KeyChar == (char)Keys.Subtract) // File View Remove
				OnFileViewRemove(null, null);
			else if (e.KeyChar == (char)Keys.Insert) // File View Add New
				OnFileViewAddNew(null, null);
			else if (e.KeyChar == (char)Keys.Delete) // File View Remove Perma
				OnFileViewRemovePermently(null, null);
		}
	};
}