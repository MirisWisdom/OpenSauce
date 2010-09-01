/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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
#pragma once

#if !PLATFORM_IS_DEDI
#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingEffectGenericBase.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace External {

		/////////////////////////////////////////////////////////////////////
		// Tag struct for holding the external effect shader indices 
		/////////////////////////////////////////////////////////////////////
		struct s_shader_postprocess_external_effect : public TagGroups::s_shader_postprocess_effect_generic
		{
			TAG_TBLOCK_(shader_indices, TagGroups::s_shader_postprocess_collection_shader_index);

			~s_shader_postprocess_external_effect() 
			{				
				// delete any memory that may have been assigned to the shader_indicies tag block
				delete [] shader_indices.Address;
			}
		};

		/////////////////////////////////////////////////////////////////////
		// External effect class for non tag cache based effects 
		/////////////////////////////////////////////////////////////////////
		class c_external_effect : public c_generic_effect_base
		{
		public:
			s_shader_postprocess_external_effect* m_effect_external;

			bool				ValidateEffect();
			void				AllocateCustomResources(IDirect3DDevice9* pDevice);
			void				SetSource(void* pSource);

			virtual void		Ctor()
			{
				m_effect_external = NULL;

				c_generic_effect_base::Ctor();
			}

			virtual void		Dtor()
			{
				// the tag reference is manually allocated memory, so delete it
				delete m_effect_external;

				c_generic_effect_base::Dtor();
			}
		};

	}; }; };
};
#endif