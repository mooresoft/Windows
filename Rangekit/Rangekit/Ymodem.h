#pragma once

#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             (0x100000)
#define MAX_ERRORS              (5)


class CYmodem
{
public:
	CYmodem();
	CYmodem(CWnd* pMainFrm, HANDLE hCom);
	~CYmodem();

	int Receive(unsigned char*);
	unsigned char Transmit(unsigned char *, const  unsigned char*, unsigned int);

private:
	void Int2Str(unsigned char* str, int intnum);
	unsigned short UpdateCRC16(unsigned short crcIn, unsigned char byte);
	unsigned short Cal_CRC16(const unsigned char* data, unsigned int size);
	unsigned char CalChecksum(const unsigned char* data, unsigned int size);
	void PrepareIntialPacket(unsigned char *data, const unsigned char* fileName, unsigned int *length);
	void PreparePacket(unsigned char *SourceBuf, unsigned char *data, unsigned char pktNo, unsigned int sizeBlk);

	void SendPacket(const unsigned char* szBuff, DWORD dwBytesWrite);
	BOOL ReceiveByte(unsigned char* szBuff, unsigned int timeout);
	BOOL ReadSerial(unsigned char* szBuff);

	CWnd* m_pMainFrm;
	HANDLE m_hCom;
};

