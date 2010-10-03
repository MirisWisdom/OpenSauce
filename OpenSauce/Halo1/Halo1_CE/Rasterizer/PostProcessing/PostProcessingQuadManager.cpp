#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/PostProcessingQuadManager.hpp"

namespace Yelo
{
	namespace Postprocessing
	{
		/////////////////////////////////////////////////////////////////////
		// c_quad_instance
		uint32				c_quad_instance::GetRef()
		{
			return m_reference_count;
		}
		void				c_quad_instance::AddRef()
		{
			m_reference_count++;
		}
		void				c_quad_instance::Release()
		{
			m_reference_count--;
		}

		c_quad_instance*	c_quad_instance::GetNext()
		{
			return m_list.next;
		}
		void				c_quad_instance::SetNext(c_quad_instance* next)
		{
			m_list.next = next;
		}
		c_quad_instance*	c_quad_instance::GetPrevious()
		{
			return m_list.previous;
		}
		void				c_quad_instance::SetPrevious(c_quad_instance* previous)
		{
			m_list.previous = previous;
		}
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// c_quad_manager
		c_quad_manager		c_quad_manager::g_instance;

		void				c_quad_manager::CullQuads()
		{
			c_quad_instance* quad_instance = m_quad_list;
			while(quad_instance)
			{
				c_quad_instance* quad_next = quad_instance->GetNext();

				if(!quad_instance->GetRef())
				{
					if(quad_instance->GetPrevious())
						quad_instance->GetPrevious()->SetNext(quad_instance->GetNext());
					if(quad_instance->GetNext())
						quad_instance->GetNext()->SetPrevious(quad_instance->GetPrevious());

					if(quad_instance == m_quad_list)
						m_quad_list = quad_next;

					quad_instance->Ctor();
					delete quad_instance;
				}

				quad_instance = quad_next;
			}
		}
		c_quad_instance*	c_quad_manager::GetExistingQuad(const int16 x_tessellation, int16 y_tessellation)
		{
			c_quad_instance* quad_out = NULL;
			// loop through quad list
			c_quad_instance* quad_instance = m_quad_list;
			while(quad_instance)
			{
				// compare x and y tesselation
				if(quad_instance->m_quad.tessellation.x == x_tessellation &&
				quad_instance->m_quad.tessellation.y == y_tessellation)
				{
					// return quad instance pointer if a match is found
					quad_out = quad_instance;
					break;
				}
				quad_instance = quad_instance->GetNext();
			}
			// return NULL if no match is found
			return quad_out;
		}
		int32				c_quad_manager::GetVertexCount()
		{
			int32 count = 0;
			// loop through quads adding vertex_count for that quad
			c_quad_instance* quad_instance = m_quad_list;
			while(quad_instance)
			{
				count += quad_instance->m_quad.vertex_count;
				quad_instance = quad_instance->GetNext();
			}
			return count;
		}
		int32				c_quad_manager::GetIndexCount()
		{
			int32 count = 0;
			// loop through quads adding primitive_count * 3 to get the index count for that quad
			c_quad_instance* quad_instance = m_quad_list;
			while(quad_instance)
			{
				count += quad_instance->m_quad.primitive_count * 3;
				quad_instance = quad_instance->GetNext();
			}
			return count;
		}
		void				c_quad_manager::AddVertices(s_postprocess_vertex*& buffer_pointer, 
			const c_quad_instance* quad,
			const uint32 width,
			const uint32 height)
		{
			// loop through tess x and tess y calculating each vertex position and texture coordinates
			real x_pos_increment = CAST(real, width) / quad->m_quad.tessellation.x;
			real y_pos_increment = CAST(real, height) / quad->m_quad.tessellation.y;
			real x_tex_increment = 1.0f / quad->m_quad.tessellation.x;
			real y_tex_increment = 1.0f / quad->m_quad.tessellation.y;
			for(int y = 0; y < quad->m_quad.tessellation.y + 1; y++)
			{
				for(int x = 0; x < quad->m_quad.tessellation.x + 1; x++)
				{
					buffer_pointer->m_position.x = (x * x_pos_increment) - 0.5f;
					buffer_pointer->m_position.y = -((y * y_pos_increment) - 0.5f);
					buffer_pointer->m_position.z = 10.0f;
					buffer_pointer->m_texcoord0.x = x * x_tex_increment;
					buffer_pointer->m_texcoord0.y = y * y_tex_increment;
					buffer_pointer->m_texcoord1 = buffer_pointer->m_texcoord0;
					buffer_pointer++;
				}
			}
		}
		void				c_quad_manager::AddIndices(s_face*& buffer_pointer, c_quad_instance* quad)
		{
			// build the index table
			int counter = 0;
			int16** index_array = new int16*[quad->m_quad.tessellation.y + 1];
			for(int y = 0; y < quad->m_quad.tessellation.y + 1; y++)
			{
				index_array[y] = new int16[quad->m_quad.tessellation.x + 1];
				for(int x = 0; x < quad->m_quad.tessellation.x + 1; x++)
				{
					index_array[y][x] = counter;
					counter++;
				}
			}

			// loop through tess x and tess y setting the indeices according to the index table
			for(int y = 0; y < quad->m_quad.tessellation.y; y++)
			{
				for(int x = 0; x < quad->m_quad.tessellation.x; x++)
				{
					buffer_pointer->m_indices[0] = index_array[ y + 1 ][ x + 0 ];
					buffer_pointer->m_indices[1] = index_array[ y + 0 ][ x + 0 ];
					buffer_pointer->m_indices[2] = index_array[ y + 0 ][ x + 1 ];
					buffer_pointer++;
					buffer_pointer->m_indices[0] = index_array[ y + 0 ][ x + 1 ];
					buffer_pointer->m_indices[1] = index_array[ y + 1 ][ x + 1 ];
					buffer_pointer->m_indices[2] = index_array[ y + 1 ][ x + 0 ];
					buffer_pointer++;
				}
			}

			// delete the index table
			for(int y = 0; y < quad->m_quad.tessellation.y + 1; y++)
				delete [] index_array[y];
			delete [] index_array;
		}

