#include <p18f8520.h>
#include <delays.h>
#include <xlcd.h>
#include <stdio.h>

#pragma config OSC=HS, WDT=OFF, LVP=OFF

volatile int teclaspulsadas=0;

void DelayFor18TCY(void){
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}
void DelayPORXLCD(void){
	Delay1KTCYx(75);
}
void DelayXLCD(void){
	Delay1KTCYx(25);
}

void comandXLCD(unsigned char a){
	while(BusyXLCD());
	WriteCmdXLCD(a);
}

void gotoxyXLCD(unsigned char x, unsigned char y){
	unsigned char direccion;

	if(y != 1){
		direccion = 0x40;
	}else{
		direccion = 0;
	}

	direccion += x-1;
	comandXLCD(0x80 | direccion);
}

#pragma interrupt ISRPulsacion
void ISRPulsacion(void){
	char c[3];
	LATB=0x00;
	gotoxyXLCD(1,1);
	if(INTCONbits.INT0IF)
	{
	INTCONbits.INT0IF=0; //si salta la interrupcion, quitamos el flag
		LATB=0x10; //ponemos tension en esta linea
		if(INTCONbits.INT0IF) //y comprobamos si la que esta pulsada es esta
			if(PORTBbits.RB4)
			{
				putcXLCD('1');
				teclaspulsadas++;
				INTCONbits.INT0IF=0;
			}
		LATB=0X20; //esta
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB5)
			{
				putcXLCD('4');
				teclaspulsadas++;
				INTCONbits.INT0IF=0;
			}
		LATB=0x40; //o esta
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB6)
			{
				putcXLCD('7');
				teclaspulsadas++;
				INTCONbits.INT0IF=0;
			}
	}
	if(INTCON3bits.INT1IF) //hacemos lo mismo con la otra interrupcion
	{
		INTCON3bits.INT1IF=0;
		LATB=0x10;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB4)
			{
				putcXLCD('2');
				teclaspulsadas++;
				INTCON3bits.INT1IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB5)
			{
				putcXLCD('5');
				teclaspulsadas++;
				INTCON3bits.INT1IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB6)
			{
				putcXLCD('8');
				teclaspulsadas++;
				INTCON3bits.INT1IF=0;
			}
		LATB=0x80;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB7)
			{
				putcXLCD('0');
				teclaspulsadas++;
				INTCON3bits.INT1IF=0;
			}
	}
	if(INTCON3bits.INT2IF)
	{
		INTCON3bits.INT2IF=0;
		LATB=0x10;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB4)
			{
				putcXLCD('3');
				teclaspulsadas++;
				INTCON3bits.INT2IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB5)
			{
				putcXLCD('6');
				teclaspulsadas++;
				INTCON3bits.INT2IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB6)
			{
				putcXLCD('9');
				teclaspulsadas++;
				INTCON3bits.INT2IF=0;
			}
	}

	sprintf(c,"%d",teclaspulsadas);
	gotoxyXLCD(4,2);
	putsXLCD(c);

  	LATB=0xF0;

	/*limpiamos los flag para evitar interrupciones recursivas*/
	INTCONbits.INT0IF=0;
	INTCON3bits.INT1IF=0;
	INTCON3bits.INT2IF=0;
	INTCON3bits.INT3IF=0;
 
	Delay10KTCYx(45); //evitamos rebotes
}

#pragma code inter_pulsacion=0x08
void vector_inter_pulsacion(void)
{
	_asm goto ISRPulsacion _endasm
}
#pragma code

void main(void)
{

	INTCONbits.INT0IF=0;
	INTCONbits.INT0IE=1; //activamos la interrupcion INT0
	INTCON3bits.INT1IF=0;
	INTCON3bits.INT1IE=1; //activamos la interrupcion INT1
	INTCON3bits.INT2IF=0;
	INTCON3bits.INT2IE=1; //activamos la interrupcion INT2
	INTCON3bits.INT3IF=0;
	INTCON3bits.INT3IE=1; //activamos la interrupcion INT3
	INTCONbits.GIE=1; //activamos las interrupciones generales

	TRISB=0x0F; //el nible de menor peso se configura como entrada
	LATB=0xF0; // se inicializan los LAT a 0
	OpenXLCD(EIGHT_BIT & LINES_5X7);   //Iniciamos el LCD

	gotoxyXLCD(1,2);
	putrsXLCD("T.P."); //teclas pulsadas
	while(1)
	{
		Sleep();
	}	
}