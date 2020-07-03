using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;
using System.IO;

namespace Yelo_Neighborhood
{
    partial class ModuleManager : Form
    {
        BindingList<Executable.Module> modules = new BindingList<Executable.Module>();

        public ModuleManager()
        {
            InitializeComponent();

            for (int e = 0; e < Program.Executables.Count; e++)
                for (int m = 0; m < Program.Executables[e].Modules.Count; m++)
                modules.Add(Program.Executables[e].Modules[m]);

            gridModules.DataSource = modules;
        }

        public void LoadModule(Executable.Module module)
        {
            if (!Program.XBox.Ping()) new Settings().ShowDialog();
            if(this.Visible) this.Enabled = false;
            Program.MainWindow.Enabled = false;
            Program.MainWindow.probar.Style = ProgressBarStyle.Marquee;
            Program.MainWindow.lblStatus.Text = "Loading Module: " + module.Name;
            launchModuleWorker.RunWorkerAsync(module);
        }

        private void cmdNew_Click(object sender, EventArgs e)
        {
            if (Program.NewModule.ShowDialog() == DialogResult.OK)
            {
                Executable.Module newModule = new Executable.Module(Program.NewModule.Filename, Program.NewModule.BaseAddress) { Name = Program.NewModule.ModuleName };
                if(Program.NewModule.ExecutableIndex != -1)
                    Program.Executables[Program.NewModule.ExecutableIndex].Modules.Add(newModule);
                modules.Add(newModule);
                Program.MainWindow.SaveExecutables();
                Program.MainWindow.LoadScripts();
                LoadModule(newModule);
            }
        }

        private void cmdReload_Click(object sender, EventArgs e)
        {
            if (!Program.XBox.Ping()) new Settings().ShowDialog();
            for (int i = 0; i < gridModules.SelectedRows.Count; i++)
                modules[gridModules.SelectedRows[i].Index].Reload();
        }

        private void cmdEnd_Click(object sender, EventArgs e)
        {
            if (!Program.XBox.Ping()) new Settings().ShowDialog();
            for (int i = 0; i < gridModules.SelectedRows.Count; i++)
                modules[gridModules.SelectedRows[i].Index].Unload();
        }

        private void launchModuleWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            Executable.Module module = (Executable.Module)e.Argument;
            module.Reload();
        }

        private void launchModuleWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            this.Enabled = true;
            Program.MainWindow.Enabled = true;
            Program.MainWindow.probar.Style = ProgressBarStyle.Blocks;
            Program.MainWindow.lblStatus.Text = "Ready.";
        }
    }
}
