/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.TagInterface
{
	public static class Exceptions
	{
		public sealed class InvalidTagHeader : Debug.ExceptionLog
		{
			public InvalidTagHeader(string file, string expected, string got)
			{
				Debug.LogFile.WriteLine(
					"'{0}' has a bad header: '{1}', expected '{2}'",
					file, got, expected);
			}
		};

		public sealed class InvalidVersion : Debug.ExceptionLog
		{
			public InvalidVersion(IO.ITagStream tag)
			{
				Debug.LogFile.WriteLine("Invalid versioning" + Program.NewLine +
					"\tIn {0}",
					tag.GetExceptionDescription());
			}
			public InvalidVersion(IO.ITagStream tag, string msg)
			{
				Debug.LogFile.WriteLine("Invalid versioning ({0})" + Program.NewLine +
					"\tIn {1}",
					msg,
					tag.GetExceptionDescription());
			}
			public InvalidVersion(IO.ITagStream tag, int expected, int got) : base()
			{
				Debug.LogFile.WriteLine("Invalid versioning: expected {0} but got {1}" + Program.NewLine +
					"\tIn {2}",
					expected.ToString(), got.ToString(),
					tag.GetExceptionDescription());
			}
			public InvalidVersion(IO.ITagStream tag, int expected, int expected_size, int got, int got_size) : base()
			{
				Debug.LogFile.WriteLine("Invalid versioning: expected {0} (0x{1}) but got {2} (0x{3})" + Program.NewLine +
					"\tIn {4}",
					expected.ToString(), expected_size.ToString("X4"), got.ToString(), got_size.ToString("X4"),
					tag.GetExceptionDescription());
			}
		};

		public abstract class InvalidField : Debug.ExceptionLog
		{
			protected InvalidField(Exception inner, string msg) : base(inner, msg) {}
		};

		public sealed class InvalidStringId : InvalidField
		{
			/// <summary>
			/// Raise an exception for an invalid <c>string_id</c> field in a stream
			/// </summary>
			/// <param name="inner"></param>
			/// <param name="offset_field">Offset of the <c>string_id</c> field</param>
			/// <param name="offset_data">Offset of the <c>string_id</c> data</param>
			/// <param name="tag">Tag which this <c>string_id</c> ultimately belongs to</param>
			/// <param name="length">Expected length of the <c>string_id</c></param>
			/// <param name="value">String read from the stream as the <c>string_id</c>'s value. Can be null.</param>
			public InvalidStringId(
				Exception inner, 
				uint offset_field, uint offset_data, 
				IO.ITagStream tag,
				int length, string value) : base(inner, "Invalid String Id")
			{
				Debug.LogFile.WriteLine(
					"Invalid string_id: field@{0:X8}\tdata@{1:X8}\tlength:{2:X2}\tvalue:{3}" + Program.NewLine +
					"\tIn {4}",
					offset_field, offset_data, 
					length, value != null ? value : "UNKNOWN",
					tag.GetExceptionDescription());
			}
		};

		public sealed class InvalidTagReference : InvalidField
		{
			/// <summary>
			/// Raise an exception for an invalid <c>tag_reference</c> field in a stream
			/// </summary>
			/// <param name="inner"></param>
			/// <param name="offset_field">Offset of the <c>tag_reference</c> field</param>
			/// <param name="offset_data">Offset of the <c>tag_reference</c> data (ie, tag name)</param>
			/// <param name="owner">Owner of the <see cref="TagReference"/> field. Can be null.</param>
			/// <param name="tag">Tag which this <c>tag_reference</c> ultimately belongs to</param>
			/// <param name="group_tag"><c>tag_reference</c>'s group tag</param>
			/// <param name="value"><c>tag_reference</c>'s tag name data. Can be null.</param>
			public InvalidTagReference(
				Exception inner, 
				uint offset_field, uint offset_data, 
				IStructureOwner owner, IO.ITagStream tag, 
				uint group_tag, string value) : base(inner, "Invalid Tag Reference")
			{
				Debug.LogFile.WriteLine(
					"Invalid tag_reference: field@{0:X8}\tdata@{1:X8}\tgroup={2,4}\tpath={3}" + Program.NewLine +
					"\tIn {4} in {5}",
					offset_field, offset_data, 
					group_tag, value != null ? value : "UNKNOWN",
					tag.GetExceptionDescription(), 
					owner != null ? owner.GetType().FullName : "UNKNOWN");
			}
		};

		public sealed class InvalidTagBlock : InvalidField
		{
			/// <summary>
			/// Raise an exception for an invalid <c>tag_block</c> field in a stream
			/// </summary>
			/// <param name="inner"></param>
			/// <param name="offset_field">Offset of the <c>tag_block</c> field</param>
			/// <param name="offset_data">Offset of the <c>tag_block</c> data</param>
			/// <param name="owner">Owner of the <see cref="Block{T}"/> field. Can be null.</param>
			/// <param name="tag">Tag which this <c>tag_block</c> ultimately belongs to</param>
			public InvalidTagBlock(
				Exception inner, 
				uint offset_field, uint offset_data, 
				IStructureOwner owner, IO.ITagStream tag
				) : base(inner, "Invalid Tag Block")
			{
				Debug.LogFile.WriteLine(
					"Invalid tag_block: field@{0:X8}\tdata@{1:X8}" + Program.NewLine +
					"\tIn {2} in {3}",
					offset_field, offset_data,
					tag.GetExceptionDescription(),
					owner != null ? owner.GetType().FullName : "UNKNOWN");
			}
		};

		public sealed class InvalidTagData : InvalidField
		{
			/// <summary>
			/// Raise an exception for an invalid <c>tag_data</c> field in a stream
			/// </summary>
			/// <param name="inner"></param>
			/// <param name="offset_field">Offset of the <c>tag_data</c> field</param>
			/// <param name="offset_data">Offset of the <c>tag_data</c> data</param>
			/// <param name="owner">Owner of the <see cref="TagInterface.Data"/> field. Can be null.</param>
			/// <param name="tag">Tag which this <c>tag_data</c> ultimately belongs to</param>
			public InvalidTagData(
				Exception inner, 
				uint offset_field, uint offset_data,
				IStructureOwner owner, IO.ITagStream tag
				) : base(inner, "Invalid Tag Data")
			{
				Debug.LogFile.WriteLine(
					"Invalid tag_data: field@{0:X8}\tdata@{1:X8}" + Program.NewLine +
					"\tIn {2} in {3}",
					offset_field, offset_data,
					tag.GetExceptionDescription(),
					owner != null ? owner.GetType().FullName : "UNKNOWN");
			}
		};

		public sealed class InvalidTagStruct : InvalidField
		{
			/// <summary>
			/// Raise an exception for an invalid inline struct field in a stream
			/// </summary>
			/// <param name="inner"></param>
			/// <param name="offset_data">Offset of the inline struct data</param>
			/// <param name="owner">Owner of the <see cref="Struct{T}"/> field. Can be null.</param>
			/// <param name="tag">Tag which this inline struct ultimately belongs to</param>
			public InvalidTagStruct(
				Exception inner, 
				uint offset_data,
				IStructureOwner owner, IO.ITagStream tag
				) : base(inner, "Invalid Tag Struct")
			{
				Debug.LogFile.WriteLine(
					"Invalid inline struct: data@{0:X8}" + Program.NewLine +
					"\tIn {1} in {2}",
					offset_data,
					tag.GetExceptionDescription(),
					owner != null ? owner.GetType().FullName : "UNKNOWN");
			}
		};

		public sealed class InvalidStructReference : InvalidField
		{
			/// <summary>
			/// Raise an exception for an invalid data reference field in a stream
			/// </summary>
			/// <param name="inner"></param>
			/// <param name="offset_field">Offset of the data reference field</param>
			/// <param name="offset_data">Offset of the data reference data</param>
			/// <param name="owner">Owner of the <see cref="StructReference{T}"/> field. Can be null.</param>
			/// <param name="tag">Tag which this data reference ultimately belongs to</param>
			public InvalidStructReference(
				Exception inner, 
				uint offset_field, uint offset_data, 
				IStructureOwner owner, IO.ITagStream tag
				) : base(inner, "Invalid Data Reference")
			{
				Debug.LogFile.WriteLine(
					"Invalid data reference: field@{0:X8}\tdata@{1:X8}" + Program.NewLine +
					"\tIn {2} in {3}",
					offset_field, offset_data,
					tag.GetExceptionDescription(),
					owner != null ? owner.GetType().FullName : "UNKNOWN");
			}
		};
	};
}