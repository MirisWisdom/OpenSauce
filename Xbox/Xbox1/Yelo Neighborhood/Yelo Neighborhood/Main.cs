using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using YeloDebug;
using System.IO;
using System.Threading;
using System.Xml;

namespace Yelo_Neighborhood
{
    public partial class Main : Form
    {
        public int Version { get { return 4; } }

        public enum Images
        {
            HDD = 0,
            Folder = 1,
            XBE = 2,
            Map = 3,
            Bik = 4,
            Other = 5,
        }

        string CurrentDirectory = "";
        Stack<string> DirectoryBackHistory = new Stack<string>();
        Stack<string> DirectoryForwardHistory = new Stack<string>();

        public Main()
        {
            InitializeComponent();
            LoadPartitions();

            Text = Program.XBox.DebugName + " - " + Program.XBox.DebugIP + " - Yelo Neighborhood v" + Version;

            LoadScripts();
        }

        public void RefreshFiles()
        {
            if (CurrentDirectory == "") LoadPartitions();
            else LoadDirectory(CurrentDirectory);
        }

        public void LoadPartitions()
        {
            CurrentDirectory = "";
            
            listFiles.Clear();

            ListViewGroup partitionsGroup = new ListViewGroup("Partitions");
            listFiles.Groups.Add(partitionsGroup);

            List<string> Partitions = Program.XBox.GetPartitions();
            foreach (string s in Partitions)
                listFiles.Items.Add(new ListViewItem(s, (int)Images.HDD, partitionsGroup));

            listFiles.LabelEdit = false;
            listFiles.ContextMenuStrip = null;
            cmdNewFolder.Enabled = false;
        }

        public void LoadDirectory(string dir)
        {
            List<FileInformation> files = null;
            try
            { files = Program.XBox.GetDirectoryList(dir); }
            catch
            {
                MessageBox.Show("Could Not Access: " + dir, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }

            CurrentDirectory = dir;

            listFiles.Clear();
            
            ListViewGroup foldersGroup = new ListViewGroup("Folders");
            ListViewGroup filesGroup = new ListViewGroup("Files");

            listFiles.Groups.Add(foldersGroup);
            listFiles.Groups.Add(filesGroup);
            
            foreach (FileInformation fi in files)
            {
                ListViewItem lvt = new ListViewItem();
                lvt.Tag = fi;

                if (fi.Attributes == FileAttributes.Directory)
                {
                    lvt.Group = foldersGroup;
                    lvt.ImageIndex = (int)Images.Folder;
                }
                else
                {
                    lvt.Group = filesGroup;
                    switch (new FileInfo(fi.Name).Extension)
                    {
                        case ".xbe":
                            lvt.ImageIndex = (int)Images.XBE;
                            break;
                        case ".bik":
                            lvt.ImageIndex = (int)Images.Bik;
                            break;
                        case ".map":
                            lvt.ImageIndex = (int)Images.Map;
                            break;
                        default:
                            lvt.ImageIndex = (int)Images.Other;
                            break;
                    }
                }
                lvt.Text = fi.Name;
                lvt.SubItems.Add(new ListViewItem.ListViewSubItem(lvt, fi.ChangeTime.ToString()));
                lvt.SubItems.Add(new ListViewItem.ListViewSubItem(lvt, fi.Size.ToString()));
                listFiles.Items.Add(lvt);
            }

            listFiles.LabelEdit = true;
            listFiles.ContextMenuStrip = mnuFiles;
            cmdNewFolder.Enabled = true;
        }

        #region Drag-Drop Files
        private void listFiles_DragEnter(object sender, DragEventArgs e)
        { if (CurrentDirectory != "" && e.Data.GetDataPresent(DataFormats.FileDrop, false)) e.Effect = DragDropEffects.Copy; }

        private void listFiles_DragDrop(object sender, DragEventArgs e)
        {
            this.Enabled = false;
            List<FileInformation> files = new List<FileInformation>();
            foreach (string s in (string[])e.Data.GetData(DataFormats.FileDrop))
            {
                FileInformation fi = new FileInformation();
                fi.Name = s;
                if (Directory.Exists(s)) fi.Attributes = FileAttributes.Directory;
                files.Add(fi);
            }

            probar.Style = ProgressBarStyle.Marquee;
            sendFileWorker.RunWorkerAsync(files);
        }

        private void sendFileWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            string workingDir = CurrentDirectory;
            List<FileInformation> files = (List<FileInformation>)e.Argument;
            foreach (FileInformation fi in files)
            {
                if (fi.Attributes == FileAttributes.Directory) SendDirectory(fi, workingDir);
                else SendFile(fi, workingDir);
            }
        }

