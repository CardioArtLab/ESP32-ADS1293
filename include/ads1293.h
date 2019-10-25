#ifndef __ADS_1293_H
#define __ADS_1293_H

#include <Arduino.h>

#define ADS1293_CSB_PIN  4
#define ADS1293_DRDY_PIN 32

#define CONFIG          0x00
#define FLEX_CH1_CN     0x01
#define FLEX_CH2_CN     0x02
#define FLEX_CH3_CN     0x03
#define FLEX_PACE_CN    0x04
#define FLEX_VBAT_CN    0x05

#define LOD_CN          0x06
#define LOD_EN          0x07
#define LOD_CURRENT     0x08
#define LOD_AC_CN       0x09

#define CMDET_EN        0x0a
#define CMDET_CN        0x0b
#define RLD_CN          0x0c

#define WILSON_EN1      0x0d
#define WILSON_EN2      0x0e
#define WILSON_EN3      0x0f
#define WILSON_CN       0x10

#define REF_CN          0x11
#define OSC_CN          0x12

#define AFE_RES         0x13
#define AFE_SHDN_CN     0x14
#define AFE_FAULT_CN    0x15
#define AFE_PACE_CN     0x17

#define ERROR_LOD       0x18
#define ERROR_STATUS    0x19
#define ERROR_RANGE1    0x1a
#define ERROR_RANGE2    0x1b
#define ERROR_RANGE3    0x1c
#define ERROR_SYNC      0x1d
#define ERROR_MISC      0x1e

#define DIGO_STRENGTH   0x1f
#define R2_RATE         0x21
#define R3_RATE_CH1     0x22
#define R3_RATE_CH2     0x23
#define R3_RATE_CH3     0x24
#define R1_RATE         0x25
#define DIS_EFILTER     0x26
#define DRDYB_SRC       0x27
#define SYNCB_CN        0x28
#define MASK_DRDYB      0x29
#define MASK_ERB        0x2a
#define ALARM_FILTER    0x2e
#define CH_CNFG         0x2f
#define DATA_STATUS     0x30
#define DATA_CH1_PACE   0x31
#define DATA_CH2_PACE   0x33
#define DATA_CH3_PACE   0x35
#define DATA_CH1_ECG    0x37
#define DATA_CH2_ECG    0x3a
#define DATA_CH3_ECG    0x3d
#define REVID           0x40
#define DATA_LOOP       0x50

class ads1293
{
  public:
    static void spi_Init(uint8_t sck = -1, uint8_t miso = -1, uint8_t mosi = -1, uint8_t ss = -1);
    static void Init(bool* is_init);
    static void Test_Mode(bool* is_init);
	  static void Reg_Write (unsigned char READ_WRITE_ADDRESS, unsigned char DATA);
	  static byte Reg_Read (unsigned char READ_WRITE_ADDRESS);
    static void Read_Data_Stream(byte* data, int length = 6);
    static void Disable_Start(void);
    static void Enable_Start(void);
};

#endif