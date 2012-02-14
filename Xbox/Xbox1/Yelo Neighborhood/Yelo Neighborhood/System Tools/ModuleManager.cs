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
			uint mainAddress, exitAddress;
			FileInfo fileInfo;

			public uint BaseAddress { get; private set; }
			public string Module { get { return fileInfo.Name; } }
			ModuleStatus status;
			public ModuleStatus Status
			{
				get { return status; }
				set
				{
					if (status != value)
					{
						status = value;
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

			public YeloModule(string filename, uint baseAddress)
			{
				mainAddress = exitAddress = uint.MaxValue;

				BaseAddress = baseAddress;
				fileInfo = new FileInfo(filename);
				status = ModuleStatus.Error;
				Reload();
			}

			bool Run(out string error_details)
			{
				YeloDebug.XboxDll.RebaseModuleAndSave(fileInfo.FullName, BaseAddress, out error_details); // Testing...

				bool result = false;
				if (result)
				{
					result = YeloDebug.XboxDll.RunModule(Program.XBox, fileInfo.FullName, BaseAddress, 
						out mainAddress, out exitAddress, out error_details);
				}
				return result;
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
				if (exitAddress != uint.MaxValue)
				{
					YeloDebug.XboxDll.UnloadModule(Program.XBox, exitAddress);
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
            if (Program.NewModule.ShowDialog() == DialogResult.OK)
				modules.Add(new YeloModule(Program.NewModule.Filename, Program.NewModule.BaseAddress));
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
