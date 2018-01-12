#include <p18f8520.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <xlcd.h>
#include <string.h>
#include <spi.h>

#pragma config OSC=HS, WDT=OFF, LVP=OFF

#define SPI_CS LATCbits.LATC2

char var;

void set_wren(void);
void busy_polling(void);
void status_write(unsigned char data);
unsigned char status_read(void);

void DelayFor18TCY(void)
{
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

void DelayPORXLCD(void)
{
	Delay1KTCYx(75);
}

void DelayXLCD(void)
{
	Delay1KTCYx(25);
}

void comandXLCD(unsigned char a)
{
	while(BusyXLCD());
	WriteCmdXLCD(a);
}

void gotoxyXLCD(unsigned char x, unsigned char y)
{
	unsigned char direccion;

	if(y != 1)
	{
		direccion = 0x40;
	}
	else
	{
		direccion = 0;
	}

	direccion += x-1;
	comandXLCD(0x80 | direccion);
}

void main(void)
{

	unsigned int lectAnalog, x1, x2, digital1, digital2;
	//char lectEscribir[3], digitalEscribir;

	/*Inicializacion LCD*/
	//TRISB=0x0F; //el nible de menor peso se configura como entrada
	//LATB=0xF0; // se inicializan los LAT a 0
	//OpenXLCD(EIGHT_BIT & LINES_5X7);   //Iniciamos el LCD

	/*Inicializacion SPI*/
	OpenSPI(SPI_FOSC_64, MODE_11, SMPEND); //sync_mode, bus_mode, smp_phase
	LATEbits.LATE3 = 1; // chip select a 1 para que luego podamos detectar flanco de bajada en el a/d
	LATEbits.LATE4 = 1; //ponemos a 1 para que luego podamos detectar un flanco de bajada en el d/a
	LATEbits.LATE5 = 0;  //Poniendo el pin LDAC a 0, el contenido almacenado en el registro de entrada del DAC se transfiere al registro de salida del DAC(VOUT ), y este se actualiza
	TRISEbits.TRISE3 = 0;
	TRISEbits.TRISE4 = 0;
	TRISEbits.TRISE5 = 0;

	/*Inicio algoritmo*/
	while(1!=2){
		LATEbits.LATE3 = 0;
		WriteSPI(0x06);
		WriteSPI(0x00);
		//leemos un los primeros 4 bits
		x1 = SSPBUF & 0x0F; //cogemos los 4 �ltimos bits del buffer
		x2 = ReadSPI(); //leemos los 8 �ltimos bits
 		LATEbits.LATE3 = 1; //ponemos a 1 para que haya un flanco de bajada en el a/d

		lectAnalog = (x1 << 8) | x2; //transformamos la lectura a una �nica variable entera

		/*Escribimos en el LCD*/
		//itoa(lectAnalog, lectEscribir); //pasamos de entero a string
		//gotoxyXLCD(1,1);
		//putsXLCD(lectEscribir); //imprimimos la lectura

		digital1 = 0x30 | x1 ; //añadimos la palabra de configuración
		digital2 = x2;

		/*Enviamos al conversor D/A*/
		LATEbits.LATE4 = 0;  //provocamos flanco de bajada
		WriteSPI(digital1);  //escribimos en el DA
		WriteSPI(digital2);

		LATEbits.LATE4 = 1;

	}
}
