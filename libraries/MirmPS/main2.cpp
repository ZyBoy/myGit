#include <Arduino.h>
#include "MirmPS.h"

ISR(TIMER2_OVF_vect,ISR_NAKED)
{
saveContext();				// Сохраняем контекст.

if ((SPH*0x100+SPL)<0x420) 		// Очень глупый способ 
					// вычисления активного потока.

{
  copyStackPointer(SPstore[1]);		
  loadStackPointer(SPstore[0]);
}					// Сохраняем один SP
else					
{					// И загружаем другой.
    copyStackPointer(SPstore[0]);
  loadStackPointer(SPstore[1]);
}

loadContext();				//Загружаем новый контекст.
asm("reti");				//Возврат из прерывания.
}

volatile void programm1 (void);
volatile void programm2 (void);


volatile SPstore_t SPstore[2];		// Здесь храняться указатели стека
					// сохраненных потоков.
volatile int Taskcount=0;		// Это счетчик выходов из ветвителя. 

void branching(void)__attribute__((always_inline));
void branching_2 (void)__attribute__((naked,noinline));

void branching(void)
{	setStackPointer(0x04,0xFF);	// установка SP в RAMEND
	branching_2();			// точка вызова процедуры 
					// копирования.

//В эту точку возвращаются новые потоки. 

//векторы выхода потоков из ветвителя:
	if (Taskcount==0) {Taskcount++;goto *programm1;}
	if (Taskcount==1) {Taskcount++;goto *programm2;}
}

void branching_2 (void)
{	saveContext();				//Сохраняем
	SPstore[1].i=copyContext(0x040A);	//Копируем
	loadContext();				//Загружаем
		
//т.к. функция naked, нужно явно объявить возврат
	asm("ret");				//Возвращаемся.		
}




void loop1(void);
void loop2(void);          //Функции реализованы в скетче.
void setup(void);              

 int main (void)__attribute__((__noreturn__));

int main(void)                      //Точка входа программы.
{
	init();     //Настройка ядра Ардуино. В основном таймеры.
	cli();      //init() разрешает прерывания. но 
                    //нам они пока не нужны
	branching();   
        return 0;   //Сюда программа не попадёт.
}

volatile void programm1(void){         //А это два наших потока.
	setup(); sei();     //Первый поток вызывает также cодержит      
	for (;;) {loop1();  //функцию setup.И разрешает прерывания.
	}
}                                       //За их вызов ответственна

volatile void programm2 (void){
for (;;) {loop2();}                      //функция branching()
}
