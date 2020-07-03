/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

﻿using System;
using System.Collections.Generic;
using System.Reflection;

namespace BlamLib.TagInterface
{
	/// <summary>
	/// Allows us to interface easily with <see cref="StructBase{T}"/> since its a template class
	/// </summary>
	/// <remarks>Most useful in <c>BlamLib.Forms</c> code</remarks>
	public interface IStruct : Blam.ICacheStreamable, IMemoryStreamable, IO.ITagStreamable, IStructureOwner
	{
		void Initialize();
		void Dispose();

		/// <summary>
		/// Get the definition state this struct
		/// </summary>
		/// <returns></returns>
		DefinitionState GetDefinition();

		/// <summary>
		/// Gets the definition instance for this struct
		/// </summary>
		/// <returns>Definition state</returns>
		Definition GetValue();

		/// <summary>
		/// Calculate how much memory this struct consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		int CalculateRuntimeSize(BlamVersion engine, bool cache);
	};

	/// <summary>
	/// Base generic implementation for tag struct fields
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <remarks>
	/// While <see cref="Struct{T}"/> doesn't really apply when it comes to inheriting from 
	/// <see cref="FieldWithPointers"/>, <see cref="StructReference{T}"/> does...and it at least 
	/// gives Struct some helpful internal stuff...but not really. Shut-up.
	/// 
	/// Also, I didn't have StructBase inherit from <see cref="IStruct"/> because I would have ended up redeclaring 
	/// the methods as abstract anyways to satisfy the compiler. However, both sealed Struct field classes
	/// inherit from <see cref="IStruct"/>
	/// </remarks>
	/// <see cref="Struct{T}"/>
	/// <see cref="StructReference{T}"/>
	public abstract class StructBase<T> : FieldWithPointers, IStructureOwner where T : Definition, new()
	{
		protected static readonly Type DefinitionType = typeof(T);
		protected static readonly DefinitionState kState = new T().State;

		#region IStructureOwner
		public Blam.DatumIndex OwnerId
		{
			get
			{
				if (owner != null) return owner.OwnerId;

				return Blam.DatumIndex.Null;
			}
		}

		public Blam.DatumIndex TagIndex
		{
			get
			{
				if (owner != null) return owner.TagIndex;

				return Blam.DatumIndex.Null;
			}
		}

		protected IStructureOwner owner = null;
		public IStructureOwner OwnerObject
		{
			get { return owner; }
			internal set { owner = value; }
		}
		#endregion

		#region IStruct
		public DefinitionState GetDefinition() { return kState; }

		/// <summary>
		/// Gets the definition instance for this container
		/// </summary>
		/// <returns>Definition instance</returns>
		public Definition GetValue() { return Value; }

		/// <summary>
		/// Actual instance data for this container
		/// </summary>
		public T Value;

