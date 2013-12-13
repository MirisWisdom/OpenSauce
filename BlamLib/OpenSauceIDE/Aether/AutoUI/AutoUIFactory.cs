using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using OpenSauceIDE.Aether.AutoUI.Attributes;
using OpenSauceIDE.Aether.AutoUI.BlamControls;
using OpenSauceIDE.Aether.AutoUI.Controls;
using OpenSauceIDE.Aether.AutoUI.TypeDescriptor;

namespace OpenSauceIDE.Aether.AutoUI
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Singleton UI factory class. </summary>
	///-------------------------------------------------------------------------------------------------
	public class AutoUIFactory
	{
		#region Singleton
		private static AutoUIFactory mInstance = new AutoUIFactory();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the factory instance. </summary>
		/// <value>	The factory instance. </value>
		///-------------------------------------------------------------------------------------------------
		public static AutoUIFactory Instance
		{
			get
			{
				return mInstance;
			}
		}
		#endregion Singleton

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Constructor that prevents a default instance of this class from being created.
		/// 	Initialises the factory with known control types.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		private AutoUIFactory()
		{			
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(bool), ControlType = typeof(AutoUIBoolean) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(byte), ControlType = typeof(AutoUIByte) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(sbyte), ControlType = typeof(AutoUISByte) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(char), ControlType = typeof(AutoUIChar) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(float), ControlType = typeof(AutoUISingle) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(double), ControlType = typeof(AutoUIDouble) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(ushort), ControlType = typeof(AutoUIUInt16) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(uint), ControlType = typeof(AutoUIUInt32) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(ulong), ControlType = typeof(AutoUIUInt64) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(short), ControlType = typeof(AutoUIInt16) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(int), ControlType = typeof(AutoUIInt32) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(long), ControlType = typeof(AutoUIInt64) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(string), ControlType = typeof(AutoUIString) });

			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.Real), ControlType = typeof(AutoUIBlamReal) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealPoint2D), ControlType = typeof(AutoUIBlamRealPoint2D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealPoint3D), ControlType = typeof(AutoUIBlamRealPoint3D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealVector2D), ControlType = typeof(AutoUIBlamRealVector2D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealVector3D), ControlType = typeof(AutoUIBlamRealVector3D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealQuaternion), ControlType = typeof(AutoUIBlamRealQuaternion) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealEulerAngles2D), ControlType = typeof(AutoUIBlamRealEulerAngles2D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealEulerAngles3D), ControlType = typeof(AutoUIBlamRealEulerAngles3D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealPlane2D), ControlType = typeof(AutoUIBlamRealPlane2D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealPlane3D), ControlType = typeof(AutoUIBlamRealPlane3D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealBounds), ControlType = typeof(AutoUIBlamRealBounds) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.RealColor), ControlType = typeof(AutoUIBlamRealColor) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.ByteInteger), ControlType = typeof(AutoUIBlamByteInteger) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.ShortInteger), ControlType = typeof(AutoUIBlamShortInteger) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.LongInteger), ControlType = typeof(AutoUIBlamLongInteger) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.Tag), ControlType = typeof(AutoUIBlamTag) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.Enum), ControlType = typeof(AutoUIBlamEnum) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.Flags), ControlType = typeof(AutoUIBlamFlags) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.Point2D), ControlType = typeof(AutoUIBlamPoint2D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.Rectangle2D), ControlType = typeof(AutoUIBlamRectangle2D) });
			mUITypeControlPairs.Add(new TypeControlPair { MemberType = typeof(BlamLib.TagInterface.BlockIndex), ControlType = typeof(AutoUIBlamBlockIndex) });
		}

		#region Prototype
		///-------------------------------------------------------------------------------------------------
		/// <summary>	A struct containing the type of control to use for a types member. </summary>
		///-------------------------------------------------------------------------------------------------
		public struct TypeMember
		{
			#region Fields
			public Type ControlType;
			public IAutoUIMemberInfo MemberInfo;
			#endregion

			public TypeMember(Type controlType, IAutoUIMemberInfo memberInfo)
			{
				ControlType = controlType;
				MemberInfo = memberInfo;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Prototype UI definition for an object type, used to generate a UI control when needed.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		public struct UIPrototype
		{
			#region Fields
			public Type ObjectType;
			public List<TypeMember> ControlTypes;
			#endregion

			public UIPrototype(Type objectType)
			{
				ObjectType = objectType;
				ControlTypes = new List<TypeMember>();
			}
		}

		public struct TypeControlPair
		{
			public Type MemberType;
			public Type ControlType;
		}

		private List<UIPrototype> mUIPrototypes = new List<UIPrototype>();
		private List<TypeControlPair> mUITypeControlPairs = new List<TypeControlPair>();
		#endregion Prototype

		#region Control Design
		public static void SetControlBounds(System.Windows.Forms.Control control, int x, int y, int w, int h)
		{
			if (control.GetType().IsDefined(typeof(AutoUISizeAttribute), true))
			{
				AutoUISizeAttribute sizeAttribute = control.GetType().GetCustomAttributes(typeof(AutoUISizeAttribute), true)[0] as AutoUISizeAttribute;

				int setW = w, setH = h;
				setW = (sizeAttribute.MinWidth == 0 ? setW : Math.Max(setW, sizeAttribute.MinWidth));
				setW = (sizeAttribute.MaxWidth == 0 ? setW : Math.Min(setW, sizeAttribute.MaxWidth));
				setH = (sizeAttribute.MinHeight == 0 ? setH : Math.Max(setH, sizeAttribute.MinHeight));
				setH = (sizeAttribute.MaxHeight == 0 ? setH : Math.Min(setH, sizeAttribute.MaxHeight));

				control.SetBounds(x, y, setW, setH);
			}
			else
			{
				control.SetBounds(x, y, w, h);
			}
		}
		#endregion Control Design

		#region UI Construction
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the control type for the specified object member. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	The control type. </returns>
		///-------------------------------------------------------------------------------------------------
		private Type GetControlType(IAutoUIMemberInfo memberInfo)
		{
			// Find a matching control for the member type
			var controlPairIndex = mUITypeControlPairs.FindIndex(pair => pair.MemberType == memberInfo.MemberValueType);

			Type controlType = null;

			if (controlPairIndex == -1)
			{
				// No matching control was found, so determine the base control type to use
				if (memberInfo.MemberValueType.IsEnum && memberInfo.MemberValueType.IsDefined(typeof(FlagsAttribute), false))
				{
					controlType = typeof(AutoUIFlags);
				}
				else if (memberInfo.MemberValueType.IsEnum)
				{
					controlType = typeof(AutoUIEnum);
				}
				else if (Type.GetTypeCode(memberInfo.MemberValueType) == TypeCode.Object)
				{
					controlType = typeof(AutoUIClass);
				}
			}
			else
			{
				controlType = mUITypeControlPairs[controlPairIndex].ControlType;
			}

			return controlType;
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Builds a user interface prototype. </summary>
		/// <exception cref="Exception">	Thrown when an exception error condition occurs. </exception>
		/// <param name="classType">	Type of the class to prototype for. </param>
		///-------------------------------------------------------------------------------------------------
		public void BuildUIPrototype(Type classType)
		{
			// Get a type descriptor for the class
			IAutoUITypeDescriptor typeDescriptor = AutoUITypeDescriptionManager.Instance.GetDescriptor(classType);

			// Build a new ui prototype for the class
			UIPrototype uiPrototype = new UIPrototype(classType);

			// Iterate the class types members
			foreach (var member in typeDescriptor.GetMemberInfos())
			{
				if (!GetAutoUIEnabled(member) || GetAutoUIIgnored(member))
				{
					continue;
				}

				// Get the control type to use for the member
				Type memberControlType = GetControlType(member);

				if (memberControlType == null)
				{
					throw new Exception(String.Format("Failed to find a suitable UI type for a type {0} member {1}", classType.Name, member.Name));
				}

				// Add a new control type to the prototype, tying it to the member info of the class member
				uiPrototype.ControlTypes.Add(new TypeMember(memberControlType, member));
			}

			mUIPrototypes.Add(uiPrototype);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Builds a control set for the specified object. </summary>
		/// <exception cref="Exception">	Thrown when an exception error condition occurs. </exception>
		/// <param name="objectInstance">	The object instance to create controls for. </param>
		/// <returns>	A list of controls, bound to the object instance. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<System.Windows.Forms.Control> BuildAutoUI(object objectInstance)
		{
			Type objectType = objectInstance.GetType();

			// If a ui prototype for the class type does not exist, create one
			if(!mUIPrototypes.Exists(prototype => prototype.ObjectType == objectType))
			{
				BuildUIPrototype(objectType);
			}

			// Find the ui prototype for the object
			var uiPrototype = mUIPrototypes.Find(prototype => prototype.ObjectType == objectType);

			List<System.Windows.Forms.Control> controls = new List<System.Windows.Forms.Control>();

			foreach (var controlType in uiPrototype.ControlTypes)
			{
				// Create an instance of each control type
				object newControl = Activator.CreateInstance(controlType.ControlType);

				if (!(newControl is System.Windows.Forms.Control) || !(newControl is IAutoUIControl))
				{
					throw new Exception("A ui prototype control type is either not a control or does not implement IAutoUIControl");
				}

				// Bind the control to the object instance
				IAutoUIControl autoUIControl = newControl as IAutoUIControl;
				autoUIControl.SetMemberInfo(controlType.MemberInfo);
				autoUIControl.SetSource(objectInstance);

				// Add the control to the list
				controls.Add(newControl as System.Windows.Forms.Control);
			}

			return controls;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Wraps a list of AutoUI controls in field containers. </summary>
		/// <param name="controlList">	List of controls. </param>
		/// <returns>	A list of AutoUIFieldContainers. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<AutoUIFieldContainer> BuildAutoUIFieldContainers(List<System.Windows.Forms.Control> controlList)
		{
			List<AutoUIFieldContainer> fieldContainers = new List<AutoUIFieldContainer>();

			controlList.ForEach(
				control =>
				{
					AutoUIFieldContainer fieldContainer = new AutoUIFieldContainer();
					IAutoUIControl autoUIControl = control as IAutoUIControl;

					var name = AutoUIFactory.GetAutoUIName(autoUIControl.MemberInfo);
					name = name == null ? autoUIControl.MemberInfo.Name : name;

					fieldContainer.SetTitle(name);
					fieldContainer.AddControl(control);

					fieldContainers.Add(fieldContainer);
				}
			);

			return fieldContainers;
		}
		#endregion UI Construction

		#region AutoUI Member Functions
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets whether the member is AutoUI enabled. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	true if enabled, otherwise false. </returns>
		///-------------------------------------------------------------------------------------------------
		public static bool GetAutoUIEnabled(MemberInfo memberInfo)
		{
			var parentDescriptor = AutoUITypeDescriptionManager.Instance.GetDescriptor(memberInfo.DeclaringType);

			var attributes = parentDescriptor.GetMemberAttributes(memberInfo.Name);
			return attributes.Exists(info => info is AutoUIAttribute);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets whether the member is AutoUI enabled. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	true if enabled, otherwise false. </returns>
		///-------------------------------------------------------------------------------------------------
		public static bool GetAutoUIEnabled(IAutoUIMemberInfo memberInfo)
		{
			return GetAutoUIEnabled(memberInfo.MemberInfo);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the AutoUI name of the specified member. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	The name of the member if it has an AutoUIName attribute, otherwise null. </returns>
		///-------------------------------------------------------------------------------------------------
		public static string GetAutoUIName(MemberInfo memberInfo)
		{
			var parentDescriptor = AutoUITypeDescriptionManager.Instance.GetDescriptor(memberInfo.DeclaringType);

			var attributes = parentDescriptor.GetMemberAttributes(memberInfo.Name);

			if (attributes.Exists(info => info is AutoUINameAttribute))
			{
				AutoUINameAttribute memberName = attributes.Find(info => info is AutoUINameAttribute) as AutoUINameAttribute;

				return memberName.Name;
			}
			else
			{
				return null;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the AutoUI name of the specified member. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	The name of the member if it has an AutoUIName attribute, otherwise null. </returns>
		///-------------------------------------------------------------------------------------------------
		public static string GetAutoUIName(IAutoUIMemberInfo memberInfo)
		{
			return GetAutoUIName(memberInfo.MemberInfo);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the AutoUI name of the specified object. </summary>
		/// <param name="sourceObject">	The object to get the name from. </param>
		/// <returns>
		/// 	The name of the member if it has an AutoUIName or AutoUIFormatted attribute, otherwise
		/// 	the object types name.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		public static string GetAutoUIName(object sourceObject)
		{
			// Get the objects type
			var typeDescriptor = AutoUITypeDescriptionManager.Instance.GetDescriptor(sourceObject.GetType());

			var typeAttributes = typeDescriptor.GetAttributes();
			if (typeAttributes.Exists(info => info is AutoUINameAttribute))
			{
				// If the object type has an AutoUIName attribute, return it's value
				AutoUINameAttribute memberName = typeAttributes.Find(info => info is AutoUINameAttribute) as AutoUINameAttribute;

				return memberName.Name;
			}
			else if (typeAttributes.Exists(info => info is AutoUINameFormattedAttribute))
			{
				// If the object type has an AutoUINameFormatted attribute, get the class member to use as the name and return the formatted name
				AutoUINameFormattedAttribute memberName = typeAttributes.Find(info => info is AutoUINameFormattedAttribute) as AutoUINameFormattedAttribute;

				var autoUIMemberInfo = typeDescriptor.GetMemberInfos().Find(info => info.Name.Equals(memberName.Source));

				object memberValue = autoUIMemberInfo.GetValue(sourceObject);

				return String.Format(memberName.Format, memberValue.ToString());
			}
			else
			{
				// Return the object type name
				return sourceObject.GetType().Name;
			}
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets whether the specified member should be ignored. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	true if it should be ignored, otherwise false. </returns>
		///-------------------------------------------------------------------------------------------------
		public static bool GetAutoUIIgnored(MemberInfo memberInfo)
		{
			var parentDescriptor = AutoUITypeDescriptionManager.Instance.GetDescriptor(memberInfo.DeclaringType);

			return parentDescriptor.GetMemberAttributes(memberInfo.Name).Exists(
				attribute =>
				{
					return attribute is AutoUI.Attributes.AutoUIIgnoreAttribute;
				}
			);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets whether the specified member should be ignored. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	true if it should be ignored, otherwise false. </returns>
		///-------------------------------------------------------------------------------------------------
		public static bool GetAutoUIIgnored(IAutoUIMemberInfo memberInfo)
		{
			return GetAutoUIIgnored(memberInfo.MemberInfo);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates an interface class for a member that is a field or property. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>
		/// 	A field or property interface class for the member. Null if the member isn't a field or
		/// 	property.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		public static IAutoUIMemberInfo GetAutoUIMemberInfo(MemberInfo memberInfo)
		{
			IAutoUIMemberInfo autoUIMemberInfo;
			switch (memberInfo.MemberType)
			{
				case MemberTypes.Field:
					autoUIMemberInfo = new AutoUIFieldInterface(memberInfo as FieldInfo);
					break;
				case MemberTypes.Property:
					autoUIMemberInfo = new AutoUIPropertyInterface(memberInfo as PropertyInfo);
					break;
				default:
					return null;
			}

			return autoUIMemberInfo;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets an AutoUI member info interface for the specified member. </summary>
		/// <exception cref="Exception">	Thrown if the specified member is not valid. </exception>
		/// <param name="objectType">	Type of the object that the member resides in. </param>
		/// <param name="memberName">	Name of the member to get. </param>
		/// <returns>
		/// 	A field or property interface class for the member. Null if the member isn't a field or
		/// 	property.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		public static IAutoUIMemberInfo GetAutoUIMemberInfo(Type objectType, string memberName)
		{
			// Get a type descriptor for the class
			IAutoUITypeDescriptor typeDescriptor = AutoUITypeDescriptionManager.Instance.GetDescriptor(objectType);

			var memberInfo = typeDescriptor.GetMemberInfos().Find(
				info =>
				{
					return info.Name.Equals(memberName);
				}
			);

			if (memberInfo == null)
			{
				throw new Exception(String.Format("Failed to get the member info for member {0} in {1}", memberName, objectType.Name));
			}

			return memberInfo;
		}
		#endregion AutoUI Member Functions
	}
}
