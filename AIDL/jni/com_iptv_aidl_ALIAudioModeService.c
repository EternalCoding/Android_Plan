#include <string.h>
#include <jni.h>
#include <hld/snd/adr_snd.h>
#include <hld/snd/adr_snd_dev.h>
#include <hld/deca/adr_deca.h>
#include <hld/deca/adr_deca_dev.h>

/*
 * Class:     ali_AudioModeAidl_ALIAudioModeService
 * Method:    jnigetAudioMode
 * Signature: ()I
 */

enum ASndOutSpdifType spdif_type;

struct snd_device    *SndDev;
struct deca_device   *DecaDev;
/*
 * Class:     com_iptv_aidl_ALIAudioModeService
 * Method:    jnigetAudioMode
 * Signature: ()I
 */
 jint  Java_com_iptv_aidl_ALIAudioModeService_jnigetAudioMode(JNIEnv *env,
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	 jobject thiz)
 {
	 SndDev = (struct snd_device *)dev_get_by_id(1, 0);
	 return (jint) snd_io_control(SndDev, SND_GET_SPDIF_TYPE, &spdif_type);
 }

/*
 * Class:     com_iptv_aidl_ALIAudioModeService
 * Method:    jnisetAudioMode
 * Signature: (I)I
 */
 jint  Java_com_iptv_aidl_ALIAudioModeService_jnisetAudioMode(JNIEnv * env,
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	jobject thiz, jint flag)
 {
	 jint AudioFormat;
	 	jint ret;
	 	DecaDev = (struct deca_device *)dev_get_by_id(1, 0);
	 	SndDev = (struct snd_device *)dev_get_by_id(1, 0);

	 	switch(flag) {
	 	 case 0: ret = (jint)snd_io_control(SndDev, FORCE_SPDIF_TYPE, SND_OUT_SPDIF_PCM);
	 	 	 	 break;
	 	 case 1:
	 	 case 2: ret =  (jint)snd_io_control(SndDev, FORCE_SPDIF_TYPE, SND_OUT_SPDIF_BS);
	 	 	 	 ret |= (jint)deca_io_control(DecaDev, DECA_EMPTY_BS_SET, 0);
	 	 	 	 ret |= (jint)deca_io_control(DecaDev, DECA_GET_STR_TYPE, &AudioFormat);
	 	         ret |= (jint)deca_io_control(DecaDev, DECA_ADD_BS_SET, AudioFormat);
	 	 	 	 break;
	 	 default:
	 		 ret = 1;
	 		 break;
	 	 }
	 	 return ret;
 }
