/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/BuildPackedFiles.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Tool/PackedFile/Definition/c_packed_file_definition.hpp"
#include "Tool/PackedFile/Processing/i_packed_file_processor.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_factory.hpp"

using namespace boost;

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Processes the packed file. </summary>
		///
		/// <param name="source_path">			 	Full pathname of the source directory. </param>
		/// <param name="output_path">			 	Full pathname of the output directory. </param>
		/// <param name="packed_file_definition">	[in,out] The packed file definition. </param>
		void ProcessPackedFile(const filesystem::path& source_path
			, const filesystem::path& output_path
			, c_packed_file_definition_container& packed_file_definition)
		{
			// Build the output file path
			filesystem::path output_file(output_path);
			output_file /= packed_file_definition.m_name.Get();

			c_packed_file packed_file;
			
			printf("building packed file '%s'\n", packed_file_definition.m_name.Get().c_str());
			for(auto& file : packed_file_definition.m_files)
			{
				auto& id = file.m_id.Get();
				auto& type = file.m_type.Get();
				auto& path = file.m_path.Get();

				// Validate the file variables
				YELO_ASSERT_DISPLAY(!id.empty(), "ERROR: A files id is empty");
				YELO_ASSERT_DISPLAY(!type.empty(), "ERROR: A files type is empty");
				YELO_ASSERT_DISPLAY(!path.empty(), "ERROR: A files path is empty");
				
				printf("creating element '%s'\n", id.c_str());

				filesystem::path file_path(source_path);
				file_path /= file.m_path.Get();

				YELO_ASSERT_DISPLAY(filesystem::exists(file_path), "ERROR: A file to be packed does not exist: %s", file_path.string().c_str());

				// Get the file processor
				const auto file_processor = c_packed_file_processor_factory::GetPackedFileProcessor(type);

				// Build element
				c_packed_file::s_element_editor element;

				UINT id_length = id.length() + 1;
				element.source_id = new char[id_length];
				element.source_id[0] = 0;
				strcpy_s(element.source_id, id_length, id.c_str());

				// create the file element
				bool result = file_processor->ProcessElement(element, file_path);

				YELO_ASSERT_DISPLAY(result, "ERROR: Failed to create packed file entry");

				// Add the element to the packed file
				packed_file.AddElement(element);
			}

			// save the packed file
			puts("saving packed file");
			if(FAILED(packed_file.Save(output_file.string().c_str())))
			{
				puts("ERROR: Failed to save packed file");
				return;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Packs a number of files into a standard format for OS. </summary>
		///
		/// <param name="arguments">	[in,out] If non-null, the function arguments. </param>
		void PLATFORM_API build_packed_file(char* arguments[])
		{
			struct s_arguments {
				cstring source_directory;
				cstring output_directory;
				cstring definition_file;
			}* args = CAST_PTR(s_arguments *, arguments);
			
			// Validate input arguments
			filesystem::path source_path(args->source_directory); 
			filesystem::path output_path(args->output_directory); 
			
			YELO_ASSERT_DISPLAY(filesystem::exists(source_path), "ERROR: The source path does not exist: %s", source_path.string().c_str());
			
			if (!filesystem::exists(output_path))
			{
				YELO_ASSERT_DISPLAY(create_directories(output_path), "ERROR: failed to create packed file output directory");
			}

			// Open the definition file
			filesystem::path definition_path(source_path);
			definition_path /= args->definition_file;
			definition_path.replace_extension("xml");

			YELO_ASSERT_DISPLAY(filesystem::exists(definition_path), "ERROR: The definition file does not exist: %s", definition_path.string().c_str());

			printf("loading packed file definition ...");

			c_packed_file_definition packed_file_definition;
			packed_file_definition.Load(definition_path);

			puts("done");

			ProcessPackedFile(source_path, output_path, packed_file_definition.m_packed_file_defintion);
		};
	};};
};
#endif