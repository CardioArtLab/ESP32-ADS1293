#include "ads1293.h"
#include <SPI.h>

void ads1293::spi_Init(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss)
{
  pinMode(ADS1293_CSB_PIN, OUTPUT);
  pinMode(ADS1293_DRDY_PIN, INPUT_PULLUP);
  // start the SPI library:
  SPI.begin(sck, miso, mosi, ss);
  SPI.setBitOrder(MSBFIRST); 
  //CPOL = 0, CPHA = 0 SPI_MODE0
  //CPOL = 0, CPHA = 1 SPI_MODE1
  SPI.setDataMode(SPI_MODE0);
  // Selecting 2Mhz clock for SPI
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void ads1293::Init(bool* is_init)
{
    *is_init = false;
    Disable_Start();
    Reg_Write(FLEX_CH1_CN, 0b00010001); // INP to IN2 and INN to IN1
    //Reg_Write(FLEX_CH1_CN, 0b11000000); // TEST MODE: Zero
    //Reg_Write(FLEX_CH2_CN, 0b00011001); // INP to IN3 and INN to IN1
    //Reg_Write(FLEX_CH2_CN, 0b11000000); // TEST MODE: Zero
    //Reg_Write(FLEX_CH3_CN, 0b11000000); // Zero Test ADCout = 1/2ADCmax
    Reg_Write(FLEX_PACE_CN, 0);
    Reg_Write(FLEX_VBAT_CN, 0);
    Reg_Write(LOD_CN, 0b0000100); // Disable Lead-Off detection
    Reg_Write(LOD_EN, 0);
    Reg_Write(CMDET_EN, 0b00000111); // enable common-mode-detector from IN1,IN2,IN3
    Reg_Write(RLD_CN, 0b00000011); // connect RLD to IN3
    Reg_Write(REF_CN, 0); // Turn on internal REF
    Reg_Write(OSC_CN, 0b100); // use external clock
    Reg_Write(AFE_RES, 0b00001000); // 204800Hz ADC
    Reg_Write(AFE_SHDN_CN, 0b110110); // Shut down ADC CH2, CH3
    Reg_Write(AFE_PACE_CN, 0b000); // turn off PACE
    Reg_Write(R1_RATE, 0b000); // standard DATA rate, R1 = 4
    Reg_Write(R2_RATE, 0b0010); // R2 = 5
    Reg_Write(R3_RATE_CH1, 0b100); // R3 = 8 
    Reg_Write(R3_RATE_CH2, 0b100); // R3 = 8, 1280 Hz, ADCMax = 0xC35000, Bandwidth: 260Hz
    Reg_Write(DRDYB_SRC, 0b001000); // connect DRDY to CH1
    Reg_Write(CH_CNFG, 0b00010000); // loop read-back mode, for CH1
    Enable_Start(); 
    *is_init = true;
}

void ads1293::Reg_Write (unsigned char READ_WRITE_ADDRESS, unsigned char DATA)
{
    digitalWrite(ADS1293_CSB_PIN, LOW);
    SPI.transfer(READ_WRITE_ADDRESS & 0x7f);
    SPI.transfer(DATA);
    digitalWrite(ADS1293_CSB_PIN, HIGH);
}

byte ads1293::Reg_Read (unsigned char READ_WRITE_ADDRESS)
{
    byte tmp = 0;
    digitalWrite(ADS1293_CSB_PIN, LOW);
    SPI.transfer(READ_WRITE_ADDRESS | 0x80);
    tmp = SPI.transfer(0xff);
    digitalWrite(ADS1293_CSB_PIN, HIGH);
    return tmp;
}

void ads1293::Read_Data_Stream(byte* data, int length)
{
    digitalWrite(ADS1293_CSB_PIN, LOW);
    SPI.transfer(DATA_LOOP | 0x80);
    for (int i=0; i<length; i++) {
        data[i] = SPI.transfer(0xff);
    }
    digitalWrite(ADS1293_CSB_PIN, HIGH);
}

void ads1293::Disable_Start(void)
{
    Reg_Write(CONFIG, 0x00);
}

void ads1293::Enable_Start(void)
{
    Reg_Write(CONFIG, 0x01);
    delayMicroseconds(10);
}