/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Reflection;

namespace BlamLib.TagInterface
{
	/// <summary>
	/// Allows us to interface easily with <see cref="Block{T}"/> since its a template class
	/// </summary>
	/// <remarks>Most useful in <c>BlamLib.Forms</c> code</remarks>
	public interface IBlock : Blam.ICacheStreamable, IMemoryStreamable, IO.ITagStreamable, IStructureOwner
	{
		/// <summary>
		/// Get the definition state this block
		/// </summary>
		/// <returns></returns>
		DefinitionState GetDefinition();

		#region MaxElements
		/// <summary>
		/// Max amount of elements this block can hold
		/// </summary>
		/// <remarks>if 0, it can hold 0-n elements (limited only by memory)</remarks>
		int MaxElements { get; }
		#endregion

		#region Elements
		/// <summary>
		/// Gets the IElementArray interface object for the block
		/// </summary>
		/// <returns>IElementArray interface object</returns>
		IElementArray GetElements();

		/// <summary>
		/// Gets a definition in this block
		/// </summary>
		/// <param name="element">Element index of the definition we want</param>
		/// <returns>Definition object at <paramref name="element"/></returns>
		Definition GetDefinition(int element);

		/// <summary>
		/// Number of elements in the block
		/// </summary>
		int Count { get; }

		/// <summary>
		/// Adds a new element definition to the Block
		/// </summary>
		void Add();

		/// <summary>
		/// Adds a new element definition to the Block, using explicit versioning
		/// </summary>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		void Add(int major, int minor);

		/// <summary>
		/// Inserts a new element definition at index
		/// </summary>
		/// <param name="index"></param>
		void Insert(int index);

		/// <summary>
		/// Copies the element at index and adds it to the end of the element list
		/// </summary>
		/// <param name="index"></param>
		void Duplicate(int index);

		/// <summary>
		/// Removes a certain element definition from the Block
		/// </summary>
		/// <param name="index"></param>
		void Delete(int index);

		/// <summary>
		/// Removes all the element definitions from the Block
		/// </summary>
		void DeleteAll();

		/// <summary>
		/// Clears a block's elements and resizes to size
		/// </summary>
		/// <remarks>Sanity checks <paramref name="size"/> with this block's max elements count</remarks>
		/// <param name="size">The new size (in the elements)</param>
		void Resize(int size);

		/// <summary>
		/// Clears a block's elements and resizes to size using <paramref name="major"/>
		/// and <paramref name="minor"/> for explicit versioning
		/// </summary>
		/// <remarks>Sanity checks <paramref name="size"/> with this block's max elements count</remarks>
		/// <param name="size">The new size (in the elements)</param>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		void Resize(int size, int major, int minor);
		#endregion

		/// <summary>
		/// Calculate how much memory this block consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		int CalculateRuntimeSize(BlamVersion engine, bool cache);

		#region Indexers
		/// <summary>
		/// Wrapper for the ElementArray 'int, int' indexer
		/// </summary>
		/// <param name="element">Index of the Definition which holds the wanted field</param>
		/// <param name="field">Field index of the Field to retrieve</param>
		/// <returns>Field of one of the Definitions of this Block</returns>
		Field this[int element, int field] { get; }
		#endregion

		/// <summary>
		/// Exchanges data with a Block field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args);
	};

	/// <summary>
	/// Block of linear atomic and super-atomic fields
	/// </summary>
	/// <typeparam name="T"></typeparam>
	public sealed class Block<T> : FieldWithPointers, IBlock where T : Definition, new()
	{
		static readonly Type DefinitionType = typeof(T);
		static readonly DefinitionState kState = new T().State;

		#region IStructureOwner
		public Blam.DatumIndex OwnerId {
			get {
				if (owner != null) return owner.OwnerId;

				return Blam.DatumIndex.Null;
			}
		}

		public Blam.DatumIndex TagIndex { 
			get {
				if (owner != null) return owner.TagIndex;

				return Blam.DatumIndex.Null;
			}
		}

		IStructureOwner owner = null;
		public IStructureOwner OwnerObject {
			get { return owner; }
			internal set { owner = value; }
		}
		#endregion

		#region MaxElements
		int maxElements = 0;
		/// <summary>
		/// Max amount of elements this block can hold
		/// </summary>
		/// <remarks>if 0, it can hold 0-n elements (limited only by memory)</remarks>
		public int MaxElements { get { if (maxElements != 0) return maxElements; else return -1; } }
		#endregion

		#region Definition
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		public DefinitionState GetDefinition() { return kState; }
		#endregion

