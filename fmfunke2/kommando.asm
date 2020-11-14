include 'm8c.inc'
include 'memory.inc'
include 'PSoCAPI.inc'
	export 	_Kommando
    export  _kommando_setvol
	export  _responsebuffer
	export  _kommando_heartbeat
	export  _kommando_rssi
	export  _kommando_rufton
	export  _kommando_setfilter
	export  _wait_ptt
	export  _lcdInit
	export  _clear_LCD
	export  _setgroup_response
	export   lastPrt1
	export   ton_on1
    export  _counter	
	export   bTablePos
	export   w3

	
	
area 	bss(RAM)

;-----------------------------------------------------------------------------
;  Defines
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;  Private data: 
;-----------------------------------------------------------------------------

_responsebuffer:      blk 20
speicher:             blk 1
lastPrt1:             blk 1
bTablePos:            blk 1
counter1:             blk 1
_counter:             blk 1
area 	text(ROM,REL)

_clear_LCD:      push       a
                 push       x
                 mov        a,1
                 lcall      LCD_1_Control
				 pop        x
			     pop        a
				 ret
_lcdInit:   
                 lcall    LCD_1_Start;
				 call      _clear_LCD
   				 mov    	A, >Str_Welcome      
   				 mov    	X, <Str_Welcome
   				 lcall  	LCD_1_PrCString   
				 ;mov        a,reg[PRT0DR]
                 ;and        a,24
				 ;mov        [lastPrt0],a
				 mov        a,reg[PRT1DR]
				 and        a,192
				 mov        [lastPrt1],a
				 ;M8C_SetBank1
				 ;and        reg[ABF_CR0],0xe2
				 ;M8C_SetBank0
				 ret
				 
; show the RSSI on the Character Display  Argument 1 : how many rows display in the variable Counter1  max: 8 rows
;                                         return : none
;
display_pegel:   
                 mov        a,1
				 mov        x,12
				 lcall      LCD_1_Position
				 mov        a,[counter1]
				 push       a
l45:             
                 mov        a,0xff
				 lcall      LCD_1_WriteData				 
				 dec        [counter1]
				 cmp        [counter1],0
				 jnz        l45
				 pop        A
				 mov        [counter1],a
				 mov        a,8
				 sub        a,[counter1]
				 mov        [counter1],a
				 
l43:		     mov        A,0x20
                 lcall      LCD_1_WriteData
				 dec        [counter1]
				 cmp        [counter1],0
				 jnz        l43
				 ret
; command for DRA818 set filter    Argument : filter value  0-3 in the variable _filter
;                                  return   : response from the DRA818 in the Buffer _response_data 1 -> 14
_kommando_setfilter:
                 mov      a,>dmofilter
	             mov      x,<dmofilter
                 lcall    UART_1_CPutString
				 cmp      [_filter],0
				 jnz      f1
				 mov      a,>fil0
				 mov      x,<fil0
				 lcall    UART_1_CPutString
				 jmp      f5
f1:				 cmp      [_filter],1
                 ;cmp      a,1
                 jnz      f2
				 mov      a,>fil1
				 mov      x,<fil1
				 lcall    UART_1_CPutString
				 jmp      f5
f2:				 cmp      [_filter],2
                 ;cmp      a,2
                 jnz      f3
				 mov      a,>fil2
				 mov      x,<fil2
                 lcall    UART_1_CPutString
				 jmp      f5
f3:				 cmp      [_filter],3
                 ;cmp      a,3
				 jz       f4
				 ret
f4:				  mov     a,>fil3
				 mov      x,<fil3
				 lcall    UART_1_CPutString
f5:				 lcall    UART_1_PutCRLF
                 
				 mov      a,16
				 call    responsedata
				 ;mov        a,1
				 ;mov        x,0
				 ;lcall     LCD_1_Position
				 ;mov        a ,>_responsebuffer
				 ;mov        x ,<_responsebuffer
				 ;lcall      LCD_1_PrString
				 cmp       [_responsebuffer+14],0x30
		         jnz        end9
				 cmp        [_responsebuffer+13],0x3a
				 jnz        end9
				 ret
end9:			 mov        a,1    		; Set row
   	             mov      	X, 0			; Set col
                 lcall	    LCD_1_Position
				 mov    	A, >filterror       
                 mov    	X, <filterror
  			 	 lcall  	LCD_1_PrCString 
				 ret		
