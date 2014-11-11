package com.iptv.aidl;

import java.util.Locale;

import android.os.Parcel;
import android.os.Parcelable;

public final class ALIEthernetDevInfo implements Parcelable {
	
	public String dev_name;
	public String username;
	public String password; 
	public String ipmode; 
	public String ipaddr;
	public String netmask;
	public String route;
	public String dns;
	public String dns2;
	public String mode;
	public String ETH_CONN_MODE_DHCP;
	public String ETH_CONN_MODE_STATIC;
	public String ETH_CONN_MODE_DHCPPLUS; 
	
	
	public ALIEthernetDevInfo() {
		
    } 
	
	public static final Parcelable.Creator<ALIEthernetDevInfo> CREATOR = new  
            Parcelable.Creator<ALIEthernetDevInfo>() {
  
                public ALIEthernetDevInfo createFromParcel(Parcel in) {
                    return new ALIEthernetDevInfo(in);  
                }  
  
                public ALIEthernetDevInfo[] newArray(int size) {  
                    return new ALIEthernetDevInfo[size];
                }
  
            };  
	
	
	@Override  
	   public int describeContents() {  
	        return 0;  
	   }  
	
	@Override
	   public void writeToParcel(Parcel dest, int flags) {
		 dest.writeString(dev_name);
		 dest.writeString(username);
		 dest.writeString(password);
		 dest.writeString(ipmode);
		 dest.writeString(ipaddr);
		 dest.writeString(netmask);
		 dest.writeString(route);
		 dest.writeString(dns);
		 dest.writeString(dns2);
		 dest.writeString(mode);
		 dest.writeString(ETH_CONN_MODE_DHCP);
		 dest.writeString(ETH_CONN_MODE_STATIC);
		 dest.writeString(ETH_CONN_MODE_DHCPPLUS);
	}
	
	public ALIEthernetDevInfo(Parcel in) {
		dev_name = in.readString();
		username = in.readString();
		password = in.readString();
		ipmode = in.readString();
		ipaddr = in.readString();
		netmask = in.readString();
		route = in.readString();
		dns = in.readString();
		dns2 = in.readString();
		mode = in.readString();
		ETH_CONN_MODE_DHCP = in.readString();
		ETH_CONN_MODE_STATIC = in.readString();
		ETH_CONN_MODE_DHCPPLUS = in.readString();
	}	
	
}
