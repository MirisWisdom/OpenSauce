// Code Adopted from: http://www.codeproject.com/KB/edit/ValidatingTextBoxControls.aspx
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Text;
using WinForms = System.Windows.Forms;

namespace BlamLib.Forms
{
	#region Behavior
	/// <summary>
	/// Base class for all behavior classes in this namespace.
	/// It is designed to represent a behavior object that may be associated with a TextBox object. </summary>
	/// <seealso cref="AlphanumericBehavior" />
	/// <seealso cref="NumericBehavior" />
	/// <seealso cref="IntegerBehavior" />
	public abstract class Behavior : IDisposable
	{
		/// <summary> The caption to use for all error message boxes. </summary>		
		private static string errorCaption;

		/// <summary> The TextBox object associated with this Behavior. </summary>
		protected TextBox textBox;
		/// <summary> The flags turned on for this Behavior. </summary>
		protected int flags;
		/// <summary> When true it indicates that HandleTextChanged should behave as if no text had changed and not call <see cref="UpdateText" />. </summary>
		protected bool noTextChanged;
		/// <summary> Helper object used to manipulate the selection of the TextBox object. </summary>
		protected Selection selection;
		/// <summary> The object used to show a blinking icon (with an error) next to the control. </summary>		
		protected WinForms.ErrorProvider errorProvider;

		#region Constructors
		/// <summary>
		/// Initializes a new instance of the Behavior class by associating it with a TextBox derived object. </summary>
		/// <param name="textBox">
		/// The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <param name="addEventHandlers">
		/// If true, the textBox's event handlers are tied to the corresponding methods on this behavior object. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor is <c>protected</c> since this class is only meant to be used as a base for other behaviors. </remarks>
		/// <seealso cref="System.Windows.Forms.TextBox" />	
		/// <seealso cref="AddEventHandlers" />	
		protected Behavior(TextBox textBox, bool addEventHandlers)
		{
			if (textBox == null)
				throw new ArgumentNullException("textBox");

			this.textBox = textBox;
			this.selection = new Selection(this.textBox);
			this.selection.TextChanging += new EventHandler(HandleTextChangingBySelection);

			if (addEventHandlers)
				AddEventHandlers();
		}

		/// <summary>
		/// Initializes a new instance of the Behavior class by copying it from 
		/// another Behavior object. </summary>
		/// <param name="behavior">
		/// The Behavior object to copied (and then disposed of).  It must not be null. </param>
		/// <exception cref="ArgumentNullException">behavior is null. </exception>
		/// <remarks>
		/// This constructor is <c>protected</c> since this class is only meant to be used as a base for other behaviors. 
		/// After the behavior.TextBox object is copied, Dispose is called on the behavior parameter. </remarks>
		/// <seealso cref="TextBox" />	
		/// <seealso cref="Dispose" />	
		protected Behavior(Behavior behavior)
		{
			if (behavior == null)
				throw new ArgumentNullException("behavior");

			TextBox = behavior.TextBox;
			this.flags = behavior.flags;

			behavior.Dispose();
		}
		#endregion

		/// <summary>Handles the text changing as a result of direct manipulation of the selection. </summary>
		/// <remarks>
		/// This method sets this.noTextChanged flag to true so that UpdateText is not called 
		/// unnecessarily inside HandleTextChanged. </remarks>
		private void HandleTextChangingBySelection(object sender, EventArgs e) { this.noTextChanged = true; }

		/// <summary>Retrieves the textbox's text in valid form. </summary>
		/// <returns>If the textbox's text is valid, it is returned; otherwise a valid version of it is returned. </returns>
		/// <remarks>
		/// This method is designed to be overridden by derived Behavior classes.
		/// Here it just returns the textbox's text. </remarks>
		protected virtual string GetValidText() { return this.textBox.Text; }

		/// <summary>Checks if the textbox's text is valid and if not updates it with a valid value. </summary>
		/// <returns>If the textbox's text is updated (because it wasn't valid), the return value is true; otherwise it is false. </returns>
		/// <remarks>This method is used by derived classes to ensure the textbox's text is kept valid. </remarks>
		public virtual bool UpdateText()
		{
			string validText = GetValidText();
			if (validText != this.textBox.Text)
			{
				this.textBox.Text = validText;
				return true;
			}
			return false;
		}

		/// <summary>Gets or sets the TextBox object associated with this Behavior object (which is not allowed to be null). </summary>
		/// <exception cref="ArgumentNullException">TextBox is set to null. </exception>
		/// <remarks>
		/// Before the TextBox object gets replaced, its event handlers are detached from this behavior object. 
		/// Then they're attached to the new object. </remarks>
		public TextBox TextBox
		{
			get { return this.textBox; }
			set
			{
				if (value == null)
					throw new ArgumentNullException("value");

				RemoveEventHandlers();

				this.textBox = value;
				this.selection = new Selection(this.textBox);
				this.selection.TextChanging += new EventHandler(HandleTextChangingBySelection);

				AddEventHandlers();
			}
		}

		/// <summary>Converts the given text to an integer. </summary>
		/// <returns>The return value is the text as an integer, or 0 if the conversion cannot be done. </returns>
		/// <remarks>
		/// This method serves as a convenience for derived Behavior classes that
		/// need to convert a string to an integer without worrying about a System.FormatException 
		/// exception being thrown. </remarks>
		/// <seealso cref="ToDouble" />	
		protected int ToInt(string text)
		{
			try
			{
				// Make it work like "atoi" -- ignore any trailing non-digit characters
				for (int i = 0, length = text.Length; i < length; i++)
				{
					if (!Char.IsDigit(text[i]))
						return Convert.ToInt32(text.Substring(0, i));
				}

				return Convert.ToInt32(text);
			}
			catch
			{
				return 0;
			}
		}

		/// <summary>Converts the given text to a double. </summary>
		/// <returns>The return value is the text as a double, or 0 if the conversion cannot be done. </returns>
		/// <remarks>
		/// This method serves as a convenience for derived Behavior classes that
		/// need to convert a string to a double without worrying about a System.FormatException 
		/// exception being thrown. </remarks>
		/// <seealso cref="ToInt" />	
		protected double ToDouble(string text)
		{
			try { return Convert.ToDouble(text); }
			catch { return 0; }
		}

		/// <summary>Gets or sets the flags associated with this Behavior object. </summary>
		/// <remarks>
		/// This property serves as a convenience for derived Behavior classes
		/// which can use it to store binary attributes (flags) inside its individual bits. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="ModifyFlags" />
		public virtual int Flags
		{
			get { return this.flags; }
			set
			{
				if (this.flags == value)
					return;

				this.flags = value;
				UpdateText();
			}
		}

		/// <summary>Adds or removes flags from the behavior. </summary>
		/// <param name="flags">The bits to be turned on (ORed) or turned off in the internal flags member. </param>
		/// <param name="addOrRemove">If true the flags are added, otherwise they're removed. </param>
		/// <remarks>
		/// This method is a convenient way to modify the <see cref="Flags" /> property without overwriting its value.
		/// If the internal flags value is changed, <see cref="UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="Flags" />
		public void ModifyFlags(int flags, bool addOrRemove)
		{
			if (addOrRemove)
				Flags = this.flags | flags;
			else
				Flags = this.flags & ~flags;
		}

		/// <summary>Checks if a flag value is set (turned on). </summary>
		/// <param name="flag">The flag to check. </param>
		/// <returns>If the flag is set, the return value is true; otherwise it is false. </returns>
		/// <seealso cref="Behavior.Flags" />
		public bool HasFlag(int flag) { return (this.flags & flag) != 0; }

		/// <summary>Shows an error message box. </summary>
		/// <param name="message">The message to show. </param>
		/// <remarks>Although doing so is not expected, this method may be overridden by derived classes. </remarks>
		public virtual void ShowErrorMessageBox(string message)
		{
			WinForms.MessageBox.Show(this.textBox, message, ErrorCaption, WinForms.MessageBoxButtons.OK, WinForms.MessageBoxIcon.Exclamation);
		}

		/// <summary>Shows a blinking icon next to the textbox with an error message. </summary>
		/// <param name="message">The message to show when the cursor is placed over the icon. </param>
		/// <remarks>Although doing so is not expected, this method may be overridden by derived classes. </remarks>
		public virtual void ShowErrorIcon(string message)
		{
			if (this.errorProvider == null)
			{
				if (message == "")
					return;
				this.errorProvider = new WinForms.ErrorProvider();
			}
			this.errorProvider.SetError(this.textBox, message);
		}

