#pragma once

namespace Yelo
{
	namespace Postprocessing
	{	
		/////////////////////////////////////////////////////////////////////
		// Vertex structure for full screen quad
		/////////////////////////////////////////////////////////////////////
		struct s_postprocess_vertex
		{
			real_point3d m_position;
			real_point2d m_texcoord0;
			real_point2d m_texcoord1;
		};

		class c_quad_instance
		{
			uint32		m_reference_count;
			struct{
				c_quad_instance*	previous;
				c_quad_instance*	next;
			}m_list;
		public:
			struct{
				point2d		tessellation;
				int32		vertex_count;
				int32		primitive_count;
				int32		start_vertex;
				int32		start_index;
			}m_quad;

			void Ctor(const int16 x = 1, const int16 y = 1)
			{
				m_quad.tessellation.x = x;
				m_quad.tessellation.y = y;
				m_quad.start_index = 0;
				m_quad.vertex_count = (m_quad.tessellation.x + 1) * (m_quad.tessellation.y + 1);
				m_quad.primitive_count = (m_quad.tessellation.x * m_quad.tessellation.y) * 2;
				m_list.next = NULL; 
				m_list.previous = NULL;
				m_reference_count = 0;
			}

			// handles reference counting
			uint32					GetRef();
			void					AddRef();
			void					Release();

			// linked list functions
			c_quad_instance*		GetNext();
			void					SetNext(c_quad_instance* next);
			c_quad_instance*		GetPrevious();
			void					SetPrevious(c_quad_instance* previous);
		};

		class c_quad_manager
		{ 		
			enum {
				k_maximum_quads_per_axis = 20, // maximum number of quads allowed on each axis
			};
			struct s_face
			{
				int16 m_indices[3];
			};

			struct{
				LPDIRECT3DVERTEXBUFFER9		vertex;
				LPDIRECT3DINDEXBUFFER9		index;
			}m_buffers;
			// head of quad list
			c_quad_instance*		m_quad_list;

		public:
			static c_quad_manager	g_instance;

			c_quad_instance*		CreateQuad(const int16 x_tessellation, int16 y_tessellation);
			HRESULT					BuildBuffers(IDirect3DDevice9* device, uint32 width, uint32 height);
			HRESULT					SetBuffers(IDirect3DDevice9* device);
			void					Release();

		private:
			void					CullQuads();
			c_quad_instance*		GetExistingQuad(const int16 x_tessellation, int16 y_tessellation);
			int32					GetVertexCount();
			int32					GetIndexCount();
			void					AddVertices(s_postprocess_vertex*& buffer_pointer, 
				const c_quad_instance* quad,
				const uint32 width,
				const uint32 height);
			void					AddIndices(s_face*& buffer_pointer, c_quad_instance* quad);			
		};
	};
};