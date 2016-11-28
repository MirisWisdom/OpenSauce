/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Managers
{
	/// <summary>
	/// Interface for the DataArray class
	/// </summary>
	public interface IDataArray : System.Collections.IEnumerable
	{
		/// <summary>
		/// Add a new object to the index and return its handle
		/// </summary>
		/// <returns>Handle to the new object</returns>
		Blam.DatumIndex Add();

		/// <summary>
		/// Removes a reference to a object by the handle 
		/// <paramref name="index"/> and sets that index to null if that
		/// was the last reference
		/// </summary>
		/// <param name="index"></param>
		/// <returns>true if set to null</returns>
		bool Remove(Blam.DatumIndex index);

		/// <summary>
		/// Disposes of the object and removes it and sets that index to null
		/// </summary>
		/// <param name="index"></param>
		/// <returns>index of the closed element</returns>
		Blam.DatumIndex Close(Blam.DatumIndex index);

		/// <summary>
		/// Disposes and removes all datums data in the array
		/// and sets each index to be null
		/// </summary>
		void CloseAll();

		/// <summary>
		/// Figures out if the index is valid in this data array
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		bool Exists(Blam.DatumIndex index);
	};

	public class DataArrayEventArgsMove : EventArgs
	{
		#region FromArray
		private IDataArray fromArray;
		/// <summary>
		/// Array the move event is 'taking' from
		/// </summary>
		public IDataArray FromArray	{ get { return fromArray; } }
		#endregion

		#region FromIndex
		private Blam.DatumIndex fromIndex;
		/// <summary>
		/// Index of the object that is being moving
		/// </summary>
		public Blam.DatumIndex FromIndex	{ get { return fromIndex; } }
		#endregion

		#region ToArray
		private IDataArray toArray;
		/// <summary>
		/// Array the move event is 'giving' to
		/// </summary>
		public IDataArray ToArray	{ get { return toArray; } }
		#endregion

		#region ToIndex
		private Blam.DatumIndex toIndex;
		/// <summary>
		/// Index of the object that was moved
		/// </summary>
		public Blam.DatumIndex ToIndex	{ get { return toIndex; } }
		#endregion

		public DataArrayEventArgsMove(IDataArray from, Blam.DatumIndex from_index, IDataArray to, Blam.DatumIndex to_index)
		{
			fromArray = from;
			fromIndex = from_index;
			toArray = to;
			toIndex = to_index;
		}
	};

	/// <summary>
	/// Manager for a collection of objects (ie, <see cref="TagManager"/>)
	/// </summary>
	/// <typeparam name="T">
	/// Must be a class, implement <see cref="IDisposable"/> and a default constructor.
	/// </typeparam>
	public class DataArray<T> : IDataArray, IEnumerable<T> where T : class, IDisposable, new()
	{
		static Blam.DatumIndex CalculateDatumIndex(short salt_base, ushort index)
		{
			return new BlamLib.Blam.DatumIndex(index, (short)(salt_base + (short)index));
		}

		#region Element
		/// <summary>
		/// Element data for a item in the data array
		/// </summary>
		class Element
		{
			/// <summary>
			/// Index of this element
			/// </summary>
			public ushort Header;
			/// <summary>
			/// How many times this element is referenced
			/// </summary>
			short References;
			/// <summary>
			/// Instance data for this element
			/// </summary>
			public T Data;

			public Blam.DatumIndex GetDatumIndex(short salt_base)
			{
				return CalculateDatumIndex(salt_base, this.Header);
			}

			public Element(ushort index, T data)
			{
				Header = index;
				References = 0;
				Data = data;
			}

			/// <summary>
			/// Increments the reference count
			/// </summary>
			/// <returns>new reference count</returns>
			public int AddReference() { return ++References; }
			/// <summary>
			/// Decrements the reference count, disposing and 
			/// nulling the element if this was the last reference
			/// </summary>
			/// <returns>references left</returns>
			public int RemoveReference()
			{
				if(References == 1)
				{
					Data.Dispose();
					Data = null;
				}
				return --References;
			}

			/// <summary>
			/// Forces a zero reference state on the element
			/// </summary>
			public void Nullify()
			{
				if (References != 0)
				{
					References = 0;
					Data.Dispose();
					Data = null;
				}
			}

			/// <summary>
			/// Can this element be garbage collected?
			/// </summary>
			/// <returns></returns>
			public bool GarbageCollect() { return References == 0; }
		};
		#endregion

		#region Enumerator
		/// <summary>
		/// Enumerates the elements of a <see cref="BlamLib.Managers.DataArray{T}"/>
		/// </summary>
		public struct Enumerator : IEnumerator<T>
		{
			Dictionary<Blam.DatumIndex, Element>.Enumerator enumerator;

			// we have to do this cast shit here because the class 'Element' is meant to be private
			// and we don't want it to be visible outside of this class, so we don't mark it internal.
			internal Enumerator(object data) { enumerator = (Dictionary<Blam.DatumIndex, Element>.Enumerator)data; }

			/// <summary>
			/// Gets the element's DatumIndex
			/// </summary>
			public Blam.DatumIndex CurrentIndex { get { return enumerator.Current.Key; } }

			#region IEnumerator<T> Members
			/// <summary>
			/// Gets the element at the current position of the enumerator.
			/// </summary>
			public T Current { get { return enumerator.Current.Value.Data; } }
			#endregion

			#region IDisposable Members
			public void Dispose() { enumerator.Dispose(); }
			#endregion

			#region IEnumerator Members
			object System.Collections.IEnumerator.Current { get { return enumerator.Current.Value.Data; } }

			/// <summary>
			/// Advances the enumerator to the next element of the DataArray
			/// </summary>
			/// <returns>true if the enumerator was successfully advanced to the next element; false if the enumerator has passed the end of the collection.</returns>
			public bool MoveNext()
			{
				// loop past any 'null' datums
				while (enumerator.MoveNext())
					if (!enumerator.Current.Value.GarbageCollect()) return true;
				
				return false;
			}

			/// <summary>
			/// Unsupported operation
			/// </summary>
			public void Reset() { throw new Debug.ExceptionLog("The method or operation is not implemented."); }
			#endregion
		};

		#region IEnumerable<T> Members
		/// <summary>
		/// Returns our special enumerator that iterates through the DataArray
		/// </summary>
		/// <returns></returns>
		public Enumerator GetDatumEnumerator()	{ return new Enumerator(Items.GetEnumerator()); }
		/// <summary>
		/// Returns an enumerator that iterates through the DataArray
		/// </summary>
		/// <returns></returns>
		public IEnumerator<T> GetEnumerator()	{ return new Enumerator(Items.GetEnumerator()); }

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return GetEnumerator(); }
		#endregion
		#endregion

		#region Instance data
		/// <summary>
		/// Name for this data array
		/// </summary>
		public readonly string Name;
		/// <summary>
		/// Salt base for our datum indexes
		/// </summary>
		readonly short Salt;
		/// <summary>
		/// Dictionary to hold our datum index to element matching
		/// </summary>
		Dictionary<Blam.DatumIndex, Element> Items;
		#endregion

		/// <summary>
		/// [Item count, Name, Datum type]
		/// </summary>
		/// <returns></returns>
		public override string ToString() { return string.Format("[{0}, {1}, {2}]", Items.Count, Name, typeof(T).Name); }

		#region Null Indices
		/// <summary>
		/// How many items in the Item dictionary are null
		/// </summary>
		int ItemNullCount = 0;

		/// <summary>
		/// Finds the next null item that can be re-used
		/// </summary>
		/// <returns>null if no null items</returns>
		/// <remarks>It is assumed that the index will be used after this is called</remarks>
		Blam.DatumIndex FindNextNull()
		{
			// make sure there is AT LEAST one null item
			if (ItemNullCount == 0) return Blam.DatumIndex.Null;

			ushort x = ushort.MaxValue;
			foreach(Element e in Items.Values)
				if (e.Data == null) { x = e.Header; break; }

			Blam.DatumIndex index = CalculateDatumIndex(Salt, x);
			ItemNullCount--; // making an ass out of me and you

			return index;
		}
		#endregion

		#region Ctor
		/// <summary>
		/// Constructs a new DataArray
		/// </summary>
		/// <remarks><see cref="Name"/> will equal "data array"</remarks>
		/// <param name="max_count">Max datum instances. Not actually enforced.</param>
		public DataArray(int max_count) : this(max_count, "data array") {}

		/// <summary>
		/// Constructs a new DataArray
		/// </summary>
		/// <param name="max_count">Max datum instances. Not actually enforced.</param>
		/// <param name="name">Name of this data array. Must be at least two characters in length.</param>
		/// <remarks>
		/// <paramref name="max_count"/> isn't enforced, but is used to initialize the underlying array object 
		/// with an initial count (to ease future potential memory allocations)
		/// </remarks>
		public DataArray(int max_count, string name)
		{
			Items = new Dictionary<BlamLib.Blam.DatumIndex, Element>(max_count, Blam.DatumIndex.kEqualityComparer);

			Name = name;
			Salt |= (short)(name[0] & 0xFF);
			unchecked { Salt <<= 8; }
			Salt |= (short)(name[1] & 0xFF);
		}
		#endregion

		#region Searching
		/// <summary>
		/// Searches for <paramref name="obj"/> in the index
		/// </summary>
		/// <param name="obj">Instance to find</param>
		/// <returns>null if it doesn't exist in this index</returns>
		public Blam.DatumIndex Find(T obj)
		{
			if(obj != null)
			{
				foreach (Element e in Items.Values)
					if (e.Data == obj) e.GetDatumIndex(Salt);
			}
			return Blam.DatumIndex.Null; // obj is null or item not found
		}
		#endregion

		#region Add
		/// <summary>
		/// Returns a handle that can be used for a new item entry in the index
		/// </summary>
		/// <param name="reused">Is the index being reused from our null items?</param>
		/// <returns></returns>
		Blam.DatumIndex NextIndex(out bool reused)
		{
			reused = false;
			Blam.DatumIndex index = new BlamLib.Blam.DatumIndex(0, Salt);

			if (ItemNullCount != 0)
			{
				index = FindNextNull();
				reused = true;
			}
			else
			{
				ushort count = (ushort)Items.Count;
				index.Index = count;
				unchecked { index.Salt += (short)count; } // since it doesn't equal '0' by default

				if (index.Salt == 0) index.Salt = -32768; // in case we overflow
			}

			return index;
		}

		/// <summary>
		/// Add a new <typeparamref name="T"/> object to the index
		/// and return its handle with the option of the actual
		/// object as well
		/// </summary>
		/// <param name="obj">Optional reference to a <typeparamref name="T"/> to be used in creation</param>
		/// <returns>Handle to the new <typeparamref name="T"/> object</returns>
		public Blam.DatumIndex Add(T obj)
		{
			bool reused;
			Blam.DatumIndex index = NextIndex(out reused);

			if (obj == null) obj = new T();

			Element e;
			if (!reused)
			{
				e = new Element(index.Index, obj);
				Items.Add(index, e);
			}
			else
			{
				e = Items[index];
				e.Data = obj;
			}

			e.AddReference();

			return index;
		}

		/// <summary>
		/// Add a new <typeparamref name="T"/> object to the index
		/// and return its handle
		/// </summary>
		/// <returns>Handle to the new <typeparamref name="T"/> object</returns>
		public Blam.DatumIndex Add() { return Add(null); }
		#endregion

		#region Move
		public event EventHandler<DataArrayEventArgsMove> EventMove;

		protected virtual void OnEventMove(DataArrayEventArgsMove args)
		{
			if (EventMove != null) EventMove(this, args);
		}

		public Blam.DatumIndex Move(DataArray<T> to, Blam.DatumIndex index)
		{
			Debug.Assert.If(index != Blam.DatumIndex.Null, "index != null");

			Blam.DatumIndex to_index = to.Add(this[index]);

			if (to_index != Blam.DatumIndex.Null)
			{
				OnEventMove(new DataArrayEventArgsMove(this, index, to, to_index));
				this.Close(index);
			}

			return index;
		}
		#endregion

		#region Remove
		bool RemoveImpl(Blam.DatumIndex index)
		{
			Element e = Items[index];
			if (e.RemoveReference() == 0)
			{
				ItemNullCount++;
				return true;
			}

			return false;
		}

		Blam.DatumIndex CloseImpl(Blam.DatumIndex index)
		{
			Element e = Items[index];
			e.Nullify();
			ItemNullCount++;

			return index;
		}

		/// <summary>
		/// Removes a reference to a <typeparamref name="T"/> by the handle 
		/// <paramref name="index"/> and sets that index to null if that
		/// was the last reference
		/// </summary>
		/// <param name="index"></param>
		/// <returns>true if set to null</returns>
		public bool Remove(Blam.DatumIndex index)
		{
			if(!Items.ContainsKey(index))
				Debug.Assert.If(false, "::Remove failed, {0} {1}", Name, index);

			return RemoveImpl(index);
		}

		/// <summary>
		/// Disposes of the <typeparamref name="T"/> object and removes it
		/// and sets that index to null
		/// </summary>
		/// <param name="index"></param>
		/// <returns>index of the closed element</returns>
		public Blam.DatumIndex Close(Blam.DatumIndex index)
		{
			if (!Items.ContainsKey(index))
				Debug.Assert.If(false, "::Close failed, {0} {1}", Name, index);

			return CloseImpl(index);
		}

		/// <summary>
		/// Finds <paramref name="obj"/> in the index and
		/// removes a reference to it, setting that index 
		/// to null if that was the last reference
		/// </summary>
		/// <param name="obj"></param>
		/// <returns>true if successful</returns>
		public bool Remove(T obj)
		{
			Blam.DatumIndex element_index = Find(obj);

			if(element_index == Blam.DatumIndex.Null)
				Debug.Assert.If(false, "::Remove failed, {0} {1}", Name, obj);

			return RemoveImpl(element_index);
		}

		/// <summary>
		/// Finds <paramref name="obj"/> in the index,
		/// disposes and removes it and sets that index to null
		/// </summary>
		/// <param name="obj"></param>
		/// <returns>index of the closed element</returns>
		public Blam.DatumIndex Close(T obj)
		{
			Blam.DatumIndex element_index = Find(obj);

			if(element_index == Blam.DatumIndex.Null)
				Debug.Assert.If(false, "::Close failed, {0} {1}", Name, obj);

			return CloseImpl(element_index);
		}

		/// <summary>
		/// Disposes and removes all datums data in the array
		/// and sets each index to be null
		/// </summary>
		public void CloseAll()
		{
			foreach (Blam.DatumIndex di in Items.Keys) CloseImpl(di);
		}
		#endregion

		#region Retrieve
		/// <summary>
		/// Figures out if the index is valid in this data array
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public bool Exists(Blam.DatumIndex index) { return Items.ContainsKey(index); }

		/// <summary>
		/// Retrieve a <typeparamref name="T"/> via its handle
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public T Get(Blam.DatumIndex index)
		{
			if (!Items.ContainsKey(index))
				Debug.Assert.If(false, "Datum doesn't exist. {0} {1}", Name, index);
			
			Element e = Items[index];

			if (e.GarbageCollect())
				Debug.Assert.If(false, "Tried to access an unusable datum. {0} {1}", Name, index);

			return e.Data;
		}

		// have to create an 'Impl' variant due to Element being a private class
		Element IncrementReferenceImpl(Blam.DatumIndex index)
		{
			if (index == Blam.DatumIndex.Null)
				throw new ArgumentNullException("index");

			Element e = Items[index];

			if (e.GarbageCollect())
				Debug.Assert.If(false, "Tried to access an unusable datum. {0} {1}", Name, index);

			e.AddReference();

			return e;
		}
		internal void IncrementReference(Blam.DatumIndex index) { IncrementReferenceImpl(index); }
		/// <summary>
		/// Retrieve a <typeparamref name="T"/> via its handle and increment the datum's reference count
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public T GetReference(Blam.DatumIndex index)
		{
			if (!Items.ContainsKey(index))
				Debug.Assert.If(false, "Datum doesn't exist. {0} {1}", Name, index);

			var e = IncrementReferenceImpl(index);

			return e.Data;
		}

		/// <summary>
		/// Retrieve a <typeparamref name="T"/> via its handle
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public T this[Blam.DatumIndex index] { get { return Get(index); } }
		#endregion
	};
}