		/// <summary>Gets the error message used to notify the user to enter a valid value. </summary>
		/// <remarks>
		/// This property is used by <see cref="Validate" /> to retrieve the message to 
		/// display in a message box or icon if the validation fails, depending on the flags set by the user.
		/// Here it just shows a generic error message, but it is meant to be overridden by 
		/// Behavior classes where either the allowed range of values is not controlled as 
		/// the user types (e.g. NumericBehavior, TimeBehavior), or the value is not considered 
		/// valid until the user has entered all the required characters (e.g. DateBehavior, TimeBehavior). </remarks>
		/// <seealso cref="Validate" />
		/// <seealso cref="IsValid" />
		/// <seealso cref="ErrorCaption" />
		public virtual string ErrorMessage { get { return "Please specify a valid value."; } }

		/// <summary>Gets or sets the caption to use for all error message boxes. </summary>
		/// <remarks>
		/// This property may be used to change the default caption (<see cref="System.Windows.Forms.Application.ProductName" />) used
		/// for all error message boxes shown via the <see cref="ShowErrorMessageBox" /> method. </remarks>
		/// <seealso cref="ShowErrorMessageBox" />
		public static string ErrorCaption
		{
			get
			{
				if (errorCaption == null) return Program.Name;
				return errorCaption;
			}
			set { errorCaption = value; }
		}

		/// <summary>  
		/// If TRACE are defined for the compiler, a message line is sent to the tracer. </summary>
		/// <param name="message">
		/// The message line to trace. </param>
		/// <remarks>
		/// This method is used to help diagnose problems.  It's called at the beginning of all 
		/// event handlers (the ones that begin with Handle) to trace the program's execution. </remarks>
		[Conditional("TRACE")]
		public void TraceLine(string message) { Trace.WriteLine(message); }

		/// <summary>Checks if the textbox's value is valid and if not proceeds according to the behavior's <see cref="Flags" />. </summary>
		/// <returns>If the validation succeeds, the return value is true; otherwise it is false. </returns>
		/// <remarks>This method is automatically called by the textbox's <see cref="WinForms.Control.Validating" /> event if its 
		/// <see cref="WinForms.Control.CausesValidation" /> property is set to true.
		/// It delegates to the overridable version of Validate. </remarks>
		/// <seealso cref="WinForms.Control.Validating" />
		public bool Validate() { return Validate(Flags, false); }

		/// <summary>Checks if the textbox's value is valid and if not proceeds according to the given set of flags. </summary>
		/// <param name="flags">
		/// The combination of zero or more <see cref="ValidatingFlag" /> values added (ORed) together.
		/// This determines if the value should be checked for being empty, invalid, or neither, and then what action to take. </param>
		/// <param name="setFocusIfNotValid">
		/// If true and the validation fails (based on the flags parameter), the focus is placed on the textbox. </param>
		/// <returns>If the validation succeeds, the return value is true; otherwise it is false. </returns>
		/// <remarks>
		/// This method is indirectly called by the textbox's <see cref="WinForms.Control.Validating" /> event if its 
		/// <see cref="WinForms.Control.CausesValidation" /> property is set to true.
		/// Although doing so is not expected, this method may be overriden to provide extra validation in derived classes. </remarks>
		/// <seealso cref="IsValid" />
		/// <seealso cref="WinForms.Control.Validating" />
		public virtual bool Validate(int flags, bool setFocusIfNotValid)
		{
			ShowErrorIcon("");  // clear the icon if it's being shown

			// Check if any of the flags are set
			if ((flags & (int)ValidatingFlag.Max) == 0)
				return true;

			// If we care about the value being empty, check and take the proper action
			if ((flags & (int)ValidatingFlag.MaxIfEmpty) != 0 && this.textBox.Text == "")
			{
				if ((flags & (int)ValidatingFlag.BeepIfEmpty) != 0)
					Windows.MessageBeep(WinForms.MessageBoxIcon.Exclamation);

				if ((flags & (int)ValidatingFlag.SetValidIfEmpty) != 0)
				{
					UpdateText();
					return true;
				}

				if ((flags & (int)ValidatingFlag.ShowIconIfEmpty) != 0)
					ShowErrorIcon(ErrorMessage);

				if ((flags & (int)ValidatingFlag.ShowMessageIfEmpty) != 0)
					ShowErrorMessageBox(ErrorMessage);

				if (setFocusIfNotValid)
					this.textBox.Focus();

				return false;
			}

			// If we care about the value being invalid, check and take the proper action
			if ((flags & (int)ValidatingFlag.MaxIfInvalid) != 0 && this.textBox.Text != "" && !IsValid())
			{
				if ((flags & (int)ValidatingFlag.BeepIfInvalid) != 0)
					Windows.MessageBeep(WinForms.MessageBoxIcon.Exclamation);

				if ((flags & (int)ValidatingFlag.SetValidIfInvalid) != 0)
				{
					UpdateText();
					return true;
				}

				if ((flags & (int)ValidatingFlag.ShowIconIfInvalid) != 0)
					ShowErrorIcon(ErrorMessage);

				if ((flags & (int)ValidatingFlag.ShowMessageIfInvalid) != 0)
					ShowErrorMessageBox(ErrorMessage);

				if (setFocusIfNotValid)
					this.textBox.Focus();

				return false;
			}

			return true;
		}

		/// <summary>Checks if the textbox contains a valid value. </summary>
		/// <returns>If the value is valid, the return value is true; otherwise it is false. </returns>
		/// <remarks>
		/// This method is called by the <see cref="Validate" /> to check validity. Here it just returns true, 
		/// but it is meant to be overridden by Behavior classes where either the allowed range of values is not
		/// controlled as the user types (e.g. NumericBehavior, TimeBehavior), or the value is not considered 
		/// valid until the user has entered all the required characters (e.g. DateBehavior, TimeBehavior). </remarks>
		public virtual bool IsValid() { return true; }

		/// <summary>Attaches several textBox event handlers to their corresponding virtual methods of the Behavior class. </summary>
		/// <remarks>
		/// To alter a textBox's behavior, these events may be needed: KeyDown, KeyPress, TextChanged, Validating, and LostFocus.
		/// This method binds those events to these virtual methods: HandleKeyDown, HandleKeyPress, HandleTextChanged, HandleValidating, and HandleLostFocus.
		/// Derived behavior classes may override any of these methods to accommodate their own requirements. </remarks>
		/// <seealso cref="HandleKeyDown" />
		/// <seealso cref="HandleKeyPress" />
		/// <seealso cref="HandleTextChanged" />
		/// <seealso cref="HandleLostFocus" />
		/// <seealso cref="RemoveEventHandlers" />
		protected virtual void AddEventHandlers()
		{
			this.textBox.KeyDown += new WinForms.KeyEventHandler(HandleKeyDown);
			this.textBox.KeyPress += new WinForms.KeyPressEventHandler(HandleKeyPress);
			this.textBox.TextChanged += new EventHandler(HandleTextChanged);
			this.textBox.Validating += new CancelEventHandler(HandleValidating);
			this.textBox.LostFocus += new EventHandler(HandleLostFocus);
			this.textBox.DataBindings.CollectionChanged += new CollectionChangeEventHandler(HandleBindingChanges);
		}

		/// <summary>Detaches several textBox event handlers from their corresponding virtual methods of the Behavior class. </summary>
		/// <remarks>
		/// This method does the opposite of <see cref="AddEventHandlers" /> and it allows a Behavior object to be associated with
		/// a textBox and later replaced by a different Behavior object. </remarks>
		/// <seealso cref="AddEventHandlers" />
		/// <seealso cref="Dispose" />
		protected virtual void RemoveEventHandlers()
		{
			if (this.textBox == null)
				return;

			this.textBox.KeyDown -= new WinForms.KeyEventHandler(HandleKeyDown);
			this.textBox.KeyPress -= new WinForms.KeyPressEventHandler(HandleKeyPress);
			this.textBox.TextChanged -= new EventHandler(HandleTextChanged);
			this.textBox.Validating -= new CancelEventHandler(HandleValidating);
			this.textBox.LostFocus -= new EventHandler(HandleLostFocus);
			this.textBox.DataBindings.CollectionChanged -= new CollectionChangeEventHandler(HandleBindingChanges);
		}

