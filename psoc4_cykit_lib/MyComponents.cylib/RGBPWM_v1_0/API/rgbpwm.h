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
#define `$INSTANCE_NAME`_RED_SHIFT 16
#define `$INSTANCE_NAME`_GREEN_SHIFT 8
#define `$INSTANCE_NAME`_BLUE_SHIFT 0

#define `$INSTANCE_NAME`_Red (*(reg8 *) `$INSTANCE_NAME`_red_green_u0__D0_REG )
#define `$INSTANCE_NAME`_Green (*(reg8 *) `$INSTANCE_NAME`_red_green_u0__D1_REG )
#define `$INSTANCE_NAME`_Blue (*(reg8 *) `$INSTANCE_NAME`_blue_u0__D0_REG )

void `$INSTANCE_NAME`_Start(void);
void `$INSTANCE_NAME`_SetColorW(uint32);
void `$INSTANCE_NAME`_SetColorB(uint8, uint8, uint8);
void `$INSTANCE_NAME`_SetColorF(float, float, float);
void `$INSTANCE_NAME`_SetIntensityB(uint8);
void `$INSTANCE_NAME`_SetIntensityF(float);

/* [] END OF FILE */
