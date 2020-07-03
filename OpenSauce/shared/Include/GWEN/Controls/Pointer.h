/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#pragma once

#if !PLATFORM_IS_DEDI

#ifndef GWEN_CONTROLS_POINTER_H
#define GWEN_CONTROLS_POINTER_H

#include "Gwen/Controls/Base.h"
#include "Gwen/Gwen.h"
#include "Gwen/Skin.h"


namespace Gwen
{
	namespace Controls
	{
		class GWEN_EXPORT Pointer : public Controls::Base
		{
			public:

				GWEN_CONTROL( Pointer, Controls::Base );

				virtual void Render( Skin::Base* skin );
		};
	}
}
#endif
#endif