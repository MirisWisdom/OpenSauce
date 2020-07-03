/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"

#if !PLATFORM_IS_DEDI

#include "Gwen/Controls/Pointer.h"
#include "Gwen/inputhandler.h"

using namespace Gwen;
using namespace Gwen::Controls;



GWEN_CONTROL_CONSTRUCTOR( Pointer )
{
	SetMouseInputEnabled( false );
	SetSize( 30, 30);
}

void Pointer::Render( Skin::Base* skin )
{
	skin->DrawPointer(this, Input::IsLeftMouseDown());
}
#endif