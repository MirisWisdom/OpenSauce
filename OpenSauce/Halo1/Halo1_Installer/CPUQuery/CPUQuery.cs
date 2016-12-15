/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using CPUQuery.QueryFunctions;

namespace CPUQuery
{
	public class CPUQueryCustomAction
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	An argument container. </summary>
		private class Argument
		{
			public string Name;
			public string Value;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Queries a property about the user's CPU. </summary>
		///
		/// <param name="actionData"> 	Information describing the action. </param>
		/// <param name="setProperty">	The set property callback. </param>
		/// <param name="onLog">	  	The logging callback. </param>
		/// <param name="onError">	  	The on error callback. </param>
		public static void CPUQuery(string actionData
			, Action<string, string> setProperty
			, Action<string> onLog
			, Action<string> onError)
		{
			if ((setProperty == null) || (onLog == null) || (onError == null))
			{
				throw new ArgumentNullException("One or more required callbacks have not been supplied");
			}

			onLog("Begin CPUQuery");
			onLog(String.Format("Action data: {0}", actionData));

			// Check the custom action arguments have been set
			if (String.IsNullOrEmpty(actionData))
			{
				onError("CPUQuery: ERROR : CustomActionData not set");
				return;
			}

			// Split the input string
			var actionDataTokens = actionData.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
			if (actionDataTokens.Length != 2)
			{
				onError("CPUQuery: ERROR : Argument count must be 2, with an 'OutputProp' argument and a query argument");
				return;
			}

			// Delegate for splitting the arguments
			Func<string, Argument> splitArgument =
				(token) =>
				{
					var argumentTokens = token.Split(new char[] { '=' }, StringSplitOptions.RemoveEmptyEntries);

					if ((argumentTokens.Length > 2) || (argumentTokens.Length == 0))
					{
						onError("CPUQuery: ERROR : Arguments must be either a single string or two strings delimited by an equals");
						return null;
					}

					return new Argument() { Name = argumentTokens[0], Value = (argumentTokens.Length > 1 ? argumentTokens[1] : null) };
				};

			// Parse the output property argument
			var outputPropertyToken = actionDataTokens.First(token => token.StartsWith("OutputProp"));
			if (outputPropertyToken == null)
			{
				onError("CPUQuery: ERROR : No 'OutputProp' argument defined");
				return;
			}
			var outputPropertyArg = splitArgument(outputPropertyToken);
			if (outputPropertyArg == null)
			{
				onError("CPUQuery: ERROR : Invalid argument format used for 'OutputProp");
				return;
			}
			onLog(String.Format("Output property token: {0}", outputPropertyToken));

			// Parse the query type
			var queryTypeToken = actionDataTokens.First(token => !token.StartsWith("OutputProp"));
			if (queryTypeToken == null)
			{
				onError("CPUQuery: ERROR : No query type defined");
				return;
			}
			var queryTypeArg = splitArgument(queryTypeToken);
			if (queryTypeArg == null)
			{
				onError("CPUQuery: ERROR : Invalid argument format used for query type");
				return;
			}
			onLog(String.Format("Query type token: {0}", queryTypeToken));

			// Execute the query
			try
			{
				onLog(String.Format("Executing query {0} with {1}", queryTypeArg.Name, queryTypeArg.Value));

				var result = ExecuteQuery(queryTypeArg.Name, queryTypeArg.Value);

				onLog(String.Format("Query result: {0}", result));

				if (String.IsNullOrEmpty(outputPropertyArg.Value))
				{
					onError("CPUQuery: ERROR : No output property name defined");
					return;
				}

				setProperty(outputPropertyArg.Value, result);

				onLog(String.Format("Property {0} set to {1}", outputPropertyArg.Value, result));
			}
			catch (Exception exception)
			{
				onError(String.Format("CPUQuery: ERROR : Failed to execute a CPU query : ActionData - {0}, Exception - {1}", actionData, exception.Message));
				return;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Delegate type that query functions must use. </summary>
		///
		/// <param name="queryArgs">	The query arguments. </param>
		///
		/// <returns>	A string containing the result. </returns>
		public delegate string QueryFunc(string queryArgs);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the query operation. </summary>
		///
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="queryType">	Type of the query. </param>
		/// <param name="queryArgs">	The query arguments. </param>
		///
		/// <returns>	A string containing the result. </returns>
		private static string ExecuteQuery(string queryType, string queryArgs)
		{
			Dictionary<string, QueryFunc> queryFunctions = new Dictionary<string, QueryFunc>()
			{
				{ "IsProcessorFeaturePresent", IsProcessorFeaturePresentQuery.Execute }
			};

			if (!queryFunctions.ContainsKey(queryType))
			{
				throw new ArgumentException("Unknown query type");
			}

			return queryFunctions[queryType](queryArgs).ToLower();
		}
	}
}
