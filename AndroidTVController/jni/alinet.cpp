#include "alinet.h"
#define	JNIDEC_TAG	"ALIJNI"
static const char* const AliClassPathName = "com/androidtvcontroller/AliLowlevel";
static AliPlayCons* pplay = NULL;

#define	NORMAL_DELAY	3000
#define	EPG_DELAY		4000
bool PlayInit(JNIEnv* env, jobject thiz)
{
	pplay = (AliPlayCons*)malloc(sizeof(AliPlayCons));
	memset(pplay, 0, sizeof(AliPlayCons));
	pthread_mutex_init(&pplay->m_lock, NULL);
	return true;
}

void epg_free(AliPlayCons* pplay)
{
	if(pplay->plastepg != NULL)
	{
		free(pplay->plastepg);
		pplay->plastepg = NULL;
	}
	if(pplay->pdata != NULL)
	{
		free(pplay->pdata);
		pplay->pdata = NULL;
	}
	pplay->pdata_size = 0;
	if(pplay->m_pepg != NULL)
	{
		if(pplay->m_pepg->ALi_epg_data != NULL)
			free(pplay->m_pepg->ALi_epg_data);
		free(pplay->m_pepg);
		pplay->m_pepg = NULL;
	}
}

void PlayUnInit(JNIEnv* env, jobject thiz)
{
	if(pplay != NULL)
	{
		if(pplay->curstbsock != 0)
		{
			tcp_close(pplay->curstbsock);
			pplay->curstbsock = 0;
		}
		epg_free(pplay);
		pthread_mutex_destroy(&pplay->m_lock);
		free(pplay);
		pplay = NULL;
	}
}

bool ConnectStb(JNIEnv* env, jobject thiz, int index)
{
	int consock;
	bool ret = false;

	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "connect stb %s\n", pplay->stbip[index]);
	pthread_mutex_lock(&pplay->m_lock);
	consock = tcp_connect(pplay->stbip[index], 6280, 3000);
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "connect stb 111 %d\n", consock);
	if(consock > 0)
	{
		if(pplay->curstbsock != 0)
			tcp_close(pplay->curstbsock);
		pplay->curstbsock = consock;
		ret = true;
	}
	epg_free(pplay);
	pthread_mutex_unlock(&pplay->m_lock);
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "connect stb 222 %d\n", ret);
	return ret;
}

bool DisConnectStb(JNIEnv* env, jobject thiz)
{
	ALI_MSG msg;
	unsigned int msg_snd = 0;
	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->curstbsock != 0)
	{
		msg.type = (short)ALI_MSG_DISCONNECT;
		memcpy(&msg_snd, &msg, 4);
		tcp_send(pplay->curstbsock, (char *)&msg_snd, sizeof(msg_snd), 100);
		tcp_close(pplay->curstbsock);
		pplay->curstbsock = 0;
	}
	pthread_mutex_unlock(&pplay->m_lock);
	return true;
}

bool SendKaypad(JNIEnv* env, jobject thiz, int keyval)
{
	ALI_MSG msg;
	bool ret = false;
    unsigned int msg_snd = 0;
	pthread_mutex_lock(&pplay->m_lock);
	msg.type = (short)ALI_MSG_KEY;
	msg.key = keyval;
	memcpy(&msg_snd, &msg, 4);
	unsigned int send_msg = htonl(msg_snd);
   if(tcp_send(pplay->curstbsock, (char *)&send_msg, sizeof(send_msg), NORMAL_DELAY) == sizeof(send_msg))

	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "SendKaypad keyval %d\n", keyval);
//	if(tcp_send(pplay->curstbsock, (char *)&msg, sizeof(ALI_MSG), NORMAL_DELAY) == sizeof(ALI_MSG))
	{
		ret=true;
//		if(tcp_recv(pplay->curstbsock, (char *)&msg, 4, NORMAL_DELAY) == 4)
//		{
//			ret = true;
//		}
	}
	pthread_mutex_unlock(&pplay->m_lock);
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "SendKaypad %d\n", ret);
	return ret;
}

bool SendSynData(JNIEnv* env, jobject thiz)
{
	ALI_MSG msg;
	bool ret = false;
	unsigned int msg_snd = 0;

	pthread_mutex_lock(&pplay->m_lock);
	msg.type = (short)ALI_MSG_SYN;
	memcpy(&msg_snd, &msg, 4);
	unsigned int sync_msg = htonl(msg_snd);
	if(tcp_send(pplay->curstbsock, (char *)&sync_msg, sizeof(sync_msg), NORMAL_DELAY) == sizeof(sync_msg))
	{
		__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "SendSynData OK\n");
//		if(tcp_recv(pplay->curstbsock, (char *)&msg, 4, NORMAL_DELAY) == 4)
//		{
//			__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "SendSynData Recv OK\n");
			ret = true;
//		}
	}
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "SendSynData %d\n", ret);
	pthread_mutex_unlock(&pplay->m_lock);
	return ret;
}

