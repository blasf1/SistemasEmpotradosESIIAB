#include <p18f8520.h>
#include <delays.h>
#include <xlcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <i2c.h>

#pragma config OSC=HS, WDT=OFF, LVP=OFF

unsigned char codigoAnio(int a);
int Diasemana (int anyo, int mes, int dia);
void semana(int sem);
int cambioHora();
int bisiestoCercano(int a);
void LeeI2C();
int EscribirI2C(char sec, char min, char hor, char di, char me);

int bool=0, i, j = 0;
int anyo, mes, dia, hora, minuto, segundo;
char total[14];
int leerSegundos, leerMinutos, leerHoras, leerDias, leerMeses, leerAnios;

int anioActual = 2017;

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

#pragma interrupt ISRPulsacion
void ISRPulsacion(void)
{
	comandXLCD(0x0d);
	LATB=0x00;

	if(INTCONbits.INT0IF)
	{
		INTCONbits.INT0IF=0;
		LATB=0x10;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB4)
			{
				while(BusyXLCD());
				putcXLCD('1');
				total[j] = '1'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCONbits.INT0IF=0;
			}
		LATB=0X20;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB5)
			{
				while(BusyXLCD());
				putcXLCD('4');
				total[j] = '4'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCONbits.INT0IF=0;
			}
		LATB=0x40;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB6)
			{
				while(BusyXLCD());
				putcXLCD('7');
				total[j] = '7'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCONbits.INT0IF=0;
			}
		LATB=0x80;
		if(INTCONbits.INT0IF)
			if(PORTBbits.RB7)
			{
				bool = 1; //rompemos el bucle para recoger la nueva hora y fecha
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
				while(BusyXLCD());
				putcXLCD('2');
				total[j] = '2'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCON3bits.INT1IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB5)
			{
				while(BusyXLCD());
				putcXLCD('5');
				total[j] = '5'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCON3bits.INT1IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB6)
			{
				while(BusyXLCD());
				putcXLCD('8');
				total[j] = '8'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCON3bits.INT1IF=0;
			}
		LATB=0x80;
		if(INTCON3bits.INT1IF)
			if(PORTBbits.RB7)
			{
				while(BusyXLCD());
				putcXLCD('0');
				total[j] = '0'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
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
				while(BusyXLCD());
				putcXLCD('3');
				total[j] = '3'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCON3bits.INT2IF=0;
			}
		LATB=0X20;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB5)
			{
				while(BusyXLCD());
				putcXLCD('6');
				total[j] = '6'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCON3bits.INT2IF=0;
			}
		LATB=0x40;
		if(INTCON3bits.INT2IF)
			if(PORTBbits.RB6)
			{
				while(BusyXLCD());
				putcXLCD('9');
				total[j] = '9'; //guardamos tecla pulsada para luego enviarla al RTC
				j++;
				INTCON3bits.INT2IF=0;
			}
		}

		if(INTCON3bits.INT2IF)
		{
			INTCON3bits.INT2IF=0;
			LATB=0x10;
			if(INTCON3bits.INT2IF)
				if(PORTBbits.RB4)
				{
					while(BusyXLCD());
					putcXLCD('3');
					total[j] = '3'; //guardamos tecla pulsada para luego enviarla al RTC
					j++;
					INTCON3bits.INT2IF=0;
				}
		}


		if(j == 10 || j == 12)
		{
			while(BusyXLCD());
			putcXLCD(':');
		}
		else if(j == 2 || j == 4)
		{
			while(BusyXLCD());
			putcXLCD('/');
		}
		else if(j == 8)
			gotoxyXLCD(1,2);

		LATB=0xF0;

		/*limpiamos los flag para evitar interrupciones recursivas*/
		INTCONbits.INT0IF=0;
		INTCON3bits.INT1IF=0;
		INTCON3bits.INT2IF=0;

		PIR1bits.RC1IF = 0;		//resetea el flag de la interrupci�n RX del USART
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
	char d[2], m[2], a[2], h[2], min[2], sec[2];

	RCONbits.IPEN=0; // desactivamos los niveles de prioridad
	INTCONbits.INT0IF=0;
	INTCONbits.INT0IE=1; //activamos la interrupcion INT0
	INTCON3bits.INT1IF=0;
	INTCON3bits.INT1IE=1; //activamos la interrupcion INT1
	INTCON3bits.INT2IF=0;
	INTCON3bits.INT2IE=1; //activamos la interrupcion INT2
	INTCON3bits.INT3IF=0;
	INTCON3bits.INT3IE=1; //activamos la interrupcion INT3
	INTCONbits.PEIE = 1; //habilita interrupcion de perifericos
	INTCONbits.GIE=1; //activamos las interrupciones generales

	TRISB=0x0F; //el nible de menor peso se configura como entrada
	LATB=0xF0; // se inicializan los LAT a 0
	OpenXLCD(EIGHT_BIT & LINES_5X7);   //Iniciamos el LCD
	OpenI2C( MASTER, SLEW_OFF);
	SSPADD = 0x19; 				// 100 KHz

	while(1)
	{
		bool=0; // se pone a 0 para entrar al bucle. Se romperá el bucle cuando se pulse asterisco
		while(!bool)
		{
			comandXLCD(0x0c); //desactivamos el cursor
			gotoxyXLCD(1,1); // posicion de escritura de la fecha
			LeeI2C();

			leerAnios=(leerDias & 0xC0) >> 6; // cogemos el codigo del año
	 	  leerDias=(((leerDias & 0x30) >> 4)*10 + (leerDias & 0x0F)) & 0x3F; // cogemos el día
			sprintf(d,"%02d",leerDias); //pasamos el entero a string
			putsXLCD(d);
			while(BusyXLCD());
			putcXLCD('/');

			leerMeses=(((leerMeses & 0x10)  >> 4)*10 + (leerMeses & 0x0F)) & 0x3F;
			sprintf(m,"%02d",leerMeses);
			putsXLCD(m);
			while(BusyXLCD());
			putcXLCD('/');

			sprintf(a,"%04d",anioActual);
			putsXLCD(a);

			gotoxyXLCD(12,1); // vamos al lugar en el que se imprimirá el día de la semana
			semana(Diasemana(anioActual, (int)leerMeses, (int)leerDias)); // calcula e imprime el día de la semana

			gotoxyXLCD(1,2);

			leerHoras=(((leerHoras & 0xF0)  >> 4)*10  + (leerHoras & 0x0F)) & 0x3F;
			sprintf(h,"%02d",leerHoras);
			putsXLCD(h);
			while(BusyXLCD());
			putcXLCD(':');

			leerMinutos  =  ((leerMinutos & 0xF0) >> 4)*10 + (leerMinutos & 0x0F);
			sprintf(min,"%02d",leerMinutos);
			putsXLCD(min);
			while(BusyXLCD());
			putcXLCD(':');

			leerSegundos  =  ((leerSegundos & 0xF0) >> 4)*10 + (leerSegundos & 0x0F);
			sprintf(sec,"%02d",leerSegundos);
			putsXLCD(sec);
		}

		cambioHora();
	}
}

