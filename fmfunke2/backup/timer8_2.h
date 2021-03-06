//*****************************************************************************
//*****************************************************************************
//  FILENAME: Timer8_2.h
//   Version: 2.70, Updated on 2015/3/4 at 22:27:48
//  Generated by PSoC Designer 5.4.3191
//
//  DESCRIPTION: Timer8 User Module C Language interface file
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef Timer8_2_INCLUDE
#define Timer8_2_INCLUDE

#include <m8c.h>

#pragma fastcall16 Timer8_2_EnableInt                 //                 Proxy 1
#pragma fastcall16 Timer8_2_DisableInt                //                 Proxy 1
#pragma fastcall16 Timer8_2_Start                     //                 Proxy 1
#pragma fastcall16 Timer8_2_Stop                      //                 Proxy 1
#pragma fastcall16 Timer8_2_bReadTimer                // Read  DR0       Proxy 1
#pragma fastcall16 Timer8_2_bReadTimerSaveCV          // Read  DR0       Proxy 2
#pragma fastcall16 Timer8_2_WritePeriod               // Write DR1       Proxy 1
#pragma fastcall16 Timer8_2_bReadCompareValue         // Read  DR2       Proxy 1
#pragma fastcall16 Timer8_2_WriteCompareValue         // Write DR2       Proxy 1

// The following symbols are deprecated.
// They may be omitted in future releases
//
#pragma fastcall16 bTimer8_2_ReadCounter              // Read  DR0 "Obsolete"
#pragma fastcall16 bTimer8_2_CaptureCounter           // Read  DR0 "Obsolete"
#pragma fastcall16 bTimer8_2_ReadTimer                // Read  DR0 (Deprecated)
#pragma fastcall16 bTimer8_2_ReadTimerSaveCV          // Read  DR0 (Deprecated)
#pragma fastcall16 bTimer8_2_ReadCompareValue         // Read  DR2 (Deprecated)


//-------------------------------------------------
// Prototypes of the Timer8_2 API.
//-------------------------------------------------

extern void Timer8_2_EnableInt(void);
extern void Timer8_2_DisableInt(void);
extern void Timer8_2_Start(void);
extern void Timer8_2_Stop(void);
extern BYTE Timer8_2_bReadTimer(void);
extern BYTE Timer8_2_bReadTimerSaveCV(void);
extern void Timer8_2_WritePeriod(BYTE bPeriod);
extern BYTE Timer8_2_bReadCompareValue(void);
extern void Timer8_2_WriteCompareValue(BYTE bCompareValue);

// The following functions are deprecated.
// They may be omitted in future releases
//
extern BYTE bTimer8_2_ReadCompareValue(void);       // Deprecated
extern BYTE bTimer8_2_ReadTimerSaveCV(void);        // Deprecated
extern BYTE bTimer8_2_ReadCounter(void);            // Obsolete
extern BYTE bTimer8_2_ReadTimer(void);              // Deprecated
extern BYTE bTimer8_2_CaptureCounter(void);         // Obsolete


//--------------------------------------------------
// Constants for Timer8_2 API's.
//--------------------------------------------------

#define Timer8_2_CONTROL_REG_START_BIT         ( 0x01 )
#define Timer8_2_INT_REG_ADDR                  ( 0x0e1 )
#define Timer8_2_INT_MASK                      ( 0x20 )


//--------------------------------------------------
// Constants for Timer8_2 user defined values
//--------------------------------------------------

#define Timer8_2_PERIOD                        ( 0x1f )
#define Timer8_2_COMPARE_VALUE                 ( 0x00 )


//-------------------------------------------------
// Register Addresses for Timer8_2
//-------------------------------------------------

#pragma ioport  Timer8_2_COUNTER_REG:   0x034              //DR0 Count register
BYTE            Timer8_2_COUNTER_REG;
#pragma ioport  Timer8_2_PERIOD_REG:    0x035              //DR1 Period register
BYTE            Timer8_2_PERIOD_REG;
#pragma ioport  Timer8_2_COMPARE_REG:   0x036              //DR2 Compare register
BYTE            Timer8_2_COMPARE_REG;
#pragma ioport  Timer8_2_CONTROL_REG:   0x037              //Control register
BYTE            Timer8_2_CONTROL_REG;
#pragma ioport  Timer8_2_FUNC_REG:  0x134                  //Function register
BYTE            Timer8_2_FUNC_REG;
#pragma ioport  Timer8_2_INPUT_REG: 0x135                  //Input register
BYTE            Timer8_2_INPUT_REG;
#pragma ioport  Timer8_2_OUTPUT_REG:    0x136              //Output register
BYTE            Timer8_2_OUTPUT_REG;
#pragma ioport  Timer8_2_INT_REG:       0x0e1              //Interrupt Mask Register
BYTE            Timer8_2_INT_REG;


//-------------------------------------------------
// Timer8_2 Macro 'Functions'
//-------------------------------------------------

#define Timer8_2_Start_M \
   ( Timer8_2_CONTROL_REG |=  Timer8_2_CONTROL_REG_START_BIT )

#define Timer8_2_Stop_M  \
   ( Timer8_2_CONTROL_REG &= ~Timer8_2_CONTROL_REG_START_BIT )

#define Timer8_2_EnableInt_M   \
   M8C_EnableIntMask(  Timer8_2_INT_REG, Timer8_2_INT_MASK )

#define Timer8_2_DisableInt_M  \
   M8C_DisableIntMask( Timer8_2_INT_REG, Timer8_2_INT_MASK )

#endif
// end of file Timer8_2.h