		/// <summary>
		/// Disposes of the object by detaching the textBox event handlers from their corresponding virtual 
		/// methods of the Behavior class and setting the Textbox to null. </summary>
		/// <seealso cref="RemoveEventHandlers" />
		public virtual void Dispose()
		{
			RemoveEventHandlers();
			this.textBox = null;
		}

		/// <summary>Handles keyboard presses inside the textbox. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior. 
		/// Here it just sets e.Handled to false so that the keydown can happen. </remarks>
		/// <seealso cref="AddEventHandlers" />
		/// <seealso cref="WinForms.Control.KeyDown" />
		protected virtual void HandleKeyDown(object sender, WinForms.KeyEventArgs e)
		{
			TraceLine("Behavior.HandleKeyDown " + e.KeyCode);

			e.Handled = false;
		}

		/// <summary>Handles keyboard presses inside the textbox. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior.
		/// Here it just sets e.Handled to false so that the keypress can happen. </remarks>
		/// <seealso cref="AddEventHandlers" />
		/// <seealso cref="WinForms.Control.KeyPress" />
		protected virtual void HandleKeyPress(object sender, WinForms.KeyPressEventArgs e)
		{
			TraceLine("Behavior.HandleKeyPress " + e.KeyChar);

			e.Handled = false;
		}

		/// <summary>Handles changes in the textbox text. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior.
		/// Here it calls <see cref="UpdateText" /> (unless the internal <see cref="noTextChanged" /> flag is <c>true</c>) 
		/// to ensure the text is kept valid. </remarks>
		/// <seealso cref="AddEventHandlers" />
		/// <seealso cref="WinForms.Control.TextChanged" />
		protected virtual void HandleTextChanged(object sender, EventArgs e)
		{
			TraceLine("Behavior.HandleTextChanged " + this.noTextChanged);

			if (!this.noTextChanged)
				UpdateText();

			this.noTextChanged = false;
		}

		/// <summary>Handles when the control is being validated as a result of losing its focus. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method calls <see cref="Validate" /> to determine if the textbox's value is valid and
		/// the return value is used to set <see cref="CancelEventArgs.Cancel">e.Cancel</see>.  
		/// Although not expected, this method may be overridden by derived classes to accommodate their own behavior. </remarks>
		/// <seealso cref="AddEventHandlers" />
		/// <seealso cref="Validate" />
		/// <seealso cref="WinForms.Control.Validating" />
		protected virtual void HandleValidating(object sender, CancelEventArgs e)
		{
			TraceLine("Behavior.HandleValidating");

			e.Cancel = !Validate();
		}

		/// <summary>Handles when the control has lost its focus. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior.
		/// Here it does nothing. </remarks>
		/// <seealso cref="AddEventHandlers" />
		/// <seealso cref="WinForms.Control.LostFocus" />
		protected virtual void HandleLostFocus(object sender, EventArgs e) { TraceLine("Behavior.HandleLostFocus"); }

		/// <summary>Handles when changes are made to the DataBindings property of the control. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior.
		/// Here it checks if a Binding object has been added to the DataBindings collection
		/// so that its Parse event can be wired to the <see cref="HandleBindingFormat" /> and 
		/// <see cref="HandleBindingParse" />  methods.  </remarks>
		/// <seealso cref="HandleBindingFormat" />
		/// <seealso cref="HandleBindingParse" />
		/// <seealso cref="WinForms.BindingsCollection.CollectionChanged" />
		protected virtual void HandleBindingChanges(object sender, CollectionChangeEventArgs e)
		{
			if (e.Action == CollectionChangeAction.Add)
			{
				WinForms.Binding binding = (WinForms.Binding)e.Element;
				binding.Format += new WinForms.ConvertEventHandler(HandleBindingFormat);
				binding.Parse += new WinForms.ConvertEventHandler(HandleBindingParse);
			}
		}

		/// <summary>
		/// Handles when the value of the object bound to this control needs to be formatted to be 
		/// placed on the control. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior.
		/// Here it does nothing. </remarks>
		/// <seealso cref="HandleBindingChanges" />
		/// <seealso cref="WinForms.Binding.Format" />
		protected virtual void HandleBindingFormat(object sender, WinForms.ConvertEventArgs e) { }

		/// <summary>
		/// Handles when the control's text gets parsed to be converted to the type expected by the 
		/// object that it's bound to. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is virtual so that it can be overridden by derived classes to accommodate their own behavior.
		/// Here it checks if the control's text is empty so that it can set it to DBNull.Value. </remarks>
		/// <seealso cref="HandleBindingChanges" />
		/// <seealso cref="WinForms.Binding.Parse" />
		protected virtual void HandleBindingParse(object sender, WinForms.ConvertEventArgs e)
		{
			if (e.Value.ToString() == "")
				e.Value = DBNull.Value;
		}
	};
	#endregion

	/// <summary>
	/// Values that may be added/removed to the <see cref="Behavior.Flags" /> property related 
	/// to validating the textbox. </summary>
	/// <seealso cref="Behavior.ModifyFlags" />
	/// <seealso cref="Behavior.HasFlag" />
	/// <seealso cref="Behavior.Validate" />
	/// <seealso cref="Behavior.HandleValidating" />
	[Flags]
	public enum ValidatingFlag
	{
		/// <summary> If the value is not valid, make beeping sound. </summary>
		BeepIfInvalid = 0x00000001,

		/// <summary> If the value is empty, make beeping sound. </summary>
		BeepIfEmpty = 0x00000002,

		/// <summary> If the value is empty or not valid, make beeping sound. </summary>
		Beep = BeepIfInvalid | BeepIfEmpty,

		/// <summary> If the value is not valid, change its value to something valid. </summary>
		SetValidIfInvalid = 0x00000004,

		/// <summary> If the value is empty, change its value to something valid. </summary>
		SetValidIfEmpty = 0x00000008,

		/// <summary> If the value is empty or not valid, change its value to something valid. </summary>
		SetValid = SetValidIfInvalid | SetValidIfEmpty,

		/// <summary> If the value is not valid, show an error message box. </summary>
		ShowMessageIfInvalid = 0x00000010,

		/// <summary> If the value is empty, show an error message box. </summary>
		ShowMessageIfEmpty = 0x00000020,

		/// <summary> If the value is empty or not valid, show an error message box. </summary>
		ShowMessage = ShowMessageIfInvalid | ShowMessageIfEmpty,

		/// <summary> If the value is not valid, show a blinking icon next to it. </summary>
		ShowIconIfInvalid = 0x00000040,

		/// <summary> If the value is empty, show a blinking icon next to it. </summary>
		ShowIconIfEmpty = 0x00000080,

		/// <summary> If the value is empty or not valid, show a blinking icon next to it. </summary>
		ShowIcon = ShowIconIfInvalid | ShowIconIfEmpty,

		/// <summary> Combination of all IfInvalid flags (above); used internally by the program. </summary>
		[EditorBrowsable(EditorBrowsableState.Never)]
		MaxIfInvalid = BeepIfInvalid | SetValidIfInvalid | ShowMessageIfInvalid | ShowIconIfInvalid,

		/// <summary> Combination of all IfEmpty flags (above); used internally by the program. </summary>
		[EditorBrowsable(EditorBrowsableState.Never)]
		MaxIfEmpty = BeepIfEmpty | SetValidIfEmpty | ShowMessageIfEmpty | ShowIconIfEmpty,

		/// <summary> Combination of all flags; used internally by the program. </summary>
		[EditorBrowsable(EditorBrowsableState.Never)]
		Max = MaxIfInvalid | MaxIfEmpty
	};

	#region Alphanumeric behavior
	/// <summary>Behavior class which prevents entry of one or more characters. </summary>
	public class AlphanumericBehavior : Behavior
	{
		private char[] invalidChars = { '%', '\'', '*', '"', '+', '?', '>', '<', ':', '\\' };

