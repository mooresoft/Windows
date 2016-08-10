#include "stdafx.h"
#include "UsartConf.h"


CUsartConf::CUsartConf()
{
	memset(&m_devParam, 0, sizeof(m_devParam));
}

CUsartConf::~CUsartConf()
{

}

unsigned char CUsartConf::package_head(unsigned char* msg, unsigned char cmd_type, unsigned char cmd_len)
{
	msg[0] = PKG_HEAD;
	msg[1] = cmd_type;
	msg[2] = cmd_len;

	return 3;
}

unsigned char CUsartConf::package_end(unsigned char* msg, unsigned char data_len)
{
	unsigned char pkg_len = 0;
	unsigned short chk_sum;

	chk_sum = CheckSum(&msg[1], data_len + 2);

	memcpy(&msg[3 + data_len], &chk_sum, sizeof(chk_sum));
	data_len += sizeof(chk_sum);

	pkg_len = 1;
	pkg_len += encode_data(&msg[1], data_len);
	msg[pkg_len] = PKG_END;
	pkg_len++;
	return pkg_len;
}

unsigned char CUsartConf::QueryDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;	

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = QUERY_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	memset(&m_devParam, 0, sizeof(m_devParam));
	return len;
}

unsigned char CUsartConf::ConfDevice(unsigned char* msg, unsigned char* dest_mac, DEVICE_PARAM& dev_param)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;
	USART_CONF   *pBody   = NULL;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = CONF_DEV;
	pHeader->len = sizeof(USART_CONF);
	len = sizeof(USART_HEADER);

	pBody = (USART_CONF*)(msg + len);
	memcpy(pBody->dest_mac, dest_mac, 2);
	pBody->dest_param.sys_type = dev_param.sys_type;
	pBody->dest_param.sleep_time = dev_param.sleep_time;
	pBody->dest_param.rf_mode = dev_param.rf_mode;
	pBody->dest_param.antenna_delay = dev_param.antenna_delay;
	memcpy(pBody->dest_param.host_mac, dev_param.host_mac, sizeof(dev_param.host_mac));
	memcpy(pBody->dest_param.anchors_mac, dev_param.anchors_mac, sizeof(dev_param.anchors_mac));
	len += sizeof(USART_CONF);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::ReadDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = READ_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::RebootDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = REBOOT_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::ResetDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = RESET_DEV;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::PowerTest(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = POWER_TEST;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::PreUpDevice(unsigned char* msg)
{
	unsigned char len;
	USART_HEADER *pHeader = NULL;	
	USART_END    *pEnd    = NULL;


	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = UPGRADE_PRE;
	pHeader->len = 0;
	len = sizeof(USART_HEADER);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::UpgradeDevice(unsigned char* msg, unsigned char seq, unsigned char msgtype, unsigned char* pbin, unsigned char sendlen)
{
	unsigned char len;
	USART_HEADER  *pHeader = NULL;	
	USART_END     *pEnd    = NULL;
	USART_UPGRADE *pBody   = NULL;
	unsigned char dst_len;

	pHeader = (USART_HEADER*)msg;
	pHeader->header = PKG_HEAD;
	pHeader->cmd = UPGRADE_DEV;
	pHeader->len = sizeof(USART_UPGRADE);
	len = sizeof(USART_HEADER);

	pBody = (USART_UPGRADE*)(msg + len);
	pBody->seq = seq;
	pBody->appdata.apptype = msgtype;

	dst_len = ((sendlen + 0x03) & 0xFFFFFFFC);
	pBody->appdata.applen  = sendlen;
	memset(pBody->appdata.app_data, 0, WRITE_SIZE);
	memcpy(pBody->appdata.app_data, pbin, sendlen);
	pBody->appdata.chksum = CheckSum(pbin, sendlen);
	len += sizeof(USART_UPGRADE);

	pEnd = (USART_END*)(msg + len);
	pEnd->checksum = CheckSum(msg, len);

	len += 2;

	len = encode_data(msg + 1, len - 1) + 1;

	*(msg + len) = PKG_END;
	len++;

	return len;
}

unsigned char CUsartConf::QueryAck(unsigned char* pBody)
{
	memcpy(m_devParam.host_mac, pBody, 2);
	return 0;
}

unsigned char CUsartConf::ParseUart(unsigned char* szCmd, unsigned char len)
{
	unsigned short calc_sum  = 0;
	unsigned short check_sum = 0;
	USART_HEADER*  pHeader   = NULL;
	unsigned char* pBody     = NULL;
	unsigned char* pmsg      = NULL;

	pmsg = szCmd;
	decode_data(szCmd, len);

	pHeader = (USART_HEADER*)pmsg;
	pBody = pmsg + sizeof(USART_HEADER);
	memcpy(&check_sum, pBody+pHeader->len, sizeof(check_sum));
	calc_sum = CheckSum(pmsg, sizeof(USART_HEADER)+pHeader->len);

	if(check_sum != calc_sum)
	{
		return 0xFF;
	}
	else
	{
		switch(pHeader->cmd)
		{
		case QUERY_ACK:
			QueryAck(pBody);
			break;
		default:
			break;
		}
	}

	return pHeader->cmd;
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
		if (*(str+i) == PKG_HEAD)
		{
			buf[buf_len++] = PKG_MID;
			buf[buf_len++] = 0xA3;
		}
		else if (*(str+i) == PKG_END)
		{
			buf[buf_len++] = PKG_MID;
			buf[buf_len++] = 0xA2;
		}
		else if (*(str+i) == PKG_MID)
		{
			buf[buf_len++] = PKG_MID;
			buf[buf_len++] = 0xA1;
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
		if (*(str+i) == PKG_MID)
		{
			if (*(str+i+1) == 0xA3)
			{
				buf[buf_len++] = PKG_HEAD;
				i++;
			}
			else if (*(str+i+1) == 0xA2)
			{
				buf[buf_len++] = PKG_END;
				i++;
			}
			else if (*(str+i+1) == 0xA1)
			{
				buf[buf_len++] = PKG_MID;
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