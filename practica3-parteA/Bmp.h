/** @file Bmp.h
 *  @brief Definicion mapas de bits del LCD
 * 
 * @author 
 */

#ifndef __BMP_H_
#define __BMP_H_

#include "def.h"

/* estructura mapa de bits */
typedef struct BITMAP
{
    INT8U 		ucFlags;          	// combination of flags above
    INT8U 		ucBitsPix;        	// 1, 2, 4, or 8
    INT16U 		usWidth;        	// in pixels
    INT16U  	usHeight;        	// in pixels
    INT32U 		ulTransColor;     	// transparent color for > 8bpp bitmaps
    INT8U 		*pucStart;  		// bitmap data pointer
}STRU_BITMAP, *pSTRU_BITMAP;

/*--- definicion macros ---*/

#define BLACK 		0xf
#define WHITE 		0x0
#define LIGHTGRAY   0x5
#define DARKGRAY    0xa
#define TRANSPARENCY 0xff

/** 
 * @brief Display bitmap in virtual buffer
 * 
 * @param[in] x X coordinate of the position to draw
 * @param[in] y Y coordinate of the position to draw
 * @param[in] Stru_Bitmap Bitmap to draw
 */
void BitmapView(INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap);




void BitmapViewHorizontallyCentered(INT16U y, STRU_BITMAP Stru_Bitmap);
void BitmapViewVerticallyCentered(INT16U x, STRU_BITMAP Stru_Bitmap);
void BitmapViewCentered(STRU_BITMAP Stru_Bitmap);

/** 
 * @brief Push bitmap data into LCD active buffer
 * 
 * @param[in] x X coordinate of the position to draw
 * @param[in] y Y coordinate of the position to draw
 * @param[in] Stru_Bitmap Bitmap to draw
 */
void BitmapPush(INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap);

/** 
 * @brief Pop bitmap data into LCD active buffer
 * 
 * @param[in] x X coordinate of the position to pop into
 * @param[in] y Y coordinate of the position to pop into
 * @param[in] Stru_Bitmap Bitmap to pop
 */
void BitmapPop(INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap);

/** 
 * @brief Cursor init
 */
void CursorInit(void);

/** 
 * @brief Cursor display
 * 
 * @param[in] x X coordinate of the position to draw
 * @param[in] y Y coordinate of the position to draw
 */
void CursorView(INT16U x, INT16U y);

/** 
 * @brief Cursor push
 * 
 * @param[in] x X coordinate of the position to push
 * @param[in] y Y coordinate of the position to push
 */
void CursorPush(INT16U x, INT16U y);

/** 
 * @brief Cursor pop
 */
void CursorPop(void);
#endif /* __BMP_H_ */
