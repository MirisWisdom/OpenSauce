/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include "Tool/Shader/Compiler/Collection/i_collection_definition.hpp"
#include "Tool/Shader/Compiler/Collection/i_collection_writer.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class c_collection_writer
			abstract
			: public i_collection_writer
		{
			virtual void WriteEffects(FileIO::s_file_info& collection_file
				, const boost::filesystem::path& effects_directory
				, i_collection_definition& collection_definition) = 0;

		protected:
			void AppendEffectToCollection(FileIO::s_file_info& collection_file
				, const boost::filesystem::path& effect_file);

		public:
			void WriteCollection(const boost::filesystem::path& source_directory
				, const boost::filesystem::path& collection_path
				, i_collection_definition& collection_definition) final override;
		};
	};};};
};
#endif