/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "cytypes.h"
#include "cyfitter.h"
#include "`$INSTANCE_NAME`_rgbpwm.h"

void `$INSTANCE_NAME`_Start(void) {
    `$INSTANCE_NAME`_SetColorB(0x0, 0x0, 0x0);
}

void `$INSTANCE_NAME`_SetColorB(uint8 red, uint8 green, uint8 blue) {
    `$INSTANCE_NAME`_Red = red;
    `$INSTANCE_NAME`_Green = green;
    `$INSTANCE_NAME`_Blue = blue;
}

void `$INSTANCE_NAME`_SetColorW(uint32 color) {
    if( color > 0xFFFFFF ) {
        return;
    }
    `$INSTANCE_NAME`_SetColorB(
        (uint8)(color >> `$INSTANCE_NAME`_RED_SHIFT),
        (uint8)(color >> `$INSTANCE_NAME`_GREEN_SHIFT),
        (uint8)(color >> `$INSTANCE_NAME`_BLUE_SHIFT)
     );
}

void `$INSTANCE_NAME`_SetColorF(float red, float green, float blue) {
    if( !(red >= 0 && red <= 1 && green >= 0 && green <= 1 && blue >= 0 && blue <= 1) ) {
        return;
    }
    `$INSTANCE_NAME`_SetColorB(
        (uint8)(red*0xFF),
        (uint8)(green*0xFF),
        (uint8)(blue*0xFF)
    );
}

// void `$INSTANCE_NAME`_SetIntensityB(uint8);

void `$INSTANCE_NAME`_SetIntensityF(float intensity) {
    if( !(intensity >= 0 && intensity <= 1) ) {
        return;
    }
    // get colors & mix with intensity
    float red = `$INSTANCE_NAME`_Red * intensity / 0xFF;
    float green = `$INSTANCE_NAME`_Green * intensity / 0xFF;
    float blue = `$INSTANCE_NAME`_Blue * intensity / 0xFF;
    
    `$INSTANCE_NAME`_SetColorF(red, green, blue);
}

/* [] END OF FILE */
