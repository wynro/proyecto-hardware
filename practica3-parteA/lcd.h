/** @file lcd.h
 *  @brief Funciones de visualizacion y control LCD
 *  @version <P6-ARM>
 */

#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

  /*--- define macros---*/

  /* tamano pantalla */
#define TLCD_160_240            (0)
#define VLCD_240_160            (1)
#define CLCD_240_320            (2)
#define MLCD_320_240            (3)
#define ELCD_640_480            (4)
#define SLCD_160_160            (5)
#define LCD_TYPE                        MLCD_320_240

#if(LCD_TYPE==TLCD_160_240)
#define SCR_XSIZE                       (160)
#define SCR_YSIZE                       (240)
#define LCD_XSIZE                       (160)
#define LCD_YSIZE                       (240)
#elif(LCD_TYPE==VLCD_240_160)
#define SCR_XSIZE                       (240)
#define SCR_YSIZE                       (160)
#define LCD_XSIZE                       (240)
#define LCD_YSIZE                       (160)
#elif(LCD_TYPE==CLCD_240_320)
#define SCR_XSIZE                       (240)
#define SCR_YSIZE                       (320)
#define LCD_XSIZE                       (240)
#define LCD_YSIZE                       (320)
#elif(LCD_TYPE==MLCD_320_240)
#define SCR_XSIZE                       (320)
#define SCR_YSIZE                       (240)
#define LCD_XSIZE                       (320)
#define LCD_YSIZE                       (240)
#elif(LCD_TYPE==ELCD_640_480)
#define SCR_XSIZE                       (640)
#define SCR_YSIZE                       (480)
#define LCD_XSIZE                       (640)
#define LCD_YSIZE                       (480)
#elif(LCD_TYPE==SLCD_160_160)
#define SCR_XSIZE                       (160)
#define SCR_YSIZE                       (160)
#define LCD_XSIZE                       (160)
#define LCD_YSIZE                       (160)
#endif

  /* screen color */
#define MODE_MONO                       (1)
#define MODE_GREY4                      (4)
#define MODE_GREY16             (16)
#define MODE_COLOR                      (256)

#define Ascii_W                         8
#define XWIDTH                          6
#define BLACK                           0xf
#define WHITE                           0x0
#define LIGHTGRAY               0x5
#define DARKGRAY                0xa
#define TRANSPARENCY            0xff

#define HOZVAL                          (LCD_XSIZE/4-1)
#define HOZVAL_COLOR            (LCD_XSIZE*3/8-1)
#define LINEVAL                         (LCD_YSIZE -1)
#define MVAL                            (13)
#define M5D(n)                          ((n) & 0x1fffff)
#define MVAL_USED                       0

  /* tamano array */
#define ARRAY_SIZE_MONO         (SCR_XSIZE/8*SCR_YSIZE)
#define ARRAY_SIZE_GREY4        (SCR_XSIZE/4*SCR_YSIZE)
#define ARRAY_SIZE_GREY16       (SCR_XSIZE/2*SCR_YSIZE)
#define ARRAY_SIZE_COLOR        (SCR_XSIZE/1*SCR_YSIZE)

  /* clkval */
#define CLKVAL_MONO             (12)
#define CLKVAL_GREY4            (12)
#define CLKVAL_GREY16           (12)
#define CLKVAL_COLOR            (10)

#define LCD_BUF_SIZE            (SCR_XSIZE*SCR_YSIZE/2)
#define LCD_ACTIVE_BUFFER       (0xc300000)
#define LCD_VIRTUAL_BUFFER      (0xc300000 + LCD_BUF_SIZE)

#define LCD_PutPixel(x, y, c)						\
  (*(INT32U *)(LCD_VIRTUAL_BUFFER+ (y) * SCR_XSIZE / 2 + ( (x)) / 8 * 4)) = \
    (((*(INT32U *)(LCD_VIRTUAL_BUFFER+ (y) * SCR_XSIZE / 2 + ( (x)) / 8 * 4)) & \
      (~(0xf0000000 >> ((( (x))%8)*4))))|((c) << (7 - ( (x))%8) * 4))
#define LCD_Active_PutPixel(x, y, c)					\
  (*(INT32U *)(LCD_ACTIVE_BUFFER + (y) * SCR_XSIZE / 2 + (319 - (x)) / 8 * 4)) = \
    (((*(INT32U *)(LCD_ACTIVE_BUFFER + (y) * SCR_XSIZE / 2 + (319 - (x)) / 8 * 4)) & \
      (~(0xf0000000 >> (((319 - (x))%8)*4)))) |((c) << (7 - (319 - (x))%8) * 4))
  
