#include <p18f8520.h>
#include <delays.h>
#include <xlcd.h>

#pragma config OSC=HS, WDT=OFF, LVP=OFF

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
  //Envia comando al LCD
void comandXLCD(unsigned char a){
	BusyXLCD();
	WriteCmdXLCD(a);
}
//Ubica cursor en: x = posición en linea, y = N de linea.
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

void main(){
	int a=7, i;
	char sis[8]="SISTEMAS";
	char emp[10]="EMPOTRADOS";

	OpenXLCD(EIGHT_BIT & LINES_5X7);   //Iniciamos el LCD			

	while(a>=0){
		gotoxyXLCD(a+5,1);  //Pasamos a la primera linea, posicion 12
		putcXLCD(sis[a]); //imprimimos 1 a 1 los caracteres
		Delay10KTCYx(200); //delay para poder apreciar la salida
		a--;
	}

	a=0;

	while(a<10){
		gotoxyXLCD(5+a,2); //hacemos lo mismo que antes con la línea e abajo
		putcXLCD(emp[a]); 
		Delay10KTCYx(200);
		a++;
	}

	for(i=0;i<=4;i++){
		comandXLCD(0x01);
		Delay10KTCYx(50);

		gotoxyXLCD(5,1); //imprimimos la primera linea
		putrsXLCD("SISTEMAS");
		gotoxyXLCD(5,2); //imprimimos la segunda linea
		putrsXLCD("EMPOTRADOS");

		Delay10KTCYx(50);
	}

	comandXLCD(0x01);  //Borrar pantalla y vuelve a origen	

	Delay10KTCYx(50);

	gotoxyXLCD(5,1);  
	putrsXLCD("EMPOTRADOS"); //imprimimos la primera linea

	gotoxyXLCD(5,2);  
	putrsXLCD("SISTEMAS"); //imprimimos la segunda linea

	while(1){}
}