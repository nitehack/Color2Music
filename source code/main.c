#include "main.h"
#include "sound.c"
#include "color.c"

//COLOR2MUSIC
//Autor: Nicolás Guerrero García (Nitehack)
//Resumen: Transforma un sonido físico a una nota músical, pudiendo así generar musica con diferentes objetos de diferentes colores.



//ANOTACIONES:
//Hay que poner el máximo de frecuencia para medir tiempos muy cortos y el minimo de
// rango para que el error para medir la frecuencia con el metodo de medir
// el periodo sea minimo ya que falla solo cuando son muy altas las frecuencias
//Preescaler de 4 con minimo de 7 Hz
// Si se desborda el timer poner una interrupcion y poner de frecuencia 0Hz
//Poner reloj de 20 MHz externo
//Al final el reloj va a ser el de 8 MHz interno
// Se puede poner un umbral para considerar colo o no

//Informacion
//-----------
// Con 12 KHz
//Maxima frecuencia para R: 13157 Hz 
//Maxima frecuencia para B: 13157 Hz (En hexadecimal 3365 Hz)
// Maxima frecuencia para G:  11111 Hz (En hexadecimal 2B67)
//MinimaR:169 Hz (A9)
// Minima G: 135 Hz (87)
//Minima B: 150 Hz (96)

//Que hay que hacer:
//------------------
//Corregir error de 0 y de ganancia para los 3 filtros
//Posibles problemas!!: 
//----------------------
//Que el contador del timer se desborde por que pase demasiado tiempo , es decir cuando sean bajas frecuencias
//
//Sonido
//------
//Espectro de frecuencias de sonido: 30 Hz a 15 KHz
//PeridoPWM= (periodo_TMR2+1)·4·Tosc· prescaler_TMR2
//Hay que poner el preescaler el mas alto (16) ya que buscamos el myor rango entre 30 y 15 KHz, y para conseguir
//He usado desde 523 Hz hasta 980 Hz, y se puede usar un total de 12 notas


//Para corregir el offset del sensor

//Capturar flanco-> contar tiempo-> Capturar flanco-> Parar tiempo-> Leer tiempo-> Cambiar a frecuencia


void main()
{
   int16 frecuencia_R=0;
   int16 frecuencia_G=0;
   int16 frecuencia_B=0;
   int8 dato=0;
   int8 maximo=0;
   int8 direccion=0;

//Configuracion
//!   ext_int_edge(0,L_TO_H); //Flanco ascendente
//!   enable_interrupts(INT_EXT);
//!   clear_interrupt(INT_EXT); //Borramos el flag
//!   enable_interrupts(GLOBAL);
//!   setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);//para llegar a medir frecuencias bajas
//!
//
   
   init_tcs();
   init_sound();
   
   while(true){
      leer_frecuencia_color(&frecuencia_R,&frecuencia_G, &frecuencia_B);
      frecuencia_R=frecuencia_R-offset_r;
      frecuencia_G=frecuencia_G-offset_g;
      frecuencia_B=frecuencia_B-offset_b;
      
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
            genera_sonido(DOn);
            break;
         case 2:
            output_low(PIN_A0);
            output_low(PIN_A2);
            output_high(PIN_A1);
            genera_sonido(REn);
            break;
            
         case 3:
            output_low(PIN_A0);
            output_low(PIN_A1);
            output_high(PIN_A2);
            genera_sonido(MIn);
            break;
         default:
            output_low(PIN_A0);
            output_low(PIN_A1);
            output_low(PIN_A2);
            genera_sonido(FAn);
            break;
      }
      if (direccion>=240){
         direccion=0;
      }
   }
   
}
