const int PIN_LDR_1 = 34; 
const int PIN_LDR_2 = 39; 

const int PIN_BUZZER_1 = 12;
const int PIN_BUZZER_2 = 27;

int umbralLuz = 900;   
int maxLuz = 3500;     

void setup() {
  Serial.begin(115200);

  ledcAttach(PIN_BUZZER_1, 2000, 8);
  ledcAttach(PIN_BUZZER_2, 2000, 8);
}

void loop() {

  int ldr1 = analogRead(PIN_LDR_1);
  int ldr2 = analogRead(PIN_LDR_2);

  if (ldr1 < umbralLuz) {

    ledcWriteTone(PIN_BUZZER_1, 0);
  } else {
    int freq1 = map(ldr1, umbralLuz, maxLuz, 300, 2000);
    freq1 = constrain(freq1, 300, 2000);
    ledcWriteTone(PIN_BUZZER_1, freq1);
  }

  if (ldr2 < umbralLuz) {
    ledcWriteTone(PIN_BUZZER_2, 0);
  } else {

    int freq2 = map(ldr2, umbralLuz, maxLuz, 2000, 400);
    freq2 = constrain(freq2, 400, 2000);
    ledcWriteTone(PIN_BUZZER_2, freq2);
  }

  Serial.print("LDR1: "); Serial.print(ldr1);
  Serial.print(" | LDR2: "); Serial.println(ldr2);

  delay(10);
}
