/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include "system\typedefs.h"

#include "system\usb\usb.h"

#include "io_cfg.h"             // I/O pin mapping
#include "user\user.h"
#include <spi.h> //librería SPI para el sensor de temperatura

/** V A R I A B L E S ********************************************************/
#pragma udata

// Definir variables
DATA_PACKET buffer;

/** P R I V A T E  P R O T O T Y P E S ***************************************/

void BlinkUSBStatus(void);
void ServiceRequests(void);


/** D E C L A R A T I O N S **************************************************/
#pragma code
void UserInit(void)
{
    mInitAllLEDs();

    /*Inicializacion del potenciometro*/
    TRISAbits.TRISA0 = 1; //entrada
    ADCON0 = 0x01; //activa el A/D y selecciona canal 0
    ADCON2 = 0x3C; //
    ADCON2bits.ADFM = 1; //resultados A/D justificados a la derecha
    /*Inicialización del SPI para el sensor de temperatura*/
    LATBbits.LATB2 = 1; //ponemos un uno, para poder provocar flanco de bajada después y seleccionar (RB2 es el chip select)
    TRISBbits.TRISB2 = 0; //configuramos pin como salida (es el chip select)
    OpenSPI(SPI_FOSC_64, MODE_11, SMPMID); //inicializamos SPI

}//end UserInit


/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{
    BlinkUSBStatus();
    // User Application USB tasks
    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) return;

    ServiceRequests();


}//end ProcessIO



void ServiceRequests(void)
{
   //incluir aqui vuestras rutinas
   byte len; //longitud de la información a enviar
   WORD temp; //variable para guardar la temperatura

   /*Leemos del USB datos hasta completar el tamaño de un paquete (buffer es de tipo DATA_PACKET)*/
   if(USBGenRead((byte*) &buffer, sizeof(buffer)))
   {
     len = 0;
     switch(buffer.CMD) //leemos el primer campo de la estructura de datos DATA_PACKET (dada en user.h)
     {
       case LEER_VERSION: //estas etiquetas se dan definidas en user.h para el campo CMD, en función del comando que recibamos se realizará una cosa u otra
          buffer._byte[1] = MAYOR_VERSION; //se reciben primero los bits de mayor peso y luego los de menor peso
          buffer._byte[2] = MENOR_VERSION;
          len = 0x03;
          break;
       case LEER_POT:
          ADCON0bits.GO = 1; // ponemos a uno el bit que inicia la conversión
          while(ADCON0bits.NOT_DONE); // esperamos a que se ponga a 0 el bit que indica si la conversión está en curso
          buffer._byte[1] = ADRESL; //se reciben los datos en sentido contrario a la versión primero los de menor peso y luego los de mayor peso
          buffer._byte[2] = ADRESH;
          len = 0x03;
          break;
       case ACTUALIZA_LED:
          if(buffer.led_num == 3) //comprobamos el número de led (led_num también viene dado en user.h)
            mLED_3 = buffer.led_status; //actualiza el led 3 según la info en buffer (variable definida en io_cfg)
          else if(buffer.led_num == 4)
            mLED_4 = buffer.led_status; //enciende el led 4 según la info en buffer
          len = 0x01;
          break;
       case LEER_TEMP:
          LATBbits.LATB2 = 0; //provocamos flanco de bajada para seleccionar
          temp.v[1] = ReadSPI(); //primero bits más significativos
          temp.v[0] = ReadSPI();
          LATBbits.LATB2 = 1; //deseleccionamos
          buffer.word_data = temp._word;
          len = 0x03;
          break;
     }
     if(!mUSBGenTxIsBusy()) //Si Tx no está ocupado
       USBGenWrite((byte*)&buffer,len); //escribimos
   }
}//end ServiceRequests

/******************************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs corresponding to
 *                  the USB device state.
 *
 * Note:            mLED macros can be found in io_cfg.h
 *                  usb_device_state is declared in usbmmap.c and is modified
 *                  in usbdrv.c, usbctrltrf.c, and usb9.c
 *****************************************************************************/
void BlinkUSBStatus(void)
{
    static word led_count=0;

    if(led_count == 0)led_count = 10000U;
    led_count--;

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(UCONbits.SUSPND == 1)
    {
        if(led_count==0)
        {
            mLED_1_Toggle();
            mLED_2 = mLED_1;        // Both blink at the same time
        }//end if
    }
    else
    {
        if(usb_device_state == DETACHED_STATE)
        {
            mLED_Both_Off();

        }
        else if(usb_device_state == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(usb_device_state == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(usb_device_state == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(usb_device_state == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(usb_device_state == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                mLED_1_Toggle();
                mLED_2 = !mLED_1;       // Alternate blink
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus

/** EOF user.c ***************************************************************/
