#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

const int ledRojo = 25; //Pin para encender diodo Rojo
const int ledIR   = 26; // pin para encender diodo IR

//CONFIGURACIÓN PRINCIPAL 
float SpO2_target = 90.0;      // En este parametro se configura el oxigeno del fantoma. Esta pensado para funcionar entre 90 a 100%
int BPM = 90;                  // Ritmo cardíaco (60–100)

//Parámetros LED 
const int LED_INTENSITY = 100; // Intensidad con la cual se encienden los diodos tanto rojo como IR
const int LED_BIAS      = 40; // Intensidad base con la cual quedan los diodos, para evitar que se pierda la señal.

//Parámetros morfológicos PPG 
float systolicSharpness   = 1.2; // Valor para la formula de Ascenso Sistolico
float dicroticNotchDepth  = 0.4; // Valor para profundidad en muesca dicrotica
float diastolicPeakHeight = 0.8; // Altura del pico diastolico

const int pointsPerCycle = 200; //Cantidad de puntos en cada "latido", con 100 falla en bajos BPM
float x = 0;
bool isRed = true;
unsigned long lastUpdate = 0;

// Variables globales de ratio (calculadas en setup/loop según SpO2_target)
float R_red, R_ir;

//CONFIGURACIÓN RS485 
#define RS485_DE_PIN  4  // Pin DE/RE del módulo MAX485 (conectado a GPIO17 del ESP32)
#define RS485_RE_PIN  4  // Misma pin para DE y RE (normalmente se conectan juntos)
HardwareSerial RS485Serial(2); // Usamos Serial2 (UART2) del ESP32

String receivedCommand = "";

//FUNCIÓN CALIBRADA PARA MEDITECH G3H
void updateRatiosFromSpO2(float spo2) { // Debido a que no se encontro una formula en la cual pueda simular de manera adecuada el SpO2
  if (spo2 >= 100.0) {// Los valores se crearon de manera manual, de manera de prueba y error.
    R_red = 0.35;
    R_ir  = 0.05;
  }
  else if (spo2 == 99.0) {
    R_red = 0.01;
    R_ir  = 1.2;
  }
  else if (spo2 == 98.0) {
    R_red = 0.4;
    R_ir  = 1.1;
  }
  else if (spo2 == 97.0) {
    R_red = 0.31;
    R_ir  = 1.0;
  }
  else if (spo2 == 96.0) {
    R_red = 0.2;
    R_ir  = 0.8;
  }
  else if (spo2 == 95.0) {
    R_red = 0.3;
    R_ir  = 1.77;
  }
  else if (spo2 == 94.0) {
    R_red = 0.38;
    R_ir  = 1.9;
  }
  else if (spo2 == 93.0) {
    R_red = 0.38;
    R_ir  = 1.85;
  }
  else if (spo2 == 92.0) {
    R_red = 0.28;
    R_ir  = 1.85;
  }
  else if (spo2 == 91.0) {
    R_red = 0.80;
    R_ir  = 1.85;
  }
  else if (spo2 == 90.0) {
    R_red = 0.1;
    R_ir  = 0.8;
  }
  else {
    R_red = 0.8;
    R_ir  = 0.05;
  }

  if (spo2 >= 95.0) {
    systolicSharpness = 1.2;
    dicroticNotchDepth = 0.4;
    diastolicPeakHeight = 0.8;
  } else if (spo2 >= 90.0) {
    systolicSharpness = 1.2;
    dicroticNotchDepth = 0.4;
    diastolicPeakHeight = 0.8;
  } else {
    systolicSharpness = 1.4;
    dicroticNotchDepth = 0.02;
    diastolicPeakHeight = 0.05;
  }

  // Limitar rangos físicos
  R_red = constrain(R_red, 0.01, 5.0);
  R_ir  = constrain(R_ir,  0.05, 5.0);
}

//FUNCIÓN: Calcular intervalo de muestreo
unsigned long getPointInterval() {
  float beatsPerSecond = BPM / 60.0;
  return (unsigned long)(1000000.0 / (beatsPerSecond * pointsPerCycle));
}

