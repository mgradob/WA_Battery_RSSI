/* CODDIGO FINAL REDES PARA LA AUTOMATIZACION MAYO 2014
   AUTOR: MARIO EDGAR RAMIREZ PALOMO */
  
int analogPin = 3; //Entrada analogica 3 (A3)
int val = 0; //Valor para recibir la lectura de la señal analogica
byte paquete[21]; //declaramos un array para la estructura
byte paquete2[8]; //array para AT command
byte response[10];

void setup()
{
  Serial.begin(9600); //Inicializacion del modulo serial
}

void loop()
{
  val = analogRead(analogPin); //val guarda la lectura de la entrada analogica
  delay(5000); //Delay de 1 segundo
  arreglo(); //Llamamos al metodo arreglo()
  transmitir(); //Llamamos al metodo transmitir()
  AtCmd();
  TransmitirAt();
  leerAt();
}

void arreglo() //Este metodo forma el paquete en formato API
{
  int chk = 0; //variable para guardar la suma de la localidad 3 - 19
  int checks = 0, checksum = 0; //chacks = guarda el valor de la suma despues del bitwise, checksum = guarda el checksum total
  int valH = 0, valL = 0; //variables para el payload
  int bitWH = 786, bitWL = 255, bitWC = 255; //constantes para el bitwise
  
  valH = val & bitWH; //bitwise del valor analogico para obtener la parte alta
  valH = valH >> 8; //shifting del valor 8 posiciones
  valL = val & bitWL; //bitwise del valor analogico para obtener la parte baja
  
  paquete[0] = 0x7E; //Delimitador
  paquete[1] = 0x00; //MSB Length
  paquete[2] = 0x11; //LSB Length, la longitud es de 16 bytes
  paquete[3] = 0x10; //Frame type: 0x10 = transmitter
  paquete[4] = 0x00; //0x00 = no ACK is sent
  paquete[5] = 0x00; //MSB Destination Adress
  paquete[6] = 0x13;
  paquete[7] = 0xA2;
  paquete[8] = 0x00;
  paquete[9] = 0x40;
  paquete[10] = 0xA8;
  paquete[11] = 0x50;
  paquete[12] = 0x6E; //LSB Destination Adress
  paquete[13] = 0xFF; //Reserved
  paquete[14] = 0xFE; //Reserved
  paquete[15] = 0x00; //Broadcast Radious: 0x00 = radious equals max hops value
  paquete[16] = 0x00; //ACK desactivado
  paquete[17] = valH; //Valor alto del payload
  paquete[18] = valL; //Valor bajo del payload
  paquete[19] = response[8]; //Valor RSSI
   
  for(int k=3; k<20; k++) //ciclo de suma
  {
    chk = chk + paquete[k];
  }
  checks = chk & bitWC; //bitwise para obtener la parte baja de la suma
  checksum = 255 - checks; //obtenemos el checksum
  
  paquete[20] = checksum; //Valor del Checksum
}

void transmitir() //este metodo transmite el paquete formado
{
  Serial.write(paquete[0]);
  Serial.write(paquete[1]);
  Serial.write(paquete[2]);
  Serial.write(paquete[3]);
  Serial.write(paquete[4]);
  Serial.write(paquete[5]);
  Serial.write(paquete[6]);
  Serial.write(paquete[7]);
  Serial.write(paquete[8]);
  Serial.write(paquete[9]);
  Serial.write(paquete[10]);
  Serial.write(paquete[11]);
  Serial.write(paquete[12]);
  Serial.write(paquete[13]);
  Serial.write(paquete[14]);
  Serial.write(paquete[15]);
  Serial.write(paquete[16]);
  Serial.write(paquete[17]);
  Serial.write(paquete[18]);
  Serial.write(paquete[19]);
  Serial.write(paquete[20]);
}

void AtCmd()
{
  int chk = 0;
  int checksum = 0;
  
  paquete2[0] = 0x7E; //Delimitador
  paquete2[1] = 0x00; //MSB Length
  paquete2[2] = 0x04; //LSB Length, la longitud es de 4 bytes
  paquete2[3] = 0x08; //Frame type: 0x08 = AT Command
  paquete2[4] = 0x52; //(R) Identifies the UART data frame for the host to correlate with a subsequent ACK (acknowledgement).
  paquete2[5] = 0x44; //(D) 1er Byte del comando AT
  paquete2[6] = 0x42; //(B) 2do Byte del comando AT
  
  for(int k = 3; k < 7; k++)
  {
    chk = chk + paquete2[k];
  }
  checksum = 255 - chk; //Obtenemos el Checksum
  
  paquete2[7] = checksum; //Valor del Checksum
}

void TransmitirAt()
{
  Serial.write(paquete2[0]);
  Serial.write(paquete2[1]);
  Serial.write(paquete2[2]);
  Serial.write(paquete2[3]);
  Serial.write(paquete2[4]);
  Serial.write(paquete2[5]);
  Serial.write(paquete2[6]);
  Serial.write(paquete2[7]);
}

void leerAt()
{
  int n = 0;
  for(int k=0; k<10; k++)
  {
    if(Serial.available() > 0)
    {
      response[k] = Serial.read();
      Serial.write(response[k]);
    }
  }
}
