/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma region

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		/// <summary>	The OpenSauceUI screen interface. </summary>
		class i_screen
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the root control of the screen. </summary>
			///
			/// <returns>	The root control of the screen. </returns>
			virtual t_control_ptr GetRootControl() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a control based on it's resource id. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			///
			/// <returns>	The found control or nullptr if not found. </returns>
			virtual t_control_ptr GetControl(const uint32 resource_id) = 0;
		};

		/// <summary>	Defines an alias representing a shared screen pointer. </summary>
		typedef std::shared_ptr<i_screen> t_screen_ptr;
	};};
};
#endif