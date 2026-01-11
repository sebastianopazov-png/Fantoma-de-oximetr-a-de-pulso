#include <SoftwareSerial.h>

#define RS485_DE_PIN  2   // Pin DE/RE del módulo MAX485
SoftwareSerial rs485Serial(10, 11); // RX=10, TX=11

void setup() {
  Serial.begin(9600);
  rs485Serial.begin(9600);
  pinMode(RS485_DE_PIN, OUTPUT);
  digitalWrite(RS485_DE_PIN, LOW); // Modo recepción inicial

  Serial.println("Arduino UNO listo. Enviar comandos:");// Comandos iniciales que salen en el monitor serial
  Serial.println("Ej: S95 para SpO2=95, B70 para BPM=70"); // Con estos parametros se envian al ESP32
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    sendRS485Command(command);
  }
}

void sendRS485Command(String cmd) {
  digitalWrite(RS485_DE_PIN, HIGH); // Habilitar transmisión
  Serial.print("Enviando: ");
  Serial.println(cmd);
  rs485Serial.print(cmd);
  rs485Serial.println();  // Terminar con salto de línea
  delay(10);              // Esperar antes de volver a recepción
  digitalWrite(RS485_DE_PIN, LOW); // Volver a modo recepción
}