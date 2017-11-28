#include <p18f8520.h>

#pragma config OSC=HS, LVP=OFF, WDT=OFF

void ISRPulsacion(void);

#pragma code inter_pulsacion=0x08
void vector_inter_pulsacion(void)
{
	_asm goto ISRPulsacion _endasm
}
#pragma code

#pragma interrupt ISRPulsacion
void ISRPulsacion(void){
	TRISB=0x00;
	LATB=0xFF;
	LATD=0xFF;
}

void main(void){
	int i, valor;

	LATB=0x00;
	LATD=0x00;

	RCONbits.IPEN=0; // desactivamos los niveles de prioridad
	INTCONbits.INT0IE=1; //activamos la interrupcion INT0
	INTCONbits.GIE=1; //activamos las interrupciones generales

	TRISB=0x00;
	TRISD=0x00;

	while(1){
			valor=1; //inicializamos el valor a mostrar
			INTCONbits.INT0IF=0; //activamos la interrupcion INT0
			TRISB=0x00; //ponemos el puerto B como salida

			for(i=0;i<=7; i++){
				LATB=valor; //mostramos el valor (inicialmente 1)
				INTCONbits.INT0IF=0;
				valor=valor*2; //multiplicamos por 2 para corre el bit
				Delay10KTCYx(50); //retardo para poder apreciar la salida
				TRISB=0x01; //ponemos el valor pin RB0 como entrada
			}
			
			LATB=0x00; // dejamos a 0 la salida del puerto B
			valor=128; //inicializamos a 128

			for(i=0; i<=7; i++){
				LATD=valor; //ponemos el valor en el puerto D (tras multiplicar vale 128)
				valor=valor/2; //dividimos entre 2 para correr el bit hacia la derecha
				Delay10KTCYx(50); //retardo para poder apreciar la salida
			}
			LATD=0x00; //dejamos a 0 la salida del puerto D
	}
}