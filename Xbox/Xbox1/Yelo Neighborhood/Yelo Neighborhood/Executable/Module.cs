using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Windows.Forms;

namespace Yelo_Neighborhood
{
    public partial class Executable
    {
        public class Module
        {
            public enum ModuleStatus
            {
                Error,

                Unloaded,
                Running,
            };

            uint mainAddress, exitAddress;
            FileInfo fileInfo;

            public string Name { get; set; }

            [Browsable(false)]
            public string Filename { get { return fileInfo.FullName; } }

            uint baseAddress;
            public uint BaseAddress { get { return baseAddress; } }

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

            public Module(string filename, uint baseAddress)
            {
                mainAddress = exitAddress = uint.MaxValue;

                this.baseAddress = baseAddress;
                fileInfo = new FileInfo(filename);
                status = ModuleStatus.Unloaded;
            }

            bool Run(out string error_details)
            {
                //YeloDebug.XboxDll.RebaseModuleAndSave(fileInfo.FullName, BaseAddress, out error_details); // Testing...

                bool result = true;
                //if (result)
                {
                    result = YeloDebug.XboxDll.RunModule(Program.XBox, fileInfo.FullName, ref baseAddress,
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
                        Filename, MessageBoxButtons.OK, MessageBoxIcon.Hand);
            }

            public void Unload()
            {
                if (exitAddress != uint.MaxValue)
                {
                    YeloDebug.XboxDll.UnloadModule(Program.XBox, baseAddress, exitAddress);
                    Status = ModuleStatus.Unloaded;
                }
                else
                    MessageBox.Show(Program.ModuleManager, "Module has no unload functionality!",
                        Filename, MessageBoxButtons.OK, MessageBoxIcon.Hand);
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
                return string.Format("[{0}] {1}", Status.ToString(), Filename);
            }
        }
    }
}
