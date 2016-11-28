/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Enums
{
	[SerializableAttribute()]
	[XmlTypeAttribute(Namespace = "http://www.collada.org/2005/11/COLLADASchema")]
	public enum ColladaInputSharedSemantic
	{
		BINORMAL,
		COLOR,
		CONTINUITY,
		IMAGE,
		INPUT,
		IN_TANGENT,
		INTERPOLATION,
		INV_BIND_MATRIX,
		JOINT,
		LINEAR_STEPS,
		MORPH_TARGET,
		MORPH_WEIGHT,
		NORMAL,
		OUTPUT,
		OUT_TANGENT,
		POSITION,
		TANGENT,
		TEXBINORMAL,
		TEXCOORD,
		TEXTANGENT,
		UV,
		VERTEX,
		WEIGHT
	}
}