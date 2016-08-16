#pragma once

typedef unsigned char uint8;
typedef unsigned short uint16;


uint8 south_BCC_CheckSum(uint8 *buf, uint8 len);
uint8 south_decode_data(uint8 *str, uint8 len);

uint8 Send_Ranges(uint8 nDim, uint16 aids[MAX_DIM], UINT16 rngs[MAX_DIM][MAX_DIM], uint8 **outbuf);