unsigned char codigoAnio(int a)
{
	if(a - bisiestoCercano(a) == 0)
		return 0x00;
	else if(a - bisiestoCercano(a) == 1)
		return 0x01;
	else if(a - bisiestoCercano(a) == 2)
		return 0x02;
	else if(a - bisiestoCercano(a) == 3)
		return 0x03;
	else return 0x00;
}

int Diasemana (int anyo, int mes, int dia)
{
	int j, k, h;
	//tenemos en cuenta el caso de enero y febrero

	if(mes == 00 || anyo == 00 || dia == 00)
	{
		return 9;
	}

	if(mes<=2)
	{
		mes = mes + 12;
		anyo = anyo - 1;
	}
	else
	{
		mes = mes - 2;
	}

	//Calculamos k y h
	k = anyo % 100;
	h = anyo / 100;

	//aplicamos la formula
	j = ((700 + ((26 * mes - 2) / 10) + dia + k + (k/4) + ((h/4) + 5*h)) % 7);

  return j;
}

void semana(int sem)
{
	//imprime dia de la semana en funcion del resultado de la funcion Diasemana
	switch(sem)
	{
		case 1:
			putrsXLCD("LUN");
			break;
		case 2:
			putrsXLCD("MAR");
			break;
		case 3:
			putrsXLCD("MIE");
			break;
		case 4:
			putrsXLCD("JUE");
			break;
		case 5:
			putrsXLCD("VIE");
			break;
		case 6:
			putrsXLCD("SAB");
			break;
		case 0:
			putrsXLCD("DOM");
			break;
		default:
			putrsXLCD("FALLO");
			break;
	}
}

