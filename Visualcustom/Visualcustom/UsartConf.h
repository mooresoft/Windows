

#ifndef __USART_CONF_H__
#define __USART_CONF_H__

#define USART_BUF_SIZE 64

typedef enum _ENUM_MGMT_TYPE
{
	QUERY_DEV    = 0x00,
	QUERY_ACK    = 0x01,
	CONF_DEV     = 0x02,
	READ_DEV     = 0x03,
	REBOOT_DEV   = 0x04,
	RESET_DEV    = 0x05,
	POWER_TEST   = 0x06,
	UPGRADE_DEV  = 0x07,
	MGMT_UNKNOWN = 0xFF,
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
	unsigned char  host_mac[2];
	unsigned char  anchors_mac[20];
	unsigned short sleep_time;	
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
	unsigned char UpgradeDevice(unsigned char* msg);

	DEVICE_PARAM dev_param;
	DEVICE_PARAM set_param;

private:
	unsigned short CheckSum(unsigned char *str, unsigned char sum);
	unsigned char encode_data(unsigned char *str, unsigned char len);
	unsigned char decode_data(unsigned char *str, unsigned char len);
};

#endif