// Code Adopted from: http://www.codeproject.com/KB/edit/ValidatingTextBoxControls.aspx

using System;
using System.ComponentModel;

namespace BlamLib.Forms
{
	#region TextBox
	/// <summary>
	/// Base class for all TextBox classes in this namespace. 
	/// It holds a Behavior object that may be associated to it by a derived class. </summary>
	/// <seealso cref="AlphanumericTextBox" />
	/// <seealso cref="NumericTextBox" />
	/// <seealso cref="IntegerTextBox" />
	[Browsable(false)]
	//[Designer(typeof(TextBox.Designer))]	
	public abstract class TextBox : System.Windows.Forms.TextBox//DevComponents.DotNetBar.Controls.TextBoxX
	{
		/// <summary> The Behavior object associated with this TextBox. </summary>
		protected Behavior behavior = null; 	// must be initialized by derived classes

		#region Constructors
		/// <summary>
		/// Initializes a new instance of the TextBox class. </summary>
		/// <remarks>
		/// This constructor is just for convenience for derived classes.  It does nothing. </remarks>
		protected TextBox() { }

		/// <summary>
		/// Initializes a new instance of the TextBox class by explicitly assigning its Behavior field. </summary>
		/// <param name="behavior">
		/// The <see cref="Behavior" /> object to associate the textbox with. </param>
		/// <remarks>
		/// This constructor provides a way for derived classes to set the internal Behavior object
		/// to something other than the default value (such as <c>null</c>). </remarks>
		protected TextBox(Behavior behavior) { this.behavior = behavior; }
		#endregion

		/// <summary>
		/// Checks if the textbox's text is valid and if not updates it with a valid value. </summary>
		/// <returns>
		/// If the textbox's text is updated (because it wasn't valid), the return value is true; otherwise it is false. </returns>
		/// <remarks>
		/// This method delegates to <see cref="Behavior.UpdateText">Behavior.UpdateText</see>. </remarks>
		public bool UpdateText() { return behavior.UpdateText(); }

		#region Flags
		/// <summary>
		/// Gets or sets the flags associated with self's Behavior. </summary>
		/// <remarks>
		/// This property delegates to <see cref="Behavior.Flags">Behavior.Flags</see>. </remarks>
		/// <seealso cref="ModifyFlags" />
		[Category("Behavior")]
		[Description("The flags (on/off attributes) associated with the Behavior.")]
		public int Flags
		{
			get { return behavior.Flags; }
			set { behavior.Flags = value; }
		}

		/// <summary>
		/// Adds or removes flags from self's Behavior. </summary>
		/// <param name="flags">
		/// The bits to be turned on (ORed) or turned off in the internal flags member. </param>
		/// <param name="addOrRemove">
		/// If true the flags are added, otherwise they're removed. </param>
		/// <remarks>
		/// This method delegates to <see cref="Behavior.ModifyFlags">Behavior.ModifyFlags</see>. </remarks>
		/// <seealso cref="Flags" />
		public void ModifyFlags(int flags, bool addOrRemove) { behavior.ModifyFlags(flags, addOrRemove); }
		#endregion

		#region Validatation
		/// <summary>
		/// Checks if the textbox's value is valid and if not proceeds according to the behavior's <see cref="Flags" />. </summary>
		/// <returns>
		/// If the validation succeeds, the return value is true; otherwise it is false. </returns>
		/// <remarks>
		/// This method delegates to <see cref="Behavior.Validate">Behavior.Validate</see>. </remarks>
		/// <seealso cref="IsValid" />
		public bool Validate() { return behavior.Validate(); }

		/// <summary>
		/// Checks if the textbox contains a valid value. </summary>
		/// <returns>
		/// If the value is valid, the return value is true; otherwise it is false. </returns>
		/// <remarks>
		/// This method delegates to <see cref="Behavior.IsValid">Behavior.IsValid</see>. </remarks>
		/// <seealso cref="Validate" />
		public bool IsValid() { return behavior.IsValid(); }
		#endregion

		#region Errors
		/// <summary>
		/// Show an error message box. </summary>
		/// <param name="message">
		/// The message to show. </param>
		/// <remarks>
		/// This property delegates to <see cref="Behavior.ShowErrorMessageBox">Behavior.ShowErrorMessageBox</see>. </remarks>
		/// <seealso cref="ShowErrorIcon" />
		/// <seealso cref="ErrorMessage" />
		public void ShowErrorMessageBox(string message) { behavior.ShowErrorMessageBox(message); }