		/// <summary>Initializes a new instance of the AlphanumericBehavior class by associating it with a TextBox derived object. </summary>
		/// <param name="textBox">The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor sets the invalid characters to <c>%</c>, <c>'</c>, <c>*</c>, <c>"</c>, <c>+</c>, <c>?</c>, <c>></c>, <c>&lt;</c>, <c>:</c>, and <c>\</c>. </remarks>
		/// <seealso cref="InvalidChars" />
		public AlphanumericBehavior(TextBox textBox) : base(textBox, true) {}

		/// <summary>Initializes a new instance of the AlphanumericBehavior class by associating it with a TextBox derived object and setting its invalid characters. </summary>
		/// <param name="textBox">The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <param name="invalidChars">An array of characters that should not be allowed. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		public AlphanumericBehavior(TextBox textBox, char[] invalidChars) : base(textBox, true) { this.invalidChars = invalidChars; }

		/// <summary>Initializes a new instance of the AlphanumericBehavior class by associating it with a TextBox derived object and setting its invalid characters. </summary>
		/// <param name="textBox">The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <param name="invalidChars">The set of characters not allowed, concatenated into a string. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		public AlphanumericBehavior(TextBox textBox, string invalidChars) : base(textBox, true) { this.invalidChars = invalidChars.ToCharArray(); }

		/// <summary>
		/// Initializes a new instance of the AlphanumericBehavior class by copying it from 
		/// another AlphanumericBehavior object. </summary>
		/// <param name="behavior">The AlphanumericBehavior object to copied (and then disposed of).  It must not be null. </param>
		/// <exception cref="ArgumentNullException">behavior is null. </exception>
		/// <remarks>After the behavior.TextBox object is copied, Dispose is called on the behavior parameter. </remarks>
		public AlphanumericBehavior(AlphanumericBehavior behavior) : base(behavior) { this.invalidChars = behavior.invalidChars; }

		/// <summary>Gets or sets the array of characters considered invalid (not allowed). </summary>
		/// <remarks>If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		public char[] InvalidChars
		{
			get { return this.invalidChars; }
			set
			{
				if (this.invalidChars == value)
					return;

				this.invalidChars = value;
				UpdateText();
			}
		}

		/// <summary>Retrieves the textbox's text in valid form. </summary>
		/// <returns>If the textbox's text is valid, it is returned; otherwise a valid version of it is returned. </returns>
		protected override string GetValidText()
		{
			string text = this.textBox.Text;

			// Check if there are any invalid characters and if so, remove them
			if (this.invalidChars != null && text.IndexOfAny(this.invalidChars) >= 0)
			{
				// There are invalid characters -- remove them
				foreach (char c in this.invalidChars)
				{
					if (text.IndexOf(c) >= 0)
						text = text.Replace(c.ToString(), "");
				}
			}

			// Check the max length
			if (text.Length > this.textBox.MaxLength)
				text = text.Remove(this.textBox.MaxLength, text.Length - this.textBox.MaxLength);

			return text;
		}

		/// <summary>Handles keyboard presses inside the textbox. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is overridden from the Behavior class and it  
		/// handles the textbox's KeyPress event. </remarks>
		/// <seealso cref="WinForms.Control.KeyPress" />
		protected override void HandleKeyPress(object sender, WinForms.KeyPressEventArgs e)
		{
			TraceLine("AlphanumericBehavior.HandleKeyPress " + e.KeyChar);

			// Check to see if it's read only
			if (this.textBox.ReadOnly || this.invalidChars == null)
				return;

			char c = e.KeyChar;
			e.Handled = true;

			// Check if the character is invalid				
			if (Array.IndexOf(this.invalidChars, c) >= 0)
			{
				Windows.MessageBeep(WinForms.MessageBoxIcon.Exclamation);
				return;
			}

			// If the number of characters is already at Max, overwrite
			string text = this.textBox.Text;
			if (text.Length == this.textBox.MaxLength && this.textBox.MaxLength > 0 && !Char.IsControl(c))
			{
				int start, end;
				this.selection.Get(out start, out end);

				if (start < this.textBox.MaxLength)
					this.selection.SetAndReplace(start, start + 1, c.ToString());

				return;
			}

			base.HandleKeyPress(sender, e);
		}
	};
	#endregion

	#region Numeric behavior
	/// <summary>
	/// Behavior class which handles numeric input. </summary>
	/// <remarks>
	/// This is the base class for the other numeric behavior classes.  
	/// It ensures that the user enters a valid number and provides features such as automatic formatting. 
	/// It also allows precise control over what the number can look like, such as how many digits to the 
	/// left and right of the decimal point, and whether it can be negative or not. </remarks>
	public class NumericBehavior : Behavior
	{
		private int maxWholeDigits = 9;
		private int maxDecimalPlaces = 7;
		private int digitsInGroup = 0;
		private char negativeSign = NumberFormatInfo.CurrentInfo.NegativeSign[0];
		private char decimalPoint = NumberFormatInfo.CurrentInfo.NumberDecimalSeparator[0];
		private char groupSeparator = NumberFormatInfo.CurrentInfo.NumberGroupSeparator[0];
		private string prefix = "";
		private double rangeMin = Single.MinValue;
		private double rangeMax = Single.MaxValue;

		private int previousSeparatorCount = -1;
		private bool textChangedByKeystroke = false;

		/// <summary>
		/// Internal values that are added/removed to the <see cref="Behavior.Flags" /> property by other
		/// properties of this class. </summary>
		[Flags]
		protected enum Flag
		{
			/// <summary> The value is not allowed to be negative; the user is not allowed to enter a negative sign. </summary>
			CannotBeNegative = 0x00010000,

			/// <summary> If the user enters a digit after the <see cref="MaxWholeDigits" /> have been entered, a <see cref="DecimalPoint" /> is inserted and then the digit. </summary>
			AddDecimalAfterMaxWholeDigits = 0x00020000
		};

		/// <summary>
		/// Values that may be added/removed to the <see cref="Behavior.Flags" /> property related to 
		/// what occurs when the textbox loses focus. </summary>
		/// <seealso cref="Behavior.ModifyFlags" />
		/// <seealso cref="Behavior.HasFlag" />
		[Flags]
		public enum LostFocusFlag
		{
			/// <summary> When the textbox loses focus, pad the value with up to <see cref="MaxWholeDigits" /> zeros left of the decimal symbol. </summary>
			PadWithZerosBeforeDecimal = 0x00000100,

			/// <summary> When the textbox loses focus, pad the value with up to <see cref="MaxDecimalPlaces" /> zeros right of the decimal symbol. </summary>
			PadWithZerosAfterDecimal = 0x00000200,

			/// <summary> When combined with the <see cref="PadWithZerosBeforeDecimal" /> or <see cref="PadWithZerosAfterDecimal" />, the padding is only done if the textbox is not empty. </summary>
			DontPadWithZerosIfEmpty = 0x00000400,

			/// <summary> Insignificant zeros are removed from the numeric value left of the decimal point, unless the number itself is 0. </summary>
			RemoveExtraLeadingZeros = 0x00000800,

			/// <summary> Combination of all the above flags; used internally by the program. </summary>
			[EditorBrowsable(EditorBrowsableState.Never)]
			Max = 0x00000F00,

			/// <summary> If the Text property is set, the LostFocus handler is called. </summary>
			CallHandlerWhenTextPropertyIsSet = 0x00001000,

			/// <summary> If the text changes, the LostFocus handler is called. </summary>
			CallHandlerWhenTextChanges = 0x00002000
		};

		/// <summary>
		/// Initializes a new instance of the NumericBehavior class by associating it with a TextBox derived object. </summary>
		/// <param name="textBox">
		/// The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor sets <see cref="MaxWholeDigits" /> = 9, <see cref="MaxDecimalPlaces" /> = 4, 
		/// <see cref="DigitsInGroup" /> = 0, <see cref="Prefix" /> = "", <see cref="AllowNegative" /> = true, 
		/// and the rest of the properties according to user's system. </remarks>
		public NumericBehavior(TextBox textBox) : base(textBox, true) { AdjustDecimalAndGroupSeparators(); }

