#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>

// Configuração do Display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C e tamanho do display

// Configuração do Sensor de Umidade de Solo
const int sensorSoloPin = A0;

// Configuração do Sensor de Chuva
const int sensorChuvaPin = 2;

// Configuração do Módulo Relé
const int relePin = 3;

// Configuração da Mini Bomba de Água
const int bombaPin = 4;

// Configuração do ThingSpeak
char ssid[] = "SEU_SSID_WIFI";
char pass[] = "SUA_SENHA_WIFI";
char server[] = "api.thingspeak.com";
String apiKey = "SUA_CHAVE_API_THINGSPEAK";
WiFiEspClient client;

// Configuração do intervalo de leitura e envio para o ThingSpeak (em milissegundos)
const unsigned long intervalLeitura = 60000; // 1 minuto

// Variáveis globais
unsigned long previousMillis = 0;

// Configuração do Sensor de Chuva
bool estaChovendo() {
  return digitalRead(sensorChuvaPin) == LOW;
}

// Configuração do Sensor de Umidade de Solo
int lerUmidadeSolo() {
  return analogRead(sensorSoloPin);
}

// Configuração da Bomba de Água
void acionarBomba(bool ligar) {
  digitalWrite(bombaPin, ligar ? HIGH : LOW);
}

// Configuração do WiFi
void conectarWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando ao WiFi...");
    WiFi.begin(ssid, pass);
    delay(5000);
  }
  Serial.println("Conectado ao WiFi com sucesso!");
}

// Configuração do ThingSpeak
void enviarDadosParaThingSpeak(int umidadeSolo, bool irrigando) {
  if (client.connect(server, 80)) {
    String dados = "field1=" + String(umidadeSolo) + "&field2=" + String(irrigando);
    String requisicao = "POST /update HTTP/1.1\n";
    requisicao += "Host: " + String(server) + "\n";
    requisicao += "Connection: close\n";
    requisicao += "X-THINGSPEAKAPIKEY: " + apiKey + "\n";
    requisicao += "Content-Type: application/x-www-form-urlencoded\n";
    requisicao += "Content-Length: " + String(dados.length()) + "\n\n";
    requisicao += dados;
    
    client.print(requisicao);
    Serial.println("Enviando dados para ThingSpeak...");
    delay(100);
    client.stop();
  } else {
    Serial.println("Falha ao conectar-se ao ThingSpeak.");
  }
}

void setup() {
  Serial.begin(9600);

  // Inicialização do Display LCD
  lcd.begin(16, 2);
  lcd.print("Umidade Solo:");
  lcd.setCursor(0, 1);
  lcd.print("Irrigando: Não");

  // Inicialização do Sensor de Chuva
  pinMode(sensorChuvaPin, INPUT);

  // Inicialização do Módulo Relé e da Bomba de Água
  pinMode(relePin, OUTPUT);
  pinMode(bombaPin, OUTPUT);

  acionarBomba(false);

  // Inicialização do WiFi
  WiFi.init(&Serial);
  conectarWiFi();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= intervalLeitura) {
    int umidadeSolo = lerUmidadeSolo();
    bool chuva = estaChovendo();

    lcd.setCursor(13, 0);
    lcd.print("   ");
    lcd.setCursor(13, 0);
    lcd.print(umidadeSolo);

    if (chuva) {
      lcd.setCursor(0, 1);
      lcd.print("Chuva detectada ");
      acionarBomba(false);
      enviarDadosParaThingSpeak(umidadeSolo, false);
    } else {
      if (umidadeSolo < 500) { // Ajuste o valor de acordo com o seu sensor e condições do solo
        lcd.setCursor(0, 1);
        lcd.print("Irrigando: Sim  ");
        acionarBomba(true);
        enviarDadosParaThingSpeak(umidadeSolo, true);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Irrigando: Não  ");
        acionarBomba(false);
        enviarDadosParaThingSpeak(umidadeSolo, false);
      }
    }

    previousMillis = currentMillis;
  }
}
