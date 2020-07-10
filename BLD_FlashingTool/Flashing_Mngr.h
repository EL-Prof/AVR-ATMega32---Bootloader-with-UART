/*
 * Flashing_Mngr.h
 *
 *  Created on: Jun 8, 2020
 *      Author: elProf
 */

#ifndef FLASHING_MNGR_H_
#define FLASHING_MNGR_H_


#define VALID_APP_ADDRESS			((u8*) 0x00)
#define REQ_FROM_APP_ADDRESS	((u8*) 0x01)

void flashingMngr_vidMainTask() ;
void flashingMngr_vidHandleReqFromApp(void);

#endif /* FLASHING_MNGR_H_ */
