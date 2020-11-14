  //----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h" // PSoC API definitions for all User Module
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern void kommando_setvol(void);
extern void kommando_setfilter(void);
extern void kommando_heartbeat(void);
extern void lcdInit(void);
extern void clear_LCD(void);
extern void kommando_rssi(void);
extern void kommando_rufton(void);
extern void setgroup_response(void);
extern void wait_ptt(void );
extern void ton_on(void);
extern void print_vol(void);
extern char responsebuffer[];
extern unsigned char filter,counter;
//extern unsigned char TXstatus2;//,TXstatus2,Squelch;
//char lptext[]="LP ",hptext[]="HP ",pretext[]="PRE";	
//char offtext[]="off",sqtext[]="SQ:",modetext[]="Mode:";
unsigned char TXstatus;// Status Bit 0=channel space 25 khz/1=channel space 12,5 khz ;Bit 1=simplex/duplex; Bit 2= hfpower low/high;bit 3 =ablage plus/minus;                    
//Bit 4= PTT is off/on ; Bit 5 Rufton on/off;Bit 6 = 0 vox off/on; Bit 7 = 0 Menustatus off/on
unsigned char TXstatus2;  //Bit 0  0=Normal/1= Aprs mode Bit 1 = 0 APRS Daten valid/ 1 Aprs Daten ready 
unsigned char direction;
int shift;
long int frequenz;
unsigned char volume,Squelch;//squelch wert 0-8
unsigned char filter;//filter 1 =Pre/DE, 2=Hihghpass, 3=Lowpass  0---> all filter off
unsigned char micgain;


/* function delay for 1 millisecond
   Argument : none
   Return   : none
#########################################################################################################################*/
void warte1ms(void)
{
	 LCD_1_Delay50uTimes(20);
	
}
/* function delay 
   Argument : how many millisecond wait
   Return   : none
#########################################################################################################################*/
void warte(unsigned int e)
{
register unsigned int i;
 for (i=1;i<e;i++)
	                         {
							  warte1ms();	
	                          }
}
/* function wait for menu button down
   Argument : none
   Return   : none
#########################################################################################################################*/
void    menu_taste_down(void)
{
while 	((PRT1DR & 16)!=16){
							warte(100);
							M8C_ClearWDT;														
					      	} 
}
/* function Print the Frequency on display and print the Frequency to the CAT USB Port
   Argument : none
   Return   : none
#########################################################################################################################*/
void Print_Frequenz (void )
{
char e[11];
char d[4];
char c[5];	
	
	if ((TXstatus & 18)==18){ 
                           if ((TXstatus & 8)==0){
		                                           ltoa(e,frequenz-shift,10);
							                       }
										else
							                       {
											        ltoa(e,frequenz+shift,10);
													}
	                        }
	               else     
                       	{
							ltoa(e,frequenz,10);
				          
	                     }		
    strncpy(d,e+0,3);
	d[3]='\0';
	strncpy(c,e+3,4); 
	c[4]='\0';						
						LCD_1_Position(0,0);
						LCD_1_PrCString("QRG:");
    					LCD_1_PrString(d);				
						LCD_1_WriteData(0x2e);
    					LCD_1_PrString(c);

				        UART_2_CPutString("TRX Status:");	
				        UART_2_PutString(d);	
				        UART_2_PutChar(0x2e);					
				        UART_2_PutString(c);
				        UART_2_PutCRLF();
}
/* function read or Write changes from the HAM to EEprom
   Argument : char 1 for Read  or 0 for Write
   Return   : none
#########################################################################################################################*/
void eprom(unsigned char x)
{
BYTE f[64];
unsigned char status;
E2PROM_1_Start();	
if (x==1) {
	       
           E2PROM_1_E2Read( 0,f,64);
           if ((f[1]!=0x30)&&(f[2]!=0x30))
                                {
								  frequenz=atol(f);
								  shift=atoi(f+8);
	                              filter=f[12];
	                              Squelch=f[13];
								  volume=f[14];
								  TXstatus=f[15];
								  micgain=f[16];
								  TXstatus &=0x7f;
								}	
			else 
								{
								frequenz=1448000;
									shift=6000;
									filter=3;
									Squelch=2;
									volume=2;
									TXstatus=5;
									TXstatus2=0;
									micgain=5;
								}
         }						
   else 									  
		{	
			ltoa(f,frequenz,10);
			f[7]=32;
			itoa(f+8,shift,10);
			f[12]=filter;
			f[13]=Squelch;
			f[14]=volume;
			f[15]=TXstatus;
			f[16]=micgain;
		
		status= E2PROM_1_bE2Write(0,f,64,25);	
		}
	E2PROM_1_Stop();	
 }
