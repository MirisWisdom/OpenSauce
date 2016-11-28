/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace BlamLib.Forms
{
	public interface IBlockControl
	{
		/// <summary>
		/// Returns weather there are changes made to this block since it was loaded
		/// </summary>
		bool IsDirty { get; }

		/// <summary>
		/// The field that acts as this block's
		/// name, and replaces the block's element
		/// name list string for the current active element
		/// </summary>
		Field NameField { get; } // TODO: BlamLib.TagInterface.Editors.IField

		/// <summary>
		/// All the block index controls that use
		/// this block for indexing
		/// </summary>
		List<Field> LinkedBlockIndexFields { get; } // TODO: BlamLib.TagInterface.Editors.IField

		/// <summary>
		/// The current active element index
		/// </summary>
		int CurrentIndex { get; }

		/// <summary>
		/// The block object this control is editing
		/// </summary>
		TagInterface.IBlock BlockField { get; }

		ComboBox ControlIndex { get; }

		//Button ControlAdd { get; }

		//Button ControlInsert { get; }

		//Button ControlDuplicate { get; }

		//Button ControlDelete { get; }

		//Button ControlDeleteAll { get; }

		// <summary>
		// Control that copys the current element's
		// data
		// </summary>
		//Control ControlCopy { get; }

		// <summary>
		// Control that performs the copying
		// of all this block's data
		// </summary>
		//Control ControlCopyAll { get; }

		// <summary>
		// Control that performs the pasting
		// of block data into this one
		// </summary>
		//Control ControlPaste { get; }

		// <summary>
		// Control that performs the loading
		// of the block data
		// </summary>
		//Control ControlLoad { get; }

		// <summary>
		// Control that performs the saving
		// of the block data
		// </summary>
		//Control ControlSave { get; }

		/// <summary>
		/// Control where all the fields are placed
		/// </summary>
		Panel ControlView { get; }

		/// <summary>
		/// Shows\Hides the edit controls:
		/// Add, Insert, Duplicate, Delete, DeleteAll,
		/// Paste, Load
		/// </summary>
		/// <param name="show"></param>
		void ShowEditControls(bool show);

		// <summary>
		// Like <c>Field.Clear()</c>, but iterates through all
		// the controls in the block and clears them
		// </summary>
		//void ClearFields();

		/// <summary>
		/// Causes the block to perform data exchange between
		/// the controls and field data at the specified element
		/// </summary>
		/// <remarks>Control data FROM Field data</remarks>
		/// <param name="element"></param>
		void UpdateFieldControls(int element);

		/// <summary>
		/// Causes the block to perform data exchange between
		/// the field data and the controls at the specified element
		/// </summary>
		/// <remarks>Control data TO Field data</remarks>
		/// <param name="element"></param>
		void UpdateFieldData(int element);

		/// <summary>
		/// 
		/// </summary>
		/// <remarks>Used by a nested block when a parent block changes and needs to update it</remarks>
		void UpdateAllElements();

		// TODO: BlamLib.TagInterface.Editors.IField
		// For the WinForms Block control, we could just as easily cast
		// [f] to a Field object, but we don't really want to limit this interface
		// to the Field class
		void AddField(Field f);
	};
}