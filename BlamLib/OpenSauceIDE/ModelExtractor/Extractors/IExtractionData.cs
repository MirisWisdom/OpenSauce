/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>	Interface for extraction data. </summary>
	public interface IExtractionData
		: ICloneable
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a data entry cast to the requested type. </summary>
		///
		/// <tparam name="T">	The type of data to get. </tparam>
		///
		/// <returns>	The requested data. </returns>
		T Get<T>();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds data to the object. </summary>
		///
		/// <param name="data">	The data to add. </param>
		void Set(object data);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds data to the object. </summary>
		///
		/// <param name="data">	The data to add. </param>
		/// <param name="key"> 	The type key to use. </param>
		void Set(object data, Type key);
	}
}
