# 🏠 Sistem Alarm Keamanan Rumah dengan Sensor PIR dan Notifikasi Blynk

![IoT Security](https://img.shields.io/badge/IoT-ESP32-blue?style=for-the-badge&logo=espressif)
![Blynk](https://img.shields.io/badge/Platform-Blynk%202.0-green?style=for-the-badge&logo=blynk)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

Sistem Alarm Keamanan Rumah ini adalah proyek berbasis **Internet of Things (IoT)** yang dirancang untuk mendeteksi dua ancaman utama pada rumah tinggal: **intrusi/penyusupan** dan **bahaya kebakaran (kebocoran gas/asap)**. Menggunakan ESP32 sebagai unit pemroses utama, sistem ini mengintegrasikan sensor lokal dengan kontrol *cloud* jarak jauh melalui aplikasi Blynk.

---

## 🚀 Fitur Utama
* **Sistem Keamanan Berlapis:** Deteksi gerakan manusia (PIR) dan deteksi kadar asap/gas (MQ2) secara simultan.
* **Sistem Prioritas Bahaya:** Peringatan kebakaran memiliki prioritas tertinggi dan akan mengabaikan status *arm/disarm* alarm demi keselamatan.
* **Kontrol Jarak Jauh (Cloud Control):** Mengaktifkan (*Arm*) atau menonaktifkan (*Disarm*) mode aman sensor PIR melalui aplikasi Blynk di smartphone.
* **Notifikasi Real-Time:** Mengirimkan *push notification* instan ke smartphone via Blynk Event jika terjadi kondisi bahaya.
* **Display & Indikator Lokal:** Dilengkapi dengan LCD 16x2 I2C untuk monitoring status dan nilai gas secara langsung, serta Buzzer sebagai alarm suara lokal.

---

## 🛠️ Alat dan Bahan

### Perangkat Keras (Hardware)
1. **NodeMCU ESP32 DevKit** (1x)
2. **Sensor PIR (HC-SR501)** - Deteksi gerakan (1x)
3. **Sensor Gas MQ2** - Deteksi asap/LPG (1x)
4. **LCD 16x2 dengan Modul I2C** (1x)
5. **Buzzer Piezoelektrik 5V** (1x)
6. Kabel Jumper & Breadboard

### Perangkat Lunak (Software)
* **Arduino IDE** (Versi 2.x direkomendasikan)
* **Blynk Mobile App** (Android/iOS)
* **Library Arduino:**
  * `WiFi.h` & `WiFiClient.h`
  * `BlynkSimpleEsp32.h`
  * `LiquidCrystal_I2C.h`

---

## 🔌 Skema Rangkaian (Circuit Diagram)

Berikut adalah panduan pengkabelan (*wiring*) komponen ke ESP32:

| Komponen | Pin Komponen | Pin ESP32 | Keterangan |
| :--- | :--- | :--- | :--- |
| **Sensor PIR** | VCC / OUT / GND | 5V / **GPIO 13** / GND | Input Digital |
| **Sensor MQ2** | VCC / AO / GND | 5V / **GPIO 34** / GND | Input Analog |
| **Buzzer** | Positif (+) / Negatif (-) | **GPIO 12** / GND | Output Digital |
| **LCD I2C** | VCC / GND / SDA / SCL | 5V / GND / **GPIO 21** / **GPIO 22** | Komunikasi I2C |

> *Catatan: Pastikan alamat I2C LCD Anda sesuai (umumnya `0x27` atau `0x3F`).*

---

## 💻 Logika Program

Sistem bekerja berdasarkan urutan prioritas kondisi berikut:
1. **Kondisi 1 (Kebakaran):** Jika Nilai MQ2 > 1500 $\rightarrow$ Buzzer ON, LCD menampilkan pesan bahaya gas, Notifikasi Blynk terkirim.
2. **Kondisi 2 (Intrusi):** Jika Mode Alarm AKTIF (dari Blynk) **DAN** PIR mendeteksi gerakan $\rightarrow$ Buzzer ON, LCD menampilkan pesan intrusi, Notifikasi Blynk terkirim.
3. **Kondisi 3 (Aman):** Jika semua sensor normal $\rightarrow$ Buzzer OFF, LCD menampilkan status alarm (Aktif/Nonaktif) dan nilai real-time sensor MQ2.

---

## 🔧 Panduan Instalasi & Penggunaan

1. **Konfigurasi Blynk:**
   * Buat template baru di Blynk Cloud bernama "Sistem Alarm Keamanan Rumah".
   * Buat Datastream:
     * **V0:** Virtual Pin (Integer) untuk data sensor Asap/Gas.
     * **V1:** Virtual Pin (Integer) untuk Tombol Switch Aktif/Nonaktif Alarm.
   * Buat Events & Notifications untuk `bahaya_kebakaran` dan `intrusi_terdeteksi`.

2. **Setup Kode Program:**
   * Clone repositori ini ke komputer Anda.
   * Buka file `.ino` menggunakan Arduino IDE.
   * Masukkan konfigurasi Blynk Anda pada baris kode berikut:
     ```cpp
     #define BLYNK_TEMPLATE_ID   "ISI_TEMPLATE_ID_ANDA"
     #define BLYNK_TEMPLATE_NAME "Sistem Alarm Keamanan Rumah"
     #define BLYNK_AUTH_TOKEN    "ISI_AUTH_TOKEN_ANDA"
     ```
   * Sesuaikan nama Wi-Fi dan Password:
     ```cpp
     char ssid[] = "NAMA_WIFI_ANDA";
     char pass[] = "PASSWORD_WIFI_ANDA";
     ```

3. **Upload & Jalankan:**
   * Hubungkan ESP32 ke komputer, pilih *board* dan *port* yang sesuai di Arduino IDE, lalu klik **Upload**.
   * Buka Serial Monitor (baudrate 115200) untuk memantau status koneksi.

---

## 📄 Lisensi
Proyek ini dilisensikan di bawah **MIT License** - lihat file [LICENSE](LICENSE) untuk detailnya.