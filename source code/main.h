#include <16F88.h>
#device adc=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES INTRC_IO                 //Internal RC Osc, no CLKOUT
#FUSES NOPUT                    //No Power Up Timer
#FUSES MCLR                     //Master Clear pin enabled
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOCPD                    //No EE protection
#FUSES NOWRT                    //Program memory not write protected
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOPROTECT                //Code not protected from reading
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES RESERVED                 //Used to set the reserved FUSE bits
#FUSES CCPB3                    // Ponemos este fuse para que el PWM salga por la patilla 9 ya que  por defecto está en la 6 y esa ya se usa

#use delay(clock=8000000)

