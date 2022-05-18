
// PIC18F2320 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS     // Oscillator Selection bits (Internal RC oscillator, CLKO function on RA6 and port function on RA7)
#pragma config FSCM = ON        // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal/External Switchover bit (Internal/External Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset enable bit (Brown-out Reset enabled)
#pragma config BORV = 20        // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBAD = ANA       // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-0007FFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (000800-000FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (001000-0017FFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (001800-001FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0001FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-0007FFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (000800-000FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (001000-0017FFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (001800-001FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0001FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-0007FFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (000800-000FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (001000-0017FFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (001800-001FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0001FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <stdint.h>
#include <xc.h>


#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0


uint8_t flag = 0; 
uint16_t convert_ad =0;


void main(void) {
    //Ports Inicializer
    PORTA = 0x00; 
    PORTC = 0x00;
    TRISC = 0x00;
    TRISAbits.RA0 = INPUT;
    
    //Config AD
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    ADCON0 = 0b00000001;  // Configura canal AN0 - R0 para A/D
    ADCON1 = 0b00001110; // Configura canal AN0 - R0 para A/D
    ADCON2 = 0b10001101; // F = 8Mhz/2 = 4Hmz - Aquisition Time = 2*TAD
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1; 
    
    //Timers
    
    T0CON = 0b10010111;
    

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 1;   
    PIE1bits.RCIE = 0;
    TMR0H = 0xFF; //Timer 10ms
    TMR0L = 0xB1;
    INTCON2bits.TMR0IP =1; 
    RCONbits.IPEN = 1; //Desabilita prioridades;
    ADCON0bits.GO = 1;
    while(1){
       /* if (PORTAbits.RA0 == LOW){
            LATCbits.LATC7 = HIGH;
        }else{
            LATCbits.LATC7 = LOW;
        }*/
    }
    
    return;
}

void __interrupt(high_priority) my_isr(void){
    
    
     if (INTCONbits.TMR0IF == 1){  
        TMR0H = 0xFF;
        TMR0L = 0xB1;
        INTCONbits.TMR0IF = 0; 
    }
     
     if (PIR1bits.ADIF == 1){
         convert_ad = (uint16_t) ADRESH<<8;
         convert_ad = convert_ad + ADRESL;
         
         if(convert_ad > 0x0067){
             LATCbits.LATC7 =1;
         }else{
             LATCbits.LATC7 =0;
         }
         
          PIR1bits.ADIF = 0;
          ADCON0bits.GO = 1;
     }
}