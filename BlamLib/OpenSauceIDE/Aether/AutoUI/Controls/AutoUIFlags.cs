using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	An automatic user interface checked list box for a flagged enum. </summary>
	public class AutoUIFlags : CheckedListBox, IAutoUIControl
	{
		#region Fields
		private object mSourceObject = null;
		private List<ulong> mFlagValues = new List<ulong>();
		private bool mIgnoreFlagsChanged = false;
		#endregion Fields

		#region Methods
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the controls checkboxes from a value. </summary>
		///
		/// <param name="value">	The value. </param>
		private void SetFlagsFromValue(ulong value)
		{
			// Bitwise and the values in the values list, setting the check box states at each index
			for (int i = 0; i < mFlagValues.Count; i++)
			{
				SetItemChecked(i, (mFlagValues[i] & value) == mFlagValues[i]); 
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the value defined by the check box values. </summary>
		///
		/// <param name="indexOverride">	(Optional) The index to override the value of. </param>
		/// <param name="overrideValue">	(Optional) The value to use instead of the value at indexOverride. </param>
		///
		/// <returns>	The value defined by the check box values. </returns>
		private ulong GetValueFromFlags(int indexOverride = -1, bool overrideValue = false)
		{
			ulong value = 0;
			for (int i = 0; i < mFlagValues.Count; i++)
			{
				bool itemChecked = false;

				// Override the items value if needed, otherwise use it's checked state
				if (indexOverride == i)
				{
					itemChecked = overrideValue;
				}
				else 
				{
					itemChecked = GetItemChecked(i);
				}

				// Bitwise or the flag values together to get the final value
				if (itemChecked)
				{
					value |= mFlagValues[i];
				}
			}
			return value;
		}
		#endregion Methods

		#region Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Raises the selected index changed event. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information to send to registered event handlers. </param>
		void OnItemCheck(object sender, ItemCheckEventArgs e)
		{
			if (mIgnoreFlagsChanged)
			{
				return;
			}

			// Build the flags value from the stored list and set the objects property to the selected flags
			object newValue = Enum.ToObject(MemberInfo.MemberValueType, GetValueFromFlags(e.Index, e.NewValue == CheckState.Checked));
			MemberInfo.SetValue(mSourceObject, newValue);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Raised when the objects value is changed. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		void OnValueChanged(object sender, PropertyChangedEventArgs e)
		{
			mIgnoreFlagsChanged = true;

			// Set the flags to the new value
			object newValue = MemberInfo.GetValue(mSourceObject);
			SetFlagsFromValue(Convert.ToUInt64(newValue));

			mIgnoreFlagsChanged = false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Registers the events that bind the control to the data. </summary>
		///
		/// <exception cref="Exception">	Thrown if the source object does not implement INotifyPropertyChanged. </exception>
		private void RegisterEvents()
		{
			if (mSourceObject != null)
			{
				if (!(mSourceObject is INotifyPropertyChanged))
				{
					throw new Exception("Attempted to bind an AutoUIFlags instance to an object that does not implement INotifyPropertyChanged");
				}

				var propertyChangedInterface = mSourceObject as INotifyPropertyChanged;

				propertyChangedInterface.PropertyChanged += new PropertyChangedEventHandler(OnValueChanged);
				ItemCheck += new ItemCheckEventHandler(OnItemCheck);
			}
		}

		/// <summary>	Unregisters the events that bind the control to the data. </summary>
		private void UnregisterEvents()
		{
			if (mSourceObject != null)
			{
				var propertyChangedInterface = mSourceObject as INotifyPropertyChanged;

				propertyChangedInterface.PropertyChanged -= new PropertyChangedEventHandler(OnValueChanged);
				ItemCheck -= new ItemCheckEventHandler(OnItemCheck);
			}
		}
		#endregion Events

		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the target member. </summary>
		///
		/// <value>	Information describing the target member. </value>
		public IAutoUIMemberInfo MemberInfo { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target member info for the control. </summary>
		///
		/// <param name="memberInfo">	Information describing the target member. </param>
		public void SetMemberInfo(IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the source object to bind the control to. </summary>
		///
		/// <exception cref="Exception">	Thrown if the enum type is not a flag type. </exception>
		///
		/// <param name="sourceObject">	The source object to bind to. </param>
		public void SetSource(object sourceObject)
		{
			SuspendLayout();

			// Reset the control
			UnregisterEvents();

			Items.Clear();
			mFlagValues.Clear();

			mSourceObject = sourceObject;

			// Get the property's enum type
			Type enumType = MemberInfo.MemberValueType;

			if (!enumType.IsEnum || !enumType.IsDefined(typeof(FlagsAttribute), false))
			{
				throw new Exception("Attempted to set a AutoUIFlags to a property that is not an enum with flags");
			}

			// Add a checkbox for each of the enums fields
			var enumFields = enumType.GetFields(BindingFlags.Public | BindingFlags.Static);
			var enumValues = Enum.GetValues(enumType);

			for (int i = 0; i < enumFields.Length; i++)
			{
				// Ignore values if needed
				if (AutoUIFactory.GetAutoUIIgnored(enumFields[i]))
				{
					continue;
				}

				// Add an entry for each value
				Items.Add(AutoUIFactory.GetAutoUIName(enumFields[i]));
				mFlagValues.Add(Convert.ToUInt64(enumValues.GetValue(i)));
			}

			// Set the initial flag values
			ulong flagsInstanceValue = Convert.ToUInt64(MemberInfo.GetValue(sourceObject));
			SetFlagsFromValue(flagsInstanceValue);

			// Disable the control if the member is read only
			Enabled = MemberInfo.CanWrite;

			RegisterEvents();

			ResumeLayout();
		}
		#endregion IAutoUIControl Members
	}
}