		/// <summary>
		/// Initializes a new instance of the NumericBehavior class by associating it with a TextBox derived object
		/// and setting the maximum number of digits allowed left and right of the decimal point. </summary>
		/// <param name="textBox">The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <param name="maxWholeDigits">
		/// The maximum number of digits allowed left of the decimal point.
		/// If it is less than 1, it is set to 1. </param>
		/// <param name="maxDecimalPlaces">
		/// The maximum number of digits allowed right of the decimal point.
		/// If it is less than 0, it is set to 0. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor sets <see cref="DigitsInGroup" /> = 0, <see cref="Prefix" /> = "", <see cref="AllowNegative" /> = true, 
		/// and the rest of the properties according to user's system. </remarks>
		/// <seealso cref="MaxWholeDigits" />
		/// <seealso cref="MaxDecimalPlaces" />
		public NumericBehavior(TextBox textBox, int maxWholeDigits, int maxDecimalPlaces)
			: this(textBox)
		{
			this.maxWholeDigits = maxWholeDigits;
			this.maxDecimalPlaces = maxDecimalPlaces;

			if (this.maxWholeDigits < 1)
				this.maxWholeDigits = 1;
			if (this.maxDecimalPlaces < 0)
				this.maxDecimalPlaces = 0;
		}

		/// <summary>
		/// Initializes a new instance of the NumericBehavior class by associating it with a TextBox derived object
		/// and assiging its attributes from a mask string. </summary>
		/// <param name="textBox">The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <param name="mask">
		/// The string used to set several of the object's properties. 
		/// See <see cref="Mask" /> for more information. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor sets <see cref="AllowNegative" /> = true
		/// and the rest of the properties using the mask. </remarks>
		/// <seealso cref="Mask" />
		public NumericBehavior(TextBox textBox, string mask) : base(textBox, true) { Mask = mask; }

		/// <summary>
		/// Initializes a new instance of the NumericBehavior class by copying it from 
		/// another NumericBehavior object. </summary>
		/// <param name="behavior">
		/// The NumericBehavior object to copied (and then disposed of).  It must not be null. </param>
		/// <exception cref="ArgumentNullException">behavior is null. </exception>
		/// <remarks>
		/// After the behavior.TextBox object is copied, Dispose is called on the behavior parameter. </remarks>
		public NumericBehavior(NumericBehavior behavior)
			: base(behavior)
		{
			this.maxWholeDigits = behavior.maxWholeDigits;
			this.maxDecimalPlaces = behavior.maxDecimalPlaces;
			this.digitsInGroup = behavior.digitsInGroup;
			this.negativeSign = behavior.negativeSign;
			this.decimalPoint = behavior.decimalPoint;
			this.groupSeparator = behavior.groupSeparator;
			this.prefix = behavior.prefix;
			this.rangeMin = behavior.rangeMin;
			this.rangeMax = behavior.rangeMax;
		}

		/// <summary>Gets or sets the maximum number of digits allowed left of the decimal point. </summary>
		/// <remarks>
		/// If this property is set to a number less than 1, it is set to 1. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="MaxDecimalPlaces" />
		public int MaxWholeDigits
		{
			get { return this.maxWholeDigits; }
			set
			{
				if (this.maxWholeDigits == value)
					return;

				this.maxWholeDigits = value;
				if (this.maxWholeDigits < 1)
					this.maxWholeDigits = 1;

				UpdateText();
			}
		}

		/// <summary>Gets or sets the maximum number of digits allowed right of the decimal point. </summary>
		/// <remarks>
		/// If this property is set to a number less than 0, it is set to 0. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="MaxWholeDigits" />
		public int MaxDecimalPlaces
		{
			get { return this.maxDecimalPlaces; }
			set
			{
				if (this.maxDecimalPlaces == value)
					return;

				this.maxDecimalPlaces = value;
				if (this.maxDecimalPlaces < 0)
					this.maxDecimalPlaces = 0;

				UpdateText();
			}
		}

		/// <summary>Gets or sets whether the value is allowed to be negative or not. </summary>
		/// <remarks>
		/// By default, this property is set to true, meaning that negative values are allowed. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="NegativeSign" />
		public bool AllowNegative
		{
			get { return !HasFlag((int)Flag.CannotBeNegative); }
			set { ModifyFlags((int)Flag.CannotBeNegative, !value); }
		}

		/// <summary>
		/// Gets or sets whether a <see cref="DecimalPoint" /> is automatically inserted if the user enters a digit 
		/// after the <see cref="MaxWholeDigits" /> have been entered </summary>
		/// <remarks>
		/// By default, this property is set to false, meaning that when the <see cref="MaxWholeDigits" /> have
		/// been entered, a <see cref="DecimalPoint" /> is not automaticall inserted -- the user has to do it manually. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="NegativeSign" />
		public bool AddDecimalAfterMaxWholeDigits
		{
			get { return HasFlag((int)Flag.AddDecimalAfterMaxWholeDigits); }
			set { ModifyFlags((int)Flag.AddDecimalAfterMaxWholeDigits, value); }
		}

		/// <summary>Gets or sets the number of digits to place in each group to the left of the decimal point. </summary>
		/// <remarks>
		/// By default, this property is set to 0. It may be set to 3 to group thousands using the <see cref="GroupSeparator">group separator</see>. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="GroupSeparator" />
		public int DigitsInGroup
		{
			get { return this.digitsInGroup; }
			set
			{
				if (this.digitsInGroup == value)
					return;

				this.digitsInGroup = value;
				if (this.digitsInGroup < 0)
					this.digitsInGroup = 0;

				UpdateText();
			}
		}

		/// <summary>Gets or sets the character to use for the decimal point. </summary>
		/// <remarks>
		/// By default, this property is set based on the user's system settings. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="GroupSeparator" />
		public char DecimalPoint
		{
			get { return this.decimalPoint; }
			set
			{
				if (this.decimalPoint == value)
					return;

				this.decimalPoint = value;
				AdjustDecimalAndGroupSeparators();
				UpdateText();
			}
		}

		/// <summary>Gets or sets the character to use for the group separator. </summary>
		/// <remarks>
		/// By default, this property is set based on the user's system settings. 
		/// In the US, this is typically a comma and it is used to separate the thousands. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="GroupSeparator" />
		public char GroupSeparator
		{
			get { return this.groupSeparator; }
			set
			{
				if (this.groupSeparator == value)
					return;

				this.groupSeparator = value;
				AdjustDecimalAndGroupSeparators();
				UpdateText();
			}
		}

		/// <summary>Gets or sets the character to use for the negative sign. </summary>
		/// <remarks>
		/// By default, this property is set based on the user's system settings, but it will likely be a minus sign. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		/// <seealso cref="AllowNegative" />
		public char NegativeSign
		{
			get { return this.negativeSign; }
			set
			{
				if (this.negativeSign == value)
					return;

				this.negativeSign = value;
				UpdateText();
			}
		}

		/// <summary>Gets or sets the text to automatically insert in front of the number, such as a currency symbol. </summary>
		/// <remarks>
		/// By default, this property is set to an empty string. 
		/// If this property is changed, <see cref="Behavior.UpdateText" /> is automatically called. </remarks>
		public string Prefix
		{
			get { return this.prefix; }
			set
			{
				if (this.prefix == value)
					return;

				this.prefix = value;
				UpdateText();
			}
		}

		/// <summary>Gets or sets the minimum value allowed. </summary>
		/// <remarks>
		/// By default, this property is set to <see cref="Double.MinValue" />, however the range is 
		/// only checked when the control loses focus if one of the <see cref="ValidatingFlag" /> flags is set. </remarks>	
		/// <seealso cref="RangeMax" />
		public double RangeMin
		{
			get { return this.rangeMin; }
			set { this.rangeMin = value; }
		}

		/// <summary>Gets or sets the maximum value allowed. </summary>
		/// <remarks>
		/// By default, this property is set to <see cref="Double.MaxValue" />, however the range is 
		/// only checked when the control loses focus if one of the <see cref="ValidatingFlag" /> flags is set. </remarks>	
		/// <seealso cref="RangeMin" />
		public double RangeMax
		{
			get { return this.rangeMax; }
			set { this.rangeMax = value; }
		}

		/// <summary>If necessary, adjusts the decimal and group separators so they're not the same. </summary>
		/// <remarks>
		/// If the decimal and group separators are found to be same, they are adjusted to be different. 
		/// This prevents potential problems as the user is entering the value. </remarks>	
		protected void AdjustDecimalAndGroupSeparators()
		{
			if (this.decimalPoint == this.groupSeparator)
				this.groupSeparator = (this.decimalPoint == ',' ? '.' : ',');
		}

