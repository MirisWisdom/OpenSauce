/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Configuration
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A singleton wrapper around a configuration container. </summary>
		///
		/// <typeparam name="ContainerType">	Type of the container. </typeparam>
		/// <typeparam name="SingletonType">	Type of the singleton. </typeparam>
		template<typename ContainerType, typename SingletonType>
		class c_configuration_singleton
			abstract
		{
			static_assert(std::is_convertible<ContainerType, i_configuration_container>::value, "c_configuration_singleton can only be used with types that derive from i_configuration_container");

#pragma region Static
		private:
			static std::unique_ptr<SingletonType> g_instance;
			
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the static instance of the singleton. </summary>
			///
			/// <returns>	The singleton instance </returns>
			static SingletonType& Instance()
			{
				if(!g_instance)
				{
					g_instance = std::make_unique<SingletonType>();
				}

				return *g_instance;
			}
#pragma endregion

#pragma region Instance
		protected:
			std::unique_ptr<ContainerType> m_container;

			c_configuration_singleton()
			{
				m_container = std::make_unique<ContainerType>();
			}

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the container as a referece. </summary>
			///
			/// <returns>	The container as a reference </returns>
			ContainerType& Get() const
			{
				return *m_container;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a pointer to the container. </summary>
			///
			/// <returns>	The pointer to the wrapped container. </returns>
			ContainerType* GetPtr() const
			{
				return m_container.get();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Member dereference operator. </summary>
			///
			/// <returns>	The dereferenced object. </returns>
			ContainerType* operator->() const
			{
				return GetPtr();
			}
#pragma endregion
		};
		
		template<typename ContainerType, typename SingletonType>
		std::unique_ptr<SingletonType> c_configuration_singleton<ContainerType, SingletonType>::g_instance;
	};
};