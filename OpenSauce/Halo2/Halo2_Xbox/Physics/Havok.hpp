/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum BroadPhaseType
		{
			BROAD_PHASE_INVALID,
			BROAD_PHASE_ENTITY,
			BROAD_PHASE_PHANTOM,
			BROAD_PHASE_MAX_ID,
		};

		enum ResponseType
		{
			RESPONSE_INVALID,
			RESPONSE_SIMPLE_CONTACT,
			RESPONSE_REPORTING,
			RESPONSE_NONE,
			RESPONSE_MAX_ID,
		};

		enum DeactivatorType
		{
			DEACTIVATOR_INVALID,
			DEACTIVATOR_NEVER,
			DEACTIVATOR_SPATIAL,
			DEACTIVATOR_MAX_ID,
		};

		enum MotionType
		{
			MOTION_INVALID,
			MOTION_DYNAMIC,
			MOTION_SPHERE_INERTIA,
			MOTION_STABILIZED_SPHERE_INERTIA,
			MOTION_BOX_INERTIA,
			MOTION_STABILIZED_BOX_INERTIA,
			MOTION_KEYFRAMED,
			MOTION_FIXED,
			MOTION_MAX_ID,
		};

		enum SolverType
		{
			SOLVER_TYPE_INVALID,
			SOLVER_TYPE_2ITERS_SOFT,
			SOLVER_TYPE_2ITERS_MEDIUM,
			SOLVER_TYPE_2ITERS_HARD,
			SOLVER_TYPE_4ITERS_SOFT,
			SOLVER_TYPE_4ITERS_MEDIUM,
			SOLVER_TYPE_4ITERS_HARD,
			SOLVER_TYPE_8ITERS_SOFT,
			SOLVER_TYPE_8ITERS_MEDIUM,
			SOLVER_TYPE_8ITERS_HARD,
			SOLVER_TYPE_MAX_ID,
		};
	};

	namespace Havok
	{
		template<typename T> class hkArray
		{
		public:
			T* array; // 0x0
			int32 count; // 0x4
			int32 capacity; // 0x8
		}; BOOST_STATIC_ASSERT( sizeof(hkArray<void>) == 12);

		template<typename T, unsigned N> class hkInplaceArray : public hkArray<T>
		{
		public:
			T storage[N]; // 0xC
		};

		class hkVector3
		{
		public:
			float x, y, z, w;
		}; BOOST_STATIC_ASSERT( sizeof(hkVector3) == 16);

		class hkPlane
		{
		public:
			hkVector3 normal;
			float distance;
		}; BOOST_STATIC_ASSERT( sizeof(hkPlane) == 20);

		class hkVector4 : public hkVector3 {}; BOOST_STATIC_ASSERT( sizeof(hkVector4) == 16);

		class hkQuaternion : public hkVector4 {}; BOOST_STATIC_ASSERT( sizeof(hkQuaternion) == 16);

		class hkMatrix3
		{
		public:
			hkVector3 column[3]; // 'w' acts as padding due to alignment issues with mat3x3
		}; BOOST_STATIC_ASSERT( sizeof(hkMatrix3) == 48);

		class hkRotation : public hkMatrix3 {}; BOOST_STATIC_ASSERT( sizeof(hkRotation) == 48);

		class hkTransform
		{
		public:
			hkRotation rotation;
			hkVector3 translation;
		}; BOOST_STATIC_ASSERT( sizeof(hkTransform) == 64);



		class hkBaseObject
		{
		public:
			void** lpVtbl; // 0x0
		}; BOOST_STATIC_ASSERT( sizeof(hkBaseObject) == 4);

		class hkReferencedObject : public hkBaseObject
		{
		public:
			uint16 size; // 0x4
			uint16 referenceCount; // 0x6
		}; BOOST_STATIC_ASSERT( sizeof(hkReferencedObject) == 8);

		class hkSerializeable : public hkReferencedObject {}; BOOST_STATIC_ASSERT( sizeof(hkSerializeable) == 8);

		class hkShape abstract : public hkSerializeable
		{
		public:
			int32 userData; // 0x8
		}; BOOST_STATIC_ASSERT( sizeof(hkShape) == 12);

		class hkSphereRepShape abstract : public hkShape {}; BOOST_STATIC_ASSERT( sizeof(hkSphereRepShape) == 12);

		class hkConvexShape abstract : public hkShape
		{
		public:
			float radius; // 0xC
		}; BOOST_STATIC_ASSERT( sizeof(hkConvexShape) == 16);

		class hkTriangleShape : public hkConvexShape
		{
		public:
			hkVector4	vertexA, // 0x10
				vertexB, // 0x20
				vertexC; // 0x30
		}; BOOST_STATIC_ASSERT( sizeof(hkTriangleShape) == 64);

		class hkConvexSweepShape : public hkConvexShape
		{
		private:
			hkVector4 unknown1; // 0x10, init'd to 0
		public:
			float maxLinearVelocitySquared; // 0x20, or just maxVelocity. when init'd from a Cinfo its set to (maxLinearVelocity*maxLinearVelocity)
			float maxLinearVelocity; // 0x24
			float linearVelocityClipDiff; // 0x28
			float maxVelocityWithClip; // 0x2C, (maxLinearVelocity - linearVelocityClipDiff) when init'd from Cinfo
			hkConvexShape* childShape; // 0x30
		}; BOOST_STATIC_ASSERT( sizeof(hkConvexSweepShape) == 52);

		class hkCapsuleShape : public hkConvexShape
		{
		public:
			hkVector4	vertexA, // 0x10, 'w' set to Cinfo's 'radius'
				vertexB; // 0x20, 'w' set to Cinfo's 'radius'
		}; BOOST_STATIC_ASSERT( sizeof(hkCapsuleShape) == 48);

		class hkConvexVerticesShape : public hkConvexShape
		{
		public:
			struct FourVectors
			{
				hkVector4 x;
				hkVector4 y;
				hkVector4 z;
			};

			class hkInplaceArray : public hkArray<FourVectors> // FUCKING 16BYTE ALIGNMENT!
			{
			public:
				PAD32; // 0xC
				FourVectors storage[3]; // 0x10
			};

			hkVector4 aabbHalfExtents; // 0x10
			hkVector4 aabbCenter; // 0x20
			hkInplaceArray rotatedVertices; // 0x30
			// 0x30, 0x34, 0x38
			// [0x3C] 0x40

			int32 numVertices; // 0xD0

			hkArray<hkVector4> planeEquations; // 0xD4

		}; BOOST_STATIC_ASSERT( sizeof(hkConvexVerticesShape) == 224);

		class hkSphereShape abstract : public hkConvexShape {}; BOOST_STATIC_ASSERT( sizeof(hkSphereShape) == 16);

		class hkMultiSphereShape : public hkSphereRepShape
		{
		public:
			uint32 spheresCount; // 0xC
			hkVector4 spheres[8]; // 0x10, 'w' is radius
		}; BOOST_STATIC_ASSERT( sizeof(hkMultiSphereShape) == 144);

		class hkTransformShape : public hkShape
		{
		public:
			hkShape* childShape; // 0xC
			hkTransform transform; // 0x10
		}; BOOST_STATIC_ASSERT( sizeof(hkTransformShape) == 80);

		class hkShapeCollection abstract : public hkShape {}; BOOST_STATIC_ASSERT( sizeof(hkShapeCollection) == 12);

		class hkListShape : public hkShapeCollection
		{
		public:
			struct ChildInfo
			{
				hkShape* shape;
				uint32 collisionFilterInfo;
			};

			hkInplaceArray<ChildInfo, 4> childInfo; // 0xC
			// 0xC, 0x10, 0x14
			// 0x18
		}; BOOST_STATIC_ASSERT( sizeof(hkListShape) == 56);

		class hkBvTreeShape abstract : public hkShape {}; BOOST_STATIC_ASSERT( sizeof(hkBvTreeShape) == 12);

		class hkMoppCode
		{
		public:
			hkVector4 vector; // 0x0, 0x4, 0x8, 0xC
			int32 byteOrdering; // 0x10, 0xFF means little
		private:
			PAD32; PAD32; PAD32 // 0x14, 0x18, 0x1C
		public:
			int32 size; // 0x20
			int32 referenceCount; // 0x24
		private:
			PAD32; PAD32; // 0x28 0x2C
		public:
#pragma warning(push)
#pragma warning(disable : 4200)
			byte codes[];
#pragma warning(pop)
		}; BOOST_STATIC_ASSERT( sizeof(hkMoppCode) == 48);
		class hkMoppBvTreeShape : public hkBvTreeShape
		{
		public:
			hkShapeCollection* shapeCollection; // 0xC
			hkMoppCode* code; // 0x10
		}; BOOST_STATIC_ASSERT( sizeof(hkMoppBvTreeShape) == 20);
		class hkMoppCodeGenerator : public hkReferencedObject
		{
		public:
			hkArray<char> code; // 0x8
		}; BOOST_STATIC_ASSERT( sizeof(hkMoppCodeGenerator) == 20);  // i guess this is its total size...

		class hkMoppFitToleranceRequirements : hkReferencedObject//hkBaseObject
		{
			PAD32; // 0x8
			PAD32; // 0xC
		public:
			hkVector4 AbsoluteFitToleranceOfAxisAlignedTriangles; // 0x10
			float RelativeFitToleranceOfInternalNodes; // 0x20
			float AbsoluteFitToleranceOfInternalNodes; // 0x24
			float AbsoltueFitToleranceOfTrianles; // 0x28
		};  BOOST_STATIC_ASSERT( sizeof(hkMoppFitToleranceRequirements) == 44);

		class hkPhantomCallbackShape abstract : public hkShape {}; BOOST_STATIC_ASSERT( sizeof(hkPhantomCallbackShape) == 12);

		class hkConvexWelderShape : public hkShape
		{
		public:
			hkBvTreeShape* compoundShapeToBeWelded; // 0xC
		}; BOOST_STATIC_ASSERT( sizeof(hkConvexWelderShape) == 16);

		class hkCollisionFilter abstract : public hkSerializeable
		{
		public:
			void** somethinglpVtbl1; // 0x8, same as 0x14
			void** somethinglpVtbl2; // 0xC
			void** somethinglpVtbl3; // 0x10
			void** somethinglpVtbl4; // 0x14, same as 0x8
		}; BOOST_STATIC_ASSERT( sizeof(hkCollisionFilter) == 24);

		class hkNullCollisionFilter : public hkCollisionFilter {}; BOOST_STATIC_ASSERT( sizeof(hkNullCollisionFilter) == 24);

		class hkGroupFilter : public hkNullCollisionFilter
		{
		public:
			int32 nextFreeSystemGroup; // 0x18
			uint32* collisionLookupTable; // 0x1C, max is 32
		}; BOOST_STATIC_ASSERT( sizeof(hkGroupFilter) == 32);

		class hkBvShape : public hkShape
		{
		public:
			hkShape* boundingVolumeShape; // 0xC
			hkShape* childShape; // 0x10
		}; BOOST_STATIC_ASSERT( sizeof(hkBvShape) == 20);

		class hkBoxShape : public hkConvexShape
		{
		public:
			hkVector4 halfExtents; // 0x10
		}; BOOST_STATIC_ASSERT( sizeof(hkBoxShape) == 32);

		struct Property
		{
			uint32 key;
			uint32 value;
		}; BOOST_STATIC_ASSERT( sizeof(Property) == 8);

		class hkWorld;

		class hkWorldObject abstract : public hkSerializeable
		{
		private:
			hkWorld* World; // 0x8

		public:
			hkShape* shape; // 0xC, hkCollidable

		private:
			int32 UNKNOWN(1); // 0x10, set to -1
			hkVector4* Position; // 0x14
			int32 UNKNOWN(2)[3]; // 0x18
			// 0x1C
			// 0x20

		public:
			long_enum broadPhaseType; // 0x24, BroadPhaseType

		private:
			uint32 collisionFilterInfo; // 0x28
			hkWorldObject* UNKNOWN(3); // 0x2C, points to 'this'
			hkArray<Property> properties; // 0x30

		private:
			hkBaseObject* UNKNOWN(4); // 0x3C, object that describes the current state of this (ie position). Acts as the motion object
			// 1, isFixed.
			int32 UNKNOWN(5)[2];// 0x40
			// 0x44
		}; BOOST_STATIC_ASSERT( sizeof(hkWorldObject) == 72);

		class hkEntity abstract : public hkWorldObject
		{
		public:
			uint32 collisionResponse; // 0x48, rly a byte // ResponseType
			float friction; // 0x4C
			float restitution; // 0x50

		private:
			int32 UNKNOWN(0);// 0x54
		public:
			int32 storageIndex; // 0x58
			uint32 processContactCallbackDelay; // 0x5C, rly a ushort

		private:
			int32 UNKNOWN(1)[16];// 0x60
			// 0x64
			// 0x68
			// 0x6C
			// 0x70
			// 0x74
			// 0x78
			// 0x7C
			// 0x80
			// 0x84
			// 0x88
			// 0x8C
			// 0x90
			// 0x94
			// 0x98
			// 0x9C
		}; BOOST_STATIC_ASSERT( sizeof(hkEntity) == 160);

		class hkRigidBody : public hkEntity
		{
		public:
		}; BOOST_STATIC_ASSERT( sizeof(hkRigidBody) == 160);


		class hkWorld
		{
			// 0xC0, hkWorldPostSimulationListener*, EndOfFrameListener
			// 0xE0, hkVector, gravity
		};







		/*  BvShape is the only Havok object that isn't overloaded and also resides in
			tag data (phmo only)
		*/ 




		class c_transform_shape : public hkTriangleShape // phmo, crea & bipd
		{
		};

		class c_sphere_shape : public hkSphereShape // phmo, crea & bipd
		{
		};

		class c_capsule_shape : public hkCapsuleShape // phmo, crea & bipd
		{
		};

		class c_box_shape : public hkBoxShape
		{
		};

		class c_mopp_bv_tree_shape : public hkMoppBvTreeShape
		{
		};

		class c_phantom_callback_shape : public hkPhantomCallbackShape
		{
		};

		class c_list_shape : public hkListShape
		{
		};

		class c_convex_vertices_shape : public hkConvexVerticesShape
		{
		};

		class c_triangle_shape : public hkTriangleShape
		{
		};

		class c_multi_sphere_shape : public hkMultiSphereShape
		{
		};

		class c_convex_shape : public hkConvexShape
		{
		};

		class c_shape_collection : public hkShapeCollection
		{
		};

		class c_shape : public hkShape
		{
		};
	};
};