		/// <summary>Gets or sets a mask value representative of this object's properties. </summary>
		/// <remarks>
		/// This property can be set to configure these other properties: <see cref="MaxWholeDigits" />,
		/// <see cref="MaxDecimalPlaces" />, <see cref="DigitsInGroup" />, <see cref="Prefix" />, <see cref="DecimalPoint" />, 
		/// and <see cref="GroupSeparator" />.
		/// <para>
		/// For example, <c>"$ #,###.##"</c> means MaxWholeDigits = 4, MaxDecimalPlaces = 2, DigitsInGroup = 3, 
		/// Prefix = "$ ", DecimalPoint = '.', and GroupSeparator = ','. </para>
		/// <para>
		/// The # character is used to denote a digit placeholder. </para> </remarks>
		public string Mask
		{
			get
			{
				StringBuilder mask = new StringBuilder();

				for (int iDigit = 0; iDigit < this.maxWholeDigits; iDigit++)
					mask.Append('0');

				if (this.maxDecimalPlaces > 0)
					mask.Append(this.decimalPoint);

				for (int iDigit = 0; iDigit < this.maxDecimalPlaces; iDigit++)
					mask.Append('0');

				mask = new StringBuilder(GetSeparatedText(mask.ToString()));

				for (int iPos = 0, length = mask.Length; iPos < length; iPos++)
				{
					if (mask[iPos] == '0')
						mask[iPos] = '#';
				}

				return mask.ToString();
			}
			set
			{
				int decimalPos = -1;
				int length = value.Length;

				this.maxWholeDigits = 0;
				this.maxDecimalPlaces = 0;
				this.digitsInGroup = 0;
				this.flags = (this.flags & (int)~Flag.CannotBeNegative);  // allow it to be negative
				this.prefix = "";

				for (int iPos = length - 1; iPos >= 0; iPos--)
				{
					char c = value[iPos];
					if (c == '#')
					{
						if (decimalPos >= 0)
							this.maxWholeDigits++;
						else
							this.maxDecimalPlaces++;
					}
					else if ((c == '.' || c == this.decimalPoint) && decimalPos < 0)
					{
						decimalPos = iPos;
						this.decimalPoint = c;
					}
					else if (c == ',' || c == this.groupSeparator)
					{
						if (this.digitsInGroup == 0)
						{
							this.digitsInGroup = (((decimalPos >= 0) ? decimalPos : length) - iPos) - 1;
							this.groupSeparator = c;
						}
					}
					else
					{
						this.prefix = value.Substring(0, iPos + 1);
						break;
					}
				}

				if (decimalPos < 0)
				{
					this.maxWholeDigits = this.maxDecimalPlaces;
					this.maxDecimalPlaces = 0;
				}

				System.Diagnostics.Debug.Assert(this.maxWholeDigits > 0);	// must have at least one digit on left side of decimal point

				AdjustDecimalAndGroupSeparators();
				UpdateText();
			}
		}

		/// <summary>Copies a string while inserting zeros into it. </summary>
		/// <param name="text">The text to copy with the zeros inserted. </param>
		/// <param name="startIndex">
		/// The zero-based position where the zeros should be inserted. 
		/// If this is less than 0, the zeros are appended. </param>
		/// <param name="count">The number of zeros to insert. </param>
		/// <returns>The return value is a copy of the text with the zeros inserted. </returns>
		protected string InsertZeros(string text, int startIndex, int count)
		{
			if (startIndex < 0 && count > 0)
				startIndex = text.Length;

			StringBuilder result = new StringBuilder(text);
			for (int iZero = 0; iZero < count; iZero++)
				result.Insert(startIndex, '0');

			return result.ToString();
		}

		/// <summary>Checks if the textbox's numeric value is within the allowed range. </summary>
		/// <returns>If the value is within the allowed range, the return value is true; otherwise it is false. </returns>
		/// <seealso cref="RangeMin" />
		/// <seealso cref="RangeMax" />
		public override bool IsValid()
		{
			double value = ToDouble(RealNumericText);
			return (value >= this.rangeMin && value <= this.rangeMax);
		}

		/// <summary>
		/// Gets the error message used to notify the user to enter a valid numeric value 
		/// within the allowed range. </summary>
		/// <seealso cref="IsValid" />
		public override string ErrorMessage
		{
			get
			{
				if (this.rangeMin > double.MinValue && this.rangeMax < double.MaxValue)
					return "Please specify a numeric value between " + this.rangeMin.ToString() + " and " + this.rangeMax.ToString() + ".";
				else if (this.rangeMin > double.MinValue)
					return "Please specify a numeric value greater than or equal to " + this.rangeMin.ToString() + ".";
				else if (this.rangeMax < double.MinValue)
					return "Please specify a numeric value less than or equal to " + this.rangeMax.ToString() + ".";
				return "Please specify a valid numeric value.";
			}
		}

		/// <summary>Adjusts the textbox's value to be within the range of allowed values. </summary>
		protected void AdjustWithinRange()
		{
			// Check if it's already within the range
			if (IsValid())
				return;

			// If it's empty, set it a valid number
			if (this.textBox.Text == "")
				this.textBox.Text = " ";
			else
				UpdateText();

			// Make it fall within the range
			double value = ToDouble(RealNumericText);
			if (value < this.rangeMin)
				this.textBox.Text = this.rangeMin.ToString();
			else if (value > this.rangeMax)
				this.textBox.Text = this.rangeMax.ToString();
		}

		/// <summary>Retrieves the textbox's value without any non-numeric characters. </summary>
		/// <seealso cref="RealNumericText" />
		public string NumericText { get { return GetNumericText(this.textBox.Text, false); } }

		/// <summary>
		/// Retrieves the textbox's value without any non-numeric characters,
		/// and with a period for the decimal point and a minus for the negative sign. </summary>
		/// <seealso cref="NumericText" />
		public string RealNumericText { get { return GetNumericText(this.textBox.Text, true); } }

		/// <summary>Copies a string while removing any non-numeric characters from it. </summary>
		/// <param name="text">The text to parse and copy. </param>
		/// <param name="realNumeric">
		/// If true, the value is returned as a real number 
		/// (with a period for the decimal point and a minus for the negative sign);
		/// otherwise, it is returned using the expected symbols. </param>
		/// <returns>The return value is a copy of the original text containing only numeric characters. </returns>
		protected string GetNumericText(string text, bool realNumeric)
		{
			StringBuilder numericText = new StringBuilder();
			bool isNegative = false;
			bool hasDecimalPoint = false;

			foreach (char c in text)
			{
				if (Char.IsDigit(c))
					numericText.Append(c);
				else if (c == this.negativeSign)
					isNegative = true;
				else if (c == this.decimalPoint && !hasDecimalPoint)
				{
					hasDecimalPoint = true;
					numericText.Append(realNumeric ? '.' : this.decimalPoint);
				}
			}

			// Add the negative sign to the front of the number.
			if (isNegative)
				numericText.Insert(0, realNumeric ? '-' : this.negativeSign);

			return numericText.ToString();
		}

		/// <summary>Returns the number of group separator characters in the given text. </summary>
		private int GetGroupSeparatorCount(string text)
		{
			int count = 0;
			foreach (char c in text)
				if (c == this.groupSeparator) count++;

			return count;
		}

		/// <summary>Retrieves the textbox's text in valid form. </summary>
		/// <returns>If the textbox's text is valid, it is returned; otherwise a valid version of it is returned. </returns>
		protected override string GetValidText()
		{
			string text = this.textBox.Text;
			StringBuilder newText = new StringBuilder();
			bool isNegative = false;

			// Remove any invalid characters from the number
			for (int iPos = 0, decimalPos = -1, newLength = 0, length = text.Length; iPos < length; iPos++)
			{
				char c = text[iPos];

				// Check for a negative sign
				if (c == this.negativeSign && AllowNegative)
					isNegative = true;

				// Check for a digit
				else if (Char.IsDigit(c))
				{
					// Make sure it doesn't go beyond the limits
					if (decimalPos < 0 && newLength == this.maxWholeDigits)
						continue;

					if (decimalPos >= 0 && newLength > decimalPos + this.maxDecimalPlaces)
						break;

					newText.Append(c);
					newLength++;
				}

				// Check for a decimal point
				else if (c == this.decimalPoint && decimalPos < 0)
				{
					if (this.maxDecimalPlaces == 0)
						break;

					newText.Append(c);
					decimalPos = newLength;
					newLength++;
				}
			}

			// Insert the negative sign if it's there
			if (isNegative)
				newText.Insert(0, this.negativeSign);

			return GetSeparatedText(newText.ToString());
		}

