/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

// Official branch build identifier this codebase is based on
#define K_OPENSAUCE_VERSION_BUILD_MAJ	4
#define K_OPENSAUCE_VERSION_BUILD_MIN	0
#define K_OPENSAUCE_VERSION_BUILD		0
#define K_OPENSAUCE_VERSION				\
	BOOST_PP_CAT(K_OPENSAUCE_VERSION_BUILD_MAJ, BOOST_PP_CAT(., K_OPENSAUCE_VERSION_BUILD_MIN))

// MAJ.MIN.BUILD.ce_build_num
#define K_OPENSAUCE_VERSION_HCE_BUILD_NUMBER(ce_build_num) \
	"04.00.00." BOOST_PP_STRINGIZE(ce_build_num)
#define K_OPENSAUCE_BUILD_DATE_STR BOOST_PP_STRINGIZE(OPENSAUCE_BUILD_DATE)