int ScanStb(JNIEnv* env, jobject thiz)
{
	int recvsocket;
	char buf[80];
	in_addr n;
	int i;

	pplay->stbnum = 0;
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "ScanStbfunc started\n");
	recvsocket = udp_create(5211);

	if(recvsocket < 0)
	{
		return -1;
	}

	if(sendbrocast(recvsocket, 5210) < 0)
	{
		close(recvsocket);
		return -1;
	}
	
	while(1)
	{
		if(udp_recv(recvsocket, NORMAL_DELAY, buf) < 0)
			break;
		for(i = 0; i < pplay->stbnum; i++)
		{
//			__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "stb %d ip is %s %s %d\n", i, buf, pplay->stbip[i], strcmp(buf, pplay->stbip[i]));
			if(strcmp(buf, pplay->stbip[i]) == 0)
				break;
		}
		if(i == pplay->stbnum)
		{
			strcpy(pplay->stbip[pplay->stbnum], buf);
			pplay->stbnum++;
		}
	}
	close(recvsocket);
	for(i = 0; i < pplay->stbnum; i++)
		__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "stb %d ip is %s\n", i, pplay->stbip[i]);
	return pplay->stbnum;
}


jstring GetMacAddress(JNIEnv*env, jobject thiz, int item)
{
	jstring jvalue;
	char databuf[128];
	DayStartEnd* pcur;
	strcpy(databuf, "        ");
	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->stbip == NULL)
		goto fail;
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "stb %d ip is %s\n", item, pplay->stbip[item]);
	sprintf(databuf, "%s", pplay->stbip[item]);
fail:
	pthread_mutex_unlock(&pplay->m_lock);
	jvalue = env->NewStringUTF(databuf);
	return jvalue;
}


