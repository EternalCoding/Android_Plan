package com.androidtvcontroller;


import java.util.Timer;
import java.util.TimerTask;

import com.androidtvconroller.R;

import android.app.TabActivity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;

@SuppressWarnings("deprecation")
public class AliMainAct extends TabActivity 
{
	public TabHost mth;
	public static final String TAB_CALL="CTL";

	static AliMainAct instance;
	public Handler handler = null;
	private final Timer timer = new Timer();
	private TimerTask task;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.mainact);
		mth = this.getTabHost();
		TabSpec ts1 = mth.newTabSpec(TAB_CALL).setIndicator(TAB_CALL);
		ts1.setContent(new Intent(AliMainAct.this,AndroidTVController.class));
		mth.addTab(ts1);
		mth.setCurrentTabByTag(TAB_CALL);
		instance = this;
        handler = new Handler()
        {
        	@Override
        	public void handleMessage(Message msg)
        	{
				Log.i("AliMainAct", "receive msg " + msg.what);
				switch(msg.what)
				{
					case 1:
						AliMainAct.instance.finish();
						break;
					case 2:
						if(AliLowlevel.instance.SendSynData() != true)
							AliMainAct.instance.finish();
						break;
				}
        		super.handleMessage(msg);
        	}
        };
        task = new TimerTask()
        {
        	@Override
        	public void run()
        	{
        		handler.sendEmptyMessage(2);
            }
        };
        timer.schedule(task, 5000, 5000);
	}

	@Override
	protected void onDestroy()
	{
		timer.cancel();
		AliLowlevel.instance.DisConnectStb();
		super.onDestroy();
	}
    
    protected void onStart(){
    	super.onStart();
    }
    
    protected void onStop(){
    	super.onStop();
    }
    
}
