/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Linq;
using System.Linq.Expressions;
using System.Threading;

namespace BlamLib
{
	/// <summary>
	/// Standard global utility classes and functions
	/// </summary>
	public static partial class Util
	{
		#region Flags
		/// <summary>
		/// 
		/// </summary>
		/// <remarks>
		/// This is a class so we can effectively use it as a property. 
		/// If it was treated as a struct doing <code>[obj].Flags.Add(1);</code>
		/// wouldn't affect [obj]'s Flags property
		/// </remarks>
		public sealed class Flags : IO.IStreamable
		{
			uint Value;
			/// <summary>
			/// Reset the internal value with another
			/// </summary>
			/// <param name="value"></param>
			public void Reset(uint value) { Value = value; }
			public void Reset(params uint[] values) { foreach (uint f in values) Add(f); }

			public static implicit operator uint(Flags value) { return value.Value; }

			public Flags(uint value) { Reset(value); }

			public Flags(params uint[] values) { Reset(values); }

			/// <summary>
			/// Tests if this.Value has <paramref name="flag"/>
			/// </summary>
			/// <param name="flag">flag to test</param>
			/// <returns>True if <paramref name="flag"/> is set</returns>
			public bool Test(uint flag) { return (Value & flag) != 0; }

			/// <summary>
			/// Adds <paramref name="flags"/> from this.Value
			/// </summary>
			/// <param name="flags">flags to add</param>
			public void Add(uint flags) { Value |= flags; }

			/// <summary>
			/// Adds <paramref name="flags"/> when <paramref name="cond"/> is true
			/// </summary>
			/// <param name="cond">condition to use to determine when to set flags</param>
			/// <param name="flags">flags to add</param>
			/// <returns><paramref name="cond"/>'s value</returns>
			public bool Add(bool cond, uint flags) { if (cond) Add(flags); return cond; }

			/// <summary>
			/// Removes <paramref name="flags"/> from this.Value
			/// </summary>
			/// <param name="flags">flags to remove</param>
			public void Remove(uint flags) { Value &= ~flags; }

			/// <summary>
			/// Removes <paramref name="flags"/> when <paramref name="cond"/> is true
			/// </summary>
			/// <param name="cond">condition to use to determine when to remove flags</param>
			/// <param name="flags">flags to remove</param>
			/// <returns><paramref name="cond"/>'s value</returns>
			public bool Remove(bool cond, uint flags) { if (cond) Remove(flags); return cond; }

			public override bool Equals(object obj) { return obj is Flags && (obj as Flags).Value == Value; }
			public override int GetHashCode() { return Value.GetHashCode(); }
			public override string ToString() { return Value.ToString(); }

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)		{ Value = s.ReadUInt32(); }
			public void Write(BlamLib.IO.EndianWriter s)	{ s.Write(Value); }
			#endregion

			#region Util
			/// <summary>
			/// Adds <paramref name="flags"/> from <paramref name="value"/>
			/// </summary>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			/// <returns></returns>
			public static uint Add(uint value, uint flags) { return value |= flags; }
			/// <summary>
			/// Adds <paramref name="flags"/> from <paramref name="value"/>
			/// </summary>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			public static void Add(ref uint value, uint flags) { value |= flags; }

			/// <summary>Adds <paramref name="rhs"/> to <paramref name="lhs"/></summary>
			/// <param name="lhs">Existing bit-vector</param>
			/// <param name="rhs">Other bit-vector whose bits we wish to add to <paramref name="lhs"/></param>
			/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
			public static ulong Add(ulong lhs, ulong rhs)		{ return lhs |= rhs; }
			/// <summary>Adds <paramref name="rhs"/> to <paramref name="lhs"/></summary>
			/// <param name="lhs">Existing bit-vector reference</param>
			/// <param name="rhs">Other bit-vector whose bits we wish to add to <paramref name="lhs"/></param>
			public static void Add(ref ulong lhs, ulong rhs)	{ lhs |= rhs; }


