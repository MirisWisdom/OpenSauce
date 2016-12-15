/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once
__MCPP_CODE_START__

namespace LowLevel { namespace Cryptography
{
	using namespace System;

	public mcpp_class XXTEA abstract sealed
	{
	mcpp_public
		static mcpp_bool Encrypt256(array<Byte>^ data, int key_1, int key_2, int key_3, int key_4);

		static mcpp_bool Decrypt256(array<Byte>^ data, int key_1, int key_2, int key_3, int key_4);
	};
};};

__MCPP_CODE_END__