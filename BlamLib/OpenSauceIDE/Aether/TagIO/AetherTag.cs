using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Blam;
using BlamLib.Managers;
using BlamLib.TagInterface;
using OpenSauceIDE.Aether.SceneObject;
using System.IO;

namespace OpenSauceIDE.Aether.TagIO
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Class for simple access to a tag. </summary>
	/// <typeparam name="T">	The tag group to access. </typeparam>
	///-------------------------------------------------------------------------------------------------
	public class AetherTag : SceneObjectBase
	{
		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="tag_index">	Index the tag belongs to. </param>
		/// <param name="index">		The index if the tag. </param>
		///-------------------------------------------------------------------------------------------------
		public AetherTag(SceneObjectBase parent, TagIndex tagIndex, DatumIndex index)
		{
			mTagIndex = tagIndex;
			mTagManager = tagIndex[index];
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="tag_index">	Index the tag belongs to. </param>
		/// <param name="index">		The index if the tag. </param>
		///-------------------------------------------------------------------------------------------------
		public AetherTag(TagIndex tagIndex, DatumIndex index)
			: this(null, tagIndex, index)
		{ }
		#endregion Constructor

		#region Methods
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the tag definition. </summary>
		/// <typeparam name="T">	Definition type parameter. </typeparam>
		/// <returns>	The tag definition. </returns>
		///-------------------------------------------------------------------------------------------------
		public T GetTagDefinition<T>()
			where T : Definition
		{
			return mTagManager.TagDefinition as T;
		}
		#endregion

		#region Properties
		public override string ObjectName
		{
			get { return Path.GetFileNameWithoutExtension(Location); }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	The tag datum index for this tag. </summary>
		/// <value>	The index. </value>
		///-------------------------------------------------------------------------------------------------
		public DatumIndex Index
		{
			get { return mTagManager.TagIndex; }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the manager for tag. </summary>
		/// <value>	The tag manager. </value>
		///-------------------------------------------------------------------------------------------------
		public BlamLib.Managers.TagManager TagManager
		{
			get { return mTagManager; }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	The relative location of this tag in the tags directory. </summary>
		/// <value>	The tags location. </value>
		///-------------------------------------------------------------------------------------------------
		public string Location
		{
			get { return mTagManager.Name; }
		}
		#endregion Properties

		#region Fields
		protected BlamLib.Managers.TagIndex mTagIndex;
		protected BlamLib.Managers.TagManager mTagManager;
		#endregion Fields
	};
}
