
// Bibliotecas Utilizadas

#include <SPI.h> //para o módulo micro-SD 
#include <SD.h> // para o micro-SD 
#include <DHT.h> // para o sensor DHT 
#include <RTClib.h> // para o RTC

//Pino definido para leitura do sensor DHT 

#define DHTPIN 2 // Qual tipo de sensor será utilizado

#define DHTTYPE DHT11  // O tipo de sensor utilizado

// Incializar o sensor DHT em 16mhz 

DHT dht(DHTPIN, DHTTYPE);

// Pino definido para a leitura do módulo micro SD 

const int chipSelect = 4;

// Criar um documento para armazenamento 

File myFile;

// Relógio 

RTC RTC_DS1307 rtc;

void setup () {
//inicializar o sensor DHT11 

dht.begin();

//inicializar o montior serial 

Serial.begin(9600);

// Configurar o RTC 

while(! Serial);

if(!rtc.begin()) {

Serial.println("Não foi encontrado o RTC"); 
while (1);
}

else {

rtc.adjust(DateTime(F(	DATE	), F(  TIME  )));
}
if(! rtc.isrunning()) {
Serial.println("RTC não está funcionado!");
}

// Configura o cartão micro SD 

Serial.print("Inicializando o SD...");

if(!SD.begin(chipSelect)) { 
    Serial.println("Erro na inicialização!"); 
    return;
}

Serial.println("inicialização realizada.");

//Abrir o documento 

myFile=SD.open("DATA.txt", FILE_WRITE);

// Se o documento abrir ok, escrever:

if (myFile) { 
    
    Serial.println("Documento aberto");

// Como será mostrado no documento de texto// 
    
    myFile.println("Data,Horas,Temperatura ºC,Umidade relativa do Ar %");
}

myFile.close();

}

void loggingTime() { 

    DateTime now = rtc.now();
    myFile = SD.open("DATA.txt", FILE_WRITE); 
    if (myFile) {
    myFile.print(now.year(), DEC); 
    myFile.print('/'); 
    myFile.print(now.month(), DEC); 
    myFile.print('/'); 
    myFile.print(now.day(), DEC); 
    myFile.print(','); 
    myFile.print(now.hour(), DEC); 
    myFile.print(':'); 
    myFile.print(now.minute(), DEC); 
    myFile.print(':'); 
    myFile.print(now.second(), DEC); 
    myFile.print(",");
}

Serial.print(now.year(), DEC); 
Serial.print('/'); 
Serial.print(now.month(), DEC); 
Serial.print('/'); 
Serial.println(now.day(), DEC); 
Serial.print(now.hour(), DEC); 
Serial.print(':'); 
Serial.print(now.minute(), DEC); 
Serial.print(':'); 
Serial.println(now.second(), DEC); 
myFile.close();
delay(1000);

}

void loggingTemperatureandHumidity() {

// Leitura da Temperatura ou Umidade relativa do Ar em 250 millisegundos! 

float h =dht.readHumidity();

// Leitura da Temperatura em Celsius 

float t = dht.readTemperature();

// Verificar se ocorreu alguma falha (Tentar novamente!). 

if (isnan(h) || isnan(t)) {

    Serial.println(F("A leitura do sensor DHT11 falhou!")); 
    return;
}

Serial.print("Temperatura: "); 
Serial.print(t);
Serial.println(" *C"); 
Serial.print("Umidade relativa do ar:"); 
Serial.print(h);
Serial.println("%");
myFile = SD.open("DATA.txt", FILE_WRITE); 

if (myFile) {
    
    Serial.println("Aberto com sucesso");
    myFile.print(t);
    myFile.print(","); 
    myFile.print(h); 
    myFile.print(",");
}
myFile.close();

}
void loop() { 
    
    loggingTime();
    loggingTemperatureandHumidity(); delay(5000);
}
