#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

// Pinos para sensores
const int pinUmidadeSolo = A0;   // Sensor de umidade do solo
const int pinPHSolo = A1;        // Sensor de pH do solo
const int pinECSolo = A2;        // Sensor de condutividade elétrica do solo
const int pinTemperaturaSolo = A3; // Sensor de temperatura do solo

// Pinos do LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Arquivo no cartão SD
File dataFile;

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Inicializa o LCD
  lcd.begin(16, 2);

  // Inicializa o cartão SD
  if (!SD.begin(10)) {
    Serial.println("Falha ao inicializar o cartão SD!");
    lcd.print("Erro no SD!");
    return;
  }
  Serial.println("Cartão SD pronto.");
  lcd.print("Cartao SD pronto!");

  // Abre o arquivo para escrita (se existir, será substituído)
  dataFile = SD.open("diagnostico_solo.txt", FILE_WRITE);
  
  if (dataFile) {
    // Escreve cabeçalho no arquivo
    dataFile.println("### Diagnóstico do Solo ###");
    dataFile.println();
    dataFile.close(); // Fecha o arquivo
    Serial.println("Arquivo criado com sucesso.");
  } else {
    Serial.println("Erro ao criar o arquivo.");
    lcd.print("Erro no arquivo!");
  }

  // Limpa o LCD após 3 segundos
  delay(3000);
  lcd.clear();
}

void loop() {
  // Leitura dos sensores
  float umidadeSolo = lerUmidadeSolo();
  float pHSolo = lerPHSolo();
  float ecSolo = lerECSolo();
  float temperaturaSolo = lerTemperaturaSolo();

  // Exibe os valores no LCD
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidadeSolo);
  
  lcd.setCursor(0, 1);
  lcd.print("pH: ");
  lcd.print(pHSolo);

  // Aguarda um intervalo antes de ler novamente
  delay(10000); // Por exemplo, a cada 10 segundos
}

// Função para ler umidade do solo
float lerUmidadeSolo() {
  int valorSensor = analogRead(pinUmidadeSolo); // Leitura do sensor de umidade do solo

  // Convertendo o valor lido para umidade percentual
  float umidadePercentual = map(valorSensor, 0, 1023, 0, 100); // Mapeia para uma faixa de 0 a 100%

  return umidadePercentual;
}

// Função para ler pH do solo
float lerPHSolo() {
  int valorSensor = analogRead(pinPHSolo); // Leitura do sensor de pH do solo

  // Convertendo o valor lido para pH 
  float pH = valorSensor * (14.0 / 1023.0); // Conversão linear

  return pH;
}

// Função para ler condutividade elétrica do solo
float lerECSolo() {
  int valorSensor = analogRead(pinECSolo); // Leitura do sensor de condutividade elétrica do solo

  // Convertendo o valor lido para microsiemens por centímetro (µS/cm)
  float condutividade = map(valorSensor, 0, 1023, 0, 2000); // Mapeia para uma faixa de 0 a 2000 µS/cm 

  return condutividade;
}

// Função para ler temperatura do solo
float lerTemperaturaSolo() {
  int valorSensor = analogRead(pinTemperaturaSolo); // Leitura do sensor de temperatura do solo

  // Convertendo o valor lido para graus Celsius
  float tensao = valorSensor * (5.0 / 1023.0); // Convertendo a leitura para tensão 
  float temperaturaC = (tensao - 0.5) * 100.0; //  Fórmula para conversão para temperatura em Celsius

  return temperaturaC;
}
