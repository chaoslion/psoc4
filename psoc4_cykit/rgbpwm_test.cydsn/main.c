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
#include <project.h>

int main() {
    uint32 i, j;
    j = 0;
    CyGlobalIntEnable;
    
    rgbpwm_Start();
    rgbpwm_SetColorB(255, 0 ,0 );
    rgbpwm_SetColorW(0xFF00FF);
    rgbpwm_SetIntensityF(.5);
    rgbpwm_SetColorF(.5, .1, .3);
    rgbpwm_SetIntensityF(.5);
        
    for(;;) {
        for(i=0;i<256;i+=2) {
            rgbpwm_SetColorW(i << j);                    
            CyDelay(20);
        }
        j = (j+8) % 24;   
    }
}

/* [] END OF FILE */
