#include <SPI.h>

// Pines SPI por defecto para VSPI en ESP32:
#define PIN_MOSI 11
#define PIN_MISO 13
#define PIN_SCK  12
#define PIN_SS   10  // Chip Select

uint16_t  dato = 0x7000;  // Dato a enviar (por ejemplo: 0xAB)
uint16_t  respuesta = 0x00;

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
    
  // Configurar el pin SS (Chip Select)
  pinMode(PIN_SS, OUTPUT);
  digitalWrite(PIN_SS, HIGH);  // Desactivar esclavo por defecto

  // Inicializar el bus SPI
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SS);

  Serial.println("SPI Master listo.");
  //Prueba resoluciones maximas y minimas:
  digitalWrite(PIN_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer16(0x7000);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);
  delay(5000);
  int dato2=micros();
  int dato1=micros();
  if (dato == 0x7000){
    digitalWrite(PIN_SS, LOW);
  }
  dato2=micros()-dato2;
  Serial.println("TIEMPO CONFIGURACIÓN SPI DAC:");
  Serial.println(dato2);

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  
  SPI.transfer16(0x7FFF);
  SPI.endTransaction();
  digitalWrite(PIN_SS, HIGH);

  dato1=micros()-dato1;
  Serial.println("TIEMPO EJECUCIÓN 1 OPERACIÓN DAC:");
  Serial.println(dato1);
  delay(2000);
}

void loop() {

  // Activar esclavo
  digitalWrite(PIN_SS, LOW);
  // Enviar el dato SPI y recibir el dato del esclavo (si responde)
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  
  SPI.transfer16(dato);
  SPI.endTransaction();
  // Desactivar esclavo
  digitalWrite(PIN_SS, HIGH);

  // Imprimir respuesta recibida (si hay)
  Serial.print("Enviado: 0x");
  Serial.println(dato, HEX);
  delay(1000);  // Esperar un segundo antes de enviar de nuevo
  if(dato<0x7FFF){
    dato=dato+100;
  }else{
    dato=0x7000;
  }
  if(dato>0x7FFF){
    dato=0x7000;
  }
}