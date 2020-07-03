/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.IO
{
	/// <summary>
	/// Flags for <see cref="ITagStream.Flags"/>
	/// </summary>
	public static class ITagStreamFlags
	{
		#region Halo 2 related
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that says this is a halo 2
		/// tag which uses the old file format, with old fieldset headers
		/// </summary>
		public const uint Halo2OldFormat_Fieldset = 1 << 0;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that says this is a halo 2
		/// tag which uses the old file format, with old string id storage
		/// </summary>
		public const uint Halo2OldFormat_StringId = 1 << 1;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that says this is a halo 2
		/// tag which uses the old file format, with useless padding
		/// </summary>
		public const uint Halo2OldFormat_UselessPadding = 1 << 2;
		#endregion

		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that says we should
		/// load dependent tags after we load this tag
		/// (assuming we have a owner)
		/// </summary>
		public const uint LoadDependents = 1 << 3;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that says this tag 
		/// resides in a cache file
		/// </summary>
		public const uint ResidesInCacheFile = 1 << 4;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we've read the definition
		/// from a stream, but due to versioning info, had to upgrade the definition object, thus
		/// if someone did a call to <see cref="Managers.TagManager.Manage(TagInterface.Definition)"/>, their object will be out of sync
		/// </summary>
		public const uint DefinitionWasUpgraded = 1 << 5;

		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we want 
		/// <see cref="TagInterface.FieldWithPointers"/> fields and the like to 
		/// stream their "pointer" data's position to the header, and to use 
		/// that position data when streaming from a source
		/// </summary>
		public const uint UseStreamPositions = 1 << 6;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we don't want 
		/// fields with strings (ie, tag reference or string id) to stream their 
		/// string values
		/// </summary>
		public const uint DontStreamStringData = 1 << 7;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we don't want 
		/// a fieldset header being written even when the specified engine is said 
		/// to support them
		/// </summary>
		public const uint DontStreamFieldSetHeader = 1 << 8;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we don't want 
		/// postprocessing code to be called (ie for tag blocks and tag definitions)
		/// </summary>
		public const uint DontPostprocess = 1 << 9;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we don't want tag 
		/// reference fields adding themselves to owner tag manager's valid and/or bad references
		/// </summary>
		/// <remarks>Used when opening a tag from a cache file (eg, for extraction)</remarks>
		public const uint DontTrackTagManagerReferences = 1 << 10;
		/// <summary>
		/// Bit for <see cref="ITagStream.Flags"/> that is set when we don't want tag 
		/// reference fields adding themselves to owner tag index's name reference manager 
		/// as a 'referencer'
		/// </summary>
		public const uint DontTrackTagReferencers = 1 << 11;

		/// <summary>
		/// Hack for Halo 3 tag resource fields which I initially thought were tag_data 
		/// fields. TagData uses this to know when to read an extra 4 bytes
		/// </summary>
		public const uint Halo3VertexBufferMegaHack = 1 << 12;

		/// <summary>
		/// Next available bit that can be used
		/// </summary>
		public const uint NextBit = 13;
	};

	public interface ITagStream
	{
		/// <summary>
		/// Handle to the owner object of this tag stream
		/// </summary>
		Blam.DatumIndex OwnerId { get; }

		/// <summary>
		/// Index of this tag stream within the owner object
		/// </summary>
		Blam.DatumIndex TagIndex { get; }

		Blam.DatumIndex ReferenceName { get; }

		/// <summary>
		/// Special flags
		/// </summary>
		Util.Flags Flags { get; }

		/// <summary>
		/// Engine version this stream belongs to
		/// </summary>
		BlamVersion Engine { get; }

		EndianReader GetInputStream();
		EndianWriter GetOutputStream();

		string GetExceptionDescription();
	};

	/// <summary>
	/// Interface to an object which can be streamed to\from a <see cref="ITagStream"/>
	/// </summary>
	public interface ITagStreamable
	{
		/// <summary>
		/// Reads the object from the tag stream
		/// </summary>
		/// <param name="ts"></param>
		void Read(ITagStream ts);
		/// <summary>
		/// Writes the object to the tag stream
		/// </summary>
		/// <param name="ts"></param>
		void Write(ITagStream ts);
	};
}