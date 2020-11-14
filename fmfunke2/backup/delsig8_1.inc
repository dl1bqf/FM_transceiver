;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: DELSIG8_1.inc
;;   Version: 3.2, Updated on 2015/3/4 at 22:25:37
;;
;;  DESCRIPTION: Assembler declarations for the 8-bit Delta-Sigma A/D Converter
;;               User Module with a 2nd-order modulator.
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"


;--------------------------------------------------
; Constants for DELSIG8_1 API's.
;--------------------------------------------------

DELSIG8_1_INT_REG:                         equ  0e1h
DELSIG8_1_INT_MASK:                        equ  02h     ; Interrupt mask

DELSIG8_1_OFF:                      equ 00h
DELSIG8_1_LOWPOWER:                 equ 01h
DELSIG8_1_MEDPOWER:                 equ 02h
DELSIG8_1_HIGHPOWER:                equ 03h

DELSIG8_1_POLL_ENABLE:              equ 1
DELSIG8_1_DATA_READY_BIT:           equ 10h


;--------------------------------------------------
; Register Address Constants for DELSIG8_1
;--------------------------------------------------

DELSIG8_1_TimerDR0:            equ 0x24    ; Period reg
DELSIG8_1_TimerDR1:            equ 0x25    ; Down count reg
DELSIG8_1_TimerDR2:            equ 0x26    ; Compare reg
DELSIG8_1_TimerCR0:            equ 0x27    ; Control reg
DELSIG8_1_TimerFN:             equ 0x24    ; Function reg
DELSIG8_1_TimerSL:             equ 0x25    ; Input  select reg
DELSIG8_1_TimerOS:             equ 0x26    ; Output select reg

DELSIG8_1_AtoD1cr0:            equ 0x88    ; SC Block 1 Control Reg 0
DELSIG8_1_AtoD1cr1:            equ 0x89    ; SC Block 1 Control Reg 1
DELSIG8_1_AtoD1cr2:            equ 0x8a    ; SC Block 1 Control Reg 2
DELSIG8_1_AtoD1cr3:            equ 0x8b    ; SC Block 1 Control Reg 3
DELSIG8_1_AtoD2cr0:            equ 0x98    ; SC Block 2 Control Reg 0
DELSIG8_1_AtoD2cr1:            equ 0x99    ; SC Block 2 Control Reg 1
DELSIG8_1_AtoD2cr2:            equ 0x9a    ; SC Block 2 Control Reg 2
DELSIG8_1_AtoD2cr3:            equ 0x9b    ; SC Block 2 Control Reg 3


; end of file DELSIG8_1.inc
