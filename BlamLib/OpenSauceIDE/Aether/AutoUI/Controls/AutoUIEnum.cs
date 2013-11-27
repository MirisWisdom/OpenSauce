using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	An automatic user interface combobox for enums. </summary>
	///-------------------------------------------------------------------------------------------------
	[Attributes.AutoUISize(100, 0, 200, 0)]
	public class AutoUIEnum : System.Windows.Forms.ComboBox, IAutoUIControl
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		///-------------------------------------------------------------------------------------------------
		public AutoUIEnum()
		{
			DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
		}

		#region Fields
		private object mSourceObject = null;
		private List<object> mEnumValues = new List<object>();
		private bool mIgnoreIndexChanged = false;
		#endregion Fields

		#region Methods
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets index of a value in the values list. </summary>
		/// <param name="value">	The value to find. </param>
		/// <returns>	The index of the value in the values list. </returns>
		///-------------------------------------------------------------------------------------------------
		private int GetIndexFromValue(object value)
		{
			return mEnumValues.FindIndex(obj => obj.Equals(value));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets value at the specified index in the values list. </summary>
		/// <param name="index">	The index of the value to retrieve. </param>
		/// <returns>	The enum value from the values list. </returns>
		///-------------------------------------------------------------------------------------------------
		private object GetValueFromIndex(int index)
		{
			return mEnumValues[index];
		}
		#endregion Methods

		#region Events
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Raises the selected index changed event. </summary>
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information to send to registered event handlers. </param>
		///-------------------------------------------------------------------------------------------------
		void OnSelectedIndexChanged(object sender, EventArgs e)
		{
			if (mIgnoreIndexChanged)
			{
				return;
			}

			// Build the enum value from the stored list and set the objects property to the selected value
			object newValue = Enum.ToObject(MemberInfo.MemberValueType, GetValueFromIndex(SelectedIndex));
			MemberInfo.SetValue(mSourceObject, newValue);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Raised when the objects value is changed. </summary>
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		///-------------------------------------------------------------------------------------------------
		void OnValueChanged(object sender, PropertyChangedEventArgs e)
		{
			mIgnoreIndexChanged = true;

			// Set the selected index to the new value
			object newValue = MemberInfo.GetValue(mSourceObject);
			SelectedIndex = GetIndexFromValue(newValue);

			mIgnoreIndexChanged = false;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Registers the events that bind the control to the data. </summary>
		/// <exception cref="Exception">
		/// 	Thrown if the source object does not implement INotifyPropertyChanged.
		/// </exception>
		///-------------------------------------------------------------------------------------------------
		private void RegisterEvents()
		{
			if (mSourceObject != null)
			{
				// Exception if the source object does not implement INotifyPropertyChanged
				if (!(mSourceObject is INotifyPropertyChanged))
				{
					throw new Exception("Attempted to bind an AutoUIEnum instance to an object that does not implement INotifyPropertyChanged");
				}

				var propertyChangedInterface = mSourceObject as INotifyPropertyChanged;

				propertyChangedInterface.PropertyChanged += new PropertyChangedEventHandler(OnValueChanged);
				SelectedIndexChanged += new EventHandler(OnSelectedIndexChanged);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Unregisters the events that bind the control to the data. </summary>
		///-------------------------------------------------------------------------------------------------
		private void UnregisterEvents()
		{
			if (mSourceObject != null)
			{
				var propertyChangedInterface = mSourceObject as INotifyPropertyChanged;

				propertyChangedInterface.PropertyChanged -= new PropertyChangedEventHandler(OnValueChanged);
				SelectedIndexChanged -= new EventHandler(OnSelectedIndexChanged);
			}
		}
		#endregion Events

		#region IAutoUIControl Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets or sets information describing the target member. </summary>
		/// <value>	Information describing the target member. </value>
		///-------------------------------------------------------------------------------------------------
		public IAutoUIMemberInfo MemberInfo
		{
			get;
			private set;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the target member info for the control. </summary>
		/// <param name="memberInfo">	Information describing the target member. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetMemberInfo(IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the source object to bind the control to. </summary>
		/// <exception cref="Exception">	Thrown if the enum is used as binary flags. </exception>
		/// <param name="sourceObject">	The source object to bind to. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetSource(object sourceObject)
		{
			SuspendLayout();

			// Reset the control
			UnregisterEvents();

			Items.Clear();
			mEnumValues.Clear();

			// Initialise the new enum source object
			mSourceObject = sourceObject;

			// Get the enum type
			Type enumType = MemberInfo.MemberValueType;

			if (!enumType.IsEnum || enumType.IsDefined(typeof(FlagsAttribute), false))
			{
				throw new Exception("Attempted to set a AutoUIEnum to a property that is not an enum without flags");
			}

			// Add an entry for each of the enums fields
			var enumFields = enumType.GetFields(BindingFlags.Public | BindingFlags.Static);
			var enumValues = Enum.GetValues(enumType);

			for (int i = 0; i < enumFields.Length; i++)
			{
				// Ignore values if needed
				if (AutoUIFactory.GetAutoUIIgnored(enumFields[i]))
				{
					continue;
				}

				// Get the name of the enum value
				string name = AutoUIFactory.GetAutoUIName(enumFields[i]);
				name = (name == null ? enumFields[i].Name : name);

				// Add the name to the list and store it's value
				Items.Add(name);
				mEnumValues.Add(enumValues.GetValue(i));
			}

			// Set the initial enum value
			object enumInstanceValue = MemberInfo.GetValue(sourceObject);
			SelectedIndex = GetIndexFromValue(enumInstanceValue);

			// Disable the control if the member is read only
			Enabled = MemberInfo.CanWrite;

			RegisterEvents();

			ResumeLayout();
		}
		#endregion IAutoUIControl Members
	}
}
