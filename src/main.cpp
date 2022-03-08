#include <Arduino.h>

/* El programa funciona de la siguiente manera:
- Rutina 1: Prueba de motores, inicia con nada de velocidad y va aumentando paulatinamente, luego cambia de sentido de giro y repite el proceso
- Rutina 2: Prueba de sensores, debes iniciar el monitor Serial de Arduino, se va a poder visualizar los valores de cada sensor de 0 a 1023. */

///////////////////////
// ELECCIÓN DE PLACA //
///////////////////////

// EN ESTA SECCIÓN SOLO UNA LINEA PUEDE ESTAR DESCOMENTADA A LA VEZ 

// Si quieres probar la placa verde descomenta la siguiente línea:
//#define Lamborghino

// Si quieres probar la placa negra descomenta la siguiente línea:
#define Predator

////////////////////////
// ELECCIÓN DE RUTINA //
////////////////////////

// EN ESTA SECCIÓN SOLO UNA LINEA PUEDE ESTAR DESCOMENTADA A LA VEZ 

// Si quieres probar motores descomenta la siguiente línea:
//#define Rutina 1

// Si quieres probar sensores descomenta la siguiente línea:
#define Rutina 2

#ifdef Lamborghino
  #define AIN1 4
  #define AIN2 9
  #define PWMA 5
  #define BIN1 7 
  #define BIN2 8
  #define PWMB 6
  #define LEDON 11
#elif defined(Predator)
  #define AIN1 7
  #define AIN2 6
  #define PWMA 5
  #define BIN1 9
  #define BIN2 10
  #define PWMB 11
  #define STBY 8
  #define LEDON 13
#endif

int sensorPins[] = {14,15,16,17,18,19};
int motorPins[] = {AIN1, AIN2, PWMA, BIN1, BIN2, PWMB};

bool aux = false;

void setup() 
{
  #if Rutina == 2
    Serial.begin(9600);
  #endif
  for(unsigned int i = 0; i<sizeof(sensorPins)/sizeof(sensorPins[0]); i++)
  {
    pinMode(sensorPins[i], 0);
  }
  for(unsigned int i = 0; i<sizeof(motorPins)/sizeof(motorPins[0]); i++)
  {
    pinMode(motorPins[i], 1);
  }
  #if defined(Predator)
      pinMode(STBY,OUTPUT);
      pinMode(LEDON,OUTPUT);
  #endif
}

void loop() 
{
  #if Rutina == 1
    #if defined(Predator)
      digitalWrite(STBY, 1);
    #endif
    aux = !aux;
    digitalWrite(AIN1, aux);
    digitalWrite(AIN2, !aux);
    digitalWrite(BIN1, aux);
    digitalWrite(BIN2, !aux);
    for(int i=0; i<255; i++)
    {
      analogWrite(PWMA, i);
      analogWrite(PWMB, i);
      delay(100);
    }
    for(int i=255; i>0; i--)
    {
      analogWrite(PWMA, i);
      analogWrite(PWMB, i);
      delay(100);
    }
  #elif Rutina == 2
    digitalWrite(LEDON, 1);
    for(unsigned int i = 0; i<sizeof(sensorPins)/sizeof(sensorPins[0]); i++)
    {
      Serial.print(analogRead(sensorPins[i]));
      Serial.print("\t");
    }
    Serial.println();
  #endif
}