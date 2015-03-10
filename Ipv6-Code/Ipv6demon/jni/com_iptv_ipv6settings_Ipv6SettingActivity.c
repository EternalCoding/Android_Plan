#include <jni.h>
#include <string.h>
#include <setipv6add.h>
jint  Java_com_iptv_ipv6settings_Ipv6SettingActivity_SetIpv6Add(JNIEnv *env, jobject thiz,
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	 jstring eth, jstring ipv6add, jint prefix){
	 	 char const *dev = (*env)->GetStringUTFChars(env, eth, NULL);
		 char const *ip6add = (*env)->GetStringUTFChars(env, ipv6add, NULL);
		 int pfix = (int) prefix;

		 return (jint) setipv6(dev, ip6add, pfix);

		  (*env)->ReleaseStringUTFChars(env, eth, dev);
		  (*env)->ReleaseStringUTFChars(env, ipv6add, ip6add);

 }
