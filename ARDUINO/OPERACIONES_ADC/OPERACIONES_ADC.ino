#include <SPI.h>

// Pines SPI por defecto para VSPI en ESP32:
#define PIN_MOSI 11
#define PIN_MISO 13

#define PIN_SCK  12
#define PIN_SS   10  // Chip Select

uint16_t  dato = 0x7000;  // Dato a enviar (por ejemplo: 0xAB)
uint8_t  respuesta1 = 0x00,respuesta2 = 0x00;
uint16_t  val_final= 0x00;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  
  int i=micros();
  // Configurar el pin SS (Chip Select)
  pinMode(PIN_SS, OUTPUT);
  digitalWrite(PIN_SS, HIGH);  // Desactivar esclavo por defecto
  // Inicializar el bus SPI
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SS);
  i=micros()-i;
  Serial.println("TIEMPO CONFIGURACIÓN MASTER SPI (en microsegundos)");
  Serial.println(i);   
  Serial.println("SPI Master listo.");
  //Pruebas:
  Serial.println("LECTURA ADC CH0:");
  i=micros(); 
  digitalWrite(PIN_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x01);
  respuesta1=SPI.transfer(0x80);
  respuesta2=SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);
  i=micros()-i;
  Serial.println("TIEMPO LECTURA CH0 (en microsegundos)");
  Serial.println(i);
  i=micros();
  val_final=((respuesta1 & 0x03) << 8) | respuesta2;
  i=micros()-i;
  Serial.println("TIEMPO PROCESADO DE DATOS (en microsegundos):");
  Serial.println(i);
  Serial.println("VALOR DEL CANAL 0(BITS):");
  Serial.println(val_final);
  Serial.println("VALOR DEL CANAL 0 (V):");
  float voltaje = (val_final / 1023.0) * 5;
  Serial.println(voltaje,3);
  
  delay(5000);  

  Serial.println("LECTURA ADC CH1:");
  i=micros();
  digitalWrite(PIN_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x01);
  respuesta1=SPI.transfer(0x90);
  respuesta2=SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);
  i=micros()-i;
  Serial.println("TIEMPO LECTURA CH1 (en microsegundos)");
  Serial.println(i);
  i=micros();
  val_final=((respuesta1 & 0x03) << 8) | respuesta2;
  i=micros()-i;
  Serial.println("TIEMPO PROCESADO DE DATOS (en microsegundos):");
  Serial.println(i);
  Serial.println("VALOR DEL CANAL 1(BITS):");
  Serial.println(val_final);
  Serial.println("VALOR DEL CANAL 1 (V):");
  voltaje = (val_final / 1023.0) * 5;
  Serial.println(voltaje,3);

  delay(5000);  

  Serial.println("LECTURA ADC CH2:");
  i=micros();
  digitalWrite(PIN_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x01);
  respuesta1=SPI.transfer(0xA0);
  respuesta2=SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);
  i=micros()-i;
  Serial.println("TIEMPO LECTURA CH2 (en microsegundos)");
  Serial.println(i);
  i=micros();
  val_final=((respuesta1 & 0x03) << 8) | respuesta2;
  i=micros()-i;
  Serial.println("TIEMPO PROCESADO DE DATOS (en microsegundos):");
  Serial.println(i);
  Serial.println("VALOR DEL CANAL 2(BITS):");
  Serial.println(val_final);
  Serial.println("VALOR DEL CANAL 2 (V):");
  voltaje = (val_final / 1023.0) * 5;
  Serial.println(voltaje,3);

  delay(5000);

  Serial.println("RESPUESTA DETECCIONES DE ERRORES CH0 Y CH2:");

}

int j=0;

void loop() {

  Serial.println("LECTURA ADC CH0:");
  digitalWrite(PIN_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x01);
  respuesta1=SPI.transfer(0x80);
  respuesta2=SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);
  j=micros();
  val_final=((respuesta1 & 0x03) << 8) | respuesta2;
  Serial.println("VALOR DEL CANAL 0(BITS):");
  Serial.println(val_final);
  Serial.println("VALOR DEL CANAL 0 (V):");
  float voltaje = (val_final / 1023.0) * 5;
  Serial.println(voltaje,3);
  
  if(voltaje<2.5){
    Serial.println("ERROR EN LA ALIMENTACIÓN DE 12V");
    Serial.println("DESHABILITANDO LAS ALIMENTACIONES: 12V, 5V Y Vdac");
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    j=micros()-j;
    Serial.println("TIEMPO DE RESPUESTA UC");
    Serial.println(j);
  }  

  delay(2500);
    

  Serial.println("LECTURA ADC CH2:");
  digitalWrite(PIN_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x01);
  respuesta1=SPI.transfer(0xA0);
  respuesta2=SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);
  j=micros();
  val_final=((respuesta1 & 0x03) << 8) | respuesta2;
  Serial.println("VALOR DEL CANAL 2(BITS):");
  Serial.println(val_final);
  Serial.println("VALOR DEL CANAL 2 (V):");
  voltaje = (val_final / 1023.0) * 5;
  Serial.println(voltaje,3);

  if(voltaje<2.5){
    Serial.println("ERROR EN LA ALIMENTACIÓN DE 5V");
    Serial.println("DESHABILITANDO LAS ALIMENTACIONES: 5V Y Vdac");
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    j=micros()-j;
    Serial.println("TIEMPO DE PROCESADO Y RESPUESTA UC");
    Serial.println(j);
  }  

  delay(2500); 

}