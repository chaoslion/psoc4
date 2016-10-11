#define ACTIVE
#ifdef ACTIVE
#include <project.h>
#include <stdio.h>

static const uint8 eep_data[4] = {0x0, 0x0, 0x0, 0x0};
volatile uint8 systick = 0;
char buffer[64];
uint8 systick_divider = 0;

void get_die_location(uint8* wafer, uint8* x, uint8* y) {
    *wafer = *((reg8 *) CYREG_SFLASH_DIE_WAFER);
    *x = *((reg8 *) CYREG_SFLASH_DIE_X);
    *y = *((reg8 *) CYREG_SFLASH_DIE_Y);
}

void redled_set(uint8 state) {    
    rgb_red_Write(state?0x0:0x1);
}

CY_ISR(systick_irq) { 
    if(!systick_divider) {
        systick = 1;
    }
    systick_divider = (systick_divider+1) % 25;
}

int main() {
    cystatus result;    
    uint32 eep_word = 0xDEADBEEF;    
    uint32 chipid[2];
    uint8 die_wafer, die_waferx, die_wafery;
    
    CyGlobalIntEnable;    
    
    serial_Start();    
    eeprom_Start();        
    
    // led
    redled_set(0x0);    
    
    CyGetUniqueId(chipid);    
    sprintf(buffer, "Hello World from PSoC4 ID=%lx%lx\r\n", chipid[1], chipid[0]);
    serial_UartPutString(buffer);
    
    get_die_location(&die_wafer, &die_waferx, &die_wafery);
    sprintf(buffer, "I was born on Wafer #%d @ [%d, %d]\r\n", die_wafer, die_waferx, die_wafery);
    serial_UartPutString(buffer);

    // eeprom
    // check eep data for DEADBEEF        
    if( *(volatile uint32*)eep_data == 0xDEADBEEF ) {        
        serial_UartPutString("EEP: storage is valid\r\n");
    } else {
        result = eeprom_Write((uint8*)&eep_word, eep_data, 4); 
        if( result == CYRET_SUCCESS ) {
            serial_UartPutString("EEP: wrote word\r\n");
        } else {
            serial_UartPutString("EEP: failed to write word\r\n");
        }
    }

    // systick
    // 100 Hz
    CyIntSetSysVector((SysTick_IRQn + 16), systick_irq);    
    SysTick_Config(240000);   
       
    while(1) {   
        if( !systick ) {            
            continue;
        }
        systick = 0;       

        uint8 state = rgb_red_Read();        
        redled_set(state);
        sprintf(buffer, "Led is: %d\r\n", state);
        serial_UartPutString(buffer);      
    }
}
#endif