		/// <summary>
		/// Show a blinking icon next to the textbox with an error message. </summary>
		/// <param name="message">
		/// The message to show when the cursor is placed over the icon. </param>
		/// <remarks>
		/// This property delegates to <see cref="Behavior.ShowErrorIcon">Behavior.ShowErrorIcon</see>. </remarks>
		/// <seealso cref="ShowErrorMessageBox" />
		/// <seealso cref="ErrorMessage" />
		public void ShowErrorIcon(string message) { behavior.ShowErrorIcon(message); }

		/// <summary>
		/// Gets the error message used to notify the user to enter a valid value. </summary>
		/// <remarks>
		/// This property delegates to <see cref="Behavior.ErrorMessage">Behavior.ErrorMessage</see>. </remarks>
		/// <seealso cref="Validate" />
		/// <seealso cref="IsValid" />
		[Browsable(false)]
		public string ErrorMessage { get { return behavior.ErrorMessage; } }
		#endregion
	};
	#endregion

	#region Alphanumeric textbox
	/// <summary>
	///   TextBox class which supports the <see cref="AlphanumericBehavior">Alphanumeric</see> behavior. </summary>	
	[Description("TextBox control which supports the Alphanumeric behavior.")]
	public class AlphanumericTextBox : TextBox
	{
		#region Constructors
		/// <summary>
		/// Initializes a new instance of the AlphanumericTextBox class by assigning its Behavior field
		/// to an instance of <see cref="AlphanumericBehavior" />. </summary>
		public AlphanumericTextBox() { behavior = new AlphanumericBehavior(this); }

		/// <summary>
		/// Initializes a new instance of the AlphanumericTextBox class by assigning its Behavior field
		/// to an instance of <see cref="AlphanumericBehavior" /> and passing it the characters to consider invalid. </summary>
		/// <param name="invalidChars">
		/// An array of characters that should not be allowed. </param>
		public AlphanumericTextBox(char[] invalidChars) { behavior = new AlphanumericBehavior(this, invalidChars); }

		/// <summary>
		/// Initializes a new instance of the AlphanumericTextBox class by assigning its Behavior field
		/// to an instance of <see cref="AlphanumericBehavior" /> and passing it string of characters to consider invalid. </summary>
		/// <param name="invalidChars">
		/// The set of characters not allowed, concatenated into a string. </param>
		public AlphanumericTextBox(string invalidChars) { behavior = new AlphanumericBehavior(this, invalidChars); }

		/// <summary>
		/// Initializes a new instance of the AlphanumericTextBox class by explicitly assigning its Behavior field. </summary>
		/// <param name="behavior">
		/// The <see cref="AlphanumericBehavior" /> object to associate the textbox with. </param>
		public AlphanumericTextBox(AlphanumericBehavior behavior) : base(behavior) { }
		#endregion

		#region Behavior
		/// <summary>
		/// Gets the Behavior object associated with this class. </summary>
		[Browsable(false)]
		public AlphanumericBehavior Behavior { get { return behavior as AlphanumericBehavior; } }
		#endregion

		#region InvalidChars
		/// <summary>
		/// Gets or sets the array of characters considered invalid (not allowed). </summary>
		/// <remarks>
		/// This property delegates to <see cref="AlphanumericBehavior.InvalidChars">AlphanumericBehavior.InvalidChars</see>. </remarks>
		/// <seealso cref="AlphanumericBehavior.InvalidChars" />
		[Category("Behavior")]
		[Description("The array of characters considered invalid (not allowed).")]
		public char[] InvalidChars
		{
			get { return Behavior.InvalidChars; }
			set { Behavior.InvalidChars = value; }
		}
		#endregion
	};
	#endregion

	#region Numeric textbox
	/// <summary>
	/// TextBox class which supports the <see cref="NumericBehavior">Numeric</see> behavior. </summary>	
	[Description("TextBox control which supports the Numeric behavior.")]
	//[Designer(typeof(NumericTextBox.Designer))]	
	public class NumericTextBox : TextBox
	{
		#region Constructors
		/// <summary>
		/// Initializes a new instance of the NumericTextBox class by assigning its Behavior field
		/// to an instance of <see cref="NumericBehavior" />. </summary>
		public NumericTextBox() { behavior = new NumericBehavior(this); }

		/// <summary>
		/// Initializes a new instance of the NumericTextBox class by assigning its Behavior field 
		/// to an instance of <see cref="NumericBehavior" /> and setting the maximum number of digits 
		/// allowed left and right of the decimal point. </summary>
		/// <param name="maxWholeDigits">
		/// The maximum number of digits allowed left of the decimal point.
		/// If it is less than 1, it is set to 1.</param>
		/// <param name="maxDecimalPlaces">
		/// The maximum number of digits allowed right of the decimal point.
		/// If it is less than 0, it is set to 0. </param>
		public NumericTextBox(int maxWholeDigits, int maxDecimalPlaces) { behavior = new NumericBehavior(this, maxWholeDigits, maxDecimalPlaces); }