//SETUP
void setup() {
  Serial.begin(115200);

  // Configurar RS485
  pinMode(RS485_DE_PIN, OUTPUT);
  digitalWrite(RS485_DE_PIN, LOW); // Poner en modo recepción
  RS485Serial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17 (pines GPIO del ESP32)
  Serial.println("Esperando comandos...");

  Wire.begin(21, 22);
  if (!dac.begin(0x60)) {
    Serial.println("DAC no encontrado");
    while (1);
  }
  pinMode(ledRojo, OUTPUT); //Determinar pin para su funcion de "salida"
  pinMode(ledIR,   OUTPUT); //Determinar pin para su funcion de "salida"
  lastUpdate = micros();

  // Inicializar ratios según SpO2_target
  updateRatiosFromSpO2(SpO2_target);
  Serial.print("Configurado para SpO2 = "); Serial.print(SpO2_target);
  Serial.print("% → R_red = "); Serial.print(R_red);
  Serial.print(", R_ir = "); Serial.println(R_ir);
}

//FUNCION: PROCESAR COMANDOS RECIBIDOS
void processCommand(String cmd) {// Recepcion de la imformacion desde el Arduino UNO
  if (cmd.startsWith("S")) {
    float newSpo2 = cmd.substring(1).toFloat();
    if (newSpo2 >= 80 && newSpo2 <= 100) { //Se coloca un parametro que vaya desde los 80 a 100% de SpO2
      SpO2_target = newSpo2;
      updateRatiosFromSpO2(SpO2_target);
      Serial.print("Nuevo SpO2: "); //Mensae para saber que se efectuo el cambio
      Serial.println(SpO2_target);
    }
  } else if (cmd.startsWith("B")) {
    int newBPM = cmd.substring(1).toInt();
    if (newBPM >= 40 && newBPM <= 180) {//Se coloca un parametro que vaya desde los 40 a 180 de BPM, aunque la version 4.0 esta pensado para funcionar entre 60 a 100 BPM
      BPM = newBPM;
      Serial.print("Nuevo BPM: ");//Mensae para saber que se efectuo el cambio
      Serial.println(BPM);
    }
  }
}

// LOOP 
void loop() {
  // Leer comandos RS485
  if (RS485Serial.available()) {
    char c = (char)RS485Serial.read();
    if (c == '\n' || c == '\r') {
      if (receivedCommand.length() > 0) {
        processCommand(receivedCommand);
        receivedCommand = "";
      }
    } else {
      receivedCommand += c;
    }
  }



  unsigned long now = micros();
  if (now - lastUpdate >= getPointInterval()) {
    lastUpdate = now;
    isRed = !isRed;

    // Alternar LEDs (modulación de 2 fases)
    if (isRed) {
      analogWrite(ledRojo, LED_INTENSITY);
      analogWrite(ledIR,   LED_BIAS);
    } else {
      analogWrite(ledRojo, LED_BIAS);
      analogWrite(ledIR,   LED_INTENSITY);
    }

    // USO DE RATIOS CALIBRADOS
    float amplitude = isRed ? R_red : R_ir;

    // Generar forma de onda PPG
    float t = x / TWO_PI;
    float systolic_end    = 0.28 + 0.0002 * (100 - BPM);
    float notch_start     = 0.40 + 0.0002 * (100 - BPM);
    float notch_end       = 0.58 + 0.0002 * (100 - BPM);
    float diastolic_start = 0.60 + 0.0003 * (100 - BPM);

    float systolic = (t < systolic_end) //Formula para poder hacer la onda Sistolica
                     ? pow(t / systolic_end, systolicSharpness)
                     : exp(-6.0 * (t - systolic_end));

    float notch = 0.0;
    if (t > notch_start && t < notch_end) { // Formula para hacer la onda dicrotica
      float u = (t - notch_start) / (notch_end - notch_start);
      float shape = u * (1.0 - u);
      notch = -dicroticNotchDepth * shape;
    }

    float diastolic = 0.0;
    if (t > diastolic_start && t < 0.85) { //Formula para hacer la onda Diastolica
      float c = diastolic_start + 0.08;
      float w = 0.04;
      diastolic = diastolicPeakHeight * exp(-sq((t - c) / w));
    }

    float rawPPG = systolic + notch + diastolic;// Suma de las 3 formulas y con ello tener la PPG completa
    float ac = (rawPPG - 0.52) / 0.50;
    ac = constrain(ac, -1.0, 1.0);

    // SEÑAL DAC EN MODO DIRECTO (validado para G3H)
    float signal = 0.65 - 0.25 * amplitude * ac;
    signal = constrain(signal, 0.15, 0.92);  // Evita saturación

    // SALIDA DIRECTA AL DAC (más luz → más voltaje)
    dac.setVoltage((int)(signal * 4095), false); // Valores para el DAC

    x = fmod(x + (TWO_PI / pointsPerCycle), TWO_PI);
  }
}