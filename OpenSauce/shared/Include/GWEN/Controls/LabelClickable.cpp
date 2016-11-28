/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"

#if !PLATFORM_IS_DEDI

#include "Gwen/Gwen.h"
#include "Gwen/Skin.h"
#include "Gwen/Controls/LabelClickable.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( LabelClickable )
{
	SetIsToggle( false );
	SetCursor( Gwen::CursorType::Finger );
	SetAlignment( Gwen::Pos::Left | Gwen::Pos::CenterV );
}

void LabelClickable::Render( Skin::Base* /*skin*/ )
{
	//skin->DrawButton( this, IsDepressed(), IsToggle() && GetToggleState() );
}
#endif