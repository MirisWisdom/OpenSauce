/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using YeloDebug.Exceptions;

namespace YeloDebug
{
	public partial class Xbox : IDisposable
    {
        /// <summary>
        /// Dont use this, higher-level methods are available.  Use GetDriveFreeSpace or GetDriveSize instead.
        /// </summary>
        /// <param name="drive"></param>
        /// <param name="freeBytes"></param>
        /// <param name="driveSize"></param>
        /// <param name="totalFreeBytes"></param>
        private void GetDriveInformation(DriveLabel drive, out ulong freeBytes, out ulong driveSize, out ulong totalFreeBytes)
        {
            freeBytes = 0; driveSize = 0; totalFreeBytes = 0;
            SendCommand("drivefreespace name=\"{0}\"", drive.ToString() + ":\\");

            string msg = ReceiveMultilineResponse();
            freeBytes = Convert.ToUInt64(msg.Substring(msg.IndexOf("freetocallerlo") + 17, 8), 16);
            freeBytes |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("freetocallerhi") + 17, 8), 16) << 32);

            driveSize = Convert.ToUInt64(msg.Substring(msg.IndexOf("totalbyteslo") + 15, 8), 16);
            driveSize |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("totalbyteshi") + 15, 8), 16) << 32);

            totalFreeBytes = Convert.ToUInt64(msg.Substring(msg.IndexOf("totalfreebyteslo") + 19, 8), 16);
            totalFreeBytes |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("totalfreebyteshi") + 19, 8), 16) << 32);
        }

        /// <summary>
        /// Retrieves xbox drive freespace.
        /// </summary>
        /// <param name="drive">Drive name.</param>
        /// <returns>Free space available.</returns>
        public ulong GetDriveFreeSpace(DriveLabel drive)
        {
            ulong FreeBytes = 0, DriveSize = 0, TotalFreeBytes = 0;
            GetDriveInformation(drive, out FreeBytes, out DriveSize, out TotalFreeBytes);
            return FreeBytes;
        }

        /// <summary>
        /// Retrieves xbox drive size.
        /// </summary>
        /// <param name="drive">Drive name.</param>
        /// <returns>Total space available.</returns>
        public ulong GetDriveSize(DriveLabel drive)
        {
            ulong FreeBytes = 0, DriveSize = 0, TotalFreeBytes = 0;
            GetDriveInformation(drive, out FreeBytes, out DriveSize, out TotalFreeBytes);
            return DriveSize;
        }

        /// <summary>
        /// Gets the current file access count.
        /// </summary>
        /// <returns></returns>
        public int GetFileAccessCount() { return GetInt32(0xB002BAA8); }

        /// <summary>
        /// Gets a list of partitions on the xbox hard drive.
        /// </summary>
        public System.Collections.Generic.List<string> GetPartitions()
        {
            int oldTimeout = timeout;   // sometimes hdd can be slow so we increase our timeout
            timeout = 10000;
			var List = new System.Collections.Generic.List<string>();
            StatusResponse response = SendCommand("drivelist");

            for (int i = 0; i < response.Message.Length; i++)
                List.Add(response.Message[i] + ":\\");

            List.Sort();

            timeout = oldTimeout;
            return List;
        }

        /// <summary>
        /// Retrieves files that belong to a given directory.
        /// </summary>
        /// <param name="name">Directory name.</param>
        /// <returns>List of files.</returns>
		public System.Collections.Generic.List<FileInformation> GetDirectoryList(string name)
        {
			var files = new System.Collections.Generic.List<FileInformation>();

            StatusResponse response = SendCommand("dirlist name=\"{0}\"", name);
            if (response.Type == ResponseType.MultiResponse)
            {
                for(string msg = ReceiveSocketLine(); msg[0] != '.'; msg = ReceiveSocketLine())
                {
                    FileInformation info = new FileInformation();

                    info.Name = msg.Substring(msg.IndexOf("\"") + 1, msg.LastIndexOf("\"") - msg.IndexOf("\"") - 1);

                    // devs fucked up size output so we need to parse carefully... ;X
                    int sizehistart = msg.IndexOf("sizehi") + 9;
                    int sizehiend = msg.IndexOf("sizelo") - 1;
                    int sizelostart = sizehiend + 10;
                    int sizeloend = msg.IndexOf("createhi") - 1;

                    info.Size = Convert.ToUInt64(msg.Substring(sizelostart, sizeloend - sizelostart), 16);
                    info.Size |= (Convert.ToUInt64(msg.Substring(sizehistart, sizehiend - sizehistart), 16) << 32);

                    ulong createtime;
                    createtime = Convert.ToUInt64(msg.Substring(msg.IndexOf("createlo") + 11, 8), 16);
                    createtime |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("createhi") + 11, 8), 16) << 32);
                    info.CreationTime = DateTime.FromFileTime((long)createtime);

                    ulong changetime;
                    changetime = Convert.ToUInt64(msg.Substring(msg.IndexOf("changelo") + 11, 8), 16);
                    changetime |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("changehi") + 11, 8), 16) << 32);
                    info.ChangeTime = DateTime.FromFileTime((long)changetime);

                    if (msg.Contains("directory")) info.Attributes |= FileAttributes.Directory;
                    else info.Attributes |= FileAttributes.Normal;

                    if (msg.Contains("readonly")) info.Attributes |= FileAttributes.ReadOnly;
                    if (msg.Contains("hidden")) info.Attributes |= FileAttributes.Hidden;

                    files.Add(info);
                }
            }
            return files;
        }

        /// <summary>
        /// Creates a directory on the xbox.
        /// </summary>
        /// <param name="name">Directory name.</param>
        public void CreateDirectory(string name)
        {
            SendCommand("mkdir name=\"{0}\"", name);
        }

        /// <summary>
        /// Deletes a directory on the xbox.
        /// </summary>
        /// <param name="name">Directory name.</param>
        public void DeleteDirectory(string name)
        {
            SendCommand("delete name=\"{0}\" dir", name);
        }

        /// <summary>
        /// Determines if the given file exists.
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public bool FileExists(string fileName)
        {
            try
            {
                SendCommand("getfileattributes name=\"{0}\"", fileName);
                ReceiveMultilineResponse();
                return true;
            }
            catch { return false; }
        }

        /// <summary>
        /// Sends a file to the xbox.
        /// </summary>
        /// <param name="localName">PC file name.</param>
        /// <param name="remoteName">Xbox file name.</param>
        public void SendFile(string localName, string remoteName)
        {
			using (var fs = new FileStream(localName, FileMode.Open))
			using (var xfs = new XboxFileStream(this, remoteName, FileMode.Create))
			{
				byte[] data = new byte[(int)fs.Length];
				fs.Read(data, 0, data.Length);
				xfs.Write(data, 0, data.Length);
			}

            //System.IO.FileStream lfs = new System.IO.FileStream(localName, FileMode.Open);
            //byte[] fileData = new byte[connection.Client.SendBufferSize];
            //SendCommand("sendfile name=\"{0}\" length={1}", remoteName, lfs.Length);

            //int mainIterations = (int)lfs.Length / connection.Client.SendBufferSize;
            //int remainder = (int)lfs.Length % connection.Client.SendBufferSize;

            //for (int i = 0; i < mainIterations; i++)
            //{
            //    lfs.Read(fileData, 0, fileData.Length);
            //    SendBinaryData(fileData);
            //}
            //lfs.Read(fileData, 0, remainder);
            //SendBinaryData(fileData, remainder);

            //lfs.Close();
        }

        /// <summary>
        /// Receives a file from the xbox.
        /// </summary>
        /// <param name="localName">PC file name.</param>
        /// <param name="remoteName">Xbox file name.</param>
        public void ReceiveFile(string localName, string remoteName)
        {
            SendCommand("getfile name=\"{0}\"", remoteName);
            int fileSize = BitConverter.ToInt32(ReceiveBinaryData(4), 0);
			using (var lfs = new System.IO.FileStream(localName, FileMode.Create))
			{
				byte[] fileData = new byte[connection.Client.ReceiveBufferSize];

				int mainIterations = fileSize / connection.Client.ReceiveBufferSize;
				int remainder = fileSize % connection.Client.ReceiveBufferSize;

				for (int i = 0; i < mainIterations; i++)
				{
					fileData = ReceiveBinaryData(fileData.Length);
					lfs.Write(fileData, 0, fileData.Length);
				}
				fileData = ReceiveBinaryData(remainder);
				lfs.Write(fileData, 0, remainder);
			}
        }

        /// <summary>
        /// Renames or moves a file on the xbox.
        /// </summary>
        /// <param name="oldFileName">Old file name.</param>
        /// <param name="newFileName">New file name.</param>
        public void RenameFile(string oldFileName, string newFileName)
        {
            SendCommand("rename name=\"{0}\" newname=\"{1}\"", oldFileName, newFileName);
        }

        /// <summary>
        /// Creates a file on the xbox.
        /// </summary>
        /// <param name="fileName">File to create.</param>
        /// <param name="createDisposition">Creation options.</param>
        public void CreateFile(string fileName, FileMode createDisposition)
        {
            if (createDisposition == FileMode.Open) { if (!FileExists(fileName)) throw new FileNotFoundException("File does not exist."); }
            else if (createDisposition == FileMode.Create) SendCommand("fileeof name=\"" + fileName + "\" size=0 cancreate");
            else if (createDisposition == FileMode.CreateNew) SendCommand("fileeof name=\"" + fileName + "\" size=0 mustcreate");
            else throw new UnsupportedException("Unsupported FileMode.");
        }

        /// <summary>
        /// Deletes a file on the xbox.
        /// </summary>
        /// <param name="fileName">File to delete.</param>
        public void DeleteFile(string fileName)
        {
            SendCommand("delete name=\"{0}\"", fileName);
        }

        /// <summary>
        /// Sets the size of a specified file on the xbox.  This method will not zero out any extra bytes that may have been created.
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="size"></param>
        public void SetFileSize(string fileName, int size)
        {
            SendCommand("fileeof name=\"{0}\" size={1}", fileName, size);
        }

        /// <summary>
        /// Modifies file creation information.  If you wish to specify a new file size, use SetFileSize instead.
        /// </summary>
        /// <param name="fileName">File name.</param>
        /// <param name="info">File information.</param>
        public void SetFileInformation(string fileName, FileInformation info)
        {
            uint createhi = (uint)(info.CreationTime.ToFileTime() >> 32);
            uint createlo = (uint)(info.CreationTime.ToFileTime() & 0xFFFFFFFF);
            uint changehi = (uint)(info.ChangeTime.ToFileTime() >> 32);
            uint changelo = (uint)(info.ChangeTime.ToFileTime() & 0xFFFFFFFF);

            string attr = string.Empty;
            if ((info.Attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                attr += "readonly=1";
            else attr += "readonly=0";
            if ((info.Attributes & FileAttributes.Hidden) == FileAttributes.Hidden)
                attr += "hidden=1";
            else attr += "hidden=0";

            SendCommand("setfileattributes name=\"{0}\" createhi=0x{1} createlo=0x{2} changehi=0x{3} changelo=0x{4} {5}",
                fileName,
                Convert.ToString(createhi, 16), Convert.ToString(createlo, 16),
                Convert.ToString(changehi, 16), Convert.ToString(changelo, 16),
                attr);
        }

        /// <summary>
        /// Retrieves file information.
        /// </summary>
        /// <param name="fileName">File name.</param>
        /// <returns>File information.</returns>
        public FileInformation GetFileInformation(string fileName)
        {
            FileInformation info = new FileInformation();
            info.Name = fileName;
            SendCommand("getfileattributes name=\"{0}\"", fileName);
            string msg = ReceiveMultilineResponse();

            // devs fucked up size output so we need to parse carefully... ;X
            int sizehiend = msg.IndexOf("sizelo") - 1;
            int sizelostart = sizehiend + 10;
            int sizeloend = msg.IndexOf("createhi") - 1;

            info.Size = Convert.ToUInt64(msg.Substring(sizelostart, sizeloend - sizelostart), 16);
            info.Size |= (Convert.ToUInt64(msg.Substring(9, sizehiend - 9), 16) << 32); // should be 0

            ulong createtime;
            createtime = Convert.ToUInt64(msg.Substring(msg.IndexOf("createlo") + 11, 8), 16);
            createtime |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("createhi") + 11, 8), 16) << 32);
            info.CreationTime = DateTime.FromFileTime((long)createtime);

            ulong changetime;
            changetime = Convert.ToUInt64(msg.Substring(msg.IndexOf("changelo") + 11, 8), 16);
            changetime |= (Convert.ToUInt64(msg.Substring(msg.IndexOf("changehi") + 11, 8), 16) << 32);
            info.ChangeTime = DateTime.FromFileTime((long)changetime);

            if (msg.Contains("directory")) info.Attributes |= FileAttributes.Directory;
            else info.Attributes |= FileAttributes.Normal;

            if (msg.Contains("readonly")) info.Attributes |= FileAttributes.ReadOnly;
            if (msg.Contains("hidden")) info.Attributes |= FileAttributes.Hidden;

            return info;
        }

        /// <summary>
        /// Retrieves file attributes.
        /// </summary>
        /// <param name="fileName">File name.</param>
        /// <returns>File attributes.</returns>
        public FileAttributes GetFileAttributes(string fileName)
        {
            return GetFileInformation(fileName).Attributes;
        }

        /// <summary>
        /// Sets file attributes.
        /// </summary>
        /// <param name="fileName">File name.</param>
        /// <returns>File attributes.</returns>
        public void SetFileAttributes(string fileName, FileAttributes attributes)
        {
            if ((attributes & FileAttributes.Normal) != FileAttributes.Normal && (attributes & FileAttributes.ReadOnly) != FileAttributes.ReadOnly && (attributes & FileAttributes.Hidden) != FileAttributes.Hidden)
                throw new UnsupportedException("Unsupported file attribute.");

            // TODO: look into hidden attributes
            FileInformation fi = GetFileInformation(fileName);
            fi.Attributes = attributes;
            SetFileInformation(fileName, fi);
        }

        /// <summary>
        /// Retrieves file size.
        /// </summary>
        /// <param name="fileName">File name.</param>
        /// <returns>File size.</returns>
        public uint GetFileSize(string fileName)
        {
            return (uint)GetFileInformation(fileName).Size;
        }

        /// <summary>
        /// Mounts the specified device to the specified drive letter.
        /// </summary>
        /// <param name="device">Device name</param>
        /// <param name="drive">Drive letter</param>
        public void MountDevice(Drive device, DriveLabel drive)
        {
            string driveName = "\\??\\" + drive.ToString() + ":";
            string deviceName = string.Empty;
            switch (device)
            {
                case Drive.CDRom: deviceName = "\\Device\\CdRom0"; break;
                case Drive.DriveC: deviceName = "\\Device\\Harddisk0\\Partition2"; break;
                case Drive.DriveE: deviceName = "\\Device\\Harddisk0\\Partition1"; break;
                case Drive.DriveF: deviceName = "\\Device\\Harddisk0\\Partition6"; break;
                //case Device.DriveG: deviceName = "\\Device\\Harddisk0\\Partition7"; break;    // seems to be disabled in debug bios
                //case Device.DriveH: deviceName = "\\Device\\Harddisk0\\Partition8"; break;    // seems to be disabled in debug bios
                case Drive.DriveX: deviceName = "\\Device\\Harddisk0\\Partition3"; break;
                case Drive.DriveY: deviceName = "\\Device\\Harddisk0\\Partition4"; break;
                case Drive.DriveZ: deviceName = "\\Device\\Harddisk0\\Partition5"; break;
            }

            // send mounting info to xbox
            SetMemory(History.ScratchBuffer, (ushort)driveName.Length, (ushort)(driveName.Length + 1),
                (uint)(History.ScratchBuffer + 16), (ushort)deviceName.Length, (ushort)(deviceName.Length + 1),
                (uint)(History.ScratchBuffer + 16 + driveName.Length + 1), driveName, deviceName);

            // attempt to mount device
            uint error = CallAddressEx(Kernel.IoCreateSymbolicLink, null, true, History.ScratchBuffer, History.ScratchBuffer + 8);
            if (error != 0) throw new ApiException("Failed to mount the device");
        }

        /// <summary>
        /// Unmounts the specified drive.
        /// </summary>
        /// <param name="drive">Drive letter.</param>
        public void UnMountDevice(DriveLabel drive)
        {
            string driveName = "\\??\\" + drive.ToString() + ":";

            // send unmounting info to xbox
            SetMemory(History.ScratchBuffer, (ushort)driveName.Length, (ushort)(driveName.Length + 1),
                (uint)(History.ScratchBuffer + 8), driveName);

            // attempt to unmount device
            uint error = CallAddressEx(Kernel.IoDeleteSymbolicLink, null, true, History.ScratchBuffer);
            if (error != 0) throw new ApiException("Failed to unmount the device");
        }
    };
}