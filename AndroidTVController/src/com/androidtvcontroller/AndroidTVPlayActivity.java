package com.androidtvcontroller;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.androidtvconroller.R;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

public class AndroidTVPlayActivity extends Activity implements View.OnClickListener{
	public static  final String TAG = "ATVActivity";
	public static final  String IPADDRESS="Connected Host";
	
	GridView myGrid;
	StbAdapter myAdapter;
	ImageView wifiSet;
	ImageView stbSearch;
	ProgressBar circleProgressBar;
	
	TextView title;
	RelativeLayout rlTip;
	TextView txtStbSearch;
	TextView txt1;
	TextView txt2;
	
	
	
	
	private boolean wifiFlag = false;
	private boolean stbFlag = false;
	private boolean isBackPressed = false;
	private Handler handlerCheckWifi;
	private Handler handlerCheckSTB;
	private Handler handlerProgress;
	//private Timer timerCheckWifi = new Timer();
	//private Timer timerCheckSTB = new Timer();
	private TimerTask taskCheckWifi;
	private TimerTask taskCheckSTB;
	private ArrayList<String> IpAddress = null ;
	private WifiManager myWifi;
	private WifiBroadcast wifiListener = new WifiBroadcast();
	private int stbNum = 0;
	private int stbNumSaved = 0;
	private int progress = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.atvmain);
		
		
		IntentFilter filter=new IntentFilter();
		filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		this.registerReceiver(wifiListener, filter);
		
		
		
		new AliLowlevel();
		AliLowlevel.instance.Init();
		
		wifiSet = (ImageView)findViewById(R.id.wifiset);
		stbSearch = (ImageView)findViewById(R.id.stbsearch);
		wifiSet.setOnClickListener(this);
		stbSearch.setOnClickListener(this);
		
		circleProgressBar = (ProgressBar)findViewById(R.id.circleProgressBar);
		title = (TextView)findViewById(R.id.title);
		myGrid = (GridView)findViewById(R.id.myGridView);
		rlTip = (RelativeLayout)findViewById(R.id.msg);
		txtStbSearch = (TextView)findViewById(R.id.tip);
		txt1 = (TextView)findViewById(R.id.tip1);
		txt2 = (TextView)findViewById(R.id.tip2);
		  
		
		myWifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		
		if (!myWifi.isWifiEnabled()){
			
			txt1.setText(R.string.tip1);
			txt2.setText(R.string.tip2);
			
			title.setText(R.string.WLAN);
			myGrid.setVisibility(View.GONE);
			rlTip.setVisibility(View.VISIBLE);
		}
			
			//Log.e(TAG,"WIFI Enabled");
		
			myAdapter = new StbAdapter(this);
			myAdapter.initStb(stbNum);
			myGrid.setAdapter(myAdapter);
			myGrid.setHorizontalScrollBarEnabled(false);
			
			IpAddress = new ArrayList<String>();
			
			myGrid.setOnItemClickListener(new OnItemClickListener(){

				@Override
				public void onItemClick(AdapterView<?> parent, View v,
						int position, long id) {
					// TODO Auto-generated method stub
					if(AliLowlevel.instance.ConnectStb(position)){
						
						Log.e(TAG, "Connect to the STB");
						
						Intent intent = new Intent(AndroidTVPlayActivity.this, AndroidTVController.class);
						Bundle bundle = new Bundle();
						bundle.putString(IPADDRESS, IpAddress.get(position));
						intent.putExtras(bundle);
						startActivity(intent);
						
					}else {
						title.setText(R.string.NoSTB);
						txt1.setText(R.string.tip3);
						txt2.setText(R.string.tip4);
						myGrid.setVisibility(View.GONE);
						rlTip.setVisibility(View.VISIBLE);
					}
				}
			});
			
			myGrid.setVisibility(View.GONE);
			if(wifiFlag)  {
				circleProgressBar.setVisibility(View.VISIBLE);
				txtStbSearch.setVisibility(View.VISIBLE);
				
			}
			
			handlerProgress = new Handler(){
				@Override 
				public void handleMessage(Message msg){
					switch(msg.what){
					case 0:
						 if(!Thread.currentThread().isInterrupted())  
			                  circleProgressBar.setProgress(progress);
						break;
					case 1:
						circleProgressBar.setVisibility(View.GONE);
						txtStbSearch.setVisibility(View.GONE);
						Thread.currentThread().interrupt();
						break;
					}
					super.handleMessage(msg);  
				}
			};
			Thread mThread = new Thread(new Runnable(){
				@Override
				public void run() {
					// TODO Auto-generated method stub
					for(int i= 0; i < 20; i++){
						i = i % 20;
						progress = i*5;
						handlerProgress.sendEmptyMessage(0);
					}
				}
				
			});
			mThread.start();
			
			handlerCheckSTB = new Handler(){
				@Override 
				public void handleMessage(Message msg){
					
					switch(msg.what){
					
					case 0: 
						if(myWifi.isWifiEnabled()){
							
							stbNum = AliLowlevel.instance.Scan();
							Log.e(TAG,"Check STB: " + stbNum + stbFlag);
							circleProgressBar.setVisibility(View.GONE);
							txtStbSearch.setVisibility(View.GONE);
							handlerProgress.sendEmptyMessage(1);
								if( stbNum <= 0){
									//stbFlag = false;
									title.setText(R.string.NoSTB);
									txt1.setText(R.string.tip3);
									txt2.setText(R.string.tip4);
									myGrid.setVisibility(View.GONE);
									rlTip.setVisibility(View.VISIBLE);
								} else {
									
									//stbFlag = true;
									rlTip.setVisibility(View.GONE);
									title.setText(R.string.title);
									myGrid.setVisibility(View.VISIBLE);
									
									if (stbNumSaved != stbNum) {
										
										Log.e(TAG, "redrawar!!!");
										stbNumSaved = stbNum;
										for(int i = 0; i < stbNum; i++) {
											String ipAddress = AliLowlevel.instance.GetMacAddress(i);
											IpAddress.add(ipAddress);
										}
										myAdapter.initStb(stbNum);
										myAdapter.notifyDataSetChanged();
									}
								}
							} else {
								circleProgressBar.setVisibility(View.GONE);
								txtStbSearch.setVisibility(View.GONE);
								title.setText(R.string.NoSTB);
								txt1.setText(R.string.tip3);
								txt2.setText(R.string.tip4);
								myGrid.setVisibility(View.GONE);
								rlTip.setVisibility(View.VISIBLE);
							}
						

						break;
					default:
						break;
					}
					 super.handleMessage(msg);  
					
				}
			};
			
			
			taskCheckSTB = new TimerTask(){
				@Override
				public void run() {
					// TODO Auto-generated method stub
					handlerCheckSTB.sendEmptyMessage(0);
				}
			};
			/*
			handlerCheckSTB.sendEmptyMessage(0);
			timerCheckSTB.schedule(taskCheckSTB, 2000);
			*/
			
			handlerCheckWifi = new Handler(){
				@Override 
				public void handleMessage(Message msg){
					
					Log.e(TAG, "wifi status:" + myWifi.isWifiEnabled());		
					if(myWifi.isWifiEnabled()){
						
						//wifiFlag = true;
						title.setText(R.string.title);
						myGrid.setVisibility(View.GONE);
						rlTip.setVisibility(View.GONE);
						circleProgressBar.setVisibility(View.VISIBLE);
						txtStbSearch.setVisibility(View.VISIBLE);
						handlerCheckSTB.postDelayed(taskCheckSTB, 2000);
						//handlerCheckSTB.sendEmptyMessage(0);
						
					} else {
						//wifiFlag = false;
						txt1.setText(R.string.tip1);
						txt2.setText(R.string.tip2);
						myGrid.setVisibility(View.GONE);
						rlTip.setVisibility(View.VISIBLE);
						title.setText(R.string.WLAN);
					}
					
					 super.handleMessage(msg);  
				}
			};
			taskCheckWifi = new TimerTask(){

				@Override
				public void run() {
					// TODO Auto-generated method stub
					handlerCheckWifi.sendEmptyMessage(0);
				}
			};	
			//timerCheckWifi.schedule(taskCheckWifi, 100);
			handlerCheckWifi.sendEmptyMessage(0);
	}
	

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch(v.getId()){
			case R.id.wifiset:
				Log.i(TAG, "click wifiset button");
				startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));
				break;
			case R.id.stbsearch:
				Log.i(TAG,"click stb search");
				checkStbOneTime();
				break;
		
		}
	}

	@Override
	protected void onDestroy() {
		Log.e(TAG,"onDestroy!!");
		unregisterReceiver(wifiListener);
		taskCheckSTB.cancel();
		taskCheckWifi.cancel();
		super.onDestroy();
	}
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.android_tvplay, menu);
		return true;
	}
	
	
	
	
	
	private void checkStbOneTime(){
		
		myGrid.setVisibility(View.GONE);
		rlTip.setVisibility(View.GONE);
		circleProgressBar.setVisibility(View.VISIBLE);
		txtStbSearch.setVisibility(View.VISIBLE);
		handlerCheckSTB.postDelayed(taskCheckSTB, 2000);
		
	}

	@Override
	public void onBackPressed(){
		if(isBackPressed){
			Thread.currentThread().interrupt();
			super.onBackPressed();
		}else {
			isBackPressed = true;
			Toast.makeText(this, R.string.backsure, Toast.LENGTH_SHORT).show();
			new BackCheck().start();
		}
		
	}
	
	class BackCheck extends Thread {
		@Override 
		public void run() {
			try {
				sleep(2000);
			}catch (InterruptedException e){
				e.printStackTrace();
			}
			isBackPressed = false;
		}
		
	}
	
	/**
	 * StbAdapter类，GridView通过此类来实现动态添加View
	 * @author ning.hu
	 *
	 */
	public class StbAdapter extends BaseAdapter {
		private List<Picture> pictures;
		private LayoutInflater inflater;
		
		
		public StbAdapter(Context c){
			super();
			pictures = new ArrayList<Picture>();
			inflater = LayoutInflater.from(c);
			
		}
		
		private void initStb(int size){
			pictures.clear();
			for(int i = 0; i< size; i++){
				Picture picture = new Picture(IpAddress.get(i),R.drawable.ic_item_tv);
				pictures.add(picture);
			}
		}

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			if (!pictures.isEmpty()){
				return pictures.size();
			}
			return 0;
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return pictures.get(position);
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
			ViewFactory viewFactory;
			if(convertView == null){
				viewFactory = new ViewFactory();
				convertView = inflater.inflate(R.layout.atvpicture_item, null);
				viewFactory.txtTitle = (TextView)convertView.findViewById(R.id.atvtitle);
				viewFactory.img = (ImageView)convertView.findViewById(R.id.atvimage);
				
				convertView.setTag(viewFactory);
			}else {
				viewFactory = (ViewFactory) convertView.getTag();
			} 
			
			viewFactory.txtTitle.setText(pictures.get(position).getTitle());
			viewFactory.img.setImageResource(pictures.get(position).getImageId());
			return convertView;
		}
		
		
		class ViewFactory{
			public TextView txtTitle;
			public ImageView img;
		}
		
		class Picture {
			private String title;
			private int imageId;
			
			public Picture() {
				super();
			}
			
			public Picture(String title, int imageId) {
				super();
				this.title = title;
				this.imageId = imageId;
			}
			
			public String getTitle(){
				return this.title;
			}
			
			public int getImageId() {
				return this.imageId;
			}
			
			public void setImageId(int id){
				this.imageId = id;
			}
			
			public void setTitle(String title){
				this.title = title;
			}
		}
		
	}
	
	
	public class WifiBroadcast extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			if(intent.getAction().endsWith(WifiManager.WIFI_STATE_CHANGED_ACTION)){
				Log.e(TAG,"Wifi Status Changed!!");
				if(myWifi.isWifiEnabled()){
					//handlerCheckWifi.sendEmptyMessage(0);
					Log.e(TAG, "Open");
					rlTip.setVisibility(View.GONE);
					circleProgressBar.setVisibility(View.VISIBLE);
					txtStbSearch.setVisibility(View.VISIBLE);
					handlerCheckWifi.postDelayed(taskCheckWifi, 2000);
					}
				else{ 
					Log.e(TAG, "Close");
					handlerCheckWifi.sendEmptyMessage(0);
				}
			}
		}
		
	}
	
	
}