/*void get_response(unsigned char anzahl)
                           {
						    register unsigned char i;
							responsebuffer[0]=UART_1_cGetChar();
							for (i=0;i<anzahl-1;i++){
								              responsebuffer[i]=UART_1_cGetChar();
                    	                      }			
							}
*/
/* function measure the volt from the Device
   Argument : none
   Return   : float measure Value in Volt
#########################################################################################################################*/
float get_voltage(void)
{		
int mess;
char mess1;
float sp;	
register unsigned char i;
    mess=0;
	PGA_1_Start(PGA_1_HIGHPOWER);
	ADCINC_1_Start(ADCINC_1_HIGHPOWER);
	ADCINC_1_GetSamples(0);
	warte(100);
	for (i=1;i<11;i++)
	                         {
							 while (ADCINC_1_fIsDataAvailable() == 0){
							                                 }
                            mess1=ADCINC_1_bClearFlagGetData();							
							mess=mess+mess1;
							mess1=ADCINC_1_fClearFlag();
	                          }						
	sp=(mess/10)*0.012890625;				
	sp=((26.8*sp)/6.8)+0.5;	
	PGA_1_Stop();
	ADCINC_1_Stop();
return sp;							
}
/* function Print the Voltage on the Display or CAT USB Port
   Argument : device=1 Print to CAT Port/ device= 0 Print to Display
   Return   : none
#########################################################################################################################*/
void  print_voltage(char device)
{
char help[4];
int t;
float Spannung;
Spannung=get_voltage();	
strncpy(help,ftoa(Spannung,&t),4);
help[4]='\0';
if (device==1){
               UART_2_PutString(help);
               UART_2_CPutString(" Volt");
               UART_2_PutCRLF();
}
else 
   {
	LCD_1_Position(0,14);
	LCD_1_PrString(help);
	LCD_1_PrCString(" V");
}
}
/* function  Print Status 2 on the Display and Cat USB Port
   Argument : none
   Return   : none
#########################################################################################################################*/
void Print_Status2(void)
{
	LCD_1_Position(2,0);
	LCD_1_PrCString("Mic:");
    LCD_1_Write_Data(micgain+48);
	UART_2_CPutString("Mic:");
	UART_2_PutChar(micgain+48);
	UART_2_PutChar(0x20);
	LCD_1_WriteData(0x20);
	/*LCD_1_PrCString("Func:");
	UART_2_CPutString("Func:");
	if ((TXstatus2 & 1)==0){
		                   LCD_1_PrCString("Norm");
		                   UART_2_CPutString("Norm");
	                     }
	      else     
                       	{
				           LCD_1_PrCString("APRS");
     						UART_2_CPutString("APRS");
	                     }	*/
	UART_2_PutChar(0x20);
	LCD_1_PrCString("Vox:");
	UART_2_CPutString("Vox:");	
	if ((TXstatus & 64)==0){
		                   LCD_1_PrCString("off");
		                   UART_2_CPutString("off");
	                     }
	      else     
                       	{
				           LCD_1_PrCString("on ");
     						UART_2_CPutString("on ");
	                     }						
	UART_2_PutCRLF();					
}
/* function  Print Status 1 on the Display and Cat USB Port
   Argument : none
   Return   : none
#########################################################################################################################*/
void Print_status(void)
{	
	LCD_1_Position(3,0);
	LCD_1_PrCString("SQ:");
	LCD_1_WriteData(Squelch+48);
	UART_2_CPutString("SQ:");
	UART_2_PutChar(Squelch+48);
	UART_2_PutChar(0x20);
	LCD_1_WriteData(0x20);
	UART_2_CPutString("Mode:");
	if ((TXstatus & 2)==0){
		                   LCD_1_PrCString("Sim");
		                   UART_2_CPutString("Simplex");
	                     }
	      else     
                       	{
				           LCD_1_PrCString("Dup");
     						UART_2_CPutString("Duplex");
	                     }		
	UART_2_PutChar(0x20);					
	LCD_1_WriteData(0x20);
	
	LCD_1_PrCString("Fil:");
	UART_2_CPutString("Filter:");			
	if (filter==0){
		           LCD_1_PrCString("off");
			       UART_2_CPutString("off");
                 	}
	if (filter==1){
		           LCD_1_PrCString("PRE");
			       UART_2_CPutString("PRE");
                 	}
    if (filter==2){
		            LCD_1_PrCString("HP ");
			              UART_2_CPutString("HP");
                	}
    if (filter==3){
		            LCD_1_PrCString("LP ");
			        UART_2_CPutString("LP"); 
              	}
	UART_2_PutChar(0x20);
	LCD_1_Position(2,0);
	LCD_1_PrCString("Bw:");
	UART_2_CPutString("Bw:");
	if ((TXstatus & 1)==0){
		                   LCD_1_PrCString("12,5 Khz");
		                   UART_2_CPutString("12,5 Khz");
	                     }
	      else     
                       	{
				           LCD_1_PrCString("25,0 Khz");
						   UART_2_CPutString("25,0 Khz");
	                     }	
	UART_2_PutChar(0x20);					
	LCD_1_WriteData(0x20);
	LCD_1_PrCString("Pwr:");
	UART_2_CPutString("Pwr:");					
	if ((TXstatus & 4)==0){
		                   LCD_1_PrCString(" low");
		                   UART_2_CPutString("low");
	                     }
	      else     
                       	{
				           LCD_1_PrCString("high");
	    					UART_2_CPutString("high");
	                     }
	UART_2_PutChar(0x20);
	print_voltage(1);									
//					
}	
/* function for setGroup from DRA818
   Argument : none
   Return   : response from DRA 818 Chip in the responsebuffer 1 -> 14
#########################################################################################################################*/
void komando_setgroup(void)
{ 	
char setgroup[31],komma[]=",";
char ctss[]="0000";	
char f[11],s[1];
char x[]="0";
char z[]=".";
char y[]="1";	
               PRT1IE &=0x7f; //drehgeber Interupt sperren
	           if ((TXstatus & 1) ==0){
				                   strcpy(setgroup,x);
		                        	}
			    else
			                       {
									strcpy(setgroup,y);
								}	
							
	            strcat(setgroup,komma); 
						
				if ((TXstatus & 2)==0){
					                 ltoa(f,frequenz,10);
					                 strncat(setgroup,f,3);
					                 strcat(setgroup,z);
					                
					                 strncat(setgroup,f+3,4); 
	                                 strcat(setgroup,komma); 
					                 strncat(setgroup,f,3);
					                 strcat(setgroup,z);
					                 strncat(setgroup,f+3,4); 
				                  
					               
			                       	}
				else 
			                     	{
								    
									ltoa(f,frequenz-shift,10);
								     strncat(setgroup,f,3);
					                 strcat(setgroup,z);
					                 strncat(setgroup,f+3,4); 
	                                strcat(setgroup,komma);    
									ltoa(f,frequenz,10);
					                  strncat(setgroup,f,3);
					                 strcat(setgroup,z);
					                 strncat(setgroup,f+3,4); 
									}
				strcat(setgroup,komma);
			    strcat(setgroup,ctss);
		        strcat(setgroup,komma);
				itoa(s,Squelch,10);					
			    strcat(setgroup,s);
				strcat(setgroup,komma);
			    strcat(setgroup,ctss);						
    UART_1_CPutString("AT+DMOSETGROUP=");
	UART_1_PutString(setgroup);				
	UART_1_PutCRLF();								
	setgroup_response();
	//get_response(15);							
	if (responsebuffer[12]!=0x3a || responsebuffer[13]!=0x30){
		                        LCD_1_Position(1,0);
								LCD_1_PrCString("GR");
		                      
	                           }
	                else  
							{
		                      Print_Frequenz();
							}		
	warte(10);
	PRT1IE |= 0x80; //drehgeber Interupt erlauben
							
}
/*void kommando_setfilter(unsigned char z)
{
char str[]="+DMOSETFILTER:0";
    UART_1_CPutString("AT+SETFILTER=");
	 switch (z){
		         case 0: UART_1_CPutString("0,0,0");
			             break;
		         case 1: UART_1_CPutString("1,0,0");
			             break;
		         case 2: UART_1_CPutString("0,1,0");
						break;
		         case 3: UART_1_CPutString("0,0,1");
		                break;
	}
		          
    UART_1_PutCRLF();
	get_response(16);
	if (strncmp(responsebuffer,str,15)){
		                        LCD_1_Position(0,17);
								LCD_1_PrCString("FI");	                       
						}	
	warte(100);
	}*/

