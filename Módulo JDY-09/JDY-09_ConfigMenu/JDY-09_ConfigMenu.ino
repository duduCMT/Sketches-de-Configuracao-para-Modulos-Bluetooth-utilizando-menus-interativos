
/* 
 * Sketch: Módulo Bluetooth JDY-09 - Config Menu.
 * Autor: Eduardo Lima Costa.
 */

#include <SoftwareSerial.h>

//Defina os Pinos RX e TX do Módulo
#define PIN_RX 2
#define PIN_TX 3
SoftwareSerial mySerial(PIN_RX, PIN_TX);

boolean naoImprimir = false;
void menu();

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(1500);
}

void loop() {
  menu();
}

void menu(){
  if(naoImprimir == false){
    Serial.println(F("Sketch de Configuração do Módulo Bluetooth JDY-09 via Menu Serial"));
    Serial.println(F("  1- Verificar Comunicação com o Módulo"));
    Serial.println(F("  2- Modificar Nome do Módulo"));
    Serial.println(F("  3- Modificar PIN do Módulo"));
    Serial.println(F("  4- Modificar Solicitação de Senha do Módulo"));
    Serial.println(F("  5- Modificar Modo de uso do Módulo"));
    Serial.println(F("  6- Modificar UUID de serviço do Módulo"));
    Serial.println(F("  7- Modificar UUID de caracterisação do Módulo"));
    Serial.println(F("  8- Resetar Configurações do Módulo")); 
  }
  
  while(!Serial.available()){}
  int i = Serial.parseInt();

  if(i != 0){
    Serial.print(F("Você digitou: "));
    Serial.println(i);
  }else{
    Serial.println(F("..."));
  }
  Serial.println(F(""));

  if(i == 1){
    sendCommand("AT");  //Verifica a Comunicação com o Módulo            
    delay(1000);
    
  } else if(i == 2){
    Serial.println(F("Digite o novo Nome... "));
    while(!Serial.available()){}
    String nome = leStringSerial();
    
    buildAndSendComand("AT+NAME", nome);
    delay(1000);
    
  } else if(i == 3){
    boolean tenteNovamente = true;
    while(tenteNovamente){
      Serial.println(F("Digite o novo PIN..."));
      while(!Serial.available()){}
      String pin = leStringSerial(); 
      
      if(pin.length() != 6){
        delay(1000);
        Serial.println(F("ERRO - Sua Senha deve ter 6 Digitos, tente novamente..."));
      } else {
        tenteNovamente = false;
        buildAndSendComand("AT+PIN", pin);
        delay(1000);
        break;
      }
    }
  } else if(i == 4){
    Serial.println(F("Você deseja que seu Módulo Solicite uma Senha (PIN) para se Conectar?"));
    Serial.println(F("  1- SIM"));
    Serial.println(F("  2- NÂO"));      
    Serial.println(F("(Digite -1 para Cancelar a ação e Voltar ao Menu)\n"));
    while(!Serial.available()){}
    int resposta = Serial.parseInt();

    switch(resposta){
      case -1:
        break;
      case 1:
        sendCommand("AT+TYPE2");
        break;
      case 2:
        sendCommand("AT+TYPE0");
        break;
      default:
        Serial.println("Resposta Invalida\n");
        break;
    }
    delay(1000);
    
  } else if(i == 5){
    Serial.println(F("Como você deseja que seu Módulo se Comporte?"));
    Serial.println(F("  1- PERIFERICO"));
    Serial.println(F("  2- CENTRAL"));
    Serial.println(F("(Digite -1 para Cancelar a ação e Voltar ao Menu)\n"));      
    while(!Serial.available()){}
    int resposta = Serial.parseInt();

    switch(resposta){
      case -1:
        break;
      case 1:
        sendCommand("AT+RULE0");
        break;
      case 2:
        sendCommand("AT+RULE1");
        break;
      default:
        Serial.println(F("Resposta Invalida\n"));
        break;
    }
    delay(1000);
  
  } else if(i == 6){
    Serial.println(F("Digite o novo UUID de serviço para o seu Módulo... (Hexadecimal)"));
    Serial.println(F("(Caso o valor fornecido não for em Hexadecimal, seu módulo não será configurado!)"));
    while(!Serial.available()){}
    
    String uuidS = leStringSerial(); 
    buildAndSendCommand("AT+UUID", uuidS);
    delay(1000);
  
  } else if(i == 7){
    Serial.println(F("Digite o novo UUID de caracterização para o seu Módulo... (Hexadecimal)"));
    Serial.println(F("(Caso o valor fornecido não for em Hexadecimal, seu módulo não será configurado!)"));
    while(!Serial.available()){}
    
    String uuidC = leStringSerial();
    buildAndSendCommand("AT+UUID", uuidC);
    delay(1000);
    
  } else if(i == 8){
    Serial.println(F("Resetando Configurações do Módulo...\n"));
    sendCommand("AT+RESET");
    delay(1000);
      
  }
}

String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
    
  Serial.print("Você digitou: ");
  Serial.println(conteudo);
  Serial.println("");
    
  return conteudo;
}
  
void sendCommand(const char * command){
  Serial.print("Command send: ");
  Serial.println(command);
  mySerial.println(command);
  
  delay(100);
  
  char reply[100];
  int i = 0;
  
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  
  reply[i] = '\0';
  Serial.print(reply);
  Serial.println("Reply end\n");
}

void buildAndSendComand(String comandoAT, String complemento){
  //Constroi o comando em formato de String
  String comando = (comandoAT + complemento);
  
  //Converte a String em const char para usar no método sendCommand
  const char __comando[comando.length()+1];
  comando.toCharArray(__comando, sizeof(__comando));

  //Envia o Comando em cost char para o metodo sendCommand 
  sendCommand(__comando);
}
