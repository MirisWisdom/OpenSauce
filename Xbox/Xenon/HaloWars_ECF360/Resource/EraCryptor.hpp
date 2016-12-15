/*
	OpenSauce: HaloWars Data Interop

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Phoenix
{
	class c_era_cryptor
	{
	public:
		static void DecryptMemory(void* buffer, size_t buffer_size);
		static void EncryptMemory(void* buffer, size_t buffer_size);
	};

	class c_campaign_save_cryptor
	{
	public:
		static void DecryptMemory(void* buffer, size_t buffer_size);
		static void EncryptMemory(void* buffer, size_t buffer_size);
	};
};