ALI_EPG_DATA* epg_parse(char* pdata)
{
	unsigned char* dec_ptr;
	char* rst_ptr;
	unsigned int len, i, j;
	ALI_EPG_DATA* pepg;
	ALI_EPG_EVENT*	pepgdata;
	int dayindex = 0;
	int daynum = 0;

	pepg = (ALI_EPG_DATA *)malloc(sizeof(ALI_EPG_DATA));
	memset(pepg, 0, sizeof(ALI_EPG_DATA));
	dec_ptr = (unsigned char*)pdata;
	rst_ptr = pdata;

	// process channel name
	len = dec_ptr[3] << 24 | dec_ptr[2] << 16 | dec_ptr[1] << 8 | dec_ptr[0];
	dec_ptr += 4;
	memcpy(rst_ptr, dec_ptr, len);
	rst_ptr[len] = 0;
	pepg->chan_name = rst_ptr;
	rst_ptr += len + 1;
	dec_ptr += len;

	// get event number
	len = dec_ptr[3] << 24 | dec_ptr[2] << 16 | dec_ptr[1] << 8 | dec_ptr[0];
	if(len == 0)
		return pepg;
	pepg->event_num = len;
	pepgdata = (ALI_EPG_EVENT*)malloc(len * sizeof(ALI_EPG_EVENT));
	memset(pepgdata, 0, len * sizeof(ALI_EPG_EVENT));
	dec_ptr += 4;
	pepg->ALi_epg_data = pepgdata;
	for(i = 0; i < pepg->event_num; i++)
	{
		ALI_TIME mtime;
		memcpy(&mtime, dec_ptr, sizeof(ALI_TIME));
		dec_ptr += sizeof(ALI_TIME);
		if(i == 0)
		{
			pepg->datainfo[dayindex].startevent = 0;
			pepg->datainfo[dayindex].start_year = mtime.year;
			pepg->datainfo[dayindex].start_month= mtime.month;
			pepg->datainfo[dayindex].start_day = mtime.day;
			pepg->datainfo[dayindex].weekday = mtime.weekday;
		}
		else
		{
			if(pepg->datainfo[dayindex].weekday != mtime.weekday)
			{
				pepg->datainfo[dayindex].endevent = i;
				dayindex++;
				pepg->datainfo[dayindex].startevent = i;
				pepg->datainfo[dayindex].start_year = mtime.year;
				pepg->datainfo[dayindex].start_month= mtime.month;
				pepg->datainfo[dayindex].start_day = mtime.day;
				pepg->datainfo[dayindex].weekday = mtime.weekday;
				if(dayindex > 6)
				{
					dayindex = 6;
					break;
				}
			}
		}

		pepgdata[i].start_time.hour = mtime.hour;
		pepgdata[i].start_time.min = mtime.min;
		pepgdata[i].start_time.sec = mtime.sec;
		memcpy(&mtime, dec_ptr, sizeof(ALI_TIME));
		dec_ptr += sizeof(ALI_TIME);
		pepgdata[i].end_time.hour = mtime.hour;
		pepgdata[i].end_time.min = mtime.min;
		pepgdata[i].end_time.sec = mtime.sec;

		for(j = 0; j < 3; j++)
		{
			// get lang
			if(i == 0)
			{
				memcpy(rst_ptr, dec_ptr, 3);
				rst_ptr[3] = 0;
				pepg->language[j] = rst_ptr;
				rst_ptr += 4;
			}
			dec_ptr += 3;

			// get event name
			len = dec_ptr[3] << 24 | dec_ptr[2] << 16 | dec_ptr[1] << 8 | dec_ptr[0];
			dec_ptr += 4;
			if(len != 0)
			{
				memcpy(rst_ptr, dec_ptr, len);
				rst_ptr[len] = 0;
				pepgdata[i].lang[j].event_name = rst_ptr;
				rst_ptr += len + 1;
			}
			dec_ptr += len;

			// get short detail
			len = dec_ptr[3] << 24 | dec_ptr[2] << 16 | dec_ptr[1] << 8 | dec_ptr[0];
			dec_ptr += 4;
			if(len != 0)
			{
				memcpy(rst_ptr, dec_ptr, len);
				rst_ptr[len] = 0;
				pepgdata[i].lang[j].ext_detail = rst_ptr;
				rst_ptr += len + 1;
			}
			dec_ptr += len;

			// get detail
			len = dec_ptr[3] << 24 | dec_ptr[2] << 16 | dec_ptr[1] << 8 | dec_ptr[0];
			dec_ptr += 4;
			if(len != 0)
			{
				memcpy(rst_ptr, dec_ptr, len);
				rst_ptr[len] = 0;
				pepgdata[i].lang[j].short_detail = rst_ptr;
				rst_ptr += len + 1;
			}
			dec_ptr += len;
		}
	}
	if(pepg->datainfo[dayindex].endevent == 0)
		pepg->datainfo[dayindex].endevent = pepg->event_num;
	
	for(i = 0, pepg->langnum = 0; i < 3; i++)
	{
		if(memcmp(pepg->language[i], "   ", 3) == 0)
			continue;
		pepg->langnum++;
	}
/*
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "channel name: %s event num %d\n", pepg->chan_name, pepg->event_num);
	for(i = 0; i < 7; i++)
	{
		__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "Date:%04d/%02d/%02d week%d\n", pepg->datainfo[i].start_year, pepg->datainfo[i].start_month, pepg->datainfo[i].start_day, pepg->datainfo[i].weekday);
		for(j = pepg->datainfo[i].startevent; j < pepg->datainfo[i].endevent; j++)
		{
			int k;
			__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "event %d time: %02d:%02d:%02d -> %02d:%02d:%02d\n", j, pepg->ALi_epg_data[j].start_time.hour, pepg->ALi_epg_data[j].start_time.min, 
				pepg->ALi_epg_data[j].start_time.sec, pepg->ALi_epg_data[j].end_time.hour, pepg->ALi_epg_data[j].end_time.min, pepg->ALi_epg_data[j].end_time.sec);
			for(k = 0; k < 3; k++)
			{
				__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "name %s\n", pepg->ALi_epg_data[j].lang[k].event_name);
				__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "short %s\n", pepg->ALi_epg_data[j].lang[k].short_detail);
				__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "detail %s\n", pepg->ALi_epg_data[j].lang[k].ext_detail);
			}
		}
	}
*/
	return pepg;
}

