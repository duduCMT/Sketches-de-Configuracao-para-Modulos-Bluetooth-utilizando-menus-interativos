/* 
 * Sketch: Modulo Bluetooth JDY-09 - Config Commands.
 * Autor: Eduardo Lima Costa.
 */

#include <SoftwareSerial.h>

//Defina os Pinos RX e TX do Módulo
#define PIN_RX 2
#define PIN_TX 3
SoftwareSerial mySerial(PIN_RX, PIN_TX);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  sendCommand("AT");              //Verifica a Comunicação com o Módulo  
  sendCommand("AT+NAMExyz");      //Define o nome do Modulo como xyz
  sendCommand("AT+PIN000000");    //Define a senha do Módulo como 000000 (Senha de 6 Digitos)
  sendCommand("AT+TYPE2");        //Ativação da solicitação de Senha
  sendCommand("AT+ROLE0");        //Define o Módulo como periférico
  sendCommand("AT+UUID0xFFE0");   //Define o UUID de serviso do Módolo como 0xFFE0
  sendCommand("AT+CHAR0xFFE1");   //Define o UUID de caracterisação do Módulo como 0xFFE1
  //sendCommand("AT+RESET");        //Restaura as configurações do Módulo
}

void loop() {
  //
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