		/// <summary>
		/// Interfaces with this container's Definition object
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as T; }
		}
		#endregion

		protected StructBase(FieldType type) : base(type) { }

		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, kState.Handle); }

		/// <summary>
		/// Implicit conversion of a container to its definition
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static implicit operator T(StructBase<T> value) { return value.Value; }

		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { throw new Exception("The method or operation is not implemented."); }
	};

	/// <summary>
	/// Provides a way to mimic an C-like in-place struct in a Definition
	/// </summary>
	/// <remarks>
	/// If <typeparamref name="T"/> overrides the internal post-process 
	/// function, it <b>will</b> be called still before the data is written to file.
	/// 
	/// Supported in Halo 1 and Stubbs, with no special treatment (ie, field headers).
	/// </remarks>
	/// <typeparam name="T">Definition that defines the struct</typeparam>
	/// <see cref="BlamLib.TagInterface.Definition"/>
	public sealed class Struct<T> : StructBase<T>, IStruct, IIOProcessable where T : Definition, new()
	{
		void IStruct.Initialize() { }
		void IStruct.Dispose() { }

		/// <summary>
		/// Calculate how much memory this definition consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		public int CalculateRuntimeSize(BlamVersion engine, bool cache) { return Value.CalculateRuntimeSize(engine, cache); }

		#region Ctor
		/// <summary>
		/// Construct a struct field
		/// </summary>
		Struct() :										base(FieldType.Struct)	{ Value = new T(); Value.SetOwnerObject(this); }
		/// <summary>
		/// Construct a struct field
		/// </summary>
		/// <param name="owner">specific owner object</param>
		public Struct(IStructureOwner owner) :					this()			{ this.owner = owner; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Struct(Struct<T> value) :						this()			{ Value = (T)value.Value.Clone(); }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		/// <param name="owner"></param>
		public Struct(Struct<T> value, IStructureOwner owner) :	this(value)		{ this.owner = owner; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)						{ return new Struct<T>(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guaranteed not to reference any tag data which we copied</returns>
		public override object Clone()											{ return new Struct<T>(this); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <param name="owner"></param>
		/// <returns>Complete copy guaranteed not to reference any tag data which we copied</returns>
		public override object Clone(IStructureOwner owner)						{ return new Struct<T>(this, owner); }
		#endregion

		#region I\O
		bool NeedsUpgrading = false;

		#region Cache I\O
		/// <summary>
		/// Process the struct field from a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		/// <param name="iop">Processing method</param>
		public void Read(BlamLib.Blam.CacheFile c, IOProcess iop)
		{
			VersionCtorAttribute upgrade_parameters;
			if (c.EngineVersion == BlamVersion.Halo2_Alpha ||  c.EngineVersion == BlamVersion.Halo2_Epsilon || c.EngineVersion == BlamVersion.Halo3_Beta)
			{
				if ((upgrade_parameters = kState.VersionForEngine(c.EngineVersion)) != null)
				{
					// It is ASSUMED that the group tag won't ever be needed for version construction
					Value = (T)kState.NewInstance(this, upgrade_parameters.Major, upgrade_parameters.Minor);
					NeedsUpgrading = true;
				}
				else
					NeedsUpgrading = false;
			}

			Value.Read(c, iop);

			if (NeedsUpgrading)
#if DEBUG
				Debug.Assert.If(Value.Upgrade(), "Failed to upgrade struct. {0} {1}", typeof(T).Name, Value.ToVersionString());
#else
				Value.Upgrade();
#endif
		}

		/// <summary>
		/// Process the struct field to a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		/// <param name="iop">Processing method</param>
		public void Write(BlamLib.Blam.CacheFile c, IOProcess iop) { Value.Write(c, iop); }
		#endregion

		#region Tag
		/// <summary>
		/// Stream the field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidTagStruct"></exception>
		public override void Read(IO.ITagStream ts)
		{
			IO.EndianReader s = ts.GetInputStream();
			base.relativeOffset = s.PositionUnsigned; // use relative offset since the 'fieldset header' is really...well the header
			NeedsUpgrading = false;
			try
			{
				if(DefinitionState.FieldSetRequiresVersioningHeader(ts))
				{
					IFieldSetVersioning fs = kState.FieldSetReadVersion(ts, FieldType.Struct, base.relativeOffset, 1);

					if (NeedsUpgrading = fs.NeedsUpgrading)
					{
						int index, size_of;
						fs.GetUpgradeParameters(out index, out size_of);

						// It is ASSUMED that the group tag won't ever be needed for version construction
						if (!fs.UseImplicitUpgrading)
							Value = (T)kState.NewInstance(this, index, size_of);
						else
							Value.VersionImplicitUpgradeBegin(size_of, ts);
					}
				}
			}
			catch (Debug.ExceptionLog del) { throw del; }
			catch (Exception ex)
			{
				throw new Exceptions.InvalidTagStruct(ex,
					base.relativeOffset, this.owner, ts);
			}
		}

		/// <summary>
		/// Process the struct field from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <param name="iop">Processing method</param>
		public void Read(IO.ITagStream ts, IOProcess iop)
		{
			Value.Read(ts, iop);

			if (NeedsUpgrading)
				if (Value.VersionImplicitUpgradeIsActive)
					Value.VersionImplicitUpgradeEnd();
				else
				{
#if DEBUG
					if (!Value.Upgrade())
						Debug.Assert.If(false, "Failed to upgrade struct. {0} {1} in {2} @{3:X8}", DefinitionType.Name, Value.ToVersionString(), ts.GetExceptionDescription(), base.relativeOffset);
#else
					Value.Upgrade();
#endif
				}
		}

		/// <summary>
		/// Stream the field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void Write(IO.ITagStream ts)
		{
			if (DefinitionState.FieldSetRequiresVersioningHeader(ts))
				kState.FieldSetWriteVersion(ts, FieldType.Struct);
		}

		/// <summary>
		/// Process the struct field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <param name="iop">Processing method</param>
		public void Write(IO.ITagStream ts, IOProcess iop) { Value.Write(ts, iop); }
		#endregion

		#region Generic Stream
		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Read(BlamLib.IO.EndianReader s)		{}

		/// <summary>
		/// Process the struct field from a stream
		/// </summary>
		/// <param name="s"></param>
		/// <param name="iop">Processing method</param>
		public void Read(IO.EndianReader s, IOProcess iop)			{ Value.Read(s, iop); }

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Write(BlamLib.IO.EndianWriter s)		{}

		/// <summary>
		/// Process the struct field to a stream
		/// </summary>
		/// <param name="output">Stream</param>
		/// <param name="iop">Processing method</param>
		public void Write(IO.EndianWriter output, IOProcess iop)	{ Value.Write(output, iop); }
		#endregion
		#endregion
	};

	// TODO: verify the usage of this field for in Reach
	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <remarks><see cref="StructBase{T}.Value"/> will initially be null</remarks>
	public sealed class StructReference<T> : StructBase<T>, IStruct where T : Definition, new()
	{
		public void Initialize()
		{
			Value = new T();
			Value.SetOwnerObject(this);
		}

		public void Dispose() { Value = null; }

		/// <summary>
		/// Calculate how much memory this definition consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		int IStruct.CalculateRuntimeSize(BlamVersion engine, bool cache) { return 12/*Value.CalculateRuntimeSize(engine, cache)*/; }

		#region Ctor
		/// <summary>
		/// Construct a data reference field
		/// </summary>
		StructReference() :									base(FieldType.StructReference)		{ /*Value = new T(); Value.SetOwnerObject(this);*/ }
		/// <summary>
		/// Construct a data reference field
		/// </summary>
		/// <param name="owner">specific owner object</param>
		public StructReference(IStructureOwner owner) :								this()		{ this.owner = owner; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public StructReference(StructReference<T> value) :							this()		{ if(value.Value != null) Value = (T)value.Value.Clone(this); }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		/// <param name="owner"></param>
		public StructReference(StructReference<T> value, IStructureOwner owner) :	this(value) { this.owner = owner; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new StructReference<T>(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guaranteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new StructReference<T>(this); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <param name="owner"></param>
		/// <returns>Complete copy guaranteed not to reference any tag data which we copied</returns>
		public override object Clone(IStructureOwner owner)										{ return new StructReference<T>(this, owner); }
		#endregion

		#region I\O
		bool NeedsUpgrading = false;

		#region Cache I\O
		/// <summary>
		/// Process the data reference from the cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void ReadHeader(BlamLib.Blam.CacheFile c)
		{
			if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
				(c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadUInt32(); // ?
				c.InputStream.ReadUInt32(); // definition pointer
			}
			else relativeOffset = 0;
		}

		/// <summary>
		/// Process the data reference field from a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void Read(BlamLib.Blam.CacheFile c)
		{
			if (relativeOffset == 0) return;

			if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
				(c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				c.InputStream.Seek(relativeOffset, System.IO.SeekOrigin.Begin);
				this.Initialize();
				Value.Read(c);
			}
		}

		/// <summary>
		/// Process the data reference to the cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void WriteHeader(BlamLib.Blam.CacheFile c)
		{
			if (Value == null) return;

			if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
				(c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0); // will remain null if we have no data
				c.OutputStream.Write(0);
				c.OutputStream.Write(0);
			}
		}

		/// <summary>
		/// Process the data reference field to a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public override void Write(BlamLib.Blam.CacheFile c)
		{
			if (Value == null) return;

			if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
				(c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				relativeOffset = c.OutputStream.PositionUnsigned; // store offset
				c.OutputStream.Seek(headerOffset, System.IO.SeekOrigin.Begin); // go to the reflexive header
				c.OutputStream.WritePointer(relativeOffset); // write offset
				c.OutputStream.Seek(relativeOffset, System.IO.SeekOrigin.Begin); // go back to where we were
				Value.Write(c);
			}
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
				relativeOffset = s.ReadPointer(); // data
				s.ReadInt32(); // ?
				s.ReadInt32(); // definition
			}
			catch (Exception ex)
			{
				throw new Exceptions.InvalidTagBlock(ex,
					base.headerOffset, uint.MaxValue,
					this.owner, ts);
			}

			// TODO: removing this check could lead to problems in the future.
			// We removed it due to resource loading code in h3 where we would 
			// have the definition at the beginning of the stream
			Initialize();//if (relativeOffset > 0) Initialize();
		}

		/// <summary>
		/// Process the data reference from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidStructReference"></exception>
		public override void Read(IO.ITagStream ts)
		{
			NeedsUpgrading = false;

			if (Value != null)
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
						IFieldSetVersioning fs = kState.FieldSetReadVersion(ts, FieldType.StructReference, relativeOffset);

						if (NeedsUpgrading = fs.NeedsUpgrading)
						{
							int index, size_of;
							fs.GetUpgradeParameters(out index, out size_of);

							if (!fs.UseImplicitUpgrading)
								Value = (T)kState.NewInstance(this, index, size_of);
							else
								Value.VersionImplicitUpgradeBegin(size_of, ts);
						}
					}

					Value.Read(ts);
				}
				catch (Debug.ExceptionLog del) { throw del; }
				catch (Exception ex)
				{
					throw new Exceptions.InvalidStructReference(ex,
						base.headerOffset, base.relativeOffset,
						this.owner, ts);
				}

				if (NeedsUpgrading)
					if (Value.VersionImplicitUpgradeIsActive)
						Value.VersionImplicitUpgradeEnd();
					else
					{
#if DEBUG
						Debug.Assert.If(Value.Upgrade(), "Failed to upgrade data reference. {0} {1} in {2} @{3:X8}", DefinitionType.Name, Value.ToVersionString(), ts.GetExceptionDescription(), base.relativeOffset);
#else
						Value.Upgrade();
#endif
					}
			}
		}

		/// <summary>
		/// Process the data reference to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void WriteHeader(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();
			headerOffset = s.PositionUnsigned; // we'll come back here and write the definition offset
			s.Write(0);
			s.Write(0); // ?
			s.Write(0); // definition
		}

		/// <summary>
		/// Process the block data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void Write(IO.ITagStream ts)
		{
			if (Value == null) return; // don't do anything when empty
			IO.EndianWriter s = ts.GetOutputStream();

			if (ts.Flags.Test(IO.ITagStreamFlags.UseStreamPositions))
			{
				relativeOffset = s.PositionUnsigned;				// store offset
				s.Seek(headerOffset, System.IO.SeekOrigin.Begin);	// go to the reflexive header
				s.WritePointer(relativeOffset);						// write offset
				s.Seek(relativeOffset, System.IO.SeekOrigin.Begin);	// go back to where we were
			}

			if (DefinitionState.FieldSetRequiresVersioningHeader(ts))
				kState.FieldSetWriteVersion(ts, FieldType.StructReference);

			Value.Write(ts);
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Process the data reference from a stream
		/// </summary>
		/// <param name="s">Stream</param>
		/// <exception cref="Exceptions.InvalidTagBlock"></exception>
		public override void ReadHeader(IO.EndianReader s)
		{
			headerOffset = s.PositionUnsigned; // nifty for debugging

			if (s.ReadInt32() > 0) this.Initialize();
			s.ReadInt32(); // ?
			s.ReadInt32(); // definition
		}

		/// <summary>
		/// Process the data reference from a stream
		/// </summary>
		/// <param name="s">Stream</param>
		/// <exception cref="Exceptions.InvalidTagBlock"></exception>
		public override void Read(IO.EndianReader s)
		{
			if (Value == null) return;

			relativeOffset = s.PositionUnsigned; // nifty for debugging
			Value.Read(s);
		}

		/// <summary>
		/// Process the data reference to a stream
		/// </summary>
		/// <param name="s">Stream</param>
		public override void WriteHeader(IO.EndianWriter s)
		{
			headerOffset = s.PositionUnsigned; // we'll come back here and write the element's offset
			s.Write(0);
			s.Write(0);
			s.Write(0);
		}

		/// <summary>
		/// Process the data reference to a stream
		/// </summary>
		/// <param name="s">Stream</param>
		public override void Write(IO.EndianWriter s)
		{
			if (Value == null) return; // don't do anything when empty

			relativeOffset = s.PositionUnsigned;				// store offset
			s.Seek(headerOffset, System.IO.SeekOrigin.Begin);	// go to the reflexive header
			s.WritePointer(relativeOffset);						// write offset
			s.Seek(relativeOffset, System.IO.SeekOrigin.Begin);	// go back to where we were

			Value.Write(s);
		}
		#endregion
		#endregion
	};
}