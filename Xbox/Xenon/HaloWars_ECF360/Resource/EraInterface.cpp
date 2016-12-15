/*
	OpenSauce: HaloWars Data Interop

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Precompile.hpp"
#include "Resource/EraInterface.hpp"

#include "Resource/EraCryptor.hpp"

namespace Phoenix
{
	const char* c_era_interface::k_input_dir =	"game:\\input\\";
	const char* c_era_interface::k_output_dir =	"game:\\output\\";

	const char* c_era_interface::k_era_ext =			".era";
	const char* c_era_interface::k_decrypted_era_ext =	".era.bin";
	const char* c_era_interface::k_encrypted_era_ext =	".era";

	const char* c_era_interface::k_save_ext =			".sav";
	const char* c_era_interface::k_decrypted_save_ext = ".sav.bin";
	const char* c_era_interface::k_encrypted_save_ext = ".sav";

	void* c_era_interface::g_base_address = NULL;

	void c_era_interface::Initialize()
	{
		CreateDirectory(k_output_dir, NULL);

		g_base_address = VirtualAlloc(NULL, 0x04000000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		if(g_base_address == NULL) g_log_writer.Write("failed to allocate memory\n");
	}

	void c_era_interface::Dispose()
	{
	}

	BOOL c_era_interface::CryptFile(LPCSTR input_dir, LPCSTR filename, LPCSTR ext, LPCSTR output_dir, crypt_memory_proc crypt_memory)
	{
		if(g_base_address == NULL) return FALSE;

		// Build the input file's path
		CHAR input_path[MAX_PATH];
		strcpy_s(input_path, input_dir);
		strcat_s(input_path, filename);
		// Build the output file's path
		CHAR output_path[MAX_PATH];
		memset(output_path, 0, sizeof(output_path));
		strcpy_s(output_path, output_dir);
		strcat_s(output_path, filename);
		strcat_s(output_path, ext);

		HANDLE input_file = CreateFile(input_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE output_file = CreateFile(output_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		//DebugBreak();
#if _DEBUG
		g_log_writer.Write("\treading file...");
#endif
		DWORD lpNumberOfBytesRead;
		BOOL result = ReadFile(input_file, g_base_address, GetFileSize(input_file, NULL), &lpNumberOfBytesRead, NULL);
#if _DEBUG
		g_log_writer.Write("finished\n");
#endif

		if(result != FALSE)
		{
#if _DEBUG
			g_log_writer.Write("\tcrypting memory...");
#endif
			crypt_memory(g_base_address, lpNumberOfBytesRead);
#if _DEBUG
			g_log_writer.Write("finished\n");
#endif
			result = WriteFile(output_file, g_base_address, lpNumberOfBytesRead, &lpNumberOfBytesRead, NULL);

			if(result == FALSE)
				g_log_writer.Write("\tfailed to write bytes\n");
		}
		else g_log_writer.Write("\tfailed to read bytes\n");

		CloseHandle(input_file);
		CloseHandle(output_file);

		return result;
	}

	void c_era_interface::CryptDirectory(LPCSTR input_dir, LPCSTR output_dir, LPCSTR ext, crypt_file_proc crypt_file)
	{
		// Build the directory's file search mask
		CHAR path[MAX_PATH];
		memset(path, 0, sizeof(path));
		strcpy_s(path, input_dir);
		strcat_s(path, "*");
		strcat_s(path, ext);

		WIN32_FIND_DATA wfd;
		// Start the find and check for failure.
		HANDLE hFind = FindFirstFile(path, &wfd );

		if( INVALID_HANDLE_VALUE == hFind )
			g_log_writer.Write( "FindFirstFile failed.\n" );
		else
		{
			do
			{
				g_log_writer.Write("%s:\n", wfd.cFileName);
				BOOL result = crypt_file(input_dir, wfd.cFileName, output_dir);
				g_log_writer.Write("\t%s\n", 
					result != FALSE ? "SUCCESS" : "FAILED");

			} while( FindNextFile( hFind, &wfd ) );

			FindClose( hFind );
		}
	}


	BOOL c_era_interface::DecryptEraFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir)
	{
		return CryptFile(input_dir, filename, k_decrypted_era_ext, output_dir, c_era_cryptor::DecryptMemory);
	}
	BOOL c_era_interface::DecryptCampaignSaveFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir)
	{
		return CryptFile(input_dir, filename, k_decrypted_save_ext, output_dir, c_campaign_save_cryptor::DecryptMemory);
	}
	void c_era_interface::DecryptInputDirectory()
	{
		CryptDirectory(k_input_dir, k_output_dir, k_encrypted_era_ext, DecryptEraFile);
		CryptDirectory(k_input_dir, k_output_dir, k_encrypted_save_ext, DecryptCampaignSaveFile);
	}


	BOOL c_era_interface::EncryptEraFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir)
	{
		return CryptFile(input_dir, filename, k_encrypted_era_ext, output_dir, c_era_cryptor::EncryptMemory);
	}
	BOOL c_era_interface::EncryptCampaignSaveFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir)
	{
		return CryptFile(input_dir, filename, k_encrypted_save_ext, output_dir, c_campaign_save_cryptor::EncryptMemory);
	}
	void c_era_interface::EncryptOutputDirectory()
	{
		DebugBreak();
		CryptDirectory(k_output_dir, k_input_dir, k_decrypted_era_ext, EncryptEraFile);
		CryptDirectory(k_output_dir, k_input_dir, k_decrypted_save_ext, EncryptCampaignSaveFile);
	}
};