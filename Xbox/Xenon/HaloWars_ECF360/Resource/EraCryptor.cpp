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


	static const s_ensemble_cryptor_context k_campaign_save_cryptor_context = { // TODO
		0,
		0,
		0,
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