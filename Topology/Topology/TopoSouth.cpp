#include "stdafx.h"
#include "TopoSouth.h"


#define SOUTH_BUF_LEN   255
#define SOUTH_HEAD_LEN  3

uint8 south_send_buf[SOUTH_BUF_LEN] = {0};
uint8 south_recv_buf[SOUTH_BUF_LEN] = {0};

uint8 south_encode_buf[SOUTH_BUF_LEN] = {0};
uint8 south_decode_buf[SOUTH_BUF_LEN] = {0};

uint8 south_status = 0;

void DMA_Tx_Data4(uint8 *buf, int len)
{

}

uint8 south_BCC_CheckSum(uint8 *buf, uint8 len)
{
	uint8 i;
	uint8 checksum = 0;

	for(i = 0; i < len; i++)
	{
		checksum ^= *buf++;
	}

	return checksum;
}

uint8 south_encode_data(uint8 *str, uint8 len)
{
	uint8 i = 0;
	uint8 buf_len = 0;

	memset(south_encode_buf, 0, SOUTH_BUF_LEN);
	for (i = 0; i < len; i++)
	{    
		if (0xAA == *(str+i))
		{
			south_encode_buf[buf_len++] = 0xFD;
			south_encode_buf[buf_len++] = 0x02;
		}
		else if (0x55 == *(str+i))
		{
			south_encode_buf[buf_len++] = 0xFD;
			south_encode_buf[buf_len++] = 0x01;
		}
		else if (0xFD == *(str+i))
		{
			south_encode_buf[buf_len++] = 0xFD;
			south_encode_buf[buf_len++] = 0x00;
		}
		else
		{
			south_encode_buf[buf_len++] = *(str+i);
		}
	}
	memcpy(str, south_encode_buf, buf_len);
	return buf_len;
}

uint8 south_decode_data(uint8 *str, uint8 len)
{
	uint8 i = 0;
	uint8 buf_len = 0;
	memset(south_decode_buf, 0, SOUTH_BUF_LEN);
	for (i = 0; i < len; i++)
	{    
		if (0xFD == *(str+i))
		{
			if (0x02 == *(str+i+1))
			{
				south_decode_buf[buf_len++] = 0xAA;
				i++;
			}
			else if (0x01 == *(str+i+1))
			{
				south_decode_buf[buf_len++] = 0x55;
				i++;
			}
			else if (0x00 == *(str+i+1))
			{
				south_decode_buf[buf_len++] = 0xFD;
				i++;
			}
		}
		else
		{
			south_decode_buf[buf_len++] = *(str+i);
		}
	}

	memset(str,0,len);
	memcpy(str, south_decode_buf, buf_len);
	return buf_len;
}

////////////////////////////////////////////////////////////////////////////////
//TX 
void south_package_head(uint8 cmd_type, uint8 cmd_len)
{
	memset(south_send_buf, 0, SOUTH_BUF_LEN);
	south_send_buf[0] = 0xAA;
	south_send_buf[1] = cmd_type;
	south_send_buf[2] = cmd_len;
}

uint8 south_package_end(uint8 data_len)
{
	uint8 pkg_len = 0;
	south_send_buf[SOUTH_HEAD_LEN + data_len] = south_BCC_CheckSum(&south_send_buf[1], data_len + 2);  
	pkg_len = 1;
	pkg_len += south_encode_data(&south_send_buf[1], data_len + 3);
	south_send_buf[pkg_len] = 0x55;
	pkg_len++;
	return pkg_len;
}

