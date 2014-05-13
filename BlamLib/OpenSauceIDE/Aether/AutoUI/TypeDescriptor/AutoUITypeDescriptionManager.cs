using System;
using System.Collections.Generic;

namespace OpenSauceIDE.Aether.AutoUI.TypeDescriptor
{
	/// <summary>	Manager for automatic user interface type descriptions. </summary>
	public class AutoUITypeDescriptionManager
	{
		#region Singleton
		public static AutoUITypeDescriptionManager mInstance = new AutoUITypeDescriptionManager();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the manager instance. </summary>
		///
		/// <value>	The manager instance. </value>
		public static AutoUITypeDescriptionManager Instance
		{
			get
			{
				return mInstance;
			}
		}
		#endregion

		#region Fields
		private Dictionary<Type, IAutoUITypeDescriptor> mTypeDescriptors;
		#endregion Fields

		#region Constructor
		/// <summary>	Constructor that prevents a default instance of this class from being created. </summary>
		private AutoUITypeDescriptionManager()
		{
			mTypeDescriptors = new Dictionary<Type, IAutoUITypeDescriptor>();

			AddDescriptor(typeof(BlamLib.TagInterface.String), new AutoUITypeDescriptorBlamString());
			AddDescriptor(typeof(BlamLib.TagInterface.Real), new AutoUITypeDescriptorBlamReal());
			AddDescriptor(typeof(BlamLib.TagInterface.RealPoint2D), new AutoUITypeDescriptorBlamRealPoint2D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealPoint3D), new AutoUITypeDescriptorBlamRealPoint3D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealVector2D), new AutoUITypeDescriptorBlamRealVector2D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealVector3D), new AutoUITypeDescriptorBlamRealVector3D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealQuaternion), new AutoUITypeDescriptorBlamRealQuaternion());
			AddDescriptor(typeof(BlamLib.TagInterface.RealEulerAngles2D), new AutoUITypeDescriptorBlamRealEulerAngles2D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealEulerAngles3D), new AutoUITypeDescriptorBlamRealEulerAngles3D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealPlane2D), new AutoUITypeDescriptorBlamRealPlane2D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealPlane3D), new AutoUITypeDescriptorBlamRealPlane3D());
			AddDescriptor(typeof(BlamLib.TagInterface.RealColor), new AutoUITypeDescriptorBlamRealColor());
			AddDescriptor(typeof(BlamLib.TagInterface.RealBounds), new AutoUITypeDescriptorBlamRealBounds());
			AddDescriptor(typeof(BlamLib.TagInterface.ByteInteger), new AutoUITypeDescriptorBlamByteInteger());
			AddDescriptor(typeof(BlamLib.TagInterface.ShortInteger), new AutoUITypeDescriptorBlamShortInteger());
			AddDescriptor(typeof(BlamLib.TagInterface.LongInteger), new AutoUITypeDescriptorBlamLongInteger());
			AddDescriptor(typeof(BlamLib.TagInterface.Tag), new AutoUITypeDescriptorBlamTag());
			AddDescriptor(typeof(BlamLib.TagInterface.Enum), new AutoUITypeDescriptorBlamEnum());
			AddDescriptor(typeof(BlamLib.TagInterface.Flags), new AutoUITypeDescriptorBlamFlags());
			AddDescriptor(typeof(BlamLib.TagInterface.Point2D), new AutoUITypeDescriptorBlamPoint2D());
			AddDescriptor(typeof(BlamLib.TagInterface.Rectangle2D), new AutoUITypeDescriptorBlamRectangle2D());
			AddDescriptor(typeof(BlamLib.TagInterface.Color), new AutoUITypeDescriptorBlamColor());
			AddDescriptor(typeof(BlamLib.TagInterface.ShortIntegerBounds), new AutoUITypeDescriptorBlamShortIntegerBounds());
			AddDescriptor(typeof(BlamLib.TagInterface.BlockIndex), new AutoUITypeDescriptorBlamBlockIndex());
		}
		#endregion Constructor

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a type descriptor to the manager. </summary>
		///
		/// <exception cref="Exception">	Thrown if a type is described more than once. </exception>
		///
		/// <param name="objectType">	Type of the object. </param>
		/// <param name="descriptor">	The type descriptor. </param>
		public void AddDescriptor(Type objectType, IAutoUITypeDescriptor descriptor)
		{
			if(mTypeDescriptors.ContainsKey(objectType))
			{
				throw new Exception("Attempted to re-describe a type");
			}

			mTypeDescriptors.Add(objectType, descriptor);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a type descriptor. </summary>
		///
		/// <remarks>	A new descriptor is made if a matching one is not found. </remarks>
		///
		/// <param name="objectType">	Type of the object. </param>
		///
		/// <returns>	The descriptor. </returns>
		public IAutoUITypeDescriptor GetDescriptor(Type objectType)
		{
			if (!mTypeDescriptors.ContainsKey(objectType))
			{
				AddDescriptor(objectType, new AutoUITypeDescriptor(objectType));
			}

			return mTypeDescriptors[objectType];
		}
	}
}
