/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"

#if !PLATFORM_IS_DEDI

#include "Gwen/Controls/RadioButton.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( RadioButton )
{
	SetSize( 15, 15 );
	SetMouseInputEnabled( true );
	SetTabable( false );
}

void RadioButton::Render( Skin::Base* skin )
{
	skin->DrawRadioButton( this, IsChecked(), IsDepressed() );
}
#endif