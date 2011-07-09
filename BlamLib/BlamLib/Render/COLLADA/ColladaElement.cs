/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.Reflection;

namespace BlamLib.Render.COLLADA
{
	namespace Enums
	{
		public enum ColladaElementType
		{
			/// <summary>
			/// Used for helper elements that are not officially defined in the schema
			/// </summary>
			Undefined,
			/// <summary>
			/// Used when validating to run tests regardless of the parent type
			/// </summary>
			All,

			Core_Animation,
			Core_AnimationClip,
			Core_Channel,
			Core_InstanceAnimation,
			Core_LibraryAnimationClips,
			Core_LibraryAnimations,
			Core_Sampler,

			Core_Camera,
			Core_Imager,
			Core_InstanceCamera,
			Core_LibraryCameras,
			Core_Optics,
			Core_Orthographic,
			Core_Perspective,

			Core_Controller,
			Core_InstanceController,
			Core_Joints,
			Core_LibraryControllers,
			Core_Morph,
			Core_Skeleton,
			Core_Skin,
			Core_Targets,
			Core_VertexWeights,

			Core_Accessor,
			Core_BoolArray,
			Core_FloatArray,
			Core_IDREFArray,
			Core_IntArray,
			Core_NameArray,
			Core_Param,
			Core_Source,
			Core_InputShared,
			Core_InputUnshared,

			Core_Extra,
			Core_Technique,
			Core_TechniqueCommon,

			Core_ControlVertices,
			Core_Geometry,
			Core_InstanceGeometry,
			Core_LibraryGeometries,
			Core_Lines,
			Core_Linestrips,
			Core_Mesh,
			Core_Polygons,
			Core_Polylist,
			Core_Spline,
			Core_Triangles,
			Core_Trifans,
			Core_Tristrips,
			Core_Vertices,

			Core_Ambient,
			Core_Color,
			Core_Directional,
			Core_InstanceLight,
			Core_LibraryLights,
			Core_Light,
			Core_Point,
			Core_Spot,

			Core_Asset,
			Core_COLLADA,
			Core_Contributor,

			Core_InstanceNode,
			Core_InstanceVisualScene,
			Core_LibraryNodes,
			Core_LibraryVisualScenes,
			Core_Node,
			Core_Scene,
			Core_VisualScene,

			Core_Lookat,
			Core_Matrix,
			Core_Rotate,
			Core_Scale,
			Core_Skew,
			Core_Translate,


			Fx_BindVertexInput,
			Fx_Effect,
			Fx_InstanceEffect,
			Fx_LibraryEffects,
			Fx_Semantic,
			Fx_Technique,
			Fx_TechniqueHint,

			Fx_Bind,
			Fx_BindMaterial,
			Fx_InstanceMaterial,
			Fx_LibraryMaterials,
			Fx_Material,

			Fx_Annotate,
			Fx_Modifier,
			Fx_Newparam,
			Fx_Param,
			Fx_Setparam,

			Fx_ProfileCOMMON,

			Fx_Blinn,
			Fx_CommonColorOrTextureType,
			Fx_CommonFloatOrParamType,
			Fx_Constant,
			Fx_Lambert,
			Fx_Phong,

			Fx_Image,
			Fx_LibraryImages,
			Fx_Sampler1D,
			Fx_Sampler2D,
			Fx_Sampler3D,
			Fx_SamplerCUBE,
			Fx_SamplerDEPTH,
			Fx_SamplerRECT,
			Fx_Surface,
		};
	};
	/// <summary>
	/// Base element class that all collada elements are derived from.
	/// 
	/// This class contains a list of ColladaObject's and validations tests, so that unique validation functions for each element are not necessary.
	/// </summary>
	[Serializable]
	public class ColladaElement
	{
		#region Class Members
		[XmlIgnore]
		protected readonly Enums.ColladaElementType elementType;
		/// <summary>
		/// Overriden in derived classes to provide access to a standard name format for each element that has a name
		/// </summary>
		[XmlIgnore]
		public static readonly string ElementIDFormat = null;
		// TODO: change all derived classes to override this, not declare a new static by the same name, then change this to be 'abstract'
		public string GetElementIdFormat()
		{
			return null;
		}

		protected List<ColladaObject> Fields = new List<ColladaObject>();
		protected List<Validation.ColladaValidationTest> ValidationTests = new List<Validation.ColladaValidationTest>();
		#endregion

		#region Constructors
		public ColladaElement()
		{
			elementType = Enums.ColladaElementType.Undefined;
		}
		/// <summary>
		/// Initialises the base element class with a specific type
		/// </summary>
		/// <param name="type">The COLLADA element type of the derived class</param>
		public ColladaElement(Enums.ColladaElementType type)
		{
			elementType = type;
		}
		#endregion

		/// <summary>
		/// Checks whether this element conforms to the schema
		/// Throws a validation exception when errors are found
		/// </summary>
		/// <param name="parent_type">The collada element type of the parent</param>
		public void ValidateElement(Enums.ColladaElementType parent_type)
		{
			// run each validation test
			foreach (var test in ValidationTests)
			{
				ColladaValidationException exception = test.Validate(parent_type);
				// if the test returned an exception, create a detail list for it then throw it
				if (exception != null)
				{
					exception.ElementDetails = GetDetailList();

					throw exception;
				}
			}
			// validate children
			foreach (var field in Fields)
				field.ValidateField(elementType);
		}

		/// <summary>
		/// Uses reflection to get a string list of all the elements properties
		/// </summary>
		/// <returns></returns>
		public List<string> GetDetailList()
		{
			List<string> string_list = new List<string>();

			// add the element type
			string_list.Add(String.Format("DETAIL: Type : {0}\n", this.GetType().Name));

			string detail_format_string = "DETAIL: Property : {0} : {1}";

			// work through all of the public properties of this element
			PropertyInfo[] properties = this.GetType().GetProperties();
			foreach (PropertyInfo property in properties)
			{
				// get the property value
				object value = property.GetValue(this, null);

				// if the value is null, say as much and move on
				if (value == null)
				{
					string_list.Add(String.Format(detail_format_string, property.Name, "null"));
					continue;
				}

				// if the value has the IList interface it has the Count property
				// so add a string showing how many elements are in the list
				if (value.GetType().GetInterface("IList") != null)
				{
					PropertyInfo count_property = value.GetType().GetProperty("Count");
					if (count_property != null)
						string_list.Add(String.Format(detail_format_string, property.Name, count_property.GetValue(value, null)));
				}
				else
					string_list.Add(String.Format(detail_format_string, property.Name, value));
			}
			return string_list;
		}
	};
}