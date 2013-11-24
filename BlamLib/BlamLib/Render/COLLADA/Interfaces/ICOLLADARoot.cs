/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlamLib.Render.COLLADA
{
	public interface ICOLLADARoot
	{
		List<ICOLLADAObject> GetObjectList();
		COLLADA.Core.ColladaAsset GetAssetElement();
	}
}
