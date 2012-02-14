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
        class YeloModule : INotifyPropertyChanged
        {
            FileInfo fileInfo;

            public string Module { get { return fileInfo.Name; } }
            public ModuleStatus Status { get { return status; } }
            ModuleStatus status;

            public enum ModuleStatus
            {
                Stopped,
                Running
            }

            public YeloModule(string filename)
            {
                fileInfo = new FileInfo(filename);
                status = ModuleStatus.Running;
            }

            public void Reload()
            { NotifyPropertyChanged("Status"); }

            public void Stop()
            { NotifyPropertyChanged("Status"); }

            #region INotifyPropertyChanged Members

            public event PropertyChangedEventHandler PropertyChanged;

            private void NotifyPropertyChanged(string name)
            {
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs(name));
            }

            #endregion
        }

        BindingList<YeloModule> modules = new BindingList<YeloModule>();

        public ModuleManager()
        {
            InitializeComponent();
            gridModules.DataSource = modules;
        }

        private void cmdNew_Click(object sender, EventArgs e)
        {
            if (OFD.ShowDialog() != DialogResult.OK) return;
            modules.Add(new YeloModule(OFD.FileName));
        }

        private void cmdReload_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < gridModules.SelectedRows.Count; i++)
                modules[gridModules.SelectedRows[i].Index].Reload();
        }

        private void cmdEnd_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < gridModules.SelectedRows.Count; i++)
                modules[gridModules.SelectedRows[i].Index].Stop();
        }
    }
}
