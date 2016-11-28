/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>   An dictionary for passing data to an extraction job in a generic fashion. </summary>
	public class ExtractionData
		: IExtractionData
	{
		private Dictionary<Type, object> mData = new Dictionary<Type, object>();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a data entry cast to the requested type. </summary>
		///
		/// <tparam name="T">	Generic type parameter. </tparam>
		///
		/// <returns>	The requested data. </returns>
		public T Get<T>()
		{
			if (!mData.ContainsKey(typeof(T)))
			{
				return default(T);
			}

			return (T)mData[typeof(T)];
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds data to the object. </summary>
		///
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="data">	The data to add. </param>
		public void Set(object data)
		{
			if (!(data is ICloneable))
			{
				throw new ArgumentException("Cannot add extraction data that is not clonable");
			}

			mData[data.GetType()] = data;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds data to the object with a custom type key. </summary>
		///
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="data">	The data to add. </param>
		/// <param name="key"> 	The type key to use. </param>
		public void Set(object data, Type key)
		{
			if(!key.IsAssignableFrom(data.GetType()))
			{
				throw new ArgumentException("Cannot set exception data with a non matching key type");
			}

			if (!(data is ICloneable))
			{
				throw new ArgumentException("Cannot add extraction data that is not clonable");
			}

			mData[key] = data;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes a deep copy of this object. </summary>
		///
		/// <returns>	A copy of this object. </returns>
		public object Clone()
		{
			var clonedData = new ExtractionData();

			foreach (var entry in mData)
			{
				var entryClone = (entry.Value as ICloneable).Clone();

				clonedData.Set(entryClone, entry.Key);
			}

			return clonedData;
		}
	}
}
