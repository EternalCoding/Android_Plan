package com.androidtvcontroller;

import android.util.Log;

public class AliLowlevel {
	static AliLowlevel instance = null;
	private boolean sLoaded = false; 
	public static final String TAG = "LowLevelCall";

	public native boolean Init();
	public native void UnInit();
	public native int Scan();
	public native boolean ConnectStb(int index);
	public native boolean DisConnectStb();
	public native boolean SendKaypad(int keyval);
	public native boolean SendSynData();
	public native int GetEpgList();
	public native String GetEpgChannel();
	public native String GetEpgLang(int langidx);
	public native String GetEpgDate(int day);
	public native int GetEpgLangNum();
	public native int GetEpgNumByIndex(int index);
	public native String GetEpgEventName(int day, int index, int lang);
	public native String GetEpgEventTime(int day, int index);
	public native String GetEpgEventDetail(int day, int index, int lang);
	public native String GetMacAddress(int item);

	public boolean loadLibs()
	{
		if(sLoaded )
		{
			return true;
		}
		boolean err = false;
		try
		{
			System.loadLibrary("netcall");
		}
		catch(UnsatisfiedLinkError e)
		{
			Log.d(TAG, "Couldn't load lib: " + "jnidec" + e.getMessage());
			err = true;
		}

		if(!err)
		{
			sLoaded  = true;
		}
		return sLoaded;
	}

	public AliLowlevel()
    {
		loadLibs();
		instance = this;
    }
}
