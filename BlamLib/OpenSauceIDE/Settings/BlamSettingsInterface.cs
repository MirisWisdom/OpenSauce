using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Managers;

namespace OpenSauceIDE.Settings
{
	//public class BlamSettingsObject
	//	: SettingsObjectBase
	//	, BlamDefinition.IBlamSettingsInterface
	//{ }

	//public class BlamSettingsManager
	//{
	//	private Dictionary<BlamLib.BlamVersion, BlamSettingsObject> mBlamSettingsObjects = new Dictionary<BlamLib.BlamVersion, BlamSettingsObject>();

	//	#region Singleton
	//	static private BlamSettingsManager mInstance = new BlamSettingsManager();

	//	static BlamSettingsManager Instance
	//	{
	//		get
	//		{
	//			return mInstance;
	//		}
	//	}

	//	private BlamSettingsManager()
	//	{
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.Halo1, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.Halo1", typeof(BlamSettingsObject)));
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.Halo2, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.Halo2", typeof(BlamSettingsObject)));
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.Halo3, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.Halo3", typeof(BlamSettingsObject)));
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.HaloOdst, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.HaloOdst", typeof(BlamSettingsObject)));
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.HaloReach, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.HaloReach", typeof(BlamSettingsObject)));
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.Halo4, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.Halo4", typeof(BlamSettingsObject)));
	//		mBlamSettingsObjects.Add(BlamLib.BlamVersion.Stubbs, (BlamSettingsObject)SettingsManager.GetSettings("OpenSauceIDE.Blam.VersionVariables.Stubbs", typeof(BlamSettingsObject)));
	//	}
	//	#endregion

	//	public BlamDefinition.IBlamSettingsInterface GetSettingsInterface(BlamLib.BlamVersion blamVersion)
	//	{
	//		return mBlamSettingsObjects[blamVersion];
	//	}
	//}
}
