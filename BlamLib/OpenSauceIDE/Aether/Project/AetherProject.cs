using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using BlamLib;
using OpenSauceIDE.Aether.TagIO;
using System.Reflection;
using OpenSauceIDE.Aether.SceneObject;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.Aether.Extraction;

namespace OpenSauceIDE.Aether.Project
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Base Aether project class that has the events required to interop with the UI, export functions, input
	/// 	functions, etc.
	/// </summary>
	public abstract class AetherBaseProject
	{
		#region Fields
		/// <summary>	Manager for tag index. </summary>
		protected AetherTagIndexHandler<BlamLib.Managers.TagIndex> mTagIndexManager = null;
		/// <summary>	The scene objects. </summary>
		private List<SceneObjectBase> mSceneObjects = new List<SceneObjectBase>();
		#endregion Fields

		#region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the settings object for the project. </summary>
		///
		/// <value>	The settings object. </value>
		public OpenSauceIDE.Aether.Settings.AetherSettings Settings { get; protected set; }
		#endregion Properties

		#region Methods
		/// <summary>	Creates a tag manager. </summary>
		public abstract void CreateTagManager();

		/// <summary>	Disposes of the tag manager. </summary>
		public abstract void DisposeTagManager();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Opens a scenario. </summary>
		///
		/// <param name="location">	The location of the scenario. </param>
		public abstract bool OpenScenario(string location);

		/// <summary>	Closes the current scenario. </summary>
		public abstract void CloseScenario();
		#endregion Methods

		#region Accessors
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the tag extension for the specfied tag type. </summary>
		///
		/// <param name="tagTypeID">	Identifier for the tag type. </param>
		///
		/// <returns>	The tag extension. </returns>
		public abstract string GetTagExtension(string tagTypeID);
		#endregion Accessors

		#region Scene Objects
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a scene object. </summary>
		///
		/// <param name="sceneObject">	The scene object. </param>
		protected void AddSceneObject(SceneObjectBase sceneObject)
		{
			mSceneObjects.Add(sceneObject);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Removes the scene object described by sceneObject. </summary>
		///
		/// <param name="sceneObject">	The scene object. </param>
		protected void RemoveSceneObject(SceneObjectBase sceneObject)
		{
			mSceneObjects.Remove(sceneObject);
		}

		/// <summary>	Clears the scene objects list. </summary>
		protected void ClearSceneObjects()
		{
			mSceneObjects.Clear();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets scene objects of the specified class. </summary>
		///
		/// <param name="sceneObjectClass">	The scene object class. </param>
		///
		/// <returns>	A list of scene objects. </returns>
		public List<SceneObjectBase> GetSceneObjects(SceneObjectClassEnum sceneObjectClass)
		{
			return mSceneObjects.FindAll(
				so =>
				{
					return so.GetSceneObjectClass() == sceneObjectClass;
				}
			);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets all scene objects of the specified type. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		///
		/// <returns>	A list of scene objects cast to the requested type. </returns>
		public List<T> GetSceneObjects<T>()
			where T : SceneObjectBase
		{
			List<T> sceneObjectsList = new List<T>();
			
			mSceneObjects.FindAll(
				so =>
				{
					return so.GetType() == typeof(T);
				}
			).ForEach(
				so =>
				{
					sceneObjectsList.Add(so as T);
				}
			);

			return sceneObjectsList;
		}
		#endregion Scene Objects
	}
}
