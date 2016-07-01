#include "stdafx.h"
#include "Ymodem.h"
#include "Rangekit.h"

#define FLASH_IMAGE_SIZE 1024 * 1024

CYmodem::CYmodem()
{
}

CYmodem::CYmodem(CWnd* pMainFrm, HANDLE hCom)
{
	m_pMainFrm = pMainFrm;
	m_hCom = hCom;
}

CYmodem::~CYmodem()
{
}

int CYmodem::Receive(unsigned char*)
{
	return 0;
}

/**
* @brief  Convert an Integer to a string
* @param  str: The string
* @param  intnum: The intger to be converted
* @retval None
*/
void CYmodem::Int2Str(unsigned char* str, int intnum)
{
	unsigned int i, Div = 1000000000, j = 0, Status = 0;

	for (i = 0; i < 10; i++)
	{
		str[j++] = (intnum / Div) + 48;

		intnum = intnum % Div;
		Div /= 10;
		if ((str[j - 1] == '0') & (Status == 0))
		{
			j = 0;
		}
		else
		{
			Status++;
		}
	}
}

/**
* @brief  Update CRC16 for input byte
* @param  CRC input value
* @param  input byte
* @retval None
*/
unsigned short CYmodem::UpdateCRC16(unsigned short crcIn, unsigned char byte)
{
	unsigned int crc = crcIn;
	unsigned int in = byte | 0x100;
	do
	{
		crc <<= 1;
		in <<= 1;
		if (in & 0x100)
			++crc;
		if (crc & 0x10000)
			crc ^= 0x1021;
	} while (!(in & 0x10000));
	return crc & 0xffffu;
}

/**
* @brief  Cal CRC16 for YModem Packet
* @param  data
* @param  length
* @retval None
*/
unsigned short CYmodem::Cal_CRC16(const unsigned char* data, unsigned int size)
{
	unsigned int crc = 0;
	const unsigned char* dataEnd = data + size;
	while (data<dataEnd)
		crc = UpdateCRC16(crc, *data++);

	crc = UpdateCRC16(crc, 0);
	crc = UpdateCRC16(crc, 0);

	crc = crc & 0xffffu;

	crc = htons(crc);
	return crc;
}

/**
* @brief  Cal Check sum for YModem Packet
* @param  data
* @param  length
* @retval None
*/
unsigned char CYmodem::CalChecksum(const unsigned char* data, unsigned int size)
{
	unsigned int sum = 0;
	const unsigned char* dataEnd = data + size;
	while (data < dataEnd)
		sum += *data++;
	return sum & 0xffu;
}

void CYmodem::PrepareIntialPacket(unsigned char *data, const unsigned char* fileName, unsigned int *length)
{
	unsigned short i, j;
	unsigned char file_ptr[10] = { 0 };
	unsigned char a;

	/* Make first three packet */
	data[0] = SOH;
	data[1] = 0x00;
	data[2] = 0xFF;

	a = ((data[2] ^ 0xFF) & 0xFF);

	if (data[1] == a)
	{
		TRACE("SUCESS\r\n");
	}

	/* Filename packet has valid data */
	for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH); i++)
	{
		data[i + PACKET_HEADER] = fileName[i];
	}

	data[i + PACKET_HEADER] = 0x00;

	Int2Str(file_ptr, *length);
	for (j = 0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0'; )
	{
		data[i++] = file_ptr[j++];
	}

	for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
	{
		data[j] = 0;
	}
}

void CYmodem::SendPacket(const unsigned char* szBuff, DWORD dwBytesWrite)
{
	OVERLAPPED m_osWrite;
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	bWriteStat = WriteFile(m_hCom, szBuff,
		dwBytesWrite, &dwBytesWrite, &m_osWrite);

	if (!bWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent, 1000);
		}
	}
	CloseHandle(m_osWrite.hEvent);
	return;
}