			/// <summary>
			/// Removes <paramref name="flags"/> from <paramref name="value"/>
			/// </summary>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			/// <returns></returns>
			public static uint Remove(uint value, uint flags) { return value &= ~flags; }
			/// <summary>
			/// Removes <paramref name="flags"/> from <paramref name="value"/>
			/// </summary>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			public static void Remove(ref uint value, uint flags) { value &= ~flags; }

			/// <summary>Removes <paramref name="rhs"/> from <paramref name="lhs"/></summary>
			/// <param name="lhs">Existing bit-vector</param>
			/// <param name="rhs">Other bit-vector whose bits we wish to remove from <paramref name="lhs"/></param>
			/// <returns><paramref name="lhs"/> &= <paramref name="rhs"/></returns>
			public static ulong Remove(ulong lhs, ulong rhs)	{ return lhs &= ~rhs; }
			/// <summary>Removes <paramref name="rhs"/> from <paramref name="lhs"/></summary>
			/// <param name="lhs">Existing bit-vector</param>
			/// <param name="rhs">Other bit-vector whose bits we wish to remove from <paramref name="lhs"/></param>
			public static void Remove(ref ulong lhs, ulong rhs)	{ lhs &= ~rhs; }


			/// <summary>
			/// Modify <paramref name="value"/> with <paramref name="flags"/>
			/// </summary>
			/// <param name="add_or_remove">True to add <paramref name="flags"/>, false to remove</param>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			/// <returns></returns>
			public static ushort Modify(bool add_or_remove, ushort value, ushort flags)
			{
				return (add_or_remove == true ?
					value |= flags :
					value &= (ushort)~flags);
			}
			/// <summary>
			/// Modify <paramref name="value"/> with <paramref name="flags"/>
			/// </summary>
			/// <param name="add_or_remove">True to add <paramref name="flags"/>, false to remove</param>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			public static void Modify(bool add_or_remove, ref ushort value, ushort flags)
			{
				if (add_or_remove)
					value |= flags;
				else
					value &= (ushort)~flags;
			}
			/// <summary>
			/// Modify <paramref name="value"/> with <paramref name="flags"/>
			/// </summary>
			/// <param name="add_or_remove">True to add <paramref name="flags"/>, false to remove</param>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			/// <returns></returns>
			public static uint Modify(bool add_or_remove, uint value, uint flags)
			{
				return (add_or_remove == true ?
					value |= flags : 
					value &= ~flags);
			}
			/// <summary>
			/// Modify <paramref name="value"/> with <paramref name="flags"/>
			/// </summary>
			/// <param name="add_or_remove">True to add <paramref name="flags"/>, false to remove</param>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			public static void Modify(bool add_or_remove, ref uint value, uint flags)
			{
				if (add_or_remove == true)
					value |= flags;
				else
					value &= ~flags;
			}

			/// <summary>
			/// Returns true if <paramref name="flag"/> is active in <paramref name="value"/>
			/// </summary>
			/// <param name="value">Value to test in</param>
			/// <param name="flag">Value to test for</param>
			/// <returns></returns>
			public static bool Test(short value, short flag) { return (value & flag) != 0; }

			/// <summary>
			/// Returns true if <paramref name="flag"/> is active in <paramref name="value"/>
			/// </summary>
			/// <param name="value">Value to test in</param>
			/// <param name="flag">Value to test for</param>
			/// <returns></returns>
			public static bool Test(ushort value, ushort flag) { return (value & flag) != 0; }

			/// <summary>
			/// Returns true if <paramref name="flag"/> is active in <paramref name="value"/>
			/// </summary>
			/// <param name="value">Value to test in</param>
			/// <param name="flag">Value to test for</param>
			/// <returns></returns>
			public static bool Test(int value, int flag) { return (value & flag) != 0; }

