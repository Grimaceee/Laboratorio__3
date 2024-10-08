#include <Arduino.h>

#define boton_mas 12
#define boton_menos 14
#define boton_clear 33
#define led_uno 16
#define led_dos 17
#define led_tres 18
#define led_cuatro 19
#define led_a 21
#define led_b 22
#define led_c 23
#define led_d 25
#define led_alerta 26
volatile int cuenta_manual = 0;
volatile int cuenta_auto = 0;
hw_timer_t * temporizador = NULL;

void actualizar_leds_manual() {
  digitalWrite(led_uno, (cuenta_manual & 0x01) ? HIGH : LOW);
  digitalWrite(led_dos, (cuenta_manual & 0x02) ? HIGH : LOW);
  digitalWrite(led_tres, (cuenta_manual & 0x04) ? HIGH : LOW);
  digitalWrite(led_cuatro, (cuenta_manual & 0x08) ? HIGH : LOW);
}

void actualizar_leds_auto() {
  digitalWrite(led_a, (cuenta_auto & 0x01) ? HIGH : LOW);
  digitalWrite(led_b, (cuenta_auto & 0x02) ? HIGH : LOW);
  digitalWrite(led_c, (cuenta_auto & 0x04) ? HIGH : LOW);
  digitalWrite(led_d, (cuenta_auto & 0x08) ? HIGH : LOW);
}

void IRAM_ATTR manejar_mas() {
  cuenta_manual = (cuenta_manual + 1) % 16; 
  actualizar_leds_manual();
}

void IRAM_ATTR manejar_menos() {
  cuenta_manual = (cuenta_manual - 1 + 16) % 16;
  actualizar_leds_manual();
}

void IRAM_ATTR manejar_tiempo() {
  cuenta_auto = (cuenta_auto + 1) % 16;
  actualizar_leds_auto();
  if (cuenta_auto == cuenta_manual) {
    digitalWrite(led_alerta, !digitalRead(led_alerta)); 
    cuenta_auto = 0; 
  }
}

void IRAM_ATTR manejar_clear() {
  cuenta_auto = 0;
  timerAlarmWrite(temporizador, 250000, true);
}

void setup() {
  pinMode(boton_mas, INPUT_PULLUP);
  pinMode(boton_menos, INPUT_PULLUP);
  pinMode(led_uno, OUTPUT);
  pinMode(led_dos, OUTPUT);
  pinMode(led_tres, OUTPUT);
  pinMode(led_cuatro, OUTPUT);
  pinMode(led_a, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(led_c, OUTPUT);
  pinMode(led_d, OUTPUT);
  pinMode(led_alerta, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(boton_mas), manejar_mas, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton_menos), manejar_menos, FALLING);
  touchAttachInterrupt(boton_clear, manejar_clear, 40); 

  temporizador = timerBegin(0, 80, true); 
  timerAttachInterrupt(temporizador, &manejar_tiempo, true);
  timerAlarmWrite(temporizador, 250000, true); 
  timerAlarmEnable(temporizador);

  actualizar_leds_manual();
  actualizar_leds_auto();
}

void loop() {
  
}