        private void SendDirectory(FileInformation dir, string workingDir)
        {
            string dirname = Path.GetFileName(dir.Name);
            if (!Program.XBox.FileExists(Path.Combine(workingDir, dirname))) Program.XBox.CreateDirectory(Path.Combine(workingDir, dirname));
            foreach (string s in Directory.GetFiles(dir.Name, "*", SearchOption.TopDirectoryOnly))
            {
                FileInformation fi = new FileInformation();
                fi.Name = s;
                SendFile(fi, Path.Combine(workingDir, dirname));
            }
            foreach (string s in Directory.GetDirectories(dir.Name, "*", SearchOption.TopDirectoryOnly))
            {
                FileInformation fi = new FileInformation();
                fi.Name = s;
                SendDirectory(fi, Path.Combine(workingDir, dirname));
            }
        }

        private void SendFile(FileInformation file, string workingDir)
        {
            string filename = Path.GetFileName(file.Name);
            string xboxFilename = Path.Combine(workingDir, filename);
            if (Program.XBox.FileExists(xboxFilename) && MessageBox.Show(filename + "\n\nWould You Like To Overwrite The Old File?", "File Already Exists.", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.No) return;
            lblStatus.Text = "Sending File: " + filename;

            Program.XBox.SendFile(file.Name, xboxFilename);
        }
        #endregion

        #region Navigation
        private void cmdUpDir_Click(object sender, EventArgs e)
        {
            if (CurrentDirectory == "") return;
            
            DirectoryBackHistory.Push(CurrentDirectory);
            cmdBack.Enabled = true;

            DirectoryForwardHistory.Clear();
            cmdForward.Enabled = false;
            
            DirectoryInfo di = new DirectoryInfo(CurrentDirectory);
            if (di.Parent != null) LoadDirectory(di.Parent.FullName);
            else LoadPartitions();
        }

        private void cmdBack_Click(object sender, EventArgs e)
        {
            DirectoryForwardHistory.Push(CurrentDirectory);
            string dir = DirectoryBackHistory.Pop();

            if (dir == "") LoadPartitions();
            else LoadDirectory(dir);

            if (DirectoryBackHistory.Count == 0) cmdBack.Enabled = false;
            cmdForward.Enabled = true;
        }

        private void cmdForward_Click(object sender, EventArgs e)
        {
            DirectoryBackHistory.Push(CurrentDirectory);
            string dir = DirectoryForwardHistory.Pop();

            if (dir == "") LoadPartitions();
            else LoadDirectory(dir);

            if (DirectoryForwardHistory.Count == 0) cmdForward.Enabled = false;
            cmdBack.Enabled = true;
        }

        private void cmdRefresh_Click(object sender, EventArgs e)
        { RefreshFiles(); }
        #endregion

        #region Reboot
        private void cmdCyclePower_Click(object sender, EventArgs e)
        {
            this.Enabled = false;
            try
            { Program.XBox.CyclePower(); }
            catch { }
            finally
            {
                Hide();
                if (Properties.Settings.Default.AutoDiscover) Program.FindXBox();
                else Program.FindXBox(Properties.Settings.Default.XBoxIP);
            }
        }

        private void cmdReset_Click(object sender, EventArgs e)
        {
            this.Enabled = false;
            Program.XBox.Reset();
            Hide(); 
            if (Properties.Settings.Default.AutoDiscover) Program.FindXBox();
            else Program.FindXBox(Properties.Settings.Default.XBoxIP);
        }

        private void cmdShutdown_Click(object sender, EventArgs e)
        {
            this.Enabled = false;
            try
            { Program.XBox.Shutdown(); }
            catch { }
            finally
            { Application.Exit(); }
        }

        private void cmdCustomReboot_Click(object sender, EventArgs e)
        { }
        #endregion

        #region Tray
        private void cmdEjectTray_Click(object sender, EventArgs e)
        { Program.XBox.EjectTray(); }

        private void cmdLoadTray_Click(object sender, EventArgs e)
        { Program.XBox.LoadTray(); }
        #endregion

        #region Update
        private void cmdCheckForUpdates_Click(object sender, EventArgs e)
        {
            Updater.Jobs.DownloadDirectory = new Uri("ftp://ftp.acemods.org/Halo%202%20Xbox%20Apps/Yelo%20Neighborhood/");
            Updater.Jobs.UserName = "h2apps@acemods.org";
            Updater.Jobs.Password = ",QI&X4?iUEpY";
            Updater.Jobs.ProgramName = "Yelo%20Neighborhood";
            Updater.Jobs.CheckForUpdates(Version);
        }
        #endregion

        private void listFiles_ItemActivate(object sender, EventArgs e)
        {
            FileInformation selectedFile = (FileInformation)listFiles.SelectedItems[0].Tag;
            if (selectedFile == null || selectedFile.Attributes == FileAttributes.Directory)
            {
                DirectoryBackHistory.Push(CurrentDirectory);
                cmdBack.Enabled = true;

                DirectoryForwardHistory.Clear();
                cmdForward.Enabled = false;

                LoadDirectory(Path.Combine(CurrentDirectory, listFiles.SelectedItems[0].Text));
            }
            else
            {
                FileInfo fi = new FileInfo(Path.Combine(CurrentDirectory, selectedFile.Name));
                switch (fi.Extension)
                {
                    case ".xbe":
                        lblStatus.Text = "Launching: " + fi.Name;
                        probar.Style = ProgressBarStyle.Marquee;
                        launchTitleWorker.RunWorkerAsync(fi);
                        break;
                }
            }
        }

        private void screenshotToolToolStripMenuItem_Click(object sender, EventArgs e)
        { Program.ScreenshotTool.ShowDialog(); }

        private void launchTitleWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            FileInfo fi = (FileInfo)e.Argument;
            Program.XBox.LaunchTitle(fi.FullName);
        }

