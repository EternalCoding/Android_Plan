@Override
protected void onCreated(Bundle savedInstance){		 
    intent intent = new Intent();
    ComponentName cn = new ComponentName("com.iptv.usercheck","com.iptv.usercheck.UserCheckActivity");
    intent.setComponent(cn);
    startActivityForResult(intent, 100);
}

@Override
	    protected void onActivityResult(int requestCode, int resultCode, Intent data)
	    {
	        super.onActivityResult(requestCode, resultCode, data);
	        if(requestCode == 100 && resultCode == 101)
	        {
	            if (data.getStringExtra("Result").equals("Exit") ){
	            	
	            	finish();
	            	
	            }
	            
	        }
	    }
	