		/// <summary>
		/// Initializes a new instance of the NumericTextBox class by explicitly assigning its Behavior field. </summary>
		/// <param name="behavior">
		/// The <see cref="NumericBehavior" /> object to associate the textbox with. </param>
		public NumericTextBox(NumericBehavior behavior) : base(behavior) { }
		#endregion

		#region Behavior
		/// <summary>
		/// Gets the Behavior object associated with this class. </summary>
		[Browsable(false)]
		public NumericBehavior Behavior { get { return behavior as NumericBehavior; } }
		#endregion

		#region Double
		/// <summary>
		/// Gets or sets the textbox's Text as a double. </summary>
		/// <remarks>
		/// If the text is empty or cannot be converted to a double, a 0 is returned. </remarks>
		/// <seealso cref="Long" />
		/// <seealso cref="Int" />
		[Browsable(false)]
		public double Double
		{
			get
			{
				try { return Convert.ToDouble(Behavior.NumericText); }
				catch { return 0; }
			}
			set { Text = value.ToString(); }
		}
		#endregion

		#region Int
		/// <summary>
		/// Gets or sets the textbox's Text as an int. </summary>
		/// <remarks>
		/// If the text empty or cannot be converted to an int, a 0 is returned. </remarks>
		/// <seealso cref="Long" />
		/// <seealso cref="Double" />
		[Browsable(false)]
		public int Int
		{
			get
			{
				try { return Convert.ToInt32(Behavior.NumericText); }
				catch { return 0; }
			}
			set { Text = value.ToString(); }
		}
		#endregion

		#region Long
		/// <summary>
		/// Gets or sets the textbox's Text as a long. </summary>
		/// <remarks>
		/// If the text empty or cannot be converted to an long, a 0 is returned. </remarks>
		/// <seealso cref="Int" />
		/// <seealso cref="Double" />
		[Browsable(false)]
		public long Long
		{
			get
			{
				try { return Convert.ToInt64(Behavior.NumericText); }
				catch { return 0; }
			}
			set { Text = value.ToString(); }
		}
		#endregion

		#region NumericText
		/// <summary>
		/// Retrieves the textbox's value without any non-numeric characters. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.NumericText">NumericBehavior.NumericText</see>. </remarks>
		[Browsable(false)]
		public string NumericText { get { return Behavior.NumericText; } }
		#endregion

		#region RealNumericText
		/// <summary>
		/// Retrieves the textbox's value without any non-numeric characters,
		/// and with a period for the decimal point and a minus for the negative sign. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.RealNumericText">NumericBehavior.RealNumericText</see>. </remarks>
		[Browsable(false)]
		public string RealNumericText { get { return Behavior.RealNumericText; } }
		#endregion

		#region MaxWholeDigits
		/// <summary>
		/// Gets or sets the maximum number of digits allowed left of the decimal point. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.MaxWholeDigits">NumericBehavior.MaxWholeDigits</see>. </remarks>
		/// <seealso cref="NumericBehavior.MaxWholeDigits" />
		[Category("Behavior")]
		[Description("The maximum number of digits allowed left of the decimal point.")]
		public int MaxWholeDigits
		{
			get { return Behavior.MaxWholeDigits; }
			set { Behavior.MaxWholeDigits = value; }
		}
		#endregion

		#region MaxDecimalPlaces
		/// <summary>
		/// Gets or sets the maximum number of digits allowed right of the decimal point. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.MaxDecimalPlaces">NumericBehavior.MaxDecimalPlaces</see>. </remarks>
		/// <seealso cref="NumericBehavior.MaxDecimalPlaces" />
		[Category("Behavior")]
		[Description("The maximum number of digits allowed right of the decimal point.")]
		public int MaxDecimalPlaces
		{
			get { return Behavior.MaxDecimalPlaces; }
			set { Behavior.MaxDecimalPlaces = value; }
		}
		#endregion

		#region AllowNegative
		/// <summary>
		/// Gets or sets whether the value is allowed to be negative or not. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.AllowNegative">NumericBehavior.AllowNegative</see>. </remarks>
		/// <seealso cref="NumericBehavior.AllowNegative" />
		[Category("Behavior")]
		[Description("Determines whether the value is allowed to be negative or not.")]
		public bool AllowNegative
		{
			get { return Behavior.AllowNegative; }
			set { Behavior.AllowNegative = value; }
		}
		#endregion

