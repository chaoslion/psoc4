#include <project.h>
#include <stdio.h>

#define RES_VL 0
#define RES_CL 1
#define RES_OL 2
#define RES_OFFSET 200.0f
#define V_MAX (uint16)4095 // 2.048V
#define V_MIN (uint16)1599 // 0.8002 V

char buffer[64];
uint8 idac_val = 0;
uint8 update_counter = 0;
volatile uint8 done = 0;
volatile uint8 samples = 0;
volatile float accumulator = 0;
volatile uint8 result;

CY_ISR(tim_handler) {    
    // clear tim register
    tim_ReadStatusRegister();    
    uint16 vdrop;
       
    adc_StartConvert();            
    adc_IsEndConversion(adc_WAIT_FOR_RESULT);    
    vdrop = (uint16)adc_GetResult16(0);
        
    if( vdrop >= V_MAX ) {    
        result = RES_OL;        
        idac_val--;
    } else if( vdrop <= V_MIN ) {
        result = RES_CL;                    
    } else {          
        result = RES_VL;
        accumulator += vdrop / idac_val;
        samples++;
    }      
    
    
    if( !update_counter ) {   
        done = 1;
        idac_val = 0xFF;
    }
    
    idac_SetValue(idac_val);    
    update_counter++;    
}

int main() {             
    float resistance;
    
    CyGlobalIntEnable;               
    // uart
    uart_Start();        
    // idac
    idac_Start();    
    // adc+op amp
    adc_buffer_Start();
    adc_Start();
    // timer
    tim_Start();
    tim_irq_StartEx(tim_handler);
        
    while(1) {
        if( done ) {
            // all off
            rgb_red_Write(0x1);
            rgb_green_Write(0x1); 
            
            switch(result) {
                case RES_OL:
                case RES_CL:                    
                    rgb_red_Write(0x0);
                    if( result == RES_OL ) {
                        // resistor is too small, cant detect voltage level with 12 bit                    
                        uart_UartPutString("<OL>");                                    
                    } else {
                        // resistor is too large, even < 1.2uA generates voltages > 2048mV                                
                        uart_UartPutString("<CL>");  
                    }
                    break;
                default:                    
                    rgb_green_Write(0x0);
                    resistance = accumulator / samples;
                    resistance *=  2048.0f / (4095.0f * 0.0012f);
                    resistance -= RES_OFFSET;                    
                    sprintf(buffer, "<%f>", resistance);
                    uart_UartPutString(buffer);                   
                    accumulator = 0;
                    samples = 0;
            }                     
            done = 0;
        }
    }
}