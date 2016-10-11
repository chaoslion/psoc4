#ifdef ACTIVE
#include <project.h>
#include <stdio.h>

const uint8 eep_word[4] = { 0xDE, 0xAD, 0xBE, 0xEF };
static const uint8 eep_data[4] = {0x0, 0x0, 0x0, 0x0};

extern uint8 capsense_SensorSignal[capsense_TOTAL_SENSOR_COUNT];
char buffer[64];
volatile uint8 systick = 0;
uint8 systick_divider = 0;
volatile uint8 sigpot = 0;

void get_die_location(uint8* wafer, uint8* x, uint8* y) {
    *wafer = *((reg8 *) CYREG_SFLASH_DIE_WAFER);
    *x = *((reg8 *) CYREG_SFLASH_DIE_X);
    *y = *((reg8 *) CYREG_SFLASH_DIE_Y);
}

void piezo_shift(uint8 delta) {
    piezo_pwm_WritePeriod(255 + delta);            
    piezo_pwm_WriteCompare(127 + delta / 2);
}

void redled_set(uint8 state) {    
    rgb_red_Write(state?0x0:0x1);
}

void greenled_set(uint8 alpha) {
    led_pwm_WriteCompare1(alpha);
}

void blueled_set(uint8 alpha) {
    led_pwm_WriteCompare2(alpha);
}

void adc_convert(uint8 with_inject) {
    if( with_inject ) {
        adc_EnableInjection();
    }        
    adc_StartConvert();
    adc_IsEndConversion(adc_WAIT_FOR_RESULT);
}


#define VREF (float)CYDEV_VDDD / 2.0f
int32 read_dietemp(void) {
    int32 counts;        
    counts = (int32)adc_GetResult16(adc_SEQUENCED_CHANNELS_NUM);
    // convert range
    counts = (int32)(VREF / 1.024f * counts);
    return dietemp_CountsTo_Celsius(counts);
}

uint16 read_pot(void) {
    uint16 counts;        
    counts = (uint16)adc_GetResult16(0);    
    return counts;
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

CY_ISR(comp_irq) {
    uint32 cmp;
    uint32 is = comp_GetInterruptSource();
    if( is & comp_INTR ) {         
        cmp = comp_GetCompare();
        sigpot = cmp;        
    }    
    comp_ClearInterrupt(is);
    comp_int_ClearPending();    
}

uint8 random8(void) {
    prs_Step();
    return prs_Read();
}

int main() {
    cystatus result;    
    uint32 eep_word = 0xDEADBEEF;    
    uint32 chipid[2];
    uint8 die_wafer, die_waferx, die_wafery;
    uint16 slider, proximity;
    int32 dietemp;          
    uint16 potval;
    float vpotval;
    uint8 tempdiv = 0;
    uint8 potperc;
    
    CyGlobalIntEnable;    
            
    // capsense
    capsense_EnableWidget(capsense_SLIDER__LS);
    capsense_EnableWidget(capsense_PROXIMITY__PROX);
    
    #ifdef TUNE
    capsense_TunerStart();
    #else
    serial_Start();    
    prs_Start();
    eeprom_Start();        
    sw2_clk_Start();
    sw2_int_StartEx(sw2_irq);
    adc_Start();
    led_pwm_clk_Start();
    led_pwm_Start();      
    piezo_pwm_Start();
    comp_Start();
    comp_int_StartEx(comp_irq);
    capsense_Start();
    
    capsense_InitializeAllBaselines();
    
    // set sigpot
    sigpot = comp_GetCompare();
    
    // leds
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
    #endif
       
    while(1) {   
        #ifdef TUNE
           capsense_TunerComm();
        #else
        capsense_ScanEnabledWidgets();
        while(capsense_IsBusy());        
        capsense_UpdateEnabledBaselines();
        
        slider = capsense_GetCentroidPos(capsense_SLIDER__LS);    
        proximity = capsense_GetDiffCountData(5);
        
        blueled_set((uint8)proximity);                                   
        piezo_shift(proximity);
                
        if( proximity == 0 ) {
            // turn off
            piezo_pwm_WriteCompare(0);
        }
                
        if( slider != 0xFFFF ) {            
            greenled_set((uint8)slider);            
        }
                                    
        if( !systick ) {            
            continue;
        }
        systick = 0;       
        
        // clear screen
        serial_UartPutChar(12);
        
        //greenled_set(random8());
        //blueled_set(random8());
        // read temp only every 2 sec -> 250ms/8
        adc_convert(tempdiv == 0);
        tempdiv = (tempdiv+1) % 8;
        
        dietemp = read_dietemp();
        potval = read_pot();
        vpotval = adc_CountsTo_Volts(0, potval);
        potperc = (uint8)(100.0f * vpotval / 1.1f);
        
        sprintf(buffer, "PROX: %d\r\nSLID: %d\r\n", proximity, slider);
        serial_UartPutString(buffer);    
                
        
        sprintf(buffer, "TEMP: %d\r\nPOT: %.2f(%d%%)\r\nCMP:[%c]\r\n", (int)dietemp, vpotval, potperc, sigpot?'>':'<');
        serial_UartPutString(buffer);      
        #endif
    }
}
#endif