		#region DigitsInGroup
		/// <summary>
		/// Gets or sets the number of digits to place in each group to the left of the decimal point. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.DigitsInGroup">NumericBehavior.DigitsInGroup</see>. </remarks>
		/// <seealso cref="NumericBehavior.DigitsInGroup" />
		[Category("Behavior")]
		[Description("The number of digits to place in each group to the left of the decimal point.")]
		public int DigitsInGroup
		{
			get { return Behavior.DigitsInGroup; }
			set { Behavior.DigitsInGroup = value; }
		}
		#endregion

		#region DecimalPoint
		/// <summary>
		/// Gets or sets the character to use for the decimal point. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.DecimalPoint">NumericBehavior.DecimalPoint</see>. </remarks>
		/// <seealso cref="NumericBehavior.DecimalPoint" />
		[Browsable(false)]
		public char DecimalPoint
		{
			get { return Behavior.DecimalPoint; }
			set { Behavior.DecimalPoint = value; }
		}
		#endregion

		#region GroupSeparator
		/// <summary>
		/// Gets or sets the character to use for the group separator. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.GroupSeparator">NumericBehavior.GroupSeparator</see>. </remarks>
		/// <seealso cref="NumericBehavior.GroupSeparator" />
		[Browsable(false)]
		public char GroupSeparator
		{
			get { return Behavior.GroupSeparator; }
			set { Behavior.GroupSeparator = value; }
		}
		#endregion

		#region NegativeSign
		/// <summary>
		/// Gets or sets the character to use for the negative sign. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.NegativeSign">NumericBehavior.NegativeSign</see>. </remarks>
		/// <seealso cref="NumericBehavior.NegativeSign" />
		[Browsable(false)]
		public char NegativeSign
		{
			get { return Behavior.NegativeSign; }
			set { Behavior.NegativeSign = value; }
		}
		#endregion

		#region Prefix
		/// <summary>
		///  Gets or sets the text to automatically insert in front of the number, such as a currency symbol. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.Prefix">NumericBehavior.Prefix</see>. </remarks>
		/// <seealso cref="NumericBehavior.Prefix" />
		[Category("Behavior")]
		[Description("The text to automatically insert in front of the number, such as a currency symbol.")]
		public string Prefix
		{
			get { return Behavior.Prefix; }
			set { Behavior.Prefix = value; }
		}
		#endregion

		#region RangeMin
		/// <summary>
		/// Gets or sets the minimum value allowed. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.RangeMin">NumericBehavior.RangeMin</see>. </remarks>
		/// <seealso cref="NumericBehavior.RangeMin" />
		[Category("Behavior")]
		[Description("The minimum value allowed.")]
		public double RangeMin
		{
			get { return Behavior.RangeMin; }
			set { Behavior.RangeMin = value; }
		}
		#endregion

		#region RangeMax
		/// <summary>
		/// Gets or sets the maximum value allowed. </summary>
		/// <remarks>
		/// This property delegates to <see cref="NumericBehavior.RangeMax">NumericBehavior.RangeMax</see>. </remarks>
		/// <seealso cref="NumericBehavior.RangeMax" />
		[Category("Behavior")]
		[Description("The maximum value allowed.")]
		public double RangeMax
		{
			get { return Behavior.RangeMax; }
			set { Behavior.RangeMax = value; }
		}
		#endregion
	};
	#endregion

	#region Integer textbox
	/// <summary>
	/// TextBox class which supports the <see cref="IntegerBehavior">Integer</see> behavior. </summary>	
	[Description("TextBox control which supports the Integer behavior.")]
	public class IntegerTextBox : NumericTextBox
	{
		/// <summary>
		/// Initializes a new instance of the IntegerTextBox class by assigning its Behavior field
		/// to an instance of <see cref="IntegerBehavior" />. </summary>
		public IntegerTextBox() : base(null) { behavior = new IntegerBehavior(this); }

		/// <summary>
		/// Initializes a new instance of the IntegerTextBox class by assigning its Behavior field 
		/// to an instance of <see cref="IntegerBehavior" /> and setting the maximum number of digits 
		/// allowed left of the decimal point. </summary>
		/// <param name="maxWholeDigits">
		/// The maximum number of digits allowed left of the decimal point.
		/// If it is less than 1, it is set to 1. </param>
		public IntegerTextBox(int maxWholeDigits) : base(null) { behavior = new IntegerBehavior(this, maxWholeDigits); }

		/// <summary>
		/// Initializes a new instance of the IntegerTextBox class by explicitly assigning its Behavior field. </summary>
		/// <param name="behavior">
		/// The <see cref="IntegerBehavior" /> object to associate the textbox with. </param>
		public IntegerTextBox(IntegerBehavior behavior) : base(behavior) { }
	};
	#endregion
}