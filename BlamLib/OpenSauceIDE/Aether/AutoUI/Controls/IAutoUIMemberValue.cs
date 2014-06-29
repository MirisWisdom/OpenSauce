using System;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	Interface for automatic user interface member information. </summary>
	public interface IAutoUIMemberInfo
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the member. </summary>
		///
		/// <value>	The name of the member. </value>
		string Name { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the member. </summary>
		///
		/// <value>	Information describing the member. </value>
		MemberInfo MemberInfo { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type of the member. </summary>
		///
		/// <value>	The type of the member. </value>
		Type MemberValueType { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether we can read the member. </summary>
		///
		/// <value>	true if we can read, false if not. </value>
		bool CanRead { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether we can write to the member. </summary>
		///
		/// <value>	true if we can write, false if not. </value>
		bool CanWrite { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the value of the member from a source object. </summary>
		///
		/// <param name="source">	The object to get the value from. </param>
		///
		/// <returns>	The member's value. </returns>
		object GetValue(object source);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the value of the member in a target object. </summary>
		///
		/// <param name="target">	The target object. </param>
		/// <param name="value"> 	The value to set. </param>
		void SetValue(object target, object value);
	}

	/// <summary>	A member info inferface for a field member. </summary>
	public class AutoUIFieldInterface : IAutoUIMemberInfo
	{
		#region Fields
		private FieldInfo mFieldInfo = null;
		#endregion Fields

		#region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the information describing the member. </summary>
		///
		/// <value>	Information describing the member. </value>
		public MemberInfo MemberInfo
		{
			get
			{
				return mFieldInfo;
			}
		}
		#endregion Properties

		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="fieldInfo">	Information describing the field. </param>
		public AutoUIFieldInterface(FieldInfo fieldInfo)
		{
			mFieldInfo = fieldInfo;
		}
		#endregion Constructor

		#region IAutoUIMemberValue Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the field. </summary>
		///
		/// <value>	The name of the field. </value>
		public string Name
		{
			get
			{
				return mFieldInfo.Name;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the fields type. </summary>
		///
		/// <value>	The type of the field. </value>
		public Type MemberValueType
		{
			get
			{
				return mFieldInfo.FieldType;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether we can read the field. </summary>
		///
		/// <value>	true if we can read, false if not. </value>
		public bool CanRead
		{
			get
			{
				return !mFieldInfo.IsPrivate;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether we can write the field. </summary>
		///
		/// <value>	true if we can write, false if not. </value>
		public bool CanWrite
		{
			get
			{
				return !mFieldInfo.IsInitOnly && !mFieldInfo.IsLiteral && !mFieldInfo.IsPrivate;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a fields value from the specified object. </summary>
		///
		/// <param name="source">	Source object to get the value from. </param>
		///
		/// <returns>	The fields value. </returns>
		public object GetValue(object source)
		{
			return mFieldInfo.GetValue(source);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the fields value in the target object. </summary>
		///
		/// <param name="target">	Target object to set. </param>
		/// <param name="value"> 	The value to use. </param>
		public void SetValue(object target, object value)
		{
			mFieldInfo.SetValue(target, value);
		}
		#endregion
	}

	/// <summary>	A member info inferface for a property member. </summary>
	public class AutoUIPropertyInterface : IAutoUIMemberInfo
	{
		#region Fields
		private PropertyInfo mPropertyInfo = null;
		#endregion Fields

		#region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the information describing the member. </summary>
		///
		/// <value>	Information describing the member. </value>
		public MemberInfo MemberInfo
		{
			get
			{
				return mPropertyInfo;
			}
		}
		#endregion Properties

		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="propertyInfo">	Information describing the property. </param>
		public AutoUIPropertyInterface(PropertyInfo propertyInfo)
		{
			mPropertyInfo = propertyInfo;
		}
		#endregion Constructor

		#region IAutoUIMemberValue Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the property. </summary>
		///
		/// <value>	The name of the property. </value>
		public string Name
		{
			get
			{
				return mPropertyInfo.Name;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the property's type. </summary>
		///
		/// <value>	The type of the property. </value>
		public Type MemberValueType
		{
			get
			{
				return mPropertyInfo.PropertyType;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether we can read the property. </summary>
		///
		/// <value>	true if we can read, false if not. </value>
		public bool CanRead
		{
			get
			{
				return mPropertyInfo.CanRead;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether we can write the property. </summary>
		///
		/// <value>	true if we can write, false if not. </value>
		public bool CanWrite
		{
			get
			{
				return mPropertyInfo.CanWrite;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a property's value from the specified object. </summary>
		///
		/// <param name="source">	Source object to get the value from. </param>
		///
		/// <returns>	The property's value. </returns>
		public object GetValue(object source)
		{
			return mPropertyInfo.GetValue(source, null);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the property's value in the target object. </summary>
		///
		/// <param name="target">	Target object to set. </param>
		/// <param name="value"> 	The value to use. </param>
		public void SetValue(object target, object value)
		{
			mPropertyInfo.SetValue(target, value, null);
		}
		#endregion
	}
}
