#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <time.h>  // Para obtener fecha y hora real

// ===== WIFI =====
#define WIFI_SSID "lauranotfound"
#define WIFI_PASSWORD "l@ur@xf0und"

// ===== FIREBASE =====
#define API_KEY "AIzaSyDf8bq0k3Cqx5W9gW4vN7TIiKbl0gA-F6Y"
#define DATABASE_URL "studio-1021005707-95c20-default-rtdb.firebaseio.com"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ===== PINES =====
#define TRIG1 5
#define ECHO1 12
#define TRIG2 14
#define ECHO2 27
#define BUZZER 4

// ===== REFERENCIAS FÍSICAS =====
#define UMBRAL_ENTRADA 25  // cm
#define UMBRAL_SALIDA 25   // cm

// ===== ESTADOS =====
bool vehiculoEntrada = false;
bool vehiculoSalida = false;

// ===== TIEMPOS =====
unsigned long lastEventoEntrada = 0;
unsigned long lastEventoSalida = 0;
#define MIN_INTERVALO_EVENTO 2000  // 2 segundos entre detecciones del mismo sensor

// =====================================================
void enviarEvento(String tipo) {
  if (Firebase.ready()) {
    FirebaseJson json;

    // Obtener hora local
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char fechaHora[25];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", timeinfo);

    json.set("tipo", tipo);
    json.set("fecha_hora", fechaHora); // Hora legible
    json.set("timestamp", now);        // Timestamp en segundos

    Firebase.RTDB.pushJSON(&fbdo, "/parking/eventos", &json);
  }
}

// =====================================================
long medirDistancia(int trig, int echo) {
  long suma = 0;
  int validas = 0;

  for (int i = 0; i < 5; i++) { // 5 mediciones para mayor estabilidad
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duracion = pulseIn(echo, HIGH, 30000); // timeout 30ms
    if (duracion != 0) {
      long distancia = duracion * 0.034 / 2; // convertir a cm
      if (distancia > 2 && distancia < 400) {
        suma += distancia;
        validas++;
      }
    }
    delay(5);
  }

  if (validas == 0) return 999; // valor inválido
  return suma / validas;
}

// =====================================================
void setup() {
  Serial.begin(115200);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(BUZZER, OUTPUT);

  // Conectar WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Conectado a WiFi!");

  // Configuración Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.test_mode = true;

  // Sincronizar hora con NTP
  configTime(-5 * 3600, 0, "pool.ntp.org");
  while (time(nullptr) < 100000) { delay(100); }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// =====================================================
void loop() {
  unsigned long now = millis();

  int distEntrada = medirDistancia(TRIG1, ECHO1);
  int distSalida = medirDistancia(TRIG2, ECHO2);

  Serial.print("Entrada: "); Serial.print(distEntrada); 
  Serial.print(" cm | Salida: "); Serial.println(distSalida);

  // ===== DETECCIÓN ENTRADA =====
  if (distEntrada < UMBRAL_ENTRADA) {
    vehiculoEntrada = true;
  }
  if (distEntrada >= UMBRAL_ENTRADA && vehiculoEntrada) {
    if (now - lastEventoEntrada > MIN_INTERVALO_EVENTO) {
      enviarEvento("entrada");
      tone(BUZZER, 2500, 100);
      lastEventoEntrada = now;
    }
    vehiculoEntrada = false;
  }

  // ===== DETECCIÓN SALIDA =====
  if (distSalida < UMBRAL_SALIDA) {
    vehiculoSalida = true;
  }
  if (distSalida >= UMBRAL_SALIDA && vehiculoSalida) {
    if (now - lastEventoSalida > MIN_INTERVALO_EVENTO) {
      enviarEvento("salida");
      tone(BUZZER, 1500, 100);
      lastEventoSalida = now;
    }
    vehiculoSalida = false;
  }

  delay(50); // pequeña pausa para estabilidad
}