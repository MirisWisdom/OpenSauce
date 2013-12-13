using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using BlamLib;
using OpenSauceIDE.Aether.TagIO;
using System.Reflection;
using OpenSauceIDE.Aether.SceneObject;
using BlamLib.Render.COLLADA;

namespace OpenSauceIDE.Aether.Project
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// 	Base Aether project class that has the events required to interop with the UI, export
	/// 	functions, input functions, etc.
	/// </summary>
	///-------------------------------------------------------------------------------------------------
	public abstract class AetherBaseProject
	{
		#region Fields
		protected AetherTagIndexHandler<BlamLib.Managers.TagIndex> mTagIndexManager = null; ///< Manager for tag index
		private List<SceneObjectBase> mSceneObjects = new List<SceneObjectBase>();  ///< The scene objects
		#endregion Fields

		#region Properties
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the settings object for the project. </summary>
		/// <value>	The settings object. </value>
		///-------------------------------------------------------------------------------------------------
		public OpenSauceIDE.Aether.Settings.AetherSettings Settings { get; protected set; }
		#endregion Properties

		#region Methods
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a tag manager. </summary>
		///-------------------------------------------------------------------------------------------------
		public abstract void CreateTagManager();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Disposes of the tag manager. </summary>
		///-------------------------------------------------------------------------------------------------
		public abstract void DisposeTagManager();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Opens a scenario. </summary>
		/// <param name="location">	The location of the scenario. </param>
		///-------------------------------------------------------------------------------------------------
		public abstract void OpenScenario(string location);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Closes the current scenario. </summary>
		///-------------------------------------------------------------------------------------------------
		public abstract void CloseScenario();
		#endregion Methods

		#region Accessors
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the tag extension for the specfied tag type. </summary>
		/// <param name="tagTypeID">	Identifier for the tag type. </param>
		/// <returns>	The tag extension. </returns>
		///-------------------------------------------------------------------------------------------------
		public abstract string GetTagExtension(string tagTypeID);
		#endregion Accessors

		#region Scene Objects
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a scene object. </summary>
		/// <param name="sceneObject">	The scene object. </param>
		///-------------------------------------------------------------------------------------------------
		protected void AddSceneObject(SceneObjectBase sceneObject)
		{
			mSceneObjects.Add(sceneObject);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Removes the scene object described by sceneObject. </summary>
		/// <param name="sceneObject">	The scene object. </param>
		///-------------------------------------------------------------------------------------------------
		protected void RemoveSceneObject(SceneObjectBase sceneObject)
		{
			mSceneObjects.Remove(sceneObject);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Clears the scene objects list. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void ClearSceneObjects()
		{
			mSceneObjects.Clear();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets scene objects. </summary>
		/// <param name="sceneObjectClass">	The scene object class. </param>
		/// <returns>	The scene objects. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<SceneObjectBase> GetSceneObjects(SceneObjectClassEnum sceneObjectClass)
		{
			return mSceneObjects.FindAll(
				so =>
				{
					return so.GetSceneObjectClass() == sceneObjectClass;
				}
			);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets scene objects. </summary>
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <returns>	A list of scene objects. </returns>
		///-------------------------------------------------------------------------------------------------
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
		
		#region Extraction
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Extracts this object. </summary>
		///-------------------------------------------------------------------------------------------------
		public void Extract()
		{
			mSceneObjects.FindAll(
				so =>
				{
					if(typeof(ISceneObjectExtractable).IsAssignableFrom(so.GetType()))
					{
						return (so as ISceneObjectExtractable).EnableExtraction;
					}

					return false;
				}
			).ForEach(
				so =>
				{
					(so as ISceneObjectExtractable).Extract();
				}
			);
		}
		#endregion Extraction
	}
}
