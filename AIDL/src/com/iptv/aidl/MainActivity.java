package com.iptv.aidl;

import com.iptv.aidl.ALIAudioMode;
import com.iptv.aidl.ALIDataServiceCfg;
import android.app.Activity;  
import android.content.ComponentName;  
import android.content.Intent;  
import android.content.ServiceConnection;  
import android.os.Bundle;  
import android.os.IBinder;  
import android.os.RemoteException;  
import android.view.View;  
import android.view.View.OnClickListener;  
import android.widget.Button;  
import android.widget.EditText;
import android.widget.TextView;


public class MainActivity extends Activity  implements OnClickListener{

	private TextView mtv;
	private Button btngetAM;
	private EditText met;
	private Button btnsetAM;
	
	private Button btninsert;
	private Button btnqurey;
	
	public ALIEthernetDevInfo DevInfo;
	private TextView devinfo;
	
	public int flag = -1;
	
	private ALIAudioMode AMService = null;
    private ALIDataServiceCfg dataservice = null;
  
    /**
     * 获取AM模式
     */
	private ServiceConnection mConnection_getAM = new ServiceConnection() {

		@Override
		public void onServiceDisconnected(ComponentName name) {
		    //mService = null;
			dataservice = null;
		}
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			//mService = ALIAudioMode.Stub.asInterface(service); 
			dataservice = ALIDataServiceCfg.Stub.asInterface(service); 
			try {
				/*
				 switch (mService.getAudioMode())
				 {
				 	case 0: mtv.setText("SND_OUT_SPDIF_PCM");
				 	  		break;
				 	case 1: mtv.setText("SND_OUT_SPDIF_BS");
				 			break;
				 	case 2: mtv.setText("SND_OUT_SPDIF_FORCE_DD");
				 			break;
				 	default: mtv.setText("Error");
				 			break;
				 }
				 */
				mtv.setText(dataservice.getString(KeyIndex.DHCPUserName, "ALi"));
				devinfo.setText(DevInfo.dev_name + "\n" + DevInfo.username + "\n" +  DevInfo.password );
			} catch (RemoteException e) {
                e.printStackTrace();
            }  
		}
	};
	
    /**
     * 获取setAM的 模式
     */
	private ServiceConnection mConnection_setAM = new ServiceConnection() {

		@Override
		public void onServiceDisconnected(ComponentName name) {
			dataservice = null;
		}
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			
			dataservice = ALIDataServiceCfg.Stub.asInterface(service); 
			try {
				if (dataservice.putString(KeyIndex.DHCPUserName, "Ning.Hu")) {
					  devinfo.setText("Sucessed!");
				}	
				else  devinfo.setText("Failed!");
			} catch (RemoteException e) {
                e.printStackTrace();
                devinfo.setText("Exception!");
            }  
		}
	};
	
	@Override
	 public void onClick(View v){
		switch (v.getId()) {
			case R.id.btngetAM: {
				//Intent service = new Intent("com.iptv.aidl.ALIAudioModeService");  
				Intent service = new Intent("com.iptv.aidl.DataServiceCfgService");
				bindService(service, mConnection_getAM, BIND_AUTO_CREATE);
				break;
			}
			case R.id.btnsetAM: {
				
				if (getValue()){
					Intent service = new Intent("com.iptv.aidl.DataServiceCfgService");  
					bindService(service, mConnection_setAM, BIND_AUTO_CREATE);
				}
				else {
					met.setText("");
				}
				flag = -1;
				break;
			}
			case R.id.btninsert:
				break;
			case R.id.btnqurey:
				break;
		}
		
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        mtv = (TextView)findViewById(R.id.textview);
        met = (EditText)findViewById(R.id.edittext);
        devinfo = (TextView)findViewById(R.id.txtview);
        btngetAM = (Button)findViewById(R.id.btngetAM);
        btnsetAM = (Button)findViewById(R.id.btnsetAM);
        btninsert = (Button)findViewById(R.id.btninsert);
        btnqurey = (Button)findViewById(R.id.btnqurey);
        
        btngetAM.setOnClickListener(this);
        btnsetAM.setOnClickListener(this);
        btninsert.setOnClickListener(this);
        btnqurey.setOnClickListener(this);
        
        
        
        /**
         * testing for class 
        */
        DevInfo = new ALIEthernetDevInfo();
        DevInfo.dev_name = "CT-IPTV";
        DevInfo.username = "ALi(SH)";
        DevInfo.password = "hnyy123456";
        DevInfo.ipmode = "ipv4";
        DevInfo.ipaddr = "10.41.150.146";
        DevInfo.netmask = "255.255.255.0";
        DevInfo.route = "10.41.255.8";
        
    }
 
    private boolean getValue() {
        String str = met.getText().toString();
        try {
            flag = Integer.parseInt(str);
            if (flag< 0  || flag > 2) {
            	return false;
            }
        } catch(NumberFormatException e) {
            return false;
        }
        return true;
    }


}
