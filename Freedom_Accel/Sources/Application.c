/*
 * Application.c
 *
 *  Created on: Mar 30, 2013
 *      Author: Erich Styger
 */

#include "Application.h"
#include "CDC1.h"

#define USE_PEX_COMPONENT  1
#define USE_FREEMASTER     1

#if !USE_PEX_COMPONENT
#include "MMA8451.h"
#else
#include "MMA1.h"
#endif
#include "LED1.h"
#include "LED2.h"
#include "LED3.h"
#include "FMSTR1.h"

#if USE_FREEMASTER
/* global variables for FreeMaster */
static uint8_t AccelX, AccelY, AccelZ;
static FMSTR_APPCMD_CODE cmd; /* application command */
static FMSTR_APPCMD_PDATA cmdDataP; /* pointer to application command data */
static FMSTR_SIZE cmdSize; /* size of application command arguments */
static uint8_t stimulatedVar; /* variable stimulated with FreeMASTER */
#endif
static bool enableLED = TRUE; /* if LED's are used or not */

void APP_Run(void) {
  uint8_t res=ERR_OK;
  uint8_t xyz[3];
  unsigned char buf[16];
  
  LED1_On();
  LED2_On();
  LED3_On();
#if USE_PEX_COMPONENT
  res = MMA1_Init();
  res = MMA1_SetFastMode(FALSE);
#else
  res = MMA8451_Init();
#endif
  (void)CDC1_SendString((unsigned char*)"Testing sensor values:\r\n");
  while (res==ERR_OK) {
#if USE_PEX_COMPONENT
    xyz[0] = MMA1_GetX()>>8;
    xyz[1] = MMA1_GetY()>>8;
    xyz[2] = MMA1_GetZ()>>8;
#else
    res = MMA8451_GetRaw8XYZ(&xyz[0]);
#endif
    if (enableLED) {
      LED1_Put(xyz[0]>50);
      LED2_Put(xyz[1]>50);
      LED3_Put(xyz[2]>50);
    }
#if USE_FREEMASTER
    /* stimulated variable by FreeMaster: turn OFF LEDs if variable is greater than 0 */
    if (stimulatedVar>0) { /* turn off LEDs */
      LED1_Off();
      LED2_Off();
      LED3_Off();
    }
    /* check application commands */
    cmd = FMSTR1_GetAppCmd();
    if (cmd!=FMSTR_APPCMDRESULT_NOCMD) { /* received command */
      cmdDataP = FMSTR1_GetAppCmdData(&cmdSize);
      if (cmd==0 && cmdSize==1) { /* ToggleLED_cmd (id: 0) has just one byte argument */
        switch(*cmdDataP) {
          case 0: 
            LED1_Neg();
            FMSTR1_AppCmdAck(0); /* acknowledge the command */
            break;
          case 1: 
            LED2_Neg();
            FMSTR1_AppCmdAck(0); /* acknowledge the command */
            break;
          case 2: 
            LED3_Neg();
            FMSTR1_AppCmdAck(0); /* acknowledge the command */
            break;
          default: 
            FMSTR1_AppCmdAck(1); /* failed */
            break;
        }
      } else {
        FMSTR1_AppCmdAck(1); /* acknowledge the command with failure code */
      }
    }
    /* FreeMaster needs global variables... */
    AccelX = xyz[0];
    AccelY = xyz[1];
    AccelZ = xyz[2];
    FMSTR1_Poll();
    FMSTR1_Recorder();
    
    UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AccelX: ");
    UTIL1_strcatNum8u(buf, sizeof(buf), AccelX);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
    (void)CDC1_SendString(buf);
    UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AccelY: ");
    UTIL1_strcatNum8u(buf, sizeof(buf), AccelY);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
    (void)CDC1_SendString(buf);
    UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AccelZ: ");
    UTIL1_strcatNum8u(buf, sizeof(buf), AccelZ);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
    (void)CDC1_SendString(buf);
#endif
  }
  /* get here if things were failing... */
  LED1_Off();
  LED2_Off();
  LED3_Off();
}