		#region Elements
		ElementArray<T> elements;
		/// <summary>
		/// All the instances of this block
		/// </summary>
		public ElementArray<T> Elements
		{
			get { return elements; }
			set
			{
				elements = value;
				OnPropertyChanged("Elements");
			}
		}
		/// <summary>
		/// Gets the IElementArray interface object for this block
		/// </summary>
		/// <returns>IElementArray interface object</returns>
		public IElementArray GetElements() { return elements; }

		/// <summary>
		/// Interfaces with this Block's elements
		/// </summary>
		public override object FieldValue
		{
			get { return elements; }
			set { elements = value as ElementArray<T>; }
		}

		/// <summary>
		/// Gets a definition in this block
		/// </summary>
		/// <param name="element">Element index of the definition we want</param>
		/// <returns>Definition object at <paramref name="element"/></returns>
		public Definition GetDefinition(int element) { return elements[element]; }

		/// <summary>
		/// Number of elements in the block
		/// </summary>
		public int Count { get { return elements.Count; } }

		/// <summary>
		/// Get a <typeparamref name="T"/> enumerator to enumerate through
		/// this block's elements
		/// </summary>
		/// <returns></returns>
		public List<T>.Enumerator GetEnumerator() { return elements.GetEnumerator(); }

		/// <summary>
		/// Add pre-processor to validate block state
		/// </summary>
		[System.Diagnostics.Conditional("DEBUG")]
		private void AddPreprocess()
		{
			if (maxElements < 1) return;

			Debug.Assert.If(elements.Count <= maxElements, 
				"Block tried passing its max allowed elements: {0} {1} @{2:X8}",
				DefinitionType.ToString(), maxElements, headerOffset);
		}

		/// <summary>
		/// Delete pre-processor to validate block state
		/// </summary>
		[System.Diagnostics.Conditional("DEBUG")]
		private void DeletePreprocess()
		{
			if (maxElements < 1) return;

			Debug.Assert.If(elements.Count != 0, 
				"Block tried deleting an element when it has none: {0}", 
				DefinitionType.ToString());
		}

		/// <summary>
		/// Resize pre-processor to validate block state
		/// </summary>
		/// <param name="resize"></param>
		[System.Diagnostics.Conditional("DEBUG")]
		private void ResizePreprocess(int resize)
		{
			if (maxElements < 1) return;

			Debug.Assert.If(resize <= maxElements, 
				"Block tried passing its max allowed elements: {0} [{1}, !{2}] @{3:X8}",
				DefinitionType.ToString(), maxElements, resize, headerOffset);
		}

