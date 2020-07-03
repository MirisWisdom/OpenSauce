/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BlamLib.Forms
{
	public partial class Block : BlamLib.Forms.Field, IBlockControl
	{
		#region Properties
		#region DefaultBlockName
		private string defaultBlockName;
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public string DefaultBlockName
		{
			get { return defaultBlockName; }
			set { defaultBlockName = value; }
		}
		#endregion

		#region IsDirty
		private bool isDirty = false;
		/// <summary>
		/// Returns weather there are changes made to this block since it was loaded
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public bool IsDirty { get { return isDirty; } }
		#endregion

		#region LinkedBlockIndexFields
		private List<Forms.Field> linkedBlockIndexFields = new List<Field>();
		/// <summary>
		/// All the block index controls that use
		/// this block for indexing
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public List<Forms.Field> LinkedBlockIndexFields { get { return linkedBlockIndexFields; } }
		#endregion

		#region CurrentIndex
		private int currentIndex = -1;
		/// <summary>
		/// The current active element index
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public int CurrentIndex { get { return currentIndex; } }
		#endregion

		#region BlockField
		private TagInterface.IBlock blockField = null;
		/// <summary>
		/// The block object this control is editing
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public TagInterface.IBlock BlockField
		{
			get { return blockField; }
			set { blockField = value; }
		}
		#endregion

		#region NameField
		private Field nameField = null;
		/// <summary>
		/// The field that acts as this block's
		/// name, and replaces the block's element
		/// name list string for the current active element
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public Field NameField { get { return nameField; } }
		#endregion

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public ComboBox ControlIndex { get { return FieldIndex; } }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public Panel ControlView { get { return FieldView; } }

		/// <summary>
		/// Controls the updating of fields
		/// </summary>
		private bool suspendUpdation = false;
		#endregion

		public Block()
		{
			InitializeComponent();
			_Setup(FieldName);

			OnFieldChangedTextHandler = new EventHandler(OnFieldChangedText);
			OnFieldChangedBlockHandler = new EventHandler(OnFieldChangedBlock);
			OnFieldChangedFlagHandler = new ItemCheckedEventHandler(OnFieldChangedFlag);
		}

		#region Enabling controls
		private void EnableMenuItems(bool enable)
		{
			FileMenu.Enabled = enable;
			EditMenu.Enabled = enable;
			OptionsMenu.Enabled = enable;
			FieldIndex.Enabled = enable;
			FieldAdd.Enabled = enable;
			FieldInsert.Enabled = enable;
			FieldDuplicate.Enabled = enable;
			FieldDelete.Enabled = enable;
			FieldDeleteAll.Enabled = enable;
		}

		/// <summary>
		/// Used for setting up the block for editing
		/// a cache file's tag data
		/// </summary>
		/// <param name="enable"></param>
		private void EnableEditMenuItems(bool enable)
		{
			FileLoad.Enabled = enable;
			EditPaste.Enabled = enable;
			
			// Maybe we could allow it to delete
			// elements and then allow a person to
			// Add/Insert/Duplicate another element(s)
			//FieldDelete.Enabled = enable;
			//FieldDeleteAll.Enabled = enable;
		}

		/// <summary>
		/// Shows\Hides the edit controls:
		/// Add, Insert, Duplicate, Delete, DeleteAll,
		/// Paste, Load
		/// </summary>
		/// <param name="show"></param>
		public void ShowEditControls(bool show)
		{
			EnableMenuItems(show);
			EnableEditMenuItems(show);
		}
		#endregion

		public override void Clear()
		{
			foreach (Field fi in FieldView.Controls)
				//if (fi is Block)
				//	(Block(fi)).OnDeleteAll(this, null);
				//else
					fi.Clear();
		}

		#region Add Field
		private int _x = 0, _y = 0;

		public void AddField(Field f)
		{
			SuspendLayout();
			f.SuspendLayout();

			if (f.Attribute.IsBlockName) nameField = f;
			if (f.Attribute.IsHidden) f.Size = new Size(f.Width, 0);
			if (f.Attribute.IsReadonly) f.Enabled = false;

			f.Clear();
			f.AddEventHandlers(
				OnFieldChangedTextHandler,
				OnFieldChangedBlockHandler,
				OnFieldChangedFlagHandler);

			f.Location = new Point(_x, _y); _y = f.Size.Height;
			Size = new Size(Size.Width, f.Size.Height + Size.Height);

			FieldView.Controls.Add(f);

			f.ResumeLayout();
			ResumeLayout();
		}
		#endregion

		#region Block Updating
		#region OnFieldChanged
		private EventHandler OnFieldChangedTextHandler;
		private void OnFieldChangedText(object sender, System.EventArgs e)
		{
			if (!suspendUpdation)
				UpdateFieldData(currentIndex, sender as Field); // we need to include a field index as well
		}

		private EventHandler OnFieldChangedBlockHandler;
		private void OnFieldChangedBlock(object sender, System.EventArgs e)
		{
			if (!suspendUpdation)
			{
				// get the menu item or combo box's panel
				// parent and get it's parent (which should be a block)
				Block b = (sender as Control).Parent.Parent.Parent as Block;
				b.UpdateParent();
				UpdateFieldData(currentIndex); // we need to include a field index as well
			}
		}

		private ItemCheckedEventHandler OnFieldChangedFlagHandler;
		private void OnFieldChangedFlag(object sender, System.Windows.Forms.ItemCheckedEventArgs e)
		{
			if (!suspendUpdation)
				UpdateFieldData(currentIndex, sender as Field); // we need to include a field index as well
		}
		#endregion

		private void UpdateField(int element, Field field, bool set_control)
		{
			if (element == -1) return;

			TagInterface.Definition elem = blockField.GetDefinition(element);

			// TODO: update this shit fool
// 			try { elem[field.DefinitionIndex].DoDataExchange(field, set_control); }
// 			catch (System.Exception ex)
// 			{
// 				Debug.LogFile.WriteLine(ex);
// 				Debug.LogFile.WriteLine("Element index was: {0}", element);
// 				Debug.LogFile.WriteLine("Field index was: {0}", field.DefinitionIndex);
// 				Debug.LogFile.WriteLine("Data exchange between a '{0}' control and a '{1}' field failed on update of {2}", field.GetType(), elem[field.DefinitionIndex].GetType(), set_control ? "control" : "field data");
// 			}
		}

		private void UpdateFields(int element, bool set_control)
		{
			if (element == -1) return;

			TagInterface.Definition elem = blockField.GetDefinition(element);

			foreach (Field field in FieldView.Controls)
			{
				// TODO: update this shit fool
// 				try { elem[field.DefinitionIndex].DoDataExchange(field, set_control); }
// 				catch (System.Exception ex)
// 				{
// 					Debug.LogFile.WriteLine(ex);
// 					Debug.LogFile.WriteLine("Element index was: {0}", element);
// 					Debug.LogFile.WriteLine("Field index was: {0}", field.DefinitionIndex);
// 					Debug.LogFile.WriteLine("Data exchange between a '{0}' control and a '{1}' field failed on update of {2}", field.GetType(), elem[field.DefinitionIndex].GetType(), set_control ? "control" : "field data");
// 				}
			}
		}

		/// <summary>
		/// Causes the block to perform data exchange between
		/// the controls and field data at the specified element
		/// </summary>
		/// <remarks>Control data FROM Field data</remarks>
		/// <param name="element"></param>
		public void UpdateFieldControls(int element) { UpdateFields(element, true); }

		/// <summary>
		/// Causes the block to perform data exchange between
		/// <paramref name="field"/> and the field data at the specified element
		/// </summary>
		/// <remarks>Control data FROM Field data</remarks>
		/// <param name="element"></param>
		/// <param name="field"></param>
		public void UpdateFieldControl(int element, Field field) { UpdateField(element, field, true); }

		/// <summary>
		/// Causes the block to perform data exchange between
		/// the field data and the controls at the specified element
		/// </summary>
		/// <remarks>Control data TO Field data</remarks>
		/// <param name="element"></param>
		public void UpdateFieldData(int element) { UpdateFields(element, false); }

		/// <summary>
		/// Causes the block to perform data exchange between
		/// the field data and <paramref name="field"/>
		/// </summary>
		/// <remarks>Control data TO Field data</remarks>
		/// <param name="element"></param>
		/// <param name="field"></param>
		public void UpdateFieldData(int element, Field field) { UpdateField(element, field, false); }

		/// <summary>
		/// 
		/// </summary>
		/// <remarks>Used by a nested block when a parent block changes and needs to update it</remarks>
		public void UpdateAllElements()
		{
			suspendUpdation = true;
			FieldIndex.Items.Clear();
			TagInterface.IElementArray elems;

			elems = blockField.GetElements();

			// if there are no elements for the nested block, lock it
			if (elems.Count == 0)
			{
				currentIndex = -1;
				FieldIndex.SelectedIndex = -1;

				Clear();
			}
			else
			{
				for (int x = 0; x < elems.Count; x++)
					FieldIndex.Items.Add(string.Format("{0}. {1}", x, defaultBlockName)); // TODO: call a formatting delegate and get rid of this stupid hack

				//if (nameField != null || predefinedNames != null || linkedBlockIndexFields != null)
					//UpdateIndexNames();

				FieldIndex.SelectedIndex = 0;
				FieldIndex.Enabled = true;
				FieldView.Enabled = true;

				if (elems.Count != blockField.MaxElements) // if we haven't reached the max elements yet...
				{
					FieldAdd.Enabled = true;
					FieldInsert.Enabled = true;
					FieldDuplicate.Enabled = true;
				}

				if (elems.Count <= blockField.MaxElements && elems.Count >= 1) // if we even have some elements...
				{
					FieldDelete.Enabled = true;
					FieldDeleteAll.Enabled = true;
				}
			}
			suspendUpdation = false;
		}
		#endregion

		private void OnShowMenuClick(object sender, EventArgs e)
		{
		}

		#region File Menu
		private void OnFileLoad(object sender, EventArgs e)
		{
		}

		private void OnFileSave(object sender, EventArgs e)
		{
		}
		#endregion

		#region Edit Menu
		private void OnEditCopy(object sender, EventArgs e)
		{
		}

		private void OnEditCopyAll(object sender, EventArgs e)
		{
		}

		private void OnEditPaste(object sender, EventArgs e)
		{
		}
		#endregion

		#region Block Menu
		private void OnAdd(object sender, EventArgs e)
		{
			suspendUpdation = true;

			// if we have the max number of elements, then lock anything that adds
			if (FieldIndex.Items.Count == blockField.MaxElements)
			{
				FieldAdd.Enabled = false;
				FieldInsert.Enabled = false;
				FieldDuplicate.Enabled = false;
				EditPaste.Enabled = false;
			}

			// if this is our first time adding
			// a element, unlock stuff
			if (currentIndex == -1)
			{
				FieldView.Enabled = true;
				FieldIndex.Enabled = true;
				FieldDuplicate.Enabled = true;
				FieldDelete.Enabled = true;
				FieldDeleteAll.Enabled = true;
				EditCopy.Enabled = true;
				EditCopyAll.Enabled = true;
				FileSave.Enabled = true;
			}
			else
				// else, update the current element's field data
				// before we switch to the newly added one
				UpdateFieldData(currentIndex);

			// hurrrrrrrrrrrrrr
			blockField.Add();

			FieldIndex.Items.Add(string.Format("{0}.{1}", FieldIndex.Items.Count, defaultBlockName)); // TODO: replace code with format delegate

			// sometimes a user will add a element when they're viewing 
			// some random element, so don't assume anything
			currentIndex = FieldIndex.Items.Count - 1;

			// we update even on very first element because the block could 
			// have special post-processing code and we would want the block's
			// fields to reflect it
			UpdateFieldControls(currentIndex);

			// change the field index view to the index we're now at
			FieldIndex.SelectedIndex = currentIndex;

			suspendUpdation = false;
		}

		private void OnInsert(object sender, EventArgs e)
		{
			suspendUpdation = true;

			// if we have the max number of elements, then lock anything that adds
			if (FieldIndex.Items.Count == blockField.MaxElements)
			{
				FieldAdd.Enabled = false;
				FieldInsert.Enabled = false;
				FieldDuplicate.Enabled = false;
				EditPaste.Enabled = false;
			}

			// update the current element's field data
			// before we switch to the newly inserted one
			UpdateFieldData(currentIndex);

			FieldIndex.Items.Insert(currentIndex, string.Format("{0}.{1}", currentIndex, defaultBlockName)); // TODO: replace code with format delegate

			blockField.Insert(currentIndex);

			UpdateFieldControls(currentIndex);

			suspendUpdation = false;
		}

		private void OnDuplicate(object sender, EventArgs e)
		{
			suspendUpdation = true;

			// if we have the max number of elements, then lock anything that adds
			if (FieldIndex.Items.Count == blockField.MaxElements)
			{
				FieldAdd.Enabled = false;
				FieldInsert.Enabled = false;
				FieldDuplicate.Enabled = false;
				EditPaste.Enabled = false;
			}

			// update the current element's field data
			// before we switch to the newly inserted one
			UpdateFieldData(currentIndex);

			FieldIndex.Items.Add(string.Format("{0}.{1}", FieldIndex.Items.Count, defaultBlockName)); // TODO: replace code with format delegate

			blockField.Duplicate(currentIndex); // duplicate the element we're viewing...
			currentIndex = FieldIndex.Items.Count - 1; // set to the index of the duplicate
			FieldIndex.SelectedIndex = currentIndex; // set the index view to it

			UpdateFieldControls(currentIndex);

			suspendUpdation = false;
		}

		private void OnDelete(object sender, EventArgs e)
		{
			try
			{
				// if we have the max number of elements, then unlock anything that adds
				if (FieldIndex.Items.Count == blockField.MaxElements)
				{
					FieldAdd.Enabled = true;
					FieldInsert.Enabled = true;
					FieldDuplicate.Enabled = true;
					EditPaste.Enabled = true;
				}

				// if we're about to delete our last element
				if (FieldIndex.Items.Count == 1)
				{
					FieldIndex.Enabled = false;
					FieldInsert.Enabled = false;
					FieldDuplicate.Enabled = false;
					EditCopy.Enabled = false;
					EditCopyAll.Enabled = false;

					Clear();
				}

				FieldIndex.Items.RemoveAt(currentIndex);
				blockField.Delete(currentIndex);

				FieldIndex.SelectedIndex = (currentIndex == 0 && FieldIndex.Items.Count > 0) ? 0 : --currentIndex;

				if (currentIndex != -1)
					UpdateFieldControls(currentIndex);
				else
					FieldView.Enabled = false;
			}
			catch(Exception ex)
			{
				throw new Debug.ExceptionLog(ex);
			}
		}

		private void OnDeleteAll(object sender, EventArgs e)
		{
			if(sender is IBlockControl ||
				MessageBox.Show("Are you sure you want to delete all block elements?", Program.Name, MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation) == DialogResult.OK)
			{
				foreach(Field f in FieldView.Controls)
				{
					if(f is Block)
					{
						Block tmp = (Block)f;
						tmp.FieldIndex.Items.Clear();
						tmp.blockField.DeleteAll();
						tmp.UpdateAllElements();
					}
				}

				FieldIndex.Items.Clear();
				blockField.DeleteAll();
				UpdateAllElements();
				Clear();
				FieldView.Enabled = false;

				FieldIndex.SelectedIndex = -1;
				currentIndex = -1;
				FieldIndex.Enabled = false;
				FieldInsert.Enabled = false;
				FieldDuplicate.Enabled = false;
				FieldDelete.Enabled = false;
				FieldDeleteAll.Enabled = false;
				EditCopy.Enabled = false;
				EditCopyAll.Enabled = false;
				FileSave.Enabled = false;
			}
		}
		#endregion

		#region Index
		private void OnIndexChanged(object sender, EventArgs e)
		{
			if (FieldIndex.Items.Count > 0)
			{
				suspendUpdation = true;

				UpdateFieldControls(currentIndex);

				suspendUpdation = false;
			}
		}

		private void OnIndexChangeCommited(object sender, EventArgs e)
		{
			if (FieldIndex.Items.Count > 0)
			{
				suspendUpdation = true;

				UpdateFieldData(currentIndex = FieldIndex.SelectedIndex);
				// we need to have something
				// that updates a single element's name
				// string.

				suspendUpdation = false;
			}
		}
		#endregion
	};
}