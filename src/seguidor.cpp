#include <Arduino.h>

#define DEBUG_LEVEL 3

#define printV(x) Serial.print("\e[0;36m" + String(x) + "\e[0m")
#define printlnV(x) Serial.println("\e[0;36m" + String(x) + "\e[0m")

#define printD(x) Serial.print("\e[0;34m" + String(x) + "\e[0m")
#define printlnD(x) Serial.println("\e[0;34m" + String(x) + "\e[0m")

#define printI(x) Serial.print("\e[0;32m" + String(x) + "\e[0m")
#define printlnI(x) Serial.println("\e[0;32m" + String(x) + "\e[0m")

#define printW(x) Serial.print("\e[0;33m" + String(x) + "\e[0m")
#define printlnW(x) Serial.println("\e[0;33m" + String(x) + "\e[0m")

#define printE(x) Serial.print("\e[0;31m" + String(x) + "\e[0m")
#define printlnE(x) Serial.println("\e[0;31m" + String(x) + "\e[0m")

#define printA(x) Serial.print("\e[0;37m" + String(x) + "\e[0m")
#define printlnA(x) Serial.println("\e[0;37m" + String(x) + "\e[0m")

#if DEBUG_LEVEL == 0 // Verbose
#elif DEBUG_LEVEL == 1 // Debug
  #define printV(x)
  #define printlnV(x)
#elif DEBUG_LEVEL == 2 // Information
  #define printV(x)
  #define printlnV(x)
  #define printD(x)
  #define printlnD(x)
#elif DEBUG_LEVEL == 3 // Warning
  #define printV(x)
  #define printlnV(x)
  #define printD(x)
  #define printlnD(x)
  #define printI(x)
  #define printlnI(x)
#endif

// Si quieres probar la placa verde descomenta la siguiente línea:
#define Lamborghino

// Si quieres probar la placa negra descomenta la siguiente línea:
//#define Predator

#ifdef Lamborghino
  #define AIN1 4
  #define AIN2 9
  #define PWMA 5
  #define BIN1 7 
  #define BIN2 8
  #define PWMB 6
  #define LEDON 11
  #define PULS1 12
  #define PULS2 12
  #define LED1 10
  #define LED2 10
#elif defined(Predator)
  #define AIN1 7
  #define AIN2 6
  #define PWMA 5
  #define BIN1 9
  #define BIN2 10
  #define PWMB 11
  #define STBY 8
  #define LEDON 13
  #define PULS1 2
  #define PULS2 12
  #define LED1 4
  #define LED2 3
#endif

#define kp 0.1
#define kd 0.005
#define ki 0.000001
#define base 50

int maxLec[] = {0,0,0,0,0,0};

int pinesSensores[] = {14,15,16,17,18,19};
int motorPins[] = {AIN1, AIN2, PWMA, BIN1, BIN2, PWMB};

int * minimos, * maximos;

int * calibracionBaja(int lecturas);
int * calibracionAlta(int lecturas);
int getPos();
double PID(int input, int setpoint);
void power(double error);

void setup()
{
  Serial.begin(9600);

  for (byte i = 0; i < sizeof(pinesSensores)/sizeof(pinesSensores[0]); i++)
  {
    pinMode(pinesSensores[i],INPUT);
  }

  for(unsigned int i = 0; i<sizeof(motorPins)/sizeof(motorPins[0]); i++)
  {
    pinMode(motorPins[i], 1);
  }

  #if defined(Predator)
      pinMode(STBY,OUTPUT);
      pinMode(LEDON,OUTPUT);
      digitalWrite(STBY, 1);
  #endif

  pinMode(PULS1, INPUT);
  pinMode(PULS2, INPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);


  printlnA("Color Blanco");
  printlnA("Esperando pulsación S1");


  while(!digitalRead(PULS1)){}

  digitalWrite(LED1, HIGH);

  minimos = calibracionBaja(1000);

  digitalWrite(LED1, LOW);

  
  printlnA("Color Negro");
  printlnA("Esperando pulsación");

  while(!digitalRead(PULS2)){}

  digitalWrite(LED2, HIGH);

  maximos = calibracionAlta(1000);

  digitalWrite(LED2, LOW);

  printlnA("Esperando pulsación");

  while(!digitalRead(PULS2) || !digitalRead(PULS1)){}

  #if defined(Lamborghino)
    tone(LED1, 2000, 500);
    delay(100);
    tone(LED1, 2100, 500);
    delay(100);
    tone(LED1, 2200, 500);
    delay(100);
    tone(LED1, 2300, 500);
    delay(100);
    tone(LED1, 2400, 100);
    delay(200);
    tone(LED1, 2400, 100);
    delay(200);
    tone(LED1, 2400, 500);
    delay(100);
  #elif defined(Predator)
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(500);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(500);
  #endif
}