		/// <summary>
		/// Adds a new element definition to the Block
		/// </summary>
		public void Add()
		{
			AddPreprocess(); elements.Add(kState.NewInstance(this) as T);
			OnPropertyChanged("Elements");
		}
		void AddInternal()
		{
			elements.Add(kState.NewInstance(this) as T);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Adds a new element definition to the Block, using explicit versioning
		/// </summary>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		public void Add(int major, int minor)
		{
			AddPreprocess(); elements.Add(kState.NewInstance(this, major, minor) as T);
			OnPropertyChanged("Elements");
		}
		void AddInternal(int major, int minor)
		{
			elements.Add(kState.NewInstance(this, major, minor) as T);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Adds a new element definition to the Block, returning
		/// the reference to <paramref name="value"/>
		/// </summary>
		/// <param name="value">Value to receive the added definition reference</param>
		public void Add(out T value)
		{
			AddPreprocess(); elements.Add(value = kState.NewInstance(this) as T);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Adds a new element definition to the Block, using explicit versioning, 
		/// returning the reference to <paramref name="value"/>
		/// </summary>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		/// <param name="value">Value to receive the added definition reference</param>
		public void Add(int major, int minor, out T value)
		{
			AddPreprocess(); elements.Add(value = kState.NewInstance(this, major, minor) as T);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Inserts a new element definition at index
		/// </summary>
		/// <param name="index">Index at which the new definition will be inserted at</param>
		public void Insert(int index)
		{
			AddPreprocess(); elements.Insert(index, kState.NewInstance(this) as T);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Inserts <paramref name="data"/> definition at index
		/// </summary>
		/// <param name="index">Index at which the data will be inserted at</param>
		/// <param name="data">Definition to insert into this</param>
		public void Insert(int index, T data)
		{
			AddPreprocess(); elements.Insert(index, data);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Copies the element at index and adds it to the end of the element list
		/// </summary>
		/// <param name="index">Index of the element to duplicate</param>
		public void Duplicate(int index)
		{
			AddPreprocess(); elements.Add(elements[index].Clone() as T);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Removes a certain element definition from the Block
		/// </summary>
		/// <param name="index">Index of the element to delete</param>
		public void Delete(int index)
		{
			DeletePreprocess(); elements.RemoveAt(index);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Removes all the element definitions from the Block
		/// </summary>
		public void DeleteAll()
		{
			elements.Clear();
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Adds an existing element definition to the Block
		/// </summary>
		/// <param name="value"></param>
		/// <exception cref="ArgumentNullException"></exception>
		internal void AddElement(T value)
		{
			if (value == null)
				throw new ArgumentNullException("value");

			AddPreprocess();
			elements.Add(value);
			OnPropertyChanged("Elements");
		}

		/// <summary>
		/// Clears a block's elements and resizes to size
		/// </summary>
		/// <remarks>Sanity checks <paramref name="size"/> with this block's max elements count</remarks>
		/// <param name="size">The new size (in the elements) </param>
		public void Resize(int size)
		{
			DeleteAll();
			if (size > 0)
			{
				ResizePreprocess(size);
				for (int x = 0; x < size; x++) AddInternal();
			}
		}

		/// <summary>
		/// Clears a block's elements and resizes to size using <paramref name="major"/>
		/// and <paramref name="minor"/> for explicit versioning
		/// </summary>
		/// <remarks>Sanity checks <paramref name="size"/> with this block's max elements count</remarks>
		/// <param name="size">The new size (in the elements)</param>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		public void Resize(int size, int major, int minor)
		{
			DeleteAll();
			if (size > 0)
			{
				ResizePreprocess(size);
				for (int x = 0; x < size; x++) AddInternal(major, minor);
			}
		}

		void ResizeImplicitUpgradeBegin(int size_of, IO.ITagStream ts)
		{
			foreach (var e in this)
				e.VersionImplicitUpgradeBegin(size_of, ts);

		}
		void ResizeImplicitUpgradeEnd()
		{
			foreach (var e in this)
				e.VersionImplicitUpgradeEnd();
		}
		#endregion

		/// <summary>
		/// Calculate how much memory this definition consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		public int CalculateRuntimeSize(BlamVersion engine, bool cache) { return elements.CalculateRuntimeSize(engine, cache); }

		#region Indexers
		/// <summary>
		/// Wrapper for the ElementArray 'int' indexer
		/// </summary>
		/// <param name="element">Index of the Definition to retrieve</param>
		/// <returns>Definition of this Block</returns>
		public T this[int element] { get { return elements[element]; } }
		/// <summary>
		/// Wrapper for the ElementArray 'int, int' indexer
		/// </summary>
		/// <param name="element">Index of the Definition which holds the wanted field</param>
		/// <param name="field">Field index of the Field to retrieve</param>
		/// <returns>Field of one of the Definitions of this Block</returns>
		public Field this[int element, int field] { get { return elements[element, field]; } }
		#endregion

		#region Creation
		private Block() :										base(FieldType.Block)	{ Elements = new ElementArray<T>(); }
		/// <summary>
		/// Create a new Block with a specified owner
		/// </summary>
		/// <param name="owner">Owner object of this Block</param>
		public Block(IStructureOwner owner) :					this()					{ this.owner = owner; }
		/// <summary>
		/// Create a new Block definition with a maximum element count of <paramref name="max"/>
		/// </summary>
		/// <param name="max">Max allowed elements for this block</param>
		private Block(int max) : base(FieldType.Block)
		{
			maxElements = max;
			Elements = /*max > 0 ? new ElementArray<T>(max) :*/ new ElementArray<T>();
		}
		/// <summary>
		/// Create a new Block definition with a specified owner
		/// and a maximum element count of <paramref name="max"/>
		/// </summary>
		/// <param name="owner">Owner object of this Block</param>
		/// <param name="max">Max allowed elements for this block</param>
		public Block(IStructureOwner owner, int max) :			this(max)				{ this.owner = owner; }
		/// <summary>
		/// Create a new Block definition that is a clone of <paramref name="value"/>
		/// </summary>
		/// <param name="value">Block to clone</param>
		public Block(Block<T> value) :							this(value.maxElements)	{ this.Elements = (ElementArray<T>)value.elements.Clone(); }
		/// <summary>
		/// Create a new Block definition that is a clone of <paramref name="value"/>
		/// </summary>
		/// <param name="value">Block to clone</param>
		/// <param name="owner"></param>
		public Block(Block<T> value, IStructureOwner owner) :	this(value)				{ this.owner = owner; }
		/// <summary>
		/// Create a new Block definition that is a clone of this definition, 
		/// but doesn't hold the same runtime data (e.g. elements)
		/// </summary>
		/// <param name="from"></param>
		/// <returns></returns>
		public override Field CreateInstance(Field from)								{ return new Block<T>( (from as Block<T>).maxElements ); }
		/// <summary>
		/// Completely clone this Block Definition
		/// </summary>
		/// <returns></returns>
		public override object Clone()													{ return new Block<T>(this); }
		/// <summary>
		/// Completely clone this Block Definition
		/// </summary>
		/// <param name="owner"></param>
		/// <returns></returns>
		public override object Clone(IStructureOwner owner)								{ return new Block<T>(this, owner); }

		// TODO: blocks don't have valid handles...
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, kState.Handle); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Block field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		bool NeedsUpgrading = false;

		#region Cache
		/// <summary>
		/// Process the reference data from the cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void ReadHeader(BlamLib.Blam.CacheFile c)
		{
#if DEBUG
			relativeOffset = c.InputStream.PositionUnsigned;
#endif
			VersionCtorAttribute upgrade_parameters;
			if ((c.EngineVersion & BlamVersion.Halo1) != 0)
			{
				Resize(c.InputStream.ReadInt32()); // element count
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadUInt32();
			}
			#region Halo 2 Betas
			else if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
			{
				if ((upgrade_parameters = kState.VersionForEngine(c.EngineVersion)) != null)
				{
					NeedsUpgrading = true;
					Resize(c.InputStream.ReadInt32(), upgrade_parameters.Major, upgrade_parameters.Minor);
				}
				else
				{
					NeedsUpgrading = false;
					Resize(c.InputStream.ReadInt32()); // element count
				}
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadUInt32();
			}
			#endregion
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)
			{
				Resize(c.InputStream.ReadInt32()); // element count
				relativeOffset = c.InputStream.ReadPointer();
			}
			#region Halo 3 Betas
			else if (c.EngineVersion == BlamVersion.Halo3_Beta)
			{
				if ((upgrade_parameters = kState.VersionForEngine(c.EngineVersion)) != null)
				{
					NeedsUpgrading = true;
					Resize(c.InputStream.ReadInt32(), upgrade_parameters.Major, upgrade_parameters.Minor);
				}
				else
				{
					NeedsUpgrading = false;
					Resize(c.InputStream.ReadInt32()); // element count
				}
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadUInt32();
			}
			#endregion
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				Resize(c.InputStream.ReadInt32()); // element count
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadUInt32();
			}
		}

		/// <summary>
		/// Process the block from the cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void Read(BlamLib.Blam.CacheFile c)
		{
			if (relativeOffset == 0) return;

			c.InputStream.Seek(relativeOffset, System.IO.SeekOrigin.Begin);
			elements.Read(c);

			if (NeedsUpgrading) elements.Upgrade();
		}

		/// <summary>
		/// Process the reference data to the cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void WriteHeader(BlamLib.Blam.CacheFile c)
		{
			if ((c.EngineVersion & BlamVersion.Halo1) != 0)
			{
				c.OutputStream.Write(elements.Count);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0);
				c.OutputStream.Write(0);
			}
			else if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
			{
				c.OutputStream.Write(elements.Count);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0);
				c.OutputStream.Write(0);
			}
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)
			{
				c.OutputStream.Write(elements.Count);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0);
			}
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				c.OutputStream.Write(elements.Count);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0);
				c.OutputStream.Write(0);
			}
			// TODO: what happened? where's stubbs?
		}