; command for DRA818 set Volume    Argument : value 1-8 in the variable _volume
;                                  return   : response from the DRA818 in the Buffer _response_data 1 -> 14				 
_kommando_setvol:
                 mov      a,>dmovolume
	             mov      x,<dmovolume
                 lcall    UART_1_CPutString
                 mov      a,[_volume]
				 add      a,48
				 lcall    UART_1_PutChar
				 lcall      UART_1_PutCRLF
				 mov        a,15
				 call       responsedata
				 cmp        [_responsebuffer+14],0x30
		         jnz        end6
				 cmp        [_responsebuffer+13],0x3a
				 jz         print_volume
end6:			 mov 	    A, 1	      		; Set row
   	             mov      	X, 0 			; Set col
                 lcall	    LCD_1_Position
				 mov    	A, >voerror       
                 mov    	X, <voerror
  			 	 lcall  	LCD_1_PrCString 
print_volume:    ret			
;print_vol:        mov        a,0
;                 mov        x,12
;				 lcall      LCD_1_Position
;				 mov        a,>batstr
;				 mov        x,<batstr
;				 lcall      LCD_1_PrCString
;				 mov        a,[_volt]
;				 add        a,48
;				 lcall      LCD_1_WriteData
;				 mov        a,0x20
;				 lcall      LCD_1_WriteData
;                 ret				


; command for DRA818 Connect       Argument : none
;                                  return   : response from the DRA818 in the Buffer _response_data 1 -> 11

_kommando_heartbeat:
                 mov      a,>heartbeat
	             mov      x,<heartbeat
                 lcall    UART_1_CPutString
				 lcall    UART_1_PutCRLF
				 mov      a,13
				 call      responsedata
				 cmp      [_responsebuffer+11],48
		         jnz       end5
				 cmp      [_responsebuffer+10],0x3a
				 jz        end1
end5:	     	 mov    	A, 1	      		; Set row
   	             mov    	X, 0 			; Set col
                 lcall  	LCD_1_Position
				 mov    	A, >coerror    
                 mov    	X, <coerror
  			 	 lcall  	LCD_1_PrCString 
end1:            ret
; command for DRA818 get RSSI      Argument : none
;                                  return   : response from the DRA818 in the Buffer _response_data 1 -> 8
; function display the RSSI in Row 2 at the display
;
_kommando_rssi:
                 mov      a,>rssi
				 mov      x,<rssi
				 lcall     UART_1_CPutString
				 lcall     UART_1_PutCRLF
				 mov        a,8
				 call       responsedata
				 mov    	A, 1	      		; Set row
   	             mov    	X, 0 			; Set col
                 lcall  	LCD_1_Position
				 mov        a,1
				 mov        x,0
l76:			 push       a
                 mov        a,[x+_responsebuffer]
                 lcall      LCD_1_WriteData
				 pop        a
				 inc        x
                 inc        a
				 cmp        a,9
				 jnz        l76
				 mov        a,0x20
				 lcall      LCD_1_WriteData
				 mov        a,100
				 lcall      LCD_1_WriteData
				 mov        a,98
				 lcall      LCD_1_WriteData
				 mov        a,0x20
				 lcall      LCD_1_WriteData
				 mov        [counter1],0
				 mov        a,[5+_responsebuffer]
				 
				 sub        a,48
				 cmp        a,0
				 jz         x1
				 mov        reg[MUL_X],a
				 mov        reg[MUL_Y],100
				 mov        a,reg[MUL_DL]
				 mov        [counter1],a
x1:              mov        a,[6+_responsebuffer]
             
				 sub        a,48		
				 cmp        a,0
				 jz         x2
				 mov        reg[MUL_X],a
				 mov        reg[MUL_Y],10
				 mov        a,reg[MUL_DL]	
				
x2:				 add        a,[counter1]
				 swap       a,[counter1]
				 mov        a,[7+_responsebuffer]
				 sub        a,48		
				 add        a,[counter1]
				 
	             and        F,0xfb
				 rrc        A
				 and        F,0xfb    
				 rrc        A
				 and        F,0xfb
				 rrc        A
				 and        F,0xfb
				 rrc        A
				 mov        [counter1],a
				 call       display_pegel

				 ret	
