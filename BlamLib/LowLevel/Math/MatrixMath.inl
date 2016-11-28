/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

__CPP_CODE_START__

#pragma region matrix4x3_identity
namespace CppMath {
	void matrix4x3_identity(real_matrix4x3* mat)
	{
		static const real_matrix4x3 identity = {
			1.0f,
			{1.0f},
			{0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f}
		};

		memcpy_s(mat, sizeof(*mat), &identity, 1);
	}
};
#pragma endregion

#pragma region matrix4x3_transpose
#include <algorithm>

namespace CppMath {
	void matrix4x3_transpose(real_matrix4x3* mat)
	{
		std::swap(mat->Forward.J, mat->Left.I);
		std::swap(mat->Up.I, mat->Forward.K);
		std::swap(mat->Left.K, mat->Up.J);
	}
};
#pragma endregion


#pragma region matrix4x3_scale
namespace CppMath {
	void matrix4x3_scale(real_matrix4x3* mat, const float scale)
	{
		matrix4x3_identity(mat);
		mat->Scale = scale;
	}
};
#pragma endregion

__CPP_CODE_END__

//////////////////////////////////////////////////////////////////////////

__MCPP_CODE_START__
namespace LowLevel { namespace Math {

	real_matrix4x3::real_matrix4x3(mcpp_real scale)
	{
		pin_ptr<real_matrix4x3> _this = this;
		{
			CppMath::matrix4x3_scale(
				reinterpret_cast<CppMath::real_matrix4x3*>(_this),
				scale );
		}
	}

	void real_matrix4x3::ToIdentity()
	{
		pin_ptr<real_matrix4x3> _this = this;
		{
			CppMath::matrix4x3_identity(
				reinterpret_cast<CppMath::real_matrix4x3*>(_this) );
		}
	}

};};
__MCPP_CODE_END__