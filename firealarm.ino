#include <DHT.h>         // Tambahkan library DHT
#include <WiFi.h>       // Tambahkan library WiFi
#include <WiFiClient.h> // Tambahkan library WiFiClient
#include <BlynkSimpleEsp32.h> // Tambahkan library Blynk

#define MQ2_PIN 34     // Pin untuk sensor MQ2
#define LED_PIN 33     // Pin untuk LED
#define RELAY_PIN 27   // Pin untuk relay
#define DHT_PIN 26     // Pin untuk sensor DHT11
#define DHT_TYPE DHT11 // Tipe sensor DHT11
#define BUZZER_PIN 13  // Pin untuk buzzer

DHT dht(DHT_PIN, DHT_TYPE);  // Inisialisasi sensor DHT11

char auth[] = "zql0jPT4rw5ji8yycRxEH75KXqpAC5-q"; // Masukkan Blynk Auth Token

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();  // Mulai sensor DHT11

  Blynk.begin(auth, "delvin", "delvin1234"); // Masukkan SSID dan Password WiFi Anda

  while (!Blynk.connected()) {
    delay(100);
  }

  Serial.println("Terhubung ke Blynk...");
}

void loop() {
  Blynk.run();

  if (Blynk.connected()) {
    int mq2_value = analogRead(MQ2_PIN);    // Baca nilai dari MQ2

    Serial.print("Nilai MQ2: ");
    Serial.println(mq2_value);

    // Baca suhu dan kelembaban dari sensor DHT11
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Cek jika nilai suhu dan kelembaban valid
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Gagal membaca suhu dan kelembaban dari sensor DHT11!");
    } else {
      Serial.print("Suhu: ");
      Serial.print(temperature);
      Serial.print(" *C, Kelembaban: ");
      Serial.print(humidity);
      Serial.println(" %");
    }

    // Jika nilai dari MQ2 melebihi 1000, aktifkan LED, relay, dan bunyikan buzzer dengan pola High-Low
    if (mq2_value > 1500) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(BUZZER_PIN, HIGH);
      
    } else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(BUZZER_PIN, LOW);
    }

    Blynk.virtualWrite(V1, temperature);  // Mengirim data suhu ke widget Value Display V1 di Blynk
    Blynk.virtualWrite(V2, humidity);     // Mengirim data kelembaban ke widget Value Display V2 di Blynk
    Blynk.virtualWrite(V3, mq2_value);    // Mengirim data nilai MQ2 ke widget Value Display V3 di Blynk

    delay(1000);
  }
}
