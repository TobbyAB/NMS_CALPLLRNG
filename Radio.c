
#include "ax5043.h"
#include "spi.h"
#include "ax5043def.h"


// TX: fcarrier= 40.680MHz dev=  4.250kHz br=  0.595kBit/s pwr= 10.0dBm
// RX: fcarrier= 40.680MHz bw=  7.893kHz br=  0.595kBit/s
//SpiWriteAutoAddressRegister(REG_AX5043_
void AX5043_set_registers(void)
{




	SpiWriteAutoAddressRegister(REG_AX5043_MODULATION           , 0x08);
	SpiWriteAutoAddressRegister(REG_AX5043_ENCODING             , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FRAMING              , 0x06);
	SpiWriteAutoAddressRegister(REG_AX5043_PINFUNCSYSCLK        , 0x01);
/*	SpiWriteAutoAddressRegister(REG_AX5043_PINFUNCDCLK          , 0x01);
	SpiWriteAutoAddressRegister(REG_AX5043_PINFUNCDATA          , 0x01);
	SpiWriteAutoAddressRegister(REG_AX5043_PINFUNCANTSEL        , 0x01);
	SpiWriteAutoAddressRegister(REG_AX5043_PINFUNCPWRAMP        , 0x07);
	SpiWriteAutoAddressRegister(REG_AX5043_WAKEUPXOEARLY        , 0x01);
	SpiWriteAutoAddressRegister(REG_AX5043_IFFREQ1              , 0x01);
	SpiWriteAutoAddressRegister(REG_AX5043_IFFREQ0              , 0x9A);
	SpiWriteAutoAddressRegister(REG_AX5043_DECIMATION           , 0x19);
	SpiWriteAutoAddressRegister(REG_AX5043_RXDATARATE2          , 0x02);
	SpiWriteAutoAddressRegister(REG_AX5043_RXDATARATE1          , 0x19);
	SpiWriteAutoAddressRegister(REG_AX5043_RXDATARATE0          , 0xD0);
	SpiWriteAutoAddressRegister(REG_AX5043_MAXDROFFSET2         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_MAXDROFFSET1         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_MAXDROFFSET0         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_MAXRFOFFSET2         , 0x80);
	SpiWriteAutoAddressRegister(REG_AX5043_MAXRFOFFSET1         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_MAXRFOFFSET0         , 0x55);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDMAX1             , 0x0D);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDMAX0             , 0x97);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDMIN1             , 0xF2);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDMIN0             , 0x69);
	SpiWriteAutoAddressRegister(REG_AX5043_AMPLFILTER           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_RXPARAMSETS          , 0xF4);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCGAIN0             , 0xE9);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCTARGET0           , 0x84);
	SpiWriteAutoAddressRegister(REG_AX5043_TIMEGAIN0            , 0x8C);
	SpiWriteAutoAddressRegister(REG_AX5043_DRGAIN0              , 0x86);
	SpiWriteAutoAddressRegister(REG_AX5043_PHASEGAIN0           , 0xC3);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINA0      , 0x0F);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINB0      , 0x1F);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINC0      , 0x0B);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAIND0      , 0x0B);
	SpiWriteAutoAddressRegister(REG_AX5043_AMPLITUDEGAIN0       , 0x06);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQDEV10            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQDEV00            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_BBOFFSRES0           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCGAIN1             , 0xE9);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCTARGET1           , 0x84);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCAHYST1            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCMINMAX1           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TIMEGAIN1            , 0x8A);
	SpiWriteAutoAddressRegister(REG_AX5043_DRGAIN1              , 0x85);
	SpiWriteAutoAddressRegister(REG_AX5043_PHASEGAIN1           , 0xC3);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINA1      , 0x0F);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINB1      , 0x1F);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINC1      , 0x0B);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAIND1      , 0x0B);
	SpiWriteAutoAddressRegister(REG_AX5043_AMPLITUDEGAIN1       , 0x06);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQDEV11            , 0x05);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQDEV01            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FOURFSK1             , 0x16);
	SpiWriteAutoAddressRegister(REG_AX5043_BBOFFSRES1           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCGAIN3             , 0xFF);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCTARGET3           , 0x84);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCAHYST3            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_AGCMINMAX3           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TIMEGAIN3            , 0x89);
	SpiWriteAutoAddressRegister(REG_AX5043_DRGAIN3              , 0x84);
	SpiWriteAutoAddressRegister(REG_AX5043_PHASEGAIN3           , 0xC3);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINA3      , 0x0F);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINB3      , 0x1F);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAINC3      , 0x0D);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQUENCYGAIND3      , 0x0D);
	SpiWriteAutoAddressRegister(REG_AX5043_AMPLITUDEGAIN3       , 0x06);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQDEV13            , 0x05);
	SpiWriteAutoAddressRegister(REG_AX5043_FREQDEV03            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FOURFSK3             , 0x16);
	SpiWriteAutoAddressRegister(REG_AX5043_BBOFFSRES3           , 0x00);
*/        
        
/*	SpiWriteAutoAddressRegister(REG_AX5043_MODCFGF              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV2              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV1              , 0x11);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV0              , 0x68);
	SpiWriteAutoAddressRegister(REG_AX5043_MODCFGA              , 0x05);
	SpiWriteAutoAddressRegister(REG_AX5043_TXRATE2              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TXRATE1              , 0x02);
	SpiWriteAutoAddressRegister(REG_AX5043_TXRATE0              , 0x70);
	SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFB1         , 0x07);
	SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFB0         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_PLLVCOI              , 0x9E);
	SpiWriteAutoAddressRegister(REG_AX5043_PLLRNGCLK            , 0x03);
        */
        SpiWriteAutoAddressRegister(REG_AX5043_MODCFGF              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV2              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV1              , 0x0a);
	SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV0              , 0xb6);
	SpiWriteAutoAddressRegister(REG_AX5043_MODCFGA              , 0x06);
	SpiWriteAutoAddressRegister(REG_AX5043_TXRATE2              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TXRATE1              , 0x01);
	SpiWriteAutoAddressRegister(REG_AX5043_TXRATE0              , 0x80);
	SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFA1         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFA0         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFB1         , 0x09);//09
	SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFB0         , 0xD4);//D4
	SpiWriteAutoAddressRegister(REG_AX5043_PLLVCOI              , 0x80);
	SpiWriteAutoAddressRegister(REG_AX5043_PLLRNGCLK            , 0x04);
        
        SpiWriteAutoAddressRegister(REG_AX5043_PKTADDRCFG           , 0x80);
