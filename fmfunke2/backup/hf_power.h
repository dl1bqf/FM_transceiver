//*****************************************************************************
//*****************************************************************************
//  FILENAME: HF_power.h
//   Version: 2.00, Updated on 2015/3/4 at 22:26:37                                          
//  Generated by PSoC Designer 5.4.3191
//
//  DESCRIPTION: HF_power User Module C Language interface file
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef HF_power_INCLUDE
#define HF_power_INCLUDE

#include <m8c.h>


/* Create pragmas to support proper argument and return value passing */
#pragma fastcall16  HF_power_Stop
#pragma fastcall16  HF_power_Start
#pragma fastcall16  HF_power_On
#pragma fastcall16  HF_power_Off
#pragma fastcall16  HF_power_Switch
#pragma fastcall16  HF_power_Invert
#pragma fastcall16  HF_power_GetState


//-------------------------------------------------
// Constants for HF_power API's.
//-------------------------------------------------
//
#define  HF_power_ON   1
#define  HF_power_OFF  0

//-------------------------------------------------
// Prototypes of the HF_power API.
//-------------------------------------------------
extern void  HF_power_Start(void);                                     
extern void  HF_power_Stop(void);                                      
extern void  HF_power_On(void);                                      
extern void  HF_power_Off(void);                                      
extern void  HF_power_Switch(BYTE bSwitch);
extern void  HF_power_Invert(void);                                         
extern BYTE  HF_power_GetState(void);                                         

//-------------------------------------------------
// Define global variables.                 
//-------------------------------------------------



#endif