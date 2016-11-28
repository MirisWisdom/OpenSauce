// Code Adopted from: http://www.codeproject.com/KB/edit/ValidatingTextBoxControls.aspx
using System;

namespace BlamLib.Forms
{
	/// <summary>
	/// Encapsulates a textbox's selection. </summary>
	/// <seealso cref="Selection.Saver" />
	public class Selection
	{
		#region TextBox
		private TextBox textBox = null;
		/// <summary>
		/// Gets the TextBox object associated with this Selection object. </summary>
		public TextBox TextBox { get { return textBox; } }
		#endregion

		/// <summary>
		/// Event used to notify that the selected text is about to change. </summary>
		/// <remarks>
		/// This event is fired by Replace right before it replaces the textbox's text. </remarks>
		/// <seealso cref="Replace" />
		public event EventHandler TextChanging;

		#region Constructors
		/// <summary>
		/// Initializes a new instance of the Selection class by associating it with a TextBox derived object. </summary>
		/// <param name="textBox">
		/// The TextBox object for which the selection is being manipulated. </param>
		/// <seealso cref="System.Windows.Forms.TextBox" />	
		public Selection(TextBox textBox) { this.textBox = textBox; }

		/// <summary>
		/// Initializes a new instance of the Selection class by associating it with a TextBox derived object. 
		/// and then selecting text on it. </summary>
		/// <param name="textBox">
		/// The TextBox object for which the selection is being manipulated. </param>
		/// <param name="start">
		/// The zero-based position where to start the selection. </param>
		/// <param name="end">
		/// The zero-based position where to end the selection.  If it's equal to the start position, no text is selected. </param>
		/// <seealso cref="System.Windows.Forms.TextBox" />	
		public Selection(TextBox textBox, int start, int end)
		{
			this.textBox = textBox;
			Set(start, end);
		}
		#endregion

		/// <summary>
		/// Selects the textbox's text. </summary>
		/// <param name="start">
		/// The zero-based position where to start the selection. </param>
		/// <param name="end">
		/// The zero-based position where to end the selection.  If it's equal to the start position, no text is selected. </param>
		/// <remarks>
		/// The end must be greater than or equal to the start position. </remarks>
		/// <seealso cref="Get" />
		public void Set(int start, int end)
		{
			textBox.SelectionStart = start;
			textBox.SelectionLength = end - start;
		}

		/// <summary>
		/// Retrieves the start and end position of the selection. </summary>
		/// <param name="start">
		/// The zero-based position where the selection starts. </param>
		/// <param name="end">
		/// The zero-based position where selection ends.  If it's equal to the start position, no text is selected. </param>
		/// <seealso cref="Set" />
		public void Get(out int start, out int end)
		{
			start = textBox.SelectionStart;
			end = start + textBox.SelectionLength;

			if (start < 0)
				start = 0;
			if (end < start)
				end = start;
		}

		/// <summary>
		/// Replaces the text selected on the textbox. </summary>
		/// <param name="text">
		/// The text to replace the selection with. </param>
		/// <remarks>
		/// If nothing is selected, the text is inserted at the caret's position. </remarks>
		/// <seealso cref="SetAndReplace" />
		public void Replace(string text)
		{
			if (TextChanging != null)
				TextChanging(this, null);

			textBox.SelectedText = text;
		}

		/// <summary>
		/// Selects the textbox's text and replaces it. </summary>
		/// <param name="start">
		/// The zero-based position where to start the selection. </param>
		/// <param name="end">
		/// The zero-based position where to end the selection.  If it's equal to the start position, no text is selected. </param>
		/// <param name="text">
		/// The text to replace the selection with. </param>
		/// <remarks>
		/// The end must be greater than or equal to the start position.
		/// If nothing gets selected, the text is inserted at the caret's position. </remarks>
		/// <seealso cref="Set" />
		/// <seealso cref="Replace" />
		public void SetAndReplace(int start, int end, string text)
		{
			Set(start, end);
			Replace(text);
		}

		/// <summary>
		/// Changes the selection's start and end positions by an offset. </summary>
		/// <param name="start">
		/// How much to change the start of the selection by. </param>
		/// <param name="end">
		/// How much to change the end of the selection by. </param>
		/// <seealso cref="Set" />	
		public void MoveBy(int start, int end)
		{
			End += end;
			Start += start;
		}

		/// <summary>
		/// Changes the internal start and end positions by an offset. </summary>
		/// <param name="pos">
		/// How much to change the start and end of the selection by. </param>
		/// <seealso cref="Set" />	
		public void MoveBy(int pos)
		{
			MoveBy(pos, pos);
		}

		/// <summary>
		/// Creates a new Selection object with the internal start and end 
		/// positions changed by an offset. </summary>
		/// <param name="selection">
		/// The object with the original selection.  </param>
		/// <param name="pos">
		/// How much to change the start and end of the selection by on the resulting object. </param>
		/// <seealso cref="MoveBy" />	
		/// <seealso cref="Set" />	
		public static Selection operator +(Selection selection, int pos)
		{
			return new Selection(selection.textBox, selection.Start + pos, selection.End + pos);
		}

		/// <summary>
		/// Gets or sets the zero-based position for the start of the selection. </summary>
		/// <seealso cref="End" />	
		/// <seealso cref="Length" />	
		public int Start
		{
			get { return textBox.SelectionStart; }
			set { textBox.SelectionStart = value; }
		}

		/// <summary>
		/// Gets or sets the zero-based position for the end of the selection. </summary>
		/// <seealso cref="Start" />	
		/// <seealso cref="Length" />	
		public int End
		{
			get { return textBox.SelectionStart + textBox.SelectionLength; }
			set { textBox.SelectionLength = value - textBox.SelectionStart; }
		}