		/// <summary>
		/// Takes a piece of text containing a numeric value and inserts
		/// group separators in the proper places. </summary>
		/// <param name="text">The text to parse. </param>
		/// <returns>The return value is a copy of the original text with the group separators inserted. </returns>
		protected string GetSeparatedText(string text)
		{
			string numericText = GetNumericText(text, false);
			string separatedText = numericText;

			// Retrieve the number without the decimal point
			int decimalPos = numericText.IndexOf(this.decimalPoint);
			if (decimalPos >= 0)
				separatedText = separatedText.Substring(0, decimalPos);

			if (this.digitsInGroup > 0)
			{
				int length = separatedText.Length;
				bool isNegative = (separatedText != "" && separatedText[0] == this.negativeSign);

				// Loop in reverse and stick the separator every this.digitsInGroup digits.
				for (int iPos = length - (this.digitsInGroup + 1); iPos >= (isNegative ? 1 : 0); iPos -= this.digitsInGroup)
					separatedText = separatedText.Substring(0, iPos + 1) + this.groupSeparator + separatedText.Substring(iPos + 1);
			}

			// Prepend the prefix, if the number is not empty.
			if (separatedText != "" || decimalPos >= 0)
			{
				separatedText = this.prefix + separatedText;

				if (decimalPos >= 0)
					separatedText += numericText.Substring(decimalPos);
			}

			return separatedText;
		}

		/// <summary>Handles keyboard presses inside the textbox. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is overridden from the Behavior class and it  
		/// handles the textbox's KeyDown event. </remarks>
		/// <seealso cref="WinForms.Control.KeyDown" />
		protected override void HandleKeyDown(object sender, WinForms.KeyEventArgs e)
		{
			TraceLine("NumericBehavior.HandleKeyDown " + e.KeyCode);

			if (e.KeyCode == WinForms.Keys.Delete)
			{
				int start, end;
				this.selection.Get(out start, out end);

				string text = this.textBox.Text;
				int length = text.Length;

				// If deleting the prefix, don't allow it if there's a number after it.
				int prefixLength = this.prefix.Length;
				if (start < prefixLength && length > prefixLength)
				{
					if (end != length)
						e.Handled = true;
					return;
				}

				this.textChangedByKeystroke = true;

				// If deleting a group separator (comma), move the cursor to the right
				if (start < length && text[start] == this.groupSeparator && start == end)
					WinForms.SendKeys.SendWait("{RIGHT}");

				this.previousSeparatorCount = GetGroupSeparatorCount(text);

				// If everything on the right was deleted, put the selection on the right
				if (end == length)
					WinForms.SendKeys.Send("{RIGHT}");
			}
		}

		/// <summary>Handles keyboard presses inside the textbox. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is overridden from the Behavior class and it  
		/// handles the textbox's KeyPress event. </remarks>
		/// <seealso cref="WinForms.Control.KeyPress" />
		protected override void HandleKeyPress(object sender, WinForms.KeyPressEventArgs e)
		{
			TraceLine("NumericBehavior.HandleKeyPress " + e.KeyChar);

			// Check to see if it's read only
			if (this.textBox.ReadOnly)
				return;

			char c = e.KeyChar;
			e.Handled = true;
			this.textChangedByKeystroke = true;

			int start, end;
			this.selection.Get(out start, out end);

			string text = this.textBox.Text;
			this.previousSeparatorCount = -1;

			string numericText = NumericText;
			int decimalPos = text.IndexOf(this.decimalPoint);
			int numericDecimalPos = numericText.IndexOf(this.decimalPoint);
			int length = text.Length;
			int numericLen = numericText.Length;
			int prefixLength = this.prefix.Length;
			int separatorCount = GetGroupSeparatorCount(text);

			// Check if we're in the prefix's location
			if (start < prefixLength && !Char.IsControl(c))
			{
				char cPrefix = this.prefix[start];

				// Check if it's the same character as the prefix.
				if (cPrefix == c)
				{
					if (length > start)
					{
						end = (end == length ? end : (start + 1));
						this.selection.SetAndReplace(start, end, c.ToString());
					}
					else
						base.HandleKeyPress(sender, e);
				}
				// If it's a part of the number, enter the prefix
				else if (Char.IsDigit(c) || c == this.negativeSign || c == this.decimalPoint)
				{
					end = (end == length ? end : prefixLength);
					this.selection.SetAndReplace(start, end, this.prefix.Substring(start));
					HandleKeyPress(sender, e);
				}

				return;
			}

			// Check if it's a negative sign
			if (c == this.negativeSign && AllowNegative)
			{
				// If it's at the beginning, determine if it should overwritten
				if (start == prefixLength)
				{
					if (numericText != "" && numericText[0] == this.negativeSign)
					{
						end = (end == length ? end : (start + 1));
						this.selection.SetAndReplace(start, end, this.negativeSign.ToString());
						return;
					}
				}
				// If we're not at the beginning, toggle the sign
				else
				{
					if (numericText[0] == this.negativeSign)
					{
						this.selection.SetAndReplace(prefixLength, prefixLength + 1, "");
						this.selection.Set(start - 1, end - 1);
					}
					else
					{
						this.selection.SetAndReplace(prefixLength, prefixLength, this.negativeSign.ToString());
						this.selection.Set(start + 1, end + 1);
					}

					return;
				}
			}

			// Check if it's a decimal point (only one is allowed).
			else if (c == this.decimalPoint && this.maxDecimalPlaces > 0)
			{
				if (decimalPos >= 0)
				{
					// Check if we're replacing the decimal point
					if (decimalPos >= start && decimalPos < end)
						this.previousSeparatorCount = separatorCount;
					else
					{	// Otherwise, put the caret on it
						this.selection.Set(decimalPos + 1, decimalPos + 1);
						return;
					}
				}
				else
					this.previousSeparatorCount = separatorCount;
			}

			// Check if it's a digit
			else if (Char.IsDigit(c))
			{
				// Check if we're on the right of the decimal point.
				if (decimalPos >= 0 && decimalPos < start)
				{
					if (numericText.Substring(numericDecimalPos + 1).Length == this.maxDecimalPlaces)
					{
						if (start <= decimalPos + this.maxDecimalPlaces)
						{
							end = (end == length ? end : (start + 1));
							this.selection.SetAndReplace(start, end, c.ToString());
						}
						return;
					}
				}

				// We're on the left side of the decimal point
				else
				{
					bool isNegative = (numericText.Length != 0 && numericText[0] == this.negativeSign);

					// Make sure we can still enter digits.
					if (start == this.maxWholeDigits + separatorCount + prefixLength + (isNegative ? 1 : 0))
					{
						if (AddDecimalAfterMaxWholeDigits && this.maxDecimalPlaces > 0)
						{
							end = (end == length ? end : (start + 2));
							this.selection.SetAndReplace(start, end, this.decimalPoint.ToString() + c);
						}

						return;
					}

					if (numericText.Substring(0, numericDecimalPos >= 0 ? numericDecimalPos : numericLen).Length == this.maxWholeDigits + (isNegative ? 1 : 0))
					{
						if (text[start] == this.groupSeparator)
							start++;

						end = (end == length ? end : (start + 1));
						this.selection.SetAndReplace(start, end, c.ToString());
						return;
					}

					this.previousSeparatorCount = separatorCount;
				}
			}

			// Check if it's a non-printable character, such as Backspace or Ctrl+C
			else if (Char.IsControl(c))
				this.previousSeparatorCount = separatorCount;
			else
				return;

			base.HandleKeyPress(sender, e);
		}

