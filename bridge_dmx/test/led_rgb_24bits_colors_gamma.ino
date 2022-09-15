/*
 * Code d'exemple pour une LED RGB (+16 millions couleurs) avec correction Gamma.
 */

/* Broches */


/* ----- BEGIN OF AUTO-GENERATED CODE - DO NOT EDIT ----- */
#ifndef GAMMA_H_
#define GAMMA_H_

/* Dependency for PROGMEM */
#include <Arduino.h> 

/** Gamma correction table in flash memory. */
static const uint8_t PROGMEM _gamma[] = {
    0,  21,  28,  34,  39,  43,  46,  50,  53,  56,  59,  61,  64,  66,  68,  70,
   72,  74,  76,  78,  80,  82,  84,  85,  87,  89,  90,  92,  93,  95,  96,  98,
   99, 101, 102, 103, 105, 106, 107, 109, 110, 111, 112, 114, 115, 116, 117, 118,
  119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
  136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 149, 150,
  151, 151, 152, 153, 154, 155, 156, 156, 157, 158, 159, 160, 160, 161, 162, 163,
  164, 164, 165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175,
  175, 176, 177, 178, 178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186,
  186, 187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 195, 196,
  197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206,
  206, 207, 207, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215,
  215, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224,
  224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232,
  232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240,
  240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248,
  248, 249, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255
};

/**
 * Apply gamma correction to the given sample.
 *
 * @param x The input 8 bits sample value.
 * @return The output 8 bits sample value with gamma correction.
 */
static inline uint8_t Gamma(uint8_t x) {
  return pgm_read_byte(&_gamma[x]);
}

#endif /* GAMMA_H_ */
/* ----- END OF AUTO-GENERATED CODE ----- */

// Fonction setup(), appelée au démarrage de la carte Arduino
void setup() {

  // Initialise les broches
 /*  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  displayColor(0, 0, 0);*/
} 

// Fonction loop(), appelée continuellement en boucle tant que la carte Arduino est alimentée
void loop() {
  
  /* Code de démonstration */
  for (int i = 0; i < 256; i++) {
    displayColor(i & 255, 0, 0);
    delay(10);
  }
  delay(1000);
  
  for (int i = 0; i < 256; i++) {
    displayColor(Gamma(i & 255), 0, 0);
    delay(10);
  }
  delay(1000);
}

/** Affiche une couleur */
void displayColor(byte r, byte g, byte b) {

  // Assigne l'état des broches
  // Version cathode commune
  //analogWrite(PIN_LED_R, r);
  //analogWrite(PIN_LED_G, g);
  //analogWrite(PIN_LED_B, b);
  // Version anode commune
  //analogWrite(PIN_LED_R, ~r);
  //analogWrite(PIN_LED_G, ~g);
  //analogWrite(PIN_LED_B, ~b);
}