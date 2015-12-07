/*
 * Application.c
 *
 *  Created on: Jul 1, 2013
 *      Author: Erich Styger
 */

#include "Application.h"
#include "SW1.h"
#include "LEDR.h"
#include "LEDG.h"
#include "LEDB.h"
#include "WAIT1.h"
#include "HIDK2.h"
#include "MMA1.h"
#include "mpu6050.h"

static uint8_t AccelX, AccelY, AccelZ;

void APP_Run(void) {
  int cnt=0; /* counter to slow down LED blinking */
  
  /*Init Acceleroeter*/
  uint8_t res=ERR_OK;
  uint8_t xyz[3];
  uint8_t speed = 1;
  
  
  //res = MPU6050_Init();
  res = MMA1_Init();
  res = MMA1_SetFastMode(FALSE);
  
  for(;;) {
    WAIT1_Waitms(10);
    cnt++;
    if (HIDK2_App_Task()==ERR_BUSOFF) {
      if ((cnt%128)==0) { /* just slow down blinking */
        //LEDG_Off();
        //LEDR_Neg();
      }
    } else {
      if ((cnt%128)==0) { /* just slow down blinking */
        //LEDR_Off();
        //LEDG_Neg();
      }
      
      //AccelX = MPU6050_GetGyX() >> 8;
      AccelX = MMA1_GetX()>>8;
      AccelY = MMA1_GetY()>>8;
      AccelZ = MMA1_GetZ()>>8;
      WAIT1_Waitms(100);
      
      //Down movement
      if(AccelX < 5 && AccelY > 10 && AccelZ > 200)
      {
    	  LEDR_Off();
    	  LEDG_Off();
    	  LEDB_Off();
    	  (void)HIDK2_Move(speed, 0);
    	  LEDB_Neg();
    	  speed++;
    	  
      }
      //Up movement
	  else if(AccelX < 2  && AccelY > 10 && AccelZ < 20)
	  {
		  LEDR_Off();
		  LEDG_Off();
		  LEDB_Off();
		  (void)HIDK2_Move(-speed, 0);
		  LEDG_Neg();
		  LEDR_Neg();
		  speed++;
	  }
      //Left Movement
      else if(AccelX > 200 && AccelY < 15 && AccelZ < 15)
      {
    	  LEDR_Off();
    	  LEDG_Off();
    	  LEDB_Off();
    	  (void)HIDK2_Move(0, -speed);
    	  LEDG_Neg();
    	  speed++;
      }
      //Right movement
      else if(AccelX < 15 && AccelY < 15 && AccelZ < 5)
      {
    	  LEDR_Off();
    	  LEDG_Off();
    	  LEDB_Off();
    	  (void)HIDK2_Move(0, speed);
    	  LEDR_Neg();
    	  speed++;
      }
      //Init
		else if (AccelX < 5 && AccelY < 20 && AccelZ < 5) {
			LEDR_On();
			LEDG_On();
			LEDB_On();
			speed = 1;
		}
      else
      {
    	  LEDR_Off();
    	  LEDG_Off();
    	  LEDB_Off();
      }

#if 0 
      if (SW1_GetVal()==0) { /* button pressed */
        WAIT1_Waitms(100); /* wait for debouncing */
        if (SW1_GetVal()==0) { /* still pressed */
          //(void)HIDK2_Send(0, 8, 8); /* raw move message */
          
          //(void)HIDK2_Send(HIDK2_MOUSE_LEFT, 0, 0); /* send left mouse button */
          //(void)HIDK2_Send(0, 0, 0); /* send release button message */
          
          //(void)HIDK2_Move(-8, 0); /* moving the cursor up */
          
          //HIDK2_Click(HIDK2_MOUSE_LEFT); /* left mouse click */
          
          HIDK2_Click(HIDK2_MOUSE_RIGHT); /* right mouse click */
        }
        while(SW1_GetVal()==0) {} /* wait until button is released */
      }
#endif
    }
  }
}
