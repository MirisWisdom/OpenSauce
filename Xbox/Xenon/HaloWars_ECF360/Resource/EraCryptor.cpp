/*
	OpenSauce: HaloWars Data Interop

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Precompile.hpp"
#include "Resource/EraCryptor.hpp"

namespace Phoenix
{
	struct s_ensemble_cryptor_context
	{
		boost::int64_t data4;	// 0x0
		boost::int64_t data5;	// 0x8
		boost::int64_t data6;	// 0x10
	};

#include "EraCryptor.Dec.inl"
#include "EraCryptor.Enc.inl"

	static const s_ensemble_cryptor_context k_ecf_cryptor_context = {
		0xBC3EB6B4D0471DDB,
		0x8299E6431912BE73,
		0x4601515D43D26DF5,
	};
	void c_era_cryptor::DecryptMemory(void* buffer, size_t buffer_size)
	{
		EraDecypt(&k_ecf_cryptor_context, buffer, buffer, buffer_size, 0);
	}
	void c_era_cryptor::EncryptMemory(void* buffer, size_t buffer_size)
	{
		EraEncypt(&k_ecf_cryptor_context, buffer, buffer, buffer_size, 0);
	}


	static const s_ensemble_cryptor_context k_campaign_save_cryptor_context = {
		0x194F8D77DF360283,
		0x1385AC1E2122F575,
		0xA7392D249DC2C737,
	};
	void c_campaign_save_cryptor::DecryptMemory(void* buffer, size_t buffer_size)
	{
		EraDecypt(&k_campaign_save_cryptor_context, buffer, buffer, buffer_size, 0);
	}
	void c_campaign_save_cryptor::EncryptMemory(void* buffer, size_t buffer_size)
	{
		EraEncypt(&k_campaign_save_cryptor_context, buffer, buffer, buffer_size, 0);
	}
};