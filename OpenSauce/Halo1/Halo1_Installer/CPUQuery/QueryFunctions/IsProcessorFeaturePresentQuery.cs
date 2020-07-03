/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Runtime.InteropServices;

namespace CPUQuery.QueryFunctions
{
	public static class IsProcessorFeaturePresentQuery
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Queries whether a particular processor feature is present. </summary>
		///
		/// <param name="processorFeature">	The processor feature. </param>
		///
		/// <returns>	true if the processor feature is present, false if not. </returns>
		[DllImport("kernel32.dll")]
		[return: MarshalAs(UnmanagedType.Bool)]
		static extern bool IsProcessorFeaturePresent(ProcessorFeature processorFeature);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent processor features sets. </summary>
		public enum ProcessorFeature : uint
		{
			/// <summary>
			/// On a Pentium, a floating-point precision error can occur in rare circumstances
			/// </summary>
			FloatingPointPrecisionErrata = 0,
			/// <summary>
			/// Floating-point operations are emulated using a software emulator. 
			/// This function returns a nonzero value if floating-point operations are emulated; otherwise, it returns zero.
			/// </summary>
			FloatingPointEmulated = 1,
			/// <summary>
			/// The atomic compare and exchange operation (cmpxchg) is available
			/// </summary>
			CompareExchangeDouble = 2,
			/// <summary>
			/// The MMX instruction set is available
			/// </summary>
			InstructionsMMXAvailable = 3,
			/// <summary>
			/// The SSE instruction set is available
			/// </summary>
			InstructionsXMMIAvailable = 6,
			/// <summary>
			/// The 3D-Now instruction set is available.
			/// </summary>
			Instruction3DNowAvailable = 7,
			/// <summary>
			/// The RDTSC instruction is available
			/// </summary>
			InstructionRDTSCAvailable = 8,
			/// <summary>
			/// The processor is PAE-enabled
			/// </summary>
			PAEEnabled = 9,
			/// <summary>
			/// The SSE2 instruction set is available
			/// </summary>
			InstructionsXMMI64Available = 10,
			/// <summary>
			/// Data execution prevention is enabled. (This feature is not supported until Windows XP SP2 and Windows Server 2003 SP1)
			/// </summary>
			NXEnabled = 12,
			/// <summary>
			/// The SSE3 instruction set is available. (This feature is not supported until Windows Vista)
			/// </summary>
			InstructionsSSE3Available = 13,
			/// <summary>
			/// The atomic compare and exchange 128-bit operation (cmpxchg16b) is available. (This feature is not supported until Windows Vista)
			/// </summary>
			CompareExchange128 = 14,
			/// <summary>
			/// The atomic compare 64 and exchange 128-bit operation (cmp8xchg16) is available (This feature is not supported until Windows Vista.)
			/// </summary>
			Compare64Exchange128 = 15,
			/// <summary>
			/// TBD
			/// </summary>
			ChannelsEnabled = 16,
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Queries whether the user's CPU supports a particular feature. </summary>
		///
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="queryArgs">	The query arguments. </param>
		///
		/// <returns>	A string boolean. </returns>
		public static string Execute(string queryArgs)
		{
			ProcessorFeature processorFeature = (ProcessorFeature)Enum.Parse(typeof(ProcessorFeature), queryArgs, true);

			return IsProcessorFeaturePresent(processorFeature).ToString();
		}
	}
}
