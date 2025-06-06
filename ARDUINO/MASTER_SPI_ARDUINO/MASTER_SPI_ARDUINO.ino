#include <SPI.h>

// Pines SPI para ESP32 VSPI
#define PIN_MOSI   11
#define PIN_MISO   13
#define PIN_SCK    12
#define PIN_CS_ADC 10   // CS para ADC
#define PIN_CS_DAC 9    // CS para DAC

uint16_t dato = 0x7000;  // Dato a enviar al DAC
uint8_t respuesta1 = 0x00, respuesta2 = 0x00;
uint16_t val_final = 0x00;
float voltaje = 0.0;

void setup() {
  Serial.begin(115200);

  // Mostrar configuración SPI
  Serial.println("Inicializando SPI Master con 2 dispositivos...");
  Serial.print("MOSI: "); Serial.println(MOSI);
  Serial.print("MISO: "); Serial.println(MISO);
  Serial.print("SCK: ");  Serial.println(SCK);
  Serial.print("CS ADC: "); Serial.println(PIN_CS_ADC);
  Serial.print("CS DAC: "); Serial.println(PIN_CS_DAC);

  // Configurar pines de CS
  pinMode(PIN_CS_ADC, OUTPUT);
  pinMode(PIN_CS_DAC, OUTPUT);
  digitalWrite(PIN_CS_ADC, HIGH);
  digitalWrite(PIN_CS_DAC, HIGH);

  // Inicializar SPI
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  // Pines de error
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  delay(500);
}

void loop() {
  // ---- Lectura ADC CH0 ----
  val_final = leerADC(0); // canal 0
  voltaje = (val_final / 1023.0) * 5.0;
  Serial.print("CH0 = "); Serial.print(val_final); Serial.print(" | "); Serial.print(voltaje, 3); Serial.print(" V | +12V_c ≈ "); Serial.print(voltaje *3.94, 2); Serial.println(" V");
  if (voltaje < 2.5) {
    Serial.println("ERROR 12V: Desactivando 12V, 5V y 2.5V");
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  }

  delay(2500);
  // ---- Lectura ADC CH1 ----
  val_final = leerADC(1); // canal 1
  voltaje = (val_final / 1023.0) * 5.0;
  Serial.print("CH1 = "); Serial.print(val_final); Serial.print(" | "); Serial.print(voltaje, 3); Serial.println(" V");

  if (voltaje < 2.5) {
    Serial.println("ERROR 5V: Desactivando 5V y 2.5V");
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  }

  delay(2500);

  // ---- Lectura ADC CH2 ----
  val_final = leerADC(2); // canal 2
  voltaje = (val_final / 1023.0) * 5.0;
  Serial.print("CH2 = "); Serial.print(val_final); Serial.print(" | "); Serial.print(voltaje, 3); Serial.println(" V");

  if (voltaje < 2) {
    Serial.println("ERROR 2.5V: Desactivando 2.5V");
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  }

  delay(2500);

  // ---- Escritura DAC ----
  escribirDAC(dato);
  Serial.print("DAC -> Enviado: 0x"); Serial.println(dato, HEX);

  // Incremento
  dato = (dato < 0x7FFF) ? dato + 100 : 0x7000;

  delay(1000);
}

// ---------------- FUNCIONES ----------------

uint16_t leerADC(uint8_t canal) {
  uint8_t cmd = 0x80 | ((canal & 0x07) << 4);
  uint8_t r1, r2;

  digitalWrite(PIN_CS_ADC, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x01);           // Byte de inicio para MCP3008
  r1 = SPI.transfer(cmd);       // Comando con canal
  r2 = SPI.transfer(0x00);      // Leer respuesta
  SPI.endTransaction();
  digitalWrite(PIN_CS_ADC, HIGH);

  return ((r1 & 0x03) << 8) | r2;
}

void escribirDAC(uint16_t valor) {
  digitalWrite(PIN_CS_DAC, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer16(valor);
  SPI.endTransaction();
  digitalWrite(PIN_CS_DAC, HIGH);
}