/*	SpiWriteAutoAddressRegister(REG_AX5043_BBTUNE               , 0x0F);
	SpiWriteAutoAddressRegister(REG_AX5043_BBOFFSCAP            , 0x77);
	SpiWriteAutoAddressRegister(REG_AX5043_PKTADDRCFG           , 0x80);
	SpiWriteAutoAddressRegister(REG_AX5043_PKTLENCFG            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_PKTLENOFFSET         , 0x12);
	SpiWriteAutoAddressRegister(REG_AX5043_PKTMAXLEN            , 0xC8);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH0PAT3           , 0x5A);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH0PAT2           , 0xAA);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH0PAT1           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH0PAT0           , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH0LEN            , 0x8F);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH0MAX            , 0x0F);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH1PAT1           , 0xAA);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH1PAT0           , 0xAA);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH1LEN            , 0x8A);
	SpiWriteAutoAddressRegister(REG_AX5043_MATCH1MAX            , 0x0A);
*/	SpiWriteAutoAddressRegister(REG_AX5043_TMGTXBOOST           , 0x3e);
	SpiWriteAutoAddressRegister(REG_AX5043_TMGTXSETTLE          , 0x31);
/*	SpiWriteAutoAddressRegister(REG_AX5043_TMGRXBOOST           , 0x32);
	SpiWriteAutoAddressRegister(REG_AX5043_TMGRXSETTLE          , 0x14);
	SpiWriteAutoAddressRegister(REG_AX5043_TMGRXOFFSACQ         , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_TMGRXCOARSEAGC       , 0x73);
	SpiWriteAutoAddressRegister(REG_AX5043_TMGRXRSSI            , 0x03);
	SpiWriteAutoAddressRegister(REG_AX5043_TMGRXPREAMBLE2       , 0x1A);
	SpiWriteAutoAddressRegister(REG_AX5043_RSSIABSTHR           , 0xE0);
	SpiWriteAutoAddressRegister(REG_AX5043_BGNDRSSITHR          , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_PKTCHUNKSIZE         , 0x0D);
	SpiWriteAutoAddressRegister(REG_AX5043_PKTACCEPTFLAGS       , 0x20);
	SpiWriteAutoAddressRegister(REG_AX5043_DACVALUE1            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_DACVALUE0            , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_DACCONFIG            , 0x00);
*/	SpiWriteAutoAddressRegister(REG_AX5043_REF                  , 0x03);
	SpiWriteAutoAddressRegister(REG_AX5043_XTALOSC              , 0x04);            //REG_AX5043_0xF10
	SpiWriteAutoAddressRegister(REG_AX5043_XTALAMPL             , 0x00);            //REG_AX5043_0xF10
/*	SpiWriteAutoAddressRegister(REG_AX5043_0xF1C                , 0x07);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF21                , 0x68);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF22                , 0xFF);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF23                , 0x84);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF26                , 0x98);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF34                , 0x28);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF35                , 0x10);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF44                , 0x25);
*/
        SpiWriteAutoAddressRegister(REG_AX5043_PLLLOOP              , 0x0A);
	SpiWriteAutoAddressRegister(REG_AX5043_PLLCPI               , 0x10);
	SpiWriteAutoAddressRegister(REG_AX5043_PLLVCODIV            , 0x35);
	SpiWriteAutoAddressRegister(REG_AX5043_XTALCAP              , 0x00);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF00                , 0x0F);
	SpiWriteAutoAddressRegister(REG_AX5043_0xF18                , 0x06);
        
        //uint32_t f = 0x28AE148;
        uint32_t f = 0x1908AA2;
        SpiWriteAutoAddressRegister(REG_AX5043_FREQA0,f & 0xff);
        SpiWriteAutoAddressRegister(REG_AX5043_FREQA1,f >> 8);
        SpiWriteAutoAddressRegister(REG_AX5043_FREQA2, f >> 16);
        SpiWriteAutoAddressRegister(REG_AX5043_FREQA3, f >> 24);
        
        
        
        
}








