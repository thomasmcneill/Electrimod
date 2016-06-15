#pragma config OSC = HSPLL
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOREN = ON
#pragma config BORV = 2
#pragma config WDT = OFF
#pragma config WDTPS = 32768
#pragma config MCLRE = ON
#pragma config LPT1OSC = OFF
#pragma config PBADEN = OFF
#pragma config CCP2MX = PORTBE
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config XINST = OFF
//==========================================================================
//==========================================================================
//   IMPORTANT: For the PIC18 devices, the __CONFIG directive has been
//              superseded by the CONFIG directive.  The following settings
//              are available for this device.
//
//   Oscillator Selection:
//     OSC = LP             //LP
//     OSC = XT             //XT
//     OSC = HS             //HS
//     OSC = RC             //RC
//     OSC = EC             //EC-OSC2 as Clock Out
//     OSC = ECIO6          //EC-OSC2 as RA6
//     OSC = HSPLL          //HS-PLL Enabled
//     OSC = RCIO6          //RC-OSC2 as RA6
//     OSC = INTIO67        //INTRC-OSC2 as RA6, OSC1 as RA7
//     OSC = INTIO7         //INTRC-OSC2 as Clock Out, OSC1 as RA7
//
//   Fail-Safe Clock Monitor:
//     FCMEN = OFF          Disabled
//     FCMEN = ON           Enabled
//
//   Internal External Osc. Switch Over:
//     IESO = OFF           Disabled
//     IESO = ON            Enabled
//
//   Power-up Timer:
//     PWRT = ON            Enabled
//     PWRT = OFF           Disabled
//
//  Brown-out Reset:
//     BOREN = OFF          Disabled
//     BOREN = ON           SBOREN Enabled
//     BOREN = NOSLP        Enabled except Sleep, SBOREN Disabled
//     BOREN = SBORDIS      Enabled, SBOREN Disabled
//
//   Brown-out Voltage:
//     BORV = 0             Maximum setting
//     BORV = 1             
//     BORV = 2             
//     BORV = 3             Minimum setting
//
//   Watchdog Timer:
//     WDT = OFF            Disabled
//     WDT = ON             Enabled
//
//   Watchdog Postscaler:
//     WDTPS = 1            1:1
//     WDTPS = 2            1:2
//    WDTPS = 4            1:4
//     WDTPS = 8            1:8
//     WDTPS = 16           1:16
//     WDTPS = 32           1:32
//     WDTPS = 64           1:64
//     WDTPS = 128          1:128
//     WDTPS = 256          1:256
//     WDTPS = 512          1:512
//     WDTPS = 1024         1:1024
//     WDTPS = 2048         1:2048
//     WDTPS = 4096         1:4096
//     WDTPS = 8192         1:8192
//     WDTPS = 16384        1:16384
//     WDTPS = 32768        1:32768
//
//   MCLR Enable:
//     MCLRE = OFF          Disabled
//     MCLRE = ON           Enabled
//
//   T1 Oscillator Enable:
//     LPT1OSC = OFF        Disabled
//     LPT1OSC = ON         Enabled
//
//   PORTB A/D Enable:
//     PBADEN = OFF         PORTB<4:0> digital on Reset
//     PBADEN = ON          PORTB<4:0> analog on Reset
//
//   CCP2 MUX:
//     CCP2MX = PORTBE      Multiplexed with RB3
//     CCP2MX = PORTC       Multiplexed with RC1
//
//   Stack Overflow Reset:
//     STVREN = OFF         Disabled
//     STVREN = ON          Enabled
//
//   Low Voltage ICSP:
//     LVP = OFF            Disabled
//     LVP = ON             Enabled
//
//   XINST Enable:
//     XINST = OFF          Disabled
//     XINST = ON           Enabled
//
//   Background Debugger Enable:
//     DEBUG = ON           Enabled
//     DEBUG = OFF          Disabled
//
//   Code Protection Block 0:
//     CP0 = ON             Enabled
//     CP0 = OFF            Disabled
//
//   Code Protection Block 1:
//    CP1 = ON             Enabled
//     CP1 = OFF            Disabled
//
//   Code Protection Block 2:
//     CP2 = ON             Enabled
//     CP2 = OFF            Disabled
//
//  Boot Block Code Protection:
//     CPB = ON             Enabled
//     CPB = OFF            Disabled
//
//   Data EEPROM Code Protection:
//     CPD = ON             Enabled
//     CPD = OFF            Disabled
//
//   Write Protection Block 0:
//     WRT0 = ON            Enabled
//     WRT0 = OFF           Disabled
//
//   Write Protection Block 1:
//     WRT1 = ON            Enabled
//     WRT1 = OFF           Disabled
//
//   Write Protection Block 2:
//     WRT2 = ON            Enabled
//     WRT2 = OFF           Disabled
//
//   Boot Block Write Protection:
//     WRTB = ON            Enabled
//     WRTB = OFF           Disabled
//
//   Configuration Register Write Protection:
//     WRTC = ON            Enabled
//     WRTC = OFF           Disabled
//
//   Data EEPROM Write Protection:
//     WRTD = ON            Enabled
//     WRTD = OFF           Disabled
//
//   Table Read Protection Block 0:
//     EBTR0 = ON           Enabled
//     EBTR0 = OFF          Disabled
//
//   Table Read Protection Block 1:
//     EBTR1 = ON           Enabled
//     EBTR1 = OFF          Disabled
//
//   Table Read Protection Block 2:
//     EBTR2 = ON           Enabled
//     EBTR2 = OFF          Disabled
//
//   Boot Block Table Read Protection:
//     EBTRB = ON           Enabled
//     EBTRB = OFF          Disabled
//

