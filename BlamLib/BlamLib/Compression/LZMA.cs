using System;
using System.IO;

namespace BlamLib.Compression
{
	static public class LZMA
	{
		public class CompressionProgressData
		{
			public uint CompletionPercentage { get; private set; }
			public uint UncompressedSize { get; private set; }
			public uint ProcessedSize { get; private set; }
			public uint CompressedSize { get; private set; }
			public float CompressionRatio { get; private set; }

			CompressionProgressData() { }
			public CompressionProgressData(uint uncompressed_size,
				uint processed_size,
				uint compressed_size)
			{
				CompletionPercentage = (uint)Math.Round((100.0f / uncompressed_size) * processed_size);
				UncompressedSize = uncompressed_size;
				ProcessedSize = processed_size;
				CompressedSize = compressed_size;
				CompressionRatio = (uint)Math.Round(((float)compressed_size / (float)processed_size) * 100);
			}
		};

		public delegate void CompressProgressCallback(CompressionProgressData progress_data);

		public enum LZMADictionarySize
		{
			LZMADictionarySize_64KB		= 65536,
			LZMADictionarySize_1MB		= 1048576,
			LZMADictionarySize_2MB		= LZMADictionarySize_1MB * 2,
			LZMADictionarySize_3MB		= LZMADictionarySize_1MB * 3,
			LZMADictionarySize_4MB		= LZMADictionarySize_1MB * 4,
			LZMADictionarySize_6MB		= LZMADictionarySize_1MB * 6,
			LZMADictionarySize_8MB		= LZMADictionarySize_1MB * 8,
			LZMADictionarySize_12MB		= LZMADictionarySize_1MB * 12,
			LZMADictionarySize_16MB		= LZMADictionarySize_1MB * 16,
			LZMADictionarySize_24MB		= LZMADictionarySize_1MB * 24,
			LZMADictionarySize_32MB		= LZMADictionarySize_1MB * 32,
			LZMADictionarySize_48MB		= LZMADictionarySize_1MB * 48,
			LZMADictionarySize_64MB		= LZMADictionarySize_1MB * 64,
		};

		public static bool Compress(Stream stream_in,
			Stream stream_out,
			LZMADictionarySize dictionary_size,
			uint level,
			CompressProgressCallback progress_callback)
		{
			if (level > 9)
				level = 9;

			LowLevel.Compression.LZMA.CompressionCallback compression_callback = (uint processed_data, uint compressed_size) =>
				{
					if (progress_callback != null)
						progress_callback(new CompressionProgressData((uint)stream_in.Length, processed_data, compressed_size));
				};

			return LowLevel.Compression.LZMA.Compress(LowLevel.Compression.LZMA.LZMAStandardLayout,
				stream_in,
				stream_out,
				(uint)dictionary_size,
				level,
				compression_callback);
		}

		public static bool Decompress(Stream stream_in, Stream stream_out)
		{
			byte[] source = new byte[stream_in.Length];
			byte[] result;

			// TODO: don't like this, doubles memory usage due to copying
			stream_in.Seek(0, SeekOrigin.Begin);
			stream_in.Read(source, 0, (int)stream_in.Length);

			bool success = LowLevel.Compression.LZMA.Decompress(LowLevel.Compression.LZMA.LZMAStandardLayout, source, out result);

			if (success)
			{
				stream_out.Seek(0, SeekOrigin.Begin);
				stream_out.Write(result, 0, result.Length);
				stream_out.Flush();
			}
			return success;
		}
	}
}
