#include <PS4Controller.h>
#include <L298NX2.h>
#include <TM1637Display.h>

// Définition des ports des moteurs

const unsigned int IN1_A = 12;
const unsigned int IN2_A = 13;
const unsigned int EN_A = 14;

const unsigned int IN1_B = 25;
const unsigned int IN2_B = 26;
const unsigned int EN_B = 27;

int counter = 50; // Score au démarage

#define CLK  22 // The ESP32 pin GPIO22 connected to CLK
#define DIO  23 // The ESP32 pin GPIO23 connected to DIO

L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B); // Initialisation des moteurs principaux
L298NX2 acc(18,21,19,16,5,17); // Initialisation des moteurs accessoirs (Les valeurs sont dans le même ordre que la ligne ci-dessus)
TM1637Display display = TM1637Display(CLK, DIO); // Initialisation de l'écran avec le score


void setup() {
  Serial.begin(115200);
  PS4.begin("1C:57:DC:44:AD:2B"); //Connectez votre mannette de playstation en l'appairant avec votre ordinateur puis, en mettant ici l'adresse mac de votre ordinateur
  //Pensez bien à couper le bluetooth de votre ordinateur
  Serial.println("Ready.");
  while (!PS4.isConnected()) {
    // Attend une connection de la manette
  }
  Serial.print("Connected");
  acc.setSpeed(255); // Vitesse par défaut
  display.clear(); // Vide l'écran
  display.setBrightness(7); // Met l'intensité lumineuse de l'écran à 7 (max)

}

void stop()
{
  motors.stop(); // arrête les moteurs
}


void loop() {

  //Instruction Compteur

  if (PS4.Up()) {
    counter = counter+10;
    delay(100);
  }

  else if (PS4.Down()) {
    counter = counter-10;
    delay(100);
  }

  else if (PS4.Right()) {
    counter = counter+2;
    delay(100);
  }

  else if (PS4.Left()) {
    counter = counter-2;
    delay(100);
  }

  display.showNumberDec(counter); // Affiche le score

  //Instructions Moteur Gauche :

  if (PS4.LStickY()>=20) {
    motors.setSpeedA((PS4.LStickY()*2)-20);
    motors.forwardA();
    Serial.print((PS4.LStickY()*2)-20);
  }

  else if (PS4.LStickY()<=-20) {
    motors.setSpeedA(abs((PS4.LStickY()*2)+20));
    motors.backwardA();
    Serial.print(abs((PS4.LStickY()*2)+20));
  }

  else {
    motors.stopA();
    Serial.println("Stop A");
  }

  //Instruction Moteur Droit : 

  if (PS4.RStickY()>=20) {
    motors.setSpeedB((PS4.RStickY()*2)-20);
    motors.forwardB();
    Serial.println("Avance droit");
  }

  else if (PS4.RStickY()<=-20) {
    motors.setSpeedB(abs((PS4.RStickY()*2)+20));
    motors.backwardB();
    Serial.println("Recule droit");
  }

  else {
    motors.stopB();
    Serial.println("Stop B");
  }

  // Moteur accessoire A

  if (PS4.R2()){
    acc.forwardA();
  }

  else if (PS4.L2()) {
    acc.backwardA();
  }

  else {
    acc.stopA();
  }

  // Moteur accessoire B

    if (PS4.R1()){
    acc.forwardB();
  }

  else if (PS4.L1()) {
    acc.backwardB();
  }

  else {
    acc.stopB();
  }
}
