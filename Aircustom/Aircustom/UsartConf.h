

#ifndef __USART_CONF_H__
#define __USART_CONF_H__

#define USART_BUF_SIZE 256


#define WRITE_SIZE 80

typedef enum _ENUM_MGMT_TYPE
{
	QUERY_DEV    = 0x00,
	QUERY_ACK    = 0x01,
	CONF_DEV     = 0x02,
	READ_DEV     = 0x03,
	REBOOT_DEV   = 0x04,
	RESET_DEV    = 0x05,
	POWER_TEST   = 0x06,
	UPGRADE_PRE  = 0x07,
	UPGRADE_DEV  = 0x08,
	MGMT_ACK     = 0x09,
	OTA_ACK      = 0x0A,
	SET_SPECTRUM = 0x0B,
	MGMT_UNKNOWN = 0xFD,
}UWB_MGMT_TYPE;

typedef enum _ENUM_PARAM_TYPE
{
	PARAM_SYS_TYPE    = 0x00,
	PARAM_HOST_MAC    = 0x01,
	PARAM_SLEEP_TIME  = 0x02,
	PARAM_RF_MODE     = 0x03,
	PARAM_ANCHOR_LIST = 0x04,
}UWB_PARAM_TYPE;

#pragma pack(1)

typedef struct _STRU_DEVICE_PARAM 
{
	unsigned char  sys_type;
	unsigned char  rf_mode;
	unsigned char  spectrum;
	unsigned char  host_mac[2];
	unsigned char  anchors_mac[20];
	unsigned short sleep_time;	
	unsigned short antenna_delay;
}DEVICE_PARAM;

typedef struct _STRU_USART_HEADER
{
	unsigned char	header;
	unsigned char	cmd;
	unsigned char	len;
}USART_HEADER;

typedef struct _STRU_USART_END
{
	unsigned short	checksum;
	unsigned char	end;
}USART_END;

typedef struct _STRU_USART_CONF
{
	unsigned char dest_mac[2];
	DEVICE_PARAM  dest_param;
}USART_CONF;

typedef struct _STRU_APP_DATA
{
	unsigned char  apptype;
	unsigned char  applen;
	unsigned char  app_data[WRITE_SIZE];
	unsigned short chksum;
}APP_DATA;

typedef struct _STRU_USART_UPGRADE
{
	unsigned char  seq;
	APP_DATA       appdata;
}USART_UPGRADE;

#pragma pack()

class CUsartConf
{
public:
	CUsartConf();
	~CUsartConf();

	unsigned char QueryDevice(unsigned char* msg);
	unsigned char ConfDevice(unsigned char* msg, unsigned char* dest_mac, DEVICE_PARAM& dev_param);
	unsigned char ReadDevice(unsigned char* msg);
	unsigned char RebootDevice(unsigned char* msg);
	unsigned char ResetDevice(unsigned char* msg);
	unsigned char PowerTest(unsigned char* msg);
	unsigned char PreUpDevice(unsigned char* msg);
	unsigned char UpgradeDevice(unsigned char* msg, unsigned char seq, unsigned char msgtype, unsigned char* pbin, unsigned char sendlen);
	unsigned char SetSpectrum(unsigned char* msg, unsigned char up_down);

    unsigned char ParseUart(unsigned char* szCmd, unsigned char len);

	DEVICE_PARAM  m_devParam;

private:
	unsigned short CheckSum(unsigned char *str, unsigned char sum);
	unsigned char encode_data(unsigned char *str, unsigned char len);
	unsigned char decode_data(unsigned char *str, unsigned char len);

	unsigned char package_head(unsigned char* msg, unsigned char cmd_type, unsigned char cmd_len);
	unsigned char package_end(unsigned char* msg, unsigned char data_len);

	unsigned char QueryAck(unsigned char* pBody);
};

#define PKG_HEAD 0x57
#define PKG_END  0x58
#define PKG_MID  0x59


#endif