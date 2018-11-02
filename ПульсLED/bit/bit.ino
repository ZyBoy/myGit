/*
 *                            attiny 13
 *                            ╔═══╗
 * (PCINT5/RESET/ADC0/dW) PB5 ╣1   8╠ VCC
 *     (PCINT3/CLKI/ADC3) PB3 ╣2   7╠ PB2 (SCK/ADC1/T0/PCINT2)
 *          (PCINT4/ADC2) PB4 ╣3   6╠ PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
 *                        GND ╣4   5╠ PB0 (MOSI/AIN0/OC0A/PCINT0)
 *                            ╚═══╝
 * 
 */

//#define F_CPU 1200000UL
#define F_CPU 600000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED0 PB0 // OC0A
#define LED1 PB1 // OC0B
#define BUTT PB4 // PCINT4

int vLedSync = 1; // режим: 1 - синхронное мигание диодов
  
// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
  _delay_ms (50); // антидребезг (использовать задержки в прерываниях некошерно, но пока и так сойдёт)
  if ( (PINB & (1<<BUTT)) == 0 ) // если нажата одна из кнопок
  {
    if (vLedSync == 1)
    { 
      TCCR0A = 0xB3;
      vLedSync = 0;
    }
    else
    { 
      TCCR0A = 0xF3;
      vLedSync = 1;
    }

    while ( (PINB & (1<<BUTT))== 0 ) {
    } // ждём отпускания кнопки
  }
}

int main(void)
{
  // Светидиоды:
  DDRB |= (1 << LED0)|(1 << LED1); // выходы = 1
  PORTB &= ~((1 << LED0)|(1 << LED1)); // по умолчанию отключены = 0
  // Таймер для ШИМ:
  //  TCCR0A = 0xB3; // режим ШИМ, неинверсный сигнал на выходе OC0A, инверсный - на выходе OC0B - работа в противофазе
  TCCR0A = 0xF3; // режим ШИМ, инверсный сигнал на выходах - работа синхронная 
  TCCR0B = 0x02; // предделитель тактовой частоты CLK/8
  TCNT0=0; // начальное значение счётчика
  OCR0A=0; // регистр совпадения A
  OCR0B=0; // регистр совпадения B

  // Пин кнопки
  DDRB &= ~(1<<BUTT); // вход
  PORTB |= (1<<BUTT); // подтянут
  // Настройка прерываний
  GIMSK |= (1<<PCIE); // Разрешаем внешние прерывания PCINT0.
  PCMSK |= (1<<BUTT); // Разрешаем по маске прерывания на ногах кнопки
  sei(); // Разрешаем прерывания глобально: SREG |= (1<<SREG_I)

  while(1)
  {
    BitUp();
    BitDwn();
    _delay_ms(170); // Пауза 
  }
}

int BitUp(){
  do // Затухание
  {
    OCR0A--;
    OCR0B = OCR0A;
    _delay_ms(3);
  }
  while(OCR0A!=0);
}

int BitDwn(){
  do // Нарастание яркости
  {
    OCR0A++;
    OCR0B = OCR0A;
    _delay_ms(3);
  }
  while(OCR0A!=255);
}




