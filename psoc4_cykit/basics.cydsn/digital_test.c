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

uint8 random8(void) {
    prs_Step();
    return prs_Read();
}

void piezo_shift(uint8 delta) {
    piezo_pwm_WritePeriod(255 + delta);            
    piezo_pwm_WriteCompare(127 + delta / 2);
}

void greenled_set(uint8 alpha) {
    led_pwm_WriteCompare1(alpha);
}

void blueled_set(uint8 alpha) {
    led_pwm_WriteCompare2(alpha);
}

CY_ISR(systick_irq) { 
    if(!systick_divider) {
        systick = 1;
    }
    systick_divider = (systick_divider+1) % 25;
}

CY_ISR(sw2_irq) {
    // toggle red led
    uint8 val = rgb_red_Read();    
    redled_set(val);    
    sw2_int_ClearPending();
}

int main() {
    cystatus result;    
    uint32 eep_word = 0xDEADBEEF;    
    uint32 chipid[2];
    uint8 die_wafer, die_waferx, die_wafery;
    uint8 pwm_cmp = 0;
    
    CyGlobalIntEnable;    
    
    serial_Start();    
    eeprom_Start();    
    prs_Start();
    sw2_clk_Start();
    sw2_int_StartEx(sw2_irq);
    led_pwm_clk_Start();
    led_pwm_Start();      
    piezo_pwm_Start();

    // led
    redled_set(0x0);    
    greenled_set(0x0);
    blueled_set(0x0);
        
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
        
        greenled_set(pwm_cmp);
        blueled_set(random8());
        piezo_shift(pwm_cmp);     
        pwm_cmp += 10;        
    }
}
#endif