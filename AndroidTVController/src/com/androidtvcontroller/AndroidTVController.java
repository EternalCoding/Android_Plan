package com.androidtvcontroller;

import java.util.Timer;
import java.util.TimerTask;

import com.androidtvconroller.R;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class AndroidTVController extends Activity implements OnClickListener{
	
	ImageButton btnKeyboard;
	ImageButton btnMic;
	ImageButton btnEnter;
	ImageButton btnUp;
	ImageButton btnDown;
	ImageButton btnLeft;
	ImageButton btnRight;
	ImageButton btnBack;
	ImageButton btnHome;
	
	TextView title;
	RelativeLayout controlArea;
	LinearLayout root;
	LinearLayout touchArea;
	LinearLayout numKeyboard;
	
	GestureDetector gd;
	
	private static final String TAG = "AndroidTVController";
	private String connectedIpAddress = null;
	private Vibrator vibrator;
	private Handler dataSyncHandler;
	private TimerTask dataSyncTask;
	private Timer  dataSyncTimer = new Timer();
	private boolean isTouchAreaOpened = false;
	private boolean isNumKeyboardOpened = false;
	//private boolean waitDoubleClick = false;
	
	
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.atvcontroller);
		vibrator = ((Vibrator) getSystemService("vibrator"));
		
		connectedIpAddress = getIntent().getExtras().getString(AndroidTVPlayActivity.IPADDRESS);
		
		btnKeyboard = (ImageButton) findViewById(R.id.btnkeyboard);
		btnMic = (ImageButton) findViewById(R.id.btnmic);
		btnUp = (ImageButton) findViewById(R.id.btnup);
		btnDown = (ImageButton) findViewById(R.id.btndown);
		btnLeft = (ImageButton) findViewById(R.id.btnleft);
		btnRight = (ImageButton) findViewById(R.id.btnright);
		btnEnter = (ImageButton) findViewById(R.id.btnenter);
		btnBack = (ImageButton) findViewById(R.id.btnback);
		btnHome = (ImageButton) findViewById(R.id.btnhome);
		
		title = (TextView) findViewById(R.id.connectedtitle);
		//title.setText(getString(R.string.connected) + connectedIpAddress);
		title.append(connectedIpAddress);
		
		
		controlArea = (RelativeLayout) findViewById(R.id.controlarea);
		touchArea = (LinearLayout) findViewById(R.id.toucharea);
		numKeyboard = (LinearLayout) findViewById(R.id.numKeyboard);
		root = (LinearLayout) findViewById(R.id.root);
		
		btnEnter.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View arg0) {
				// TODO Auto-generated method stub
				Log.i(TAG, "LongClick event");
				vibrator.vibrate(30L);
				isTouchAreaOpened = true;
				controlArea.setVisibility(View.GONE);
				touchArea.setVisibility(View.VISIBLE);
				return true;
			}
		});
		
		
		btnKeyboard.setOnClickListener(this);
		btnMic.setOnClickListener(this);
		btnUp.setOnClickListener(this);
		btnDown.setOnClickListener(this);
		btnLeft.setOnClickListener(this);
		btnRight.setOnClickListener(this);
		btnEnter.setOnClickListener(this);
		btnBack.setOnClickListener(this);
		btnHome.setOnClickListener(this);
		
		
		
		gd = new GestureDetector(this, new SimpleGestureDetector());
		touchArea.setOnTouchListener(new TouchListenerCallback());
		
		
		dataSyncHandler = new Handler(){
			
			@Override
			public void handleMessage(Message msg){
				Log.i(TAG, "dataSyncOpreation: receive msg " + msg.what);
				
				switch (msg.what) {
					case 1: 
						finish();
						break;
					case 2:
						if(!AliLowlevel.instance.SendSynData()) 
							finish();
						break;
				}
				super.handleMessage(msg);
				
			}
		};
		
		dataSyncTask = new TimerTask(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				dataSyncHandler.sendEmptyMessage(2);
			}
			
		};
		
		dataSyncTimer.schedule(dataSyncTask, 5000, 5000);
		
	}


	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		vibrator.vibrate(30L);
		int keyValue = 0;
		switch(v.getId()){
		case R.id.num0:
			keyValue = KeyboardValues.num0;
			break;
		case R.id.num1:
			keyValue = KeyboardValues.num1;
			break;
		case R.id.num2:
			keyValue = KeyboardValues.num2;
			break;
		case R.id.num3:
			keyValue = KeyboardValues.num3;
			break;
		case R.id.num4:
			keyValue = KeyboardValues.num4;
			break;
		case R.id.num5:
			keyValue = KeyboardValues.num5;
			break;
		case R.id.num6:
			keyValue = KeyboardValues.num6;
			break;	
		case R.id.num7:
			keyValue = KeyboardValues.num7;
			break;
		case R.id.num8:
			keyValue = KeyboardValues.num8;
			break;
		case R.id.num9:
			keyValue = KeyboardValues.num9;
			break;
		case R.id.btnDoc:
			keyValue = KeyboardValues.doc;
			break;
		case R.id.btnDelete:
			keyValue = KeyboardValues.delete;
			break;
		case R.id.btnMenu:
			keyValue = KeyboardValues.menu;
			break;
		case R.id.btnMute:
			keyValue = KeyboardValues.mute;
			break;
		case R.id.btnmic:
			break;
		case R.id.btnkeyboard:
			isNumKeyboardOpened = true;
			controlArea.setVisibility(View.GONE);
			numKeyboard.setVisibility(View.VISIBLE);
			break;
		case R.id.btnup:
			keyValue = KeyboardValues.up;
			break;
		case R.id.btndown:
			keyValue = KeyboardValues.down;
			break;
		case R.id.btnleft:
			keyValue = KeyboardValues.left;
			break;
		case R.id.btnright:
			keyValue = KeyboardValues.right;
			break;
		case R.id.btnenter:
			keyValue = KeyboardValues.enter;
			break;
		case R.id.btnback:
			if(!isTouchAreaOpened && !isNumKeyboardOpened) keyValue = KeyboardValues.back;
			else {
				
				if (isTouchAreaOpened) {
					isTouchAreaOpened = false;
					touchArea.setVisibility(View.GONE);
					controlArea.setVisibility(View.VISIBLE);
				} else {
					isNumKeyboardOpened = false;
					numKeyboard.setVisibility(View.GONE);
					controlArea.setVisibility(View.VISIBLE);
				}	
			}
			break;
		case R.id.btnhome:
			keyValue = KeyboardValues.home;
			break;
		}
		
		Log.i(TAG, "keyValue: "+ keyValue);
		if (!AliLowlevel.instance.SendKaypad(keyValue))
			AliMainAct.instance.handler.sendEmptyMessage(1);
	}
	
	
	
	public class TouchListenerCallback implements OnTouchListener{
		
	
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			// TODO Auto-generated method stub
			gd.onTouchEvent(event);
			return true;
		}
	
	}
	
	public class SimpleGestureDetector extends 	GestureDetector.SimpleOnGestureListener {
		
		@Override
		public boolean onDoubleTap(MotionEvent e) {
			Log.i(TAG, "Toucharea  DoubleClick");
			vibrator.vibrate(30L);
			if(!AliLowlevel.instance.SendKaypad(KeyboardValues.back))
				AliMainAct.instance.handler.sendEmptyMessage(1);
			return super.onDoubleTap(e);
		}
		@Override
		public boolean onDoubleTapEvent(MotionEvent e) {
		return super.onDoubleTapEvent(e);
		}

	

		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float x, float y) {
			
			vibrator.vibrate(30L);
			int keyValue;
			float X = e2.getX() - e1.getX();
			float Y = e2.getY() - e1.getY();
			
			if(X > 0 ){
				if(Y >= 0) {
					if(X > Y) {
						keyValue = KeyboardValues.right; 
						Log.i(TAG, "Toucharea touch event: Right ," + keyValue);
					}
					else{
						keyValue = KeyboardValues.down;
						Log.i(TAG, "Toucharea touch event: Down ," + keyValue);
					}
				}else{
					if(X > Math.abs(Y)) {
						keyValue = KeyboardValues.right;
						Log.i(TAG, "Toucharea touch event: Right ," + keyValue);
					}
					else {
						keyValue = KeyboardValues.up;
						Log.i(TAG, "Toucharea touch event: Up ," + keyValue);
					}
				}
			}else {
				
				if(Y >= 0){
					if(Math.abs(X) > Y) {
						keyValue = KeyboardValues.left;
						Log.i(TAG, "Toucharea touch event: Left ," + keyValue);
						
					}
					else {
						keyValue = KeyboardValues.down;
						Log.i(TAG, "Toucharea touch event: Down ," + keyValue);
					}
				}else {
					if(Math.abs(X) > Math.abs(Y)) {
						keyValue = KeyboardValues.left;
						Log.i(TAG, "Toucharea touch event: Left ," + keyValue);
					}
					else {
						keyValue = KeyboardValues.up;
						Log.i(TAG, "Toucharea touch event: Up ," + keyValue);
					}
				}	
			}
			if(!AliLowlevel.instance.SendKaypad(keyValue))
				AliMainAct.instance.handler.sendEmptyMessage(1);
			
			return true;
		}
		
		@Override
		public boolean onSingleTapConfirmed(MotionEvent e) {
			vibrator.vibrate(30L);
			Log.i(TAG, "Toucharea SingleClick");
			if(!AliLowlevel.instance.SendKaypad(KeyboardValues.enter))
				AliMainAct.instance.handler.sendEmptyMessage(1);
		return super.onSingleTapConfirmed(e);
		}

		@Override
		public boolean onSingleTapUp(MotionEvent e) {
		return super.onSingleTapUp(e);
		}
		
		
		
	}
	
	
	@Override
	protected void onDestroy() {
		//Log.e(TAG, "TEST");
		dataSyncTimer.cancel();
		AliLowlevel.instance.DisConnectStb();
		super.onDestroy();
	}

	

}
