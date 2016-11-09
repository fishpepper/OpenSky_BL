#ifndef __CC25XX_H__
#define __CC25XX_H__
#include <stdint.h>
#include <cc2510fx.h>

#define PERCFG_U0CFG (1<<0)
#define PERCFG_U1CFG (1<<1)
#define PERCFG_T4CFG (1<<4)
#define PERCFG_T3CFG (1<<5)
#define PERCFG_T1CFG (1<<6)

#define IEN0_RFTXRXIE (1<<0)
#define IEN0_ADCIE (1<<1)
#define IEN0_URX0IE (1<<2)
#define IEN0_URX1IE (1<<3)
#define IEN0_ENCIE (1<<4)
#define IEN0_STIE (1<<5)
#define IEN0_EA (1<<7)

//bit 7 - unused
//bit 6 - unused
#define PICTL_P2IEN  (1<<5)
#define PICTL_P0IENH (1<<4)
#define PICTL_P0IENL (1<<3)
#define PICTL_P2ICON (1<<2)
#define PICTL_P1ICON (1<<1)
#define PICTL_P0ICON (1<<0)

#define IEN1_P0IE  (1<<5)
#define IEN1_T4IE  (1<<4)
#define IEN1_T3IE  (1<<3)
#define IEN1_T2IE  (1<<2)
#define IEN1_T1IE  (1<<1)
#define IEN1_DMAIE (1<<0)

#define IEN2_RFIE (1<<0)
#define IEN2_P2IE (1<<1)
#define IEN2_UTX0IE (1<<2)
#define IEN2_UTX1IE (1<<3)
#define IEN2_P1IE   (1<<4)
#define IEN2_WDTIE  (1<<5)

#define U0GCR_ORDER (1<<5)
#define U0GCR_CPHA  (1<<6)
#define U0GCR_CPOL  (1<<7)
#define U0CSR_TX_BYTE (1<<1)

#define U1GCR_ORDER (1<<5)
#define U1GCR_CPHA  (1<<6)
#define U1GCR_CPOL  (1<<7)
#define U1CSR_TX_BYTE (1<<1)

#define CLKCON_TICKSPD_001 (0b00001000)
#define CLKCON_TICKSPD_010 (0b00010000)
#define CLKCON_TICKSPD_011 (0b00011000)
#define CLKCON_TICKSPD_100 (0b00100000)
#define CLKCON_TICKSPD_101 (0b00101000)
#define CLKCON_TICKSPD_110 (0b00110000)
#define CLKCON_TICKSPD_111 (0b00111000)
#define CLKCON_OSC32K (1<<7)

#define ADCCON2_SREF_INT  (0b00<<6)
#define ADCCON2_SREF_EXT  (0b01<<6)
#define ADCCON2_SREF_AVDD (0b10<<6)
#define ADCCON2_SREF_EXTDIFF  (0b11<<6)
#define ADCCON2_SDIV_7BIT   (0b00<<4)
#define ADCCON2_SDIV_9BIT   (0b01<<4)
#define ADCCON2_SDIV_10BIT  (0b10<<4)
#define ADCCON2_SDIV_12BIT  (0b11<<4)
#define ADCCON2_SCH_AIN0   (0b0000<<0)
#define ADCCON2_SCH_AIN1   (0b0001<<0)
#define ADCCON2_SCH_AIN2   (0b0010<<0)
#define ADCCON2_SCH_AIN3   (0b0011<<0)
#define ADCCON2_SCH_AIN4   (0b0100<<0)
#define ADCCON2_SCH_AIN5   (0b0101<<0)
#define ADCCON2_SCH_AIN6   (0b0110<<0)
#define ADCCON2_SCH_AIN7   (0b0111<<0)
#define ADCCON2_SCH_AIN0AIN1   (0b1000<<0)
#define ADCCON2_SCH_AIN2AIN3   (0b1001<<0)
#define ADCCON2_SCH_AIN4AIN5   (0b1010<<0)
#define ADCCON2_SCH_AIN6AIN7   (0b1011<<0)
#define ADCCON2_SCH_GND        (0b1100<<0)
#define ADCCON2_SCH_POSVREF    (0b1101<<0)
#define ADCCON2_SCH_TEMP       (0b1110<<0)
#define ADCCON2_SCH_VDD3       (0b1111<<0)

#define ADCCON1_ST               (1<<6)
#define ADCCON1_STSEL_FULL_SPEED (0b01<<4)

#define WDCTL_EN (1<<3)
#define WDCTL_MODE (1<<2)
#define WDCTL_INT (0b11)
#define WDCTL_INT_1S (0b00)

#define FCTL_BUSY (1<<7)
#define FCTL_SWBUSY (1<<6)
#define FCTL_WRITE (1<<1)
#define FCTL_ERASE (1<<0)

#endif // __CC25XX_H__