			/// <summary>
			/// Returns true if <paramref name="flag"/> is active in <paramref name="value"/>
			/// </summary>
			/// <param name="value">Value to test in</param>
			/// <param name="flag">Value to test for</param>
			/// <returns></returns>
			public static bool Test(uint value, uint flag) { return (value & flag) != 0; }

			/// <summary>
			/// Returns true if all the flags in <paramref name="flags"/> are active in <paramref name="value"/>
			/// </summary>
			/// <param name="value">Value to test in</param>
			/// <param name="flags">Values to test for</param>
			/// <returns></returns>
			public static bool Test(uint value, params uint[] flags)
			{
				bool ret = false;
				foreach (uint i in flags)
					ret = ret & Test(value, i);
				return ret;
			}

			/// <summary>
			/// Returns true if any one of the flags in <paramref name="flags"/> are active in <paramref name="value"/>
			/// </summary>
			/// <param name="value"></param>
			/// <param name="flags"></param>
			/// <returns></returns>
			public static bool TestAny(uint value, params uint[] flags)
			{
				foreach (uint i in flags)
					if (Test(value, i)) return true;

				return false;
			}
			#endregion
		};
		#endregion

		#region optional_value
		public struct OptionalValue
		{
			private readonly int
				k_count,
				k_shift;
			public readonly uint k_mask;

			public OptionalValue(byte option_count)
			{
				option_count &= 0xDF; // make sure optional count is 32 or less
				k_count = option_count - 1;
				k_shift = 32 - k_count;
				k_mask = (uint)~(k_count << k_shift);
			}

			public uint GenerateData(byte option, uint value)	{ return (uint)( (value & k_mask) | (uint)((option << k_shift)) ); }
			public byte GetOption(uint data)					{ return (byte)((data >> k_shift) & 0xFF); }
			public uint GetValue(uint data)						{ return data & k_mask; }
		};
		#endregion

		#region Zlib util
		static byte[] ZLibBufferFromStream(System.IO.Compression.DeflateStream dec, int offset, int size)
		{
			byte[] result;

			// adjust for zlib header
			dec.BaseStream.Seek(offset + sizeof(ushort), System.IO.SeekOrigin.Begin);

			// decompress the data and fill in the result array
			result = new byte[size];
			dec.Read(result, 0, result.Length);

			return result;
		}
		public static byte[] ZLibBufferFromStream(System.IO.MemoryStream ms, int offset, int size)
		{
			using (var dec = new System.IO.Compression.DeflateStream(ms, System.IO.Compression.CompressionMode.Decompress, true))
			{
				return ZLibBufferFromStream(dec, offset, size);
			}
		}
		public static byte[] ZLibBufferFromBytes(byte[] bytes, int offset, int size)
		{
			using(var ms = new System.IO.MemoryStream(bytes))
			using (var dec = new System.IO.Compression.DeflateStream(ms, System.IO.Compression.CompressionMode.Decompress, false))
			{
				return ZLibBufferFromStream(dec, offset, size);
			}
		}
		#endregion


		#region AsyncOperation
		/// <summary>
		/// Base class designed to be used by lengthy operations that wish to 
		/// support cancellation.  It also allows those operations to invoke delegates
		/// on the UI Thread of a hosting control.
		/// </summary>
		public abstract class AsyncOperation
		{
			/// <summary>
			/// Exception thrown by AsyncOperation.Start when an operation is already in progress.
			/// </summary>
			public class AlreadyRunningException : ApplicationException { public AlreadyRunningException() : base("Asynchronous operation already running") { } };

