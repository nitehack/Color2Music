/*
Los pines que se indican en los "defines" son del sensor TCS3200 yl-64
*/
#include "frequency.c"

#define S0 PIN_B1
#define S1 PIN_B2
#define S2 PIN_B6
#define S3 PIN_B4
#define OE PIN_B5

int8 offset_r=34;
int8 offset_g=0;
int8 offset_b=15;

//Habria que hacer una funci�n "init_tcs" mas general para poder seleccionar otros rangos
void init_tcs(){
   init_freq();
   output_low(S0);// Para seleccionar el rango de 12 KHz
   output_high(S1);
}

void leer_frecuencia_color(int16 *frecuencia_R, int16 *frecuencia_G, int16 *frecuencia_B){
   output_high(OE);
   output_low(S2);
   output_low(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_R=calcula_frecuencia()-offset_r;
   
   //Azul
   output_high(OE);
   output_low(S2);
   output_high(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_B=calcula_frecuencia()-offset_b;
   
   //Verde
   output_high(OE);
   output_high(S2);
   output_high(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_G=calcula_frecuencia()-offset_g;
}