        private void Worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            this.Enabled = true;
            probar.Style = ProgressBarStyle.Blocks;
            lblStatus.Text = "Ready.";
            RefreshFiles();
        }

        private void listFiles_KeyUp(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Delete:
                    cmdDelete_Click(null, null);
                    break;
            }
        }

        private void deleteFileWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            this.Enabled = false;
            string workingDir = CurrentDirectory;
            List<FileInformation> files = (List<FileInformation>)e.Argument;
            foreach (FileInformation fi in files)
            {
                lblStatus.Text = "Deleting: " + fi.Name;
                if (fi.Attributes == FileAttributes.Directory) DeleteDirectory(fi, workingDir);
                else Program.XBox.DeleteFile(Path.Combine(workingDir, fi.Name));
            }
        }

        private void DeleteDirectory(FileInformation dir, string workingDir)
        {
            List<FileInformation> files = Program.XBox.GetDirectoryList(Path.Combine(workingDir, dir.Name));
            foreach (FileInformation fi in files)
            {
                lblStatus.Text = "Deleting: " + fi.Name;
                if (fi.Attributes == FileAttributes.Directory) DeleteDirectory(fi, Path.Combine(workingDir, dir.Name));
                else Program.XBox.DeleteFile(Path.Combine(Path.Combine(workingDir, dir.Name), fi.Name));
            }
            Program.XBox.DeleteDirectory(Path.Combine(workingDir, dir.Name));
        }

        private void xBoxToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            if (Program.XBox.Connected && Program.XBox.GetTrayState() != TrayState.Open) cmdEjectTray.Visible = true;
            else cmdEjectTray.Visible = false;
        }

        private void listFiles_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            if (e.Label == "" || e.Label == null)
            {
                e.CancelEdit = true;
                if (listFiles.Items[e.Item].Text == "") listFiles.Items.RemoveAt(e.Item);
                return;
            }

            if (listFiles.Items[e.Item].Text == "")
                Program.XBox.CreateDirectory(Path.Combine(CurrentDirectory, e.Label));
            else
            {
                Program.XBox.RenameFile(Path.Combine(CurrentDirectory, listFiles.Items[e.Item].Text), Path.Combine(CurrentDirectory, e.Label));
                ((FileInformation)listFiles.Items[e.Item].Tag).Name = e.Label;
            }
        }

        private void cmdRename_Click(object sender, EventArgs e)
        { listFiles.SelectedItems[0].BeginEdit(); }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            if (CurrentDirectory == "") return;
            if (MessageBox.Show("Would You Like To Permanently Delete " + listFiles.SelectedItems.Count + " File(s)?", "Comfirm.", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
            List<FileInformation> files = new List<FileInformation>();
            foreach (ListViewItem lvt in listFiles.SelectedItems)
                files.Add((FileInformation)lvt.Tag);
            deleteFileWorker.RunWorkerAsync(files);
        }

        private void cmdNewFolder_Click(object sender, EventArgs e)
        {
            ListViewItem newFolder = new ListViewItem("", (int)Images.Folder, listFiles.Groups[1]);
            listFiles.Items.Add(newFolder);
            newFolder.BeginEdit();
        }
        #region Executables

        private void cmdAddToScriptsMenu_Click(object sender, EventArgs e)
        {
            ExecutableTool ET = new ExecutableTool();
            Executable exe = new Executable();
            exe.Name = "Unknown";
            exe.Filename = Path.Combine(CurrentDirectory, listFiles.SelectedItems[0].Text);
            ET.Executable = exe;
            if (ET.ShowDialog() == DialogResult.OK)
            {
                Program.Executables.Add(exe);
                SaveExecutables();
                LoadScripts();
            }
        }

        private void ExecutableEdit_Click(object sender, EventArgs e)
        {
            ExecutableTool ET = new ExecutableTool();
            ET.Executable = (Executable)((ToolStripMenuItem)sender).Tag;
            if (ET.ShowDialog() == DialogResult.OK)
            {
                SaveExecutables();
                LoadScripts();
            }
        }

        private void ExecutableLaunch_Click(object sender, EventArgs e)
        {
            Executable exe = (Executable)((ToolStripMenuItem)sender).Tag;
            FileInfo fi = new FileInfo(exe.Filename);
            lblStatus.Text = "Launching: " + fi.Name;
            probar.Style = ProgressBarStyle.Marquee;
            launchTitleWorker.RunWorkerAsync(fi);
        }

        private void ExecutableScript_Click(object sender, EventArgs e)
        {
            Executable.Script cmd = (Executable.Script)((ToolStripMenuItem)sender).Tag;
            cmd.Run("");
        }

        private void ExecutableFileScript_Click(object sender, EventArgs e)
        {
            Executable.Script cmd = (Executable.Script)((ToolStripMenuItem)sender).Tag;
            cmd.Run(Path.GetFileNameWithoutExtension(listFiles.SelectedItems[0].Text));
        }

        private void LoadFileScripts(string fileType)
        {
            mnuFiles.Items.Clear();
            mnuFiles.Items.Add(cmdRename);
            mnuFiles.Items.Add(cmdDelete);
            mnuFiles.Items.Add(new ToolStripSeparator());
            if (fileType == ".xbe")
            {
                mnuFiles.Items.Add(cmdAddToScriptsMenu);
                mnuFiles.Items.Add(new ToolStripSeparator());
            }

            foreach (Executable exe in Program.Executables)
            {
                ToolStripMenuItem mi = new ToolStripMenuItem();
                mi.Tag = exe;
                mi.Text = exe.Name;

                foreach (Executable.Script s in exe.Scripts)
                {
                    if ("." + s.FileType != fileType) continue;
                    ToolStripMenuItem miScript = new ToolStripMenuItem();
                    miScript.Tag = s;
                    miScript.Text = s.Name;
                    miScript.Click += new EventHandler(ExecutableFileScript_Click);
                    mi.DropDown.Items.Add(miScript);
                }

                if (mi.DropDown.Items.Count > 0) mnuFiles.Items.Add(mi);
            }
        }

        private void LoadScripts()
        {
            mnuScripts.DropDown.Items.Clear();

            foreach (Executable exe in Program.Executables)
            {
                ToolStripMenuItem mi = new ToolStripMenuItem();
                mi.Tag = exe;
                mi.Text = exe.Name;

                foreach (Executable.Script s in exe.Scripts)
                {
                    if (s.FileType != "" && s.FileType != null) continue;
                    ToolStripMenuItem miScript = new ToolStripMenuItem();
                    miScript.Tag = s;
                    miScript.Text = s.Name;
                    miScript.Click += new EventHandler(ExecutableScript_Click);
                    mi.DropDown.Items.Add(miScript);
                }

                mi.DropDown.Items.Add(new ToolStripSeparator());

                ToolStripMenuItem miEdit = new ToolStripMenuItem();
                miEdit.Tag = exe;
                miEdit.Text = "Edit";
                miEdit.Click += new EventHandler(ExecutableEdit_Click);
                mi.DropDown.Items.Add(miEdit);

                ToolStripMenuItem miLaunch = new ToolStripMenuItem();
                miLaunch.Tag = exe;
                miLaunch.Text = "Launch";
                miLaunch.Click += new EventHandler(ExecutableLaunch_Click);
                mi.DropDown.Items.Add(miLaunch);

                mnuScripts.DropDown.Items.Add(mi);
            }
        }

        public void SaveExecutables()
        {
            XmlDocument xd = new XmlDocument();
            xd.Load("Executables.xml");
            xd.DocumentElement.RemoveAll();

            foreach (Executable exe in Program.Executables)
            {
                XmlElement exeNode = xd.CreateElement("Executable");
                exeNode.SetAttribute("Name", exe.Name);
                exeNode.SetAttribute("Filename", exe.Filename);
                xd.DocumentElement.AppendChild(exeNode);

                foreach (Executable.Script s in exe.Scripts)
                {
                    XmlElement scriptNode = xd.CreateElement("Script");
                    scriptNode.SetAttribute("Name", s.Name);
                    scriptNode.SetAttribute("FileType", s.FileType);
                    scriptNode.InnerText = s.Code;
                    exeNode.AppendChild(scriptNode);
                }
            }

            xd.Save("Executables.xml");
        }

        #endregion

        private void mnuFiles_Opening(object sender, CancelEventArgs e)
        {
            if (listFiles.SelectedItems.Count != 1)
                e.Cancel = true;
            else
                LoadFileScripts(Path.GetExtension(listFiles.SelectedItems[0].Text));
        }
    }
}