			#region Events
			/// <summary>
			/// This event will be fired if the operation runs to completion without being cancelled. 
			/// This event will be raised through the ISynchronizeTarget supplied at construction time. 
			/// Note that this event may still be received after a cancellation request has been issued.
			/// (This would happen if the operation completed at about the same time that cancellation was 
			/// requested.) But the event is not raised if the operation is cancelled successfully.
			/// </summary>
			public event EventHandler Completed;
			/// <summary>
			/// This event will be fired when the operation is successfully stopped through cancellation.
			/// This event will be raised through the ISynchronizeTarget supplied at construction time.
			/// </summary>
			public event EventHandler Cancelled;
			/// <summary>
			/// This event will be fired if the operation throws an exception. This event will be raised 
			/// through the ISynchronizeTarget supplied at construction time.
			/// </summary>
			public event ThreadExceptionEventHandler Failed;
			#endregion

			#region Properties
			protected System.ComponentModel.ISynchronizeInvoke target;

			private bool flagFailed; // Set to true if the operation fails with an exception.
			private bool isRunning; // Set to true if the operation is running

			#region CancelRequested
			private bool flagCancelled;
			/// <summary>
			/// Flag indicating whether the request has been cancelled.  Long-running operations 
			/// should check this flag regularly if they can and cancel their operations as soon 
			/// as they notice that it has been set.
			/// </summary>
			protected bool CancelRequested { get { lock (this) { return flagCancelled; } } }
			#endregion

			#region HasCompleted
			private bool flagCompleted;
			/// <summary>
			/// Flag indicating whether the request has run through to completion. 
			/// This will be false if the request has been successfully cancelled, or if it failed.
			/// </summary>
			protected bool HasCompleted { get { lock (this) { return flagCompleted; } } }
			#endregion

			/// <summary>
			/// Returns false if the operation is still in progress, or true if it has either 
			/// completed successfully, been cancelled successfully, or failed with an internal exception.
			/// </summary>
			public bool IsDone { get { lock (this) { return flagCompleted || flagCancelAcknowledged || flagFailed; } } }
			#endregion

			/// <summary>
			/// Initializes an AsyncOperation with an association to the
			/// supplied ISynchronizeInvoke.  All events raised from this
			/// object will be delivered via this target.  (This might be a
			/// Control object, so events would be delivered to that Control's
			/// UI thread.)
			/// </summary>
			/// <param name="target">An object implementing the
			/// ISynchronizeInvoke interface.  All events will be delivered
			/// through this target, ensuring that they are delivered to the
			/// correct thread.</param>
			public AsyncOperation(System.ComponentModel.ISynchronizeInvoke target)
			{
				this.target = target;
				isRunning = false;
			}

			private bool flagCancelAcknowledged;
			/// <summary>
			/// This is called by the operation when it wants to indicate that it saw the cancellation request and honored it.
			/// </summary>
			protected void AcknowledgeCancel()
			{
				lock(this)
				{
					flagCancelAcknowledged = true;
					isRunning = false;

					// Pulse the event in case the main thread is blocked
					// waiting for us to finish (e.g. in CancelAndWait or
					// WaitUntilDone).
					Monitor.Pulse(this);

					// Using async invocation to avoid a potential deadlock
					// - using Invoke would involve a cross-thread call
					// whilst we still held the object lock.  If the event
					// handler on the UI thread tries to access this object
					// it will block because we have the lock, but using
					// async invocation here means that once we've fired
					// the event, we'll run on and release the object lock,
					// unblocking the UI thread.
					FireAsync(Cancelled, this, EventArgs.Empty);
				}
			}


			/// <summary>
			/// To be overridden by the deriving class - this is where the work will be done. 
			/// The base class calls this method on a worker thread when the Start method is called.
			/// </summary>
			protected abstract void DoWork();

			/// <summary>
			/// This is called when the operation runs to completion.
			/// </summary>
			/// <remarks>
			/// This is private because it is called automatically by this base class when the 
			/// deriving class's DoWork method exits without having cancelled
			/// </remarks>
			private void CompleteOperation()
			{
				lock(this)
				{
					flagCompleted = true;
					isRunning = false;
					Monitor.Pulse(this);
					FireAsync(Completed, this, EventArgs.Empty);
				}
			}

