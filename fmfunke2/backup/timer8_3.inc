;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: Timer8_3.inc
;;   Version: 2.70, Updated on 2015/3/4 at 22:27:48
;;  Generated by PSoC Designer 5.4.3191
;;
;;  DESCRIPTION: Assembler declarations for the Timer8 user module interface
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"


;--------------------------------------------------
; Constants for Timer8_3 API's.
;--------------------------------------------------

Timer8_3_CONTROL_REG_START_BIT:            equ 0x01    ; Control register start bit
Timer8_3_INT_REG:                          equ 0x0e1
Timer8_3_INT_MASK:                         equ 0x10


;--------------------------------------------------
; Constants for Timer8_3 user defined values
;--------------------------------------------------

Timer8_3_PERIOD:                           equ 0x00
Timer8_3_COMPARE_VALUE:                    equ 0x00


;--------------------------------------------------
; Register Address Constants for Timer8_3
;--------------------------------------------------

Timer8_3_COUNTER_REG:                  equ 0x30   ; DR0 Counter register
Timer8_3_PERIOD_REG:                   equ 0x31   ; DR1 Period register
Timer8_3_COMPARE_REG:                  equ 0x32   ; DR2 CompareValue register
Timer8_3_CONTROL_REG:                  equ 0x33   ; Control register
Timer8_3_FUNC_REG:                     equ 0x30   ; Function register
Timer8_3_INPUT_REG:                    equ 0x31   ; Input register
Timer8_3_OUTPUT_REG:                   equ 0x32   ; Output register


;--------------------------------------------------
; Timer8_3 Macro 'Functions'
;--------------------------------------------------

   macro Timer8_3_Start_M
   or    reg[Timer8_3_CONTROL_REG],  Timer8_3_CONTROL_REG_START_BIT
   endm

   macro Timer8_3_Stop_M
   and   reg[Timer8_3_CONTROL_REG], ~Timer8_3_CONTROL_REG_START_BIT
   endm

   macro Timer8_3_EnableInt_M
   M8C_EnableIntMask Timer8_3_INT_REG, Timer8_3_INT_MASK
   endm

   macro Timer8_3_DisableInt_M
   M8C_DisableIntMask Timer8_3_INT_REG, Timer8_3_INT_MASK
   endm


; end of file Timer8_3.inc