		/// <summary>
		/// Process the block to the cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void Write(BlamLib.Blam.CacheFile c)
		{
			relativeOffset = c.OutputStream.PositionUnsigned; // store offset
			c.OutputStream.Seek(headerOffset, System.IO.SeekOrigin.Begin); // go to the reflexive header
			c.OutputStream.WritePointer(relativeOffset); // write offset
			c.OutputStream.Seek(relativeOffset, System.IO.SeekOrigin.Begin); // go back to where we were

			if ((c.EngineVersion & BlamVersion.Halo1) != 0)			elements.Write(c);
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)	elements.Write(c);
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0)elements.Write(c);
		}
		#endregion

		#region Tag
		/// <summary>
		/// Process the reference data from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidTagBlock"></exception>
		public override void ReadHeader(IO.ITagStream ts)
		{
			IO.EndianReader s = ts.GetInputStream();
			headerOffset = s.PositionUnsigned; // nifty for debugging
			try
			{
				Resize(s.ReadInt32()); // element count
				relativeOffset = s.ReadPointer(); // elements
				s.ReadInt32(); // definition
			}
			catch (Exception ex)
			{
				throw new Exceptions.InvalidTagBlock(ex,
					base.headerOffset, uint.MaxValue,
					this.owner, ts);
			}
		}

		/// <summary>
		/// Process the block data from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidTagBlock"></exception>
		public override void Read(IO.ITagStream ts)
		{
			NeedsUpgrading = false;
			bool resizeImplicitUpgradeIsActive = false;

			if (this.elements.Count != 0)
			{
				IO.EndianReader s = ts.GetInputStream();

				if (!ts.Flags.Test(IO.ITagStreamFlags.UseStreamPositions))
					relativeOffset = s.PositionUnsigned; // nifty for debugging
				else
					s.PositionUnsigned = relativeOffset;

				try
				{
					if (DefinitionState.FieldSetRequiresVersioningHeader(ts))
					{
						IFieldSetVersioning fs = kState.FieldSetReadVersion(ts, FieldType.Block, relativeOffset, Count);

						if (NeedsUpgrading = fs.NeedsUpgrading)
						{
							int index, size_of;
							fs.GetUpgradeParameters(out index, out size_of);

							if (!(resizeImplicitUpgradeIsActive = fs.UseImplicitUpgrading))
								Resize(Count, index, size_of);
							else
								ResizeImplicitUpgradeBegin(size_of, ts);
						}
					}

					elements.Read(ts);
				}
				catch (Debug.ExceptionLog del) { throw del; }
				catch (Exception ex)
				{
					throw new Exceptions.InvalidTagBlock(ex,
						base.headerOffset, base.relativeOffset,
						this.owner, ts);
				}

				if (NeedsUpgrading)
					if(resizeImplicitUpgradeIsActive)
						ResizeImplicitUpgradeEnd();
					else
						elements.Upgrade();
			}
		}

		/// <summary>
		/// Process the reference data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void WriteHeader(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();
			s.Write(elements.Count);
			headerOffset = s.PositionUnsigned; // we'll come back here and write the element's offset
			s.Write(-1);
			s.Write(0);
		}

		/// <summary>
		/// Process the block data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void Write(IO.ITagStream ts)
		{
			if (this.elements.Count == 0) return; // don't do anything when empty
			IO.EndianWriter s = ts.GetOutputStream();

			if (ts.Flags.Test(IO.ITagStreamFlags.UseStreamPositions))
			{
				relativeOffset = s.PositionUnsigned;				// store offset
				s.Seek(headerOffset, System.IO.SeekOrigin.Begin);	// go to the reflexive header
				s.WritePointer(relativeOffset);						// write offset
				s.Seek(relativeOffset, System.IO.SeekOrigin.Begin);	// go back to where we were
			}

			if (DefinitionState.FieldSetRequiresVersioningHeader(ts))
				kState.FieldSetWriteVersion(ts, FieldType.Block, Count);

			elements.Write(ts);
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Process the reference data from a stream
		/// </summary>
		/// <param name="s">Stream</param>
		/// <exception cref="Exceptions.InvalidTagBlock"></exception>
		public override void ReadHeader(IO.EndianReader s)
		{
			headerOffset = s.PositionUnsigned; // nifty for debugging

			Resize(s.ReadInt32()); // element count
			s.ReadInt32(); // elements
			s.ReadInt32(); // definition
		}

		/// <summary>
		/// Process the block data from a stream
		/// </summary>
		/// <param name="s">Stream</param>
		/// <exception cref="Exceptions.InvalidTagBlock"></exception>
		public override void Read(IO.EndianReader s)
		{
			if (this.elements.Count == 0) return;

			relativeOffset = s.PositionUnsigned; // nifty for debugging
			elements.Read(s);
		}

		/// <summary>
		/// Process the reference data to a stream
		/// </summary>
		/// <param name="s">Stream</param>
		public override void WriteHeader(IO.EndianWriter s)
		{
			s.Write(elements.Count);
			headerOffset = s.PositionUnsigned; // we'll come back here and write the element's offset
			s.Write(-1);
			s.Write(0);
		}

		/// <summary>
		/// Process the block data to a stream
		/// </summary>
		/// <param name="s">Stream</param>
		public override void Write(IO.EndianWriter s)
		{
			if (this.elements.Count == 0) return; // don't do anything when empty

			relativeOffset = s.PositionUnsigned;				// store offset
			s.Seek(headerOffset, System.IO.SeekOrigin.Begin);	// go to the reflexive header
			s.WritePointer(relativeOffset);						// write offset
			s.Seek(relativeOffset, System.IO.SeekOrigin.Begin);	// go back to where we were

			elements.Write(s);
		}
		#endregion
		#endregion
	};
}