/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	public partial class structure_bsp_group
	{
		#region structure_collision_materials_block
		public partial class structure_collision_materials_block
		{
			public structure_collision_materials_block() : base(2)
			{
				Add(Shader = new TI.TagReference(this, TagGroups.shdr));
				Add(new TI.Pad(4));
			}
		};
		#endregion

		#region collision_bsp_block
		public partial class collision_bsp_block
		{
			#region bsp3d_nodes_block
			public partial class bsp3d_nodes_block
			{
				public bsp3d_nodes_block() : base(3)
				{
					Add(Plane = new TI.LongInteger());
					Add(BackChild = new TI.LongInteger());
					Add(FrontChild = new TI.LongInteger());
				}
			};
			#endregion

			#region leaves_block
			public partial class leaves_block
			{
				public leaves_block() : base(3)
				{
					Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
					Add(Bsp2dReferenceCount = new TI.ShortInteger());
					Add(FirstBsp2dReference = new TI.LongInteger());
				}
			};
			#endregion

			#region bsp2d_references_block
			public partial class bsp2d_references_block
			{
				public bsp2d_references_block() : base(2)
				{
					Add(Plane = new TI.LongInteger());
					Add(Bsp2dNode = new TI.LongInteger());
				}
			};
			#endregion

			#region bsp2d_nodes_block
			public partial class bsp2d_nodes_block
			{
				public bsp2d_nodes_block() : base(3)
				{
					Add(Plane = new TI.RealPlane2D());
					Add(BackChild = new TI.LongInteger());
					Add(FrontChild = new TI.LongInteger());
				}
			};
			#endregion

			#region surfaces_block
			public partial class surfaces_block
			{
				public surfaces_block() : base(5)
				{
					Add(Plane = new TI.LongInteger());
					Add(FirstEdge = new TI.LongInteger());
					Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
					Add(BreakableSurface = new TI.ByteInteger());
					Add(Material = new TI.ShortInteger());
				}
			};
			#endregion

			#region edges_block
			public partial class edges_block
			{
				public edges_block() : base(6)
				{
					Add(StartVertex = new TI.LongInteger());
					Add(EndVertex = new TI.LongInteger());
					Add(ForwardEdge = new TI.LongInteger());
					Add(ReverseEdge = new TI.LongInteger());
					Add(LeftSurface = new TI.LongInteger());
					Add(RightSurface = new TI.LongInteger());
				}
			};
			#endregion

			#region vertices_block
			public partial class vertices_block
			{
				public vertices_block() : base(2)
				{
					Add(Point = new TI.RealPoint3D());
					Add(FirstEdge = new TI.LongInteger());
				}
			};
			#endregion

			public collision_bsp_block() : base(8)
			{
				Add(Bsp3dNodes = new TI.Block<bsp3d_nodes_block>(this, 131072));
				Add(Planes = new TI.Block<field_block<TI.RealPlane3D>>(this, 65536));
				Add(Leaves = new TI.Block<leaves_block>(this, 65536));
				Add(Bsp2dReferences = new TI.Block<bsp2d_references_block>(this, 131072));
				Add(Bsp2dNodes = new TI.Block<bsp2d_nodes_block>(this, 65535));
				Add(Surfaces = new TI.Block<surfaces_block>(this, 131072));
				Add(Edges = new TI.Block<edges_block>(this, 262144));
				Add(Vertices = new TI.Block<vertices_block>(this, 131072));
			}
		};
		#endregion

		#region structure_bsp_node_block
		public partial class structure_bsp_node_block
		{
			public structure_bsp_node_block() : base(1)
			{
				Add(new TI.Skip(6));
			}
		};
		#endregion

		#region structure_bsp_leaf_block
		public partial class structure_bsp_leaf_block
		{
			public structure_bsp_leaf_block() : base(5)
			{
				Add(new TI.Skip(6));
				Add(new TI.Pad(2));
				Add(Cluster = new TI.ShortInteger());
				Add(SurfaceReferenceCount = new TI.ShortInteger());
				Add(SurfaceReferences = new TI.BlockIndex(BlamLib.TagInterface.FieldType.LongBlockIndex));
			}
		};
		#endregion

		#region structure_bsp_surface_reference_block
		public partial class structure_bsp_surface_reference_block
		{
			public structure_bsp_surface_reference_block() : base(2)
			{
				Add(Surface = new TI.BlockIndex(BlamLib.TagInterface.FieldType.LongBlockIndex));
				Add(Node = new TI.BlockIndex(BlamLib.TagInterface.FieldType.LongBlockIndex));
			}
		};
		#endregion

		#region structure_bsp_surface_block
		public partial class structure_bsp_surface_block
		{
			public structure_bsp_surface_block() : base(3)
			{
				Add(A1 = new TI.ShortInteger());
				Add(A2 = new TI.ShortInteger());
				Add(A3 = new TI.ShortInteger());
			}
		};
		#endregion

		#region structure_bsp_lightmap_block
		public partial class structure_bsp_lightmap_block
		{
			#region structure_bsp_material_block
			public partial class structure_bsp_material_block
			{
				public structure_bsp_material_block() : base(28)
				{
					Add(Shader = new TI.TagReference(this, TagGroups.shdr));
					Add(ShaderPermutation = new TI.ShortInteger());
					Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
					Add(Surfaces = new TI.BlockIndex(TI.FieldType.LongBlockIndex));
					Add(SurfaceCount = new TI.LongInteger());
					Add(Centroid = new TI.RealPoint3D());
					Add(AmbientColor = new TI.RealColor());
					Add(DistantLightCount = new TI.ShortInteger());
					Add(new TI.Pad(2));

					Add(DistantLightColor1 = new TI.RealColor());
					Add(DistantLightDirection1 = new TI.RealVector3D());
					Add(DistantLightColor2 = new TI.RealColor());
					Add(DistantLightDirection2 = new TI.RealVector3D());
					Add(new TI.Pad(12));
					Add(ReflectionTint = new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(ShadowVector = new TI.RealVector3D());
					Add(ShadowColor = new TI.RealColor());
					Add(Plane = new TI.RealPlane3D());
					Add(BreakableSurface = new TI.ShortInteger());
					Add(new TI.Pad(2 + 4));
					Add(VerticesCount = new TI.LongInteger());
					Add(VerticesStartIndex = new TI.LongInteger());
					Add(new TI.Pad(8 + 4));
					Add(LightmapVerticesCount = new TI.LongInteger());
					Add(LightmapVerticesStartIndex = new TI.LongInteger());
					Add(new TI.Pad(8));
					Add(UncompressedVertices = new TI.Data(this, TI.DataType.Vertex));
					Add(CompressedVertices = new TI.Data(this, TI.DataType.VertexCompressed));
				}
			};
			#endregion

			public structure_bsp_lightmap_block() : base(3)
			{
				Add(Bitmap = new TI.ShortInteger());
				Add(new TI.Pad(2 + 16));
				Add(Materials = new TI.Block<structure_bsp_material_block>(this, 2048));
			}
		};
		#endregion

		#region structure_bsp_lens_flare_block
		public partial class structure_bsp_lens_flare_block
		{
			public structure_bsp_lens_flare_block() : base(1)
			{
				Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
			}
		};
		#endregion

		#region structure_bsp_lens_flare_marker_block
		public partial class structure_bsp_lens_flare_marker_block
		{
			public structure_bsp_lens_flare_marker_block() : base(5)
			{
				Add(Position = new TI.RealPoint3D());
				Add(DirectionIComponent = new TI.ByteInteger());
				Add(DirectionJComponent = new TI.ByteInteger());
				Add(DirectionKComponent = new TI.ByteInteger());
				Add(LensFlareIndex = new TI.ByteInteger());
			}
		};
		#endregion

		#region structure_bsp_cluster_block
		public partial class structure_bsp_cluster_block
		{
			#region structure_bsp_subcluster_block
			public partial class structure_bsp_subcluster_block
			{
				public structure_bsp_subcluster_block() : base(4)
				{
					Add(WorldBoundsX = new TI.RealBounds());
					Add(WorldBoundsY = new TI.RealBounds());
					Add(WorldBoundsZ = new TI.RealBounds());
					Add(SurfaceIndices = new TI.Block<field_block<TI.LongInteger>>(this, 128));
				}
			};
			#endregion

			#region structure_bsp_mirror_block
			public partial class structure_bsp_mirror_block
			{
				public structure_bsp_mirror_block() : base(4)
				{
					Add(Plane = new TI.RealPlane3D());
					Add(new TI.Pad(20));
					Add(Shader = new TI.TagReference(this, TagGroups.shdr));
					Add(Vertices = new TI.Block<field_block<TI.RealPoint3D>>(this, 512));
				}
			};
			#endregion

			public structure_bsp_cluster_block() : base(14)
			{
				Add(Sky = new TI.ShortInteger());
				Add(Fog = new TI.ShortInteger());
				Add(BackgroundSound = new TI.BlockIndex());
				Add(SoundEnvironment = new TI.BlockIndex());
				Add(Weather = new TI.BlockIndex());
				Add(TransitionStructureBsp = new TI.ShortInteger());
				Add(new TI.Pad(4 + 24));
				Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 1024));
				Add(Subclusters = new TI.Block<structure_bsp_subcluster_block>(this, 4096));
				Add(FirstLensFlareMarkerIndex = new TI.ShortInteger());
				Add(LensFlareMarkerCount = new TI.ShortInteger());
				Add(SurfaceIndices = new TI.Block<field_block<TI.LongInteger>>(this, 32768));
				Add(Mirrors = new TI.Block<structure_bsp_mirror_block>(this, 16));
				Add(Portals = new TI.Block<field_block<TI.ShortInteger>>(this, 128));
			}
		};
		#endregion

		#region structure_bsp_cluster_portal_block
		public partial class structure_bsp_cluster_portal_block
		{
			public structure_bsp_cluster_portal_block() : base(8)
			{
				Add(FrontCluster = new TI.ShortInteger());
				Add(BackCluster = new TI.ShortInteger());
				Add(PlaneIndex = new TI.LongInteger());
				Add(Centroid = new TI.RealPoint3D());
				Add(BoundingRadius = new TI.Real());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(24));
				Add(Vertices = new TI.Block<field_block<TI.RealPoint3D>>(this, 128));
			}
		};
		#endregion

		#region structure_bsp_breakable_surface_block
		public partial class structure_bsp_breakable_surface_block
		{
			public structure_bsp_breakable_surface_block() : base(4)
			{
				Add(Centroid = new TI.RealPoint3D());
				Add(Radius = new TI.Real());
				Add(CollisionSurfaceIndex = new TI.LongInteger());
				Add(new TI.Pad(28));
			}
		};
		#endregion

		#region structure_bsp_fog_plane_block
		public partial class structure_bsp_fog_plane_block
		{
			public structure_bsp_fog_plane_block() : base(4)
			{
				Add(FrontRegion = new TI.BlockIndex());
				Add(new TI.Pad(2));
				Add(Plane = new TI.RealPlane3D());
				Add(Vertices = new TI.Block<field_block<TI.RealPoint3D>>(this, 4096));
			}
		};
		#endregion

		#region structure_bsp_fog_region_block
		public partial class structure_bsp_fog_region_block
		{
			public structure_bsp_fog_region_block() : base(3)
			{
				Add(new TI.Pad(36));
				Add(FogPalette = new TI.BlockIndex());
				Add(WeatherPalette = new TI.BlockIndex());
			}
		};
		#endregion

		#region structure_bsp_fog_palette_block
		public partial class structure_bsp_fog_palette_block
		{
			public structure_bsp_fog_palette_block() : base(5)
			{
				Add(Name = new TI.String());
				Add(Fog = new TI.TagReference(this, TagGroups.fog_));
				Add(new TI.Pad(4));
				Add(FogScaleFunction = new TI.String());
				Add(new TI.Pad(52));
			}
		};
		#endregion

		#region structure_bsp_weather_palette_block
		public sealed partial class structure_bsp_weather_palette_block
		{
			public structure_bsp_weather_palette_block() : base(11)
			{
				Add(Name = new TI.String());
				Add(ParticleSystem = new TI.TagReference(this, TagGroups.rain));
				Add(new TI.Pad(4));
				Add(ParticleSystemScaleFunction = new TI.String());
				Add(new TI.Pad(44));
				Add(Wind = new TI.TagReference(this, TagGroups.wind));
				Add(WindDirection = new TI.RealVector3D());
				Add(WindMagnitude = new TI.Real());
				Add(new TI.Pad(4));
				Add(WindScaleFunction = new TI.String());
				Add(new TI.Pad(44));
			}
		};
		#endregion

		#region structure_bsp_weather_polyhedron_block
		public partial class structure_bsp_weather_polyhedron_block
		{
			public structure_bsp_weather_polyhedron_block() : base(4)
			{
				Add(BoundingSphereCenter = new TI.RealPoint3D());
				Add(BoundingSphereRadius = new TI.Real());
				Add(new TI.Pad(4));
				Add(Planes = new TI.Block<field_block<TI.RealPlane3D>>(this, 16));
			}
		};
		#endregion

		#region structure_bsp_background_sound_palette_block
		public partial class structure_bsp_background_sound_palette_block
		{
			public structure_bsp_background_sound_palette_block() : base(5)
			{
				Add(Name = new TI.String());
				Add(BackgroundSound = new TI.TagReference(this, TagGroups.lsnd));
				Add(new TI.Pad(4));
				Add(ScaleFunction = new TI.String());
				Add(new TI.Pad(32));
			}
		};
		#endregion

		#region structure_bsp_sound_environment_palette_block
		public partial class structure_bsp_sound_environment_palette_block
		{
			public structure_bsp_sound_environment_palette_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(SoundEnvironment = new TI.TagReference(this, TagGroups.snde));
				Add(new TI.Pad(32));
			}
		};
		#endregion

		#region structure_bsp_marker_block
		public partial class structure_bsp_marker_block
		{
			public structure_bsp_marker_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(Rotation = new TI.RealQuaternion());
				Add(Position = new TI.RealPoint3D());
			}
		};
		#endregion

		#region structure_bsp_detail_object_data_block
		public partial class structure_bsp_detail_object_data_block
		{
			#region global_detail_object_cells_block
			public partial class global_detail_object_cells_block
			{
				public global_detail_object_cells_block() : base(8)
				{
					Add(Shorts1 = new TI.ShortInteger());
					Add(Shorts2 = new TI.ShortInteger());
					Add(Shorts3 = new TI.ShortInteger());
					Add(Shorts4 = new TI.ShortInteger());

					Add(Longs1 = new TI.LongInteger());
					Add(Longs2 = new TI.LongInteger());
					Add(Longs3 = new TI.LongInteger());

					Add(new TI.Pad(12));
				}
			};
			#endregion

			#region global_detail_object_block
			public partial class global_detail_object_block
			{
				public global_detail_object_block() : base(5)
				{
					Add(Chars1 = new TI.ByteInteger());
					Add(Chars2 = new TI.ByteInteger());
					Add(Chars3 = new TI.ByteInteger());
					Add(Chars4 = new TI.ByteInteger());

					Add(Short = new TI.ShortInteger());
				}
			};
			#endregion

			#region global_z_reference_vector_block
			public partial class global_z_reference_vector_block
			{
				public global_z_reference_vector_block() : base(4)
				{
					Add(Fields1 = new TI.Real());
					Add(Fields2 = new TI.Real());
					Add(Fields3 = new TI.Real());
					Add(Fields4 = new TI.Real());
				}
			};
			#endregion

			public structure_bsp_detail_object_data_block() : base(5)
			{
				Add(Cells = new TI.Block<global_detail_object_cells_block>(this, 262144));
				Add(Instances = new TI.Block<global_detail_object_block>(this, 2097152));
				Add(Counts = new TI.Block<field_block<TI.ShortInteger>>(this, 8388608));
				Add(ZReferenceVectors = new TI.Block<global_z_reference_vector_block>(this, 262144));
				Add(new TI.Pad(16));
			}
		};
		#endregion

		#region structure_bsp_runtime_decal_block
		public partial class structure_bsp_runtime_decal_block
		{
			public structure_bsp_runtime_decal_block() : base(1)
			{
				Add(Skipper = new TI.Skip(16));
			}
		};
		#endregion

		#region global_map_leaf_block
		public partial class global_map_leaf_block
		{
			#region map_leaf_face_block
			public partial class map_leaf_face_block
			{
				public map_leaf_face_block() : base(2)
				{
					Add(NodeIndex = new TI.LongInteger());
					Add(Vertices = new TI.Block<field_block<TI.RealPoint2D>>(this, 64));
				}
			};
			#endregion

			public global_map_leaf_block() : base(2)
			{
				Add(Faces = new TI.Block<map_leaf_face_block>(this, 256));
				Add(PortalIndices = new TI.Block<field_block<TI.LongInteger>>(this, 256));
			}
		};
		#endregion

		#region global_leaf_portal_block
		public partial class global_leaf_portal_block
		{
			public global_leaf_portal_block() : base(4)
			{
				Add(PlaneIndex = new TI.LongInteger());
				Add(BackLeafIndex = new TI.LongInteger());
				Add(FrontLeafIndex = new TI.LongInteger());
				Add(Vertices = new TI.Block<field_block<TI.RealPoint3D>>(this, 64));
			}
		};
		#endregion

		#region Ctor
		public structure_bsp_group() : base(52)
		{
			Add(LightmapBitmaps = new TI.TagReference(this, TagGroups.bitm));
			Add(VehicleFloor = new TI.Real());
			Add(VehicleCeiling = new TI.Real());
			Add(new TI.Pad(20));

			Add(DefaultAmbientColor = new TI.RealColor());
			Add(new TI.Pad(4));

			Add(DefaultDistantLight0Color = new TI.RealColor());
			Add(DefaultDistantLight0ColorDirection = new TI.RealVector3D());
			Add(DefaultDistantLight1Color = new TI.RealColor());
			Add(DefaultDistantLightColor1Direction = new TI.RealVector3D());
			Add(new TI.Pad(12));

			Add(DefaultReflectionTint = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(DefaultShadowVector = new TI.RealVector3D());
			Add(DefaultShadowColor = new TI.RealColor());
			Add(new TI.Pad(4));

			Add(CollisionMaterials = new TI.Block<structure_collision_materials_block>(this, 512));
			Add(CollisionBsp = new TI.Block<collision_bsp_block>(this, 1));
			Add(Nodes = new TI.Block<structure_bsp_node_block>(this, 131072));

			Add(WorldBoundsX = new TI.RealBounds());
			Add(WorldBoundsY = new TI.RealBounds());
			Add(WorldBoundsZ = new TI.RealBounds());

			Add(Leaves = new TI.Block<structure_bsp_leaf_block>(this, 65536));
			Add(LeafSurfaces = new TI.Block<structure_bsp_surface_reference_block>(this, 262144));
			Add(Surfaces = new TI.Block<structure_bsp_surface_block>(this, 131072));

			Add(Lightmaps = new TI.Block<structure_bsp_lightmap_block>(this, 128));
			Add(new TI.Pad(12));

			Add(LensFlares = new TI.Block<structure_bsp_lens_flare_block>(this, 256));
			Add(LensFlareMarkers = new TI.Block<structure_bsp_lens_flare_marker_block>(this, 65536));

			Add(Clusters = new TI.Block<structure_bsp_cluster_block>(this, 8192));
			Add(ClusterData = new TI.Data(this));
			Add(ClusterPortals = new TI.Block<structure_bsp_cluster_portal_block>(this, 512));
			Add(new TI.Pad(12));

			Add(BreakableSurfaces = new TI.Block<structure_bsp_breakable_surface_block>(this, 256));
			Add(FogPlanes = new TI.Block<structure_bsp_fog_plane_block>(this, 32));
			Add(FogRegions = new TI.Block<structure_bsp_fog_region_block>(this, 32));
			Add(FogPalette = new TI.Block<structure_bsp_fog_palette_block>(this, 32));
			Add(new TI.Pad(24));

			Add(WeatherPalette = new TI.Block<structure_bsp_weather_palette_block>(this, 32));
			Add(WeatherPolyhedra = new TI.Block<structure_bsp_weather_polyhedron_block>(this, 32));
			Add(new TI.Pad(24));

			Add(PathfindingSurfaces = new TI.Block<field_block<TI.ByteInteger>>(this, 131072));
			Add(PathfindingEdges = new TI.Block<field_block<TI.ByteInteger>>(this, 262144));

			Add(BackgroundSoundPalette = new TI.Block<structure_bsp_background_sound_palette_block>(this, 64));
			Add(SoundEnvironmentPalette = new TI.Block<structure_bsp_sound_environment_palette_block>(this, 64));
			Add(SoundPASData = new TI.Data(this));
			Add(new TI.Pad(24));

			Add(Markers = new TI.Block<structure_bsp_marker_block>(this, 1024));
			Add(DetailObjects = new TI.Block<structure_bsp_detail_object_data_block>(this, 1));
			Add(RuntimeDecals = new TI.Block<structure_bsp_runtime_decal_block>(this, 6144));
			Add(new TI.Pad(8 + 
			
			// leaf_map
				4)); // tag_block*, sizeof(0xC)
			Add(LeafMapLeaves = new TI.Block<global_map_leaf_block>(this, 65536));
			Add(LeafMapPortals = new TI.Block<global_leaf_portal_block>(this, 524288));
		}
		#endregion
	};
}