		c_quad_instance*	c_quad_manager::CreateQuad(const int16 x_tessellation, int16 y_tessellation)
		{
			// lock the axis tesselation to the maximum if necessary
			c_quad_instance* quad = NULL;
			int16 x_tess = max(x_tessellation, 1);
			x_tess = min(x_tess, k_maximum_quads_per_axis);
			int16 y_tess = max(y_tessellation, 1);
			y_tess = min(y_tess, k_maximum_quads_per_axis);
			
			// look for an existing quad using GetExistingQuad, if found return that quad
			quad = GetExistingQuad(x_tess, y_tess);

			// if not, add a quad instance to the list and return that
			if(!quad)
			{
				quad = new c_quad_instance();
				quad->Ctor(x_tess, y_tess);
				if(!m_quad_list)
					m_quad_list = quad;
				else
				{
					c_quad_instance* quad_instance = m_quad_list;
					while(quad_instance->GetNext())
						quad_instance = quad_instance->GetNext();
					quad_instance->SetNext(quad);
					quad->SetPrevious(quad_instance);
				}
			}

			quad->AddRef();
			return quad;
		}
		HRESULT				c_quad_manager::BuildBuffers(IDirect3DDevice9* device, uint32 width, uint32 height)
		{
			// this is run after all quads have been added to the list
			if(!device)
				return E_FAIL;

			// remove quads that have a reference count of zero
			CullQuads();

			// to create a vertex buffer we need the vertex count
			int32 vertex_count = GetVertexCount();
			// to create an index buffer we need the index count
			int32 index_count = GetIndexCount();

			if(!vertex_count || !index_count)
				return E_FAIL;

			// create the vertex and index buffers                      
			device->CreateVertexBuffer(
				vertex_count * sizeof(s_postprocess_vertex), 
				D3DUSAGE_WRITEONLY, 
				D3DFVF_XYZ | D3DFVF_TEX2, 
				D3DPOOL_DEFAULT, 
				&m_buffers.vertex,
				NULL);
			device->CreateIndexBuffer(
				index_count * sizeof(WORD),
				D3DUSAGE_WRITEONLY, 
				D3DFMT_INDEX16, 
				D3DPOOL_DEFAULT, 
				&m_buffers.index, 
				NULL);

			// fill the vertex and index buffers 
			s_postprocess_vertex* vertices;
			m_buffers.vertex->Lock( 0, 0, CAST_PTR(void**, &vertices), NULL );

			s_face* indicies;
			m_buffers.index->Lock( 0, 0, CAST_PTR(void**, &indicies), NULL );

			int32 vertex_offset = 0;
			int32 index_offset = 0;
			c_quad_instance* quad_instance = m_quad_list;
			while(quad_instance)
			{
				AddVertices(vertices, quad_instance, width, height);
				AddIndices(indicies, quad_instance);

				quad_instance->m_quad.start_vertex = vertex_offset;
				quad_instance->m_quad.start_index = index_offset;
				vertex_offset += quad_instance->m_quad.vertex_count;
				index_offset += quad_instance->m_quad.primitive_count * 3;

				quad_instance = quad_instance->GetNext();
			}
			m_buffers.vertex->Unlock();
			m_buffers.index->Unlock();

			return S_OK;
		}
		HRESULT				c_quad_manager::SetBuffers(IDirect3DDevice9* device)
		{
			HRESULT hr = device->SetStreamSource(0, m_buffers.vertex, 0, sizeof(s_postprocess_vertex));
			hr |= device->SetIndices(m_buffers.index);
			return hr;
		}
		void				c_quad_manager::Release()
		{
			// release buffers
			Yelo::safe_release(m_buffers.vertex);
			Yelo::safe_release(m_buffers.index);
		}
		/////////////////////////////////////////////////////////////////////
	};
};