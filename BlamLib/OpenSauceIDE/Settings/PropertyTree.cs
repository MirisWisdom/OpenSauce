/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace OpenSauceIDE.Settings
{
	#region Exceptions
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Base exception for signalling property tree errors. </summary>
	///-------------------------------------------------------------------------------------------------
	public class PropertyTreeException : Exception
	{ }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Exception for signalling property tree XML errors. </summary>
	///-------------------------------------------------------------------------------------------------
	public class PropertyTreeXmlException : PropertyTreeException
	{ }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Exception for signalling property tree invalid path errors. </summary>
	///-------------------------------------------------------------------------------------------------
	public class PropertyTreeInvalidPathException : PropertyTreeException
	{ }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Exception for signalling property tree missing node errors. </summary>
	///-------------------------------------------------------------------------------------------------
	public class PropertyTreeMissingNodeException : PropertyTreeException
	{ }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Exception for signalling property tree data conversion errors. </summary>
	///-------------------------------------------------------------------------------------------------
	public class PropertyTreeDataConversionException : PropertyTreeException
	{ }
	#endregion

	///-------------------------------------------------------------------------------------------------
	/// <summary>	A property node used for generic storage of values in a tree hierarchy. </summary>
	///-------------------------------------------------------------------------------------------------
	[XmlRoot(ElementName="PropertyNode")]
	public class PropertyNode
	{
		#region Fields
		[XmlAttribute(AttributeName = "Name")]
		public string Name;

		[XmlElement(ElementName = "Value")]
		public string Value;

		[XmlElement(ElementName="PropertyNode")]
		public List<PropertyNode> Children;
		#endregion

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initial value constructor. </summary>
		/// <param name="Value">	The nodes initial value. </param>
		///-------------------------------------------------------------------------------------------------
		public PropertyNode(object Value)
		{
			Children = new List<PropertyNode>();
			SetValue(Value);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		///-------------------------------------------------------------------------------------------------
		public PropertyNode()
			: this(null)
		{}
		#endregion

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Searches for the first child node with a matching name. </summary>
		/// <param name="name">	The name to search for. </param>
		/// <returns>	The found node. </returns>
		///-------------------------------------------------------------------------------------------------
		private PropertyNode FindNode(string name)
		{
			int index = Children.FindIndex(node => node.Name == name);

			return (index == -1 ? null : Children[index]);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the current value converted to the provided type. </summary>
		/// <exception cref="PropertyTreeDataConversionException">
		/// 	Thrown when the current value could not be converted to the requested type.
		/// </exception>
		/// <param name="toType">	The type to convert the current value to. </param>
		/// <returns>	The value converted to the requested type. </returns>
		///-------------------------------------------------------------------------------------------------
		private object GetValueConverted(Type toType)
		{
			if (toType.IsEnum)
			{
				// If the type is an enum attempt to parse the current value
				try
				{
					return Enum.Parse(toType, Value);
				}
				catch
				{
					throw new PropertyTreeDataConversionException();
				}
			}
			else
			{
				// Otherwise try to convert the string to the requested type
				try
				{
					return Convert.ChangeType(Value, toType);
				}
				catch
				{
					throw new PropertyTreeDataConversionException();
				}
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Returns whether a specified child node is present. </summary>
		/// <param name="name">	The name to search for. </param>
		/// <returns>	true if it exists, false if it does not. </returns>
		///-------------------------------------------------------------------------------------------------
		public bool Has(string name)
		{
			return (FindNode(name) != null);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a child property node. </summary>
		/// <param name="name"> 	The name for the node. </param>
		/// <param name="value">	(Optional) The value for the node. </param>
		/// <returns>	The new property node. </returns>
		///-------------------------------------------------------------------------------------------------
		public PropertyNode Put(string name, IConvertible value = null)
		{
			var newNode = new PropertyNode { Name = name };
			newNode.SetValue(value);

			Children.Add(newNode);

			return newNode;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a child property node. </summary>
		/// <remarks>	If the requested node is not found it is created. </remarks>
		/// <param name="name">	The name to search for. </param>
		/// <returns>	A PropertyNode. </returns>
		///-------------------------------------------------------------------------------------------------
		public PropertyNode Get(string name)
		{
			PropertyNode node = FindNode(name);

			if (node == null)
			{
				node = Put(name);
			}

			return node;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets an existing child property node. </summary>
		/// <exception cref="PropertyTreeMissingNodeException">
		/// 	Thrown if the requested node is missing.
		/// </exception>
		/// <param name="name">	The name of the node to get. </param>
		/// <returns>	The requested property node. </returns>
		///-------------------------------------------------------------------------------------------------
		public PropertyNode GetExisting(string name)
		{
			PropertyNode node = FindNode(name);

			if (node == null)
			{
				throw new PropertyTreeMissingNodeException();
			}

			return node;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the next child node with a matching name. </summary>
		/// <exception cref="PropertyTreeMissingNodeException">
		/// 	Thrown when no matching nodes are found. or when the start noded does not exist in the
		/// 	child list.
		/// </exception>
		/// <param name="name"> 	The name of the node to get. </param>
		/// <param name="start">	The node to start the search after. </param>
		/// <returns>
		/// 	The next matching property node after the start node. If no such node exists, returns
		/// 	null.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		public PropertyNode GetNext(string name, PropertyNode start)
		{
			// Find the index to start at
			int startIndex = 0;

			if (start != null)
			{
				// Find the start node in the child list
				startIndex = Children.ToList().FindIndex(
					node =>
					{
						return node == start;
					}
				);

				// Throw a missing node exception if the start node does not exist in the child list
				if (startIndex == -1)
				{
					throw new PropertyTreeMissingNodeException();
				}

				// Move past the starting node
				startIndex += 1;

				// If the start point is the end of the child list return null
				if (startIndex >= Children.Count)
				{
					return null;
				}
			}

			// Get the next node with a matching name
			int nextNode = Children.ToList().FindIndex(startIndex,
				node =>
				{
					return node.Name == name;
				}
			);

			// Return the sibling node, or null if there are no more matching nodes
			if (nextNode == -1)
			{
				return null;
			}
			else
			{
				return Children[nextNode];
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Query if this node has a value. </summary>
		/// <returns>	true if it does, false if it does not. </returns>
		///-------------------------------------------------------------------------------------------------
		public bool HasValue()
		{
			return !String.IsNullOrEmpty(Value);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the nodes value. </summary>
		/// <typeparam name="T">	The type to convert to. Must implement IConvertible. </typeparam>
		/// <returns>	The nodes value cast to the specified type. </returns>
		///-------------------------------------------------------------------------------------------------
		public T GetValue<T>()
			where T : IConvertible
		{
			// Return the value converted to the requested type
			// PropertyTreeDataConversionException is thrown if the conversion fails
			return (T)GetValue(typeof(T));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the nodes value. </summary>
		/// <exception cref="PropertyTreeDataConversionException">
		/// 	Thrown when the object type does not implement IConvertible.
		/// </exception>
		/// <param name="castType">	The type to convert the value to. </param>
		/// <returns>	The nodes value cast to the specified type. </returns>
		///-------------------------------------------------------------------------------------------------
		public object GetValue(Type castType)
		{
			// Check that the object type implements IConvertible
			if (!typeof(IConvertible).IsAssignableFrom(castType))
			{
				throw new PropertyTreeDataConversionException();
			}

			// If the value is not value throw an exception
			if (Value == null)
			{
				throw new PropertyTreeDataConversionException();
			}

			// Return the value converted to the requested type
			// PropertyTreeDataConversionException is thrown if the conversion fails
			return GetValueConverted(castType);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the nodes value. </summary>
		/// <typeparam name="T">	The type to convert to. Must implement IConvertible. </typeparam>
		/// <param name="defaultValue">
		/// 	The default value to return if the current value is null or not valid.
		/// </param>
		/// <returns>	The nodes value cast to the specified type. </returns>
		///-------------------------------------------------------------------------------------------------
		public T GetValue<T>(T defaultValue)
			where T : IConvertible
		{
			// Attempt to return the current value converted to the requested type
			// If the conversion fails the provided default is returned
			return (T)GetValue(typeof(T), defaultValue);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the nodes value. </summary>
		/// <exception cref="PropertyTreeDataConversionException">
		/// 	Thrown when the object type does not implement IConvertible.
		/// </exception>
		/// <param name="castType">	   	The type to convert the value to. </param>
		/// <param name="defaultValue">
		/// 	The default value to return if the current value is null or not valid.
		/// </param>
		/// <returns>	The nodes value cast to the specified type. </returns>
		///-------------------------------------------------------------------------------------------------
		public object GetValue(Type castType, object defaultValue)
		{
			// Check that the object type implements IConvertible
			if (!typeof(IConvertible).IsAssignableFrom(castType))
			{
				throw new PropertyTreeDataConversionException();
			}

			// If the value is not value return the default value
			if (Value == null)
			{
				return defaultValue;
			}

			// Attempt to return the current value converted to the requested type
			// If the conversion fails the provided default is returned
			object returnValue = default(object);

			try
			{
				returnValue = GetValueConverted(castType);
			}
			catch (PropertyTreeDataConversionException)
			{
				returnValue = defaultValue;
			}

			return returnValue;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the nodes value. </summary>
		/// <typeparam name="T">	The type of the new value. Must implement IConvertible. </typeparam>
		/// <param name="value">	The value to use. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetValue<T>(T value)
			where T : IConvertible
		{
			// Convert the provided value into its string representation
			SetValue((object)value);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the nodes value. </summary>
		/// <exception cref="PropertyTreeDataConversionException">
		/// 	Thrown when the object type does not implement IConvertible.
		/// </exception>
		/// <param name="value">	The value to use. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetValue(object value)
		{
			// If the value is null, set the Value to an empty string
			if(value == null)
			{
				Value = null;
				return;
			}

			// Check that the object type implements IConvertible
			if (!typeof(IConvertible).IsAssignableFrom(value.GetType()))
			{
				throw new PropertyTreeDataConversionException();
			}

			// Convert the provided value into its string representation
			Value = (string)Convert.ChangeType(value, typeof(string));
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// 	Property node value enumerator. Enumerates the child nodes of a property node that match
	/// 	the specified name.
	/// </summary>
	/// <typeparam name="T">
	/// 	The type to convert the node values to. Must implement IConvertible.
	/// </typeparam>
	///-------------------------------------------------------------------------------------------------
	public class PropertyNodeValueEnumerator<T> : IEnumerator<T>
		where T : IConvertible
	{
		#region Fields
		private string mNodeName;
		private PropertyNode mSourceNode;
		private PropertyNode mCurrentNode;
		#endregion

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="sourceNode">	Source node. </param>
		/// <param name="nodeName">  	Name of the node to enumerate. </param>
		///-------------------------------------------------------------------------------------------------
		public PropertyNodeValueEnumerator(PropertyNode sourceNode, string nodeName)
		{
			mSourceNode = sourceNode;
			mNodeName = nodeName;
			mCurrentNode = null;
		}
		#endregion

		#region IEnumerator<T> Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the current nodes value. </summary>
		/// <value>	The current nodes value. </value>
		///-------------------------------------------------------------------------------------------------
		public T Current
		{
			get
			{
				if (mCurrentNode == null)
				{
					return default(T);
				}
				return mCurrentNode.GetValue<T>();
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Unused. </summary>
		///-------------------------------------------------------------------------------------------------
		public void Dispose() { }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Moves the enumerator to the next element. </summary>
		/// <returns>	true if it succeeds, false if the end of the list was reached. </returns>
		///-------------------------------------------------------------------------------------------------
		public bool MoveNext()
		{
			mCurrentNode = mSourceNode.GetNext(mNodeName, mCurrentNode);

			return (mCurrentNode != null);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Resets the enumerators current object. </summary>
		///-------------------------------------------------------------------------------------------------
		public void Reset()
		{
			mCurrentNode = null;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the current nodes value. </summary>
		/// <value>	The current nodes value. </value>
		///-------------------------------------------------------------------------------------------------
		object System.Collections.IEnumerator.Current
		{
			get { return Current; }
		}
		#endregion
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// 	A property tree class for storing values that can be accessed using path semantics.
	/// </summary>
	///-------------------------------------------------------------------------------------------------
	public class PropertyTree
	{
		// TODO: CODE DUPLICATION BAD BAD BAD

		#region Fields
		private readonly char mPathDelimiter;
		private PropertyNode mRootNode;
		#endregion

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="pathDelimiter">	(Optional) the path delimiter. Defaults to '\' </param>
		///-------------------------------------------------------------------------------------------------
		public PropertyTree(char pathDelimiter = '\\')
		{
			mPathDelimiter = pathDelimiter;
		}
		#endregion

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Deserializes a property tree from XML. </summary>
		/// <exception cref="PropertyTreeXmlException">
		/// 	Thrown when a Property Tree XML error condition occurs.
		/// </exception>
		/// <param name="path">	Full path to the XML file. </param>
		///-------------------------------------------------------------------------------------------------
		public void ReadXml(string path)
		{
			// Create a new XML reader
			using (var xmlReader = XmlReader.Create(path))
			{
				// If the file can be deserialized, then store it as the root node
				XmlSerializer serializer = new XmlSerializer(typeof(PropertyNode));

				if (!serializer.CanDeserialize(xmlReader))
				{
					throw new PropertyTreeXmlException();
				}

				mRootNode = serializer.Deserialize(xmlReader) as PropertyNode;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Serializes the property tree into an XML file. </summary>
		/// <exception cref="PropertyTreeXmlException">
		/// 	Thrown the property node could not be serialized.
		/// </exception>
		/// <param name="path">	Full path to the XML file. </param>
		///-------------------------------------------------------------------------------------------------
		public void SaveXml(string path)
		{
			// Set the output settings to indent the xml elements
			XmlWriterSettings settings = new XmlWriterSettings();
			settings.Indent = true;

			// Create a new XML writer
			using (var xmlWriter = XmlWriter.Create(path, settings))
			{
				// Serialise the current root node into the document
				XmlSerializer serializer = new XmlSerializer(typeof(PropertyNode));

				try
				{
					serializer.Serialize(xmlWriter, mRootNode);
				}
				catch
				{
					throw new PropertyTreeXmlException();
				}
			}
		}

		private string[] SplitPath(string path)
		{
			// Split the path string and exception if it is not valid
			string[] pathNodes = path.Split(new char[] { mPathDelimiter });

			if (pathNodes.Length == 0)
			{
				throw new PropertyTreeInvalidPathException();
			}

			return pathNodes;
		}

		public PropertyNode Get(string path)
		{
			string[] pathNodes = SplitPath(path);

			// If the current root node is null, create a new node set to the name of the first path directory
			// If the root node is not null the first path directory must match it's name
			if (mRootNode == null)
			{
				mRootNode = new PropertyNode { Name = pathNodes[0] };
			}
			else if (mRootNode.Name != pathNodes[0])
			{
				throw new PropertyTreeMissingNodeException();
			}

			// Iterate through the property tree, following the specified path to the requested value
			// If a node is missing it is created
			var currentNode = mRootNode;
			for (int i = 1; i < pathNodes.Length; i++)
			{
				currentNode = currentNode.Get(pathNodes[i]);
			}

			return currentNode;
		}

		public PropertyNode GetExisting(string path)
		{
			string[] pathNodes = SplitPath(path);

			// If the current root node is null, or the first path directory does not match it's name, throw an exception
			if ((mRootNode == null) || (mRootNode.Name != pathNodes[0]))
			{
				throw new PropertyTreeMissingNodeException();
			}

			// Iterate through the property tree, following the specified path to the requested value
			// If a node is missing an exception is thrown
			var currentNode = mRootNode;
			for (int i = 1; i < pathNodes.Length; i++)
			{
				currentNode = currentNode.GetExisting(pathNodes[i]);
			}

			return currentNode;
		}

		public PropertyNode Put(string path)
		{
			// Split the path string and exception if it is not valid
			string[] pathNodes = SplitPath(path);

			// If the current root node is null, create a new node set to the name of the first path directory
			// If the root node is not null the first path directory must match it's name
			if (mRootNode == null)
			{
				mRootNode = new PropertyNode { Name = pathNodes[0] };
			}
			else if (mRootNode.Name != pathNodes[0])
			{
				throw new PropertyTreeMissingNodeException();
			}

			// Iterate through the property tree, following the specified path to the parent directory
			// Missing nodes are automatically added
			var current = mRootNode;
			for (int i = 1; i < pathNodes.Length - 1; i++)
			{
				current = current.Get(pathNodes[i]);
			}

			return current;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the named property from the tree. </summary>
		/// <exception cref="PropertyTreeInvalidPathException">
		/// 	Thrown when the path is invalid.
		/// </exception>
		/// <exception cref="PropertyTreeMissingNodeException">
		/// 	Thrown when the path root is different from the tree's root, or it there is no root node.
		/// </exception>
		/// <typeparam name="T">	The type to cast the requested value to. </typeparam>
		/// <param name="path">	Full path to the variable. </param>
		/// <returns>	A value from the property tree. </returns>
		///-------------------------------------------------------------------------------------------------
		public T GetValue<T>(string path)
			where T : IConvertible
		{
			string[] pathNodes = SplitPath(path);

			// If the current root node is null, or the first path directory does not match it's name, throw an exception
			if((mRootNode == null) || (mRootNode.Name != pathNodes[0]))
			{
				throw new PropertyTreeMissingNodeException();
			}

			// If there is only once path directory, return the root nodes value
			if (pathNodes.Length == 1)
			{
				return mRootNode.GetValue<T>();
			}

			// Iterate through the property tree, following the specified path to the requested value
			// If a node is missing an exception is thrown
			var currentNode = mRootNode;
			for (int i = 1; i < pathNodes.Length; i++)
			{
				currentNode = currentNode.GetExisting(pathNodes[i]);
			}

			return currentNode.GetValue<T>();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the named property from the tree. If the. </summary>
		/// <exception cref="PropertyTreeInvalidPathException">
		/// 	Thrown when the path is invalid.
		/// </exception>
		/// <exception cref="PropertyTreeMissingNodeException">
		/// 	Thrown when the path root is different from the tree's root.
		/// </exception>
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="path">		   	Full path to tha variable. </param>
		/// <param name="defaultValue">	The default value. </param>
		/// <returns>	A value from the property tree. </returns>
		///-------------------------------------------------------------------------------------------------
		public T GetValue<T>(string path, T defaultValue)
			where T : IConvertible
		{
			string[] pathNodes = SplitPath(path);

			// If the current root node is null, create a new node set to the name of the first path directory
			// If the root node is not null the first path directory must match it's name
			if (mRootNode == null)
			{
				PutValue<string>(pathNodes[0], null);
			}
			else if (mRootNode.Name != pathNodes[0])
			{
				throw new PropertyTreeMissingNodeException();
			}

			// If there is only once path directory, return the root nodes value
			if (pathNodes.Length == 1)
			{
				return mRootNode.GetValue<T>();
			}

			// Iterate through the property tree, following the specified path to the requested value
			var currentNode = mRootNode;
			for (int i = 1; i < pathNodes.Length; i++)
			{
				currentNode = currentNode.Get(pathNodes[i]);
			}

			// If the node does not have a value set, set it to the default value
			if (!currentNode.HasValue())
			{
				currentNode.SetValue<T>(defaultValue);
			}

			return currentNode.GetValue<T>();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a property tree value enumerator. </summary>
		/// <exception cref="PropertyTreeInvalidPathException">
		/// 	Thrown when the path is invalid.
		/// </exception>
		/// <exception cref="PropertyTreeMissingNodeException">
		/// 	Thrown when the path root is different from the tree's root.
		/// </exception>
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="path">	Full path to the XML file. </param>
		/// <returns>	The enumerator. </returns>
		///-------------------------------------------------------------------------------------------------
		public PropertyNodeValueEnumerator<T> GetValueEnumerator<T>(string path)
			where T : IConvertible
		{
			// Split the path string and exception if it is not valid
			string[] pathNodes = SplitPath(path);

			if (pathNodes.Length <= 1)
			{
				throw new PropertyTreeInvalidPathException();
			}

			// If the current root node is null, or the first path directory does not match it's name, throw an exception
			if ((mRootNode == null) || (mRootNode.Name != pathNodes[0]))
			{
				throw new PropertyTreeMissingNodeException();
			}

			// The last node in the path defines the name of the nodes to enumerate
			string searchNode = pathNodes[pathNodes.Length - 1];

			// Iterate through the property tree, following the specified path to the parent directory
			// If a node is missing an exception is thrown
			var currentNode = mRootNode;
			for (int i = 1; i < pathNodes.Length - 1; i++)
			{
				currentNode = currentNode.GetExisting(pathNodes[i]);
			}

			// Return a node value enumerator
			return new PropertyNodeValueEnumerator<T>(currentNode, searchNode);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Puts a value into the property tree. </summary>
		/// <exception cref="PropertyTreeInvalidPathException">
		/// 	Thrown when the path is invalid.
		/// </exception>
		/// <exception cref="PropertyTreeMissingNodeException">
		/// 	Thrown when the path root is different from the tree's root.
		/// </exception>
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="path"> 	Full path to the XML file. </param>
		/// <param name="value">	The value. </param>
		///-------------------------------------------------------------------------------------------------
		public void PutValue<T>(string path, T value)
			where T : IConvertible
		{
			// Split the path string and exception if it is not valid
			string[] pathNodes = SplitPath(path);

			// If the current root node is null, create a new node set to the name of the first path directory
			// If the root node is not null the first path directory must match it's name
			if (mRootNode == null)
			{
				mRootNode = new PropertyNode { Name = pathNodes[0] };
			}
			else if (mRootNode.Name != pathNodes[0])
			{
				throw new PropertyTreeMissingNodeException();
			}

			// Iterate through the property tree, following the specified path to the parent directory
			// Missing nodes are automatically added
			var current = mRootNode;
			for(int i = 1; i < pathNodes.Length - 1; i++)
			{
				current = current.Get(pathNodes[i]);
			}

			// Put a new node into the parent directory
			current.Put(pathNodes[pathNodes.Length - 1], value);
		}
	}
}
