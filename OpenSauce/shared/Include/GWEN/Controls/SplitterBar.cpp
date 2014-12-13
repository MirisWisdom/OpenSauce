/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"

#include "Gwen/Gwen.h"
#include "Gwen/Controls/SplitterBar.h"

using namespace Gwen;
using namespace Controls;

GWEN_CONTROL_CONSTRUCTOR( SplitterBar )
{
	SetTarget( this );
	RestrictToParent( true );
}

void SplitterBar::Layout( Skin::Base* /*skin*/ )
{
	MoveTo( X(), Y() );
}