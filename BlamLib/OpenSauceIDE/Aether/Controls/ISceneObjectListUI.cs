using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.Aether.Controls
{
    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Interface for scene object user interface controls. </summary>
    ///-------------------------------------------------------------------------------------------------
    public interface ISceneObjectListUI : ISceneObjectUI
    {
        ///-------------------------------------------------------------------------------------------------
        /// <summary>   Adds a scene object to the list. </summary>
        /// <param name="sceneObject">  The scene object. </param>
        ///-------------------------------------------------------------------------------------------------
        void AddSceneObject(SceneObject.SceneObjectBase sceneObject);
    }
}
