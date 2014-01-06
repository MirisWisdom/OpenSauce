using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Blam;
using BlamLib.Managers;
using BlamLib;

namespace OpenSauceIDE.Aether.TagIO
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Aether tag index handler. </summary>
	/// <typeparam name="T">	The type of tag index. </typeparam>
	///-------------------------------------------------------------------------------------------------
	public class AetherTagIndexHandler<T> : System.IDisposable
		where T : TagIndexBase
	{
		#region Fields
		DatumIndex mIndexHandle;
		public T mIndexInterface;
		#endregion Fields

		#region Properties
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the tag index interface. </summary>
		/// <value>	The tag index interface. </value>
		///-------------------------------------------------------------------------------------------------
		public T IndexInterface
		{
			get { return mIndexInterface; }
			private set { mIndexInterface = value; }
		}
		#endregion Properties

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="game">	The game engine version. </param>
		/// <param name="path">	Full pathname of the tags directory. </param>
		///-------------------------------------------------------------------------------------------------
		public AetherTagIndexHandler(BlamVersion game, string path)
		{
			mIndexHandle = BlamLib.Program.GetManager(game).OpenTagIndex(game, path);
			mIndexInterface = BlamLib.Program.GetTagIndex(mIndexHandle) as T;
		}
		#endregion Constructor

		#region IDisposable Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Performs application-defined tasks associated with freeing, releasing, or resetting
		/// 	unmanaged resources.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		public void Dispose()
		{
			if (mIndexHandle != DatumIndex.Null)
			{
				mIndexInterface = null;
				mIndexHandle = DatumIndex.Null;
			}
		}
		#endregion

		#region Operators
		public static implicit operator T(AetherTagIndexHandler<T> handler)
		{
			return handler.IndexInterface;
		}
		#endregion Operators
	}
}
