// Librerías a utilizar
#include <Keypad.h>
#include <Servo.h>

// Configuración de variables para usar el teclado matricial
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // pines de las filas
byte colPins[COLS] = {5, 4, 3}; // pines de las columnas
// Creamos el objeto del teclado matricial, la librería ya se encarga de leer el valor cuando llamamos a getKey()
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Configuración de variables para las entradas analógicas
const int ldr1 = 0;
const int ldr2 = 1;
const int ldr3 = 2;
const int ldr4 = 3;

// Configuración de variables para controlar el servomotor
const int motor = 13; // pin del servo
int motor_status = 0; // 0: Puerta abierta, 1: Puerta cerrada
Servo servo; // Creamos el objeto del servo

// Variable a usar para leer el sensor, procesar su información y controlar el motor
float ldr1_value = 0;
float ldr2_value = 0;
float ldr3_value = 0;
float ldr4_value = 0;
float ldr_mean = 0.0; // Promedio de los cuatro valores leídos
const float ldr_limit = 60;

void setup(){
  // Configuración de pines de las entradas analógicas de los LDR
  pinMode(ldr1, INPUT);
  pinMode(ldr2, INPUT);
  pinMode(ldr3, INPUT);
  pinMode(ldr4, INPUT);

  // Configuración de los pines del motor y estado inicial de sus variables, empieza en 0 grados.
  servo.attach(motor);
  motor_status = 0;
  servo.write(0);

  // Configuramos el monitor serial
  Serial.begin(9600);
}

void loop(){
  // Lectura de los valores de los LDR
  ldr1_value = analogRead(ldr1);
  ldr1_value = map(ldr1_value, 0, 1023, 0, 100);
  ldr2_value = analogRead(ldr2);
  ldr2_value = map(ldr2_value, 0, 1023, 0, 100);
  ldr3_value = analogRead(ldr3);
  ldr3_value = map(ldr3_value, 0, 1023, 0, 100);
  ldr4_value = analogRead(ldr4);
  ldr4_value = map(ldr4_value, 0, 1023, 0, 100);
  // Cálculo de la media de los valores de los LDR
  ldr_mean = (ldr1_value + ldr2_value + ldr3_value + ldr4_value) / 4;
  
  // Capturamos la tecla presionada y la leemos
  char key = keypad.getKey();
  if (key){
    // Dependiendo del valor, mostramos el valor del sensor correspondiente
    if (key == '1') {
      Serial.println("LDR elegido: 1");
      Serial.println("Valor: " + String(ldr1_value));
    }else if(key == '2') {
      Serial.println("LDR elegido: 2");
      Serial.println("Valor: " + String(ldr2_value));
    }else if(key == '3') {
      Serial.println("LDR elegido: 3");
      Serial.println("Valor: " + String(ldr3_value));
    } else if(key == '4') {
      Serial.println("LDR elegido: 4");
      Serial.println("Valor: " + String(ldr4_value));
    } else {
      Serial.println("Comando no válido");
    }
  }
  // Si la media es mayor al límite y el motor está abierto, lo cerramos
  if (ldr_mean >= ldr_limit && motor_status == 0){
    motor_status = 1;
    for (int i = 0; i <= 180; i++)
    {
      servo.write(i);
      delay(10);
    }
  }
  // Si la media es menor al límite y el motor está cerrado, lo abrimos
  if (ldr_mean < ldr_limit && motor_status == 1){
    motor_status = 0;
    for (int i = 179; i > 0; i--)
    {
      servo.write(i);
      delay(10);
    }
  }
}