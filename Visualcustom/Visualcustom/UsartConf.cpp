#include "stdafx.h"
#include "UsartConf.h"


CUsartConf::CUsartConf()
{

}

CUsartConf::~CUsartConf()
{

}

unsigned char CUsartConf::QueryDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;
	

	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = QUERY_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned char CUsartConf::ConfDevice(unsigned char* msg, unsigned char* dest_mac, DEVICE_PARAM& dev_param)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;
	USART_CONF   *pBody   = NULL;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = CONF_DEV;
	pHeader->len = sizeof(USART_CONF);
	len = sizeof(USART_HEADER);

	pBody = (USART_CONF*)(msg + len);
	memcpy(pBody->dest_mac, dest_mac, 2);
	pBody->dest_param.sys_type = dev_param.sys_type;
	pBody->dest_param.sleep_time = dev_param.sleep_time;
	pBody->dest_param.rf_mode = dev_param.rf_mode;
	memcpy(pBody->dest_param.host_mac, dev_param.host_mac, sizeof(dev_param.host_mac));
	memcpy(pBody->dest_param.anchors_mac, dev_param.anchors_mac, sizeof(dev_param.anchors_mac));
	len += sizeof(USART_CONF);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned char CUsartConf::ReadDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;


	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = READ_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned char CUsartConf::RebootDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;


	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = REBOOT_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned char CUsartConf::ResetDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;


	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = RESET_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned char CUsartConf::PowerTest(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;


	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = POWER_TEST;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned char CUsartConf::UpgradeDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;


	pHeader = (USART_HEADER*)msg;
	pHeader->header = 0xFF;
	pHeader->cmd = UPGRADE_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += encode_data(msg + len, 2);

	*(msg + len) = 0xFE;
	len++;

	return len;
}

unsigned short CUsartConf::CheckSum(unsigned char *str, unsigned char sum)
{
	unsigned char i = 0;
	unsigned short tmp =0;
	unsigned char buf[2] = {0};
	unsigned long total = 0;
	for (i = 0; i < sum; i++)
	{
		total +=  *(str + i);
	}
	buf[0] = (unsigned char)(total >> 8);
	buf[1] = (unsigned char)total;
	memcpy(&tmp, buf, 2);
	return tmp;
}

unsigned char CUsartConf::encode_data(unsigned char *str, unsigned char len)
{
	unsigned char i = 0;
	unsigned char buf_len = 0;
	unsigned char buf[USART_BUF_SIZE] = {0};
	for (i = 0; i < len; i++)
	{    
		if (*(str+i) == 0xFF)
		{
			buf[buf_len++] = 0xFD;
			buf[buf_len++] = 0x02;
		}
		else if (*(str+i) == 0xFE)
		{
			buf[buf_len++] = 0xFD;
			buf[buf_len++] = 0x01;
		}
		else if (*(str+i) == 0xFD)
		{
			buf[buf_len++] = 0xFD;
			buf[buf_len++] = 0x00;
		}
		else
		{
			buf[buf_len++] = *(str+i);
		}
	}
	memcpy(str,buf,buf_len);
	return buf_len;
}

unsigned char CUsartConf::decode_data(unsigned char *str, unsigned char len)
{
	unsigned char i = 0;
	unsigned char buf_len = 0;
	unsigned char buf[USART_BUF_SIZE] = {0};
	for (i = 0; i < len; i++)
	{    
		if (*(str+i) == 0xFD)
		{
			if (*(str+i+1) == 0x02)
			{
				buf[buf_len++] = 0xFF;
				i++;
			}
			else if (*(str+i+1) == 0x01)
			{
				buf[buf_len++] = 0xFE;
				i++;
			}
			else if (*(str+i+1) == 0x00)
			{
				buf[buf_len++] = 0xFD;
				i++;
			}
		}
		else
		{
			buf[buf_len++] = *(str+i);
		}
	}
	memset(str, 0, len);
	memcpy(str, buf, buf_len);
	return buf_len;
}