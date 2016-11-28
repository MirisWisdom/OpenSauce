using System;
using System.ComponentModel;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	public class ProgressController
	{
		public delegate void FunctionDelegate(BackgroundWorker worker_thread, object arguments);

		#region Events
		public class ProgressControllerEventArgs : EventArgs
		{
			#region Properties
			public object UserState { get; private set; }
			#endregion

			#region Constructors
			ProgressControllerEventArgs() { }
			public ProgressControllerEventArgs(object user_state)
			{
				UserState = user_state;
			}
			#endregion
		}

		public event EventHandler<ProgressControllerEventArgs> WorkStarted;

		private void OnWorkStarted()
		{
			if (WorkStarted != null)
				WorkStarted(this, new ProgressControllerEventArgs(null));
		}

		public event EventHandler<ProgressControllerEventArgs> WorkEnded;

		private void OnWorkEnded()
		{
			if (WorkEnded != null)
				WorkEnded(this, new ProgressControllerEventArgs(null));
		}

		public event EventHandler<ProgressControllerEventArgs> WorkProgressed;

		private void OnWorkProgressed(object user_state)
		{
			if (WorkProgressed != null)
				WorkProgressed(this, new ProgressControllerEventArgs(user_state));
		}
		#endregion

		#region Fields
		BackgroundWorker m_worker = new BackgroundWorker();
		FunctionDelegate m_function_delegate = null;
		#endregion

		#region Constructor
		public ProgressController(FunctionDelegate function, object user_parameter)
		{
			m_worker.WorkerReportsProgress = true;
			m_worker.DoWork += new DoWorkEventHandler(DoWork);
			m_worker.ProgressChanged += new ProgressChangedEventHandler(ProgressChanged);
			m_worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(RunWorkerCompleted);
			m_function_delegate = function;
		}
		#endregion

		void ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			OnWorkProgressed(e.UserState);
		}

		void DoWork(object sender, DoWorkEventArgs e)
		{
			if (m_function_delegate == null) throw new NotImplementedException();

			m_function_delegate(m_worker, e.Argument);
		}

		void RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			OnWorkEnded();
		}

		public void Start(object arguments)
		{
			m_worker.RunWorkerAsync(arguments);

			OnWorkStarted();
		}
	}
}