BOOL CYmodem::ReadSerial(unsigned char* szBuff)
{
	BOOL bRecv = FALSE;
	OVERLAPPED m_osRead;
	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	COMSTAT ComStat;
	DWORD dwErrorFlags;

	static char szReadBuffer[SERIAL_BUF_SIZE];
	memset(szReadBuffer, '\0', SERIAL_BUF_SIZE);
	DWORD dwBytesRead = SERIAL_BUF_SIZE - 1;
	BOOL bReadStat;

	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	dwBytesRead = min(dwBytesRead, (DWORD)ComStat.cbInQue);

	bReadStat = ReadFile(m_hCom, szReadBuffer,
		dwBytesRead, &dwBytesRead, &m_osRead);
	if (!bReadStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osRead.hEvent, 2000);
		}
	}

	if (0 < dwBytesRead)
	{
		if (10 < dwBytesRead)
		{
			CString strRead;
			strRead = szReadBuffer;
			if (-1 == strRead.Find(_T("Failed to")))
			{
				szBuff[0] = szReadBuffer[0];
				bRecv = TRUE;
			}
		}
		else
		{
			szBuff[0] = szReadBuffer[0];
			bRecv = TRUE;
		}
	}

	CloseHandle(m_osRead.hEvent);
	return bRecv;
}
BOOL CYmodem::ReceiveByte(unsigned char* szBuff, unsigned int timeout)
{
	BOOL bRecv = FALSE;
	while (timeout-- > 0)
	{
		bRecv = ReadSerial(szBuff);
		if (TRUE == bRecv)
		{
			break;
		}

		Sleep(10);
	}
	return bRecv;
}

/**
* @brief  Prepare the data packet
* @param  timeout
*     0: end of transmission
*/
void CYmodem::PreparePacket(unsigned char *SourceBuf, unsigned char *data, unsigned char pktNo, unsigned int sizeBlk)
{
	unsigned short i, size, packetSize;
	unsigned char* file_ptr;

	/* Make first three packet */
	packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
	size = sizeBlk < packetSize ? sizeBlk : packetSize;
	if (packetSize == PACKET_1K_SIZE)
	{
		data[0] = STX;
	}
	else
	{
		data[0] = SOH;
	}
	data[1] = pktNo;
	data[2] = (~pktNo);
	file_ptr = SourceBuf;

	/* Filename packet has valid data */
	for (i = PACKET_HEADER; i < size + PACKET_HEADER; i++)
	{
		data[i] = *file_ptr++;
	}
	if (size <= packetSize)
	{
		for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
		{
			data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
		}
	}
}

