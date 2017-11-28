#include <p18f8520.h>

#pragma config OSC=HS, LVP=OFF

void main(void){

	ADCON1=0x0F; //configuramos entradas y salidas
	TRISA=0x00; 
	TRISB=0xFF;

	LATA=0x00;
	while(1){
		LATA = PORTB; // copiamos la entrada de B a la salida de A
	}
}