#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include <LXESP32DMX.h>
#include <Arduino.h>
#include "ArduinoJson.h"
#ifndef ARG_H
#define ARG_H

// ici on declare toutes les variables
const int csPin = 15;
const int txuart = 4;
const int rxuart = 32;
extern String info;
extern bool noconnection;
extern bool connecter;
extern String final;
extern int args;
extern int canal;
extern String message_canal;
extern int valeur_canal;
extern String message_valeur_canal;
extern String off;
extern String message_off;
extern String on;
extern String message_on;
extern double on_progress;
extern double off_progress;
extern String message_on_progress;
extern String message_off_progress;
extern String ramidom;
extern String message_random;
extern int increment;
extern String message_increment;
extern int decrement;
extern String message_decrement;
extern String save;
extern String message_save;
extern String on_save;
extern String message_on_save;
extern String id;
extern String message_id;
extern String supp;
extern String message_supp;
extern String CMD;
extern String message_CMD;
extern String var;
extern String message_var;
extern bool on_activer;
extern bool off_activer;
extern const char *www_username;
extern const char *www_password;
extern bool decrement_activer;
extern bool increment_activer;
extern int valeur_canalv2;
extern uint8_t dmxbuffer[DMX_MAX_FRAME];
extern uint8_t level;
extern uint8_t oldlevel;
extern bool get_loop;
extern int canal_value;
extern bool on_state;
extern bool variation_increment_activer;
extern bool variation_decrement_activer;
extern bool on_progress_activer;
extern bool off_progress_activer;
extern long randNumber;
extern uint8_t offset_max;
extern uint8_t offset_min;
extern String message_offset_max;
extern String message_offset_min;
extern bool on_save_activer;
extern bool save_activer;
extern bool variation_increment_faite;
extern bool variation_decrement_faite;
extern double gamma_val;
extern String message_gamma;
extern int g;
extern double Delay;
extern String message_delay;
extern String restore;
extern String message_restore;
extern String download;
extern String message_download;
extern String upload;
extern String message_upload;
extern bool ready_download;
extern String date;
extern String name;
extern bool download_activer;
extern bool redirect;
extern int groupe;
extern String message_groupe;
extern String lire_groupe;
extern String message_lire_groupe;
extern int arraygroupe[33];
extern int add_in_groupe;
extern String message_add_in_groupe;
extern int supp_in_groupe;
extern String message_supp_in_groupe;
extern String message_on_groupe;
extern String on_groupe;
extern String message_off_groupe;
extern String off_groupe;
extern double Delay_groupe;
extern String message_delay_groupe;
extern String message_decrement_groupe;
extern String message_increment_groupe;
extern String message_on_progress_groupe;
extern String message_off_progress_groupe;
extern String message_valeur_canal_groupe;
extern String message_var_groupe;
extern String message_offset_max_groupe;
extern String message_offset_min_groupe;
extern String message_gamma_groupe;
extern double on_progress_groupe;
extern double off_progress_groupe;
extern bool off_progress_groupe_activer;
extern bool on_progress_groupe_activer;
extern uint8_t offset_max_groupe;
extern uint8_t offset_min_groupe;
extern uint8_t level_groupe;
extern bool decrement_groupe_activer;
extern bool increment_groupe_activer;
extern int increment_groupe;
extern int decrement_groupe;
extern bool get_loop_groupe;
extern String valeur_canal_groupe;
extern TaskHandle_t Task1;
extern int canal_groupe;
extern int canal_value_groupe;
extern String message_canal_value_groupe;
extern String var_groupe;
extern bool variation_increment_groupe_activer;
extern bool variation_decrement_groupe_activer;
extern bool variation_increment_groupe_faite;
extern bool variation_decrement_groupe_faite;
extern String message_dimmer;
extern float dimmer;
extern int ALLvalue[256]; 
extern const int eeprom_start;
extern int addr;
extern bool stop_i;
extern String message_nom_groupe ;
extern String  nom_groupe ;
extern bool save_groupe;
extern bool save_canal ;
extern int nb_name;
extern DynamicJsonDocument dataT;
extern String message_nom_canal;
extern String nom_canal;
extern bool save_effectuer;
#endif