			private void FailOperation(Exception e)
			{
				lock(this)
				{
					flagFailed = true;
					isRunning = false;
					Monitor.Pulse(this);
					FireAsync(Failed, this, new ThreadExceptionEventArgs(e));
				}
			}

			/// <summary>
			/// Utility function for firing an event through the target.
			/// </summary>
			/// <param name="dlg"></param>
			/// <param name="args"></param>
			/// <remarks>
			/// This functions presumes that the caller holds the object lock. (This is because the event list is 
			/// typically modified on the UI thread, but events are usually raised on the worker thread.)
			/// </remarks>
			protected void FireAsync(Delegate dlg, params object[] args)
			{
				if (dlg != null) target.BeginInvoke(dlg, args);
			}

			/// <summary>
			/// This method is called on a worker thread (via asynchronous delegate invocation). 
			/// This is where we call the operation (as defined in the deriving class's DoWork method).
			/// </summary>
			private void InternalStart()
			{
				// Reset our state - we might be run more than once.
				// isRunning is set during Start to avoid a race condition
				flagCancelled = false;
				flagCompleted = false;
				flagCancelAcknowledged = false;
				flagFailed = false;

				try { DoWork(); }
				catch(Exception e)
				{
					// Raise the Failed event.  We're in a catch handler, so we
					// had better try not to throw another exception.
					try { }
					catch { }

					// The documentation recommends not catching
					// SystemExceptions, so having notified the caller we
					// re-throw if it was one of them.
					if (e is SystemException) throw;
				}

				lock (this)
				{
					// If the operation wasn't cancelled (or if the UI thread
					// tried to cancel it, but the method ran to completion
					// anyway before noticing the cancellation) and it
					// didn't fail with an exception, then we complete the
					// operation - if the UI thread was blocked waiting for
					// cancellation to complete it will be unblocked, and
					// the Completion event will be raised.
					if (!flagCancelAcknowledged && !flagFailed) CompleteOperation();
				}
			}

			/// <summary>
			/// Launch the operation on a worker thread. This method will return immediately, and 
			/// the operation will start asynchronously on a worker thread.
			/// </summary>
			public void Start()
			{
				lock(this)
				{
					if (isRunning) throw new AlreadyRunningException();
					// Set this flag here, not inside InternalStart, to avoid
					// race condition when Start called twice in quick
					// succession.
					isRunning = true;
				}
				new System.Windows.Forms.MethodInvoker(InternalStart).BeginInvoke(null, null);
			}

			/// <summary>
			/// Attempt to cancel the current operation.
			/// </summary>
			/// <remarks>
			/// This returns immediately to the caller. No guarantee is made as to whether the 
			/// operation will be successfully cancelled. All that can be known is that at some 
			/// point, one of the three events Completed, Cancelled, or Failed will be raised
			/// at some point.
			/// </remarks>
			public virtual void Cancel()	{ lock (this) { flagCancelled = true; } }

			/// <summary>
			/// Attempt to cancel the current operation and block until either
			/// the cancellation succeeds or the operation completes.
			/// </summary>
			/// <returns>
			/// True if the operation was successfully cancelled or it failed, false if it ran to completion.
			/// </returns>
			public bool CancelAndWait()
			{
				lock(this)
				{
					flagCancelled = true;

					// Now sit and wait either for the operation to
					// complete or the cancellation to be acknowledged.
					// (Wake up and check every second - shouldn't be
					// necessary, but it guarantees we won't deadlock
					// if for some reason the Pulse gets lost - means
					// we don't have to worry so much about bizarre
					// race conditions.)
					while (!IsDone) Monitor.Wait(this, 1000);
				}
				return !HasCompleted;
			}

