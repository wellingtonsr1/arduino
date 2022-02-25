#include <Servo.h>
#include <UltrasonicSensor.h>

#define enUm 11 
#define motorUmA 9
#define motorUmB 8

#define enDois 5 
#define motorDoisA 7
#define motorDoisB 6

#define trigPin A0
#define echoPin A1
#define setaDireita A2 
#define setaEsquerda A3 
#define luzFreioDireita 10
#define luzFreioEsquerda 12 
#define farolDireito 2 
#define farolEsquerdo 4 
#define sensorLuz A5 

unsigned int distancia, distanciaDireita, distanciaEsquerda;

int velocidadeMotorUm=103;
int velocidadeMotorDois=130;

Servo servoMotor;
UltrasonicSensor ultrasonic(trigPin, echoPin);

void setup(){
  Serial.begin(9600);

  servoMotor.attach(13);
  
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(velocidadeMotorUm, OUTPUT);
  pinMode(velocidadeMotorDois, OUTPUT);
  pinMode(motorUmA, OUTPUT);
  pinMode(motorUmB, OUTPUT);
  pinMode(motorDoisA, OUTPUT);
  pinMode(motorDoisB, OUTPUT);
  pinMode(farolDireito, OUTPUT);
  pinMode(farolEsquerdo, OUTPUT);
   
  analogWrite(velocidadeMotorUm, 0);
  analogWrite(velocidadeMotorDois, 0);
  
  digitalWrite(motorUmA,LOW);
  digitalWrite(motorUmB,LOW);
  digitalWrite(motorDoisA,LOW);
  digitalWrite(motorDoisB,LOW);
}

void loop() {
  int valorSensor = analogRead(sensorLuz);
  int porcentagem = map(valorSensor, 0, 1023, 0, 100);
  
  Serial.print("Luz: ");
  Serial.println(porcentagem);
  
  if(porcentagem > 85){
    farol(HIGH);
  } else {
    farol(LOW);
  }
   
  frente();
  
  distancia = radar();
  if (distancia < 40){ 
    direcao();
  }
} 

int radar(){ 
  distancia = ultrasonic.distanceInCentimeters();
  Serial.print(distancia);
  Serial.println(" cm");
  
  return distancia;
}

void direcao(){
  parar();                                         
  atras();                                      
  parar();   

  Serial.println("Esquerda");                                    
  servoMotor.write(180);                          
  delay(1000);              
  distanciaEsquerda = radar();                   
  delay (500);  

  Serial.println("Direita");
  servoMotor.write (0);                            
  delay(500);               
  distanciaDireita = radar();                   
  delay(500); 

  Serial.println("Frente");
  servoMotor.write(90);                         
  delay(500);    
            
  compararDistancias();  
}

void frente(){
  Serial.println("O Robô está indo para Frente.");

  //farol(HIGH);
  luzDeFreio(LOW, LOW);
  
  digitalWrite(motorUmA, HIGH);
  digitalWrite(motorUmB, LOW);
  
  digitalWrite(motorDoisA, HIGH);
  digitalWrite(motorDoisB, LOW);

  analogWrite(enUm, velocidadeMotorUm);
  analogWrite(enDois, velocidadeMotorDois);
  
  delay(1000); 
}

void parar(){
  Serial.println("O Robô parou.");
  
  luzDeFreio(HIGH, HIGH);
   
  digitalWrite(motorUmA, HIGH);
  digitalWrite(motorUmB, HIGH);

  digitalWrite(motorDoisA, HIGH);
  digitalWrite(motorDoisB, HIGH);

  //analogWrite(enUm, 0);
  //analogWrite(enDois, 0);
  
  delay(500); 
}

void atras(){
  Serial.println("O Robô está dando ré.");
  
  luzDeFreio(LOW, LOW);
  
  digitalWrite(motorUmA,LOW);
  digitalWrite(motorUmB,HIGH);
  
  digitalWrite(motorDoisA, LOW);
  digitalWrite(motorDoisB, HIGH);
  
  delay(500); 
}

void virarEsquerda(){ 
  Serial.println("O Robô está virando à esquerda.");
 
  parar();
  setas('E');
  luzDeFreio(LOW, LOW);
  
  digitalWrite(motorUmA, LOW);
  digitalWrite(motorUmB, HIGH);
  
  //digitalWrite(motorDoisA, HIGH);
  //digitalWrite(motorDoisB, LOW);
  delay(600); 

  analogWrite(enUm, velocidadeMotorUm);
  //analogWrite(enDois, velocidadeMotorDois);
  parar();
}


void virarDireita(){
  Serial.println("O Robô está virando à direita.");
   
  parar();
  setas('D');
  luzDeFreio(LOW, LOW);

  //digitalWrite(motorUmA, HIGH);
  //digitalWrite(motorUmB, LOW);
 
  digitalWrite(motorDoisA, LOW);
  digitalWrite(motorDoisB, HIGH);
  delay(600); 
 
  //analogWrite(enUm, velocidadeMotorUm);
  analogWrite(enDois, velocidadeMotorDois);
  parar();
}

void voltar(){
  Serial.println("O Robô está retornando.");  
      
  digitalWrite(motorUmA, HIGH);                          
  digitalWrite(motorUmB, LOW);
  digitalWrite(motorDoisA, LOW);
  digitalWrite(motorDoisB, HIGH);
  
  delay(1000);      
                                  
  analogWrite(enUm, velocidadeMotorUm);              
  analogWrite(enDois, velocidadeMotorDois);  
}

void compararDistancias(){
  if (distanciaDireita > distanciaEsquerda) {      
    virarDireita();                                
  } else if (distanciaEsquerda > distanciaDireita) {  
    virarEsquerda();                               
  } else {                                           
    voltar();                                     
  }
}

void setas(char direcaoSeta){
  if(direcaoSeta == 'E'){
    for(int i = 0; i < 5; i++){
      digitalWrite(setaEsquerda, HIGH);
      delay(200);
      digitalWrite(setaEsquerda, LOW);
      delay(100);
    }  
  }else if(direcaoSeta == 'D'){
    for(int i = 0; i < 5; i++){
      digitalWrite(setaDireita, HIGH);
      delay(200);
      digitalWrite(setaDireita, LOW);
      delay(100);
    }
  }
}

void farol(int valor){
  digitalWrite(farolDireito, valor);
  digitalWrite(farolEsquerdo, valor);
}

void luzDeFreio(int d, int e){
  digitalWrite(luzFreioDireita, d);
  digitalWrite(luzFreioEsquerda, e);
}
