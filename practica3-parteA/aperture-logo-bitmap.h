/*
 * @file aperture-logo-bitmap.h
 * @brief Archivo de recursos que contiene el logo de Aperture
 *
 * @author Guillermo Robles Gonzalez
 */

#ifndef APERTURE_LOGO_BITMAP_H_
#define APERTURE_LOGO_BITMAP_H_

#include "lcd.h"
#include "bmp.h"

#define APERTURE_LOGO_HEIGHT 100
#define APERTURE_LOGO_WIDTH 100

const INT8U apertureBitmap[APERTURE_LOGO_HEIGHT * APERTURE_LOGO_WIDTH];
STRU_BITMAP Stru_Bitmap_aperture;

#endif /* APERTURE_LOGO_BITMAP_H_ */
