package com.iptv.aidl;

import com.iptv.aidl.ALIDataServiceCfg;
import com.iptv.aidl.ALIDataServiceCfg.Stub;
import android.app.Service;
import android.content.Intent;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.IBinder;
import android.os.RemoteException;

public class DataServiceCfgService extends Service{

	@Override
	public IBinder onBind(Intent arg0) {
		return mbinder;
	}
	
	/**
	 * IPTV SharePerfernces
	 */
	public boolean Put_SP(String key, String value1, int value2, boolean value3, int flag){
		Context ctx = this;
		SharedPreferences sp = ctx.getSharedPreferences("Iptv_Data", MODE_PRIVATE);
		Editor editor = sp.edit();
		switch (flag) {
		case 1: editor.putString(key, value1);
				break;
		case 2: editor.putInt(key, value2);
				break;
		case 3: editor.putBoolean(key, value3);
				break;
		}
		return editor.commit();
	}

	public String GetString_SP(String key, String defValue){
		Context ctx = this;
		SharedPreferences sp = ctx.getSharedPreferences("Iptv_Data", MODE_PRIVATE);
		return sp.getString(key, defValue);
	}
	public int GetInt_SP(String key, int defValue){
		Context ctx = this;
		SharedPreferences sp = ctx.getSharedPreferences("Iptv_Data", MODE_PRIVATE);
		return sp.getInt(key, defValue);
	}
	public boolean GetBoolean_SP(String key, boolean defValue){
		Context ctx = this;
		SharedPreferences sp = ctx.getSharedPreferences("Iptv_Data", MODE_PRIVATE);
		return sp.getBoolean(key, defValue);
	}
	/**
	 * AIDL
	 */
	private ALIDataServiceCfg.Stub mbinder = new Stub() {

		@Override                      
		public boolean putString(String key, String value)
				throws RemoteException {
			return  Put_SP(key,value,0,false,1); 
		}

		@Override
		public boolean putInt(String key, int value) throws RemoteException {
			return Put_SP(key, null, value, false, 2);
		}

		@Override
		public boolean putBoolean(String key, boolean value)
				throws RemoteException {
			return Put_SP(key, null, 0, value, 3);
		}

		@Override
		public String getString(String key, String defValue)
				throws RemoteException {
			return GetString_SP(key,defValue);
		}

		@Override
		public int getInt(String key, int defValue) throws RemoteException {
			return GetInt_SP(key, defValue);
		}

		@Override
		public boolean getBoolean(String key, boolean defValue)
				throws RemoteException {
			return GetBoolean_SP(key, defValue);
		}
	};   

}