#define GUISWAP(a, b){a^=b; b^=a; a^=b;}
  
  /*--- declaracion de funciones ---*/

/**
 * Devuelve el color en las coordenadas dadas
 *
 * @param[in] usX Coordenada X del punto
 * @param[in] usY Coordenada Y del punto
 */
INT8U LCD_GetPixel(INT16U usX, INT16U usY);
  
/**
 * Devuelve el color en el punto de la pantalla dado
 *
 * @param[in] usX Coordenada X a observar
 * @param[in] usY Coordenada Y a observar
 * @return Color en el punto dado, o indeterminado en caso de que el
 * punto no pertenezca a la pantalla
 */
INT8U LCD_GetPixel(INT16U usX, INT16U usY);

/**
 * Inicializa el sistema de la pantalla
 */
void  Lcd_Init(void);

/**
 * Limpia el buffer real de la pantalla, actualizandola
 */
void  Lcd_Active_Clr(void);

/**
 * Limpia el buffer virtual de la pantalla
 */
void  Lcd_Clr(void);

/**
 * Rellena el rectangulo determinado en el lugar dado con el color
 * dado
 *
 * @param[in] usLeft Coordenada X de la esquina superior izquierda
 * @param[in] usTop Coordenada Y de la esquina superior izquierda
 * @param[in] usRight Coordenada X de la esquina inferior derecha
 * @param[in] usBottom Coordenada Y de la esquina inferior derecha
 * @param[in] ucColor Color a usar para el rellenado
 */
void  LcdClrRect(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor);

/**
 * Dibuja un rectangulo (vacio) con las coordenadas dadas, con el
 * borde del color de linea dado
 *
 * @param[in] usLeft Coordenada X de la esquina superior izquierda
 * @param[in] usTop Coordenada Y de la esquina superior izquierda
 * @param[in] usRight Coordenada X de la esquina inferior derecha
 * @param[in] usBottom Coordenada Y de la esquina inferior derecha
 * @param[in] ucColor Color a usar para el borde
 */
void  Lcd_Draw_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor);

/**
 * Invierte los colores a lo largo del borde del rectangulo dado
 *
 * @param[in] usLeft Coordenada X de la esquina superior izquierda
 * @param[in] usTop Coordenada Y de la esquina superior izquierda
 * @param[in] usRight Coordenada X de la esquina inferior derecha
 * @param[in] usBottom Coordenada Y de la esquina inferior derecha
 * @param[in] ucColor
 */
void  Lcd_Draw_Box_inverted(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor);

/**
 * Rellena el rectangulo determinado en el lugar dado con el color
 * dado
 *
 * @param[in] usLeft Coordenada X de la esquina superior izquierda
 * @param[in] usTop Coordenada Y de la esquina superior izquierda
 * @param[in] usRight Coordenada X de la esquina inferior derecha
 * @param[in] usBottom Coordenada Y de la esquina inferior derecha
 * @param[in] ucColor Color a usar para el rellenado
 */
void  Lcd_Draw_Filled_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor);

/**
 * Dibuja un linea
 *
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] usX1 Coordenada X del punto final
 * @param[in] usY1 Coordenada Y del punto final
 * @param[in] ucColor Color a usar
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_Line(INT16 usX0, INT16 usY0, INT16 usX1, INT16 usY1, INT8U ucColor, INT16U usWidth);

/**
 * Dibuja un linea horizontal
 *
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] usX1 Coordenada X del punto final
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] ucColor Color a usar
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_HLine(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor, INT16U usWidth);

/**
 * Dibuja un linea vertical
 *
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] usY1 Coordenada Y del punto final
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] ucColor Color a usar
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_VLine(INT16 usY0, INT16 usY1, INT16 usX0, INT8U ucColor, INT16U usWidth);

/**
 * Dibuja un linea vertical punteada
 *
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] usX1 Coordenada X del punto final
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] ucColor Color a usar
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_HLine_pointed(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor, INT16U usWidth);

/**
 * Dibuja un linea horizontal punteada
 *
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] usY1 Coordenada Y del punto final
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] ucColor Color a usar
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_VLine_pointed(INT16 usY0, INT16 usY1, INT16 usX0, INT8U ucColor, INT16U usWidth);

/**
 * Invierte el color a lo largo de una linea horizontal
 *
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] usX1 Coordenada X del punto final
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_HLine_inverted(INT16 usX0, INT16 usX1, INT16 usY0, INT16U usWidth);

/**
 * Invierte el color a lo largo de una linea vertical
 *
 * @param[in] usY0 Coordenada Y del punto inicial
 * @param[in] usY1 Coordenada Y del punto final
 * @param[in] usX0 Coordenada X del punto inicial
 * @param[in] usWidth Anchura de la linea
 */
