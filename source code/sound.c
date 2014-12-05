
#define SIn 126
#define LABn 133
#define LAn 141
#define SOLBn 150
#define SOLn 159
#define FABn 168
#define FAn 178
#define MIn 189
#define REBn 200
#define REn 212
#define DOBn 225
#define DOn 238

void genera_sonido(int8 sonido){
   int16 value;
   value=(4*((int16)sonido+1))/2;
   setup_timer_2(T2_DIV_BY_16, sonido ,4); 
   set_pwm1_duty(value); //se establece el ciclo de trabajo
}