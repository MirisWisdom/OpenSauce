/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.TagInterface
{
	/// <summary>
	/// Tag field editing interfaces and utilities
	/// </summary>
	public static class Editors
	{
		/// <summary>
		/// Arguments used for exchanging data between a UI and definition data
		/// </summary>
		public class DoDataExchangeEventArgs : EventArgs
		{
			#region FieldInterface
			private IField fieldInterface;
			/// <summary>
			/// The UI interface to the field this exchange is for
			/// </summary>
			public IField FieldInterface	{ get { return fieldInterface; } }
			#endregion

			#region IsPolling
			private bool isPolling;
			/// <summary>
			/// Is the UI interface polling the field data?
			/// </summary>
			/// <remarks>
			/// If <value /> is true, then the UI interface is getting the field 
			/// value from the definition data.
			/// If <value /> is false, then the UI interface is setting the field 
			/// with a new value
			/// </remarks>
			public bool IsPolling	{ get { return isPolling; } }
			#endregion

			#region ExchangeFailed
			private bool exchangeFailed;
			/// <summary>
			/// Did the exchange of data between the field it's controller fail?
			/// </summary>
			public bool ExchangeFailed	{ get { return exchangeFailed; } }
			#endregion

			public void SetExchangeFailure() // TODO: design this
			{
				exchangeFailed = true;
			}

			public void Setup(IField field) { fieldInterface = field; }

			public void Setup(IField field, bool is_polling)
			{
				Setup(field);
				isPolling = is_polling;
			}

			public DoDataExchangeEventArgs()
			{
				exchangeFailed = false;
			}
		};

		#region Field
		/// <summary>
		/// Base interface for all field editors
		/// </summary>
		public interface IField
		{
			/// <summary>
			/// The display name used for this field editor
			/// </summary>
			string ControlName { get; }
			/// <summary>
			/// The extended information for this field editor
			/// </summary>
			string Help { get; }
			/// <summary>
			/// The special units (ie, meters, game ticks, etc) for this field editor)
			/// </summary>
			string Units { get; }
			/// <summary>
			/// The field index inside a <see cref="BlamLib.TagInterface.Definition"/> this field editor links to
			/// </summary>
			int DefinitionIndex { get; }

			/// <summary>
			/// Empties the field(s) of this field editor to a 'null' value
			/// </summary>
			void Clear();
		};

		/// <summary>
		/// 
		/// </summary>
		public interface ICustomField : IField
		{
		};
		#endregion

		#region Integers
		/// <summary>
		/// <see cref="BlamLib.TagInterface.ByteInteger"/> field editor interface
		/// </summary>
		public interface IByteInteger : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			byte Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.ShortInteger"/> field editor interface
		/// </summary>
		public interface IShortInteger : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			short Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.LongInteger"/> field editor interface
		/// </summary>
		public interface ILongInteger : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			int Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Tag"/> field editor interface
		/// </summary>
		public interface ITag : IField
		{
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Enum"/> field editor interface
		/// </summary>
		public interface IEnum : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			int Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Flags"/> field editor interface
		/// </summary>
		public interface IFlags : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			uint Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Point2D"/> field editor interface
		/// </summary>
		public interface IPoint2D : IField
		{
			/// <summary>
			/// Interface to the editor's X-coordinate field data
			/// </summary>
			short Field_X { get; set; }
			/// <summary>
			/// Interface to the editor's Y-coordinate field data
			/// </summary>
			short Field_Y { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Rectangle2D"/> field editor interface
		/// </summary>
		public interface IRectangle2D : IField
		{
			/// <summary>
			/// Interface to the editor's 'Top' field data
			/// </summary>
			short Field_T { get; set; }
			/// <summary>
			/// Interface to the editor's 'Left' field data
			/// </summary>
			short Field_L { get; set; }
			/// <summary>
			/// Interface to the editor's 'Bottom' field data
			/// </summary>
			short Field_B { get; set; }
			/// <summary>
			/// Interface to the editor's 'Right' field data
			/// </summary>
			short Field_R { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Color"/> field editor interface
		/// </summary>
		public interface IColor : IField
		{
			/// <summary>
			/// Interface to the editor's Alpha field data
			/// </summary>
			byte Field_A { get; set; }
			/// <summary>
			/// Interface to the editor's Red field data
			/// </summary>
			byte Field_R { get; set; }
			/// <summary>
			/// Interface to the editor's Green field data
			/// </summary>
			byte Field_G { get; set; }
			/// <summary>
			/// Interface to the editor's Blue field data
			/// </summary>
			byte Field_B { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.ShortIntegerBounds"/> field editor interface
		/// </summary>
		public interface IShortIntegerBounds : IField
		{
			/// <summary>
			/// Interface to the editor's Lower field data
			/// </summary>
			short Field_Lower { get; set; }
			/// <summary>
			/// Interface to the editor's Upper field data
			/// </summary>
			short Field_Upper { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.BlockIndex"/> field editor interface
		/// </summary>
		public interface IBlockIndex : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			int Field { get; set; }
		};
		#endregion

		#region Reals
		/// <summary>
		/// <see cref="BlamLib.TagInterface.Real"/> field editor interface
		/// </summary>
		public interface IReal : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			float Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealPoint2D"/> field editor interface
		/// </summary>
		public interface IRealPoint2D : IField
		{
			/// <summary>
			/// Interface to the editor's X-coordinate field data
			/// </summary>
			float Field_X { get; set; }
			/// <summary>
			/// Interface to the editor's Y-coordinate field data
			/// </summary>
			float Field_Y { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealPoint3D"/> field editor interface
		/// </summary>
		public interface IRealPoint3D : IRealPoint2D
		{
			/// <summary>
			/// Interface to the editor's Z-coordinate field data
			/// </summary>
			float Field_Z { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealVector2D"/> field editor interface
		/// </summary>
		public interface IRealVector2D : IField
		{
			/// <summary>
			/// Interface to the editor's I-component field data
			/// </summary>
			float Field_I { get; set; }
			/// <summary>
			/// Interface to the editor's J-component field data
			/// </summary>
			float Field_J { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealVector3D"/> field editor interface
		/// </summary>
		public interface IRealVector3D : IRealVector2D
		{
			/// <summary>
			/// Interface to the editor's K-component field data
			/// </summary>
			float Field_K { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealQuaternion"/> field editor interface
		/// </summary>
		public interface IRealQuaternion : IRealVector3D
		{
			/// <summary>
			/// Interface to the editor's W-component field data
			/// </summary>
			float Field_W { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealEulerAngles2D"/> field editor interface
		/// </summary>
		public interface IRealEulerAngles2D : IField
		{
			/// <summary>
			/// Interface to the editor's Yaw field data
			/// </summary>
			float Field_Y { get; set; }
			/// <summary>
			/// Interface to the editor's Pitch field data
			/// </summary>
			float Field_P { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealEulerAngles3D"/> field editor interface
		/// </summary>
		public interface IRealEulerAngles3D : IRealEulerAngles2D
		{
			/// <summary>
			/// Interface to the editor's Rotation field data
			/// </summary>
			float Field_R { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealPlane2D"/> field editor interface
		/// </summary>
		public interface IRealPlane2D : IField
		{
			/// <summary>
			/// Interface to the editor's I-component field data
			/// </summary>
			float Field_I { get; set; }
			/// <summary>
			/// Interface to the editor's J-component field data
			/// </summary>
			float Field_J { get; set; }
			/// <summary>
			/// Interface to the editor's D-component field data
			/// </summary>
			float Field_D { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealPlane3D"/> field editor interface
		/// </summary>
		public interface IRealPlane3D : IRealPlane2D
		{
			/// <summary>
			/// Interface to the editor's K-component field data
			/// </summary>
			float Field_K { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealColor"/> field editor interface
		/// </summary>
		public interface IRealColor : IField
		{
			/// <summary>
			/// Interface to the editor's Alpha field data
			/// </summary>
			float Field_A { get; set; }
			/// <summary>
			/// Interface to the editor's Red field data
			/// </summary>
			float Field_R { get; set; }
			/// <summary>
			/// Interface to the editor's Green field data
			/// </summary>
			float Field_G { get; set; }
			/// <summary>
			/// Interface to the editor's Blue field data
			/// </summary>
			float Field_B { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.RealBounds"/> field editor interface
		/// </summary>
		public interface IRealBounds : IField
		{
			/// <summary>
			/// Interface to the editor's Lower field data
			/// </summary>
			float Field_Lower { get; set; }
			/// <summary>
			/// Interface to the editor's Upper field data
			/// </summary>
			float Field_Upper { get; set; }
		};
		#endregion

		#region String
		/// <summary>
		/// <see cref="BlamLib.TagInterface.String"/> field editor interface
		/// </summary>
		public interface IString : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			string Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.StringId"/> field editor interface
		/// </summary>
		public interface IStringId : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			string Field { get; set; }
		};
		#endregion

		#region Structures
		/// <summary>
		/// <see cref="BlamLib.TagInterface.Data"/> field editor interface
		/// </summary>
		public interface IData : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			byte[] Field { get; set; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.TagReference"/> field editor interface
		/// </summary>
		public interface ITagReference : IField
		{
			/// <summary>
			/// Interface to the editor's tag group field data
			/// </summary>
			TagGroup Field_GroupTag { get; set; }

			/// <summary>
			/// Interface to the editor's tag name field data
			/// </summary>
			string Field_Name { get; set; }
		};

		/// <summary>
		/// Bit flags for manipulating a block editor's view
		/// </summary>
		[System.Flags]
		public enum BlockViewState : uint
		{
			/// <summary>
			/// The editor component for selecting a block element
			/// </summary>
			Index		= 1<<0,

			/// <summary>
			/// The editor component for adding a new element
			/// </summary>
			Add			= 1<<1,
			/// <summary>
			/// The editor component for inserting a new element
			/// </summary>
			Insert		= 1<<2,
			/// <summary>
			/// The editor component for duplicating the current element
			/// </summary>
			Duplicate	= 1<<3,
			/// <summary>
			/// The editor component for deleting the current element
			/// </summary>
			Delete		= 1<<4,
			/// <summary>
			/// The editor component for deleting all the elements
			/// </summary>
			DeleteAll	= 1<<5,

			/// <summary>
			/// The editor component for copying the current element
			/// </summary>
			Copy		= 1<<6,
			/// <summary>
			/// The editor component for copying all the elements
			/// </summary>
			CopyAll		= 1<<7,
			/// <summary>
			/// The editor component for pasting elements copied elsewhere into this block
			/// </summary>
			Paste		= 1<<8,

			/// <summary>
			/// The editor component for loading elements from a stream
			/// </summary>
			Load		= 1<<9,
			/// <summary>
			/// The editor component for saving elements to a stream
			/// </summary>
			Save		= 1<<10,

			/// <summary>
			/// The editor component for viewing the block's fields editors
			/// </summary>
			Fields		= 1<<11,

			// = 1<<12,
			// = 1<<13,
			// = 1<<14,
			// = 1<<15,

			/// <summary>
			/// Editor elements that will get ENABLED\DISABLED when a block has the max 
			/// amount of elements
			/// </summary>
			StateFull = 
				Add | Insert | Duplicate | 
				Paste,

			/// <summary>
			/// Editor elements that will get ENABLED when a block is adding it's 
			/// first element, or DISABLED when a block is deleting it's last element
			/// </summary>
			StateEmpty = 
				Index | 
				Insert | Duplicate | Delete | DeleteAll | 
				Copy | CopyAll | 
				Save | 
				Fields,
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Block{T}"/> field editor interface
		/// </summary>
		public interface IBlock : IField
		{
			/// <summary>
			/// All the block index editors that use this block for indexing
			/// </summary>
			IEnumerable<IField> LinkedBlockIndexFields { get; }
			/// <summary>
			/// The current active element index
			/// </summary>
			int CurrentIndex { get; }
			/// <summary>
			/// The block object this field editor is editing
			/// </summary>
			TagInterface.IBlock BlockField { get; set; }
			/// <summary>
			/// The field that acts as this block's name and replaces the block's element 
			/// name list string for the current active element
			/// </summary>
			TagInterface.Field NameField { get; }
		};

		/// <summary>
		/// <see cref="BlamLib.TagInterface.Struct{T}"/> field editor interface
		/// </summary>
		public interface IStruct : IField
		{
			/// <summary>
			/// Interface to the editor's field data
			/// </summary>
			TagInterface.IStruct Field { get; set; }
		};
		#endregion
	};
}