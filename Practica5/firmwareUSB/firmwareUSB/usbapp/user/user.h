
#ifndef _USUARIO_DEF
#define _USUARIO_DEF

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"


/* version firmware */
#define MENOR_VERSION   0x00    // Version 1.00
#define MAYOR_VERSION   0x01

/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    byte _byte[USBGEN_EP_SIZE];  //For byte access
    word _word[USBGEN_EP_SIZE/2];//For word access(USBGEN_EP_SIZE msut be even)
    struct
    {
        enum
        {
            LEER_VERSION    = 0x00,
			      LEER_POT        = 0x01,
            ACTUALIZA_LED   = 0x02,
            LEER_TEMP       = 0x03
        }CMD;
        byte len;
    };
    struct
    {
        unsigned :8;
        byte ID;
    };
    struct
    {
        unsigned :8;
        byte led_num; //numero de led
        byte led_status; //bool encencido/apagado
    };
    struct
    {
        unsigned :8;
        word word_data;
    };
} DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);

#endif //_USUARIO_DEF
