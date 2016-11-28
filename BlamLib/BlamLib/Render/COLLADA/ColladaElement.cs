/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.Reflection;
using System.Collections;

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
		/// Returns a string formatted to the ID format expected for a specific element type
		/// </summary>
		/// <typeparam name="T">The element type to format an ID for</typeparam>
		/// <param name="id">The ID to format</param>
		/// <returns></returns>
		public static string FormatID<T>(string id)
		{
			PropertyInfo property = typeof(T).GetProperty("ID");

			if (property == null)
				throw new ColladaException("COLLADA EXCEPTION: attempted to format an ID for an element that doesn't have an ID property");

			var id_attribute = property.GetCustomAttribute<ColladaIDAttribute>();

			if (id_attribute == null)
				throw new ColladaException("COLLADA EXCEPTION: an ID property has no formatting string defined");

			return id_attribute.FormatID(id);
		}

		public static bool TestFormat<T>(string id)
		{
			PropertyInfo property = typeof(T).GetProperty("ID");

			if (property == null)
				throw new ColladaException("COLLADA EXCEPTION: attempted to unformat an ID for an element that doesn't have an ID property");

			var id_attribute = property.GetCustomAttribute<ColladaIDAttribute>();

			if (id_attribute == null)
				throw new ColladaException("COLLADA EXCEPTION: an ID property has no formatting string defined");

			return id_attribute.TestFormatted(id);
		}

		#region Validation
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

			const string detail_format_string = "DETAIL: Property : {0} : {1}";

			// work through all of the public properties of this element
			PropertyInfo[] properties = this.GetType().GetProperties();
			foreach (PropertyInfo property in properties)
			{
				// get the property value
				object value = property.GetValue(this, null);
				string value_str = "null";

				// if the value is null, say as much and move on
				if (value == null)
					value_str = "null";
				// if the value has the IList interface it has the Count property
				// so add a string showing how many elements are in the list
				else if (value.GetType().GetInterface("IList") != null)
				{
					PropertyInfo count_property = value.GetType().GetProperty("Count");
					if (count_property != null)
						value_str = count_property.GetValue(value, null).ToString();
				}
				else
					value_str = value.ToString();

				string_list.Add(String.Format(detail_format_string, property.Name, value_str));
			}
			return string_list;
		}

		/// <summary>
		/// Gets a list containing all of this elements children
		/// </summary>
		/// <returns></returns>
		List<ColladaElement> GetChildElements()
		{
			List<ColladaElement> children = new List<ColladaElement>();

			// enumerate all of the elements fields
			foreach (var field in Fields)
			{
				// if the field type is not derived from ColladaElement of its value is null, continue
				if (!field.GetObjectType().IsSubclassOf(typeof(ColladaElement)) || (field.GetValue() == null))
					continue;

				// if the actual values type is ColladaElement, add it to the list (it could be a list, which we can't add directly)
				if (field.GetValue().GetType().IsSubclassOf(typeof(ColladaElement)))
					children.Add(field.GetValue() as ColladaElement);
				// if the value is a list add each elment individually
				else if (field.GetValue().GetType().GetInterface("IList") != null)
				{
					// the list derives from IEnumerable so we can get a generic enumerator from it
					IEnumerable element_list = field.GetValue() as IEnumerable;

					if (element_list == null)
						continue;

					// get a generic enumerator, enumate through the list add add each entry assuming it is a ColladaElement
					IEnumerator enumerator = element_list.GetEnumerator();
					ColladaElement element = null;
					while (enumerator.MoveNext() && (element = (enumerator.Current as ColladaElement)) != null)
						children.Add(element);
				}
			}

			return children;
		}

		/// <summary>
		/// Returns a list of properties with a matching attribute type
		/// </summary>
		/// <param name="attribute_type">The type of attribute to look for</param>
		/// <returns></returns>
		List<PropertyInfo> GetPropertiesByAttribute(Type attribute_type)
		{
			List<PropertyInfo> property_list = new List<PropertyInfo>();

			// get all of this elements properties
			PropertyInfo[] properties = this.GetType().GetProperties() as PropertyInfo[];

			foreach (PropertyInfo property in properties)
			{
				// find out of the property has the required attribute
				var id_attributes = property.GetCustomAttributes(attribute_type, false);

				if (id_attributes == null || id_attributes.Length == 0)
					continue;

				property_list.Add(property);
			}
			return property_list;
		}

		/// <summary>
		/// Returns a string list containing all of the ID's of this element and its children
		/// </summary>
		/// <returns></returns>
		public List<string> GetIDs()
		{
			List<string> ids = new List<string>();

			List<PropertyInfo> property = GetPropertiesByAttribute(typeof(ColladaIDAttribute));
			if (property.Count != 0)
			{
				string id = property[0].GetValue(this, null) as string;
				if(id != null && id.Length > 0)
					ids.Add(id);
			}

			foreach (var child in GetChildElements())
				ids.AddRange(child.GetIDs());

			return ids;
		}

		/// <summary>
		/// Checks that the local id references in the element are valid.
		/// 
		/// Throws a validation exception if an ID reference is not found in the local ID list.
		/// </summary>
		/// <param name="local_ids">A list of all the ID's in the COLLADA file</param>
		public void ValidateLocalURIs(List<string> local_ids)
		{
			List<PropertyInfo> properties = GetPropertiesByAttribute(typeof(ColladaURIAttribute));

			foreach (var property in properties)
			{
				string value = property.GetValue(this, null) as string;

				if(value == null || value.Length == 0)
					continue;

				// if the id contains :// it is not local so ignore it
				if (value.Contains(@"://"))
					continue;
				
				// fragments have a hash at the start which needs removing
				if (value.StartsWith("#"))
					value = value.Remove(0, 1);

				if (!local_ids.Contains(value))
					throw new ColladaValidationException("COLLADA VALIDATION: URI reference a local ID that does not exist", GetDetailList());
			}

			foreach (var child in GetChildElements())
				child.ValidateLocalURIs(local_ids);
		}
		#endregion
	};
}