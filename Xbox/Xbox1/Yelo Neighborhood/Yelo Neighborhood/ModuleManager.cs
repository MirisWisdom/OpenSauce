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
			uint m_mainAddress, m_exitAddress;
			FileInfo m_fileInfo;

			public string Module { get { return m_fileInfo.Name; } }
			ModuleStatus m_status;
			public ModuleStatus Status
			{
				get { return m_status; }
				set
				{
					if (m_status != value)
					{
						m_status = value;
						NotifyPropertyChanged("Status");
					}
				}
			}

			public enum ModuleStatus
			{
				Error,

				Unloaded,
				Running,
			};

			public YeloModule(string filename)
			{
				m_mainAddress = m_exitAddress = uint.MaxValue;

				m_fileInfo = new FileInfo(filename);
				m_status = ModuleStatus.Error;
				Reload();
			}

			bool Run(out string error_details)
			{
				error_details = "";
				YeloDebug.XboxDll.RebaseModuleAndSave(m_fileInfo.FullName, 0, out error_details); // Testing...
				// TODO
				return false;
			}

			public void Reload()
			{
				string error_details;
				if (Run(out error_details))
					Status = ModuleStatus.Running;
				else
					MessageBox.Show(Program.ModuleManager, string.Format("Module failed to run!\n{0}", error_details),
						Module, MessageBoxButtons.OK, MessageBoxIcon.Hand);
			}

			public void Unload()
			{
				if (m_exitAddress != uint.MaxValue)
				{
					// TODO
					Status = ModuleStatus.Unloaded;
				}
				else
					MessageBox.Show(Program.ModuleManager, "Module has no unload functionality!",
						Module, MessageBoxButtons.OK, MessageBoxIcon.Hand);
			}

			#region INotifyPropertyChanged Members
			public event PropertyChangedEventHandler PropertyChanged;

			private void NotifyPropertyChanged(string name)
			{
				if (PropertyChanged != null)
					PropertyChanged(this, new PropertyChangedEventArgs(name));
			}
			#endregion

			public override string ToString()
			{
				return string.Format("[{0}] {1}", Status.ToString(), Module);
			}
		};

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
                modules[gridModules.SelectedRows[i].Index].Unload();
        }
    }
}