		/// <summary>
		/// Gets or sets the length of the selection. </summary>
		/// <seealso cref="Start" />	
		/// <seealso cref="End" />	
		public int Length
		{
			get { return textBox.SelectionLength; }
			set { textBox.SelectionLength = value; }
		}

		/// <summary>
		/// Saves (and later restores) the current start and end position of a textbox selection. </summary>
		/// <remarks>
		/// This class saves the start and end position of the textbox with which it is constructed
		/// and then restores it when Restore is called.  Since this is a IDisposable class, it can also
		/// be used inside a <c>using</c> statement to Restore the selection (via Dispose). </remarks>
		public class Saver : IDisposable
		{
			#region TextBox
			private TextBox textBox;
			/// <summary>
			///   Gets the TextBox object associated with this Saver object. </summary>
			public TextBox TextBox { get { return textBox; } }
			#endregion

			private Selection selection;

			#region Start
			private int start;
			/// <summary>
			/// Gets or sets the zero-based position for the start of the selection. </summary>
			/// <seealso cref="End" />	
			public int Start
			{
				get { return this.start; }
				set { this.start = value; }
			}
			#endregion

			#region End
			private int end;
			/// <summary>
			/// Gets or sets the zero-based position for the end of the selection. </summary>
			/// <seealso cref="Start" />	
			public int End
			{
				get { return this.end; }
				set { this.end = value; }
			}
			#endregion

			#region Constructors
			/// <summary>
			/// Initializes a new instance of the Saver class by associating it with a TextBox derived object. </summary>
			/// <param name="textBox">
			/// The TextBox object for which the selection is being saved. </param>
			/// <remarks>
			/// This constructor saves the textbox's start and end position of the selection inside private fields. </remarks>
			/// <seealso cref="System.Windows.Forms.TextBox" />	
			public Saver(TextBox textBox)
			{
				this.textBox = textBox;
				selection = new Selection(textBox);
				selection.Get(out this.start, out this.end);
			}

			/// <summary>
			/// Initializes a new instance of the Saver class by associating it with a TextBox derived object 
			/// and passing the start and end position of the selection. </summary>
			/// <param name="textBox">
			/// The TextBox object for which the selection is being saved. </param>
			/// <param name="start">
			/// The zero-based start position of the selection. </param>
			/// <param name="end">
			/// The zero-based end position of the selection. It must not be less than the start position. </param>
			/// <remarks>
			/// This constructor does not save the textbox's start and end position of the selection.
			/// Instead, it saves the two given parameters. </remarks>
			/// <seealso cref="System.Windows.Forms.TextBox" />	
			public Saver(TextBox textBox, int start, int end)
			{
				this.textBox = textBox;
				selection = new Selection(textBox);
				System.Diagnostics.Debug.Assert(start <= end);

				this.start = start;
				this.end = end;
			}
			#endregion

			/// <summary>
			/// Restores the selection on the textbox to the saved start and end values. </summary>
			/// <remarks>
			/// This method checks that the textbox is still <see cref="Disable">available</see> 
			/// and if so restores the selection.  </remarks>
			/// <seealso cref="Disable" />	
			public void Restore()
			{
				if (textBox == null)
					return;

				selection.Set(this.start, this.end);
				textBox = null;
			}

			/// <summary>
			/// Calls the <see cref="Restore" /> method. </summary>
			public void Dispose() { Restore(); }

			/// <summary>
			/// Changes the internal start and end positions. </summary>
			/// <param name="start">
			/// The new zero-based position for the start of the selection. </param>
			/// <param name="end">
			/// The new zero-based position for the end of the selection. It must not be less than the start position. </param>
			/// <seealso cref="MoveBy" />	
			public void MoveTo(int start, int end)
			{
				System.Diagnostics.Debug.Assert(start <= end);

				this.start = start;
				this.end = end;
			}

			/// <summary>
			/// Changes the internal start and end positions by an offset. </summary>
			/// <param name="start">
			/// How much to change the start of the selection by. </param>
			/// <param name="end">
			/// How much to change the end of the selection by. </param>
			/// <seealso cref="MoveTo" />	
			public void MoveBy(int start, int end)
			{
				this.start += start;
				this.end += end;

				System.Diagnostics.Debug.Assert(this.start <= this.end);
			}

			/// <summary>
			/// Changes the internal start and end positions by an offset. </summary>
			/// <param name="pos">
			/// How much to change the start and end of the selection by. </param>
			/// <seealso cref="MoveTo" />	
			public void MoveBy(int pos)
			{
				this.start += pos;
				this.end += pos;
			}

			/// <summary>
			/// Creates a new Saver object with the internal start and end 
			/// positions changed by an offset. </summary>
			/// <param name="saver">
			/// The object with the original saved selection.  </param>
			/// <param name="pos">
			/// How much to change the start and end of the selection by on the resulting object. </param>
			/// <seealso cref="MoveTo" />	
			public static Saver operator +(Saver saver, int pos)
			{
				return new Saver(saver.textBox, saver.start + pos, saver.end + pos);
			}

			/// <summary>
			/// Updates the internal start and end positions with the current selection on the textbox. </summary>
			/// <seealso cref="Disable" />	
			public void Update()
			{
				if (textBox != null)
					selection.Get(out this.start, out this.end);
			}

			/// <summary>
			/// Disables restoring of the textbox's selection when <see cref="Dispose" /> is called. </summary>
			/// <seealso cref="Dispose" />	
			/// <seealso cref="Update" />	
			public void Disable() { textBox = null; }
		};
	};
}