void loop()
{
  int val = map(getPos(),0,5000,-255,255);
  power(PID(val,0));
}

int * calibracionBaja(int lecturas)
{
  static int minLec[6] = {1023,1023,1023,1023,1023,1023};

  printlnA("Inicio de calibración baja");

  for (int i = 0; i < lecturas; i++)
  {
    for (byte j = 0; j < sizeof(pinesSensores)/sizeof(pinesSensores[0]); j++)
    {
      minLec[j] = minLec[j] > analogRead(pinesSensores[j]) ? analogRead(pinesSensores[j]) : minLec[j];
      printV(minLec[j]);
      printV("\t");
    }
    printV("\n");
  }

  for (byte i = 0; i < sizeof(pinesSensores)/sizeof(pinesSensores[0]); i++)
  {
    printD(minLec[i]);
    printD("\t");
  }
  printlnD();

  printlnA("Fin calibración baja");

  return minLec;
}

int * calibracionAlta(int lecturas)
{
  static int maxLec[6] = {0,0,0,0,0,0};

  printlnA("Inicio de calibración alta");

  for (int i = 0; i < lecturas; i++)
  {
    for (byte j = 0; j < sizeof(pinesSensores)/sizeof(pinesSensores[0]); j++)
    {
      maxLec[j] = maxLec[j] < analogRead(pinesSensores[j]) ? analogRead(pinesSensores[j]) : maxLec[j];
      printV(maxLec[j]);
      printV("\t");
    }
    printV("\n");
  }

  for (byte i = 0; i < sizeof(pinesSensores)/sizeof(pinesSensores[0]); i++)
  {
    printD(maxLec[i]);
    printD("\t");
  }
  printlnD();

  printlnA("Fin calibración alta");

  return maxLec;
}

int getPos()
{
  byte numSensors = sizeof(pinesSensores)/sizeof(pinesSensores[0]);
  int static lastValue = 0;
  boolean on_line = false;
  unsigned long prom;
  unsigned int sum;
  prom = 0;
  sum = 0;
  for(byte i=0;i< numSensors ;i++) 
  {
      int lectura = analogRead(pinesSensores[i]);
      lectura = map(lectura,*(minimos + i),*(maximos + i),0,1000);
      lectura = constrain(lectura,0,1000);
      if(lectura > 200) 
      {
          on_line = 1;
      }
      if(lectura > 50) 
      {
          prom += (long)(lectura) * (i * 1000);
          sum += lectura;
      }
  }

  if(!on_line)
  {
      if(lastValue < (numSensors-1)*1000/2)
          return 0;
      else
          return (numSensors-1)*1000;
  }

  lastValue = prom/sum;

  return lastValue;
}

double PID(int input, int setpoint)
{
  unsigned static long previousTime = 0;
  unsigned static long currentTime; 
  static double rateError = 0;
  static double lastError = 0;
  static double cumError = 0;

  currentTime = millis();
  long elapsedTime = currentTime - previousTime;
  int error = setpoint - input;
  cumError = cumError + (error*elapsedTime);
  rateError = (error - lastError)/elapsedTime;

  float out =  kp*error + ki*cumError + kd*rateError;

  printD("error:");
  printD(error);
  printD("\t");
  printD("cumError:");
  printD(cumError);
  printD("\t");
  printD("elapsedTime:");
  printD(elapsedTime);
  printD("\t");
  printD("error*elapsedTime:");
  printD(error*elapsedTime);
  printD("\t");
  printD("out:");
  printlnD(out);

  lastError = error;
  previousTime = currentTime;

  return out;
}

void power(double error)
{
  float vI = base + error;
  float vD = base - error;



  if(vI < 0)
  {
    digitalWrite(AIN1,0);
    digitalWrite(AIN2,1);
  }
  else
  {
    digitalWrite(AIN1,1);
    digitalWrite(AIN2,0);
  }

  vI = abs(vI);
  vD = abs(vD);

    if(vD < 0)
  {
    digitalWrite(BIN1,0);
    digitalWrite(BIN2,1);
  }
  else
  {
    digitalWrite(BIN1,1);
    digitalWrite(BIN2,0);
  }

  printD(vI);
  printD("\t");
  printlnD(vD);

  analogWrite(PWMA, vI);
  analogWrite(PWMB, vD);
}