using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace OpenSauceIDE.Aether.SceneObject
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Enum to determine whether a scene object is a root object. </summary>
	///-------------------------------------------------------------------------------------------------
	public enum SceneObjectClassEnum
	{
        Root,
        Object
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Base attribute for scene objects. </summary>
	///-------------------------------------------------------------------------------------------------
	[AttributeUsage(AttributeTargets.Class, AllowMultiple=false, Inherited=false)]
	public class SceneObjectAttribute : Attribute
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets or the scene object type. </summary>
		/// <value>	The scene object type. </value>
		///-------------------------------------------------------------------------------------------------
		public SceneObjectClassEnum SceneObjectClass { get; private set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the set the scene object belongs to. </summary>
		/// <value>	The scene object set. </value>
		///-------------------------------------------------------------------------------------------------
		public string SceneObjectSet { get; private set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="sceneObjectClass">	The scene object type. </param>
		/// <param name="sceneObjectSet">  	The scene object set. </param>
		///-------------------------------------------------------------------------------------------------
		public SceneObjectAttribute(SceneObjectClassEnum sceneObjectClass, string sceneObjectSet)
		{
			SceneObjectClass = sceneObjectClass;
			SceneObjectSet = sceneObjectSet;
		}
	}
	
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Interface for an extractable scene object. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface ISceneObjectExtractable
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets or sets a value indicating whether the extraction is enabled. </summary>
		/// <value>	true if enable extraction, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		bool EnableExtraction { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Extracts this object. </summary>
		///-------------------------------------------------------------------------------------------------
		void Extract();
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Abstract base class for scene objects. </summary>
	///-------------------------------------------------------------------------------------------------
	public abstract class SceneObjectBase : INotifyPropertyChanged
	{
		#region Constructor/Destructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		///-------------------------------------------------------------------------------------------------
		public SceneObjectBase()
		{
			mParentObject = null;
			mChildObjects = new List<SceneObjectBase>();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Finaliser. </summary>
		///-------------------------------------------------------------------------------------------------
		~SceneObjectBase()
		{
			if (mParentObject != null)
			{
				mParentObject.RemoveChild(this);
			}

			mParentObject = null;
			mChildObjects = null;
		}
		#endregion Constructor/Destructor

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the name of the object. </summary>
		/// <value>	The name of the object. </value>
		///-------------------------------------------------------------------------------------------------
		public abstract string ObjectName { get; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets scene object class. </summary>
		/// <exception cref="Exception">
		/// 	Thrown if the class does not have one description attribute.
		/// </exception>
		/// <returns>	The scene object class. </returns>
		///-------------------------------------------------------------------------------------------------
		public SceneObjectClassEnum GetSceneObjectClass()
		{
			Type sceneObjectType = this.GetType();

			var descAttributes = sceneObjectType.GetCustomAttributes(typeof(SceneObjectAttribute), false);
			if (descAttributes.Count() != 1)
			{
				throw new Exception(String.Format("SceneObject {0} must only have one description attribute", sceneObjectType.Name));
			}

			return (descAttributes[0] as SceneObjectAttribute).SceneObjectClass;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the parent scene object. </summary>
		/// <param name="parent">	The parent scene object. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetParent(SceneObjectBase parent)
		{
			mParentObject = parent;

			if (mParentObject != null)
			{
				mParentObject.AddChild(this);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a child scene object. </summary>
		/// <param name="child">	The child scene object. </param>
		///-------------------------------------------------------------------------------------------------
		public void AddChild(SceneObjectBase child)
		{
			mChildObjects.Add(child);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Removes the child scene object described by child. </summary>
		/// <param name="child">	The child scene object. </param>
		///-------------------------------------------------------------------------------------------------
		public void RemoveChild(SceneObjectBase child)
		{
			mChildObjects.Remove(child);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a child scene object of this item. </summary>
		/// <param name="index">	Zero-based index of the child scene object. </param>
		/// <returns>	The child scene object. </returns>
		///-------------------------------------------------------------------------------------------------
		public SceneObjectBase GetChild(int index)
		{
			return mChildObjects[index];
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the children of this item. </summary>
		/// <returns>	The children. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<SceneObjectBase> GetChildren()
		{
			return mChildObjects;
		}

		#region Fields
		private SceneObjectBase mParentObject;
		private List<SceneObjectBase> mChildObjects;
		#endregion Fields

		#region INotifyPropertyChanged Members
		public event PropertyChangedEventHandler PropertyChanged;   ///< Event queue for all listeners interested in PropertyChanged events.

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Executes the property changed action. </summary>
		/// <param name="name">	The name of the property. </param>
		///-------------------------------------------------------------------------------------------------
		protected void OnPropertyChanged(string name)
		{
			var eventHandler = PropertyChanged;

			if (eventHandler != null)
			{
				eventHandler(this, new PropertyChangedEventArgs(name));
			}
		}
		#endregion
	}
}
