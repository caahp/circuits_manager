//=========================================
const byte numChars = 64;
char receivedChars[numChars];
char tempChars[numChars]; // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int mensagemR = 0;
int mensagemG = 0;
int mensagemB = 0;
boolean newData = false;

//=========================================
int r = 0;
int g = 0;
int b = 0;

const int redPin = 8;   
const int greenPin = 7;
const int bluePin = 6; 

void acionaLED();
void recvWithStartEndMarkers();
void parseData();
void showParsedData();

void setup(){
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("End Setup");

}

void loop(){
  //função que recebe e atribui as mensagens às variáveis de velocidade
  acionaLED();
//  showParsedData();
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // fim da string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
//=========================================

void parseData() {      // split the data into its parts
char copy_receivedChars[sizeof(receivedChars)] = "";
  strcpy(copy_receivedChars, receivedChars);
  char * strtokIndx; // Índice de srttok()
  strtokIndx = strtok(copy_receivedChars, ",");    // Pegar primeira parte (String)
  strcpy(messageFromPC, strtokIndx);      // Armazenar em messageFromPC
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  mensagemR = atoi(strtokIndx);     // convert this part to an integer
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  mensagemG = atoi(strtokIndx);     // convert this part to an integer
  strtokIndx = strtok(NULL, ",");
  mensagemB = atoi(strtokIndx);     // convert this part to a integer
}

void acionaLED(){
recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    newData = false;
  }

  //Espera receber mensagem carrinho, velocidade e ângulo
  if (strcmp(messageFromPC, "LED") == 0) {
    r = mensagemR;
    g = mensagemG;
    b = mensagemB;
  }

    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

//=========================================

//
//void showParsedData() {
//  Serial.print("R: ");
//  Serial.print(r);
//  Serial.print("  || G: ");
//  Serial.print(g);
//  Serial.print("  ||  B: ");
//  Serial.println(b);
//}
