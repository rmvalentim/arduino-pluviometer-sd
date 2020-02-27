#include <SPI.h>
#include <SD.h>

File myFile;

// Porta digital do sensor pluviometro
const int REED = 9;              
 
// Valor atual do reed switch
int val = 0;  
// Valor anterior do reed switch
int old_val = 0;               
// Armazena a quantidade de medições realizadas
int REEDCOUNT = 0; 
// Armazena o tempo desde o inicio da execução
unsigned long time;
 
 
void setup() {
  // initializa o pino do switch como entrada
  pinMode (REED, INPUT_PULLUP); 
  
  // Inicia a a comunicaçao serial:
  Serial.begin(9600); 
  
  Serial.println("Inicializando SD Card...");

  if (!SD.begin(4)) {
    Serial.println("Falha na inicialização do SD Card!");   
  }
  Serial.println("SD Card inicializado com sucesso!");
 
}
 
void loop() {
  // Realiza leitura do estado do switch pelo pino de entrada
  val = digitalRead(REED);      

  // Verifica se o estado do reed switch se alterou 
  // ou seja, se houve leitura de chuva
  if ((val == LOW) && (old_val == HIGH)){    

    // Delay para garantir a unicidade da leitura do reed switch
    delay(10);                   
  
    // Aumenta uma contagem no acumulador de medições
    REEDCOUNT = REEDCOUNT + 1;
    // Valor anterior recebe valor atual, reseta estado do reed switch
    old_val = val; 

    time = millis();

    // Registra as medições na saída serial
    Serial.print("Segundos desde a inicialização: ");
    Serial.println(time/1000);
    Serial.print("Medida de chuva (contagem): ");
    Serial.print(REEDCOUNT);
    Serial.println(" pulso");
    Serial.print("Medida de chuva (calculado): ");
    Serial.print(REEDCOUNT*0.25); 
    Serial.println(" mm"); 

    // Grava as medições no SD card
    if (SD.begin(4)) {
      // Abre o arquivo dados.txt
      myFile = SD.open("dados.txt", FILE_WRITE);

      if (myFile) {
        Serial.println("Gravando medições no SD Card...");
        myFile.print("Segundos desde a inicialização: ");
        myFile.print(time/1000);
        myFile.print(" - Medida de chuva (calculado): ");
        myFile.println(REEDCOUNT*0.25);
        // Fecha o arquivo dados.txt
        myFile.close();
        Serial.println("Gravação finalizada com sucesso.");
      } else {
        // Se o arquivo não puder ser aberto, mensagem de erro é exibida.
        Serial.println("Erro ao abrir arquivo dados.txt");
      }
    }    
      
  } else {
  // Se o status não se alterou, nada é feito
  old_val = val;
  }
}
