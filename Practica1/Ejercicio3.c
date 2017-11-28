#include <p18f8520.h>
#pragma config OSC=HS, WDT=OFF, LVP=OFF

volatile int contador = 0;

#pragma interrupt ISRPulsacion
void ISRPulsacion(void){
	contador++;
	LATD=contador;
	INTCONbits.INT0IF=0;
}

#pragma code inter_pulsacion=0x08
void vector_inter_pulsacion(void)
{
	_asm goto ISRPulsacion _endasm
}
#pragma code

void main(void){

	RCONbits.IPEN=0; // desactivamos los niveles de prioridad
	INTCONbits.INT0IE=1; //activamos la interrupcion INT0
	INTCONbits.INT0IF=0;
	INTCONbits.GIE=1; //activamos las interrupciones generales

	TRISB=0x01; //Se configura RB0 como entrada
	TRISD=0x00; //TRISD como salida
	LATB=0x00; //cargamos 0 en los latch para asegurarnos de que empiezan a 0
	LATD=0x00;
	
	Delay10TCYx(40);

	while(1){}
}