void south_Get_Zigbee_Id(void)
{
	uint8 pkg_len  = 0;
	uint8 data_len = 0x00;

	south_package_head(0x01, data_len);

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void south_Start_Mesh(uint8 mesh_flag)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x00;

	south_package_head(0x02, data_len);

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void Send_AZ_Info(uint16 AZ_info)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x0A;

	south_package_head(0x03, data_len);

	south_send_buf[3] = 0x11;//g_ConfigData.mac[0];
	south_send_buf[4] = 0x12;//g_ConfigData.mac[1];
	memcpy(&south_send_buf[11], &AZ_info, sizeof(AZ_info));

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void Get_Work_Mode(uint8* dst_id)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x08;

	south_package_head(0x04, data_len);

	south_send_buf[3] = dst_id[0];
	south_send_buf[4] = dst_id[1];

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void Set_Work_Mode(uint8 work_mode, uint8* dst_id)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x09;

	south_package_head(0x05, data_len);

	south_send_buf[3]  = dst_id[0];
	south_send_buf[4]  = dst_id[1];
	south_send_buf[11] = work_mode;

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void Get_Nwk_Status(void)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x00;

	south_package_head(0x06, data_len);

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void Re_Mesh(void)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x00;

	south_package_head(0x07, data_len);

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

void Send_Infrared_Info(uint8 dst_type, uint16 AZ_info, uint16 EL_info)
{
	uint8 pkg_len = 0;
	uint8 data_len = 0x05;

	south_package_head(0x08, data_len);

	south_send_buf[3] = dst_type;
	memcpy(&south_send_buf[4], &AZ_info, sizeof(AZ_info));
	memcpy(&south_send_buf[6], &EL_info, sizeof(EL_info));

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
}

uint8 Send_Ranges(uint8 nDim, uint16 aids[MAX_DIM], UINT16 rngs[MAX_DIM][MAX_DIM], uint8 **outbuf)
{
	uint8 pkg_len = 0;
	uint8 data_len = 6*nDim*nDim;
	uint8 range_idx = 0;

	south_package_head(0x09, data_len);

	for (uint8 i = 0; i < 5; i++)
	{
		for (uint8 j = 0; j < 5; j++)
		{
			memcpy(&south_send_buf[3 + range_idx * 6], &aids[i], 2);
			memcpy(&south_send_buf[5 + range_idx * 6], &aids[j], 2);
			memcpy(&south_send_buf[7 + range_idx * 6], &rngs[i][j], 2);
			range_idx++;
		}
	}  

	pkg_len = south_package_end(data_len);

	DMA_Tx_Data4(south_send_buf, pkg_len);
	*outbuf = south_send_buf;
	return pkg_len;
}

void Send_Ranges(uint8 nDim, uint16 aids[MAX_DIM], UINT16 rngs[MAX_DIM][MAX_DIM])
{

}

////////////////////////////////////////////////////////////////////////////////
/*
//RX
void south_parse_msg(void)
{
	uint8  buf_len = 0;
	uint8* pmsg    = NULL;
	uint8  bcc     = 0;
	uint8  chk_bcc = 0;

	if (0 == recv_msg_flag4)
	{
		return;
	}

	for (uint8 i = 0; i < USART_ARRAY_NUM; i++)
	{
		if (0 != usart_recv_array4[i].size)
		{     
			pmsg = usart_recv_array4[i].buf;
			buf_len = south_decode_data(pmsg, usart_recv_array4[i].size);
			bcc = pmsg[buf_len - 1];
			chk_bcc = south_BCC_CheckSum(pmsg, buf_len - 1);

			usart_recv_array4[i].size = 0;

			if (bcc != chk_bcc)
			{
				continue; 
			}

			switch(pmsg[0])
			{
			case 0x91:
				Recv_Zigbee_Id(&pmsg[3]);
				break;
			case 0x92:
				Recv_Mesh_Ack(pmsg[3]);
				break;
			case 0x93:
				Recv_AZ_Info(&pmsg[3], &pmsg[11]);
				break;
			case 0x94:
				Recv_Work_Mode(&pmsg[3], pmsg[11]);
				break;
			case 0x95:
				Recv_Work_Mode_Ack(&pmsg[3], pmsg[11]);
				break;
			case 0x96:
				//Recv_Nwk_Status(&pmsg[3], pmsg[11], );
				break;
			case 0x97:
				Recv_Remesh_Ack(pmsg[3]);
				break;
			case 0x98:

				break;
			case 0x99:

				break;
			case 0x9A:
				Recv_Alert_Info(&pmsg[3], &pmsg[11], pmsg[13], pmsg[14]);
				break;
			case 0x9B:
				Recv_Detect_Info(&pmsg[3]);
				break;
			default:
				break;
			}
		}
	}

	recv_msg_flag2 = 0;
}

void Recv_Zigbee_Id(uint8* zigbee_id)
{
	if (zigbee_id[1] != g_ConfigData.mac[1]
	|| zigbee_id[0] != g_ConfigData.mac[0])
	{
		g_ConfigData.mac[0] = zigbee_id[0];
		g_ConfigData.mac[1] = zigbee_id[1];
	}
}

void Recv_Mesh_Ack(uint8 mesh_info)
{
	south_status = mesh_info;
}

void Recv_AZ_Info(uint8* dst_id, uint8* AZ_info)
{
	uint16 azimuth;
	memcpy(&azimuth, AZ_info, sizeof(azimuth));

}

void Recv_Work_Mode(uint8* dst_id, uint8 work_mode)
{

}

void Recv_Work_Mode_Ack(uint8* dst_id, uint8 suc_ack)
{

}

void Recv_Remesh_Ack(uint8 suc_ack)
{
}

void Recv_Alert_Info(uint8* dst_id, uint8* ts, uint8 dev_type, uint8 trust_level)
{

}

void Recv_Detect_Info(uint8* pbody)
{
	uint8  dst_id[2] = {0};
	uint16 ts;
	uint8  dev_type;
	uint16 azimuth;
	uint16 el_info;
	uint16 range;
	uint16 tm_dly;
	uint8  trust_level;

	memcpy(dst_id, pbody, 2);
	memcpy(&ts, &pbody[2], sizeof(ts));
	dev_type = pbody[4];
	memcpy(&azimuth, &pbody[5], sizeof(azimuth));
	memcpy(&el_info, &pbody[7], sizeof(el_info));
	memcpy(&range, &pbody[9], sizeof(range));
	memcpy(&tm_dly, &pbody[11], sizeof(tm_dly));
	trust_level = pbody[13];
}
*/