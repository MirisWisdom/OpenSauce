/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Screen/i_screen.hpp"
#include "Interface/OpenSauceUI/Screen/i_screen_controller.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	Base screen controller. </summary>
		class c_screen_controller_base
			abstract
			: public i_screen_controller
		{
		protected:
			enum screen_anchor
			{
				_screen_anchor_top		= 1 << 0,
				_screen_anchor_center_v = 1 << 1,
				_screen_anchor_bottom	= 1 << 2,
				_screen_anchor_left		= 1 << 3,
				_screen_anchor_center_h = 1 << 4,
				_screen_anchor_right	= 1 << 5
			};

		private:
			/// <summary>	Defines an alias representing a dynamic property update function. </summary>
			typedef std::function<void(Control::i_control&, Control::i_property_interface&)> dynamic_property_update_t;

			struct s_dynamic_property
			{
				Control::i_control& m_control;
				Control::i_property_interface& m_property;
				const dynamic_property_update_t m_update;
			};

			std::vector<s_dynamic_property> m_dynamic_properties;

		protected:
			screen_ptr_t m_target_screen;
			Definitions::c_screen_definition m_screen_definition;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a control's anchor point. </summary>
			///
			/// <param name="control">	[in] The control. </param>
			/// <param name="anchor"> 	The anchor point. </param>
			///
			/// <returns>	The control's anchor point. </returns>
			point2d GetControlAnchorPoint(Control::i_control& control, const screen_anchor anchor);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Anchors the screen to a point on it's parent. </summary>
			///
			/// <param name="parent_anchor">	The parents anchor point. </param>
			/// <param name="child_anchor"> 	The childs anchor point. </param>
			void AnchorScreen(const screen_anchor parent_anchor, const screen_anchor child_anchor);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="definition">	[in] The screen definition. </param>
			c_screen_controller_base(Definitions::c_screen_definition& definition);
			
#pragma region i_screen_controller
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds the screen and adds it to the canvas. </summary>
			///
			/// <param name="control_factory">	  	[in] The control factory. </param>
			/// <param name="target_canvas">	  	[in] Target canvas. </param>
			void BuildScreen(ControlFactory::c_control_factory& control_factory
				, Control::i_canvas& target_canvas) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the screen from the canvas and destroys it. </summary>
			///
			/// <param name="target_canvas">	[in] Target canvas. </param>
			void DestroyScreen(Control::i_canvas& target_canvas) final override;

			/// <summary>	Updates the screen. </summary>
			void Update() final override;

		private:
			/// <summary>	Sets a screens static properties. </summary>
			void SetStaticProperties() override { };

			/// <summary>	Binds a screens dynamic properties. </summary>
			void BindDynamicProperties() override { };

			/// <summary>	Binds a screens events. </summary>
			void BindEvents() override { };

			/// <summary>	Unbinds a screens events. </summary>
			void UnbindEvents() override { };

			/// <summary>	Unbinds a screens dynamic properties. </summary>
			void UnbindDynamicProperties() final override;
#pragma endregion

#pragma region Property Setup
		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a control. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="control_out">	[out] The found control. </param>
			void FindControl(const uint32 control_id, Control::control_ptr_t& control_out);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a control and property. </summary>
			///
			/// <param name="control_id">  	Identifier for the control. </param>
			/// <param name="property_id"> 	Identifier for the property. </param>
			/// <param name="control_out"> 	[out] The found control. </param>
			/// <param name="property_out">	[out] The found property. </param>
			void FindControlAndProperty(const uint32 control_id
				, const uint32 property_id
				, Control::control_ptr_t& control_out
				, Control::i_property_interface*& property_out);

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Set control property implementation. </summary>
			///
			/// <typeparam name="Type">	Type of the property. </typeparam>
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="property_id">	Identifier for the property. </param>
			/// <param name="value">	  	The value to set. </param>
			template<typename Type>
			void SetControlPropertyImpl(const uint32 control_id, const uint32 property_id, Type value);

		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a bool control property. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="property_id">	Identifier for the property. </param>
			/// <param name="value">	  	true to value. </param>
			void SetControlProperty(const uint32 control_id, const uint32 property_id, const bool value);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets an int32 control property. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="property_id">	Identifier for the property. </param>
			/// <param name="value">	  	The value. </param>
			void SetControlProperty(const uint32 control_id, const uint32 property_id, const int32 value);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a string control property. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="property_id">	Identifier for the property. </param>
			/// <param name="value">	  	The value. </param>
			void SetControlProperty(const uint32 control_id, const uint32 property_id, const real value);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a string control property. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="property_id">	Identifier for the property. </param>
			/// <param name="value">	  	The value. </param>
			void SetControlProperty(const uint32 control_id, const uint32 property_id, cstring value);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a dynamic property. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="property_id">	Identifier for the property. </param>
			/// <param name="update">	  	The update function. </param>
			void AddDynamicProperty(const uint32 control_id, const uint32 property_id, const dynamic_property_update_t& update);
#pragma endregion

#pragma region Event Setup
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attaches an event callback to a control. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="event_id">   	Identifier for the event. </param>
			/// <param name="callback_id">	Identifier for the callback. </param>
			/// <param name="userdata">   	[in] The userdata for the callback. </param>
			/// <param name="function">   	The callback function. </param>
			void AttachEvent(const uint32 control_id, const uint32 event_id, const uint32 callback_id, void* userdata, Control::event_callback_t function);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Detach event. </summary>
			///
			/// <param name="control_id"> 	Identifier for the control. </param>
			/// <param name="event_id">   	Identifier for the event. </param>
			/// <param name="callback_id">	Identifier for the callback. </param>
			void DetachEvent(const uint32 control_id, const uint32 event_id, const uint32 callback_id);
#pragma endregion

#pragma region i_visibility_toggle
		public:
			/// <summary>	Shows the screen. </summary>
			void Show() override;

			/// <summary>	Hides the screen. </summary>
			void Hide() override;
#pragma endregion
		};
	};};};
};
#endif