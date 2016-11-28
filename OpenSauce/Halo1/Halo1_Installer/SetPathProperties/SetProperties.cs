/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;

namespace SetProperties
{
	public class SetPropertiesCustomAction
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets properties using the specified definition. </summary>
		/// 
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		///
		/// <param name="definition">   	The definition to use. </param>
		/// <param name="onLog">			The log callback. </param>
		/// <param name="onSetProperty">	The set property callback. </param>
		/// <param name="onGetProperty">	The get property callback. </param>
		/// <param name="onError">			The error callback. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public static bool SetProperties(string definition,
			Action<string> onLog,
			Action<string, string> onSetProperty,
			Func<string, string> onGetProperty,
			Action<string> onError)
		{
			if ((onLog == null) || (onSetProperty == null) || (onGetProperty == null) || (onError == null))
			{
				throw new ArgumentNullException("A required callback delegate was not supplied");
			}

			onLog("Begin SetProperties");

			// Check the required property has been set
			if (String.IsNullOrEmpty(definition))
			{
				onError("InstallValidator: ERROR : SETPROPERTIESDEF not set. Inform the developers of the problem.");
				return false;
			}

			// Log the properties
			onLog("LOG: SETPROPERTIESDEF : " + definition);

			var propertySetter = new PropertySetter.PropertySetter();

			propertySetter.ErrorOccurred +=
				(sender, args) =>
				{
					onError(args.Message);
				};

			if (!propertySetter.SetProperties(definition, onSetProperty, onGetProperty))
			{
				onSetProperty("SETPROPERTIESRESULT", "FAIL");
			}
			else
			{
				onSetProperty("SETPROPERTIESRESULT", "PASS");
			}
			return true;
		}
	}
}
