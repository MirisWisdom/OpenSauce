/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Microsoft.Win32;

namespace BlamLib
{
	/// <summary>
	/// WinAPI utility class
	/// </summary>
	public static class Windows
	{
		public static readonly string WinRarExePath;
		/// <summary>
		/// Does Rar.exe exist on this system?
		/// </summary>
		public static bool HasWinRar { get { return WinRarExePath != null; } }
		static Windows()
		{
			// BUG: If we're running an x86 build on a x64 platform, and the system doesn't have 
			// WinRar x86 installed, this will always fail. This is because the .NET registry classes 
			// won't allow access to the x64 registry, we'd have to use P\Invoke :|
			var subkey = Registry.LocalMachine.OpenSubKey(@"Software\WinRAR", 
				RegistryKeyPermissionCheck.Default,System.Security.AccessControl.RegistryRights.QueryValues);
			if (subkey == null) return;

			var key = subkey.GetValue("exe64") as string; // use 64-bit by default
			if(key == null) // Either the user is running a 32-bit OS or somehow just installed the 32-bit version
				key = subkey.GetValue("exe32") as string;

			WinRarExePath = null;
			if (!string.IsNullOrEmpty(key))
			{
				key = System.IO.Path.GetDirectoryName(key);
				key = System.IO.Path.Combine(key, "Rar.exe");

				if (System.IO.File.Exists(key))
					WinRarExePath = key;
			}
			subkey.Close();
		}


		#region Functions
#if !_WIN64
		[return: MarshalAs(UnmanagedType.Bool)] 
		[DllImport("Winmm.dll")]
		public extern static
		bool PlaySound(string szSound, IntPtr hMod, int flags);
#endif

		[return: MarshalAs(UnmanagedType.Bool)] 
		[DllImport("kernel32.dll", SetLastError=true)] public extern static
		bool WriteProcessMemory(
			IntPtr hProcess,
			IntPtr lpBaseAddress,
			[In, Out] byte[] buffer,
#if _WIN64
			UInt64 size,
			out UInt64 lpNumberOfBytesWritten
#else
			UInt32 size,
			out UInt32 lpNumberOfBytesWritten
#endif
			);

		[return: MarshalAs(UnmanagedType.Bool)] 
		[DllImport("kernel32.dll", SetLastError=true)] public extern static
		bool ReadProcessMemory(
			IntPtr hProcess,
			IntPtr lpBaseAddress,
			[In, Out] byte[] buffer,
#if _WIN64
			UInt64 size,
			out UInt64 lpNumberOfBytesRead
#else
			UInt32 size,
			out UInt32 lpNumberOfBytesRead
#endif
			);

		/// <summary>
		/// Closes an open object handle
		/// </summary>
		/// <param name="hObject"></param>
		/// <returns></returns>
		[return: MarshalAs(UnmanagedType.Bool)] 
		[DllImport("kernel32.dll", SetLastError=true)] public extern static
		bool CloseHandle(IntPtr hObject);

		/// <summary>
		/// Opens an existing process object
		/// </summary>
		/// <param name="dwDesiredAccess">
		/// Access to the process object. This access right is checked against any security 
		/// descriptor for the process. This parameter can be one or more of the process 
		/// access rights
		/// </param>
		/// <param name="bInheritHandle">
		/// If this parameter is TRUE, the handle is inheritable. If the parameter is FALSE, 
		/// the handle cannot be inherited
		/// </param>
		/// <param name="dwProcessId">Identifier of the process to open</param>
		/// <returns></returns>
		[DllImport("kernel32.dll", SetLastError=true)] public extern static
		IntPtr OpenProcess(uint dwDesiredAccess, [MarshalAs(UnmanagedType.Bool)] bool bInheritHandle, uint dwProcessId);

		/// <summary>
		/// Makes a beeping sound.
		/// </summary>
		/// <param name="mbi">
		/// The type of sound to make, based on the situation.
		/// </param>
		[return: MarshalAs(UnmanagedType.Bool)] 
		[DllImport("user32.dll")] public extern static
		bool MessageBeep(MessageBoxIcon mbi);

		[DllImport("kernel32.dll", EntryPoint="LoadLibraryW", CharSet=CharSet.Unicode)] public extern static
		IntPtr LoadLibraryW(string dll);
		[return: MarshalAs(UnmanagedType.Bool)]
		[DllImport("kernel32.dll", EntryPoint="FreeLibrary")] public extern static
		bool FreeLibrary(IntPtr dll);

		[DllImport("user32.dll", EntryPoint="LoadStringW", CharSet=CharSet.Unicode)] extern static
		int LoadStringW(IntPtr hInstance, uint id, [In, Out] System.Text.StringBuilder path, int length);
		public static string LoadString(IntPtr hInstance, uint id)
		{
			System.Text.StringBuilder sb = new System.Text.StringBuilder(4096);
			LoadStringW(hInstance, id, sb, sb.Capacity);
			return sb.ToString();
		}

		/// <summary>
		/// Truncates a file path to fit within a given pixel width by 
		/// replacing path components with ellipses
		/// </summary>
		/// <param name="hDC">Handle to the device context used for font metrics</param>
		/// <param name="path">
		/// Pointer to a null-terminated string of length MAX_PATH that contains the path to be 
		/// modified. On return, this buffer will contain the modified string
		/// </param>
		/// <param name="dx"></param>
		/// <returns></returns>
		[return: MarshalAs(UnmanagedType.Bool)] 
		[DllImport("shlwapi.dll", EntryPoint = "PathCompactPathW")] extern static
		bool PathCompactPath(IntPtr hDC, [MarshalAs(UnmanagedType.LPWStr), In, Out] System.Text.StringBuilder path, uint dx);
		/// <summary>
		/// Truncates a file path to fit within a given pixel width by 
		/// replacing path components with ellipses
		/// </summary>
		/// <param name="handle">Handle to the device context used for font metrics</param>
		/// <param name="path">Path to truncate</param>
		/// <param name="dx">Width, in pixels, within which the string will be forced to fit</param>
		/// <returns></returns>
		public static string PathCompact(IntPtr handle, string path, uint dx)
		{
			System.Text.StringBuilder sb = new System.Text.StringBuilder(kMaxPath);
			sb.Append(path);
			PathCompactPath(handle, sb, dx);
			return sb.ToString();
		}
		#endregion

		#region Constants
		const int kMaxPath = 260;
		#endregion
	};
}