int GetEpgList(JNIEnv* env, jobject thiz)
{
	ALI_MSG msg;
	int epgsize;
	char* pbuf;
	char* plastbuf;
	int ret = -1;
	ALI_EPG_DATA* pepg;

	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "request epgdata\n");
	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->curstbsock > 0)
	{
		msg.type = (short)ALI_MSG_EPG;
		if(tcp_send(pplay->curstbsock, (char *)&msg, sizeof(ALI_MSG), NORMAL_DELAY) == sizeof(ALI_MSG))
		{
			__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "send epg msg ok\n");
			if(tcp_recv(pplay->curstbsock, (char *)&epgsize, 4, NORMAL_DELAY) == 4)
			{
				__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "epgsize %d\n", epgsize);
				pbuf = (char*)malloc(epgsize);
				memset(pbuf, 0, epgsize);
				if(tcp_recv(pplay->curstbsock, pbuf, epgsize- 4, EPG_DELAY) == (epgsize- 4))
					ret = 0;
				else
				{
					__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "request epgdata fail\n");
					free(pbuf);
				}
			}
		}
	}
	pthread_mutex_unlock(&pplay->m_lock);
	if(ret < 0)
		return ret;
	if((pplay->pdata_size == epgsize- 4)
		&& (pplay->plastepg != NULL)
		&& (memcmp(pplay->plastepg, pbuf, epgsize - 4) == 0))
	{
		__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "epg data same as last\n");
		free(pbuf);
		return 0;
	}
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "parse epg data\n");
	plastbuf = (char*)malloc(epgsize);
	memset(plastbuf, 0, epgsize);
	memcpy(plastbuf, pbuf, epgsize - 4);
	pepg = epg_parse(pbuf);
	pthread_mutex_lock(&pplay->m_lock);
	epg_free(pplay);
	pplay->m_pepg = pepg;
	pplay->pdata = pbuf;
	pplay->plastepg = plastbuf;
	pplay->pdata_size = epgsize- 4;
	pthread_mutex_unlock(&pplay->m_lock);
	return true;
}

jstring GetEpgChannel(JNIEnv*env, jobject thiz)
{
	jstring jvalue;
	pthread_mutex_lock(&pplay->m_lock);
	if((pplay->m_pepg != NULL)
		&& (pplay->m_pepg->chan_name != NULL))
		jvalue = env->NewStringUTF(pplay->m_pepg->chan_name);
	else
		jvalue = env->NewStringUTF("      ");
	pthread_mutex_unlock(&pplay->m_lock);
	return jvalue;
}

int GetEpgNumByIndex(JNIEnv*env, jobject thiz, int day)
{
	int len = 0;

	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->m_pepg != NULL)
		len = pplay->m_pepg->datainfo[day].endevent - pplay->m_pepg->datainfo[day].startevent; 
	pthread_mutex_unlock(&pplay->m_lock);
	return len;
}


jstring GetEpgEventName(JNIEnv*env, jobject thiz, int day, int index, int lan)
{
	jstring jvalue = NULL;
	char* pstr = NULL;

	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->m_pepg != NULL)
	{
		if((pplay->m_pepg->datainfo[day].startevent + index) < pplay->m_pepg->event_num)
			pstr = pplay->m_pepg->ALi_epg_data[pplay->m_pepg->datainfo[day].startevent + index].lang[lan].event_name;
	}
	if(pstr != NULL)
		jvalue = env->NewStringUTF(pstr);
	pthread_mutex_unlock(&pplay->m_lock);
	return jvalue;
}

int GetEpgLangNum(JNIEnv*env, jobject thiz)
{
	int len = 0;
	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->m_pepg != NULL)
		len = pplay->m_pepg->langnum;
	pthread_mutex_unlock(&pplay->m_lock);
	return len;
}

jstring GetEpgDate(JNIEnv*env, jobject thiz, int index)
{
	jstring jvalue;
	char databuf[128];
	DayStartEnd* pcur;
	strcpy(databuf, "        ");
	pthread_mutex_lock(&pplay->m_lock);
	if(pplay->m_pepg == NULL)
		goto fail;
	if(pplay->m_pepg->datainfo[index].endevent == 0)
		goto fail;
	pcur = &pplay->m_pepg->datainfo[index];
	sprintf(databuf, "%04d-%02d-%02d<%d>", pcur->start_year, pcur->start_month, pcur->start_day, pcur->weekday);
fail:
	pthread_mutex_unlock(&pplay->m_lock);
	jvalue = env->NewStringUTF(databuf);
	return jvalue;
}

jstring GetEpgLang(JNIEnv*env, jobject thiz, int lan)
{
	jstring jvalue;
	int i, index = 0;

	pthread_mutex_lock(&pplay->m_lock);
	if((pplay->m_pepg == NULL)
		|| (lan >= pplay->m_pepg->langnum))
		jvalue = env->NewStringUTF("   ");
	else
	{
		for(i = 0; i < 3; i++)
		{
			if(memcmp(pplay->m_pepg->language[i], "   ", 3) != 0)
			{
				if(index == lan)
				{
					jvalue = env->NewStringUTF(pplay->m_pepg->language[lan]);
					break;
				}
				else
					index++;
			}
		}
	}
	pthread_mutex_unlock(&pplay->m_lock);
	return jvalue;
}