void main(void)
{

char *strPtr;
int  count;		
    
if(CPU_SCR0 & CPU_SCR0_WDRS_MASK)
                                    {
                                     // Reset kommt von der Watchdog der SRam wird gesichert
									LCD_1_Start();
								    LCD_1_Control(1);
									count=15;	
                                    }
             else 
									{	
									lcdInit();	
									warte(1000);
	                                clear_LCD()	;
									// reset ist por ,xres 	
									
									}
									
    eprom(1);
									
	M8C_EnableGInt;
	PTT_Start();
	HF_power_Start();								
	PTT_On();
	HF_power_On();
	Timer8_1_EnableInt();
    Timer8_1_Start();	
	UART_1_Start(UART_1_PARITY_NONE);
	UART_1_EnableInt();
	UART_1_IntCntl(UART_1_ENABLE_RX_INT);
    counter=1;
    UART_2_Start(UART_2_PARITY_NONE);
    UART_2_EnableInt();
 	//UART_2_IntCntl(UART_2_ENABLE_RX_INT);
	UART_2_CmdReset();									
	kommando_heartbeat();	
	warte(200);
	komando_setgroup();
	warte(10);
    kommando_setvol();
	warte(200);
	kommando_setfilter();
	warte(100);
	Print_status();
	direction=0;
	Eingangsamp_Start(Eingangsamp_HIGHPOWER);
	Eingangsamp_SetGain(0x98-(micgain*16));
	RefMux_1_Start(RefMux_1_LOWPOWER);							
    M8C_EnableWatchDog;
	M8C_EnableIntMask(INT_MSK0, INT_MSK0_GPIO);	
	while (1)
		{
		M8C_ClearWDT;	
	    print_voltage(0);
	    kommando_rssi();
		 if (UART_2_bCmdCheck()>0) { 
			                       M8C_DisableIntMask(INT_MSK0, INT_MSK0_GPIO);
			                       // command list for the CAT Port USB
			                     // > 10 liefert den Herzschlag des Moduls
		                         // > 11 liefert die Aktuelle Frequenz aus
		                         // > 12 get Status 2 
		                         // > 13 get Status 
			                     // > 14 increment Frequency for 1 channel independend from channel Value (12,5 khz/25 Khz)  
			                      //> 15 decrement Frequency for 1 Channel
			                      //> 16 Display off
			                     // > 17 Display on
			                      // > 18 get voltage from the device
			                      // > 19 increment squelch value for 1
			                      // > 20 decrement squelch value for 1
			                      // > 21 change Simplex/Duplex mode
			                       //> 22 change Power high/low
			                       //> 23 trx 1750hz tone for opening the FM Relais
			                       //> 24 change next Filter  
			                       //> 25 change channel value (12,5 khz/25 khz)
		                           strPtr=UART_2_szGetParam();
                                   if(strchr(strPtr,'>')!=0) 
						                              {      
														   
                                                       while(strPtr = UART_2_szGetParam()) 
													                                    {    
																		                count=atoi(strPtr);
																						switch (count){
																							           case 10:
                                                                                                               UART_2_CPutString("Heartbeat");
																								                break;
																										case 11:
																							           			Print_Frequenz();
																												break;
																							            case 12:
																								                Print_Status2();
																							           // case 13:
																								         //       Print_status();
																								           //     break;
																							            case 14:
																								                if (((TXstatus & 1)==0)&&(frequenz<1459999)){
																													                                         frequenz=frequenz+125;
																													                                      
																																							}
																												else 
																																							{
																																							 frequenz=frequenz+250;							
																																							}
																												//komando_setgroup();  											
																												break;
																										case 15:	
																												if (((TXstatus & 1)==0)&&(frequenz>1440001)){
																													                                         frequenz=frequenz-125;  
																																							}
																												else 
																																							{
																																							 frequenz=frequenz-250;							
																													                                         }
																												//komando_setgroup();  											
																												break;
																										case 16:	
																												LCD_1_Control(8);
																								                  break;
																							            case 17:
																												LCD_1_Control(12);
																								                break;
																										case 18:
																								                //get_voltage();
																								                print_voltage(1);
																								                break;
																							            case 19:
																								                if (Squelch<8){
																													           Squelch++;
																												
																											                	}
																												break;
																							            case 20:
																								                if (Squelch>0){
																													           Squelch--;
																												
																											                	}
																												break;
																							             case 21:
																								                TXstatus^= 0x02;
																								                break;
																							             case 22:
																								                TXstatus^= 0x04;
															                									HF_power_Invert(); 
																								                break;
																							             case 24:
																								                filter++;
															                               						if (filter>3){
																           													filter=0;
							       						              														}
																															kommando_setfilter();
																															//Print_status();
																								                break;
																							             case 25:
																												TXstatus^= 0x01;
																								                if (((frequenz & 1)==1)&&((TXstatus & 1)==1)){
																							    					                                          frequenz=(float)frequenz+125;
																													                                      
																													                                         }
																												break;					
																					                           		
																						              } 
																						               komando_setgroup();
																						               eprom(0);
																						               Print_status();
																						}
																						
													}
								                    UART_2_CmdReset();
													M8C_EnableIntMask(INT_MSK0, INT_MSK0_GPIO);
	                              	}
		if ((TXstatus & 16)==16){
			      M8C_DisableIntMask(INT_MSK0, INT_MSK0_GPIO);
	              if ((TXstatus & 32)==32){
			                              kommando_rufton();
	                          	          }
				  else
				                          {
			                                wait_ptt();
					                         Eingangsamp_Stop();
		                                    }
		          PTT_Invert();
	              TXstatus &=207;
	              Print_Frequenz();
			      M8C_EnableIntMask(INT_MSK0, INT_MSK0_GPIO);
	      }
		/*if (((TXstatus2 & 3)==3)&&((TXstatus & 128)==0)){
			                                             //clear_LCD();
			                                             Print_Frequenz();
			                                             line();
			                                            
	                          	}*/
	    if (direction >=128){
			                    
			                 if ((direction==129)&& ((TXstatus & 1)==0)){
								                                        if (frequenz<1459999){
								                                                           frequenz=frequenz+125;
																		                  }
																		}
							 if ((direction==128)&& ((TXstatus & 1)==0)){
								                    				    if (frequenz>1440001){
								                                                            frequenz=frequenz-125;
						                                                           			}
																		}
							if ((direction==129)&& ((TXstatus & 1)==1)){
								 										if (frequenz<1459999){
																							frequenz=frequenz+250;
																							}
																			}
							 if ((direction==128)&& ((TXstatus & 1)==1)){
								                    					if (frequenz>1440001){
																	                        frequenz=frequenz-250;
						                                         							}
							                    						}	
							
							    
								direction=0;						
								komando_setgroup();
							    eprom(0);
                         		}
		//Menue abfrage
	    if (((PRT1DR & 16)!=16) &&((TXstatus & 32)==0)){ //Menutaste gedrückt und Sender aus 
			                 PRT1IE &=0xdf; //Tx on sperren solange Menu aktiv
			                 PRT0IE &=0xF7; // drehgeber 2 Interupt sperren solange Menu aktiv
			                  TXstatus |=0x80;
			                 Timer8_2_DisableInt(); 
			              
			                while (((TXstatus & 128)==128)&&((PRT1DR & 16)!=16)){
								                                                M8C_ClearWDT;	
							                                    				LCD_1_Position(0,0);
							 													LCD_1_PrCString("     Menue          ");
							 													LCD_1_Control(15);														
								                                                menu_taste_down();
																				LCD_1_Position(2,2);
						      													while ((PRT1DR & 16)==16){
								           																	if (direction>=128){
																            													direction=0;
																												                TXstatus^= 0x01;
																								                                if (((frequenz & 1)==1)&&((TXstatus & 1)==1)){
																							    					                                          frequenz=(float)frequenz+125;
																															                                 	}
																            													Print_status();
																																
																																LCD_1_Position(2,2);
																																}
																											M8C_ClearWDT;	
							                         													 }
																			
																				menu_taste_down();
																				LCD_1_Position(2,15);
																				//Auswahl HF Power
                            													while 	((PRT1DR & 16)==16){
								                         													if (direction>=128){
																            													direction=0;
															                													TXstatus^= 0x04;
															                													Print_status();
															                													HF_power_Invert();
																																LCD_1_Position(2,15);					
												                      															}
																											M8C_ClearWDT;	
					                          																} 
																				menu_taste_down();
																				LCD_1_Position(3,2);
																				//Auswahl Squelch level
																				while 	((PRT1DR & 16)==16){
								                         								   					if((direction==128)&&(Squelch<=8)){
															               
																                            													direction=0;
															                                													Squelch++;
															                               													    if (Squelch>8){
																								            												Squelch=8;
																						                 													}
															                                													Print_status();
															                                													//LCD_1_Position(1,22);
																		                														}
														  													if ((direction==129)&&(Squelch>=0)){
															                                													direction=0;
															                               														 Squelch--; 
															                                													if (Squelch>20){
																								           														Squelch=0;
																						                  														}
															                                													Print_status();
															                                													//LCD_1_Position(1,22);
														                                     													}
												                                                			//	warte(100);
																											LCD_1_Position(3,2);
																											M8C_ClearWDT;	
																											}	                
																			
																				menu_taste_down();
																				LCD_1_Position(3,6);
																				// Auswahl    simplex /Duplex
																				while 	((PRT1DR & 16)==16){
								                       														 if (direction>=128){
															                													direction=0;
															                													TXstatus^= 0x02;
															                													Print_status();
															                 													LCD_1_Position(3,6);
														                   														}
																											M8C_ClearWDT;	
																											
							                           														}  
																				
																				menu_taste_down();
																				LCD_1_Position(3,12);
																				while 	((PRT1DR & 16)==16){
								                         													if ((direction==128)&&(filter<=3)){
															                                													filter++;
																																				direction=0;
															                                													if (filter>3){
																								           													filter=3;
																						              														}
																																				kommando_setfilter();
																																				Print_status();
													    																						//LCD_1_Position(1,32);
																							                                               
													                                													     	}
														  													if ((direction==129)&&(filter>=0)){
															                                													filter--; 
																																				direction=0;
															                                													if (filter>20){
																								            												filter=0;
																						             														 	}
																																				kommando_setfilter();
																																				Print_status();
													    																						//LCD_1_Position(1,32);
														                                     													}	
														
																											//warte(100);
																											LCD_1_Position(3,12);
																											M8C_ClearWDT;	
																											}	 
																				
																				menu_taste_down();
																				clear_LCD();
																				Print_Status2();
																				LCD_1_Position(0,5);
																				LCD_1_PrCString("Menue");
																				while 	((PRT1DR & 16)==16){
																											
																											
																					
																											LCD_1_Position(2,3);
																					                        if ((direction==128)&&(micgain<9)){
																												                                micgain++;
																												                                direction=0;
																																				Print_Status2();
																																				//LCD_1_Position(0,23);
																																			    Eingangsamp_SetGain(0x98-(micgain*16));
																																				
																										  										}
																											
																											if ((direction==129)&&(micgain>1)){
																																				 micgain--;
																												                                direction=0;
																																				Print_Status2();
																																				//LCD_1_Position(0,23);
																																				Eingangsamp_SetGain(0x98-(micgain*16));
										
																																				}
																											M8C_ClearWDT;							
																											}
																			
																				/*menu_taste_down();
																				while 	((PRT1DR & 16)==16){
																					                         LCD_1_Position(0,30);
																					                          if (direction>=128){
															                													direction=0;
															                													TXstatus2 ^=1;
															                													Print_Status2();
																												                if ((TXstatus2 & 1)==1){
										
																																	                     frequenz=1448000;		
																																	                     Timer8_2_Start();
																																	                     Timer8_2_EnableInt();
																																	                     bit_counter =0;
																																	                     bit_buffer=0;
																																                          
																																                        }
																																else 
																															                          	{
																																						 Timer8_2_DisableInt();
																																						 Timer8_2_Stop();
																																						}
															                 													//LCD_1_Position(0,26);
														                   														}
																											M8C_ClearWDT;	
																				                            }
	          																
																				menu_taste_down();
																				
																				// vox on/off
																				/*while ((PRT1DR & 16)==16){
																					      				  LCD_1_Position(0,29);
																					                      if (direction>=128){
															                													direction=0;
															                													TXstatus ^=64;
															                													Print_Status2();
																											                    if ((TXstatus & 64)==0){
																																	                    // COMP_1_Stop();
																																                       }
																											                    else 
																															                     	{
																																						//COMP_1_EnableInt();
																																						//COMP_1_Start(COMP_1_LOWPOWER);
																																					}
																										                       }
																										 M8C_ClearWDT;
																			                           	}*/
																				menu_taste_down();
																				Print_status();
																				LCD_1_Position(0,5);
							 													while ((PRT1DR & 16)==16){ //exit Menu
								           																if (direction>=128){
																            												direction=0;
																            												TXstatus ^=128;
																             												if ((TXstatus & 128)==128){
																                                      												LCD_1_PrCString("Menue");
																																                    
																			    																	}
																									                        else 
																			          									                   			{
																																					LCD_1_PrCString("Exit ");
																																					}
																             												LCD_1_Position(0,5);
																            											   }
																										warte(100);	
																										M8C_ClearWDT;	
							                         					 								}
                               													}
							//übertragen der Daten zum RDA chip
								M8C_DisableIntMask(INT_MSK0, INT_MSK0_GPIO);
								komando_setgroup();
							    eprom(0);
								warte (100);
		                        LCD_1_Control(12);
								PRT1IE |=0x20;  //TX interupt wieder zulassen
							    PRT0IE |=0x08; //drehgeber2 Interupt wieder zulassen
								TXstatus &=0x7f;
								warte(200);
								M8C_EnableIntMask(INT_MSK0, INT_MSK0_GPIO);
							    Timer8_2_EnableInt();
	              				}
		}

}