unsigned char CYmodem::Transmit(unsigned char* buf, const  unsigned char* sendFileName, unsigned int sizeFile)
{
	unsigned char packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
	unsigned char FileName[FILE_NAME_LENGTH];
	unsigned char *buf_ptr, tempCheckSum;
	unsigned short CRC, blkNumber;
	unsigned char receivedC[2], CRC16_F = 0, i;
	unsigned int errors, ackReceived, size = 0, pktSize;
	unsigned char type;

	errors = 0;
	ackReceived = 0;
	for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
	{
		FileName[i] = sendFileName[i];
	}
	CRC16_F = 1;

	/* Prepare first block */
	PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);

	do
	{
		/* Send Packet */
		SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
		/* Send CRC or Check Sum based on CRC16_F */
		if (CRC16_F)
		{
			CRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);

			SendPacket((unsigned char*)&CRC, sizeof(CRC));
		}
		else
		{
			tempCheckSum = CalChecksum(&packet_data[3], PACKET_SIZE);
			SendPacket((unsigned char*)&tempCheckSum, sizeof(tempCheckSum));
		}
		
		/* Wait for Ack and 'C' */
		if (TRUE == ReceiveByte(&receivedC[0], 100) && receivedC[0] == ACK)
		{
			/* Packet transfered correctly */
			ackReceived = 1;
		}
		else
		{
			errors++;
		}
	} while (!ackReceived && (errors < 0x0A));

	if (errors >= 0x0A)
	{
		return errors;
	}
	buf_ptr = buf;
	size = sizeFile;
	blkNumber = 0x01;
	/* Here 1024 bytes package is used to send the packets */


	/* Resend packet if NAK  for a count of 10 else end of commuincation */
	while (size)
	{
		/* Prepare next packet */
		PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
		ackReceived = 0;
		receivedC[0] = 0;
		errors = 0;
		do
		{
			/* Send next packet */
			if (size >= PACKET_1K_SIZE)
			{
				pktSize = PACKET_1K_SIZE;

			}
			else
			{
				pktSize = PACKET_SIZE;
			}
			SendPacket(packet_data, pktSize + PACKET_HEADER);
			/* Send CRC or Check Sum based on CRC16_F */
			/* Send CRC or Check Sum based on CRC16_F */
			if (CRC16_F)
			{
				CRC = Cal_CRC16(&packet_data[3], pktSize);
				SendPacket((unsigned char*)&CRC, sizeof(CRC));
			}
			else
			{
				tempCheckSum = CalChecksum(&packet_data[3], pktSize);
				SendPacket((unsigned char*)&tempCheckSum, sizeof(tempCheckSum));
			}

			/* Wait for Ack */
			if ((TRUE == ReceiveByte(&receivedC[0], 500)) 
		     && ((receivedC[0] == ACK) || CRC16 == (receivedC[0])))
			{
				Sleep(10);
				ackReceived = 1;
				if (size > pktSize)
				{
					buf_ptr += pktSize;
					size -= pktSize;
					if (blkNumber == (FLASH_IMAGE_SIZE / 1024))
					{
						return 0xFF; /*  error */
					}
					else
					{
						blkNumber++;
					}
				}
				else
				{
					buf_ptr += pktSize;
					size = 0;
				}
			}
			else
			{
				unsigned char abort = 'a';
				SendPacket(&abort, sizeof(abort));
				errors = 0x0A;
				break;
			}
		} while (!ackReceived && (errors < 0x0A));
		/* Resend packet if NAK  for a count of 10 else end of commuincation */

		if (errors >= 0x0A)
		{
			return errors;
		}

	}
	ackReceived = 0;
	receivedC[0] = 0x00;
	errors = 0;
	do
	{
		type = EOT;
		SendPacket(&type, sizeof(type));
		
		/* Send (EOT); */
		/* Wait for Ack */
		if ((TRUE == ReceiveByte(&receivedC[0], 100)) && receivedC[0] == ACK)
		{
			ackReceived = 1;
		}
		else
		{
			errors++;
		}
	} while (!ackReceived && (errors < 0x0A));

	if (errors >= 0x0A)
	{
		return errors;
	}

	/* Last packet preparation */
	ackReceived = 0;
	receivedC[0] = 0x00;
	errors = 0;

	packet_data[0] = SOH;
	packet_data[1] = 0;
	packet_data[2] = 0xFF;

	for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
	{
		packet_data[i] = 0x00;
	}

	do
	{
		/* Send Packet */
		SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
		/* Send CRC or Check Sum based on CRC16_F */
		CRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
		SendPacket((unsigned char*)&CRC, sizeof(CRC));

		/* Wait for Ack and 'C' */
		if (TRUE == ReceiveByte(&receivedC[0], 100))
		{
			if (receivedC[0] == ACK)
			{
				/* Packet transfered correctly */
				ackReceived = 1;
			}
		}
		else
		{
			errors++;
		}

	} while (!ackReceived && (errors < 0x0A));
	/* Resend packet if NAK  for a count of 10  else end of commuincation */
	if (errors >= 0x0A)
	{
		return errors;
	}

	do
	{
		type = EOT;
		SendPacket(&type, sizeof(type));
		/* Send (EOT); */
		/* Wait for Ack */
		if ((TRUE == ReceiveByte(&receivedC[0], 100)) && receivedC[0] == ACK)
		{
			ackReceived = 1;
		}
		else
		{
			errors++;
		}
	} while (!ackReceived && (errors < 0x0A));

	if (errors >= 0x0A)
	{
		return errors;
	}
	return 0; /* file trasmitted successfully */
}