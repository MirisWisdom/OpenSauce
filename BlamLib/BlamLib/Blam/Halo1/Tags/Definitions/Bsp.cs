/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	[TI.TagGroup((int)TagGroups.Enumerated.sbsp, 5, 648)]
	public sealed partial class structure_bsp_group : TI.Definition
	{
		#region structure_collision_materials_block
		[TI.Definition(-1, 20)]
		public sealed partial class structure_collision_materials_block : TI.Definition
		{
			public TI.TagReference Shader;
		};
		#endregion

		#region collision_bsp_block
		[TI.Definition(-1, 96)]
		public sealed partial class collision_bsp_block : TI.Definition
		{
			#region bsp3d_nodes_block
			[TI.Definition(-1, 12)]
			public sealed partial class bsp3d_nodes_block : TI.Definition
			{
				public TI.LongInteger Plane;
				public TI.LongInteger BackChild, FrontChild;
			};
			#endregion

			// planes_block, field_block<TI.RealPlane3D>

			#region leaves_block
			[TI.Definition(-1, 8)]
			public sealed partial class leaves_block : TI.Definition
			{
				public TI.Flags Flags;
				public TI.ShortInteger Bsp2dReferenceCount;
				public TI.LongInteger FirstBsp2dReference;
			};
			#endregion

			#region bsp2d_references_block
			[TI.Definition(-1, 8)]
			public sealed partial class bsp2d_references_block : TI.Definition
			{
				public TI.LongInteger Plane, Bsp2dNode;
			};
			#endregion

			#region bsp2d_nodes_block
			[TI.Definition(-1, 20)]
			public sealed partial class bsp2d_nodes_block : TI.Definition
			{
				public TI.RealPlane2D Plane;
				public TI.LongInteger BackChild, FrontChild;
			};
			#endregion

			#region surfaces_block
			[TI.Definition(-1, 12)]
			public sealed partial class surfaces_block : TI.Definition
			{
				public TI.LongInteger Plane, FirstEdge;
				public TI.Flags Flags;
				public TI.ByteInteger BreakableSurface;
				public TI.ShortInteger Material;
			};
			#endregion

			#region edges_block
			[TI.Definition(-1, 24)]
			public sealed partial class edges_block : TI.Definition
			{
				public TI.LongInteger StartVertex, EndVertex;
				public TI.LongInteger ForwardEdge, ReverseEdge;
				public TI.LongInteger LeftSurface, RightSurface;
			};
			#endregion

			#region vertices_block
			[TI.Definition(-1, 16)]
			public sealed partial class vertices_block : TI.Definition
			{
				public TI.RealPoint3D Point;
				public TI.LongInteger FirstEdge;
			};
			#endregion

			public TI.Block<bsp3d_nodes_block> Bsp3dNodes;
			public TI.Block<field_block<TI.RealPlane3D>> Planes;
			public TI.Block<leaves_block> Leaves;
			public TI.Block<bsp2d_references_block> Bsp2dReferences;
			public TI.Block<bsp2d_nodes_block> Bsp2dNodes;
			public TI.Block<surfaces_block> Surfaces;
			public TI.Block<edges_block> Edges;
			public TI.Block<vertices_block> Vertices;
		};
		#endregion

		#region structure_bsp_node_block
		[TI.Definition(-1, 6)]
		public sealed partial class structure_bsp_node_block : TI.Definition
		{
		};
		#endregion

		#region structure_bsp_leaf_block
		[TI.Definition(-1, 16)]
		public sealed partial class structure_bsp_leaf_block : TI.Definition
		{
			public TI.ShortInteger Cluster, SurfaceReferenceCount;
			/// <remarks>
			/// To <see cref="structure_bsp_surface_reference_block"/>
			/// </remarks>
			public TI.BlockIndex SurfaceReferences;
		};
		#endregion

		#region structure_bsp_surface_reference_block
		[TI.Definition(-1, 8)]
		public sealed partial class structure_bsp_surface_reference_block : TI.Definition
		{
			/// <remarks>
			/// To <see cref="structure_bsp_surface_block"/>
			/// </remarks>
			public TI.BlockIndex Surface;
			/// <remarks>
			/// To <see cref="structure_bsp_node_block"/>
			/// </remarks>
			public TI.BlockIndex Node;
		};
		#endregion

		#region structure_bsp_surface_block
		[TI.Definition(-1, 6)]
		public sealed partial class structure_bsp_surface_block : TI.Definition
		{
			public TI.ShortInteger A1, A2, A3;
		};
		#endregion

		#region structure_bsp_lightmap_block
		[TI.Definition(-1, 32)]
		public sealed partial class structure_bsp_lightmap_block : TI.Definition
		{
			#region structure_bsp_material_block
			[TI.Definition(-1, 256)]
			public sealed partial class structure_bsp_material_block : TI.Definition
			{
				public TI.TagReference Shader;
				public TI.ShortInteger ShaderPermutation;
				public TI.Flags Flags;
				/// <remarks>
				/// To <see cref="structure_bsp_surface_block"/>
				/// </remarks>
				public TI.BlockIndex Surfaces;
				public TI.LongInteger SurfaceCount;
				public TI.RealPoint3D Centroid;
				public TI.RealColor AmbientColor;
				public TI.ShortInteger DistantLightCount;

				public TI.RealColor DistantLightColor1;
				public TI.RealVector3D DistantLightDirection1;
				public TI.RealColor DistantLightColor2;
				public TI.RealVector3D DistantLightDirection2;

				public TI.RealColor ReflectionTint;

				public TI.RealVector3D ShadowVector;
				public TI.RealColor ShadowColor;

				public TI.RealPlane3D Plane;
				public TI.ShortInteger BreakableSurface;

				public TI.LongInteger VerticesCount;
				public TI.LongInteger VerticesStartIndex;
				public TI.LongInteger LightmapVerticesCount;
				public TI.LongInteger LightmapVerticesStartIndex;
				public TI.Data UncompressedVertices, CompressedVertices;
			};
			#endregion

			public TI.ShortInteger Bitmap;
			public TI.Block<structure_bsp_material_block> Materials;
		};
		#endregion

		#region structure_bsp_lens_flare_block
		[TI.Definition(-1, 16)]
		public sealed partial class structure_bsp_lens_flare_block : TI.Definition
		{
			public TI.TagReference LensFlare;
		};
		#endregion

		#region structure_bsp_lens_flare_marker_block
		[TI.Definition(-1, 16)]
		public sealed partial class structure_bsp_lens_flare_marker_block : TI.Definition
		{
			public TI.RealPoint3D Position;
			public TI.ByteInteger DirectionIComponent, DirectionJComponent, DirectionKComponent;
			public TI.ByteInteger LensFlareIndex;
		};
		#endregion

		#region structure_bsp_cluster_block
		[TI.Definition(-1, 104)]
		public sealed partial class structure_bsp_cluster_block : TI.Definition
		{
			#region structure_bsp_subcluster_block
			[TI.Definition(-1, 36)]
			public sealed partial class structure_bsp_subcluster_block : TI.Definition
			{
				// structure_bsp_subcluster_surface_index_block, field_block<TI.LongInteger>

				public TI.RealBounds WorldBoundsX, WorldBoundsY, WorldBoundsZ;
				public TI.Block<field_block<TI.LongInteger>> SurfaceIndices;
			};
			#endregion

			// structure_bsp_cluster_surface_index_block, field_block<TI.LongInteger>

			#region structure_bsp_mirror_block
			[TI.Definition(-1, 64)]
			public sealed partial class structure_bsp_mirror_block : TI.Definition
			{
				// structure_bsp_mirror_vertex_block, field_block<TI.RealPoint3D>

				public TI.RealPlane3D Plane;
				public TI.TagReference Shader;
				public TI.Block<field_block<TI.RealPoint3D>> Vertices;
			};
			#endregion

			// structure_bsp_cluster_portal_index_block, field_block<TI.ShortInteger>

			public TI.ShortInteger Sky, Fog;
			/// <remarks>
			/// To <see cref="structure_bsp_background_sound_palette_block"/>
			/// </remarks>
			public TI.BlockIndex BackgroundSound;
			/// <remarks>
			/// To <see cref="structure_bsp_sound_environment_palette_block"/>
			/// </remarks>
			public TI.BlockIndex SoundEnvironment;
			/// <remarks>
			/// To <see cref="structure_bsp_weather_palette_block"/>
			/// </remarks>
			public TI.BlockIndex Weather;
			public TI.ShortInteger TransitionStructureBsp;
			public TI.Block<predicted_resource_block> PredictedResources;
			public TI.Block<structure_bsp_subcluster_block> Subclusters;
			public TI.ShortInteger FirstLensFlareMarkerIndex, LensFlareMarkerCount;
			public TI.Block<field_block<TI.LongInteger>> SurfaceIndices;
			public TI.Block<structure_bsp_mirror_block> Mirrors;
			public TI.Block<field_block<TI.ShortInteger>> Portals;
		};
		#endregion

		#region structure_bsp_cluster_portal_block
		[TI.Definition(-1, 64)]
		public sealed partial class structure_bsp_cluster_portal_block : TI.Definition
		{
			// structure_bsp_cluster_portal_vertex_block, field_block<TI.RealPoint3D>

			public TI.ShortInteger FrontCluster, BackCluster;
			public TI.LongInteger PlaneIndex;
			public TI.RealPoint3D Centroid;
			public TI.Real BoundingRadius;
			public TI.Flags Flags;
			public TI.Block<field_block<TI.RealPoint3D>> Vertices;
		};
		#endregion

		#region structure_bsp_breakable_surface_block
		[TI.Definition(-1, 48)]
		public sealed partial class structure_bsp_breakable_surface_block : TI.Definition
		{
			public TI.RealPoint3D Centroid;
			public TI.Real Radius;
			public TI.LongInteger CollisionSurfaceIndex;
		};
		#endregion

		#region structure_bsp_fog_plane_block
		[TI.Definition(-1, 32)]
		public sealed partial class structure_bsp_fog_plane_block : TI.Definition
		{
			// structure_bsp_fog_plane_vertex_block, field_block<TI.RealPoint3D>

			public TI.BlockIndex FrontRegion;
			public TI.RealPlane3D Plane;
			public TI.Block<field_block<TI.RealPoint3D>> Vertices;
		};
		#endregion

		#region structure_bsp_fog_region_block
		[TI.Definition(-1, 40)]
		public sealed partial class structure_bsp_fog_region_block : TI.Definition
		{
			/// <remarks>
			/// To <see cref="structure_bsp_fog_palette_block"/>
			/// </remarks>
			public TI.BlockIndex FogPalette;
			/// <remarks>
			/// To <see cref="structure_bsp_weather_palette_block"/>
			/// </remarks>
			public TI.BlockIndex WeatherPalette;
		};
		#endregion

		#region structure_bsp_fog_palette_block
		[TI.Definition(-1, 136)]
		public sealed partial class structure_bsp_fog_palette_block : TI.Definition
		{
			public TI.String Name;
			public TI.TagReference Fog;
			public TI.String FogScaleFunction;
		};
		#endregion

		#region structure_bsp_weather_palette_block
		[TI.Definition(-1, 240)]
		public sealed partial class structure_bsp_weather_palette_block : TI.Definition
		{
			public TI.String Name;
			public TI.TagReference ParticleSystem;
			public TI.String ParticleSystemScaleFunction;
			public TI.TagReference Wind;
			public TI.RealVector3D WindDirection;
			public TI.Real WindMagnitude;
			public TI.String WindScaleFunction;
		};
		#endregion

		#region structure_bsp_weather_polyhedron_block
		[TI.Definition(-1, 32)]
		public sealed partial class structure_bsp_weather_polyhedron_block : TI.Definition
		{
			// structure_bsp_weather_polyhedron_plane_block, field_block<TI.RealPlane3D>

			public TI.RealPoint3D BoundingSphereCenter;
			public TI.Real BoundingSphereRadius;
			public TI.Block<field_block<TI.RealPlane3D>> Planes;
		};
		#endregion

		// structure_bsp_pathfinding_surfaces_block, field_block<TI.ByteInteger>

		// structure_bsp_pathfinding_edges_block, field_block<TI.ByteInteger>

		#region structure_bsp_background_sound_palette_block
		[TI.Definition(-1, 116)]
		public sealed partial class structure_bsp_background_sound_palette_block : TI.Definition
		{
			public TI.String Name;
			public TI.TagReference BackgroundSound;
			public TI.String ScaleFunction;
		};
		#endregion

		#region structure_bsp_sound_environment_palette_block
		[TI.Definition(-1, 80)]
		public sealed partial class structure_bsp_sound_environment_palette_block : TI.Definition
		{
			public TI.String Name;
			public TI.TagReference SoundEnvironment;
		};
		#endregion

		#region structure_bsp_marker_block
		[TI.Definition(-1, 60)]
		public sealed partial class structure_bsp_marker_block : TI.Definition
		{
			public TI.String Name;
			public TI.RealQuaternion Rotation;
			public TI.RealPoint3D Position;
		};
		#endregion

		#region structure_bsp_detail_object_data_block
		[TI.Definition(-1, 64)]
		public sealed partial class structure_bsp_detail_object_data_block : TI.Definition
		{
			#region global_detail_object_cells_block
			[TI.Definition(-1, 32)]
			public sealed partial class global_detail_object_cells_block : TI.Definition
			{
				public TI.ShortInteger Shorts1, Shorts2, Shorts3, Shorts4;
				public TI.LongInteger Longs1, Longs2, Longs3;
			};
			#endregion

			#region global_detail_object_block
			[TI.Definition(-1, 6)]
			public sealed partial class global_detail_object_block : TI.Definition
			{
				public TI.ByteInteger Chars1, Chars2, Chars3, Chars4;
				public TI.ShortInteger Short;
			};
			#endregion

			// global_detail_object_counts_block, field_block<TI.ShortInteger>

			#region global_z_reference_vector_block
			[TI.Definition(-1, 16)]
			public sealed partial class global_z_reference_vector_block : TI.Definition
			{
				public TI.Real Fields1, Fields2, Fields3, Fields4;
			};
			#endregion

			public TI.Block<global_detail_object_cells_block> Cells;
			public TI.Block<global_detail_object_block> Instances;
			public TI.Block<field_block<TI.ShortInteger>> Counts;
			public TI.Block<global_z_reference_vector_block> ZReferenceVectors;
		};
		#endregion

		#region structure_bsp_runtime_decal_block
		[TI.Definition(-1, 16)]
		public sealed partial class structure_bsp_runtime_decal_block : TI.Definition
		{
			// real x
			// real y
			// real z
			// short decals index
			// sbyte yaw
			// sbyte pitch
			public TI.Skip Skipper;
		};
		#endregion

		#region global_map_leaf_block
		[TI.Definition(-1, 24)]
		public sealed partial class global_map_leaf_block : TI.Definition
		{
			#region map_leaf_face_block
			[TI.Definition(-1, 16)]
			public sealed partial class map_leaf_face_block : TI.Definition
			{
				// map_leaf_face_vertex_block, field_block<TI.RealPoint2D>

				public TI.LongInteger NodeIndex;
				public TI.Block<field_block<TI.RealPoint2D>> Vertices;
			};
			#endregion

			// map_leaf_portal_index_block, field_block<TI.LongInteger>

			public TI.Block<map_leaf_face_block> Faces;
			public TI.Block<field_block<TI.LongInteger>> PortalIndices;
		};
		#endregion

		#region global_leaf_portal_block
		[TI.Definition(-1, 24)]
		public sealed partial class global_leaf_portal_block : TI.Definition
		{
			// leaf_portal_vertex_block, field_block<TI.RealPoint3D>

			public TI.LongInteger PlaneIndex, BackLeafIndex, FrontLeafIndex;
			public TI.Block<field_block<TI.RealPoint3D>> Vertices;
		};
		#endregion

		#region Fields
		public TI.TagReference LightmapBitmaps;
		public TI.Real VehicleFloor;
		public TI.Real VehicleCeiling;
		public TI.RealColor DefaultAmbientColor;
		public TI.RealColor DefaultDistantLight0Color;
		public TI.RealVector3D DefaultDistantLight0ColorDirection;
		public TI.RealColor DefaultDistantLight1Color;
		public TI.RealVector3D DefaultDistantLightColor1Direction;
		public TI.RealColor DefaultReflectionTint;
		public TI.RealColor DefaultShadowColor;
		public TI.RealVector3D DefaultShadowVector;

		public TI.Block<structure_collision_materials_block> CollisionMaterials;
		public TI.Block<collision_bsp_block> CollisionBsp;
		public TI.Block<structure_bsp_node_block> Nodes;

		public TI.RealBounds WorldBoundsX, WorldBoundsY, WorldBoundsZ;

		public TI.Block<structure_bsp_leaf_block> Leaves;
		public TI.Block<structure_bsp_surface_reference_block> LeafSurfaces;
		public TI.Block<structure_bsp_surface_block> Surfaces;
		public TI.Block<structure_bsp_lightmap_block> Lightmaps;
		public TI.Block<structure_bsp_lens_flare_block> LensFlares;
		public TI.Block<structure_bsp_lens_flare_marker_block> LensFlareMarkers;
		public TI.Block<structure_bsp_cluster_block> Clusters;
		public TI.Data ClusterData;
		public TI.Block<structure_bsp_cluster_portal_block> ClusterPortals;

		public TI.Block<structure_bsp_breakable_surface_block> BreakableSurfaces;
		public TI.Block<structure_bsp_fog_plane_block> FogPlanes;
		public TI.Block<structure_bsp_fog_region_block> FogRegions;
		public TI.Block<structure_bsp_fog_palette_block> FogPalette;
		public TI.Block<structure_bsp_weather_palette_block> WeatherPalette;
		public TI.Block<structure_bsp_weather_polyhedron_block> WeatherPolyhedra;

		public TI.Block<field_block<TI.ByteInteger>> PathfindingSurfaces, PathfindingEdges;

		public TI.Block<structure_bsp_background_sound_palette_block> BackgroundSoundPalette;
		public TI.Block<structure_bsp_sound_environment_palette_block> SoundEnvironmentPalette;
		public TI.Data SoundPASData;
		public TI.Block<structure_bsp_marker_block> Markers;
		public TI.Block<structure_bsp_detail_object_data_block> DetailObjects;
		public TI.Block<structure_bsp_runtime_decal_block> RuntimeDecals;
		public TI.Block<global_map_leaf_block> LeafMapLeaves;
		public TI.Block<global_leaf_portal_block> LeafMapPortals;
		#endregion
	};
}