jstring GetEpgEventTime(JNIEnv*env, jobject thiz, int day, int index)
{
	jstring jvalue;
	ALI_EPG_EVENT* pevent;
	char timestr[128];

	pthread_mutex_lock(&pplay->m_lock);
	if((pplay->m_pepg != NULL)
		&& ((pplay->m_pepg->datainfo[day].startevent + index) < pplay->m_pepg->event_num))
	{
		pevent = &pplay->m_pepg->ALi_epg_data[pplay->m_pepg->datainfo[day].startevent + index];
		sprintf(timestr, "%02d:%02d - %02d:%02d", pevent->start_time.hour, pevent->start_time.min,
			pevent->end_time.hour, pevent->end_time.min);
	}
	else
		strcpy(timestr, "00:00 - 00:00");
	pthread_mutex_unlock(&pplay->m_lock);
	jvalue = env->NewStringUTF(timestr);
	return jvalue;
}

jstring GetEpgEventDetail(JNIEnv*env, jobject thiz, int day, int index, int lan)
{
	jstring jvalue = NULL;
	ALI_EPG_EVENT* pevent;
	pthread_mutex_lock(&pplay->m_lock);
	if((pplay->m_pepg != NULL)
		&& ((pplay->m_pepg->datainfo[day].startevent + index) < pplay->m_pepg->event_num))
	{
		pevent = &pplay->m_pepg->ALi_epg_data[pplay->m_pepg->datainfo[day].startevent + index];
		if(pevent->lang[lan].ext_detail != NULL)
			jvalue = env->NewStringUTF(pevent->lang[lan].ext_detail);
		else if(pevent->lang[lan].short_detail != NULL)
			jvalue = env->NewStringUTF(pevent->lang[lan].short_detail);
	}
	pthread_mutex_unlock(&pplay->m_lock);
	return jvalue;
}


static JNINativeMethod JnigMethods[] =
{
	{"Init", "()Z", (void*)PlayInit},
	{"UnInit", "()V", (void*)PlayUnInit},
	{"Scan", "()I", (void*)ScanStb},
	{"ConnectStb", "(I)Z", (void*)ConnectStb},
	{"DisConnectStb", "()Z", (void*)DisConnectStb},
	{"SendKaypad", "(I)Z", (void*)SendKaypad},
	{"SendSynData", "()Z", (void*)SendSynData},
	{"GetEpgList", "()I", (void*)GetEpgList},
	{"GetEpgChannel", "()Ljava/lang/String;", (void*)GetEpgChannel},
	{"GetEpgLangNum", "()I", (void*)GetEpgLangNum},
	{"GetEpgDate", "(I)Ljava/lang/String;", (void*)GetEpgDate},
	{"GetEpgLang", "(I)Ljava/lang/String;", (void*)GetEpgLang},
	{"GetEpgNumByIndex", "(I)I", (void*)GetEpgNumByIndex},
	{"GetEpgEventName", "(III)Ljava/lang/String;", (void*)GetEpgEventName},
	{"GetEpgEventTime", "(II)Ljava/lang/String;", (void*)GetEpgEventTime},
	{"GetEpgEventDetail", "(III)Ljava/lang/String;", (void*)GetEpgEventDetail},
	{"GetMacAddress","(I)Ljava/lang/String;",(void*)GetMacAddress},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = JNI_ERR;
	jclass clazz;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_2) != JNI_OK)
	{
		__android_log_print(ANDROID_LOG_ERROR, JNIDEC_TAG, "GetEnv failed!");
		return result;
	}

    __android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "loading . . .");

  	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "Registering %s natives\n", AliClassPathName);
	clazz = env->FindClass(AliClassPathName);
	if (clazz == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR, JNIDEC_TAG, "Native registration unable to find class '%s'\n", AliClassPathName);
		return -1;
	}

	if (env->RegisterNatives(clazz, JnigMethods, sizeof(JnigMethods) / sizeof(JnigMethods[0])) < 0)
	{
		__android_log_print(ANDROID_LOG_ERROR, JNIDEC_TAG, "RegisterNatives failed for '%s'\n", AliClassPathName);
		return -1;
	}
 
	__android_log_print(ANDROID_LOG_INFO, JNIDEC_TAG, "loaded");

    result = JNI_VERSION_1_2;
end:
    return result;
}


