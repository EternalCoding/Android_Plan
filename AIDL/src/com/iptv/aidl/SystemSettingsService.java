package com.iptv.aidl;

import com.iptv.aidl.ALIEthernetDevInfo;
import com.iptv.aidl.ALISystemSettings.Stub;
import android.app.Service;
import android.content.Intent;  
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.IBinder;  
import android.os.RemoteException;
//import android.alisdk.AliSettings;


public class SystemSettingsService extends Service{

	static {
		System.loadLibrary("adr_hld");
		System.loadLibrary("am_aidl");
	}
	
	@Override
	public IBinder onBind(Intent arg0) {
		return mbinder;
	}
	
//	private Alisettings msettings = new  Alisettings();
	private ALIEthernetDevInfo eth = new ALIEthernetDevInfo();
	
	public boolean SetValue(String key,String value){
		Context ctx = this;
		SharedPreferences sp = ctx.getSharedPreferences("Iptv_Data", MODE_PRIVATE);
		Editor editor = sp.edit();
	    editor.putString(key, value);
	    return editor.commit();
	}
	
	public String GetValue(String key,String defValue){
		Context ctx = this;
		SharedPreferences sp = ctx.getSharedPreferences("Iptv_Data", MODE_PRIVATE);
		return sp.getString(key, defValue);
	}
	
	private ALISystemSettings.Stub mbinder = new Stub() {
		
		
		public int getAudioMode() throws RemoteException {
			return 0;
		}
		public boolean setAudilMode(int flag) throws RemoteException{
			 return true;
			
		}
		
	    
		public int getResolution() throws RemoteException {
	    	return 0;
	    }
	    public boolean setResolution(int flag) throws RemoteException {
			return true;
		}
		
	    
		public boolean setEthernetDevInfo( ALIEthernetDevInfo eth) {
			return true;
		}
		public ALIEthernetDevInfo getEthernetDevInfo()throws RemoteException {
			return eth;
		}
		
		
		public boolean setValue(String key,String value) {
			return SetValue(key, value);
		}
		public String getValue(String key, String defValue) {
			return GetValue(key, defValue);
		}	
	};
	@Override 
	public void onCreate() { 
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
}
