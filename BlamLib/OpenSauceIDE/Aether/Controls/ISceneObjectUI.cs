using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.Aether.Controls
{
    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Interface for scene object user interface controls. </summary>
    ///-------------------------------------------------------------------------------------------------
    public interface ISceneObjectUI
    {
        ///-------------------------------------------------------------------------------------------------
        /// <summary>   Sets the scene object associated with the control. </summary>
        /// <param name="sceneObject">  The scene object. </param>
        ///-------------------------------------------------------------------------------------------------
        void SetSceneObject(SceneObject.SceneObjectBase sceneObject);
    }
}
