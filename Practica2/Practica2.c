/*Un cable modem nulo sirve para conectar terminales usando
un cable RS232. Para conectar terminales es necesario cruzar
las líneas de recepción y transmisión puesto que la línea de
recepción de uno será la de transmisión del otro*/

/*La velocidad máxima a la que se puede transmitir es 57600 baudios
puesto que a 115200 el error es demasiado grande y la comunicación
falla. Con 57600 el error es de 1.72 porciento, mientras que con
115200 este se dispara a un 8.5 porciento*/

#include <p18f8520.h>
#include <delays.h>
#include <xlcd.h>
#include <stdio.h>
#include <usart.h>

#pragma config OSC=HS, WDT=OFF, LVP=OFF

volatile char c;
volatile int num = 0;
volatile char scroll[16];
volatile int i = 0;

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

#pragma interrupt ISR
void ISR(void){
	LATB=0x00;
	/*Interrupcion USART*/
	if(PIR1bits.RC1IF)
	{			 	//Interrupcion en la USART
		if(!RCSTAbits.OERR)
		{	 	//Desbordamiento en la USART
			while(Busy1USART());
			c=Read1USART();		 	//Caracter leido
			putcXLCD(c);
			if(!RCSTAbits.FERR)
			{	 	//No hay errores en la trama
				PIR1bits.RCIF = 0;	//Reinicia el flag de la interrupción en la recepción
			}
			num++; //contamos cuantos caracteres mandamos
			if(num > 16 && num < 32){
				scroll[i] = c;
				i++;
			}
		}
		else
		{						//Error en la trama
			RCSTAbits.CREN = 0;		//Resetea la recepción continua
			RCSTAbits.CREN = 1;		//Habilita la recepcion continua
		}
	}
	
	/*Interrupcion tecla pulsada*/
	if(INTCONbits.INT0IF)
	{
		INTCONbits.INT0IF=0;
		LATB=0x10;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB4)
			{
				putc1USART('1');
				INTCONbits.INT0IF=0;
			}
		LATB=0X20;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB5)
			{
				putc1USART('4');
				INTCONbits.INT0IF=0;
			}
		LATB=0x40;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB6)
			{
				putc1USART('7');
				INTCONbits.INT0IF=0;
			}
		LATB=0x80;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB7)
			{
				putc1USART('*');
				INTCONbits.INT0IF=0;
			}
	}
	if(INTCON3bits.INT1IF)
	{
		INTCON3bits.INT1IF=0;
		LATB=0x10;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB4)
			{
				putc1USART('2');
				INTCON3bits.INT1IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB5)
			{
				putc1USART('5');
				INTCON3bits.INT1IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB6)
			{
				putc1USART('8');
				INTCON3bits.INT1IF=0;
			}
		LATB=0x80;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB7)
			{
				putc1USART('0');
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
				putc1USART('3');
				INTCON3bits.INT2IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB5)
			{
				putc1USART('6');
				INTCON3bits.INT2IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB6)
			{
				putc1USART('9');
				INTCON3bits.INT2IF=0;
			}
		LATB=0x80;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB7)
			{
				putc1USART('#');
				INTCON3bits.INT2IF=0;
			}
	}
	if(INTCON3bits.INT3IF)
	{
		INTCON3bits.INT3IF=0;
		LATB=0x10;
		if(INTCON3bits.INT3IF)
			if(PORTBbits.RB4)
			{
				putc1USART('A');
				INTCON3bits.INT3IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT3IF)
			if(PORTBbits.RB5)
			{
				putc1USART('B');
				INTCON3bits.INT3IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT3IF)
			if(PORTBbits.RB6)
			{
				putc1USART('C');
				INTCON3bits.INT3IF=0;
			}
		LATB=0x80;
		if(INTCON3bits.INT3IF)
			if(PORTBbits.RB7)
			{
				putc1USART('D');
				INTCON3bits.INT3IF=0;
			}
	}

	if(num == 16){
		gotoxyXLCD(1,2);
	}
	else if(num == 32)
	{
		comandXLCD(0x01); //borra lcd
		gotoxyXLCD(1,1);
		putsXLCD(scroll); //escribe la linea de abajo en la de arriba
		num=0;
		i=0;
		gotoxyXLCD(1,2);
	}
	LATB=0xF0;

	/*limpiamos los flag para evitar interrupciones recursivas*/
	INTCONbits.INT0IF=0;
	INTCON3bits.INT1IF=0;
	INTCON3bits.INT2IF=0;
	INTCON3bits.INT3IF=0;
	PIR1bits.RC1IF = 0;		//resetea el flag de la interrupción RX del USART	
	Delay10KTCYx(45); //evitamos rebotes
}
#pragma code interrupcion=0x08
void vector_interrupcion(void)
{
	_asm goto ISR _endasm
}
#pragma code

void main(void){

	RCONbits.IPEN=0; // desactivamos los niveles de prioridad
	INTCONbits.INT0IE=1; //activamos la interrupcion INT0
	INTCONbits.INT0IF=0;
	INTCON3bits.INT1IE=1; //activamos la interrupcion INT1
	INTCON3bits.INT1IF=0;
	INTCON3bits.INT2IE=1; //activamos la interrupcion INT2
	INTCON3bits.INT2IF=0;
	INTCON3bits.INT3IE=1; //activamos la interrupcion INT3
	INTCON3bits.INT3IF=0;
	INTCONbits.PEIE = 1; //habilita interrupcion de perifericos
	INTCONbits.GIE=1; //activamos las interrupciones generales

	TRISC = 0x80;		//TX1 a 1 porque es la salida y RX1 a 0 porque es la entrada
	
	//USART
	PIR1bits.RC1IF = 0;		//resetea el flag de la interrupción RX del USART
	IPR1bits.RC1IP = 1;		//Alta prioridad en la interrrupción RX del USART
	PIE1bits.RC1IE = 1;		//Habilita la interrupción de entrada en la USART
	
	TRISB=0x0F; 					   //el nible de menor peso se configura como entrada
	LATB=0xF0; 					   	   // se inicializan los LAT a 0
	OpenXLCD(EIGHT_BIT & LINES_5X7);   //Iniciamos el LCD

	//Inicializa el USART
	Open1USART(USART_TX_INT_ON &		//Interrupción de transmision ON
			   USART_RX_INT_ON &		//Interrupción de recepción ON
			   USART_ASYNCH_MODE &		//Modo asíncrono
			   USART_EIGHT_BIT &		//Transmisión/Recepción de 8bit
			   USART_CONT_RX &			//Recepción continua
			   USART_BRGH_HIGH,64);		//x = BRG(high) = [Fosc/(baund x 16)] -1
	gotoxyXLCD(1,1);
	
	while(1){};
}