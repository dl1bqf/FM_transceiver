//*****************************************************************************
//*****************************************************************************
//  FILENAME: DELSIG8_1.h
//   Version: 3.2, Updated on 2015/3/4 at 22:25:37
//
//  DESCRIPTION:  C declarations for the DELSIG8 User Module with
//                a 2nd-order modulator.
//-----------------------------------------------------------------------------
//      Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef DELSIG8_1_INCLUDE
#define DELSIG8_1_INCLUDE

#include <m8c.h>

#define DELSIG8_1_POLL_ENABLE 1

#pragma fastcall16 DELSIG8_1_Start
#pragma fastcall16 DELSIG8_1_SetPower
#pragma fastcall16 DELSIG8_1_StartAD
#pragma fastcall16 DELSIG8_1_StopAD
#pragma fastcall16 DELSIG8_1_Stop

#if ( DELSIG8_1_POLL_ENABLE )
#pragma fastcall16 DELSIG8_1_fIsDataAvailable
#pragma fastcall16 DELSIG8_1_cGetData
#pragma fastcall16 DELSIG8_1_cGetDataClearFlag
#pragma fastcall16 DELSIG8_1_ClearFlag
#endif

//-------------------------------------------------
// Prototypes of the DELSIG8_1 API.
//-------------------------------------------------
extern void DELSIG8_1_Start(BYTE bPower);       // RAM use class 2
extern void DELSIG8_1_SetPower(BYTE bPower);    // RAM use class 2
extern void DELSIG8_1_StartAD(void);            // RAM use class 1
extern void DELSIG8_1_StopAD(void);             // RAM use class 1
extern void DELSIG8_1_Stop(void);               // RAM use class 1

#if ( DELSIG8_1_POLL_ENABLE )
extern BYTE DELSIG8_1_fIsDataAvailable(void);   // RAM use class 4
extern CHAR DELSIG8_1_cGetData(void);           // RAM use class 4
extern CHAR DELSIG8_1_cGetDataClearFlag(void);  // RAM use class 4
extern void DELSIG8_1_ClearFlag(void);          // RAM use class 4
#endif

//-------------------------------------------------
// Defines for DELSIG8_1 API's.
//-------------------------------------------------
#define DELSIG8_1_OFF         0
#define DELSIG8_1_LOWPOWER    1
#define DELSIG8_1_MEDPOWER    2
#define DELSIG8_1_HIGHPOWER   3

#define DELSIG8_1_DATA_READY_BIT  0x10
//-------------------------------------------------
// Hardware Register Definitions
//-------------------------------------------------
#pragma ioport  DELSIG8_1_TimerDR0: 0x024              //Time base Counter register
BYTE            DELSIG8_1_TimerDR0;
#pragma ioport  DELSIG8_1_TimerDR1: 0x025              //Time base Period value register
BYTE            DELSIG8_1_TimerDR1;
#pragma ioport  DELSIG8_1_TimerDR2: 0x026              //Time base CompareValue register
BYTE            DELSIG8_1_TimerDR2;
#pragma ioport  DELSIG8_1_TimerCR0: 0x027              //Time base Control register
BYTE            DELSIG8_1_TimerCR0;
#pragma ioport  DELSIG8_1_TimerFN:  0x124               //Time base Function register
BYTE            DELSIG8_1_TimerFN;
#pragma ioport  DELSIG8_1_TimerSL:  0x125               //Time base Input register
BYTE            DELSIG8_1_TimerSL;
#pragma ioport  DELSIG8_1_TimerOS:  0x126               //Time base Output register
BYTE            DELSIG8_1_TimerOS;

#pragma ioport  DELSIG8_1_AtoD1cr0: 0x088              //Analog block 1 control register 0
BYTE            DELSIG8_1_AtoD1cr0;
#pragma ioport  DELSIG8_1_AtoD1cr1: 0x089              //Analog block 1 control register 1
BYTE            DELSIG8_1_AtoD1cr1;
#pragma ioport  DELSIG8_1_AtoD1cr2: 0x08a              //Analog block 1 control register 2
BYTE            DELSIG8_1_AtoD1cr2;
#pragma ioport  DELSIG8_1_AtoD1cr3: 0x08b              //Analog block 1 control register 3
BYTE            DELSIG8_1_AtoD1cr3;
#pragma ioport  DELSIG8_1_AtoD2cr0: 0x098              //Analog block 2 control register 0
BYTE            DELSIG8_1_AtoD2cr0;
#pragma ioport  DELSIG8_1_AtoD2cr1: 0x099              //Analog block 2 control register 1
BYTE            DELSIG8_1_AtoD2cr1;
#pragma ioport  DELSIG8_1_AtoD2cr2: 0x09a              //Analog block 2 control register 2
BYTE            DELSIG8_1_AtoD2cr2;
#pragma ioport  DELSIG8_1_AtoD2cr3: 0x09b              //Analog block 2 control register 3
BYTE            DELSIG8_1_AtoD2cr3;

#endif
// end of file DELSIG8_1.h
