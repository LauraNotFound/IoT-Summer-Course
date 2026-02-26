# 🚀 Portafolio Académico – Sistemas IoT con ESP32

Este repositorio integra tres proyectos desarrollados con **ESP32**, orientados a la medición física, automatización y procesamiento de señales mediante sensores electrónicos.

Los proyectos fueron implementados como parte de prácticas académicas en el área de **Internet de las Cosas (IoT)** y Sistemas Embebidos.

---

# 📂 Contenido del Repositorio

1. 🚗 Smart Parking System  
2. 🏎 Velocity Measurement Device  
3. 🎵 IoT Theremin  
4. 🎥 Videos demostrativos (uno por cada proyecto)

---

# 1️⃣ Smart Parking System

## 🎯 Objetivo

Diseñar un sistema de estacionamiento inteligente capaz de:

- Detectar la ocupación de espacios
- Enviar datos a la nube
- Permitir monitoreo remoto en tiempo real

## 🧠 Arquitectura del Sistema

- ESP32
- Sensores ultrasónicos HC-SR04
- Conectividad WiFi
- Firebase Realtime Database
- Fuente de alimentación 5V externa

## ⚙ Funcionamiento

1. El sensor ultrasónico mide la distancia al vehículo.
2. Si la distancia es menor que un umbral → espacio ocupado.
3. El ESP32 envía el estado a Firebase.
4. La base de datos actualiza la disponibilidad en tiempo real.

## 📚 Conceptos Aplicados

- Arquitectura IoT orientada a nube
- Comunicación cliente-servidor
- Integración sensor–actuador
- Calibración por umbral

## 🎥 Video demostrativo

Ubicado en la carpeta `/videos`.

---

# 2️⃣ Velocity Measurement Device

## 🎯 Objetivo

Calcular la velocidad de un vehículo mediante detección secuencial utilizando sensores físicos.

## 🧠 Principio Físico

La velocidad se calcula mediante:

v = d / t

Donde:

- d = distancia fija entre sensores  
- t = tiempo medido entre activaciones  

## ⚙ Componentes

- ESP32
- 2 sensores (ultrasónicos o reflectivos según versión)
- Medición de tiempo con `millis()`
- Cálculo en tiempo real

## 🔄 Lógica de Funcionamiento

1. Sensor 1 detecta vehículo → inicia cronómetro.
2. Sensor 2 detecta vehículo → detiene cronómetro.
3. Se calcula la velocidad.
4. El resultado se muestra por Serial Monitor.

## 📚 Conceptos Aplicados

- Medición de tiempo en microcontroladores
- Programación orientada a eventos
- Variables críticas de calibración
- Eliminación de falsas detecciones

## 🎥 Video demostrativo

Ubicado en la carpeta `/videos`.

---

# 3️⃣ IoT Theremin

## 🎯 Objetivo

Construir un instrumento electrónico tipo Theremin utilizando variaciones de luz como entrada analógica.

## 🧠 Principio de Funcionamiento

El sistema utiliza:

- 2 LDRs conectados a ADC1 (GPIO34 y GPIO39)
- 2 buzzers pasivos controlados por PWM
- Conversión de intensidad lumínica a frecuencia sonora

## 🔬 Lógica Implementada

- Si la luz supera un umbral → se genera tono
- La frecuencia se calcula usando la función `map()`
- Se utiliza `ledcWriteTone()` para generar señal PWM

Ejemplo de conversión:

```cpp
int freq = map(ldrValue, umbralLuz, maxLuz, freqMin, freqMax);
