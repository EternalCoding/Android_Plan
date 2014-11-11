package com.iptv.aidl;

import com.iptv.aidl.ALIAudioMode.Stub;
import com.iptv.aidl.ALIEthernetDevInfo;
import com.iptv.aidl.ALIAudioMode;
import android.app.Service;
import android.content.Intent;  
import android.os.IBinder;  
import android.os.RemoteException; 


public class ALIAudioModeService extends Service {
	
	static {
		System.loadLibrary("adr_hld");
		System.loadLibrary("am_aidl");
	}
	@Override
	public IBinder onBind(Intent arg0) {
		return mbinder;
	}
	
	public native int jnigetAudioMode();
	public native int jnisetAudioMode(int flag);
	
	private ALIAudioMode.Stub mbinder = new Stub(){
	
		public int getAudioMode() throws RemoteException{
			return  jnigetAudioMode(); 
		}
		
		public int setAudioMode(int flag) throws RemoteException{
			return jnisetAudioMode(flag); 
		}
		
		public String sayhello() {
			return "test!!";
		}
	};


}
