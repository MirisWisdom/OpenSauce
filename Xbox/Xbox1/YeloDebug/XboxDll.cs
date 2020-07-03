/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using System.Runtime.InteropServices;

namespace YeloDebug
{
    /// <summary>
    /// Kornman H4x - helper functions used to inject dlls into an xbox process.
    /// </summary>
    public class XboxDll
    {
		public static string GetResultString(LowLevel.HResult r)
		{
			switch (r)
			{
				case LowLevel.HResult.Success: return "No Error";

				case LowLevel.HResult.Abort: return "Invalid Module Data";
				case LowLevel.HResult.Fail: return "Invalid Input(s)";

				default: return "Unexpected Error: " + r.ToString();
			}
		}

		[DllImport("UtilDll.dll", EntryPoint = "Util_RebaseModule", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		[return: MarshalAs(UnmanagedType.U4)]
        extern static LowLevel.HResult RebaseModule(
			[MarshalAs(UnmanagedType.LPWStr)] string path,
            [In, Out, MarshalAs(UnmanagedType.LPArray)] byte[] rebased_dll,
			uint rebased_dll_size,
            ref uint base_address,
			uint reloc_alloc_count);

		[DllImport("UtilDll.dll", EntryPoint = "Util_CalculateModuleCodeSize", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		[return: MarshalAs(UnmanagedType.U4)]
        public extern static LowLevel.HResult CalculateModuleCodeSize(
			[MarshalAs(UnmanagedType.LPWStr)] string path,
            out uint code_size);

		static bool ValidateBaseAddress(Xbox xbox, uint base_address)
		{
			// 0x20000 to avoid someone overwriting the header
			return base_address >= 0x20000 && base_address < uint.MaxValue && 
				xbox.IsValidAddress(base_address);
		}

		static LowLevel.HResult RebaseModule(Xbox xbox, string path, byte[] rebased_dll, ref uint base_address, uint reloc_alloc_count)
		{
			if (xbox != null && !ValidateBaseAddress(xbox, base_address))
				throw new ArgumentOutOfRangeException("base_address", "Specific base address is out of range");

			return RebaseModule(path, rebased_dll, (uint)rebased_dll.Length, ref base_address, reloc_alloc_count);
		}
		static LowLevel.HResult RebaseModule(Xbox xbox, string path, byte[] rebased_dll, ref uint base_address)
		{
			return RebaseModule(xbox, path, rebased_dll, ref base_address, 0);
		}

		static LowLevel.HResult RebasedModuleBufferAllocate(string module_path, out byte[] rebased_module, out string error_details)
		{
			rebased_module = null; error_details = null;
			LowLevel.HResult result;

			uint rebased_module_size = 0;
			try { result = CalculateModuleCodeSize(module_path, out rebased_module_size); }
			catch (Exception ex)
			{
				result = LowLevel.HResult.Unexpected;
				error_details = ex.ToString();
			}

			if (result == LowLevel.HResult.Success)
				rebased_module = new byte[rebased_module_size];

			return result;
		}

		public static bool RebaseModuleAndSave(string module_path, uint base_address, out string error_details)
		{
			error_details = null;
			LowLevel.HResult result;

			if (!File.Exists(module_path))
			{
				error_details = "File does not exist!";
				return false;
			}

			byte[] rebased_module;
			result = RebasedModuleBufferAllocate(module_path, out rebased_module, out error_details);

			if (result == LowLevel.HResult.Success)
			{
				try { result = RebaseModule(null, module_path, rebased_module, ref base_address); }
				catch (Exception ex)
				{
					result = LowLevel.HResult.Unexpected;
					error_details = ex.ToString();
				}

				if (result == LowLevel.HResult.Success)
				{
					using (var fs = File.Create(module_path + ".rebased"))
						fs.Write(rebased_module, 0, rebased_module.Length);
				}
			}

			if (error_details == null)
				error_details = GetResultString(result);

			return result == LowLevel.HResult.Success;
		}

		const uint k_breakpoint_address = 0x12B6B5;
		static void SetBreakpointOn(Xbox xbox)
		{
			xbox.SetBreakPoint(k_breakpoint_address);
			System.Threading.Thread.Sleep(25);
		}
		static void SetBreakpointOff(Xbox xbox)
		{
			xbox.RemoveBreakPoint(k_breakpoint_address);
			xbox.Continue();
		}

		public static bool RunModule(Xbox xbox, string module_path, ref uint base_address, 
			out uint entry_point, out uint exit_address, out string error_details)
		{
			entry_point = exit_address = uint.MaxValue;
			error_details = null;
			LowLevel.HResult result;

			if (!File.Exists(module_path))
			{
				error_details = "File does not exist!";
				return false;
			}

			byte[] rebased_module;
			result = RebasedModuleBufferAllocate(module_path, out rebased_module, out error_details);

			if (result == LowLevel.HResult.Success)
			{
				try { base_address = xbox.AllocateMemory((uint)rebased_module.Length); }
				catch (Exception ex)
				{
					result = LowLevel.HResult.Unexpected;
					error_details = ex.ToString();
				}

				entry_point = base_address;
				try { result = RebaseModule(xbox, module_path, rebased_module, ref entry_point); }
				catch (Exception ex)
				{
					result = LowLevel.HResult.Unexpected;
					error_details = ex.ToString();
				}
				entry_point += base_address;

				if (result == LowLevel.HResult.Success)
				{
					xbox.Pause();
					xbox.SetMemory(base_address, rebased_module);
					exit_address = xbox.CallAddress(entry_point, true);
					xbox.Continue();
				}
			}

			if(error_details == null)
				error_details = GetResultString(result);

			return result == LowLevel.HResult.Success;
		}

		public static bool UnloadModule(Xbox xbox, uint base_address, uint exit_address)
		{
			SetBreakpointOn(xbox);//xbox.Pause();
			xbox.CallAddress(exit_address, false);
			xbox.FreeDebugMemory(base_address);
			SetBreakpointOff(xbox);//xbox.Continue();

			return true;
		}
    };
}