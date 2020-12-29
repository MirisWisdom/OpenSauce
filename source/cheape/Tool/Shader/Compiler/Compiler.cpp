/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Compiler.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Shader/Collection/c_collection_definition.hpp"
#include "Tool/Shader/Compiler/Collection/c_collection_writer_factory.hpp"
#include "Tool/Shader/Compiler/Effect/i_effect_writer.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_compiler.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_writer_factory.hpp"

#include "Tool/Console.hpp"
#include "Rasterizer/DX9/DX9.hpp"

using namespace boost;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Builds a shader effect. </summary>
		///
		/// <param name="source_file">	   	The shader source file. </param>
		/// <param name="intermediate_dir">	The intermediate output dir. </param>
		/// <param name="shader_target">   	The shader target. </param>
		/// <param name="shader_writer">   	[in] The shader writer. </param>
		void BuildEffect(const filesystem::path& source_file
			, const filesystem::path& intermediate_dir
			, const std::string& shader_target
			, i_effect_writer& shader_writer)
		{
			auto& filename = source_file.filename();

			Console::ColorPrint(0x000B, "Current Effect - ");
			Console::ColorPrintF(0x0007, "%s\r\n", filename.string().c_str());

			LPD3DXEFFECT effect;
			YELO_ASSERT_DISPLAY(c_effect_compiler::CompileEffect(source_file, shader_target, effect), "ERROR: failed to compile an effect");

			filesystem::path effect_file(intermediate_dir);
			effect_file /= filename;
			effect_file.replace_extension(".bin");

			std::string effect_name(filename.replace_extension().string());
			shader_writer.WriteEffect(effect_file, effect_name, shader_target, effect);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Builds the collections effects. </summary>
		///
		/// <param name="source_dir">	   	The shader source dir. </param>
		/// <param name="intermediate_dir">	The intermediate output dir. </param>
		/// <param name="shader_target">   	The shader target. </param>
		/// <param name="shader_writer">   	[in] The shader writer. </param>
		void BuildEffects(const filesystem::path& source_dir
			, const filesystem::path& intermediate_dir
			, const std::string& shader_target
			, i_effect_writer& shader_writer)
		{
			filesystem::directory_iterator end;
			for (filesystem::directory_iterator iter(source_dir); iter != end; ++iter)
			{
				filesystem::path current_path(iter->path());
				if (is_directory(current_path) || !exists(current_path))
				{
					continue;
				}

				if (current_path.extension() != ".fx")
				{
					continue;
				}

				BuildEffect(current_path, intermediate_dir, shader_target, shader_writer);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Compiles a shader collection. </summary>
		///
		/// <param name="arguments">	[in] If non-null, the function arguments. </param>
		void PLATFORM_API compile_shaders(char* arguments[])
		{
			struct s_arguments {
				cstring source_directory;
				cstring intermediate_directory;
				cstring output_directory;
				cstring collection_definition;
				cstring specific_shader;
			}*args = CAST_PTR(s_arguments*, arguments);

			filesystem::path source_path(args->source_directory);
			filesystem::path intermediate_path(args->intermediate_directory);
			filesystem::path output_path(args->output_directory);

			// Validate the provided paths
			YELO_ASSERT_DISPLAY(exists(source_path), "ERROR: source directory does not exist [%s]", source_path.string().c_str());

			// Initialize the render device (used to compile the shaders)
			Rasterizer::DX9::Initialize();

			// Open the collection definition
			filesystem::path collection_definition_path(source_path);
			collection_definition_path /= args->collection_definition;
			collection_definition_path.replace_extension(".xml");

			YELO_ASSERT_DISPLAY(exists(collection_definition_path), "ERROR: collection definition does not exist [%s]", collection_definition_path.string().c_str());

			c_collection_definition shader_collection;
			shader_collection.Load(collection_definition_path);

			// Build the effect output path
			filesystem::path effect_output_directory(intermediate_path);
			effect_output_directory /= shader_collection.GetName();
			if (!exists(effect_output_directory))
			{
				YELO_ASSERT_DISPLAY(create_directories(effect_output_directory), "ERROR: failed to create effect output directory");
			}

			// Build the collection output path
			filesystem::path collection_output_directory(output_path);
			if (!exists(collection_output_directory))
			{
				YELO_ASSERT_DISPLAY(create_directories(collection_output_directory), "ERROR: failed to create collection output directory");
			}

			// Build the source file path
			filesystem::path shader_source_directory(source_path);
			shader_source_directory /= shader_collection.GetSourceDirectory();

			YELO_ASSERT_DISPLAY(exists(shader_source_directory), "ERROR: shader source location does not exist");

			// Build the collection file path
			filesystem::path collection_path(output_path);
			collection_path /= shader_collection.GetName();
			collection_path.replace_extension(".enc");

			// Print resolved paths
			Console::ColorPrint(0x000E, "Source Directory: \r\n");
			Console::ColorPrintF(0x0007, "\t%s\r\n", shader_source_directory.string().c_str());
			Console::ColorPrint(0x000E, "Collection Definition: \r\n");
			Console::ColorPrintF(0x0007, "\t%s\r\n", collection_definition_path.string().c_str());
			Console::ColorPrint(0x000E, "Effects Output Directory: \r\n");
			Console::ColorPrintF(0x0007, "\t%s\r\n", effect_output_directory.string().c_str());
			Console::ColorPrint(0x000E, "Shader Collection: \r\n");
			Console::ColorPrintF(0x0007, "\t%s\r\n", collection_path.string().c_str());

			// Build the effects
			auto& effect_writer = c_effect_writer_factory::GetEffectWriter(shader_collection.GetShaderType());

			std::string specific_shader(args->specific_shader);
			if (specific_shader.empty())
			{
				BuildEffects(shader_source_directory
					, effect_output_directory
					, shader_collection.GetCompilationTarget()
					, *effect_writer);
			}
			else
			{
				filesystem::path specific_file(shader_source_directory);
				specific_file /= specific_shader;
				specific_file.replace_extension(".fx");

				YELO_ASSERT_DISPLAY(exists(specific_file), "ERROR: specified shader does not exist");

				BuildEffect(specific_file
					, effect_output_directory
					, shader_collection.GetCompilationTarget()
					, *effect_writer);
			}

			auto& collection_writer = c_collection_writer_factory::GetCollectionWriter(shader_collection.GetShaderType());
			collection_writer->WriteCollection(effect_output_directory, collection_path, shader_collection);

			// Release the render device
			Rasterizer::DX9::Dispose();
		}
	};};};
};
#endif