#define BLYNK_TEMPLATE_ID   "TMPL6sz6MDIN5"
#define BLYNK_TEMPLATE_NAME "Sistem Alarm Keamanan Rumah"
#define BLYNK_AUTH_TOKEN    "VCkbcWtkiHPBT1W5dpTfe15s9rCJWgJf"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Definisi Pin
const int pinPIR = 13;
const int pinMQ2 = 34;
const int pinBuzzer = 12;

// Inisialisasi LCD I2C (Alamat umum biasanya 0x27 atau 0x3F, ukuran 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Variabel Kontrol
int modeAlarm = 0; // 0 = NONAKTIF, 1 = AKTIF
int ambangBatasAsap = 1500; 

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Membaca status tombol AKTIF/NONAKTIF dari Blynk (Virtual Pin V1)
BLYNK_WRITE(V1) {
  modeAlarm = param.asInt();
  updateTampilanLCD(); // Perbarui tampilan saat tombol di aplikasi ditekan
}

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Menghubungkan...");
  
  Blynk.begin(auth, ssid, pass);
  
  pinMode(pinPIR, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  lcd.clear();
}

void loop() {
  Blynk.run();
  
  int statusPIR = digitalRead(pinPIR);
  int nilaiMQ2 = analogRead(pinMQ2);
  
  // Kirim data asap ke Blynk (Virtual Pin V0)
  Blynk.virtualWrite(V0, nilaiMQ2); 

  // LOGIKA UTAMA
  if (nilaiMQ2 > ambangBatasAsap) {
    // 1. Kebakaran/Gas (Prioritas Utama)
    digitalWrite(pinBuzzer, HIGH);
    Blynk.logEvent("bahaya_kebakaran", "Bahaya! Terdeteksi asap atau kebocoran gas!");
    
    lcd.setCursor(0, 0);
    lcd.print("** BAHAYA!   **");
    lcd.setCursor(0, 1);
    lcd.print("TERDETEKSI ASAP ");
  } else if (modeAlarm == 1 && statusPIR == HIGH) {
    // 2. Ada Penyusup (Hanya berfungsi saat ALARM AKTIF)
    digitalWrite(pinBuzzer, HIGH);
    Blynk.logEvent("intrusi_terdeteksi", "Peringatan! Ada pergerakan mencurigakan!");
    
    lcd.setCursor(0, 0);
    lcd.print("** PERINGATAN **");
    lcd.setCursor(0, 1);
    lcd.print("ADA INTRUSI!    ");
  } else {
    // 3. Kondisi Aman
    digitalWrite(pinBuzzer, LOW);
    
    // Tampilkan status normal pada LCD
    lcd.setCursor(0, 0);
    if (modeAlarm == 1) {
      lcd.print("STATUS: ALARM AKTIF");
    } else {
      lcd.print("STATUS: ALARM NONAKTIF");
    }
    
    // Tampilkan nilai asap di baris kedua (diberi spasi di akhir agar angka sebelumnya terhapus)
    lcd.setCursor(0, 1);
    lcd.print("Asap/Gas: ");
    lcd.print(nilaiMQ2);
    lcd.print("    "); 
  }
  
  delay(200); // Delay singkat untuk stabilitas pembacaan
}

// Fungsi bantu untuk mereset tampilan LCD saat ada perubahan mode
void updateTampilanLCD() {
  lcd.clear();
}