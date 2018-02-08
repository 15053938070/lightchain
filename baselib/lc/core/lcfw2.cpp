
#include "lcfw2.h"

const char *lcclient2_top = "\n\n\n\
[LcClientConfig] \n\
#�������� \n\
#0:����ʹ����Դ��λ \n\
#1:����ʹ�ñ������� \n\
ConfType  :  1 \n\
[.Reactor] \n\
ThreadNum  :  4 \n\
CheckTime : 1 \n\
[.LcClient] \n";

const char *lcclient2_galileo_top = "\n\
[LcClientConfig.Galileo] \n\
# ٤������Դ��������ַ \n\
GALILEO_HOST  :  db-passport-test05.vm:2181 \n\
# ٤�������ݰ���С��ע������ݰ������ǵ�������pack��С������һ�η���ʱ������ \n\
# ���ݳ��ȣ���λbyte��Ĭ�ϴ�СΪ128K \n\
GALILEO_DATA_PACK_SIZE  :  131072 \n\
# ���Zookeeper�ͻ�����־�ļ��� \n\
GALILEO_ZOO_LOG : ./log/zoo.pspui.log \n\
#��Դ��λ���ñ��صı���λ�� \n\
DIR : ./conf \n\
FILE : galileo-back.conf \n\
# ������Դ�����ע����Դ������ȫ��Ψһ��ע��������� \n\
[..REQUEST_RESOURCE] \n";

const char *lcclient2_galileo = "\
[...@RES_ARR] \n\
NAME  :  %s \n\
ADDR  :  /lcclient_node20 \n";
const char *lcclient2_metaconf = "\n[..@Service] \n\
Name  :  %s \n\
ConnectAll :  0 \n\
DefaultConnectTimeOut  :  1000 \n\
DefaultReadTimeOut  :  1000 \n\
DefaultWriteTimeOut  :  1000 \n\
DefaultMaxConnect  :  10 \n\
DefaultRetry  :  5 \n\
#LONG / SHORT \n\
DefaultConnectType  :  SHORT \n\
DefaultLinger  :  0 \n\
#������Ҫʹ�õĲ����༰���� \n\
[...CurrStrategy] \n\
ClassName  :  LcClientStrategy \n\
[...CurrHealthy] \n\
ClassName  :  LcClientHealthyChecker  \n\
[...@Server] \n\
IP : 127.0.0.1 \n\
Port : 6667 \n";

lc::LcClientManager *lc_load_clt2(lc_t *fw, const char *name, const char *cfg, lc::LcMonitor *monitor, int threadnum)
{
	if (fw == NULL || name == NULL || cfg == NULL) {
		return NULL;
	}
	int cfglen = strlen(cfg) + 10;
	if (cfglen < 256) {
		cfglen = 256;
	}
	char * svrn = (char *)malloc(cfglen);
	if (svrn == NULL) {
		return NULL;
	}
    
    lc::LcClientManager *lcmgr = new (std::nothrow)lc::LcClientManager;
    if (NULL == lcmgr) {
        free(svrn);
        return NULL;
    }

    lcmgr->setMonitor(monitor, threadnum);

	if (!fw->conf_build) {
        if (NULL != fw->conf && NULL != fw->conf->conf_new) {
            comcfg::ConfigUnit &temp_conf = *(fw->conf->conf_new);
            if (temp_conf["LcClientConfig"].selfType() != comcfg::CONFIG_ERROR_TYPE) {
	            if (0 != lcmgr->load(temp_conf["LcClientConfig"])) {
                    delete lcmgr;
                    lcmgr = NULL;
                    free(svrn);
                    return NULL;
                }
	            free (svrn);
                return lcmgr;
            }
        }
	    free (svrn);
        return NULL;
    }
    memset(svrn, 0, cfglen);

    fprintf(fw->conf->conf_file, "%s", lcclient2_top);
	const char *ptr = cfg;
	while (sscanf(ptr, "%s", svrn) == 1) {
		ptr += strlen(svrn);
		while (*ptr && (*ptr == ' ' || *ptr == '\t')) {
			++ptr;
		}
        fprintf(fw->conf->conf_file, lcclient2_metaconf, svrn);
		LC_LOG_DEBUG("scan %s module in client, left %s", svrn, ptr);
	}
    fprintf(fw->conf->conf_file, lcclient2_galileo_top, svrn);
	ptr = cfg;
    while (sscanf(ptr, "%s", svrn) == 1) {
		ptr += strlen(svrn);
		while (*ptr && (*ptr == ' ' || *ptr == '\t')) {
			++ptr;
		}
        fprintf(fw->conf->conf_file, lcclient2_galileo, svrn);
		LC_LOG_DEBUG("scan %s module in client, left %s", svrn, ptr);
	}

	free (svrn);
    return lcmgr;
}



















