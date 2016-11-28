/*
	OpenSauce: HaloWars Data Interop

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Phoenix
{
	class c_era_interface
	{
		static const char* k_input_dir;
		static const char* k_output_dir;

		static const char* k_era_ext;
		static const char* k_decrypted_era_ext;
		static const char* k_encrypted_era_ext;

		static const char* k_save_ext;
		static const char* k_decrypted_save_ext;
		static const char* k_encrypted_save_ext;

		static void* g_base_address;

		typedef void (* crypt_memory_proc)(void* buffer, size_t buffer_size);
		typedef BOOL (* crypt_file_proc)(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir);

		static BOOL CryptFile(LPCSTR input_dir, LPCSTR filename, LPCSTR ext, LPCSTR output_dir, crypt_memory_proc decrypt_memory);

		static void CryptDirectory(LPCSTR input_dir, LPCSTR output_dir, LPCSTR ext, crypt_file_proc crypt_file);
	public:
		static void Initialize();
		static void Dispose();

		static BOOL DecryptEraFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir);
		static BOOL DecryptCampaignSaveFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir);
		static void DecryptInputDirectory();

		static BOOL EncryptEraFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir);
		static BOOL EncryptCampaignSaveFile(LPCSTR input_dir, LPCSTR filename, LPCSTR output_dir);
		static void EncryptOutputDirectory();
	};
};