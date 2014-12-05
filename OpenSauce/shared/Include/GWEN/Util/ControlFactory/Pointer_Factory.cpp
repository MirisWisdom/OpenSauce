/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/
#include "Common/Precompile.hpp"

#include "Gwen/Util/ControlFactory/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
	namespace ControlFactory
	{

		using namespace Gwen;

		namespace Properties
		{

			class PointerColor : public ControlFactory::Property
			{
					GWEN_CONTROL_FACTORY_PROPERTY( Color, "Pointer's color" );

					UnicodeString GetValue( Controls::Base* ctrl )
					{
						Controls::Pointer* pPointer = gwen_cast<Controls::Pointer> ( ctrl );
						return Utility::Format( L"%i %i %i %i", pPointer->GetColor().r, pPointer->GetColor().g, pPointer->GetColor().b, pPointer->GetColor().a );
					}

					void SetValue( Controls::Base* ctrl, const UnicodeString & str )
					{
						Controls::Pointer* pPointer = gwen_cast<Controls::Pointer> ( ctrl );
						int r, g, b, a;

						if ( swscanf( str.c_str(), L"%i %i %i %i", &r, &g, &b, &a ) != 4 ) { return; }

						pPointer->SetColor( Gwen::Color( r, g, b, a ) );
					}

					int	NumCount() { return 4; };

					Gwen::String NumName( int i )
					{
						if ( i == 0 ) { return "r"; }

						if ( i == 1 ) { return "g"; }

						if ( i == 2 ) { return "b"; }

						return "a";
					}

					float NumGet( Controls::Base* ctrl, int i )
					{
						Controls::Pointer* pPointer = gwen_cast<Controls::Pointer> ( ctrl );

						if ( i == 0 ) { return pPointer->GetColor().r; }

						if ( i == 1 ) { return pPointer->GetColor().g; }

						if ( i == 2 ) { return pPointer->GetColor().b; }

						return pPointer->GetColor().a;
					}

					void NumSet( Controls::Base* ctrl, int i, float f )
					{
						Controls::Pointer* pPointer = gwen_cast<Controls::Pointer> ( ctrl );
						Gwen::Color c = pPointer->GetColor();

						if ( i == 0 ) { c.r = f; }

						if ( i == 1 ) { c.g = f; }

						if ( i == 2 ) { c.b = f; }

						if ( i == 3 ) { c.a = f; }

						pPointer->SetColor( c );
					}

			};

		}


		class Pointer_Factory : public Gwen::ControlFactory::Base
		{
			public:

				GWEN_CONTROL_FACTORY_CONSTRUCTOR( Pointer_Factory, ControlFactory::Base )
				{
					AddProperty( new Properties::PointerColor() );
				}

				virtual Gwen::String Name() { return "Pointer"; }
				virtual Gwen::String BaseName() { return "Base"; }

				virtual Gwen::Controls::Base* CreateInstance( Gwen::Controls::Base* parent )
				{
					Gwen::Controls::Pointer* pControl = new Gwen::Controls::Pointer( parent );
					pControl->SetSize( 20, 20 );
					pControl->SetColor( Gwen::Color( 255, 255, 255, 255 ));
					return pControl;
				}
		};

		GWEN_CONTROL_FACTORY( Pointer_Factory );

	}
}
