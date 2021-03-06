#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include"ST7735.h"

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // use slowest wdt    //???
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

#define MSG_LEN 100

int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // LED pin as output
    TRISAbits.TRISA4 = 0;

    // Pushbutton pin as input
    TRISBbits.TRISB4 = 1;

    // Turn on LED
    LATAbits.LATA4 = 1;

    LCD_init();
    LCD_clearScreen(BLACK);
    __builtin_enable_interrupts();

    char msg[MSG_LEN], fps[MSG_LEN];
    int counter;
    float FPS;

    while(1) {
	     // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
       // remember the core timer runs at half the sysclk
      for (counter = 0; counter < BAR_LEN; counter++) {
        _CP0_SET_COUNT(0);
        sprintf(msg, "Hello World %d  ", counter);
        LCD_drawString(28, 32, msg, WHITE, BLACK);
        sprintf(msg, "Jusong Cai ME433  ");
        LCD_drawString(27, 65, msg, YELLOW, BLACK);
        LCD_drawBar(16, 50, 3, counter, RED, BLACK);
        FPS = 24000000 / _CP0_GET_COUNT();
        sprintf(fps, "FPS = %1.2f", FPS);
        LCD_drawString(28, 90, fps, GREEN, BLACK);
        while (_CP0_GET_COUNT() < 2400000) { ; }

      }
    }
}