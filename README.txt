# Irrigador Automático com Arduino

Este projeto consiste em um sistema de irrigação automático utilizando um Arduino e sensores para medir a umidade do solo e detectar a presença de chuva. O Arduino controlará uma bomba de água e enviará os dados de umidade do solo e o status da irrigação para o ThingSpeak, uma plataforma de IoT.

## Componentes Necessários

- Arduino UNO ou similar
- Módulo Sensor de Umidade de Solo
- Módulo Sensor de Chuva
- Módulo Relé 5V 10A
- Mini Bomba de Água para Arduino 12V RS385 2L/Min
- Display LCD 16x2 com interface I2C
- Jumpers e cabos para conexão

## Bibliotecas Necessárias

- LiquidCrystal_I2C: Para controlar o display LCD com interface I2C.
- DHT: Para obter leituras do sensor de umidade de solo.
- WiFiEsp: Para conectar o Arduino à rede WiFi e enviar dados para o ThingSpeak.

## Conexões

Realize as conexões entre os componentes da seguinte maneira:

- Conecte o Módulo Sensor de Umidade de Solo ao pino analógico A0.
- Conecte o Módulo Sensor de Chuva ao pino digital 2.
- Conecte o Módulo Relé ao pino digital 3.
- Conecte a Mini Bomba de Água ao Módulo Relé, que controlará a alimentação da bomba.
- Conecte o Display LCD ao I2C do Arduino (pino SDA ao A4 e pino SCL ao A5).
- Alimente os módulos e sensores conforme as especificações de cada um.

## Configuração

Antes de fazer o upload do código para o Arduino, você precisará configurar algumas informações no código:

1. Configure as credenciais da sua rede WiFi:

   ```cpp
   char ssid[] = "SEU_SSID_WIFI";
   char pass[] = "SUA_SENHA_WIFI";
   ```

2. Obtenha uma chave de API do ThingSpeak e configure-a:

   ```cpp
   String apiKey = "SUA_CHAVE_API_THINGSPEAK";
   ```

## Funcionamento

O sistema irá medir periodicamente a umidade do solo e verificar a presença de chuva. Com base nas leituras, ele decidirá se é necessário irrigar as plantas.

- Se o sensor de chuva detectar chuva, a irrigação será desativada, e o status será enviado ao ThingSpeak.

- Caso não haja chuva, o sistema verificará o valor da umidade do solo. Se a umidade estiver abaixo de um certo limite (ajustável no código), a bomba de água será acionada para irrigar as plantas. O status da irrigação também será enviado ao ThingSpeak.

O display LCD fornecerá informações sobre a umidade do solo e se a irrigação está acontecendo ou não.

## Instalação e Execução

1. Faça as conexões conforme descrito na seção "Conexões".

2. Abra o Arduino IDE e instale as bibliotecas necessárias (LiquidCrystal_I2C, DHT, WiFiEsp).

3. Configure o código conforme descrito na seção "Configuração".

4. Faça o upload do código para o Arduino.

5. O display LCD exibirá informações sobre a um

idade do solo e o status da irrigação. Os dados também serão enviados ao ThingSpeak e podem ser visualizados em uma planilha.

## Aviso

Este projeto é apenas um exemplo básico e pode não abranger todas as situações ou condições de uso. Verifique se todas as conexões e componentes estão corretamente instalados e funcionando adequadamente. O autor não se responsabiliza por quaisquer danos ou prejuízos causados pelo uso deste projeto.