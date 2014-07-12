using BlamLib;
using BlamLib.Blam;
using BlamLib.Managers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.ModelExtractor.TagIO
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Tag index handler. </summary>
    ///
    /// <typeparam name="T">	The type of tag index. </typeparam>
    public class ExtractorTagIndexHandler<T> : System.IDisposable
        where T : TagIndexBase
    {
        #region Fields
        BlamVersion mGameVersion;
        DatumIndex mIndexHandle;
        public T mIndexInterface;
        #endregion Fields

        #region Properties
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets or sets the tag index interface. </summary>
        ///
        /// <value> The tag index interface. </value>
        public T IndexInterface
        {
            get { return mIndexInterface; }
            private set { mIndexInterface = value; }
        }
        #endregion Properties

        #region Constructor
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Constructor. </summary>
        ///
        /// <param name="game">     The game engine version. </param>
        /// <param name="path">     Full pathname of the tags root directory. </param>
        /// <param name="tags_dir"> (Optional) the tags folder name. </param>
        public ExtractorTagIndexHandler(BlamVersion game, string path, string tags_dir = "tags")
        {
            mGameVersion = game;
            mIndexHandle = BlamLib.Program.GetManager(mGameVersion).OpenTagIndex(game, path, tags_dir);
            mIndexInterface = BlamLib.Program.GetTagIndex(mIndexHandle) as T;
        }
        #endregion Constructor

        #region IDisposable Members
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        ///     Performs application-defined tasks associated with freeing, releasing, or resetting
        ///     unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            if (TagIndex.IsValid(mIndexHandle))
            {
                BlamLib.Program.GetManager(mGameVersion).CloseTagIndex(mIndexHandle);
                mIndexInterface = null;
                mIndexHandle = DatumIndex.Null;
            }
        }
        #endregion

        #region Operators
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   TagIndex casting operator. </summary>
        ///
        /// <param name="handler">  The tag index handler. </param>
        public static implicit operator T(ExtractorTagIndexHandler<T> handler)
        {
            return handler.IndexInterface;
        }
        #endregion Operators
    }
}
