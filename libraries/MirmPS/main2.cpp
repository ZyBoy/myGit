#include <Arduino.h>
#include "MirmPS.h"

ISR(TIMER2_OVF_vect,ISR_NAKED)
{
saveContext();				// ��������� ��������.

if ((SPH*0x100+SPL)<0x420) 		// ����� ������ ������ 
					// ���������� ��������� ������.

{
  copyStackPointer(SPstore[1]);		
  loadStackPointer(SPstore[0]);
}					// ��������� ���� SP
else					
{					// � ��������� ������.
    copyStackPointer(SPstore[0]);
  loadStackPointer(SPstore[1]);
}

loadContext();				//��������� ����� ��������.
asm("reti");				//������� �� ����������.
}

volatile void programm1 (void);
volatile void programm2 (void);


volatile SPstore_t SPstore[2];		// ����� ��������� ��������� �����
					// ����������� �������.
volatile int Taskcount=0;		// ��� ������� ������� �� ���������. 

void branching(void)__attribute__((always_inline));
void branching_2 (void)__attribute__((naked,noinline));

void branching(void)
{	setStackPointer(0x04,0xFF);	// ��������� SP � RAMEND
	branching_2();			// ����� ������ ��������� 
					// �����������.

//� ��� ����� ������������ ����� ������. 

//������� ������ ������� �� ���������:
	if (Taskcount==0) {Taskcount++;goto *programm1;}
	if (Taskcount==1) {Taskcount++;goto *programm2;}
}

void branching_2 (void)
{	saveContext();				//���������
	SPstore[1].i=copyContext(0x040A);	//��������
	loadContext();				//���������
		
//�.�. ������� naked, ����� ���� �������� �������
	asm("ret");				//������������.		
}




void loop1(void);
void loop2(void);          //������� ����������� � ������.
void setup(void);              

 int main (void)__attribute__((__noreturn__));

int main(void)                      //����� ����� ���������.
{
	init();     //��������� ���� �������. � �������� �������.
	cli();      //init() ��������� ����������. �� 
                    //��� ��� ���� �� �����
	branching();   
        return 0;   //���� ��������� �� ������.
}

volatile void programm1(void){         //� ��� ��� ����� ������.
	setup(); sei();     //������ ����� �������� ����� c�������      
	for (;;) {loop1();  //������� setup.� ��������� ����������.
	}
}                                       //�� �� ����� ������������

volatile void programm2 (void){
for (;;) {loop2();}                      //������� branching()
}