			/// <summary>
			/// Blocks until the operation has either run to completion, or has been successfully cancelled, 
			/// or has failed with an internal exception.
			/// </summary>
			/// <returns>
			/// True if the operation completed, false if it was cancelled before completion or 
			/// failed with an internal exception.
			/// </returns>
			public bool WaitUntilDone()
			{
				lock (this)
				{
					// Wait for either completion or cancellation.  As with
					// CancelAndWait, we don't sleep forever - to reduce the
					// chances of deadlock in obscure race conditions, we wake
					// up every second to check we didn't miss a Pulse.
					while (!IsDone) Monitor.Wait(this, 1000);
				}
				return HasCompleted;
			}
		};
		#endregion

		#region ProcessCaller
		/// <summary>
		/// This class can launch a process (like a bat file, perl
		/// script, etc) and return all of the StdOut and StdErr
		/// to GUI app for display in textboxes, etc.
		/// </summary>
		/// <remarks>
		/// This class (c) 2003 Michael Mayer
		/// Use it as you like (public domain licensing).
		/// Please post any bugs / fixes to the page where
		/// you downloaded this code.
		/// </remarks>
		public class ProcessCaller : AsyncOperation
		{
			/// <summary>
			/// Data received event handler arguments
			/// </summary>
			public class DataReceivedEventArgs : EventArgs
			{
				/// <summary>
				/// The data that was received
				/// </summary>
				public string Text;

				/// <summary>
				/// </summary>
				/// <param name="text">The data that was received for this event to be triggered.</param>
				public DataReceivedEventArgs(string text) { Text = text; }
			};

			#region Trace
			/// <summary>
			/// ProcessCaller tracing log file reference
			/// </summary>
			/// <remarks>Used for logging process output</remarks>
			private static Debug.Trace trace;

			/// <summary>
			/// Open the ProcessCaller tracing log file
			/// </summary>
			[System.Diagnostics.Conditional("DEBUG")]
			static void OpenLog()
			{
				trace = new BlamLib.Debug.Trace("Process Output", "");
			}

			static ProcessCaller() { OpenLog(); }

			/// <summary>
			/// Close the ProcessCaller tracing log file
			/// </summary>
			[System.Diagnostics.Conditional("DEBUG")]
			public static void CloseLog()
			{
				if (trace == null)
					System.Windows.Forms.MessageBox.Show(
						"CloseLog: Process Output.txt not open!",
						"Whoops");
				else
				{
					trace.CloseLog();
					trace = null;
				}
			}

			static void DefaultErrorDataReceivedHandler(object sender, DataReceivedEventArgs e)
			{
				ProcessCaller p = (ProcessCaller)sender;
				trace.WriteLine("Error from {0}:", p.FileName);
				trace.WriteLine(e.Text);
			}

			static void DefaultOutputDataReceivedHandler(object sender, DataReceivedEventArgs e)
			{
				ProcessCaller p = (ProcessCaller)sender;
				trace.WriteLine("Output from {0}:", p.FileName);
				trace.WriteLine(e.Text);
			}

			public void UseDefaultDataReceivedHandler()
			{
				StdErrReceived += new EventHandler<DataReceivedEventArgs>(DefaultErrorDataReceivedHandler);
				StdOutReceived += new EventHandler<DataReceivedEventArgs>(DefaultOutputDataReceivedHandler);
			}
			#endregion

			#region Events
			/// <summary>
			/// Fired for every line of StdOut received.
			/// </summary>
			public event EventHandler<DataReceivedEventArgs> StdOutReceived;
			/// <summary>
			/// Fired for every line of stdErr received.
			/// </summary>
			public event EventHandler<DataReceivedEventArgs> StdErrReceived;
			#endregion

			#region Properties
			private System.Diagnostics.Process process;

			/// <summary>
			/// Amount of time to sleep on threads while waiting
			/// for the process to finish.
			/// </summary>
			private int sleepTime = 500;

			#region FileName
			private string fileName;
			/// <summary>
			/// The command to run
			/// </summary>
			public string FileName
			{
				get { return fileName; }
				set { fileName = value; }
			}
			#endregion

