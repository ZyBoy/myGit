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

#define F_CPU 600000UL
#include <avr/io.h>
#include <avr/wdt.h> // здесь организована работа с ватчдогом
#include <avr/sleep.h> // здесь описаны режимы сна
#include <avr/interrupt.h> // работа с прерываниями

#define LED0 PB0 // OC0A
#define LED1 PB1 // OC0B
#define BUTT PB4 // PCINT4


ISR (WDT_vect) {
  if ( (PINB & _BV(BUTT)) == 0 ) // если нажата одна из кнопок
  {
    PORTB ^= (_BV(LED0))|(_BV(LED1)); // переключаем светодиод
  }
  WDTCR |= _BV(WDTIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
}

int main() {

  // инициализация порта светодиода
  DDRB = (_BV(LED0))|(_BV(LED1)); // на этих пинах висят светодиоды
  PORTB = (_BV(LED0))|(_BV(LED1)); // зажгем этот мир

  // Пин кнопки
  DDRB &= ~_BV(BUTT); // вход
  PORTB |= _BV(BUTT); // подтянут

  //инициализация ватчдога
  wdt_reset(); // сбрасываем
  wdt_enable(WDTO_4S); // разрешаем ватчдог 1 сек
  WDTCR |= _BV(WDTIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
  sei(); // разрешаем прерывания

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
  while(1) {
    sleep_enable(); // разрешаем сон
    sleep_cpu(); // спать!
  }
}



