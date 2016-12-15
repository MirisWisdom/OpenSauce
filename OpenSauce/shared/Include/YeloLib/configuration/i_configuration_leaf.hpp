/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_leaf_iterator.hpp>

namespace Yelo
{
	namespace Configuration
	{
		class i_configuration_leaf_iterator;

		/// <summary>	The configuration leaf interface. Derived types must provide get and set values for all base variable types. </summary>
		class i_configuration_leaf
			abstract
		{
		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a bool. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual bool		GetValue(const bool default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a signed byte. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual sbyte		GetValue(const sbyte default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a signed short. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual int16		GetValue(const int16 default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a signed int. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual int32		GetValue(const int32 default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as an unsigned byte. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual byte		GetValue(const byte default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as an unsigned short. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual uint16		GetValue(const uint16 default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as an unsigned int. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual uint32		GetValue(const uint32 default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a float. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual real		GetValue(const real default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a double. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual double		GetValue(const double default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the leaf's value as a string. </summary>
			///
			/// <param name="default_value">	The default value to use if not present. </param>
			///
			/// <returns>	The leaf's value. </returns>
			virtual std::string	GetValue(const std::string& default_value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a bool. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const bool value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a signed byte. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const sbyte value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a signed short. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const int16 value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a signed int. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const int32 value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as an unsigned byte. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const byte value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as an unsigned short. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const uint16 value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as an unsigned int. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const uint32 value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a float. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const real value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a double. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const double value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the nodes value as a string. </summary>
			///
			/// <param name="value">	The value to use. </param>
			virtual void		SetValue(const std::string& value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Query if the leaf has a child matching the supplied name. </summary>
			///
			/// <param name="child_name">	Name of the child to find. </param>
			///
			/// <returns>	true if the leaf has a matching child, otherwise false. </returns>
			virtual bool		HasChild(const std::string& child_name) const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a child iterator that iterates through children that match the supplied name. </summary>
			///
			/// <param name="child_name">	Name of the children to iterate. </param>
			///
			/// <returns>	A child iterator pointer. </returns>
			virtual std::unique_ptr<i_configuration_leaf_iterator> GetChildIterator(const std::string& child_name) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the first instance of a child with the supplied name. </summary>
			///
			/// <param name="child_name">	Name of the child to find. </param>
			///
			/// <returns>	A leaf object representing the found child, otherwise returns an empty pointer. </returns>
			virtual std::unique_ptr<i_configuration_leaf> GetChild(const std::string& child_name) const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a child with the specified name to the leaf. </summary>
			///
			/// <param name="child_name">	Name of the child to add. </param>
			///
			/// <returns>	A leaf object representing the added child. </returns>
			virtual std::unique_ptr<i_configuration_leaf> AddChild(const std::string& child_name) = 0;
		};
	};
};