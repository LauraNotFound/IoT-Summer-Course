#include <Arduino.h>

#define TRIG1 5
#define ECHO1 12
#define TRIG2 14
#define ECHO2 27

#define UMBRAL_ACTIVACION 40  // cm
#define DISTANCIA_ENTRE_SENSORES 1.0  // metros

bool sensor1Activado = false;
bool medicionEnCurso = false;

unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;

long medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracion = pulseIn(echo, HIGH, 30000);
  if (duracion == 0) return 999;

  long distancia = duracion * 0.034 / 2;
  return distancia;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
}

void loop() {

  long dist1 = medirDistancia(TRIG1, ECHO1);
  long dist2 = medirDistancia(TRIG2, ECHO2);

  // Paso 1: Detectar primer sensor
  if (!medicionEnCurso && dist1 < UMBRAL_ACTIVACION) {
    tiempoInicio = micros();
    medicionEnCurso = true;
    Serial.println("Sensor 1 activado");
  }

  // Paso 2: Detectar segundo sensor
  if (medicionEnCurso && dist2 < UMBRAL_ACTIVACION) {
    tiempoFin = micros();

    unsigned long deltaTiempo = tiempoFin - tiempoInicio;

    float tiempoSegundos = deltaTiempo / 1000000.0;
    float velocidad = DISTANCIA_ENTRE_SENSORES / tiempoSegundos;
    float velocidadKmh = velocidad * 3.6;

    Serial.print("Velocidad: ");
    Serial.print(velocidadKmh);
    Serial.println(" km/h");

    medicionEnCurso = false;

    delay(1000);  // pequeña pausa antes de nueva medición
  }
}