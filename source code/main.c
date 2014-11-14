#include "C:\Nico\Teleco 4�\TCI\Codigo\main.h"
#define S0 PIN_B1
#define S1 PIN_B2
#define S2 PIN_B3
#define S3 PIN_B4
#define OE PIN_B5

//Hay que poner el m�ximo de frecuencia para medir tiempos muy cortos y el minimo de
// rango para que el error para medir la frecuencia con el metodo de medir
// el periodo sea minimo ya que falla solo cuando son muy altas las frecuencias
//Preescaler de 4 con minimo de 7 Hz
// Si se desborda el timer poner una interrupcion y poner de frecuencia 0Hz
//Poner reloj de 20 MHz externo

//Posibles problemas!!: Que el contador del timer se desborde por que pase demasiado tiempo , es decir cuando sean bajas frecuencias
int16 tiempo=0;

#INT_EXT
void llega_pulso(void) {
   tiempo=get_timer1(); //
   set_timer1(0); //reinicia para nuevo pulso
}

int16 calcula_frecuencia(){
   int16 tiempo_cal;
   int16 frecuencia;
   tiempo_cal=tiempo*2; //Es 4ciclos*T*4 del preescaler*ELm timer
   frecuencia=1000000.0/tiempo_cal; //esto es porque 1/(2N*micro)=(10^6)/2N
   return frecuencia;
}


void leer_frecuencia_color(int16 *frecuencia_R, int16 *frecuencia_G, int16 *frecuencia_B){
   output_high(OE);
   output_low(S2);
   output_low(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_R=calcula_frecuencia();
   
   //Azul
   output_high(OE);
   output_low(S2);
   output_high(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_B=calcula_frecuencia();
   
   //Verde
   output_high(OE);
   output_high(S2);
   output_high(S3);
   
   delay_ms(1);
   output_low(OE);
   delay_ms(10);
   *frecuencia_G=calcula_frecuencia();
}
//Capturar flanco-> contar tiempo-> Capturar flanco-> Parar tiempo-> Leer tiempo-> Cambiar a frecuencia


void main()
{
   int16 frecuencia_R=0;
   int16 frecuencia_G=0;
   int16 frecuencia_B=0;
   int32 tiempo_cal=0;
   int8 dato=0;
   int8 maximo=0;
   int8 direccion=0;


   ext_int_edge(0,L_TO_H); //Flanco ascendente
   enable_interrupts(INT_EXT);
   clear_interrupt(INT_EXT); //Borramos el flag
   enable_interrupts(GLOBAL);
   // TODO: USER CODE!!
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);//para llegar a medir frecuencias bajas
   
   output_low(S0);// Para seleccionar el rango de 12 KHz
   output_high(S1);
   while(true){
      leer_frecuencia_color(&frecuencia_R,&frecuencia_G, &frecuencia_B);
      dato=(int8)(frecuencia_R>>8);
      write_eeprom(direccion, dato);
      dato=(int8)(frecuencia_R);
      write_eeprom(direccion+1, dato);
      
      dato=(int8)(frecuencia_G>>8);
      write_eeprom(direccion+2, dato);
      dato=(int8)(frecuencia_G);
      write_eeprom(direccion+3, dato);
      
      dato=(int8)(frecuencia_B>>8);
      write_eeprom(direccion+4, dato);
      dato=(int8)(frecuencia_B);
      write_eeprom(direccion+5, dato);
      
      direccion=direccion+7;
      
      if((frecuencia_R<288)&&(frecuencia_G<208)&&(frecuencia_B<208)){
         maximo=0;
      }
      else{
         if(frecuencia_R>frecuencia_G){
            if(frecuencia_R>frecuencia_B){
               maximo=1;
            }
            else{
               maximo=3;
            }
         }
         else{
            if(frecuencia_G>frecuencia_B){
               maximo=2;
            }
            else{
               maximo=3;
            }
         }
      }
      switch(maximo){
         case 1:
            output_low(PIN_A1);
            output_low(PIN_A2);
            output_high(PIN_A0);
            break;
         case 2:
            output_low(PIN_A0);
            output_low(PIN_A2);
            output_high(PIN_A1);
            break;
            
         case 3:
            output_low(PIN_A0);
            output_low(PIN_A1);
            output_high(PIN_A2);
            break;
         default:
            output_low(PIN_A0);
            output_low(PIN_A1);
            output_low(PIN_A2);
            break;
      }
      if (direccion>=240){
         direccion=0;
      }
   }
   
}