using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using OpenSauceIDE.ModelExtractor.MessageHandler;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
    /// <summary>   An dictionary for passing data to an extraction job in a generic fashion. </summary>
    public class ExtractionData
        : Dictionary<string, object>
    { }

    /// <summary>   Manager for extraction jobs. </summary>
    public class ExtractionManager
        : IMessageSource
    {
        private List<IExtractionJob> mJobList = new List<IExtractionJob>();

        #region Messaging
        private MessageHandler.MessageHandler mMessageHandler = new MessageHandler.MessageHandler();

        /// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
        public event EventHandler<ModelExtractorMessageArgs> MessageSent
        {
            add { mMessageHandler.MessageSent += value; }
            remove { mMessageHandler.MessageSent -= value; }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Redirects a message to the managers message handler. </summary>
        ///
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        The ModelExtractorMessageArgs to process. </param>
        private void MessageRedirect(object sender, ModelExtractorMessageArgs e)
        {
            mMessageHandler.SendMessage(e.Message);
        }
        #endregion

        #region Job Handling
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Adds an extraction job to the manager. </summary>
        ///
        /// <param name="job">  The job to add. </param>
        public void Add(IExtractionJob job)
        {
            // Lock the list then add the job
            lock (mJobList)
            {
                mJobList.Add(job);
            }

            // Trigger the extraction thread so that it starts the job
            mRestartEvent.Set();
        }
        #endregion

        #region Extraction Thread
        private Thread mExtractionThread;
        private volatile bool mEndThread = false;
        private ManualResetEvent mRestartEvent = new ManualResetEvent(false);

        /// <summary>   Starts the extraction thread. </summary>
        public void Start()
        {
            mExtractionThread = new Thread(ExtractionLoop);
            mExtractionThread.Start();
        }

        /// <summary>   Stops the extraction thread. </summary>
        public void Stop()
        {
            mEndThread = true;
            mRestartEvent.Set();
            mExtractionThread.Join();
            mExtractionThread = null;
        }

        /// <summary>   The manager's extraction loop. </summary>
        private void ExtractionLoop()
        {
            while(!mEndThread)
            {
                // Lock the jobs list and see if any jobs are available
                bool jobsAvailable = true;
                lock (mJobList)
                {
                    if(mJobList.Count == 0)
                    {
                        jobsAvailable = false;

                        // If no jobs are available, set the thread to do periodic updates
                        mRestartEvent.Reset();
                    }
                }

                if (jobsAvailable)
                {
                    // Lock the jobs list then get the job
                    IExtractionJob extractionJob;
                    lock (mJobList)
                    {
                        extractionJob = mJobList[0];
                    }

                    mMessageHandler.SendMessage("Job started...");

                    // Attach message handlers then run the job
                    extractionJob.MessageSent += MessageRedirect;
                    extractionJob.Execute();
                    extractionJob.MessageSent -= MessageRedirect;

                    // Lock the jobs list then remove the job
                    lock (mJobList)
                    {
                        mJobList.RemoveAt(0);
                        mMessageHandler.SendMessage("Job complete...{0} jobs remaining", mJobList.Count);
                    }
                }

                // Wait for jobs to be added
                mRestartEvent.WaitOne(TimeSpan.FromSeconds(1));
            }
        }
        #endregion
    }
}