void  Lcd_Draw_VLine_inverted(INT16 usY0, INT16 usY1, INT16 usX0, INT16U usWidth);

/**
 * Escribe la cadena dada, en una fuente de 8x16, en la coordenada
 * dada. Las coordenadas dadas seran las de la esquina superior
 * izquierda del primer caracter
 *
 * @param[in] x0 Coordenada X del primer caracter de la cadena
 * @param[in] y0 Coordenada Y del primer caracter de la cadena
 * @param[in] ForeColor Color en el que se escribiran los caracteres
 * @param[in] s Vector de caracteres a escribir, terminados en '\0'
 */
void  Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U* s);


/**
 * Escribe la cadena dada, con los caracteres invertidos, en una
 * fuente de 8x16, en la coordenada dada. Las coordenadas dadas
 * seran las de la esquina superior izquierda del primer
 * caracter. El caracter sera escrito en transparencia, mientras que
 * el espacio no ocupado en el caracter se rellenara del color dado.
 *
 * @param[in] x0 Coordenada X del primer caracter de la cadena
 * @param[in] y0 Coordenada Y del primer caracter de la cadena
 * @param[in] ForeColor Color en el que se rellenara el espacio libre
 * @param[in] s Vector de caracteres a escribir, terminados en '\0'
 */
void  Lcd_DspAscII8x16_inverted(INT16U x0, INT16U y0, INT8U ForeColor, INT8U* s);

/**
 * Escribe la cadena dada, centrado horizontalmente en la pantalla,
 * en una fuente de 8x16, en la coordenada dada. Las coordenadas
 * dadas seran las de la esquina superior izquierda del primer
 * caracter.
 *
 * @param[in] y0 Coordenada Y del primer caracter de la cadena
 * @param[in] ForeColor Color en el que se escribira el caracter
 * @param[in] s Vector de caracteres a escribir, terminados en '\0'
 */
void  Lcd_DspAscII8x16HorizontallyCentered(INT16U y0, INT8U ForeColor, INT8U * s);

/**
 * Escribe la cadena dada, centrado horizontalmente en la pantalla,
 * con los caracteres invertidos, en una fuente de 8x16, en la
 * coordenada dada. Las coordenadas dadas seran las de la esquina
 * superior izquierda del primer caracter. El caracter sera escrito
 * en transparencia, mientras que el espacio no ocupado en el
 * caracter se rellenara del color dado.
 *
 * @param[in] y0 Coordenada Y del primer caracter de la cadena
 * @param[in] ForeColor Color en el que se rellenara el espacio libre
 * @param[in] s Vector de caracteres a escribir, terminados en '\0'
 */
void  Lcd_DspAscII8x16HorizontallyCentered_inverted(INT16U y0, INT8U ForeColor, INT8U * s);

/**
 * Escribe la caracter dado, en una fuente de 8x16, en la coordenada
 * dada. Las coordenadas dadas seran las de la esquina superior
 * izquierda del primer caracter. 
 *
 * @param[in] usX0 Coordenada X del caracter
 * @param[in] usY0 Coordenada Y del caracter
 * @param[in] ForeColor Color en el que se escribira el caracter
 * @param[in] s Caracter a escribir
 */
void  Lcd_DisplayChar(INT16U usX0, INT16U usY0, INT8U ForeColor, INT8U ucChar);

/**
 * Escribe la cadena dada, con los caracteres invertidos, en una
 * fuente de 8x16, en la coordenada dada. Las coordenadas dadas
 * seran las de la esquina superior izquierda del primer
 * caracter. El caracter sera escrito en transparencia, mientras que
 * el espacio no ocupado en el caracter se rellenara del color dado.
 *
 * @param[in] usX0 Coordenada X del caracter
 * @param[in] usY0 Coordenada Y del caracter
 * @param[in] ForeColor Color en el que se rellenara el espacio libre
 * @param[in] ucChar Caracter a escribir
 */
void  Lcd_DisplayChar_inverted(INT16U usX0, INT16U usY0, INT8U ForeColor, INT8U ucChar);

