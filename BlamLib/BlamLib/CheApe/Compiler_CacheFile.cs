/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.CheApe
{
	partial class Compiler
	{
		public sealed class CacheFileHeader : IO.IStreamable
		{
			#region Constants
			const int kVersion = 2;
			const int kSizeOf =
				4 + // head
				4 + // Version
				4 + // EngineSignature;
				4 + // pad32;
				4 + // BaseAddress
				4 + // DataOffset
				4 + // StringPoolSize
				4 + // StringPoolAddress

				4 + // ScriptFunctionsCount
				4 + // ScriptFunctionsAddress
				4 + // ScriptGlobalsCount
				4 + // ScriptGlobalsAddress

				4 + // FixupCount
				4 + // FixupAddress
				4 + // ExportsAddress
				4 + // pad32;

				4; // tail
			public const int kMaxSize = 2048;
			const int kPadSize = CacheFileHeader.kMaxSize - CacheFileHeader.kSizeOf;
			public static readonly byte[] Padding = new byte[CacheFileHeader.kPadSize];

			static readonly TagInterface.TagGroup kHalo1Signature = new TagInterface.TagGroup("blm1", BlamVersion.Halo1_CE.ToString());
			static readonly TagInterface.TagGroup kHalo2Signature = new TagInterface.TagGroup("blm2", BlamVersion.Halo2_PC.ToString());
			#endregion

			TagInterface.TagGroup engineSignature;

			#region BaseAddress
			uint baseAddress;
			/// <summary>
			/// 
			/// </summary>
			public uint BaseAddress
			{
				get { return baseAddress; }
				set { baseAddress = value; }
			}
			#endregion

			#region DataOffset
			int dataOffset;
			/// <summary>
			/// 
			/// </summary>
			public int DataOffset
			{
				get { return dataOffset; }
				set { dataOffset = value; }
			}
			#endregion

			#region StringPoolSize
			int stringPoolSize;
			/// <summary>
			/// 
			/// </summary>
			public int StringPoolSize
			{
				get { return stringPoolSize; }
				set { stringPoolSize = value; }
			}
			#endregion

			#region StringPoolAddress
			uint stringPoolAddress;
			/// <summary>
			/// 
			/// </summary>
			public uint StringPoolAddress
			{
				get { return stringPoolAddress; }
				set { stringPoolAddress = value; }
			}
			#endregion


			#region ScriptFunctionsCount
			int scriptFunctionsCount = 0;
			/// <summary>
			/// 
			/// </summary>
			public int ScriptFunctionsCount
			{
				get { return scriptFunctionsCount; }
				set { scriptFunctionsCount = value; }
			}
			#endregion

			#region ScriptFunctionsAddress
			uint scriptFunctionsAddress = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint ScriptFunctionsAddress
			{
				get { return scriptFunctionsAddress; }
				set { scriptFunctionsAddress = value; }
			}
			#endregion

			#region ScriptGlobalsCount
			int scriptGlobalsCount = 0;
			/// <summary>
			/// 
			/// </summary>
			public int ScriptGlobalsCount
			{
				get { return scriptGlobalsCount; }
				set { scriptGlobalsCount = value; }
			}
			#endregion

			#region ScriptGlobalsAddress
			uint scriptGlobalsAddress = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint ScriptGlobalsAddress
			{
				get { return scriptGlobalsAddress; }
				set { scriptGlobalsAddress = value; }
			}
			#endregion


			#region FixupCount
			int fixupCount = 0;
			/// <summary>
			/// 
			/// </summary>
			public int FixupCount
			{
				get { return fixupCount; }
				set { fixupCount = value; }
			}
			#endregion

			#region FixupAddress
			uint fixupAddress = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint FixupAddress
			{
				get { return fixupAddress; }
				set { fixupAddress = value; }
			}
			#endregion

			public uint ExportsAddress { get; set; }

			internal CacheFileHeader(CacheFileHeader old_state)
			{
				if (old_state == null) throw new ArgumentNullException("old_state");

				this.engineSignature = old_state.engineSignature;
			}
			internal CacheFileHeader(BlamVersion engine)
			{
				switch (engine.ToBuild())
				{
					case BlamBuild.Halo1: engineSignature = kHalo1Signature; break;
					case BlamBuild.Halo2: engineSignature = kHalo2Signature; break;
				}
			}

			#region IStreamable Members
			public void Read(IO.EndianReader stream) { throw new NotSupportedException(); }

			public void Write(IO.EndianWriter stream)
			{
				Blam.MiscGroups.head.Write(stream);
				stream.Write(kVersion);
				engineSignature.Write(stream);
				stream.Write(uint.MinValue);
				stream.Write(baseAddress);
				stream.Write(dataOffset);
				stream.Write(stringPoolSize);
				stream.Write(stringPoolAddress);

				stream.Write(scriptFunctionsCount);
				stream.Write(scriptFunctionsAddress);
				stream.Write(scriptGlobalsCount);
				stream.Write(scriptGlobalsAddress);

				stream.Write(fixupCount);
				stream.Write(fixupAddress);
				stream.Write(ExportsAddress);
				stream.Write(uint.MinValue);

				stream.Write(Padding);
				Blam.MiscGroups.tail.Write(stream);
			}
			#endregion
		};
	};
}