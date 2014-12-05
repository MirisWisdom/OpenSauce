/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"


#include "Gwen/Controls/Pointer.h"

using namespace Gwen;
using namespace Gwen::Controls;



GWEN_CONTROL_CONSTRUCTOR( Pointer )
{
	SetMouseInputEnabled( false );
	SetSize( 15, 15 );
}

void Pointer::Render( Skin::Base* skin )
{
	skin->GetRender()->SetDrawColor( m_Color );

	Gwen::Rect rect = GetRenderBounds();
	skin->GetRender()->DrawFilledRect( rect );
}