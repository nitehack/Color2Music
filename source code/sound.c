

void genera_sonido(int8 frecuencia){
   int8 fos;
   int8 Pre=16;
   int32 PR2;
   float duty=0.5;
   int8 frecuencia;
   int8 value;
   PR2=fos/(frecuencia*4*Pre)-1; //se esteblece el PR" segun la frecuencia seleccionada
   value=duty*(4*(PR2+1));
   
   setup_timer_2(T2_DIV_BY_16, PR2_timer2 ,4); 
   set_pwm1_duty(value); //se establece el ciclo de trabajo
}