		/// <summary>Handles changes in the textbox text. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is overridden from the Behavior class and it  
		/// handles the textbox's TextChanged event. 
		/// Here it is used to adjust the selection if new separators have been added or removed. </remarks>
		/// <seealso cref="WinForms.Control.TextChanged" />
		// Fires the TextChanged event if the text is valid.
		protected override void HandleTextChanged(object sender, EventArgs e)
		{
			TraceLine("NumericBehavior.HandleTextChanged");

			Selection.Saver savedSelection = new Selection.Saver(this.textBox);  // save the selection before the text changes
			bool textChangedByKeystroke = this.textChangedByKeystroke;
			base.HandleTextChanged(sender, e);

			// Check if the user has changed the number enough to cause
			// one or more separators to be added/removed, in which case
			// the selection may need to be adjusted.
			if (this.previousSeparatorCount >= 0)
			{
				using (savedSelection)
				{
					int newSeparatorCount = GetGroupSeparatorCount(this.textBox.Text);
					if (this.previousSeparatorCount != newSeparatorCount && savedSelection.Start > this.prefix.Length)
						savedSelection.MoveBy(newSeparatorCount - this.previousSeparatorCount);
				}
			}

			// If the text wasn't changed by a keystroke and the UseLostFocusFlagsWhenTextPropertyIsSet flag is set,
			// call the LostFocus handler to adjust the value according to whatever LostFocus flags are set.
			if (HasFlag((int)LostFocusFlag.CallHandlerWhenTextChanges) ||
			   (!textChangedByKeystroke && HasFlag((int)LostFocusFlag.CallHandlerWhenTextPropertyIsSet)))
				HandleLostFocus(sender, e);

			this.textChangedByKeystroke = false;
		}

		/// <summary>Handles when the control has lost its focus. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method is overridden from the Behavior class and it  
		/// handles the textbox's LostFocus event. 
		/// Here it checks the value's against the allowed range and adds any missing zeros. </remarks>
		/// <seealso cref="WinForms.Control.LostFocus" />
		protected override void HandleLostFocus(object sender, EventArgs e)
		{
			TraceLine("NumericBehavior.HandleLostFocus");

			if (!HasFlag((int)LostFocusFlag.Max))
				return;

			string originalText = GetNumericText(this.textBox.Text, true);
			string text = originalText;
			int length = text.Length;

			// If desired, remove any extra leading zeros but always leave one in front of the decimal point
			if (HasFlag((int)LostFocusFlag.RemoveExtraLeadingZeros) && length > 0)
			{
				bool isNegative = (text[0] == this.negativeSign);
				if (isNegative)
					text = text.Substring(1);
				text = text.TrimStart('0');
				if (text == "" || text[0] == this.decimalPoint)
					text = '0' + text;
				if (isNegative)
					text = this.negativeSign + text;
			}
			// Check if the value is empty and we don't want to touch it
			else if (length == 0 && HasFlag((int)LostFocusFlag.DontPadWithZerosIfEmpty))
				return;

			int decimalPos = text.IndexOf('.');
			int maxDecimalPlaces = this.maxDecimalPlaces;
			int maxWholeDigits = this.maxWholeDigits;

			// Check if we need to pad the number with zeros after the decimal point
			if (HasFlag((int)LostFocusFlag.PadWithZerosAfterDecimal) && maxDecimalPlaces > 0)
			{
				if (decimalPos < 0)
				{
					if (length == 0 || text == "-")
					{
						text = "0";
						length = 1;
					}
					text += '.';
					decimalPos = length++;
				}

				text = InsertZeros(text, -1, maxDecimalPlaces - (length - decimalPos - 1));
			}

			// Check if we need to pad the number with zeros before the decimal point
			if (HasFlag((int)LostFocusFlag.PadWithZerosBeforeDecimal) && maxWholeDigits > 0)
			{
				if (decimalPos < 0)
					decimalPos = length;

				if (length > 0 && text[0] == '-')
					decimalPos--;

				text = InsertZeros(text, (length > 0 ? (text[0] == '-' ? 1 : 0) : -1), maxWholeDigits - decimalPos);
			}

			if (text != originalText)
			{
				if (decimalPos >= 0 && this.decimalPoint != '.')
					text = text.Replace('.', this.decimalPoint);

				// remember the current selection 
				using (Selection.Saver savedSelection = new Selection.Saver(this.textBox))
				{
					this.textBox.Text = text;
				}
			}
		}

		/// <summary>
		/// Handles when the control's text gets parsed to be converted to the type expected by the 
		/// object that it's bound to. </summary>
		/// <param name="sender">The object who sent the event. </param>
		/// <param name="e">The event data. </param>
		/// <remarks>
		/// This method checks if the control's text is empty and if so sets the value to DBNull.Value;
		/// otherwise it converts it to a simple numeric value (without any prefix). </remarks>
		/// <seealso cref="Behavior.HandleBindingChanges" />
		/// <seealso cref="WinForms.Binding.Parse" />
		protected override void HandleBindingParse(object sender, WinForms.ConvertEventArgs e)
		{
			if (e.Value.ToString() == "")
				e.Value = DBNull.Value;
			else
				e.Value = GetNumericText(e.Value.ToString(), false);
		}
	};
	#endregion

	#region Integer behavior
	/// <summary>
	/// Behavior class which only allows integer values to be entered. </summary>
	/// <remarks>This is just a <see cref="NumericBehavior" /> class which maintains</remarks>
	public class IntegerBehavior : NumericBehavior
	{
		/// <summary>Initializes a new instance of the IntegerBehavior class by associating it with a TextBox derived object. </summary>
		/// <param name="textBox">The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor sets <see cref="NumericBehavior.MaxWholeDigits" /> = 9, <see cref="NumericBehavior.MaxDecimalPlaces" /> = 0, <see cref="NumericBehavior.DigitsInGroup" /> = 0, 
		/// <see cref="NumericBehavior.Prefix" /> = "", <see cref="NumericBehavior.AllowNegative" /> = true, and the rest of the properties according to user's system. </remarks>
		public IntegerBehavior(TextBox textBox) : base(textBox, 9, 0) { SetDefaultRange(); }

		/// <summary>
		/// Initializes a new instance of the IntegerBehavior class by associating it with a TextBox derived object. </summary>
		/// <param name="textBox">
		/// The TextBox object to associate with this behavior.  It must not be null. </param>
		/// <param name="maxWholeDigits">
		/// The maximum number of digits allowed to the left of the decimal point. </param>
		/// <exception cref="ArgumentNullException">textBox is null. </exception>
		/// <remarks>
		/// This constructor sets <see cref="NumericBehavior.MaxDecimalPlaces" /> = 0, <see cref="NumericBehavior.DigitsInGroup" /> = 0, <see cref="NumericBehavior.Prefix" /> = "", <see cref="NumericBehavior.AllowNegative" /> = true, 
		/// and the rest of the properties according to user's system. </remarks>
		public IntegerBehavior(TextBox textBox, int maxWholeDigits) : base(textBox, maxWholeDigits, 0) { SetDefaultRange(); }

		/// <summary>
		/// Initializes a new instance of the IntegerBehavior class by copying it from 
		/// another IntegerBehavior object. </summary>
		/// <param name="behavior">
		/// The IntegerBehavior object to copied (and then disposed of).  It must not be null. </param>
		/// <exception cref="ArgumentNullException">behavior is null. </exception>
		/// <remarks>
		/// After the behavior.TextBox object is copied, Dispose is called on the behavior parameter. </remarks>
		public IntegerBehavior(IntegerBehavior behavior) : base(behavior) { SetDefaultRange(); }

		/// <summary>Changes the default min and max values to 32-bit integer ranges.</summary>
		private void SetDefaultRange()
		{
			RangeMin = Int32.MinValue;
			RangeMax = Int32.MaxValue;
		}

		/// <summary>Gets the maximum number of digits allowed right of the decimal point, which is always 0. </summary>
		/// <seealso cref="NumericBehavior.MaxWholeDigits" />
		public new int MaxDecimalPlaces { get { return base.MaxDecimalPlaces; } }

		/// <summary>Gets or sets a mask value representative of this object's properties.
		/// </summary>
		/// <remarks>
		/// This property behaves like <see cref="NumericBehavior.Mask" /> except that  
		/// <see cref="NumericBehavior.MaxDecimalPlaces" /> is maintained with a value of 0.
		/// </remarks>
		public new string Mask
		{
			get { return base.Mask; }
			set
			{
				base.Mask = value;
				if (base.MaxDecimalPlaces > 0)
					base.MaxDecimalPlaces = 0;
			}
		}
	};
	#endregion
}