int cambioHora()
{
	int mes, dia;
	unsigned char segundoEscribir, minutoEscribir, horaEscribir, diaEscribir, mesEscribir, anioEscribir;
	char d1, d2, m1, m2, a1, a2, a3, a4, h1, h2, min1, min2, sec1, sec2;
	gotoxyXLCD(1,1);

	while(j != 14); //bloqueamos mientras se introducen los datos
	j = 0;
	comandXLCD(0x01); //desactivamos el cursor

	/*Guardamos lo introducido en variables individuales*/
	d1 = total[0];
	d2 = total[1];
	m1 = total[2];
	m2 = total[3];
	a1 = total[4];
	a2 = total[5];
	a3 = total[6];
	a4 = total[7];
	h1 = total[8];
	h2 = total[9];
	min1 = total[10];
	min2 = total[11];
	sec1 = total[12];
	sec2 = total[13];

	segundoEscribir = ((sec1 - 48 << 4) + sec2 - 48); //pasamos a bcd desde ascii
	minutoEscribir = ((min1 - 48 << 4) + min2 - 48);
	horaEscribir = ((h1 - 48 << 4) + h2 - 48);
	diaEscribir = ((d1 - 48 << 4) + d2 - 48);
	mesEscribir = ((m1 - 48 << 4) + m2 - 48);
	anioEscribir = ((codigoAnio((a1 - 48) * 1000 + (a2 - 48) * 100 + (a3 - 48) * 10 + (a4 - 48))) << 6) & 0xC0;
	diaEscribir = diaEscribir | anioEscribir; //combinamos el dia y el año en el mismo byte
	anioActual = bisiestoCercano((a1 - 48) * 1000 + (a2 - 48) * 100 + (a3 - 48) * 10 + (a4 - 48)) + (int)codigoAnio((a1 - 48) * 1000 + (a2 - 48) * 100 + (a3 - 48) * 10 + (a4 - 48));
	EscribirI2C(segundoEscribir, minutoEscribir, horaEscribir, diaEscribir, mesEscribir);

}

int bisiestoCercano(int a)
{
	int bisiesto;
	if((a % 4 == 0 && a % 100 != 0) || a % 400 == 0) // compruebo si es bisiesto
		return a;
	else // si no es bisiesto, consigo el bisiesto más próximo
	{
		bisiesto = a;
		while(!((bisiesto % 4 == 0 && bisiesto % 100 != 0) || bisiesto % 400 == 0)) //consigo el bisiesto inferior que cumpla la condición
		{
			bisiesto--;
		}
		return bisiesto;

	}
}

void LeeI2C()
{
	IdleI2C();
 	StartI2C();
 	while(SSPCON2bits.SEN);
 	WriteI2C(0xA0); //A0 es la direccion del esclavo para escritura
 	IdleI2C();
	WriteI2C(0x02); //02 es la direccion del registro de los segundos, desde donde voy a empezar
 	RestartI2C();
 	while(SSPCON2bits.RSEN);
 	WriteI2C(0xA1);  //Direccion del esclavo para lectura

	IdleI2C();
 	leerSegundos=ReadI2C();   //lee la direccion en la que empezamos (0x02)
	AckI2C();
 	IdleI2C();

	leerMinutos=ReadI2C();   // lee el siguiente registro
 	AckI2C();
	IdleI2C();

 	leerHoras=ReadI2C();   // lee el siguiente registro
 	AckI2C();
 	IdleI2C();

 	leerDias=ReadI2C();    //lee siguiente registro (anio/dia)
 	AckI2C();
 	IdleI2C();

 	leerMeses=ReadI2C();   // lee meses
 	NotAckI2C();
	IdleI2C();
 	StopI2C();
}

int EscribirI2C(char sec, char min, char hor, char di, char me)
{
 	IdleI2C();
	StartI2C();
	WriteI2C(0xA0);// direccion de escritura
	WriteI2C(0x02); //direccion segundos
	WriteI2C(sec); // escribir segundos
	WriteI2C(min); // escribir minutos
	WriteI2C(hor); // escribir horas
	WriteI2C(di); // escribir dias/anio
	WriteI2C(me); //escribir mes
	StopI2C();
}
