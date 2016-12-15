/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using BlamLib.Blam;
using BlamLib.Render.COLLADA.Validation;
using BlamLib.Messaging;
using BlamLib.Bitmaps;

namespace BlamLib.Render.COLLADA
{
	public interface IColladaShaderDataProvider : IColladaDataProvider
	{
		#region Images
		ColladaExporter.ImageList GetImages();
		#endregion Images

		#region Effects
		ColladaExporter.EffectList GetEffects();
		ColladaExporter.EffectList GetEffectsMap();
		#endregion Effects

		#region Materials
		ColladaExporter.MaterialList GetMaterials();
		#endregion Materials
	}

	public interface IColladaSettings
	{
		bool Overwrite { get; }
		string RootDirectory { get; }
		AssetFormat BitmapFormat { get; }
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Base class for exporting a COLLADA file. </summary>
	///-------------------------------------------------------------------------------------------------
	public abstract class ColladaExporter
		: IMessageSource
	{
		#region Messaging
		protected IMessageHandler mMessageHandler = new MessageHandler();

		public event EventHandler<MessageArgs> MessageSent
		{
			add { mMessageHandler.MessageSent += value; }
			remove { mMessageHandler.MessageSent -= value; }
		}
		#endregion

		#region Error Handling
		void ValidatorErrorOccured(object sender, ColladaErrorEventArgs e)
		{
			mMessageHandler.SendMessage(e.ErrorMessage);
		}
		#endregion Error Handling

		#region Class Members
		protected IColladaSettings colladaSettings = null;
		protected ColladaFile COLLADAFile = new ColladaFile();

		protected IColladaShaderDataProvider mShaderDataProvider = null;
		
		protected List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		protected List<Core.ColladaController> listController = new List<Core.ColladaController>();
		protected List<Fx.ColladaImage> listImage = new List<Fx.ColladaImage>();
		protected List<Fx.ColladaEffect> listEffect = new List<Fx.ColladaEffect>();
		protected List<Fx.ColladaMaterial> listMaterial = new List<Fx.ColladaMaterial>();
		protected List<Core.ColladaNode> listBone = new List<Core.ColladaNode>();
		protected List<ColladaBoneMatrix> listBoneMatrix = new List<ColladaBoneMatrix>();
		protected List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		protected List<Core.ColladaNode> listMarker = new List<Core.ColladaNode>();
		#endregion

		#region Constructor
		protected ColladaExporter(IColladaSettings settings)
		{
			colladaSettings = settings;
		}
		#endregion Constructor

		#region Data Providers
		protected List<IColladaDataProvider> mDataProviders = new List<IColladaDataProvider>();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a data provider. </summary>
		/// <param name="provider">	The provider. </param>
		///-------------------------------------------------------------------------------------------------
		public void AddDataProvider(IColladaDataProvider provider)
		{
			mDataProviders.Add(provider);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Clears the data providers. </summary>
		///-------------------------------------------------------------------------------------------------
		public void ClearDataProviders()
		{
			mDataProviders.Clear();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets data provider. </summary>
		/// <typeparam name="T">	IColladaDataProvider type parameter. </typeparam>
		/// <returns>	The data provider&lt; t&gt; </returns>
		///-------------------------------------------------------------------------------------------------
		public T GetDataProvider<T>()
			where T : class, IColladaDataProvider
		{
			return mDataProviders.Find(provider => provider is T) as T;
		}
		#endregion Data Providers

		#region Element Creation
		#region Create Node
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a node. </summary>
		/// <param name="name">	Name for the node. </param>
		/// <param name="sid"> 	The SID of the node. </param>
		/// <param name="id">  	ID for the node. </param>
		/// <param name="type">	The type of node. </param>
		/// <returns>	The new node. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Core.ColladaNode CreateNode(string name, string sid, string id, Enums.ColladaNodeType type)
		{
			var node = new Core.ColladaNode();
			node.Name = name;
			node.sID = ColladaUtilities.FormatName(sid, " ", "_");

			if (!String.IsNullOrEmpty(id))
			{
				node.ID = id;
			}
			node.Type = type;

			return node;
		}
		#endregion
		#region Create Source
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a generic source element for geometric data. </summary>
		/// <exception cref="ColladaException">	Thrown when a Collada error condition occurs. </exception>
		/// <param name="semantic">   	The use for the data in the source element. </param>
		/// <param name="geometry_id">	Name of the geometry the element contains data for. </param>
		/// <param name="set">		  	Data set that the sources data is intended for. </param>
		/// <returns>	A ColladaSource element. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Core.ColladaSource CreateSource(Enums.ColladaInputSharedSemantic semantic,
			string geometry_id,
			uint set)
		{
			var source = new Core.ColladaSource();

			source.FloatArray = new Core.ColladaFloatArray();
			source.TechniqueCommon = new Core.ColladaTechniqueCommon();
			source.TechniqueCommon.Accessor = new Core.ColladaAccessor();
			source.TechniqueCommon.Accessor.Param = new List<Core.ColladaParam>();

			Action<string[]> addAccessorParams =
				names =>
				{
					foreach (var name in names)
					{
						Core.ColladaParam param = new Core.ColladaParam();

						param.Name = name;
						param.Type = "float";

						source.TechniqueCommon.Accessor.Param.Add(param);
					}
				};

			string source_id = "";
			switch (semantic)
			{
				case Enums.ColladaInputSharedSemantic.POSITION:
					source_id = String.Concat(geometry_id, "-positions");
					addAccessorParams(new string[] { "X", "Y", "Z" });
					break;
				case Enums.ColladaInputSharedSemantic.NORMAL:
					source_id = String.Concat(geometry_id, "-normals");
					addAccessorParams(new string[] { "X", "Y", "Z" });
					break;
				case Enums.ColladaInputSharedSemantic.BINORMAL:
					source_id = String.Concat(geometry_id, "-binormals");
					addAccessorParams(new string[] { "X", "Y", "Z" });
					break;
				case Enums.ColladaInputSharedSemantic.TANGENT:
					source_id = String.Concat(geometry_id, "-tangents");
					addAccessorParams(new string[] { "X", "Y", "Z" });
					break;
				case Enums.ColladaInputSharedSemantic.TEXCOORD:
					source_id = String.Format("{0}-texcoord{1}", geometry_id, set);
					addAccessorParams(new string[] { "S", "T" });
					break;
				default:
					throw new ColladaException(String.Format(
						ColladaExceptionStrings.ImplimentationBug,
						"invalid semantic enum passed to CreateSource function"));
			}

			source.ID = source_id;
			source.FloatArray.ID = String.Concat(source.ID, "-array");
			source.TechniqueCommon.Accessor.Source = ColladaUtilities.BuildUri(source.FloatArray.ID);

			return source;
		}
		#endregion Create Source
		#region Create Vertices
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates a vertices element that defines which source element contains the position data.
		/// </summary>
		/// <param name="geometry_id">	The id of the geometry the element is associated with. </param>
		/// <param name="source_id">
		/// 	The id of the source element that contains the position data.
		/// </param>
		/// <returns>	A ColladaVertices element. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Core.ColladaVertices CreateVertices(string geometry_id, string source_id)
		{
			var vertices = new Core.ColladaVertices();
			vertices.ID = String.Concat(geometry_id, "-vertices");
			vertices.Input = new List<Core.ColladaInputUnshared>();
			vertices.Input.Add(new Core.ColladaInputUnshared());
			vertices.Input[0].Semantic = Enums.ColladaInputSharedSemantic.POSITION;
			vertices.Input[0].Source = ColladaUtilities.BuildUri(source_id);

			return vertices;
		}
		#endregion Create Vertices
		#region Create Geometry
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic geometry information class. </summary>
		///-------------------------------------------------------------------------------------------------
		protected class Geometry
		{
			[Flags]
			public enum VertexComponent
			{
				POSITION,
				NORMAL,
				BINORMAL,
				TANGENT,
				TEXCOORD,
				WEIGHT
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Generic vertex information class. </summary>
			///-------------------------------------------------------------------------------------------------
			public class Vertex
			{
				public LowLevel.Math.real_point3d Position { get; private set; }
				public LowLevel.Math.real_vector3d Normal { get; private set; }
				public LowLevel.Math.real_vector3d Binormal { get; private set; }
				public LowLevel.Math.real_vector3d Tangent { get; private set; }
				public List<LowLevel.Math.real_point2d> Texcoords { get; private set; }

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Adds a texture coordinate. </summary>
				/// <param name="texcoord">	The texture coordinate. </param>
				///-------------------------------------------------------------------------------------------------
				public void AddTexcoord(LowLevel.Math.real_point2d texcoord)
				{
					if (Texcoords == null)
						Texcoords = new List<LowLevel.Math.real_point2d>();

					Texcoords.Add(texcoord);
				}

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Constructor. </summary>
				/// <param name="position">	The position. </param>
				/// <param name="normal">  	The normal. </param>
				/// <param name="binormal">	The binormal. </param>
				/// <param name="tangent"> 	The tangent. </param>
				///-------------------------------------------------------------------------------------------------
				public Vertex(LowLevel.Math.real_point3d position
					, LowLevel.Math.real_vector3d normal
					, LowLevel.Math.real_vector3d binormal
					, LowLevel.Math.real_vector3d tangent)
				{
					Position = position;
					Normal = normal;
					Binormal = binormal;
					Tangent = tangent;
				}

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Constructor. </summary>
				/// <param name="position">	The position. </param>
				///-------------------------------------------------------------------------------------------------
				public Vertex(LowLevel.Math.real_point3d position
					, LowLevel.Math.real_vector3d normal)
				{
					Position = position;
					Normal = normal;
				}

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Constructor. </summary>
				/// <param name="position">	The position. </param>
				///-------------------------------------------------------------------------------------------------
				public Vertex(LowLevel.Math.real_point3d position)
				{
					Position = position;
				}
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Generic part information class. </summary>
			///-------------------------------------------------------------------------------------------------
			public class Part
			{
				public string MaterialName { get; private set; }
				public List<int> Indices { get; private set; }

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Adds vertex indices. </summary>
				/// <param name="indices">	A variable-length parameters list containing indices. </param>
				///-------------------------------------------------------------------------------------------------
				public void AddIndices(params int[] indices)
				{
					if (Indices == null)
						Indices = new List<int>();

					Indices.AddRange(indices);
				}

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Adds vertex indices. </summary>
				/// <param name="indices">	A list containing indices. </param>
				///-------------------------------------------------------------------------------------------------
				public void AddIndices(List<int> indices)
				{
					if (Indices == null)
						Indices = new List<int>();

					Indices.AddRange(indices);
				}

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Constructor. </summary>
				/// <param name="material">	The material for the part. </param>
				///-------------------------------------------------------------------------------------------------
				public Part(string material)
				{
					MaterialName = material;
				}
			}

			public string Name { get; private set; }
			public uint TexCoordCount { get; private set; }
			public VertexComponent VertexComponentFlags { get; private set; }
			public List<Vertex> Vertices { get; private set; }
			public List<Part> Parts { get; private set; }

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Constructor. </summary>
			/// <param name="name">			   	The name of the geometry. </param>
			/// <param name="texCoordCount">   	Number of texture coordinates. </param>
			/// <param name="vertexComponents">	The vertex component mask. </param>
			///-------------------------------------------------------------------------------------------------
			public Geometry(string name, uint texCoordCount, VertexComponent vertexComponentFlags)
			{
				Name = name;
				TexCoordCount = texCoordCount;
				VertexComponentFlags = vertexComponentFlags;
				Vertices = new List<Vertex>();
				Parts = new List<Part>();
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds a vertex. </summary>
			/// <param name="vertex">	The vertex. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddVertex(Vertex vertex)
			{
				Vertices.Add(vertex);
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds a part. </summary>
			/// <param name="part">	The part. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddPart(Part part)
			{
				Parts.Add(part);
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Gets whether the vertex has the specified components. </summary>
			/// <param name="testFlags">	The test flags. </param>
			/// <returns>	true if it does, false if it doesn't. </returns>
			///-------------------------------------------------------------------------------------------------
			public bool GetVertsHave(VertexComponent testFlags)
			{
				return (VertexComponentFlags & testFlags) == testFlags;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a geometry element. </summary>
		/// <param name="geometryData">		 	The geometry data. </param>
		///-------------------------------------------------------------------------------------------------
		protected void CreateGeometry(Geometry geometryData)
		{
			var geometry = new Core.ColladaGeometry();

			// initialise the geometry attributes
			geometry.Name = geometryData.Name;
			geometry.ID = geometryData.Name;

			// create a new mesh element
			geometry.Mesh = new Core.ColladaMesh();
			geometry.Mesh.Source = new List<Core.ColladaSource>();

			Core.ColladaSource position_source = null;
			Core.ColladaSource normal_source = null;
			Core.ColladaSource binormal_source = null;
			Core.ColladaSource tangent_source = null;
			List<Core.ColladaSource> texcoord_sources = null;

			// create the source elements according to the component mask
			if (geometryData.GetVertsHave(Geometry.VertexComponent.POSITION))
			{
				position_source = CreateSource(Enums.ColladaInputSharedSemantic.POSITION, geometry.ID, 0);
				geometry.Mesh.Vertices = CreateVertices(geometry.ID, position_source.ID);
			}
			if (geometryData.GetVertsHave(Geometry.VertexComponent.NORMAL))
			{
				normal_source = CreateSource(Enums.ColladaInputSharedSemantic.NORMAL, geometry.ID, 0);
			}
			if (geometryData.GetVertsHave(Geometry.VertexComponent.BINORMAL))
			{
				binormal_source = CreateSource(Enums.ColladaInputSharedSemantic.BINORMAL, geometry.ID, 0);
			}
			if (geometryData.GetVertsHave(Geometry.VertexComponent.TANGENT))
			{
				tangent_source = CreateSource(Enums.ColladaInputSharedSemantic.TANGENT, geometry.ID, 0);
			}
			
			if (geometryData.GetVertsHave(Geometry.VertexComponent.TEXCOORD)
				&& (geometryData.TexCoordCount > 0))
			{
				texcoord_sources = new List<Core.ColladaSource>();
				for (uint i = 0; i < geometryData.TexCoordCount; i++)
				{
					texcoord_sources.Add(CreateSource(Enums.ColladaInputSharedSemantic.TEXCOORD, geometry.ID, i));
				}
			}

			// add vertex information to the source arrays
			foreach(var vertex in geometryData.Vertices)
			{
				//RealPoint3D   position
				if (position_source != null)
				{
					position_source.FloatArray.Add(vertex.Position);
				}
				//RealVector3D  normal
				if (normal_source != null)
				{
					normal_source.FloatArray.Add(vertex.Normal);
				}
				//RealVector3D  binormal
				if (binormal_source != null)
				{
					binormal_source.FloatArray.Add(vertex.Binormal);
				}
				//RealVector3D  tangent
				if (tangent_source != null)
				{
					binormal_source.FloatArray.Add(vertex.Tangent);
				}
				//RealPoint2D  texcoords
				if (texcoord_sources != null)
				{
					for (int i = 0; i < geometryData.TexCoordCount; i++)
					{
						if (i >= vertex.Texcoords.Count)
						{
							texcoord_sources[i].FloatArray.Add(0, 1);
						}
						else
						{
							texcoord_sources[i].FloatArray.Add(vertex.Texcoords[i].X, vertex.Texcoords[i].Y);
						}
					}
				}
			};

			// create triangle sets for the geometry parts
			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			foreach(var part in geometryData.Parts)
			{
				var part_triangles = new Core.ColladaTriangles();
				part_triangles.Material = ColladaUtilities.FormatName(part.MaterialName, " ", "_");
				part_triangles.Input = new List<Core.ColladaInputShared>();

				Action<Enums.ColladaInputSharedSemantic, string, uint> addInputFunc =
					(semantic, id, set) =>
					{
						Core.ColladaInputShared input = new Core.ColladaInputShared();
						input.Semantic = semantic;
						input.Source = ColladaUtilities.BuildUri(id);
						input.Offset = 0;
						input.Set = set;
						part_triangles.Input.Add(input);
					};

				// link to data sources
				if (position_source != null)
				{
					addInputFunc(Enums.ColladaInputSharedSemantic.VERTEX, geometry.Mesh.Vertices.ID, 0);
				}

				if (normal_source != null)
				{
					addInputFunc(Enums.ColladaInputSharedSemantic.NORMAL, normal_source.ID, 0);
				}

				if (binormal_source != null)
				{
					addInputFunc(Enums.ColladaInputSharedSemantic.BINORMAL, binormal_source.ID, 0);
				}

				if (tangent_source != null)
				{
					addInputFunc(Enums.ColladaInputSharedSemantic.TANGENT, tangent_source.ID, 0);
				}

				if (texcoord_sources != null)
				{
					for (uint i = 0; i < geometryData.TexCoordCount; i++)
					{
						addInputFunc(Enums.ColladaInputSharedSemantic.TEXCOORD, texcoord_sources[(int)i].ID, i);
					}
				}

				part_triangles.P = new ColladaValueArray<int>();

				part_triangles.Count = (uint)part.Indices.Count / 3;
				part_triangles.P.Add(part.Indices);

				geometry.Mesh.Triangles.Add(part_triangles);
			}

			// update the source data counts and add them to the mesh element
			if (position_source != null)
			{
				position_source.TechniqueCommon.Accessor.SetCount(position_source.FloatArray.Count);
				geometry.Mesh.Source.Add(position_source);
			}
			if (normal_source != null)
			{
				normal_source.TechniqueCommon.Accessor.SetCount(normal_source.FloatArray.Count);
				geometry.Mesh.Source.Add(normal_source);
			}
			if (binormal_source != null)
			{
				binormal_source.TechniqueCommon.Accessor.SetCount(binormal_source.FloatArray.Count);
				geometry.Mesh.Source.Add(binormal_source);
			}
			if (tangent_source != null)
			{
				tangent_source.TechniqueCommon.Accessor.SetCount(tangent_source.FloatArray.Count);
				geometry.Mesh.Source.Add(tangent_source);
			}

			if (texcoord_sources != null)
			{
				foreach (var source in texcoord_sources)
				{
					source.TechniqueCommon.Accessor.SetCount(source.FloatArray.Count);
				}
				geometry.Mesh.Source.AddRange(texcoord_sources);
			}

			// add the complete geometry to the list
			listGeometry.Add(geometry);
		}
		#endregion
		#region Create Controller
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic skin information class. </summary>
		///-------------------------------------------------------------------------------------------------
		protected class Skin
		{
			///-------------------------------------------------------------------------------------------------
			/// <summary>	Generic vertex weight information class. </summary>
			///-------------------------------------------------------------------------------------------------
			public class VertexWeight
			{
				///-------------------------------------------------------------------------------------------------
				/// <summary>	Weight. </summary>
				///-------------------------------------------------------------------------------------------------
				public class Weight
				{
					public int BoneIndex { get; private set; }
					public float BoneWeight { get; private set; }

					public Weight(int bone_index, float bone_weight)
					{
						BoneIndex = bone_index;
						BoneWeight = bone_weight;
					}
				}

				public List<Weight> Weights { get; private set; }

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Default constructor. </summary>
				///-------------------------------------------------------------------------------------------------
				public VertexWeight()
				{
					Weights = new List<Weight>();
				}

				///-------------------------------------------------------------------------------------------------
				/// <summary>	Adds a weight to the vertex. </summary>
				/// <param name="bone_index"> 	Zero-based index of the bone. </param>
				/// <param name="bone_weight">	The bone weight. </param>
				///-------------------------------------------------------------------------------------------------
				public void AddWeight(int bone_index, float bone_weight)
				{
					Weights.Add(new Weight(bone_index, bone_weight));
				}
			}

			public string Name { get; private set; }
			public string GeometryID { get; private set; }
			public List<VertexWeight> VertexWeights { get; private set; }

			public Skin(string name, string geometryID)
			{
				Name = name;
				GeometryID = geometryID;
				VertexWeights = new List<VertexWeight>();
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds a vertex weight. </summary>
			/// <param name="vertexWeight">	The vertex weight. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddVertexWeight(VertexWeight vertexWeight)
			{
				VertexWeights.Add(vertexWeight);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a common technique element with one parameter. </summary>
		/// <param name="source_id">   	The ID of the source element. </param>
		/// <param name="source_count">	The number of elements in the source array. </param>
		/// <param name="param_name">  	The name of the parameter. </param>
		/// <param name="param_type">  	The type of the the parameter. </param>
		/// <param name="stride">	   	the number of elements the parameter takes up (usually 1) </param>
		/// <returns>	The new controller technique common. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Core.ColladaTechniqueCommon CreateControllerTechniqueCommon(string source_id,
			uint source_count, string param_name, string param_type, uint stride)
		{
			var technique = new Core.ColladaTechniqueCommon();
			technique.Accessor = new Core.ColladaAccessor();
			technique.Accessor.Source = ColladaUtilities.BuildUri(source_id);
			technique.Accessor.Count = source_count;
			technique.Accessor.Param = new List<Core.ColladaParam>();
			technique.Accessor.Param.Add(new Core.ColladaParam());

			if (stride != 1)
				technique.Accessor.StrideOverride = stride;

			technique.Accessor.Param[0].Name = param_name;
			technique.Accessor.Param[0].Type = param_type;

			return technique;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates a skin controller from a generic set of vertex weights.
		/// 	
		/// 	This must only be called after the bones have been set up.
		/// </summary>
		/// <param name="skinData">			 	The skin data. </param>
		///-------------------------------------------------------------------------------------------------
		protected void CreateSkinController(Skin skinData)
		{
			// create the controller node
			var controller = new Core.ColladaController();

			// set the controllers name and id
			controller.Name = skinData.Name;
			controller.ID = skinData.Name;

			// create a skin element for the controller and set it to reference the correct geometry
			controller.Skin = new Core.ColladaSkin();
			controller.Skin.SourceAttrib = ColladaUtilities.BuildUri(skinData.GeometryID);

			// create the bind shape matrix
			controller.Skin.BindShapeMatrix = new ColladaValueArray<float>();
			controller.Skin.BindShapeMatrix.Add(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			// create a source element containing the names of all of the bones
			controller.Skin.Source = new List<Core.ColladaSource>();
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[0].ID = controller.ID + "-joints";
			controller.Skin.Source[0].NameArray = new Core.ColladaNameArray();
			controller.Skin.Source[0].NameArray.ID = controller.Skin.Source[0].ID + "-array";

			// create a float array source for the bind pose matrices
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[1].ID = controller.ID + "-bindposes";
			controller.Skin.Source[1].FloatArray = new Core.ColladaFloatArray();
			controller.Skin.Source[1].FloatArray.ID = controller.Skin.Source[1].ID + "-array";

			foreach (var bone in listBoneMatrix)
			{
				controller.Skin.Source[0].NameArray.Add(ColladaUtilities.FormatName(bone.Name, " ", "_"));

				var bind_pose_matrix = SlimDX.Matrix.Invert(bone.TransformMatrixWorld);

				// a bind pose matrix is the inverse of the world transform of the bone
				controller.Skin.Source[1].FloatArray.Add(
					bind_pose_matrix.M11, bind_pose_matrix.M21, bind_pose_matrix.M31, bind_pose_matrix.M41,
					bind_pose_matrix.M12, bind_pose_matrix.M22, bind_pose_matrix.M32, bind_pose_matrix.M42,
					bind_pose_matrix.M13, bind_pose_matrix.M23, bind_pose_matrix.M33, bind_pose_matrix.M43,
					bind_pose_matrix.M14, bind_pose_matrix.M24, bind_pose_matrix.M34, bind_pose_matrix.M44);
			}

			// create a technique that describes what the joint name array is for
			controller.Skin.Source[0].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[0].NameArray.ID, controller.Skin.Source[0].NameArray.Count,
				"JOINT", "Name", 1);

			// create a technique that describes what the bind pose array is for
			controller.Skin.Source[1].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[1].FloatArray.ID, controller.Skin.Source[1].FloatArray.Count,
				"TRANSFORM", "float4x4", 16);

			// create a source element for the vertex weights
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[2].ID = controller.ID + "-weights";
			controller.Skin.Source[2].FloatArray = new Core.ColladaFloatArray();
			controller.Skin.Source[2].FloatArray.ID = controller.Skin.Source[2].ID + "-array";

			// create a technique that describes what the weights array is for
			controller.Skin.Source[2].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[2].FloatArray.ID, 0,
				"WEIGHT", "float", 1);

			// create a joints element that references the joints and bind matrices
			controller.Skin.Joints = new Core.ColladaJoints();
			controller.Skin.Joints.Input = new List<Core.ColladaInputUnshared>();
			controller.Skin.Joints.Input.Add(new Core.ColladaInputUnshared());
			controller.Skin.Joints.Input.Add(new Core.ColladaInputUnshared());
			controller.Skin.Joints.Input[0].Semantic = Enums.ColladaInputSharedSemantic.JOINT;
			controller.Skin.Joints.Input[1].Semantic = Enums.ColladaInputSharedSemantic.INV_BIND_MATRIX;
			controller.Skin.Joints.Input[0].Source = ColladaUtilities.BuildUri(controller.Skin.Source[0].ID);
			controller.Skin.Joints.Input[1].Source = ColladaUtilities.BuildUri(controller.Skin.Source[1].ID);

			// create a vertex weights element that references the joints and weights
			controller.Skin.VertexWeights = new Core.ColladaVertexWeights();
			controller.Skin.VertexWeights.Input = new List<Core.ColladaInputShared>();
			controller.Skin.VertexWeights.Input.Add(new Core.ColladaInputShared());
			controller.Skin.VertexWeights.Input.Add(new Core.ColladaInputShared());
			controller.Skin.VertexWeights.Input[0].Semantic = Enums.ColladaInputSharedSemantic.JOINT;
			controller.Skin.VertexWeights.Input[1].Semantic = Enums.ColladaInputSharedSemantic.WEIGHT;
			controller.Skin.VertexWeights.Input[0].Source = ColladaUtilities.BuildUri(controller.Skin.Source[0].ID);
			controller.Skin.VertexWeights.Input[1].Source = ColladaUtilities.BuildUri(controller.Skin.Source[2].ID);
			controller.Skin.VertexWeights.Input[0].Offset = 0;
			controller.Skin.VertexWeights.Input[1].Offset = 1;

			controller.Skin.VertexWeights.VCount = new ColladaValueArray<int>();
			controller.Skin.VertexWeights.V = new ColladaValueArray<int>();

			// add the vertex weights
			foreach (var vertex_weight in skinData.VertexWeights)
			{
				// add the weight count and weight values to the relevant arrays
				controller.Skin.VertexWeights.VCount.Add(vertex_weight.Weights.Count);

				foreach (var weight in vertex_weight.Weights)
				{
					controller.Skin.Source[2].FloatArray.Add(weight.BoneWeight);
					controller.Skin.VertexWeights.V.Add(weight.BoneIndex, controller.Skin.Source[2].FloatArray.Values.Count - 1);
				}
			}
			controller.Skin.Source[2].TechniqueCommon.Accessor.Count = controller.Skin.Source[2].FloatArray.Count;

			// add the complete controller to the list
			listController.Add(controller);
		}
		#endregion
		#region Create Markers
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic marker information class. </summary>
		///-------------------------------------------------------------------------------------------------
		protected class Marker
		{
			public string Name { get; private set; }
			public LowLevel.Math.real_point3d Translation { get; private set; }
			public LowLevel.Math.real_quaternion Rotation { get; private set; }
			public int BoneIndex { get; private set; }

			public Marker(string name, LowLevel.Math.real_point3d translation, LowLevel.Math.real_quaternion rotation, int bone_index)
			{
				Name = name;
				Translation = translation;
				Rotation = rotation;
				BoneIndex = bone_index;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates nodes from a generic list of markers. </summary>
		///
		/// <param name="markers">			Generic list of markers to create nodes from. </param>
		/// <param name="xColumnVector">	X column vector for correct euler rotation. </param>
		/// <param name="yColumnVector">	Y column vector for correct euler rotation. </param>
		/// <param name="zColumnVector">	Z column vector for correct euler rotation. </param>
		/// <param name="order">			The rotation order. </param>
		protected void CreateMarkers(List<Marker> markers,
				LowLevel.Math.real_vector3d xColumnVector,
				LowLevel.Math.real_vector3d yColumnVector,
				LowLevel.Math.real_vector3d zColumnVector,
				ColladaUtilities.ColladaRotationOrder order)
		{
			foreach (var marker in markers)
			{
				// create a node
				var node = CreateNode(marker.Name, "", "", Enums.ColladaNodeType.NODE);

				// set the nodes position
				var translate = new Core.ColladaTranslate();
				translate.SetTranslate(marker.Translation);
				node.Add(translate);

				// set the nodes rotation
				var rotation = TagInterface.RealQuaternion.ToEuler3D(marker.Rotation);
				node.AddRange(ColladaUtilities.CreateRotationSet(rotation.Roll, rotation.Pitch, rotation.Yaw
					, xColumnVector
					, yColumnVector
					, zColumnVector
					, order)
				);

				// if the bone index is -1 add it to the node list, otherwise add it to a bone
				if (marker.BoneIndex == -1)
				{
					listNode.Add(node);
				}
				else
				{
					listBone[marker.BoneIndex].Add(node);
				}
			}
		}
		#endregion
		#region Create Images
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic image information class. </summary>
		///-------------------------------------------------------------------------------------------------
		public class Image
		{
			public string ImageName { get; private set; }
			public string ImagePath { get; private set; }

			public Image(string path)
			{
				ImageName = Path.GetFileNameWithoutExtension(path);
				ImagePath = path;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	List of images. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ImageList : List<Image> { };

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Populates the image array with image references from the image provider.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		protected void CreateImageList()
		{
			var provider = GetDataProvider<IColladaShaderDataProvider>();
			var images = provider.GetImages();

			// create references to the images
			foreach (var imageDefinition in images)
			{
				var image = new Fx.ColladaImage();
				// set the ID using the bitmaps file name, this must stay consistent so that effects can generate the same ID
				image.ID = imageDefinition.ImageName;

				string fullPath = Path.Combine(colladaSettings.RootDirectory, imageDefinition.ImagePath);
				fullPath = Path.ChangeExtension(fullPath, Bitmaps.Util.GetAssetExtension(colladaSettings.BitmapFormat));

				image.InitFrom = new Fx.ColladaInitFrom();
				image.InitFrom.Text = ColladaUtilities.BuildUri("file://", fullPath);

				listImage.Add(image);
			}
		}
		#endregion
		#region Create Effects
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Dictionary for storing phong shader values. </summary>
		///-------------------------------------------------------------------------------------------------
		public class Phong : Dictionary<string, object>
		{
			public struct Texture
			{
				public string Path;
				public int UVChannel;
				public Enums.ColladaFXOpaqueEnum Opacity;

				public Texture(string path, int uvChannel, Enums.ColladaFXOpaqueEnum opacity)
				{
					Path = path;
					UVChannel = uvChannel;
					Opacity = opacity;
				}

				public Texture(string path, int uvChannel)
					: this(path, uvChannel, Enums.ColladaFXOpaqueEnum.A_ONE)
				{ }
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic effect information class. </summary>
		///-------------------------------------------------------------------------------------------------
		public class Effect
		{
			public string Name { get; private set; }
			public Phong PhongDefinition { get; private set; }
			public ImageList Images { get; private set; }

			public void AddPhongValue(string key, object value)
			{
				if (!PhongDefinition.ContainsKey(key))
				{
					PhongDefinition.Add(key, value);
				}
				else
				{
					PhongDefinition[key] = value;
				}
			}

			public void AddImage(Image imageDefinition)
			{
				if(!Images.Exists(image => image.ImageName == imageDefinition.ImageName))
				{
					Images.Add(imageDefinition);
				}
			}

			public Effect(string name)
			{
				Name = name;
				PhongDefinition = new Phong();
				Images = new ImageList();
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	List of effects. </summary>
		///-------------------------------------------------------------------------------------------------
		public class EffectList : List<Effect> { };

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a texture element from a bitmap datum. </summary>
		/// <param name="bitmap_datum">	The bitmap to create a texture element for. </param>
		/// <param name="uv_channel">  	The texture coordinate channel to use. </param>
		/// <returns>	The new texture. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Fx.ColladaTexture CreateTexture(string imagePath, int uvChannel)
		{
			string imageName = ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(imagePath), " ", "_");

			var texture = new Fx.ColladaTexture();
			texture.Texture = imageName + "-surface-sampler";
			texture.TexCoord = "CHANNEL" + uvChannel.ToString();

			return texture;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a default phong definition with a grey diffuse. </summary>
		/// <returns>	The new default phong. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Fx.ColladaPhong CreateDefaultPhong()
		{
			var phong = new Fx.ColladaPhong();

			// set the default ambient color
			phong.Ambient = new Fx.ColladaCommonColorOrTextureType();
			phong.Ambient.Color = new Core.ColladaColor(0, 0, 0, 1);

			// set the default emission color
			phong.Emission = new Fx.ColladaCommonColorOrTextureType();
			phong.Emission.Color = new Core.ColladaColor(0, 0, 0, 1);

			// set the default reflective color
			phong.Reflective = new Fx.ColladaCommonColorOrTextureType();
			phong.Reflective.Color = new Core.ColladaColor(0, 0, 0, 1);

			// set the default reflectivity
			phong.Reflectivity = new Fx.ColladaCommonFloatOrParamType();
			phong.Reflectivity.Float = new ColladaSIDValue<float>(0);

			// set the default transparent color
			phong.Transparent = new Fx.ColladaCommonColorOrTextureType();
			phong.Transparent.Color = new Core.ColladaColor(1, 1, 1, 1);

			// set the default transparency
			phong.Transparency = new Fx.ColladaCommonFloatOrParamType();
			phong.Transparency.Float = new ColladaSIDValue<float>(1.0f);

			// set the default diffuse color
			phong.Diffuse = new Fx.ColladaCommonColorOrTextureType();
			phong.Diffuse.Color = new Core.ColladaColor(0.5f, 0.5f, 0.5f, 1);

			// set the default specular color
			phong.Specular = new Fx.ColladaCommonColorOrTextureType();
			phong.Specular.Color = new Core.ColladaColor(1, 1, 1, 1);

			// set the default shininess
			phong.Shininess = new Fx.ColladaCommonFloatOrParamType();
			phong.Shininess.Float = new ColladaSIDValue<float>(0.0f);

			return phong;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up a color or texture obejct. </summary>
		/// <exception cref="ColladaException">	Thrown when a Collada error condition occurs. </exception>
		/// <param name="definition">	The phong definition. </param>
		/// <param name="valueKey">  	The value key. </param>
		/// <param name="output">	 	The output object. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetColorOrTexture(Phong definition, string valueKey, Fx.ColladaCommonColorOrTextureType output)
		{
			if (definition.ContainsKey(valueKey) && (definition[valueKey] != null))
			{
				object value = definition[valueKey];
				if (value is TagInterface.RealColor)
				{
					output.Texture = null;
					output.Color = new Core.ColladaColor(value as TagInterface.RealColor, true);
				}
				else if (value is Phong.Texture)
				{
					var texture = (Phong.Texture)value;
					output.Color = null;
					output.Texture = CreateTexture(texture.Path, texture.UVChannel);
					output.Opaque = texture.Opacity;
				}
				else
				{
					throw new ColladaException("Invalid object type for " + valueKey + " value");
				}
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up a float or parameter object. </summary>
		/// <exception cref="ColladaException">	Thrown when a Collada error condition occurs. </exception>
		/// <param name="definition">	The phong definition. </param>
		/// <param name="valueKey">  	The value key. </param>
		/// <param name="output">	 	The output object. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetFloatOrParam(Phong definition, string valueKey, Fx.ColladaCommonFloatOrParamType output)
		{
			if (definition.ContainsKey(valueKey) && (definition[valueKey] != null))
			{
				object value = definition[valueKey];
				if (value is float)
				{
					output.Param = null;
					output.Float = new ColladaSIDValue<float>((float)value);
				}
				else if (value is string)
				{
					output.Float = null;
					output.Param = new Fx.ColladaParam();
					output.Param.Ref = (string)value;
				}
				else
				{
					throw new ColladaException("Invalid object type for " + valueKey + " value");
				}
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a phong object from a definition. </summary>
		/// <param name="definition">	The phong definition. </param>
		/// <returns>	The new phong object. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Fx.ColladaPhong CreatePhong(Phong definition)
		{
			var phong = CreateDefaultPhong();

			SetColorOrTexture(definition, "Ambient", phong.Ambient);
			SetColorOrTexture(definition, "Emission", phong.Emission);
			SetColorOrTexture(definition, "Reflective", phong.Reflective);
			SetFloatOrParam(definition, "Reflectivity", phong.Reflectivity);
			SetColorOrTexture(definition, "Transparent", phong.Transparent);
			SetFloatOrParam(definition, "Transparency", phong.Transparency);
			SetColorOrTexture(definition, "Diffuse", phong.Diffuse);
			SetColorOrTexture(definition, "Specular", phong.Specular);
			SetFloatOrParam(definition, "Shininess", phong.Shininess);

			return phong;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates an effect element from a shader tag. </summary>
		/// <param name="effect_id">	DatumIndex of the shader to create an effect from. </param>
		/// <param name="phong">		The phong. </param>
		/// <param name="bitmaps">  	The bitmaps. </param>
		/// <returns>	The new effect. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Fx.ColladaEffect CreateEffect(string effect_id,
			Phong phongDefinition,
			ImageList images)
		{
			var effect = new Fx.ColladaEffect();

			effect.ID = effect_id;
			effect.Name = effect_id;
			effect.ProfileCOMMON = new List<COLLADA.Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new COLLADA.Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new COLLADA.Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = CreatePhong(phongDefinition);
			effect.ProfileCOMMON[0].Newparam = new List<COLLADA.Fx.ColladaNewparam>();

			foreach (var image in images)
			{
				string imageName = ColladaUtilities.FormatName(image.ImageName, " ", "_");

				var newparam_surface = new COLLADA.Fx.ColladaNewparam();
				var newparam_sampler = new COLLADA.Fx.ColladaNewparam();

				newparam_surface.sID = String.Concat(imageName, "-surface");
				newparam_sampler.sID = String.Concat(imageName, "-surface-sampler");

				var surface = new COLLADA.Fx.ColladaSurface();
				var sampler2d = new COLLADA.Fx.ColladaSampler2D();

				surface.Type = COLLADA.Enums.ColladaFXSurfaceTypeEnum._2D;
				surface.InitFrom = new COLLADA.Fx.ColladaInitFrom();
				surface.InitFrom.Text = ColladaElement.FormatID<Fx.ColladaImage>(imageName);

				sampler2d.Source = newparam_surface.sID;

				newparam_surface.Surface = surface;
				newparam_sampler.Sampler2D = sampler2d;

				effect.ProfileCOMMON[0].Newparam.Add(newparam_surface);
				effect.ProfileCOMMON[0].Newparam.Add(newparam_sampler);
			}

			return effect;
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates an effect element with a phong definition set up with default values.
		/// </summary>
		/// <param name="effect_name">	ID of the effect. </param>
		/// <returns>	The new default effect. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Fx.ColladaEffect CreateDefaultEffect(string effect_name)
		{
			var effect = new Fx.ColladaEffect();

			// create a common profile element
			effect.ID = effect_name;
			effect.ProfileCOMMON = new List<Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = CreateDefaultPhong();

			return effect;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Populate the effect list with the shaders used in the model. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void CreateEffectList()
		{
			var provider = GetDataProvider<IColladaShaderDataProvider>();
			var effects = provider.GetEffects();

			foreach (var effect in effects)
			{
				// create the effect and add it to the list
				listEffect.Add(
					CreateEffect(effect.Name,
						effect.PhongDefinition,
						effect.Images)
				);
			}
		}
		#endregion
		#region Create Materials
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic material information class. </summary>
		///-------------------------------------------------------------------------------------------------
		public class Material
		{
			public string Name { get; private set; }
			public string EffectID { get; private set; }

			public Material(string name, string effectID)
			{
				Name = name;
				EffectID = effectID;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	List of materials. </summary>
		///-------------------------------------------------------------------------------------------------
		public class MaterialList : List<Material> { };

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a material element. </summary>
		/// <param name="id">	 	ID for the material. </param>
		/// <param name="name">  	Name for the material. </param>
		/// <param name="effect">	The effect the material references. </param>
		/// <returns>	The new material. </returns>
		///-------------------------------------------------------------------------------------------------
		protected static Fx.ColladaMaterial CreateMaterial(string id, string name, string effect)
		{
			var material = new Fx.ColladaMaterial();

			material.ID = id;
			material.Name = name;
			material.InstanceEffect = new Fx.ColladaInstanceEffect();
			material.InstanceEffect.sID = effect;
			material.InstanceEffect.URL = COLLADA.ColladaUtilities.BuildUri(ColladaElement.FormatID<Fx.ColladaEffect>(effect));

			return material;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a material element. </summary>
		/// <param name="id">	Name to use for everything (id, name, sID, and effect) </param>
		/// <returns>	CreateMaterial(id, id, id) </returns>
		///-------------------------------------------------------------------------------------------------
		protected static Fx.ColladaMaterial CreateMaterial(string id)
		{
			return CreateMaterial(id, id, id);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Populate the material list. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void CreateMaterialList()
		{
			var provider = GetDataProvider<IColladaShaderDataProvider>();
			var materials = provider.GetMaterials();

			// for each shader, create a new material
			foreach(var material in materials)
			{
				string effectName = ColladaUtilities.FormatName(material.Name, " ", "_");

				listMaterial.Add(
					CreateMaterial(material.Name,
						material.Name,
						effectName));
			}
		}
		#endregion
		#region Create Bones
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic bone information class. </summary>
		///-------------------------------------------------------------------------------------------------
		protected class Bone
		{
			public string Name { get; private set; }
			public LowLevel.Math.real_point3d Translation { get; private set; }
			public LowLevel.Math.real_quaternion Rotation { get; private set; }
			public float Scale { get; private set; }

			public int Parent { get; private set; }
			public int Child { get; private set; }
			public int Sibling { get; private set; }

			public Bone(string name,
				LowLevel.Math.real_point3d translation, LowLevel.Math.real_quaternion rotation, float scale,
				int parent, int child, int sibling)
			{
				Name = name;

				Translation = translation;
				Rotation = rotation;
				Scale = scale;

				Parent = parent;
				Child = child;
				Sibling = sibling;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates a bone tree list that contains the matrices necessary for skinning.
		/// </summary>
		/// <param name="bones">	The full bone list to create matrices for. </param>
		///-------------------------------------------------------------------------------------------------
		private void CreateBoneMatrixList(List<Bone> bones)
		{
			// add nodes for all of the bones in order
			foreach (var bone in bones)
			{
				listBoneMatrix.Add(new ColladaBoneMatrix(bone.Name,
					bone.Translation,
					bone.Rotation,
					bone.Scale));
			}

			// now that all the nodes have been created, set the parent node values, and calculate the matrices
			for (int i = 0; i < bones.Count; i++)
			{
				if (bones[i].Parent != -1)
				{
					listBoneMatrix[i].ParentNode = listBoneMatrix[bones[i].Parent];
				}

				listBoneMatrix[i].CreateMatrices();
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a list of nodes containing the child bones of a single bone. </summary>
		/// <param name="bone">			The bone to get the children for. </param>
		/// <param name="bone_list">	The full bone list. </param>
		/// <returns>	The child bones. </returns>
		///-------------------------------------------------------------------------------------------------
		private List<Core.ColladaNode> GetChildBones(Bone bone, List<Bone> bone_list)
		{
			// bone has no children, return
			if (bone.Child == -1)
				return null;

			var children = new List<Core.ColladaNode>();

			// cycle through the first child and its siblings
			int bone_index = bone.Child;
			while (bone_index != -1)
			{
				children.Add(listBone[bone_index]);

				bone_index = bone_list[bone_index].Sibling;
			}
			return children;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a collada node for a single bone. </summary>
		///
		/// <param name="bone">				The bone to create a node for. </param>
		/// <param name="xColumnVector">	X column vector for correct euler rotation. </param>
		/// <param name="yColumnVector">	Y column vector for correct euler rotation. </param>
		/// <param name="zColumnVector">	Z column vector for correct euler rotation. </param>
		/// <param name="order">			The rotation order. </param>
		///
		/// <returns>	The new bone. </returns>
		private Core.ColladaNode CreateBone(Bone bone,
			LowLevel.Math.real_vector3d xColumnVector,
			LowLevel.Math.real_vector3d yColumnVector,
			LowLevel.Math.real_vector3d zColumnVector,
			ColladaUtilities.ColladaRotationOrder order)
		{
			// create a node
			var node = CreateNode(bone.Name, bone.Name, bone.Name, Enums.ColladaNodeType.JOINT);

			// set the nodes position
			var translation = new Core.ColladaTranslate(bone.Translation);
			translation.sID = "translation";
			node.Add(translation);

			// set the nodes rotation
			var rotation = TagInterface.RealQuaternion.ToEuler3D(bone.Rotation);
			node.AddRange(ColladaUtilities.CreateRotationSet(rotation.Roll, rotation.Pitch, rotation.Yaw
				, xColumnVector
				, yColumnVector
				, zColumnVector
				, order));

			return node;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates the bone node and bone matrix lists from a generic list of bones. </summary>
		///
		/// <param name="bones">			A generic list of connected bones. </param>
		/// <param name="xColumnVector">	X column vector for correct euler rotation. </param>
		/// <param name="yColumnVector">	Y column vector for correct euler rotation. </param>
		/// <param name="zColumnVector">	Z column vector for correct euler rotation. </param>
		/// <param name="order">			The rotation order. </param>
		protected void CreateBones(List<Bone> bones,
			LowLevel.Math.real_vector3d xColumnVector,
			LowLevel.Math.real_vector3d yColumnVector,
			LowLevel.Math.real_vector3d zColumnVector,
			ColladaUtilities.ColladaRotationOrder order)
		{
			// create the bone nodes
			foreach (var bone in bones)
				listBone.Add(CreateBone(bone, xColumnVector, yColumnVector, zColumnVector, order));

			// link the bones together
			for (int i = 0; i < bones.Count; i++)
				listBone[i].AddRange(GetChildBones(bones[i], bones));

			// create the bone matrix list
			CreateBoneMatrixList(bones);
		}
		#endregion
		#region Create Instance
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generic material reference infor class. </summary>
		///-------------------------------------------------------------------------------------------------
		public class MaterialReference
		{
			public string Name { get; private set; }
			public string URL { get; private set; }
			public string Symbol { get; private set; }

			public MaterialReference(string name, string url, string symbol)
			{
				Name = name;
				URL = url;
				Symbol = symbol;
			}
		}

		public class MaterialReferenceList : List<MaterialReference> { }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a collada bind material. </summary>
		/// <param name="materialReferences">	The material references. </param>
		/// <returns>	The new collada bind material. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Fx.ColladaBindMaterial CreateBindMaterial(MaterialReferenceList materialReferences)
		{
			var bindMaterial = new Fx.ColladaBindMaterial();
			bindMaterial.TechniqueCommon = new Core.ColladaTechniqueCommon();
			bindMaterial.TechniqueCommon.InstanceMaterial = new List<Fx.ColladaInstanceMaterial>();

			foreach(var reference in materialReferences)
			{
				// create a new material instance referencing the required material
				var instanceMaterial = new Fx.ColladaInstanceMaterial();
				instanceMaterial.Name = reference.Name;
				instanceMaterial.Symbol = reference.Symbol;
				instanceMaterial.Target = reference.URL;

				// add the material instance to the list
				bindMaterial.TechniqueCommon.InstanceMaterial.Add(instanceMaterial);
			}

			return bindMaterial;
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a node instancing a controller. </summary>
		/// <param name="url">				 	URL of the controller to instance. </param>
		/// <param name="name">				 	The name for the node. </param>
		/// <param name="materialReferences">	The material references. </param>
		/// <returns>	The new collada instance controller. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Core.ColladaInstanceController CreateInstanceController(string url, string name, MaterialReferenceList materialReferences)
		{
			// create a new controller instance and set its attributes
			var instanceController = new Core.ColladaInstanceController();
			instanceController.URL = url;
			instanceController.Skeleton = new List<Core.ColladaSkeleton>();
			instanceController.Skeleton.Add(new Core.ColladaSkeleton());
			instanceController.Skeleton[0].Value = ColladaUtilities.BuildUri(listBone[0].ID);

			// add the bind material element if present
			if ((materialReferences != null) && (materialReferences.Count != 0))
			{
				instanceController.BindMaterial = new List<Fx.ColladaBindMaterial>();
				instanceController.BindMaterial.Add(CreateBindMaterial(materialReferences));
			}

			return instanceController;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a node instancing a geometry. </summary>
		/// <param name="url">				 	URL of the geometry to instance. </param>
		/// <param name="name">				 	The name for the node. </param>
		/// <param name="materialReferences">	The material references. </param>
		/// <returns>	The new collada instance geometry. </returns>
		///-------------------------------------------------------------------------------------------------
		protected Core.ColladaInstanceGeometry CreateInstanceGeometry(string url, string name, MaterialReferenceList materialReferences)
		{
			// create a new geometry instance and set its attributes
			var instanceGeometry = new Core.ColladaInstanceGeometry();
			instanceGeometry.URL = url;
		
			// add the bind material element if present
			if ((materialReferences != null) && (materialReferences.Count != 0))
			{
				instanceGeometry.BindMaterial = CreateBindMaterial(materialReferences);
			}

			return instanceGeometry;
		}
		#endregion
		#endregion

		#region COLLADA Creation
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds an asset element to the current collada file. </summary>
		/// <param name="author">		   	String containing the authors name. </param>
		/// <param name="authoring_tool">
		/// 	String containing the name of the program that created the file.
		/// </param>
		/// <param name="unit_name">	   	The name of the units the scene is scaled against. </param>
		/// <param name="unit_meter_scale">	The ratio of the units in relation to metres. </param>
		/// <param name="up_axis">		   	The axis that is considered to be "up". </param>
		///-------------------------------------------------------------------------------------------------
		protected void AddAsset(string author,
			string authoring_tool,
			string unit_name,
			double unit_meter_scale,
			Enums.ColladaUpAxisEnum up_axis)
		{
			COLLADAFile.Asset = new COLLADA.Core.ColladaAsset();
			COLLADAFile.Asset.Contributor = new List<COLLADA.Core.ColladaContributor>();
			COLLADAFile.Asset.Contributor.Add(new COLLADA.Core.ColladaContributor());
			COLLADAFile.Asset.Contributor[0].Author = author;
			COLLADAFile.Asset.Contributor[0].AuthoringTool = authoring_tool;

			COLLADAFile.Asset.Created = DateTime.Now;
			COLLADAFile.Asset.Modified = DateTime.Now;
			COLLADAFile.Asset.Unit = new Core.ColladaAssetUnit();
			COLLADAFile.Asset.Unit.Meter = unit_meter_scale;
			COLLADAFile.Asset.Unit.Name = unit_name;
			COLLADAFile.Asset.UpAxis = up_axis;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Adds a scene to the collada file that instances a visual scene with the specified ID.
		/// </summary>
		/// <param name="scene_id">	Identifier for the scene. </param>
		///-------------------------------------------------------------------------------------------------
		protected void AddScene(string scene_id)
		{
			COLLADAFile.Scene = new Core.ColladaScene();
			COLLADAFile.Scene.InstanceVisualScene = new Core.ColladaInstanceVisualScene();
			COLLADAFile.Scene.InstanceVisualScene.URL = ColladaUtilities.BuildUri(ColladaElement.FormatID<Core.ColladaVisualScene>(scene_id));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the library_images element in the collada file if applicable. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void AddLibraryImages()
		{
			// if the image list is null or empty, do not create the library element
			if ((listImage == null) || (listImage.Count == 0))
			{
				return;
			}
			COLLADAFile.LibraryImages = new Fx.ColladaLibraryImages();
			COLLADAFile.LibraryImages.Image = new List<Fx.ColladaImage>();
			COLLADAFile.LibraryImages.Image.AddRange(listImage);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the library_effects element in the collada file if applicable. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void AddLibraryEffects()
		{
			// if the effect list is null or empty, do not create the library element
			if ((listEffect == null) || (listEffect.Count == 0))
			{
				return;
			}

			COLLADAFile.LibraryEffects = new Fx.ColladaLibraryEffects();
			COLLADAFile.LibraryEffects.Effect = new List<Fx.ColladaEffect>();
			COLLADAFile.LibraryEffects.Effect.AddRange(listEffect);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the library_materials element in the collada file if applicable. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void AddLibraryMaterials()
		{
			// if the material list is null or empty, do not create the library element
			if ((listMaterial == null) || (listMaterial.Count == 0))
			{
				return;
			}

			COLLADAFile.LibraryMaterials = new Fx.ColladaLibraryMaterials();
			COLLADAFile.LibraryMaterials.Material = new List<Fx.ColladaMaterial>();
			COLLADAFile.LibraryMaterials.Material.AddRange(listMaterial);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the library_geometries element in the collada file. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void AddLibraryGeometries()
		{
			if ((listGeometry == null) || (listGeometry.Count == 0))
			{
				return;
			}

			COLLADAFile.LibraryGeometries = new Core.ColladaLibraryGeometries();
			COLLADAFile.LibraryGeometries.Geometry = new List<Core.ColladaGeometry>();
			COLLADAFile.LibraryGeometries.Geometry.AddRange(listGeometry);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the library_controllers element in the collada file. </summary>
		///-------------------------------------------------------------------------------------------------
		protected void AddLibraryControllers()
		{
			if ((listController == null) || (listController.Count == 0))
			{
				return;
			}

			COLLADAFile.LibraryControllers = new Core.ColladaLibraryControllers();
			COLLADAFile.LibraryControllers.Controller = new List<Core.ColladaController>();
			COLLADAFile.LibraryControllers.Controller.AddRange(listController);
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Performs the actual collada file population. Override this in derived classes to define
		/// 	how this is done.
		/// </summary>
		/// <returns>	True if no errors occurred. </returns>
		///-------------------------------------------------------------------------------------------------
		protected virtual bool BuildColladaInstanceImpl() { return false; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Populates the collada file object with the current data set. </summary>
		/// <returns>	True if no errors occurred. </returns>
		///-------------------------------------------------------------------------------------------------
		public bool BuildColladaInstance()
		{
			bool success = false;
			try
			{
				// Set the base data providers
				mShaderDataProvider = GetDataProvider<IColladaShaderDataProvider>();

				// build the collada file definition and validate it
				if (BuildColladaInstanceImpl())
				{
					var validator = new ColladaFileValidator();

					validator.ErrorOccured += new EventHandler<ColladaErrorEventArgs>(ValidatorErrorOccured);
					success = validator.ValidateFile(COLLADAFile);
					validator.ErrorOccured -= new EventHandler<ColladaErrorEventArgs>(ValidatorErrorOccured);
				}
			}
			catch (ColladaException e)
			{
				// if an exception occurred, report it and return gracefully
				mMessageHandler.SendMessage(e.Message);
				mMessageHandler.SendMessage(e.StackTrace);

				for (var except = e.InnerException; except != null; except = except.InnerException)
				{
					mMessageHandler.SendMessage(except.Message);
				}
			}
			return success;
		}
		#endregion

		#region COLLADA Output
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Saves the Collada file in its current state to file. </summary>
		/// <param name="location">	Location to save the Collada file to. </param>
		///-------------------------------------------------------------------------------------------------
		public void SaveDAE(string location)
		{
			// if the file exists but overwriting is disabled, report this and return
			if (System.IO.File.Exists(location) && !colladaSettings.Overwrite)
			{
				mMessageHandler.SendMessage(String.Format(ColladaExceptionStrings.FileExists, location));
				return;
			}

			if (System.IO.File.Exists(location))
			{
				// if the file is readonly always skip it
				var attributes = System.IO.File.GetAttributes(location);
				if ((attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
				{
					return;
				}
			}

			// serialize the COLLADA file to the xml file
			Directory.CreateDirectory(System.IO.Path.GetDirectoryName(location));

			var serializer = new XmlSerializer(typeof(ColladaFile));
			using (var writer = new XmlTextWriter(location, null))
			{
				writer.Formatting = Formatting.Indented;
				serializer.Serialize(writer, COLLADAFile);
			}
		}
		#endregion COLLADA Output

		#region Helpers
		#region Geometry
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a polygon index list for a set of planar, convex vertices. </summary>
		/// <param name="vertex_count">	Number of vertices in the plane. </param>
		/// <returns>	. </returns>
		///-------------------------------------------------------------------------------------------------
		protected List<int> BuildFaceIndices(int vertex_count)
		{
			// create an array of vertex indices
			var index_array = new List<ushort>();
			for (ushort j = 0; j < vertex_count; j++) { index_array.Add(j); }

			// in a triangle list without degenerates face count = vertex count - 2
			int vertex_index = 0;
			int surface_count = vertex_count - 2;

			// the triangle data is calculated by assuming the surface is coplanar and convex,
			// which means we can make the triangles by using 3 consecutive indices, removing the
			// middle index and repeating until only 3 indices are left, which makes the final triangle

			var indices = new List<int>();
			for (int i = 0; i < surface_count; i++)
			{
				// create a triangle
				indices.Add(index_array[vertex_index + 2]);
				indices.Add(index_array[vertex_index + 1]);
				indices.Add(index_array[vertex_index + 0]);

				index_array.RemoveAt(vertex_index + 1);
				vertex_index++;

				if (vertex_index + 2 >= index_array.Count)
					vertex_index = 0;
			}
			return indices;
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Takes an triangle strip index list and converts it into a triangle list. </summary>
		/// <param name="tri_strip">	The triangle strip to convert. </param>
		/// <returns>	The triangle converted strip to list. </returns>
		///-------------------------------------------------------------------------------------------------
		protected List<int> ConvertTriStripToList(List<int> tri_strip, bool flip = false)
		{
			var tri_list = new List<int>();
			// add indices to the list, assuming that after the first 3 strip indices, each index is another triangle
			// using itself and the previous two indices to make up the face
			for (int i = 0; i < tri_strip.Count - 2; i++)
			{
				int index0;
				int index1;
				int index2;

				index0 = tri_strip[i];
				index1 = tri_strip[i + 1];
				index2 = tri_strip[i + 2];

				// triangle strips contain degenerate triangles, we don't want these
				if (index2 == index1 || index1 == index0 || index0 == index2)
					continue;

				// each new triangle flips it's ordering, so it has to be swapped on each index so that it faces
				// the right way
				bool swap = (i % 2 != 0);

				if (swap)
				{
					int temp = index2;
					index2 = index0;
					index0 = temp;
				}

				// add the final indices
				if (flip)
				{
					tri_list.Add(index2);
					tri_list.Add(index1);
					tri_list.Add(index0);
				}
				else
				{
					tri_list.Add(index0);
					tri_list.Add(index1);
					tri_list.Add(index2);
				}
			}
			return tri_list;
		}
		#endregion Geometry
		#endregion
	};
}