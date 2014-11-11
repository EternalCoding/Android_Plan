package com.iptv.aidl;

import com.iptv.aidl.ALIEthernetDevInfo;

interface ALISystemSettings {
 	int getAudioMode();
    int getResolution();
	boolean setAudilMode(int flag);
	boolean setResolution(int flag);
	boolean setEthernetDevInfo(in ALIEthernetDevInfo eth);
	ALIEthernetDevInfo getEthernetDevInfo();
	boolean setValue(String key,String value);
	String getValue(String key,String defValue);
}