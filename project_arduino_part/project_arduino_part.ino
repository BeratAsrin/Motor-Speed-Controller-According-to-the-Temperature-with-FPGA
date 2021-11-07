#include <dht11.h>
#define temp_sensor_pin 2 // Sıcaklık sensörünün bağlı olduğu pin
#define temp_to_fpga_pin1 3 // Sıcaklık verisini FPGA'ya aktaran pinler
#define temp_to_fpga_pin2 4 // Sıcaklık verisini FPGA'ya aktaran pinler
#define temp_to_fpga_pin3 5 // Sıcaklık verisini FPGA'ya aktaran pinler
dht11 temp_sensor; // Sıcaklık sensörümü kullanabilmek için DHT11 class'ından bir adet obje üretiyorum.
int C_temp; // Sıcaklık sensöründen okunan sıcaklık değeri (Celcius)

// FPGA'ya 3 bit şeklinde sıcaklık değerini aktarmak için kullanılan fonksiyon.
void data_to_fpga(){
  // Eğer sensör tarafından okunan sıcaklık değeri 20 derecenin altındaysa:
  if(C_temp <= 20){
    digitalWrite(temp_to_fpga_pin1,HIGH); // 3 numaralı pinden true şeklinde çıkış alınır.
    digitalWrite(temp_to_fpga_pin2,LOW); // 4 numaralı pinden false şeklinde çıkış alınır.
    digitalWrite(temp_to_fpga_pin3,LOW); // 5 numaralı pinden false şeklinde çıkış alınır.
  }
  // Eğer sensör tarafından okunan sıcaklık değeri 20 derece ile 22 derece arasındaysa:
  else if(C_temp > 20 && C_temp <= 22){
    digitalWrite(temp_to_fpga_pin1,LOW); // 3 numaralı pinden false şeklinde çıkış alınır.
    digitalWrite(temp_to_fpga_pin2,HIGH); // 4 numaralı pinden true şeklinde çıkış alınır.
    digitalWrite(temp_to_fpga_pin3,LOW); // 5 numaralı pinden false şeklinde çıkış alınır.
  }
  // Eğer sensör tarafından okunan sıcaklık değeri 22 derecenin üstündeyse:
  else{
    digitalWrite(temp_to_fpga_pin1,LOW); // 3 numaralı pinden false şeklinde çıkış alınır.
    digitalWrite(temp_to_fpga_pin2,LOW); // 4 numaralı pinden false şeklinde çıkış alınır.
    digitalWrite(temp_to_fpga_pin3,HIGH); // 5 numaralı pinden true şeklinde çıkış alınır.
  }
}

// Sensörden sıcaklığı okumak için kullanılan fonksiyon.
void read_temp(){
  int start = temp_sensor.read(temp_sensor_pin); // Sensörden sıcaklık ve nem değerleri okunuyor.
  C_temp = temp_sensor.temperature; // C_temp değişkenine okunan sıcaklık değeri Celcius cinsinden atanıyor.
  data_to_fpga(); // FPGA'ya okunan veriyi aktarmak için kullanılan fonksiyon çağrılıyor.
}

// Başlangıç ayarlarını yapmak için kullanılan fonksiyon.
void setup() {
  Serial.begin(9600); // PC ile arduino arasındaki seri haberleşme hızını ayarlıyoruz.
  pinMode(temp_sensor_pin,INPUT); // 2 numaralı digital pin veri okumak için INPUT olarak tanımlanıyor.
  pinMode(temp_to_fpga_pin1,OUTPUT); // 3 numaralı digital pin veri yazmak için OUTPUT olarak tanımlanıyor.
  pinMode(temp_to_fpga_pin2,OUTPUT); // 4 numaralı digital pin veri yazmak için OUTPUT olarak tanımlanıyor.
  pinMode(temp_to_fpga_pin3,OUTPUT); // 5 numaralı digital pin veri yazmak için OUTPUT olarak tanımlanıyor.
}

// Döngü şeklinde çalışacak olan fonksiyon.
void loop() {
  read_temp(); // Sensörden gelen veriyi okumak için kullanılan fonksiyon çağrılıyor.
  // Okunan sıcaklık değeri seri port ekranına yazdırılıyor.
  Serial.print("Sıcaklık : ");
  Serial.print(C_temp);
  Serial.println(" °C");
  delay(2000); // Okuma frekansı olarak 2 saniyede bir belirleniyor.
}
