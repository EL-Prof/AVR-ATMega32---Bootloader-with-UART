/*
 * Flashing_Mngr.c
 *
 *  Created on: Jun 6, 2020
 *      Author: elProf
 */

#include "STD_Types.h"
#include "Flashing_Mngr.h"
#include "avr/boot.h"
#include "UART.h"
#include "DIO.h"
#include "App.h"
#include "util/crc16.h"
#include "avr/pgmspace.h"
#include "avr/delay.h"


#define SESSION_CONTROL			(0X10)
#define PROGRAMMING_SESSION	(0x03)
#define DOWNLOAD_REQUEST		(0X34)
#define TRANSFER_DATA				(0X36)
#define TRANSFER_EXIT				(0X37)
#define CHECK_CRC						(0X31)
#define MAX_CODE_SIZE				(0x3800)
#define PAGE_SIZE						(128) /*64 word*/



typedef enum {
waiting_ProgrammingSession,
waiting_DownloadRequest,
waiting_TransferData,
waiting_TransferExit,
waiting_CheckCRC
}downloadStates ;

static void LOC_vidSendNegResp(void);
static void LOC_vidSendPosResp(void);
static u8 LOC_vidCheckFlashCRC(u16 u16StartAdd, u16 u16EndAdd, u16 u16CRC);
static boolean bIsRequestReceived = FALSE  ;
static u8 u8SID;
static u8 u8ReqLen;
static u8 *pu8ReqData ;
static downloadStates enuDownloadState;

void boot_program_page (uint16_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;
	uint32_t address;
	uint16_t word ;

	// Disable interrupts.
	sreg = SREG;
	asm("cli");
	address = page * SPM_PAGESIZE ;

	boot_page_erase_safe (address);

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little Endian word.
		word  = *buf++;
		word += (*buf++) << 8;

		boot_page_fill_safe(address + i, word);
	}

	boot_page_write_safe (address);     // Store buffer in flash page.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable_safe();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
}


void flashingMngr_vidMainTask()
{
	static u16 u16CodeSize , u16ReceivedLen = 0 ;
	static u8 u8PageNo = 0 ;
	boolean bValidReq = FALSE, bValidCRC ;
	u16 u16ReceivedCRC ;

	if(bIsRequestReceived)
	{
		switch(u8SID)
		{
			case SESSION_CONTROL:
			{
				if(pu8ReqData[0] == PROGRAMMING_SESSION && (u8ReqLen == 2) &&
						(enuDownloadState ==waiting_ProgrammingSession ))
				{
					/*Programming session, Valid request*/
					LOC_vidSendPosResp();
					enuDownloadState = waiting_DownloadRequest ;
				}
				else
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;

			case DOWNLOAD_REQUEST:
			{
				if((enuDownloadState == waiting_DownloadRequest) &&
						(u8ReqLen == 3))
				{
					/*Valid request*/
					/*Big endian 2 bytes code size*/
					u16CodeSize = pu8ReqData[0]<<8 | pu8ReqData[1] ;
					if(u16CodeSize < MAX_CODE_SIZE)
					{
						LOC_vidSendPosResp();
						enuDownloadState = waiting_TransferData ;
						bValidReq = TRUE ;
					}
				}
				if (bValidReq != TRUE)
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;

			case TRANSFER_DATA:
			{
				if((enuDownloadState ==waiting_TransferData) &&
						(u8ReqLen == PAGE_SIZE+1))
				{
					/*Write flash page, starting from index 1 [neglect SID] */
					boot_program_page(u8PageNo ,&pu8ReqData[0]);
					LOC_vidSendPosResp();
					u8PageNo ++ ;
					u16ReceivedLen += PAGE_SIZE ;
					if(u16ReceivedLen == u16CodeSize)
					{
						/*Download is Done*/
						enuDownloadState = waiting_TransferExit;
					}
					else
					{
						/*Download is in progress .. */
					}
				}
				else
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;
			case TRANSFER_EXIT:
			{
				if((enuDownloadState == waiting_TransferExit) &&
									(u8ReqLen == 1))
				{
					LOC_vidSendPosResp();
					enuDownloadState = waiting_CheckCRC;
				}
				else
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;
			case CHECK_CRC:
			{
				if((enuDownloadState == waiting_CheckCRC) &&
									(u8ReqLen == 3))
				{
					u16ReceivedCRC = pu8ReqData[0]<<8 | pu8ReqData[1] ;
					/*Validate CRC of flashed code, in range of 0~codeSize*/
					bValidCRC = LOC_vidCheckFlashCRC(0, u16CodeSize, u16ReceivedCRC);
					if(bValidCRC)
					{
						LOC_vidSendPosResp();
						/*Mark application as Valid*/
						eeprom_update_byte (VALID_APP_ADDRESS, 1);
#if USE_INTERRUPT ==1
						/*Move vector Table to Application section*/
						APP_vidMoveIVT(APP_SECTION);
#endif
						_delay_ms(100) ;
						/*start the actual program*/
						asm("jmp 0");
					}
					else
					{
						/*Invalid request, or sequence error*/
						LOC_vidSendNegResp();
						/*TODO: specify NRC*/
					}
				}
				else
				{
					/*Invalid request, or sequence error*/
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}

				enuDownloadState = waiting_ProgrammingSession;
			}break;

			default:
			{
				/*Unknown SID*/
				/*Invalid request, or sequence error*/
				/*Reset download state*/
				enuDownloadState = waiting_ProgrammingSession ;
				LOC_vidSendNegResp();
				/*TODO: specify NRC*/
			}
		}
		bIsRequestReceived = FALSE ;
	}
}



extern void flashing_vidRxNotification(u8* pu8Data , u8 u8len)
{
	bIsRequestReceived = TRUE ;
	u8SID = pu8Data[0];
	pu8ReqData = &pu8Data[1] ;
	u8ReqLen = u8len ;
}


void flashingMngr_vidHandleReqFromApp(void)
{
	/*Programming session has been sent to Application [Reprogramming request]*/
	u8SID = 0x10 ;
	LOC_vidSendPosResp();
	enuDownloadState = waiting_DownloadRequest ;
}


void LOC_vidSendNegResp(void)
{
	/*TODO : handle NRCs*/
	UART_SendChar(0x7F) ;
}

void LOC_vidSendPosResp(void)
{
	UART_SendChar(u8SID + 0x40);
}

u8 LOC_vidCheckFlashCRC(u16 u16StartAdd, u16 u16EndAdd, u16 u16CRC)
{
    u16 addr;
    u8  u8Byte;
    u16 CRC16 = 0xFFFF ;

    /* Compute the CRC */
    for(addr = u16StartAdd; addr < u16EndAdd; addr++)
    {
    	u8Byte = pgm_read_byte(addr);
      CRC16 = _crc16_update(CRC16, u8Byte);
    }

    /*Compare calculated CRC with received one*/
    if(u16CRC != CRC16 )
    {
        return 0; /* Bad CRC */
    }
    else
    {
        return 1 ; /* Good CRC */
    }
}


