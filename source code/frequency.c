int16 tiempo=0;
//Esta librería mide la frecuencia de una señal que entr por RB0
#INT_EXT
void llega_pulso(void) {
   tiempo=get_timer1(); //
   set_timer1(0); //reinicia para nuevo pulso
}

void init_freq(){
   ext_int_edge(0,L_TO_H); //Flanco ascendente
   enable_interrupts(INT_EXT);
   clear_interrupt(INT_EXT); //Borramos el flag
   enable_interrupts(GLOBAL);
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);//para llegar a medir frecuencias bajas
}

int16 calcula_frecuencia(){
   int16 tiempo_cal;
   int16 frecuencia;
   tiempo_cal=tiempo*2; //Es 4ciclos*T*4 del preescaler*ELm timer
   frecuencia=1000000.0/tiempo_cal; //esto es porque 1/(2N*micro)=(10^6)/2N
   return frecuencia;
}


