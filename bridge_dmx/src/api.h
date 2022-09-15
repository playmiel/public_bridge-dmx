#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include <EEPROM_SPI_WE.h>
#include <Arduino.h>
#ifndef API_H
#define API_H

// ici on declare toutes les voies a utiliser pour les fonctions

#define DMX_DIRECTION_PIN 4
#define DMX_SERIAL_OUTPUT_PIN 2
extern EEPROM_SPI_WE myEEP;
bool handlerequest(AsyncWebServerRequest *request, String link);
void go_canal();
void allclear();
void go_on();
void go_off();
void copyDMXToOutput(void);
extern bool rip;
void messageclear();
void go_variation();
void go_increment();
void go_decrement();
void go_state();
void go_valeur();
void go_on_progresson_progress();
void go_off_progresson_progress();
void go_random();
void go_on_save();
void handleFileUpload(AsyncWebServerRequest *request, String filename3, size_t index, uint8_t *data, size_t len, bool final);
extern struct Config config;
extern String filename2;
void printFile(String filename2);
void saveConfiguration(String filename2, const Config &config);
void loadConfiguration(String filename2, Config &config);
void go_stop();
void go_gamma();
void go_download();
void go_restore();
void go_upload();
void go_save();
void go_save();
bool is_authenticated(AsyncWebServerRequest *request);
void handlerequete(AsyncWebServerRequest *request);
// void handlecanal(AsyncWebServerRequest *request);
bool handleFileRead(AsyncWebServerRequest *request, String path);
extern const char *filename;
String getContentType(String filename);
bool CopiaPega(AsyncWebServerRequest *request, String FileOriginal, String FileCopia);
void handleInfo(AsyncWebServerRequest *request);
void go_groupe();
void go_lire_groupe();
void go_add_in_groupe();
void go_supp_in_groupe();
void copy(int *src, int *dst, int len);
void go_on_groupe();
void go_off_groupe();
bool groupe_full();
void go_on_groupe();
void go_off_groupe();
void go_on_progresson_progress_groupe();
void go_off_progresson_progress_groupe();
void handleInfo(AsyncWebServerRequest *request);
void go_decrement_groupe();
void go_increment_groupe();
void go_valeur_canal_groupe();
void Task1code(void *pvParameters);
void go_canal_value_groupe();
void go_variation_groupe();
void go_dimmer();
void invertcopy();
String processor(const String &var);
void writeIntArrayIntoEEPROM(int groupnum);
void readIntArrayFromEEPROM(int groupnum);
void go_nom_groupe();
void invert_nom_groupe(int nom_num);
void go_nom_canal();
void invert_nom_canal(int canal_name);
void go_lire_groupe_post(AsyncWebServerRequest *request);
void go_give_nom_groupe();
void go_give_nom_canal();
void invertgroupe();
void handleNotFound(AsyncWebServerRequest *request);
#endif