/**
 * Invierte los colores a lo largo de lineas de la pantalla
 *
 * @param[in] ulHeight Numero de lineas a invertir
 * @param[in] ulY Coordenada Y de la primera linea
 */
void  ReverseLine(INT32U ulHeight, INT32U ulY);

/**
 * Invierte los colores en el rectangulo dado
 *
 * @param[in] ulX0 Coordenada X de la esquina superior izquierda
 * @param[in] ulY0 Coordenada Y de la esquina superior izquierda
 * @param[in] ulX1 Coordenada X de la esquina inferior derecha
 * @param[in] ulY1 Coordenada Y de la esquina inferior derecha
 */
void  ReverseSquare(INT32U ulX0, INT32U ulY0, INT32U ulX1, INT32U ulY1);

/**
 * Dibuja una circunferencia
 *
 * @param[in] X Coordenada X del centro
 * @param[in] Y Coordenada Y del centro
 * @param[in] radius Radio del circulo
 * @param[in] ForeColor Color a usar
 */
void  Lcd_Circle(INT8 X, INT8 Y, INT16 radius, INT8U ForeColor);

/**
 * Dibuja un circulo. Es bastante impreciso
 *
 * @param[in] X Coordenada X del centro
 * @param[in] Y Coordenada Y del centro
 * @param[in] radius Radio del circulo
 * @param[in] ForeColor Color a usar
 */
void Lcd_Circle_Filled(INT8 X, INT8 Y, INT16 radius, INT8U ForeColor);

/**
 * Copia el buffer virtual al buffer real mediante DMA, bloqueante
 * hasta terminar el mismo.
 */
void  Lcd_Dma_Trans(void);

/**
 * Copia el buffer virtual al buffer real mediante DMA, no bloqueante
 */
void Lcd_Dma_Trans_non_block(void);

/**
 * Espera a la finalizacion del DMA de pantalla, bloqueante
 */
void Lcd_Dma_Trans_wait(void);

//Las siguientes funciones estaban definidas pero no implementadas

///**
// * Invierte el rectangulo dado
// *
// * @param[in] usX0 Coordenada X de la esquina superior izquierda
// * @param[in] usY0 Coordenada Y de la esquina superior izquierda
// * @param[in] usX1 Coordenada X de la esquina inferior derecha
// * @param[in] usY1 Coordenada Y de la esquina inferior derecha
// */
//void  Lcd_Anti_Disp(INT16U usX0, INT16U usY0, INT16U usX1, INT16U usY1);
//
///**
// * Escribe la cadena dada, en una fuente de 8x16, en color negro, en
// * la coordenada dada. Las coordenadas dadas seran las de la esquina
// * superior izquierda del primer caracter
// *
// * @param[in] usX0 Coordenada X del primer caracter de la cadena
// * @param[in] usY0 Coordenada Y del primer caracter de la cadena
// * @param[in] pucStr Vector de caracteres a escribir, terminados en '\0'
// */
//void  Lcd_DisplayString(INT16U usX0, INT16U usY0, INT8U *pucStr);
//
///**
// * Escribe el caracter de codigo dado, en una fuente de 8x16, en color
// * negro, en la coordenada dada. Las coordenadas dadas seran las de la
// * esquina superior izquierda del primer caracter
// *
// * @param[in] sX Coordenada X del primer caracter de la cadena
// * @param[in] sY Coordenada Y del primer caracter de la cadena
// * @param[in] usInt Codigo ASCII del caracter a escribir
// */
//void  Lcd_DisplayShort(INT16 sX, INT16 sY, INT16U usInt);
//
///**
// * Escribe la cadena dada, en una fuente de 6x8, en la coordenada
// * dada. Las coordenadas dadas seran las de la esquina superior
// * izquierda del primer caracter
// *
// * @param[in] usX0 Coordenada X del primer caracter de la cadena
// * @param[in] usY0 Coordenada Y del primer caracter de la cadena
// * @param[in] ForeColor Color en el que se escribira la cadena
// * @param[in] pucStr Vector de caracteres a escribir, terminados en '\0'
// */
//void  Lcd_DspAscII6x8(INT16U usX0, INT16U usY0,INT8U ForeColor, INT8U* pucChar);
//
///**
// * No tengo la mas remota idea de que hace esto
// *
// * @param[in] x0 undefined
// * @param[in] y0 undefined
// * @param[in] ForeColor undefined
// * @param[in] s undefined
// */
//void  Lcd_DspHz16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U *s);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H__ */