			#region Arguments
			private string arguments;
			/// <summary>
			///  The Arguments for the command
			/// </summary>
			public string Arguments
			{
				get { return arguments; }
				set { arguments = value; }
			}
			#endregion

			#region WorkingDirectory
			private string workingDir;
			/// <summary>
			/// The command's working directory
			/// </summary>
			public string WorkingDirectory
			{
				get { return workingDir; }
				set { workingDir = value; }
			}
			#endregion

			#endregion

			/// <summary>
			/// Initializes a ProcessCaller with an association to the
			/// supplied ISynchronizeInvoke.  All events raised from this
			/// object will be delivered via this target.  (This might be a
			/// Control object, so events would be delivered to that Control's
			/// UI thread.)
			/// </summary>
			/// <param name="target">An object implementing the
			/// ISynchronizeInvoke interface.  All events will be delivered
			/// through this target, ensuring that they are delivered to the
			/// correct thread.</param>
			public ProcessCaller(System.ComponentModel.ISynchronizeInvoke target) : base(target)
			{
			}

			public ProcessCaller(System.ComponentModel.ISynchronizeInvoke target, int sleep_time) : base(target)
			{
				sleepTime = sleep_time;
			}

			/// <summary>
			/// Launch a process, but do not return until the process has exited.
			/// That way we can kill the process if a cancel is requested.
			/// </summary>
			protected override void DoWork()
			{
				StartProcess();

				// Wait for the process to end, or cancel it
				while (!process.HasExited)
				{
					Thread.Sleep(sleepTime); // sleep
					if (CancelRequested)
					{
						// Not a very nice way to end a process,
						// but effective.
						process.Kill();
						AcknowledgeCancel();
					}
				}
			}

			/// <summary>
			/// This method is generally called by DoWork()
			/// which is called by the base class Start()
			/// </summary>
			protected virtual void StartProcess()
			{
				// Start a new process for the cmd
				process = new System.Diagnostics.Process();
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.RedirectStandardError = true;
				//process.StartInfo.RedirectStandardInput = true;
				//process.StandardInput.AutoFlush = true;
				process.StartInfo.CreateNoWindow = true;
				process.StartInfo.FileName = fileName;
				process.StartInfo.Arguments = arguments;
				process.StartInfo.WorkingDirectory = workingDir;
				process.Start();


				// Invoke stdOut and stdErr readers - each
				// has its own thread to guarantee that they aren't
				// blocked by, or cause a block to, the actual
				// process running (or the gui).
				new System.Windows.Forms.MethodInvoker(ReadStdOut).BeginInvoke(null, null);
				new System.Windows.Forms.MethodInvoker(ReadStdErr).BeginInvoke(null, null);
			}

			/// <summary>
			/// Handles reading of stdout and firing an event for
			/// every line read
			/// </summary>
			protected virtual void ReadStdOut()
			{
				string str;
				while ((str = process.StandardOutput.ReadLine()) != null)
				{
					//if(process.StandardInput.BaseStream.CanWrite)
					//	MessageBox.Show("Whoops", "Seems that the process wants some attention");
					process.StandardOutput.BaseStream.Flush();

					FireAsync(StdOutReceived, this, new DataReceivedEventArgs(str));
				}
			}

			/// <summary>
			/// Handles reading of stdErr
			/// </summary>
			protected virtual void ReadStdErr()
			{
				string str;
				while ((str = process.StandardError.ReadLine()) != null)
				{
					process.StandardError.BaseStream.Flush();
					FireAsync(StdErrReceived, this, new DataReceivedEventArgs(str));
				}
			}

			public virtual void WriteStdIn(string input)
			{
				try
				{
					process.StandardInput.Write(input);
					process.StandardInput.Write('\r');
				}
				catch
				{
				}
			}
		};
		#endregion
	};
}