; command for Psoc that initate 1750 hz sub tone   
;                                  Argument : none
;                                  return   : none			 
_kommando_rufton:
                 push      a
                 mov       [bTablePos],0
				 ;or         reg[ASC21CR0],64
                 and        reg[ACB01CR1],0x7f
                 or         reg[ASC21CR2],0x80
				 M8C_ClearWDT
                 mov       a,3
                 lcall     DAC8_1_Start
                 lcall     Timer8_3_EnableInt
				 mov       a,215
				 lcall     Timer8_3_WritePeriod
                 lcall     Timer8_3_Start
				 call      w3
				 lcall     DAC8_1_Stop
				 lcall     Timer8_3_Stop
				 and       reg[ASC21CR2],0x7f
				 or        reg[ACB01CR1],0x80
				 ;and       reg[ASC21CR0],0xbf
				 pop       a
				 ret
w3:				 push       a
                 mov       [counter1],255
w2:				 M8C_ClearWDT
				 lcall     _warte1ms
				 dec       [counter1]
				 ;cmp       [counter1],0
				 jnz       w2
				 pop       a
				 ret		
; function for write a Value from Softdds to the Dac
; argument       : variable bTablepos for index in the Sinus table
; return         : none
ton_on1:  
                 mov   a,[bTablePos]
                 cmp   a,16
                 jnz   l1
                 mov   a,0
                 mov   [bTablePos],a   
l1:              
				 index  SINtable
                 lcall  DAC8_1_WriteBlind
                 inc   [bTablePos] 
                ; mov   a,[_counter]
                ; cmp   a,00
                ; jnz   ton_on1
                 ret
; function for wait until the PTT button down
; argument: none
; return  : none
_wait_ptt:       M8C_ClearWDT
                 tst    reg[PRT1DR],0x20
                 jz     _wait_ptt
				 ret
				 
_setgroup_response:
                 
                 mov      a,14
; function  read the response from the DRA 818 Chip 
; argument : how many bytes to read in the A Register
responsedata:
                 mov      [speicher],a
                 lcall    UART_1_cGetChar 
				 mov      x,0
loop:            lcall    UART_1_cGetChar
                 mov      [x+_responsebuffer],a
				 inc      x
				 mov      a,x
				 cmp      a,[speicher]
				 jnz      loop
				 ret				 	
;

                 

;--------------------------------------------------------------------------------
;
; String Table
;
;--------------------------------------------------------------------------------
.LITERAL 

Str_Welcome:     ds     "VHF FM Funke DL1BQF V1.01" ;welcome Message on Display at Device Start
                 db     00
; do not change this area 

;############################################################################################
dmovolume:	     DS 	"AT+DMOSETVOLUME="
	             DB 	00
dmofilter:		 ds     "AT+SETFILTER="
                 db     00
voerror:		 ds     "Vo"
                 db     00
filterror:       ds     "Fi"
                 db     00
;batstr:          ds     " Bat:"
 ;                db     00
;tailerror:		 ds     "Ta"
 ;                db     00
heartbeat:		 DS	    "AT+DMOCONNECT"
                 db     00
coerror:         ds     "Co"
                 db     00
;volstr:          ds     " Vol:"
;                 db     00
fil0:			 ds     "1,1,1"
                 db     00
fil1:            ds		"0,1,1"
                 db     00
fil2:            ds     "1,0,1"
                 db     00
fil3:            ds     "1,1,0"
                 db     00
rssi:            ds     "RSSI?"
                 db     00
;_rufton:         ds     "AT+SETTAIL="
;                db     00
;------------------------------------------------------------------------------------------
;
;  sinus lookup Tabelle
;
;------------------------------------------------------------------------------------------

SINtable:    
;db 0,1,2,2,3,3,4,6,8,10,13,16,19,22,26,30,34,38,42,45,48,50,52,54,56,57,57,58,58,59,60,60
;db 60,59,58,58,57,57,56,54,52,50,48,45,42,38,34,30,26,22,19,16,13,10,8,6,4,3,3,2,2,1,0,0 
db  0,3,6,12,24,34,48,50,50,48,36,26,16,8,4,1,0				 
.ENDLITERAL 