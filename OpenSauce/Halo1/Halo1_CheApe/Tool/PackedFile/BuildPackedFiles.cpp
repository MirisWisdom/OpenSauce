/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/BuildPackedFiles.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Tool/PackedFile/Definition/c_packed_file_definition.hpp"

using namespace boost;

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{		
#include "Tool/PackedFile/BuildPackedFile_Actions.inl"

		enum packed_file_type{
			/*!
			 * \brief
			 * The default action is to simple pack the entire file unchanged.
			 */
			_packed_file_type_archive,
			/*!
			 * \brief
			 * The shader action assumes the file is a HLSL shader and will pack its compiled binary code.
			 */
			_packed_file_type_shader,

			_packed_file_type
		};

		typedef bool (*proc_pack_file_action_t)(c_packed_file::s_element_editor&, const filesystem::path&);

		const proc_pack_file_action_t g_pack_file_action[] = {
			CustomAction_Default,
			CustomAction_Shader
		};

		/*!
		 * \brief
		 * Set up a packed file element.
		 * 
		 * \param element
		 * Reference to the element to set up.
		 * 
		 * \param element_id
		 * The ID of the element.
		 * 
		 * \param element_source
		 * The source file to set up the element with.
		 * 
		 * \param element_type
		 * The type of file.
		 * 
		 * \returns
		 * Returns true if the element was set up correctly.
		 * 
		 * Sets up a packed file element, applying any custom process needed on the file.
		 */
		bool BuildPackedFileElement(c_packed_file::s_element_editor& element,
			const std::string& id,
			const filesystem::path& file_path, 
			const packed_file_type element_type)
		{
			// Copy the files id
			UINT id_length = id.length() + 1;
			element.source_id = new char[id_length];
			element.source_id[0] = 0;
			strcpy_s(element.source_id, id_length, id.c_str());

			// create a new element for the file
			// custom actions are used to add new processes to run on files when packing them
			// such as compiling shaders
			return (*g_pack_file_action[element_type])(element, file_path);
		}

		/*!
		 * \brief
		 * Processes all packed file elements in the xml definition.
		 * 
		 * \param first_element
		 * The first packed file element.
		 * 
		 * Processes all packed file elements in the xml definition.
		 */
		void ProcessPackedFile(const filesystem::path& source_path
			, const filesystem::path& output_path
			, c_packed_file_definition_container& packed_file_definition)
		{
			filesystem::path output_file(output_path);
			output_file /= packed_file_definition.m_name.Get();

			c_packed_file packed_file;
			
			printf("building packed file '%s'\n", packed_file_definition.m_name.Get().c_str());
			for(auto& file : packed_file_definition.m_files)
			{
				YELO_ASSERT_DISPLAY(!file.m_id.Get().empty(), "ERROR: A files id is empty");
				YELO_ASSERT_DISPLAY(!file.m_type.Get().empty(), "ERROR: A files type is empty");
				YELO_ASSERT_DISPLAY(!file.m_path.Get().empty(), "ERROR: A files path is empty");
				
				printf("creating element '%s'\n", file.m_id.Get().c_str());

				filesystem::path file_path(source_path);
				file_path /= file.m_path.Get();

				YELO_ASSERT_DISPLAY(filesystem::exists(file_path), "ERROR: A file to be packed does not exist: %s", file_path.string().c_str());

				auto& type_string = file.m_type.Get();
				packed_file_type file_type = _packed_file_type;
				if(type_string == "archive")
				{
					file_type = _packed_file_type_archive;
				}
				if(type_string == "shader")
				{
					file_type = _packed_file_type_shader;
				}
				
				YELO_ASSERT_DISPLAY(file_type != _packed_file_type, "ERROR: Unknown file type '%s'", type_string.c_str());

				// Build element
				c_packed_file::s_element_editor element;

				// create the file element
				bool result = BuildPackedFileElement(element, file.m_id, file_path, file_type);

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