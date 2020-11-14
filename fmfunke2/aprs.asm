;include 'm8c.inc'
;include 'memory.inc'
;include 'PSoCAPI.inc'
	;export 	_aprs
	export  _bit_counter
	export  _bit_buffer
	export  _SINtable
area 	bss(RAM)

;-----------------------------------------------------------------------------
;  Defines
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;  Private data: 
;-----------------------------------------------------------------------------
_bit_buffer:         blk  1
_bit_counter:         blk  1
;data_counter:        blk 1
area 	text(ROM,REL)

_aprs:       ret
;             call           testFlag
;			 cmp            [bit_buffer],0
;			 jnz            deadress
;			 ret
;deadress:    mov            [data_counter],7
;             ret
;---------------------------------------------------------------------------------
;
;            Testet ob      UI Frame erkannt ist ---> ist bit_buffer =7e dann ready
;------------------------------------------------------------------------------------
;testFlag:    inc        	[clock] 
;			 cmp            [clock],8
;			 jz             clockfull
;			 ret
;clockfull:   mov            a,reg[PRT1DR]
;             and            a,1  
			
;			 asl            [bit_buffer]
;			 add            [bit_buffer],a
;			 mov            [clock],0
;            inc            [bit_counter]
;             cmp            [bit_counter],8
;			 jz              bytefull
;			 ret
;bytefull:     
;             mov            [clock],0
;             cmp            [bit_buffer],0x7e
;			  jz             aprsflag_ready
;			  mov            [bit_buffer],0
;			  ret
;aprsflag_ready:    mov            a,48
;              lcall          LCD_1_WriteData
;              ret
;-----------------------------------------------------------------------------------------
;
;   holt daten in den Datenbuffer anzahl steht im Data_counter  
;------------------------------------------------------------------------------------------
;get_aprs_data:
;              push            a
;			  push            x
;g1:           inc             [clock]
;              cmp             [clock],8
;			  jnz             g1
;			  
;			  ret
