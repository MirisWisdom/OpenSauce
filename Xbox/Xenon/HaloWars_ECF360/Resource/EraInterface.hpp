/*
    OpenSauce: HaloWars Data Interop

    Copyright (C)  Kornner Studios (http://kornner.com)

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