/*
Los pines que se indican en los "defines" son del sensor TCS3200 yl-64
*/
#include "frequency.c"

#IFNDEF S0
#define S0 PIN_B1
#define S1 PIN_B2
#define S2 PIN_B6
#define S3 PIN_B4
#define OE PIN_B5
#ENDIF

float nivel=1.53;
int8 offset_r=167;//167
int8 offset_g=133;//133
int8 offset_b=149;//149
float gan_r=0.654678;//1
float gan_g=0.812162;//1.22408
float gan_b=0.739696;//1.08115

//Habria que hacer una funci�n "init_tcs" mas general para poder seleccionar otros rangos
void init_tcs(){
   init_freq();
   output_low(S0);// Para seleccionar el rango de 12 KHz
   output_high(S1);
}

void leer_frecuencia_color(int16 *frecuencia_R, int16 *frecuencia_G, int16 *frecuencia_B, int16 *frecuencia_C){
   float f_R_inter;
   float f_G_inter;
   float f_B_inter;
   float f_C_inter;
   
   output_high(OE);
   output_low(S2);
   output_low(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   f_R_inter=calcula_frecuencia()-offset_r;
   f_R_inter=f_R_inter*gan_r;
   
   if(f_R_inter<0){
      *frecuencia_R=0;   
   }
   else{
      *frecuencia_R=(int16)f_R_inter;
   }
   
   
   //Azul
   output_high(OE);
   output_low(S2);
   output_high(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   f_B_inter=calcula_frecuencia()-offset_b;
   f_B_inter=f_B_inter*gan_b;
   if(f_B_inter<0){
      *frecuencia_B=0;   
   }
   else{
      *frecuencia_B=(int16)f_B_inter;
   }
   
   //Verde
   output_high(OE);
   output_high(S2);
   output_high(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   f_G_inter=calcula_frecuencia()-offset_g;
   f_G_inter=f_G_inter*gan_g;
   if(f_G_inter<0){
      *frecuencia_G=0;   
   }
   else{
      *frecuencia_G=(int16)f_G_inter;
   }
   
   //Claro
   output_high(OE);
   output_high(S2);
   output_low(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_C=calcula_frecuencia();
   

}

int8 busca_maximo(int16 vector[], int8 elementos){
   int8 i;
   int8 indice;
   int16 mayor;
   mayor=vector[0];
   indice=0;
   for(i=0;i<elementos;i++){
      if(vector[i]>mayor){
         mayor=vector[i];
         indice=i;
      }
   }
   return indice;
}

int8 busca_maximo2(int16 vector[], int8 elementos){
   int8 i;
   int8 indice;
   int8 max;
   int16 mayor;
   max=busca_maximo(vector,elementos);
   mayor=0;
   indice=0;
   for(i=0;i<elementos;i++){
      if(i!=max){
         if(vector[i]>mayor){
            mayor=vector[i];
            indice=i;
         }
      }
   }
   return indice;
}

//Otro metodo
int8 detectar_color(int16 fred, int16 fblue,int16 fgreen, int16 fclear){
   int16 rgb[3]={0,0,0};   
   int8 rgb2[3]={0,0,0};
   int8 max1,max2;
   int8 color=8;
   int16 direccion;
   
   rgb[0]=fred;
   rgb[1]=fgreen;
   rgb[2]=fblue;
   max1=busca_maximo(rgb,3);
   if(fclear>10000){
      color=7;
   }
   else if(fclear<3000){
      color=0;
   }
   else{
      if(fclear<6600){
         
         switch(max1){
            case 0:
               color=4;
               break;
            case 1:
               color=2;
               break;
            case 2:
               color=1;
               break;
            default:
               color=8;
               break;
         }
      }
      else{
         max2=busca_maximo2(rgb,3);
         rgb2[max1]=1;
         rgb2[max2]=1;
         color=(rgb2[0]<<2)+(rgb2[1]<<1)+rgb2[2];
      }
      }
   return color;

}

//Buscar el mas chico y dividirlo entre el m�s chico. Los que esten por encima de un nivel son "1" y los que sean menores son "0"
//!
//!int8 detectar_color(int16 fred, int16 fblue,int16 fgreen){
//!   int8 color;
//!   int16 mini=1;
//!   float nred;
//!   float nblue;
//!   float ngreen;
//!   int8 bitred;
//!   int8 bitblue;
//!   int8 bitgreen;
//!   
//!   if(fred<fblue){
//!      if(fred<fgreen){
//!         mini=fred;
//!      }
//!      else{
//!         mini=fgreen;
//!      }
//!   }
//!   else{
//!      if(fblue<fgreen){
//!         mini=fblue;
//!      }
//!      else{
//!         mini=fgreen;
//!      }
//!   }
//!   nred=(float)fred/(float)mini;
//!   nblue=(float)fblue/(float)mini;
//!   ngreen=(float)fgreen/(float)mini;
//!   
//!   if(nred>nivel){
//!      bitred=1;
//!   }
//!   else{
//!      bitred=0;
//!   }
//!   if(nblue>nivel){
//!      bitblue=1;
//!   }
//!   else{
//!      bitblue=0;
//!   }
//!   if(ngreen>nivel){
//!      bitgreen=1;
//!   }
//!   else{
//!      bitgreen=0;
//!   }
//!   color=(bitred<<2)+(bitgreen<<1)+bitblue;
//!   
//!   if(color==0){
//!      if(fred>1000){
//!         color=7;
//!      }
//!   }
//!   else if((fred<1000)&&(fblue<1000)&&(fgreen<1000)){
//!      color=8;
//!   }
//!   
//!   return color;
//!}
//!
//!
