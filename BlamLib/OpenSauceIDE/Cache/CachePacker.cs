/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;
using System.Windows.Forms;

namespace OpenSauceIDE.Cache
{
	public partial class CachePacker : Form
	{
		public CachePacker()
		{
			InitializeComponent();
		}

		void OnPathSource(object sender, EventArgs e)
		{
			if (dlgOpenFile.ShowDialog(this) == DialogResult.OK)
				txtPathSource.Text = dlgOpenFile.FileName;
		}

		void OnPathOutput(object sender, EventArgs e)
		{
			if (dlgSaveFile.ShowDialog(this) == DialogResult.OK)
				txtPathOutput.Text = dlgSaveFile.FileName;
		}

		void OnPackageException(Exception ex)
		{
			BlamLib.Debug.LogFile.WriteLine("CachePacker Apply failed in. Reason:{0}{1}", BlamLib.Program.NewLine, ex);
		}
		void OnPackageMsg(bool is_error, string format, params string[] args)
		{
			string msg = string.Format(format, args);
			string title = is_error ? "Error" : "Success!";

			MessageBox.Show(this, msg, title, MessageBoxButtons.OK,
				is_error ? MessageBoxIcon.Error : MessageBoxIcon.Information);
		}
		void OnPack()
		{
		}
		void OnUnpack()
		{
		}
		void OnPackage(object sender, EventArgs e)
		{
			if(string.IsNullOrEmpty(txtPathSource.Text))
				OnPackageMsg(true, "Select source file first!");
			else if (sender == btPack) OnPack();
			else if (sender == btUnpack) OnUnpack();
		}
	};
}

namespace BlamLib
{
	partial class Tool
	{
		static void PackCacheHalo1Xbox(string input_file, string output_file)
		{
		}
		static void UnPackCacheHalo1Xbox(string input_file, string output_file)
		{
		}

		static void PackCacheHA10(string input_file, string output_file)
		{
			// TODO
		}
		static void UnPackCacheHA10(string input_file, string output_file)
		{
			using (var fs_in = File.OpenRead(input_file))
			using (var s = new BinaryReader(fs_in))
			using (var fs_out = File.OpenWrite(output_file))
			{
				int count = s.ReadInt32();
				int[] offsets = new int[count];
				for (int x = 0; x < offsets.Length; x++)
					offsets[x] = s.ReadInt32();

				// TODO: this can easily be multithreaded
				byte[] dest = new byte[0x8000]; // The largest uncompressed block is 0x8000
				byte[] src = new byte[0x20000]; // window size is 1<<17
				for (int x = 0; x < offsets.Length; x++)
				{
					IntPtr context = IO.XCompress.CreateDecompressionContext();

					int offset = offsets[x];
					int size;
					if (x == offsets.Length - 1)
						size = (int)(fs_in.Length - offset);
					else
						size = offsets[x + 1] - offset;

					fs_in.Seek(offset, SeekOrigin.Begin);
					fs_in.Read(src, 0, size);

					int dest_size = dest.Length;
					uint result = IO.XCompress.Decompress(context, dest, ref dest_size, src, ref size);
					// TODO: verify result == 0 and warn or error when it non-zero
					fs_out.Write(dest, 0, dest_size);

					IO.XCompress.DestroyDecompressionContext(context);
				}
			}
		}
		static ExitCode PackCache(params string[] args)
		{
			// <version> <mode> <input-file> <output-file>
			if (args.Length < 4)
			{
				Console.WriteLine("error: invalid command argument count");
				return ExitCode.InvalidArgsCount;
			}

			// TODO

			return ExitCode.Success;
		}
	};
}