#include "api.h"
#include "arg.h"
#include <LXESP32DMX.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include <gamma.h>
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include <esp_task_wdt.h>
#include <EEPROM.h>
#include "groupe.h"
#include <EEPROM_SPI_WE.h>
#include <namei.h>
#include <WiFi.h>
#include <StreamUtils.h>

int val_moy = 0;
bool save_effectuer = false;
const int start_name_adrr = 2000;
const int start_name_adrr2 = 3500;
const int addr_name = 33;
// init des variables
int ALLvalue[256]{

};
int oldgroupe = 1;
String s = "";
DynamicJsonDocument docInfo(2048);
String info = "";
String info_array = "";
bool redirect = false;
File fsUploadFile;
int g = 1;
uint8_t dmxbuffer[DMX_MAX_FRAME];
bool variation_increment_faite = false;
bool variation_decrement_faite = false;
int value = 0;
bool get_loop = false;
uint8_t level = 0;
int canal_groupe = 0;
int canal_value_groupe = 0;
bool variation_increment_groupe_activer = false;
bool variation_decrement_groupe_activer = false;
bool variation_increment_groupe_faite = false;
bool variation_decrement_groupe_faite = false;
struct Config
{
  int canal_value;
  bool variation_decrement_activer;
  bool variation_increment_activer;
  int offset_max;
  int offset_min;
  bool connecter;
};
int canal_value = 0;
bool on_state = false;
bool variation_increment_activer = false;
bool variation_decrement_activer = false;
long randNumber;
bool add_end = false;
bool get_loop_groupe = false;
bool stop_i = false;

void copyDMXToOutput(void)
{
  // pour plus d'infos allez voir le'exemple output test de la bibliotheques dmx
  xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);
  for (int i = 1; i < DMX_MAX_FRAME; i++)
  {
    ESP32DMX.setSlot(i, dmxbuffer[i]);
  }
  xSemaphoreGive(ESP32DMX.lxDataLock);
}
void messageclear()
{
  // permet d'effacer tous les messages
  message_canal = "";
  message_valeur_canal = "";
  message_off = "";
  message_on = "";
  message_on_progress = "";
  message_off_progress = "";
  message_random = "";
  message_increment = "";
  message_decrement = "";
  message_save = "";
  message_on_save = "";
  message_id = "";
  message_CMD = "";
  message_var = "";
  message_offset_min = "";
  message_offset_max = "";
  message_gamma = "";
  message_delay = "";

  message_restore = "";

  message_download = "";

  message_upload = "";

  message_groupe = "";

  message_groupe = "";

  message_lire_groupe = "";

  message_add_in_groupe = "";

  message_supp_in_groupe = "";

  message_on_groupe = "";

  message_off_groupe = "";

  message_delay_groupe = "";
  message_decrement_groupe = "";
  message_increment_groupe = "";
  message_on_progress_groupe = "";
  message_off_progress_groupe = "";
  message_valeur_canal_groupe = "";
  message_var_groupe = "";
  message_dimmer = "";

  message_offset_max_groupe = "";
  message_offset_min_groupe = "";
  message_canal_value_groupe = "";
  message_nom_groupe = "";
  message_nom_canal = "";
}

void allclear()
{
  // permet d'effacer toutes les variables
  message_on_groupe = "";
  on_groupe = "";
  message_off_groupe = "";
  off_groupe = "";
  message_groupe = "";
  groupe = 1;
  Delay = 0.5;
  message_delay = "";
  final = "";
  canal = 1;
  message_canal = "";
  valeur_canal = 0;
  message_valeur_canal = "";
  off = "";
  message_off = "";
  on = "";
  message_on = "";
  on_progress = 0;
  off_progress = 0;
  message_on_progress = "";
  message_off_progress = "";
  ramidom = "";
  message_random = "";
  increment = 0;
  message_increment = "";
  decrement = 0;
  message_decrement = "";
  save = "";
  message_save = "";
  on_save = "";
  message_on_save = "";
  id = "";
  message_id = "";
  supp = "";
  CMD = "";
  message_CMD = "";
  var = "";
  message_var = "";
  offset_min = 0;
  offset_max = 255;
  message_offset_min = "";
  message_offset_max = "";
  gamma_val = 1.0;
  message_gamma = "";
  message_delay = "";
  restore = "";
  message_restore = "";
  download = "";
  message_download = "";
  upload = "";
  message_upload = "";
  nom_canal = "";
  groupe = 1;
  message_groupe = "";
  lire_groupe = "";
  message_lire_groupe = "";
  memset(arraygroupe, 0, sizeof(arraygroupe));
  add_in_groupe = 1;
  message_add_in_groupe = "";
  supp_in_groupe = 1;
  message_supp_in_groupe = "";
  myEEP.eraseCompleteEEPROM();
  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  message_delay_groupe = "";
  message_decrement_groupe = "";
  message_increment_groupe = "";
  message_on_progress_groupe = "";
  message_off_progress_groupe = "";
  message_valeur_canal_groupe = "";
  message_var_groupe = "";
  message_nom_canal = "";
  message_offset_max_groupe = "";
  message_offset_min_groupe = "";
  message_canal_value_groupe = "";
  canal_value_groupe = 0;
  on_progress_groupe = 0;
  off_progress_groupe = 0;
  offset_max_groupe = 255;
  offset_min_groupe = 0;
  increment_groupe = 0;
  decrement_groupe = 0;
  Delay_groupe = 0.5;
  valeur_canal_groupe = "";
  var_groupe = "";
  message_dimmer = "";
  dimmer = 1;
  message_nom_groupe = "";
  nom_groupe = "";
  save_canal = true;
  save_groupe = true;
  go_save();
  save_canal = false;
  save_groupe = false;
}

void go_on()
{
  // permet d'allumer le dmx au maximum
  if ((on_activer == true) && (off_activer == true))
  {
    message_on.clear();
    message_on = "n'activer pas on et off en meme temps";

    Serial.println("pas get_loop");
  }
  else
  {
    message_on.clear();
    dmxbuffer[canal] = 255;
    copyDMXToOutput();

    message_on += "allume le canal ";
    message_on += canal;

    Serial.println("get_loop");
  }
}

void go_off()
{
  // permet d'eteindre le dmx
  if ((on_activer == true) && (off_activer == true))
  {

    message_off.clear();
    message_off = "n'activer pas on et off en meme temps";

    Serial.println("pas ouais");
  }
  else
  {
    message_off.clear();
    dmxbuffer[canal] = 0;

    copyDMXToOutput();
    message_off += "extinction du canal ";
    message_off += canal;
    Serial.println("ouais");
  }
}
void go_canal()
{
  // permet de setup un canal
  value = ESP32DMX.getSlot(canal);
  dmxbuffer[canal] = value;
  copyDMXToOutput();
}

void go_variation()
{
  // permet de faire varier le canal designer ,si le canal est superieure a la moitier ca dimminue sinon ca augmente (soit decrement)
  message_var.clear();
  value = ESP32DMX.getSlot(canal);

  if (variation_increment_faite || value > 127)
  {
    message_var = "decremente le canal ";
    message_var += canal;
    variation_decrement_activer = true;
    variation_increment_faite = false;
    variation_decrement_faite = true;
    go_decrement();
  }
  else
  {
    message_var = "incremente le canal ";
    message_var += canal;
    variation_increment_activer = true;
    variation_increment_faite = true;
    variation_decrement_faite = false;
    go_increment();
  }
}
void go_valeur()
{
  // permet de mettre un canal a une certaine valeur
  dmxbuffer[canal] = constrain(valeur_canal, offset_min, offset_max);

  copyDMXToOutput();
}
void go_increment()
{
  // active l'increment (voir la loop)
  get_loop = true;
}
void go_decrement()
{
  // active le decrement (voir la loop)
  get_loop = true;
}
void go_state()
{

  // permet de regarde l'etat d'un canal
  canal_value = ESP32DMX.getSlot(canal);
  if (canal_value > 0)
  {
    on_state = true;
  }
  else
  {
    on_state = false;
  }
}

void go_on_progresson_progress()
{
  // verifie l'etat precedent pour la variation
  on_progress_activer = true;
}
void go_off_progresson_progress()
{
  // verifie l'etat precedent pour la variation
  off_progress_activer = true;
}

void go_random()
{
  // met un chiffre aleatoire au canal
  randNumber = random(256);
  Serial.println(randNumber);
  dmxbuffer[canal] = randNumber;
  copyDMXToOutput();
}

// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase. See:
// https://arduinojson.org/v6/faq/why-must-i-create-a-separate-config-object/

String filename2 = "/config.json"; // <- SD library uses 8.3 filenames
Config config;                     // <- global configuration object

// Loads the configuration from a file
void loadConfiguration(String filename2, Config &config)
{
  oldgroupe = groupe;
  // Open file for reading
  File file = SPIFFS.open(filename2, FILE_READ);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  DynamicJsonDocument doc(32768);
  // file.setTimeout(10000);
  //  Deserialize the JSON document
  // ReadLoggingStream wifiClientWithLog(file, Serial);
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println(F("Failed to read file, using default configuration"));
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }
  // Copy values from the JsonDocument to the Config
  variation_decrement_faite = doc["variation_decrement_faite"] | false;
  variation_increment_faite = doc["variation_increment_faite"] | false;
  if (on_save_activer)
    ;

  s = "";
  for (int i = 0; i < 256; i++)
  {
    esp_task_wdt_reset();
    s = "canal_value";
    s += i;
    config.canal_value = doc[s] | 0;
    /*     Serial.print(s); */
    dmxbuffer[i] = constrain(config.canal_value, offset_min, offset_max);
    copyDMXToOutput();
  }

  s = "";
  DynamicJsonDocument json(765);

  for (int i = 1; i < 33; i++)
  {
    esp_task_wdt_reset();
    JsonArray jsonarray = json.to<JsonArray>();
    groupe = i;
    s = "groupe";
    s += i;
    jsonarray = doc[s];
    copyArray(jsonarray, arraygroupe, 33);
    invertcopy();
    // go_groupe();
    /*     Serial.print(s); */
  }

  offset_max = doc["offset_max"] | 255;
  offset_min = doc["offset_min"] | 0;
  offset_max_groupe = doc["offset_max_groupe"] | 255;
  offset_min_groupe = doc["offset_min_groupe"] | 0;
  gamma_val = doc["gamma"] | 1;
  /* strlcpy(config.hostname,                  // <- destination
          doc["hostname"] | "example.com",  // <- source
          sizeof(config.hostname));         // <- destination's capacity */
  connecter = doc["connecter"] | true;

  for (size_t i = 1; i <= 32; i++)
  {
    esp_task_wdt_reset();
    readIntArrayFromEEPROM(i);
  }
  for (size_t i = 1; i <= 256; i++)
  {
    esp_task_wdt_reset();
    invert_nom_canal(i);
  }
  for (size_t i = 1; i <= 32; i++)
  {
    esp_task_wdt_reset();
    invert_nom_groupe(i);
  }
  // Close the file (Curiously, File's destructor doesn't close the file)
  on_save_activer = false;
  save_activer = false;
  groupe = oldgroupe;
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(String filename2, const Config &config)
{

  oldgroupe = groupe;
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename2);

  // Open file for writing
  File file = SPIFFS.open(filename2, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  DynamicJsonDocument doc(32768);
  doc["gamma"] = gamma_val;
  doc["connecter"] = connecter;
  // Set the values in the document
  doc["variation_decrement_faite"] = variation_decrement_faite;
  doc["variation_increment_faite"] = variation_decrement_faite;
  doc["offset_max"] = offset_max;
  doc["offset_min"] = offset_min;
  doc["offset_min_groupe"] = offset_min_groupe;
  doc["offset_max_groupe"] = offset_max_groupe;
  if (save_activer)
    ;

  /* sauvegarde les canaux dans l'eeprom */
  Serial.println("save canal");
  addr = eeprom_start;
  for (int i = 0; i < 256; i++)
  {
    EEPROM.write(addr, ESP32DMX.getSlot(i));
    addr = addr + 1;
    /*     Serial.println("actual_value: ");

        Serial.println(ESP32DMX.getSlot(i)); */
    // ALLvalue[i] = ESP32DMX.getSlot(i);
  }

  EEPROM.write(0, level);
  EEPROM.commit();

  s = "";
  for (int i = 0; i < 256; i++)
  {
    canal_value = ESP32DMX.getSlot(i);
    s = "canal_value";
    s += i;
    doc[s] = canal_value;
    /*     Serial.print(s); */
  }

  DynamicJsonDocument json(765);

  s = "";
  for (int i = 1; i < 33; i++)
  {
    JsonArray jsonarray = json.to<JsonArray>();
    groupe = i;
    invertcopy();
    go_groupe();
    // Serial.println("Groupe: ");
    //  Serial.println(groupe);
    s = "groupe";
    s += i;
    for (int i = 0; i < 32; i++)
    {
      jsonarray.add(arraygroupe[i]);
    }
    /*     Serial.print(s); */

    doc[s] = jsonarray;
    esp_task_wdt_reset();
  }

  // Serialize JSON to file
  if (serializeJsonPretty(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  // Close the file
  file.close();
  on_save_activer = false;
  save_activer = false;
  groupe = oldgroupe;
}

// Prints the content of a file to the Serial
void printFile(String filename2)
{
  // Open file for reading
  File file = SPIFFS.open(filename2);
  if (!file)
  {
    Serial.println(F("Failed to read file"));
    return;
  }
  // Extract each characters by one by one
  while (file.available())
  {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}

void go_on_save()
{
  // restaure les variables precedentes
  /*   dmxbuffer[canal] = constrain(config.canal_value, offset_min, offset_max);
    copyDMXToOutput(); */
}
void (*resetFunc)(void) = 0; // declare reset fuction at address 0

void go_stop()
{

  /* if (redirect == true || on_progress_activer == true || off_progress_activer == true || increment_activer == true || decrement_activer == true || variation_increment_activer == true || variation_decrement_activer == true || off_progress_groupe_activer == true || on_progress_groupe_activer == true || increment_groupe_activer == true || decrement_groupe_activer == true || variation_increment_groupe_activer == true || variation_decrement_groupe_activer == true)
  {
    redirect = false;
    on_progress_activer = false;
    off_progress_activer = false;
    decrement_activer = false;
    increment_activer = false;
    variation_increment_activer = false;
    variation_decrement_activer = false;
    off_progress_groupe_activer = false;
    on_progress_groupe_activer = false;
    increment_groupe_activer = false;
    decrement_groupe_activer = false;
    variation_increment_groupe_activer = false;
    variation_decrement_groupe_activer = false;
    Serial.println(F("desactivation"));

           httpServer.on("/offwait", HTTP_POST, [](AsyncWebServerRequest *request){
       request->send(200);
     });
  }
  // stop le dmx
  ESP.restart();
  */
  stop_i = true;
}

void go_gamma()
{
  // choisie la courrbe gamma
  g = (gamma_val * 10);
}
void go_save()
{
  // sauvegarde les variables
  on_save_activer = true;
  save_activer = true;
  saveConfiguration("/config.json", config);
  message_save = "savegarde les données";
  // printFile("/config.json");
  esp_task_wdt_reset();
  if (save_groupe == true)
  {
    /* for (size_t i = 1; i <= 32; i++)
    {
          esp_task_wdt_reset();
          writeIntArrayIntoEEPROM(i);
    } */

    esp_task_wdt_reset();
    writeIntArrayIntoEEPROM(groupe);
  }
  save_effectuer = true;
  groupe = oldgroupe;
}

void go_download()
{
  // active le telechargement
  ready_download = true;
}
void go_upload()
{
  // ne sert a rien pour l'instant (prevision pour le futur site web)
}
void go_restore()
{

  // ne sert a rien pour l'instant (prevision pour le futur site web)
}

bool CopiaPega(AsyncWebServerRequest *request, String FileOriginal, String FileCopia)
{
  esp_task_wdt_reset();
  // sert a copier un fichier dans un autre
  DynamicJsonDocument doc2(131072);

  DynamicJsonDocument doc(131072);
  Serial.println("debut de la copy"); // debug

  if (SPIFFS.exists(FileCopia) == true) // supprimer la copie de fichiers
  {
    Serial.println("fichier existe");
    SPIFFS.remove(FileCopia);
  }
  else
  {
    Serial.println("fichier existe pas");
  }

  File f1 = SPIFFS.open(FileOriginal, "r"); // fichier source ouvert à lire
  if (!f1)
  {
    Serial.println("error le fichier n'a pas pu etre lu"); // debug
  }
  else
  {
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, f1);
    if (error)
    {
      Serial.print(F("deserializeJson() rater: "));
      Serial.println(error.c_str());
    }
  }
  File f2 = SPIFFS.open(FileCopia, "a+"); // ouvrir le fichier de destination pour écrire
  if (!f2)
  {
    Serial.println("le fichier ne s'ouvre pas"); // debug
  }

  size_t sizet = f1.size();
  if (sizet > 131072)
  {
    Serial.println("le fichier est trop grand");
    return false;
  }
  else if (sizet == 0)
  {
    Serial.println("le fichier est vide");
    return false;
  }

  doc2 = doc;
  /* while (f1.available() > 0)
   { std::unique_ptr<char[]> buf(new char[sizet]);
   byte i = f1.readBytes(buf.get(), sizet); // i = number of bytes placed in buffer from file f1

     f2.write(i);             // write i bytes from buffer to file f2
   } */
  if (serializeJsonPretty(doc2, f2) == 0)
  {
    Serial.println(F("rater l'ecriture du fichier"));
  }
  f2.close();                    // terminé, fermer le fichier de destination
  f1.close();                    // terminé, fermer le fichier source
  Serial.println("fin de copy"); // debug

  if (download_activer == true)
  {
    int fileSize = 131072;
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, name, getContentType(name), true);
    // pour telecharger le fchier
    /* response->setCode(303);*/
    /*     response->addHeader("Content-Length", (String)fileSize);
        response->addHeader("Accept-Ranges", (String)fileSize);
        response->addHeader("Connection", "close"); */
    // response->addHeader("Location", "/test");
    // response->setCode(301);
    request->send(response);

    redirect = true;
    // request->redirect(url);
  }

  name.clear();
  return false;
}
bool handlerequest(AsyncWebServerRequest *request, String link)
{
  request->redirect(link);
  return false;
}

void handleFileUpload(AsyncWebServerRequest *request, String filename3, size_t index, uint8_t *data, size_t len, bool final)
{
  // permet de gerer l'upload des fichiers

  StaticJsonDocument<1024> ledoc2;
  StaticJsonDocument<1024> ledoc3;
  StaticJsonDocument<1024> ledoc;

  String filev2 = "";
  filev2 += "/";
  filev2 += filename3;
  filename3 = filev2;

  String file_temp = "";
  file_temp += filename3;
  file_temp += "_temp";
  if (!index)
  {

    Serial.println((String) "Upload commence: " + filename3);
  }

  File file = SPIFFS.open(filename3, "w+");
  if (len)
  {
    ledoc = file.write(data, len);
    file.close();
    File file = SPIFFS.open(filename3, "r+");
    if (len)
    {
      file.read(data, len);
      file.close();
    }
    for (size_t i = 0; i < len; i++)
    {
      Serial.write(data[i]);
    }
  }

  File file3 = SPIFFS.open(filename3, "a+");
  if (len)
  {
    serializeJsonPretty(ledoc, file3);
  }
  file3.close();

  if (final)
  {

    Serial.println((String) "Uploadfini: " + filename3 + "," + index + "," + len);

    request->redirect("/");
  }
  loadConfiguration(filename3, config);
  printFile("/config.json");
}

void handleInfo(AsyncWebServerRequest *request)
{
  int xlevel = 0;
  Serial.print("info running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println(F("envoie des info"));
  docInfo.clear();
  info.clear();
  xlevel = ESP32DMX.getSlot(canal);
  docInfo["level"] = level;
  docInfo["offset_max"] = offset_max;
  docInfo["offset_min"] = offset_min;
  docInfo["gamma"] = gamma_val;
  docInfo["dimmer"] = dimmer;
  go_valeur_canal_groupe();
  docInfo["val_moy"] = val_moy;
  serializeJson(docInfo, info);
  request->send(200, "application/json", info);
}

void go_groupe()
{

  switch (groupe)
  {
  case 1:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe1, arraygroupe, 32);
    break;
  case 2:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe2, arraygroupe, 32);
    break;
  case 3:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe3, arraygroupe, 32);
    break;
  case 4:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe4, arraygroupe, 32);
    break;
  case 5:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe5, arraygroupe, 32);
    break;
  case 6:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe6, arraygroupe, 32);
    break;
  case 7:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe7, arraygroupe, 32);
    break;
  case 8:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe8, arraygroupe, 32);
    break;
  case 9:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe9, arraygroupe, 32);
    break;
  case 10:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe10, arraygroupe, 32);
    break;
  case 11:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe11, arraygroupe, 32);
    break;
  case 12:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe12, arraygroupe, 32);
    break;
  case 13:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe13, arraygroupe, 32);
    break;
  case 14:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe14, arraygroupe, 32);
    break;
  case 15:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe15, arraygroupe, 32);
    break;
  case 16:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe16, arraygroupe, 32);
    break;
  case 17:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe17, arraygroupe, 32);
    break;
  case 18:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe18, arraygroupe, 32);
    break;
  case 19:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe19, arraygroupe, 32);
    break;
  case 20:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe20, arraygroupe, 32);
    break;
  case 21:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe21, arraygroupe, 32);
    break;
  case 22:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe22, arraygroupe, 32);
    break;
  case 23:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe23, arraygroupe, 32);
    break;
  case 24:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe24, arraygroupe, 32);
    break;
  case 25:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe25, arraygroupe, 32);
    break;
  case 26:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe26, arraygroupe, 32);
    break;
  case 27:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe27, arraygroupe, 32);
    break;
  case 28:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe28, arraygroupe, 32);
    break;
  case 29:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe29, arraygroupe, 32);
    break;
  case 30:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe30, arraygroupe, 32);
    break;
  case 31:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe31, arraygroupe, 32);
    break;
  case 32:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe32, arraygroupe, 32);
    break;
  default:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe1, arraygroupe, 32);
    break;
  }
}

void go_lire_groupe()
{
  groupe = oldgroupe;
  message_lire_groupe.clear();
  message_lire_groupe += "lecture du groupe:";
  message_lire_groupe += groupe;
  message_lire_groupe += ": ";
  for (int i = 0; i < 32; i++)
  {
    int valeur_actuel = 0;
    valeur_actuel = arraygroupe[i];
    message_lire_groupe += int(valeur_actuel);

    if (i == 33)
    {

      break;
    }
    message_lire_groupe += F(",");
  }
}

void go_lire_groupe_post(AsyncWebServerRequest *request)
{

  /*   for (int i = 0; i < 32; i++)
    {
      int valeur_actuel = 0;
      valeur_actuel = arraygroupe[i];
      message_lire_groupe += int(valeur_actuel);

      if (i == 33)
      {

        break;
      }
      message_lire_groupe += F(",");

    } */
  /*           JsonArray jsonarray = json.to<JsonArray>();
      // Serial.println("Groupe: ");
      //  Serial.println(groupe);

      for (int i = 0; i < 32; i++)
      {
        jsonarray.add(arraygroupe[i]);
      } */
  /*     Serial.print(s); */
  info_array.clear();
  StaticJsonDocument<768> doc;
  doc["groupe"] = groupe;

  JsonArray array = doc.createNestedArray("value");
  // copyArray(arraygroupe, 33, array);
  for (int i = 0; i < 32; i++)
  {
    array.add(arraygroupe[i]);
  }
  serializeJson(doc, info_array);
  /*     json["array_canal"] = jsonarray;
          serializeJson(json,info_array);
          serializeJsonPretty(json,Serial);
          Serial.println("\n"); */
  request->send(200, "application/json", info_array);
  // message_lire_groupe += message_lire_groupe;
}

bool groupe_full()
{
  int groupe_value = 0;
  int non_zero = 0;
  for (int i = 0; i < 33; i++)
  {
    groupe_value = arraygroupe[i];

    if (groupe_value == 0)
    {

      return false;
      break;
    }
    else

    {
      non_zero = non_zero + 1;
      if (non_zero == 32)
      {

        return true;
      }
    }
  }
  return false;
}

void go_add_in_groupe()
{
  if (groupe_full() == true)
  {
    message_add_in_groupe.clear();
    message_add_in_groupe = "le groupe est plein";
  }
  else
  {
    for (int i = 0; i < 32; i++)
    {
      message_add_in_groupe.clear();
      int groupe_value = 0;
      groupe_value = arraygroupe[i];
      Serial.println("valeur precedente: ");
      Serial.println(groupe_value);
      Serial.println("add_in_groupe: ");
      Serial.println(add_in_groupe);
      Serial.println("a la place: ");
      Serial.println(i);

      if (add_in_groupe == groupe_value)
      {

        message_add_in_groupe = "le numero du canal de demander existe deja";
        add_end = true;
      }
      else
      {
        groupe_value = arraygroupe[i];
        if (groupe_value == 0)
        {
          message_add_in_groupe += F("votre numero de canal ajouter est ");
          message_add_in_groupe += add_in_groupe;
          message_add_in_groupe += F(" et il est ajouter du groupe ");
          message_add_in_groupe += groupe;

          Serial.println("valeur actuel: ");
          Serial.println(arraygroupe[i]);
          Serial.println("changer par:  ");
          Serial.println(add_in_groupe);
          arraygroupe[i] = add_in_groupe;
          add_end = true;
        }
        else
        {
          Serial.println("pas egale à 0");
          add_end = false;
          // continue;
        }
        switch (groupe)
        {
        case 1:

          memset(groupe1, 0, sizeof(groupe1));
          copy(arraygroupe, groupe1, 32);
          break;
        case 2:
          memset(groupe2, 0, sizeof(groupe2));
          copy(arraygroupe, groupe2, 32);
          break;
        case 3:
          memset(groupe3, 0, sizeof(groupe2));
          copy(arraygroupe, groupe3, 32);
          break;
        case 4:
          memset(groupe4, 0, sizeof(groupe2));
          copy(arraygroupe, groupe4, 32);
          break;
        case 5:
          memset(groupe5, 0, sizeof(groupe2));
          copy(arraygroupe, groupe5, 32);
          break;
        case 6:
          memset(groupe6, 0, sizeof(groupe2));
          copy(arraygroupe, groupe6, 32);
          break;
        case 7:
          memset(groupe7, 0, sizeof(groupe2));
          copy(arraygroupe, groupe7, 32);
          break;
        case 8:
          memset(groupe8, 0, sizeof(groupe2));
          copy(arraygroupe, groupe8, 32);
          break;
        case 9:
          memset(groupe9, 0, sizeof(groupe2));
          copy(arraygroupe, groupe9, 32);
          break;
        case 10:
          memset(groupe10, 0, sizeof(groupe2));
          copy(arraygroupe, groupe10, 32);
          break;
        case 11:
          memset(groupe11, 0, sizeof(groupe2));
          copy(arraygroupe, groupe11, 32);
          break;
        case 12:
          memset(groupe12, 0, sizeof(groupe2));
          copy(arraygroupe, groupe12, 32);
          break;
        case 13:
          memset(groupe13, 0, sizeof(groupe2));
          copy(arraygroupe, groupe13, 32);
          break;
        case 14:
          memset(groupe14, 0, sizeof(groupe2));
          copy(arraygroupe, groupe14, 32);
          break;
        case 15:
          memset(groupe15, 0, sizeof(groupe2));
          copy(arraygroupe, groupe15, 32);
          break;
        case 16:
          memset(groupe16, 0, sizeof(groupe2));
          copy(arraygroupe, groupe16, 32);
          break;
        case 17:
          memset(groupe17, 0, sizeof(groupe2));
          copy(arraygroupe, groupe17, 32);
          break;
        case 18:
          memset(groupe18, 0, sizeof(groupe2));
          copy(arraygroupe, groupe18, 32);
          break;
        case 19:
          memset(groupe19, 0, sizeof(groupe2));
          copy(arraygroupe, groupe19, 32);
          break;
        case 20:
          memset(groupe20, 0, sizeof(groupe2));
          copy(arraygroupe, groupe20, 32);
          break;
        case 21:
          memset(groupe21, 0, sizeof(groupe2));
          copy(arraygroupe, groupe21, 32);
          break;
        case 22:
          memset(groupe22, 0, sizeof(groupe2));
          copy(arraygroupe, groupe22, 32);
          break;
        case 23:
          memset(groupe23, 0, sizeof(groupe2));
          copy(arraygroupe, groupe23, 32);
          break;
        case 24:
          memset(groupe24, 0, sizeof(groupe2));
          copy(arraygroupe, groupe24, 32);
          break;
        case 25:
          memset(groupe25, 0, sizeof(groupe2));
          copy(arraygroupe, groupe25, 32);
          break;
        case 26:
          memset(groupe26, 0, sizeof(groupe2));
          copy(arraygroupe, groupe26, 32);
          break;
        case 27:
          memset(groupe27, 0, sizeof(groupe2));
          copy(arraygroupe, groupe27, 32);
          break;
        case 28:
          memset(groupe28, 0, sizeof(groupe2));
          copy(arraygroupe, groupe28, 32);
          break;
        case 29:
          memset(groupe29, 0, sizeof(groupe2));
          copy(arraygroupe, groupe29, 32);
          break;
        case 30:
          memset(groupe30, 0, sizeof(groupe2));
          copy(arraygroupe, groupe30, 32);
          break;
        case 31:
          memset(groupe31, 0, sizeof(groupe2));
          copy(arraygroupe, groupe31, 32);
          break;
        case 32:
          memset(groupe32, 0, sizeof(groupe2));
          copy(arraygroupe, groupe32, 32);
          break;
        default:
          memset(groupe1, 0, sizeof(groupe1));
          copy(arraygroupe, groupe1, 32);
          break;
        }
      }
      if (add_end == true)
      {
        Serial.println("ca break");
        break;
      }
    }
    go_save();
  }
}

void invertcopy()
{
  switch (groupe)
  {
  case 1:

    memset(groupe1, 0, sizeof(groupe1));
    copy(arraygroupe, groupe1, 32);
    break;
  case 2:
    memset(groupe2, 0, sizeof(groupe2));
    copy(arraygroupe, groupe2, 32);
    break;
  case 3:
    memset(groupe3, 0, sizeof(groupe2));
    copy(arraygroupe, groupe3, 32);
    break;
  case 4:
    memset(groupe4, 0, sizeof(groupe2));
    copy(arraygroupe, groupe4, 32);
    break;
  case 5:
    memset(groupe5, 0, sizeof(groupe2));
    copy(arraygroupe, groupe5, 32);
    break;
  case 6:
    memset(groupe6, 0, sizeof(groupe2));
    copy(arraygroupe, groupe6, 32);
    break;
  case 7:
    memset(groupe7, 0, sizeof(groupe2));
    copy(arraygroupe, groupe7, 32);
    break;
  case 8:
    memset(groupe8, 0, sizeof(groupe2));
    copy(arraygroupe, groupe8, 32);
    break;
  case 9:
    memset(groupe9, 0, sizeof(groupe2));
    copy(arraygroupe, groupe9, 32);
    break;
  case 10:
    memset(groupe10, 0, sizeof(groupe2));
    copy(arraygroupe, groupe10, 32);
    break;
  case 11:
    memset(groupe11, 0, sizeof(groupe2));
    copy(arraygroupe, groupe11, 32);
    break;
  case 12:
    memset(groupe12, 0, sizeof(groupe2));
    copy(arraygroupe, groupe12, 32);
    break;
  case 13:
    memset(groupe13, 0, sizeof(groupe2));
    copy(arraygroupe, groupe13, 32);
    break;
  case 14:
    memset(groupe14, 0, sizeof(groupe2));
    copy(arraygroupe, groupe14, 32);
    break;
  case 15:
    memset(groupe15, 0, sizeof(groupe2));
    copy(arraygroupe, groupe15, 32);
    break;
  case 16:
    memset(groupe16, 0, sizeof(groupe2));
    copy(arraygroupe, groupe16, 32);
    break;
  case 17:
    memset(groupe17, 0, sizeof(groupe2));
    copy(arraygroupe, groupe17, 32);
    break;
  case 18:
    memset(groupe18, 0, sizeof(groupe2));
    copy(arraygroupe, groupe18, 32);
    break;
  case 19:
    memset(groupe19, 0, sizeof(groupe2));
    copy(arraygroupe, groupe19, 32);
    break;
  case 20:
    memset(groupe20, 0, sizeof(groupe2));
    copy(arraygroupe, groupe20, 32);
    break;
  case 21:
    memset(groupe21, 0, sizeof(groupe2));
    copy(arraygroupe, groupe21, 32);
    break;
  case 22:
    memset(groupe22, 0, sizeof(groupe2));
    copy(arraygroupe, groupe22, 32);
    break;
  case 23:
    memset(groupe23, 0, sizeof(groupe2));
    copy(arraygroupe, groupe23, 32);
    break;
  case 24:
    memset(groupe24, 0, sizeof(groupe2));
    copy(arraygroupe, groupe24, 32);
    break;
  case 25:
    memset(groupe25, 0, sizeof(groupe2));
    copy(arraygroupe, groupe25, 32);
    break;
  case 26:
    memset(groupe26, 0, sizeof(groupe2));
    copy(arraygroupe, groupe26, 32);
    break;
  case 27:
    memset(groupe27, 0, sizeof(groupe2));
    copy(arraygroupe, groupe27, 32);
    break;
  case 28:
    memset(groupe28, 0, sizeof(groupe2));
    copy(arraygroupe, groupe28, 32);
    break;
  case 29:
    memset(groupe29, 0, sizeof(groupe2));
    copy(arraygroupe, groupe29, 32);
    break;
  case 30:
    memset(groupe30, 0, sizeof(groupe2));
    copy(arraygroupe, groupe30, 32);
    break;
  case 31:
    memset(groupe31, 0, sizeof(groupe2));
    copy(arraygroupe, groupe31, 32);
    break;
  case 32:
    memset(groupe32, 0, sizeof(groupe2));
    copy(arraygroupe, groupe32, 32);
    break;
  default:
    memset(groupe1, 0, sizeof(groupe1));
    copy(arraygroupe, groupe1, 32);
    break;
  }
}

void go_supp_in_groupe()
{

  switch (groupe)
  {
  case 1:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe1, 32);
    break;
  case 2:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe2, 32);
    break;
  case 3:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe3, 32);
    break;
  case 4:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe4, 32);
    break;
  case 5:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe5, 32);
    break;
  case 6:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe6, 32);
    break;
  case 7:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe7, 32);
    break;
  case 8:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe8, 32);
    break;
  case 9:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe9, 32);
    break;
  case 10:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe10, 32);
    break;
  case 11:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe11, 32);
    break;
  case 12:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe12, 32);
    break;
  case 13:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe13, 32);
    break;
  case 14:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe14, 32);
    break;
  case 15:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe15, 32);
    break;
  case 16:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe16, 32);
    break;
  case 17:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe17, 32);
    break;
  case 18:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe18, 32);
    break;
  case 19:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe19, 32);
    break;
  case 20:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe20, 32);
    break;
  case 21:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe21, 32);
    break;
  case 22:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe22, 32);
    break;
  case 23:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe23, 32);
    break;
  case 24:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe24, 32);
    break;
  case 25:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe25, 32);
    break;
  case 26:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe26, 32);
    break;
  case 27:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe27, 32);
    break;
  case 28:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe28, 32);
    break;
  case 29:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe29, 32);
    break;
  case 30:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe30, 32);
    break;
  case 31:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe31, 32);
    break;
  case 32:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe32, 32);
    break;
  default:
    std::remove(std::begin(arraygroupe), std::end(arraygroupe), supp_in_groupe);
    copy(arraygroupe, groupe1, 32);
    break;
  }
}

void copy(int *src, int *dst, int len)
{
  memcpy(dst, src, sizeof(src[0]) * len);
}

void go_on_groupe()
{
  for (int i = 0; i < 32; i++)
  {
    int true_value = 0;
    true_value = arraygroupe[i];
    if (true_value != 0)
    {

      dmxbuffer[arraygroupe[i]] = 255;
      copyDMXToOutput();
    }
  }
}

void go_off_groupe()
{
  for (int i = 0; i < 32; i++)
  {
    int true_value = 0;
    true_value = arraygroupe[i];
    if (true_value != 0)
    {

      dmxbuffer[arraygroupe[i]] = 0;
      copyDMXToOutput();
    }
  }
}
void go_on_progresson_progress_groupe()
{

  on_progress_groupe_activer = true;
}

void go_off_progresson_progress_groupe()
{
  off_progress_groupe_activer = true;
}

void go_decrement_groupe()
{
  get_loop_groupe = true;
}
void go_increment_groupe()
{
  get_loop_groupe = true;
}
void go_valeur_canal_groupe()
{
  int divis = 0;
  message_valeur_canal_groupe += F("pour le groupe ");
  message_valeur_canal_groupe += groupe;
  message_valeur_canal_groupe += F(": ");
  int all_value = 0;
  for (int i = 0; i < 32; i++)
  {
    canal_groupe = arraygroupe[i];
    if (0 <= canal_groupe && canal_groupe <= 255)

    {

      canal_value_groupe = ESP32DMX.getSlot(canal_groupe);
      if (canal_value_groupe >= 1)
      {
        divis = divis + 1;
        all_value = all_value + canal_value_groupe;
      }

      /*       message_valeur_canal_groupe += String("du canal ");
            message_valeur_canal_groupe += canal_groupe;
            message_valeur_canal_groupe += F(": ");
            message_valeur_canal_groupe += canal_value_groupe;
            message_valeur_canal_groupe += String(","); */
    }
  }

  if (divis >= 1 && all_value >= 1)
  {
    val_moy = all_value / divis;
    message_valeur_canal_groupe += "la valeur moyenne est ";
    message_valeur_canal_groupe += val_moy;
    message_valeur_canal_groupe += " pour le groupe ";
    message_valeur_canal_groupe += groupe;
    Serial.println(message_valeur_canal_groupe);
  }
  else
  {
    message_valeur_canal_groupe += "la valeur moyenne est ";
    message_valeur_canal_groupe += 0;
    message_valeur_canal_groupe += " pour le groupe ";
    message_valeur_canal_groupe += groupe;
    Serial.println(message_valeur_canal_groupe);
  }
}

void go_canal_value_groupe()
{
  message_canal_value_groupe += String("valeur pour les canaux est: ");
  message_canal_value_groupe += canal_value_groupe;
  for (int i = 0; i < 32; i++)
  {
    dmxbuffer[arraygroupe[i]] = canal_value_groupe;

    copyDMXToOutput();
  }
}

void go_variation_groupe()
{
  message_var_groupe.clear();
  value = ESP32DMX.getSlot(arraygroupe[1]);

  if (value > 127)
  {
    message_var_groupe += "decremente les canaux du groupe ";
    message_var_groupe += groupe;
    variation_decrement_groupe_activer = true;
    variation_increment_groupe_faite = false;
    variation_decrement_groupe_faite = true;
    go_decrement_groupe();
  }
  else
  {
    message_var_groupe += "incremente les canaux du groupe ";
    message_var_groupe += groupe;
    variation_increment_groupe_activer = true;
    variation_increment_groupe_faite = true;
    variation_decrement_groupe_faite = false;
    go_increment_groupe();
  }
}

void go_dimmer()
{
  message_dimmer += String("dimmer pour les canaux est: ");
  message_dimmer += dimmer;
  for (int i = 0; i < 32; i++)
  {
    canal_value_groupe = ESP32DMX.getSlot(arraygroupe[i]);
    dmxbuffer[arraygroupe[i]] = canal_value_groupe * dimmer;

    copyDMXToOutput();
  }
}

void writeIntArrayIntoEEPROM(int groupnum)
{
  int address = 0;
  const int arraySize = 32;
  int addressIndex = 0;
  switch (groupnum)
  {
  case 1:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe1);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe1[i]));
      addressIndex += 1;
    }
    break;
  case 2:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe2);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe2[i]));
      addressIndex += 1;
    }
    break;
  case 3:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe3);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe3[i]));
      addressIndex += 1;
    }
    break;
  case 4:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe4);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe4[i]));
      addressIndex += 1;
    }
    break;
  case 5:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe5);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe5[i]));
      addressIndex += 1;
    }
    break;
  case 6:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe6);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe6[i]));
      addressIndex += 1;
    }
    break;
  case 7:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe7);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe7[i]));
      addressIndex += 1;
    }
    break;
  case 8:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe8);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe8[i]));
      addressIndex += 1;
    }
    break;
  case 9:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe9);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe9[i]));
      addressIndex += 1;
    }
    break;
  case 10:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe10);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe10[i]));
      addressIndex += 1;
    }
    break;
  case 11:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe11);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe11[i]));
      addressIndex += 1;
    }
    break;
  case 12:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe12);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe12[i]));
      addressIndex += 1;
    }
    break;
  case 13:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe13);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe13[i]));
      addressIndex += 1;
    }
    break;
  case 14:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe14);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe14[i]));
      addressIndex += 1;
    }
    break;
  case 15:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe15);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe15[i]));
      addressIndex += 1;
    }
    break;
  case 16:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe16);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe16[i]));
      addressIndex += 1;
    }
    break;
  case 17:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe17);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe17[i]));
      addressIndex += 1;
    }
    break;
  case 18:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe18);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe18[i]));
      addressIndex += 1;
    }
    break;
  case 19:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe19);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe19[i]));
      addressIndex += 1;
    }
    break;
  case 20:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe20);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe20[i]));
      addressIndex += 1;
    }
    break;
  case 21:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe21);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe21[i]));
      addressIndex += 1;
    }
    break;
  case 22:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe22);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe22[i]));
      addressIndex += 1;
    }
    break;
  case 23:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe23);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe23[i]));
      addressIndex += 1;
    }
    break;
  case 24:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe24);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe24[i]));
      addressIndex += 1;
    }
    break;
  case 25:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe25);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe25[i]));
      addressIndex += 1;
    }
    break;
  case 26:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe26);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe26[i]));
      addressIndex += 1;
    }
    break;
  case 27:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe27);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe27[i]));
      addressIndex += 1;
    }
    break;
  case 28:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe28);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe28[i]));
      addressIndex += 1;
    }
    break;
  case 29:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe29);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe29[i]));
      addressIndex += 1;
    }
    break;
  case 30:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe30);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe30[i]));
      addressIndex += 1;
    }
    break;
  case 31:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe31);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe31[i]));
      addressIndex += 1;
    }
    break;
  case 32:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe32);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe32[i]));
      addressIndex += 1;
    }
    break;
  default:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.put(addressIndex, groupe1);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe1[i]));
      addressIndex += 1;
    }
    break;
  }
}

void readIntArrayFromEEPROM(int groupnum)
{
  int address = 0;
  const int arraySize = 32;
  int addressIndex = 0;
  switch (groupnum)
  {
  case 1:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe1[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 2:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe2[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 3:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe3[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 4:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe4[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 5:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe5[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 6:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe6[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 7:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe7[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 8:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe8[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 9:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe9[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 10:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe10[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 11:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe11[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 12:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe12[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 13:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe13[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 14:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe14[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 15:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe15[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 16:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe16[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 17:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe17[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 18:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe18[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 19:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe19[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 20:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe20[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 21:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe21[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 22:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe22[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 23:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe23[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 24:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe24[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 25:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe25[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 26:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe26[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 27:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe27[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 28:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe28[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 29:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe29[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 30:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe30[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 31:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe31[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  case 32:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe32[i]));
      addressIndex += 1;
    }
    invertcopy();
    break;
  default:
    address = (32 * groupnum) + 1;
    groupe = groupnum;
    addressIndex = address;
    myEEP.get(addressIndex, arraygroupe);
    for (int i = 0; i < arraySize; i++)
    {
      Serial.println("value");
      Serial.println(myEEP.get(addressIndex, groupe1[i]));
      addressIndex += 1;
    }

    break;
  }
}

void go_nom_groupe()
{

  switch (nb_name)
  {
  case 1:
    strcpy(name_groupe1, nom_groupe.c_str());
    dataT["nom_groupe1"] = name_groupe1;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe1);
    break;
  case 2:
    strcpy(name_groupe2, nom_groupe.c_str());
    dataT["nom_groupe2"] = name_groupe2;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe2);
    break;
  case 3:
    strcpy(name_groupe3, nom_groupe.c_str());
    dataT["nom_groupe3"] = name_groupe3;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe3);
    break;
  case 4:
    strcpy(name_groupe4, nom_groupe.c_str());
    dataT["nom_groupe4"] = name_groupe4;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe4);
    break;
  case 5:
    strcpy(name_groupe5, nom_groupe.c_str());
    dataT["nom_groupe5"] = name_groupe5;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe5);
    break;
  case 6:
    strcpy(name_groupe6, nom_groupe.c_str());
    dataT["nom_groupe6"] = name_groupe6;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe6);
    break;
  case 7:
    strcpy(name_groupe7, nom_groupe.c_str());
    dataT["nom_groupe7"] = name_groupe7;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe7);
    break;
  case 8:
    strcpy(name_groupe8, nom_groupe.c_str());
    dataT["nom_groupe8"] = name_groupe8;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe8);
    break;
  case 9:
    strcpy(name_groupe9, nom_groupe.c_str());
    dataT["nom_groupe9"] = name_groupe9;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe9);
    break;
  case 10:
    strcpy(name_groupe10, nom_groupe.c_str());
    dataT["nom_groupe10"] = name_groupe10;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe10);
    break;
  case 11:
    strcpy(name_groupe11, nom_groupe.c_str());
    dataT["nom_groupe11"] = name_groupe11;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe11);
    break;
  case 12:
    strcpy(name_groupe12, nom_groupe.c_str());
    dataT["nom_groupe12"] = name_groupe12;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe12);
    break;
  case 13:
    strcpy(name_groupe13, nom_groupe.c_str());
    dataT["nom_groupe13"] = name_groupe13;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe13);
    break;
  case 14:
    strcpy(name_groupe14, nom_groupe.c_str());
    dataT["nom_groupe14"] = name_groupe14;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe14);
    break;
  case 15:
    strcpy(name_groupe15, nom_groupe.c_str());
    dataT["nom_groupe15"] = name_groupe15;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe15);
    break;
  case 16:
    strcpy(name_groupe16, nom_groupe.c_str());
    dataT["nom_groupe16"] = name_groupe16;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe16);
    break;
  case 17:
    strcpy(name_groupe17, nom_groupe.c_str());
    dataT["nom_groupe17"] = name_groupe17;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe17);
    break;
  case 18:
    strcpy(name_groupe18, nom_groupe.c_str());
    dataT["nom_groupe18"] = name_groupe18;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe18);
    break;
  case 19:
    strcpy(name_groupe19, nom_groupe.c_str());
    dataT["nom_groupe19"] = name_groupe19;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe19);
    break;
  case 20:
    strcpy(name_groupe20, nom_groupe.c_str());
    dataT["nom_groupe20"] = name_groupe20;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe20);
    break;
  case 21:
    strcpy(name_groupe21, nom_groupe.c_str());
    dataT["nom_groupe21"] = name_groupe21;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe21);
    break;
  case 22:
    strcpy(name_groupe22, nom_groupe.c_str());
    dataT["nom_groupe22"] = name_groupe22;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe22);
    break;
  case 23:
    strcpy(name_groupe23, nom_groupe.c_str());
    dataT["nom_groupe23"] = name_groupe23;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe23);
    break;
  case 24:
    strcpy(name_groupe24, nom_groupe.c_str());
    dataT["nom_groupe24"] = name_groupe24;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe24);
    break;
  case 25:
    strcpy(name_groupe25, nom_groupe.c_str());
    dataT["nom_groupe25"] = name_groupe25;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe25);
    break;
  case 26:
    strcpy(name_groupe26, nom_groupe.c_str());
    dataT["nom_groupe26"] = name_groupe26;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe26);
    break;
  case 27:
    strcpy(name_groupe27, nom_groupe.c_str());
    dataT["nom_groupe27"] = name_groupe27;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe27);
    break;
  case 28:
    strcpy(name_groupe28, nom_groupe.c_str());
    dataT["nom_groupe28"] = name_groupe28;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe28);
    break;
  case 29:
    strcpy(name_groupe29, nom_groupe.c_str());
    dataT["nom_groupe29"] = name_groupe29;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe29);
    break;
  case 30:
    strcpy(name_groupe30, nom_groupe.c_str());
    dataT["nom_groupe30"] = name_groupe30;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe30);
    break;
  case 31:
    strcpy(name_groupe31, nom_groupe.c_str());
    dataT["nom_groupe31"] = name_groupe31;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe31);
    break;
  case 32:
    strcpy(name_groupe32, nom_groupe.c_str());
    dataT["nom_groupe32"] = name_groupe32;
    myEEP.put(start_name_adrr + (addr_name * nb_name) + 1, name_groupe32);
    break;
  default:
    dataT["nom_groupe1"] = "bug";
    break;
  }
}

void invert_nom_groupe(int nom_num)
{
  int notzero = 0;
  switch (nom_num)
  {
  case 1:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe1);
    notzero = strlen(name_groupe1);
    if (notzero == 0)
    {
      sprintf(name_groupe1, "groupe1");
    }
    break;
  case 2:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe2);
    notzero = strlen(name_groupe2);
    if (notzero == 0)
    {
      //Serial.println("giga test2");

      sprintf(name_groupe2, "groupe2");
    }
    break;
  case 3:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe3);
    notzero = strlen(name_groupe3);
    if (notzero == 0)
    {
      sprintf(name_groupe3, "groupe3");
    }
    break;
  case 4:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe4);
    notzero = strlen(name_groupe4);
    if (notzero == 0)
    {
      sprintf(name_groupe4, "groupe4");
    }
    break;
  case 5:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe5);
    notzero = strlen(name_groupe5);
    if (notzero == 0)
    {
      sprintf(name_groupe5, "groupe5");
    }
    break;
  case 6:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe6);
    notzero = strlen(name_groupe6);
    if (notzero == 0)
    {
      sprintf(name_groupe6, "groupe6");
    }
    break;
  case 7:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe7);
    notzero = strlen(name_groupe7);
    if (notzero == 0)
    {
      sprintf(name_groupe7, "groupe7");
    }
    break;
  case 8:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe8);
    notzero = strlen(name_groupe8);
    if (notzero == 0)
    {
      sprintf(name_groupe8, "groupe8");
    }
    break;
  case 9:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe9);
    notzero = strlen(name_groupe9);
    if (notzero == 0)
    {
      sprintf(name_groupe9, "groupe9");
    }
    break;
  case 10:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe10);
    notzero = strlen(name_groupe10);
    if (notzero == 0)
    {
      sprintf(name_groupe10, "groupe10");
    }
    break;
  case 11:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe11);
    notzero = strlen(name_groupe11);
    if (notzero == 0)
    {
      sprintf(name_groupe11, "groupe11");
    }
    break;
  case 12:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe12);
    notzero = strlen(name_groupe12);
    if (notzero == 0)
    {
      sprintf(name_groupe12, "groupe12");
    }
    break;
  case 13:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe13);
    notzero = strlen(name_groupe13);
    if (notzero == 0)
    {
      sprintf(name_groupe13, "groupe13");
    }
    break;
  case 14:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe14);
    notzero = strlen(name_groupe14);
    if (notzero == 0)
    {
      sprintf(name_groupe14, "groupe14");
    }
    break;
  case 15:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe15);
    notzero = strlen(name_groupe15);
    if (notzero == 0)
    {
      sprintf(name_groupe15, "groupe15");
    }
    break;
  case 16:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe16);
    notzero = strlen(name_groupe16);
    if (notzero == 0)
    {
      sprintf(name_groupe16, "groupe16");
    }
    break;
  case 17:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe17);
    notzero = strlen(name_groupe17);
    if (notzero == 0)
    {
      sprintf(name_groupe17, "groupe17");
    }
    break;
  case 18:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe18);
    notzero = strlen(name_groupe18);
    if (notzero == 0)
    {
      sprintf(name_groupe18, "groupe18");
    }
    break;
  case 19:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe19);
    notzero = strlen(name_groupe19);
    if (notzero == 0)
    {
      sprintf(name_groupe19, "groupe19");
    }
    break;
  case 20:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe20);
    notzero = strlen(name_groupe20);
    if (notzero == 0)
    {
      sprintf(name_groupe20, "groupe20");
    }
    break;
  case 21:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe21);
    notzero = strlen(name_groupe21);
    if (notzero == 0)
    {
      sprintf(name_groupe21, "groupe21");
    }
    break;
  case 22:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe22);
    notzero = strlen(name_groupe22);
    if (notzero == 0)
    {
      sprintf(name_groupe22, "groupe22");
    }
    break;
  case 23:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe23);
    notzero = strlen(name_groupe23);
    if (notzero == 0)
    {
      sprintf(name_groupe23, "groupe23");
    }
    break;
  case 24:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe24);
    notzero = strlen(name_groupe24);
    if (notzero == 0)
    {
      sprintf(name_groupe24, "groupe24");
    }
    break;
  case 25:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe25);
    notzero = strlen(name_groupe25);
    if (notzero == 0)
    {
      sprintf(name_groupe25, "groupe25");
    }
    break;
  case 26:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe26);
    notzero = strlen(name_groupe26);
    if (notzero == 0)
    {
      sprintf(name_groupe26, "groupe26");
    }
    break;
  case 27:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe27);
    notzero = strlen(name_groupe27);
    if (notzero == 0)
    {
      sprintf(name_groupe27, "groupe27");
    }
    break;
  case 28:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe28);
    notzero = strlen(name_groupe28);
    if (notzero == 0)
    {
      sprintf(name_groupe28, "groupe28");
    }
    break;
  case 29:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe29);
    notzero = strlen(name_groupe29);
    if (notzero == 0)
    {
      sprintf(name_groupe29, "groupe29");
    }
    break;
  case 30:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe30);
    notzero = strlen(name_groupe30);
    if (notzero == 0)
    {
      sprintf(name_groupe30, "groupe30");
    }
    break;
  case 31:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe31);
    notzero = strlen(name_groupe31);
    if (notzero == 0)
    {
      sprintf(name_groupe31, "groupe31");
    }
    break;
  case 32:
    myEEP.get(start_name_adrr + (addr_name * nom_num) + 1, name_groupe32);
    notzero = strlen(name_groupe32);
    if (notzero == 0)
    {
      sprintf(name_groupe32, "groupe32");
    }
    break;
  default:
    dataT["nom_groupe1"] = "bug";
    break;
  }
}
void go_nom_canal()
{

  switch (nb_name)
  {
  case 1:
    strcpy(name_canal1, nom_canal.c_str());
    dataT["nom_canal1"] = name_canal1;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal1);
    break;
  case 2:
    strcpy(name_canal2, nom_canal.c_str());
    dataT["nom_canal2"] = name_canal2;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal2);
    break;
  case 3:
    strcpy(name_canal3, nom_canal.c_str());
    dataT["nom_canal3"] = name_canal3;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal3);
    break;
  case 4:
    strcpy(name_canal4, nom_canal.c_str());
    dataT["nom_canal4"] = name_canal4;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal4);
    break;
  case 5:
    strcpy(name_canal5, nom_canal.c_str());
    dataT["nom_canal5"] = name_canal5;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal5);
    break;
  case 6:
    strcpy(name_canal6, nom_canal.c_str());
    dataT["nom_canal6"] = name_canal6;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal6);
    break;
  case 7:
    strcpy(name_canal7, nom_canal.c_str());
    dataT["nom_canal7"] = name_canal7;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal7);
    break;
  case 8:
    strcpy(name_canal8, nom_canal.c_str());
    dataT["nom_canal8"] = name_canal8;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal8);
    break;
  case 9:
    strcpy(name_canal9, nom_canal.c_str());
    dataT["nom_canal9"] = name_canal9;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal9);
    break;
  case 10:
    strcpy(name_canal10, nom_canal.c_str());
    dataT["nom_canal10"] = name_canal10;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal10);
    break;
  case 11:
    strcpy(name_canal11, nom_canal.c_str());
    dataT["nom_canal11"] = name_canal11;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal11);
    break;
  case 12:
    strcpy(name_canal12, nom_canal.c_str());
    dataT["nom_canal12"] = name_canal12;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal12);
    break;
  case 13:
    strcpy(name_canal13, nom_canal.c_str());
    dataT["nom_canal13"] = name_canal13;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal13);
    break;
  case 14:
    strcpy(name_canal14, nom_canal.c_str());
    dataT["nom_canal14"] = name_canal14;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal14);
    break;
  case 15:
    strcpy(name_canal15, nom_canal.c_str());
    dataT["nom_canal15"] = name_canal15;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal15);
    break;
  case 16:
    strcpy(name_canal16, nom_canal.c_str());
    dataT["nom_canal16"] = name_canal16;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal16);
    break;
  case 17:
    strcpy(name_canal17, nom_canal.c_str());
    dataT["nom_canal17"] = name_canal17;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal17);
    break;
  case 18:
    strcpy(name_canal18, nom_canal.c_str());
    dataT["nom_canal18"] = name_canal18;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal18);
    break;
  case 19:
    strcpy(name_canal19, nom_canal.c_str());
    dataT["nom_canal19"] = name_canal19;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal19);
    break;
  case 20:
    strcpy(name_canal20, nom_canal.c_str());
    dataT["nom_canal20"] = name_canal20;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal20);
    break;
  case 21:
    strcpy(name_canal21, nom_canal.c_str());
    dataT["nom_canal21"] = name_canal21;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal21);
    break;
  case 22:
    strcpy(name_canal22, nom_canal.c_str());
    dataT["nom_canal22"] = name_canal22;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal22);
    break;
  case 23:
    strcpy(name_canal23, nom_canal.c_str());
    dataT["nom_canal23"] = name_canal23;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal23);
    break;
  case 24:
    strcpy(name_canal24, nom_canal.c_str());
    dataT["nom_canal24"] = name_canal24;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal24);
    break;
  case 25:
    strcpy(name_canal25, nom_canal.c_str());
    dataT["nom_canal25"] = name_canal25;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal25);
    break;
  case 26:
    strcpy(name_canal26, nom_canal.c_str());
    dataT["nom_canal26"] = name_canal26;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal26);
    break;
  case 27:
    strcpy(name_canal27, nom_canal.c_str());
    dataT["nom_canal27"] = name_canal27;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal27);
    break;
  case 28:
    strcpy(name_canal28, nom_canal.c_str());
    dataT["nom_canal28"] = name_canal28;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal28);
    break;
  case 29:
    strcpy(name_canal29, nom_canal.c_str());
    dataT["nom_canal29"] = name_canal29;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal29);
    break;
  case 30:
    strcpy(name_canal30, nom_canal.c_str());
    dataT["nom_canal30"] = name_canal30;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal30);
    break;
  case 31:
    strcpy(name_canal31, nom_canal.c_str());
    dataT["nom_canal31"] = name_canal31;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal31);
    break;
  case 32:
    strcpy(name_canal32, nom_canal.c_str());
    dataT["nom_canal32"] = name_canal32;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal32);
    break;
  case 33:
    strcpy(name_canal33, nom_canal.c_str());
    dataT["nom_canal33"] = name_canal33;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal33);
    break;
  case 34:
    strcpy(name_canal34, nom_canal.c_str());
    dataT["nom_canal34"] = name_canal34;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal34);
    break;
  case 35:
    strcpy(name_canal35, nom_canal.c_str());
    dataT["nom_canal35"] = name_canal35;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal35);
    break;
  case 36:
    strcpy(name_canal36, nom_canal.c_str());
    dataT["nom_canal36"] = name_canal36;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal36);
    break;
  case 37:
    strcpy(name_canal37, nom_canal.c_str());
    dataT["nom_canal37"] = name_canal37;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal37);
    break;
  case 38:
    strcpy(name_canal38, nom_canal.c_str());
    dataT["nom_canal38"] = name_canal38;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal38);
    break;
  case 39:
    strcpy(name_canal39, nom_canal.c_str());
    dataT["nom_canal39"] = name_canal39;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal39);
    break;
  case 40:
    strcpy(name_canal40, nom_canal.c_str());
    dataT["nom_canal40"] = name_canal40;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal40);
    break;
  case 41:
    strcpy(name_canal41, nom_canal.c_str());
    dataT["nom_canal41"] = name_canal41;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal41);
    break;
  case 42:
    strcpy(name_canal42, nom_canal.c_str());
    dataT["nom_canal42"] = name_canal42;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal42);
    break;
  case 43:
    strcpy(name_canal43, nom_canal.c_str());
    dataT["nom_canal43"] = name_canal43;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal43);
    break;
  case 44:
    strcpy(name_canal44, nom_canal.c_str());
    dataT["nom_canal44"] = name_canal44;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal44);
    break;
  case 45:
    strcpy(name_canal45, nom_canal.c_str());
    dataT["nom_canal45"] = name_canal45;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal45);
    break;
  case 46:
    strcpy(name_canal46, nom_canal.c_str());
    dataT["nom_canal46"] = name_canal46;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal46);
    break;
  case 47:
    strcpy(name_canal47, nom_canal.c_str());
    dataT["nom_canal47"] = name_canal47;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal47);
    break;
  case 48:
    strcpy(name_canal48, nom_canal.c_str());
    dataT["nom_canal48"] = name_canal48;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal48);
    break;
  case 49:
    strcpy(name_canal49, nom_canal.c_str());
    dataT["nom_canal49"] = name_canal49;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal49);
    break;
  case 50:
    strcpy(name_canal50, nom_canal.c_str());
    dataT["nom_canal50"] = name_canal50;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal50);
    break;
  case 51:
    strcpy(name_canal51, nom_canal.c_str());
    dataT["nom_canal51"] = name_canal51;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal51);
    break;
  case 52:
    strcpy(name_canal52, nom_canal.c_str());
    dataT["nom_canal52"] = name_canal52;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal52);
    break;
  case 53:
    strcpy(name_canal53, nom_canal.c_str());
    dataT["nom_canal53"] = name_canal53;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal53);
    break;
  case 54:
    strcpy(name_canal54, nom_canal.c_str());
    dataT["nom_canal54"] = name_canal54;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal54);
    break;
  case 55:
    strcpy(name_canal55, nom_canal.c_str());
    dataT["nom_canal55"] = name_canal55;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal55);
    break;
  case 56:
    strcpy(name_canal56, nom_canal.c_str());
    dataT["nom_canal56"] = name_canal56;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal56);
    break;
  case 57:
    strcpy(name_canal57, nom_canal.c_str());
    dataT["nom_canal57"] = name_canal57;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal57);
    break;
  case 58:
    strcpy(name_canal58, nom_canal.c_str());
    dataT["nom_canal58"] = name_canal58;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal58);
    break;
  case 59:
    strcpy(name_canal59, nom_canal.c_str());
    dataT["nom_canal59"] = name_canal59;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal59);
    break;
  case 60:
    strcpy(name_canal60, nom_canal.c_str());
    dataT["nom_canal60"] = name_canal60;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal60);
    break;
  case 61:
    strcpy(name_canal61, nom_canal.c_str());
    dataT["nom_canal61"] = name_canal61;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal61);
    break;
  case 62:
    strcpy(name_canal62, nom_canal.c_str());
    dataT["nom_canal62"] = name_canal62;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal62);
    break;
  case 63:
    strcpy(name_canal63, nom_canal.c_str());
    dataT["nom_canal63"] = name_canal63;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal63);
    break;
  case 64:
    strcpy(name_canal64, nom_canal.c_str());
    dataT["nom_canal64"] = name_canal64;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal64);
    break;
  case 65:
    strcpy(name_canal65, nom_canal.c_str());
    dataT["nom_canal65"] = name_canal65;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal65);
    break;
  case 66:
    strcpy(name_canal66, nom_canal.c_str());
    dataT["nom_canal66"] = name_canal66;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal66);
    break;
  case 67:
    strcpy(name_canal67, nom_canal.c_str());
    dataT["nom_canal67"] = name_canal67;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal67);
    break;
  case 68:
    strcpy(name_canal68, nom_canal.c_str());
    dataT["nom_canal68"] = name_canal68;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal68);
    break;
  case 69:
    strcpy(name_canal69, nom_canal.c_str());
    dataT["nom_canal69"] = name_canal69;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal69);
    break;
  case 70:
    strcpy(name_canal70, nom_canal.c_str());
    dataT["nom_canal70"] = name_canal70;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal70);
    break;
  case 71:
    strcpy(name_canal71, nom_canal.c_str());
    dataT["nom_canal71"] = name_canal71;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal71);
    break;
  case 72:
    strcpy(name_canal72, nom_canal.c_str());
    dataT["nom_canal72"] = name_canal72;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal72);
    break;
  case 73:
    strcpy(name_canal73, nom_canal.c_str());
    dataT["nom_canal73"] = name_canal73;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal73);
    break;
  case 74:
    strcpy(name_canal74, nom_canal.c_str());
    dataT["nom_canal74"] = name_canal74;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal74);
    break;
  case 75:
    strcpy(name_canal75, nom_canal.c_str());
    dataT["nom_canal75"] = name_canal75;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal75);
    break;
  case 76:
    strcpy(name_canal76, nom_canal.c_str());
    dataT["nom_canal76"] = name_canal76;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal76);
    break;
  case 77:
    strcpy(name_canal77, nom_canal.c_str());
    dataT["nom_canal77"] = name_canal77;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal77);
    break;
  case 78:
    strcpy(name_canal78, nom_canal.c_str());
    dataT["nom_canal78"] = name_canal78;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal78);
    break;
  case 79:
    strcpy(name_canal79, nom_canal.c_str());
    dataT["nom_canal79"] = name_canal79;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal79);
    break;
  case 80:
    strcpy(name_canal80, nom_canal.c_str());
    dataT["nom_canal80"] = name_canal80;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal80);
    break;
  case 81:
    strcpy(name_canal81, nom_canal.c_str());
    dataT["nom_canal81"] = name_canal81;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal81);
    break;
  case 82:
    strcpy(name_canal82, nom_canal.c_str());
    dataT["nom_canal82"] = name_canal82;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal82);
    break;
  case 83:
    strcpy(name_canal83, nom_canal.c_str());
    dataT["nom_canal83"] = name_canal83;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal83);
    break;
  case 84:
    strcpy(name_canal84, nom_canal.c_str());
    dataT["nom_canal84"] = name_canal84;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal84);
    break;
  case 85:
    strcpy(name_canal85, nom_canal.c_str());
    dataT["nom_canal85"] = name_canal85;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal85);
    break;
  case 86:
    strcpy(name_canal86, nom_canal.c_str());
    dataT["nom_canal86"] = name_canal86;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal86);
    break;
  case 87:
    strcpy(name_canal87, nom_canal.c_str());
    dataT["nom_canal87"] = name_canal87;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal87);
    break;
  case 88:
    strcpy(name_canal88, nom_canal.c_str());
    dataT["nom_canal88"] = name_canal88;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal88);
    break;
  case 89:
    strcpy(name_canal89, nom_canal.c_str());
    dataT["nom_canal89"] = name_canal89;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal89);
    break;
  case 90:
    strcpy(name_canal90, nom_canal.c_str());
    dataT["nom_canal90"] = name_canal90;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal90);
    break;
  case 91:
    strcpy(name_canal91, nom_canal.c_str());
    dataT["nom_canal91"] = name_canal91;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal91);
    break;
  case 92:
    strcpy(name_canal92, nom_canal.c_str());
    dataT["nom_canal92"] = name_canal92;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal92);
    break;
  case 93:
    strcpy(name_canal93, nom_canal.c_str());
    dataT["nom_canal93"] = name_canal93;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal93);
    break;
  case 94:
    strcpy(name_canal94, nom_canal.c_str());
    dataT["nom_canal94"] = name_canal94;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal94);
    break;
  case 95:
    strcpy(name_canal95, nom_canal.c_str());
    dataT["nom_canal95"] = name_canal95;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal95);
    break;
  case 96:
    strcpy(name_canal96, nom_canal.c_str());
    dataT["nom_canal96"] = name_canal96;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal96);
    break;
  case 97:
    strcpy(name_canal97, nom_canal.c_str());
    dataT["nom_canal97"] = name_canal97;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal97);
    break;
  case 98:
    strcpy(name_canal98, nom_canal.c_str());
    dataT["nom_canal98"] = name_canal98;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal98);
    break;
  case 99:
    strcpy(name_canal99, nom_canal.c_str());
    dataT["nom_canal99"] = name_canal99;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal99);
    break;
  case 100:
    strcpy(name_canal100, nom_canal.c_str());
    dataT["nom_canal100"] = name_canal100;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal100);
    break;
  case 101:
    strcpy(name_canal101, nom_canal.c_str());
    dataT["nom_canal101"] = name_canal101;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal101);
    break;
  case 102:
    strcpy(name_canal102, nom_canal.c_str());
    dataT["nom_canal102"] = name_canal102;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal102);
    break;
  case 103:
    strcpy(name_canal103, nom_canal.c_str());
    dataT["nom_canal103"] = name_canal103;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal103);
    break;
  case 104:
    strcpy(name_canal104, nom_canal.c_str());
    dataT["nom_canal104"] = name_canal104;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal104);
    break;
  case 105:
    strcpy(name_canal105, nom_canal.c_str());
    dataT["nom_canal105"] = name_canal105;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal105);
    break;
  case 106:
    strcpy(name_canal106, nom_canal.c_str());
    dataT["nom_canal106"] = name_canal106;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal106);
    break;
  case 107:
    strcpy(name_canal107, nom_canal.c_str());
    dataT["nom_canal107"] = name_canal107;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal107);
    break;
  case 108:
    strcpy(name_canal108, nom_canal.c_str());
    dataT["nom_canal108"] = name_canal108;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal108);
    break;
  case 109:
    strcpy(name_canal109, nom_canal.c_str());
    dataT["nom_canal109"] = name_canal109;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal109);
    break;
  case 110:
    strcpy(name_canal110, nom_canal.c_str());
    dataT["nom_canal110"] = name_canal110;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal110);
    break;
  case 111:
    strcpy(name_canal111, nom_canal.c_str());
    dataT["nom_canal111"] = name_canal111;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal111);
    break;
  case 112:
    strcpy(name_canal112, nom_canal.c_str());
    dataT["nom_canal112"] = name_canal112;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal112);
    break;
  case 113:
    strcpy(name_canal113, nom_canal.c_str());
    dataT["nom_canal113"] = name_canal113;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal113);
    break;
  case 114:
    strcpy(name_canal114, nom_canal.c_str());
    dataT["nom_canal114"] = name_canal114;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal114);
    break;
  case 115:
    strcpy(name_canal115, nom_canal.c_str());
    dataT["nom_canal115"] = name_canal115;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal115);
    break;
  case 116:
    strcpy(name_canal116, nom_canal.c_str());
    dataT["nom_canal116"] = name_canal116;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal116);
    break;
  case 117:
    strcpy(name_canal117, nom_canal.c_str());
    dataT["nom_canal117"] = name_canal117;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal117);
    break;
  case 118:
    strcpy(name_canal118, nom_canal.c_str());
    dataT["nom_canal118"] = name_canal118;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal118);
    break;
  case 119:
    strcpy(name_canal119, nom_canal.c_str());
    dataT["nom_canal119"] = name_canal119;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal119);
    break;
  case 120:
    strcpy(name_canal120, nom_canal.c_str());
    dataT["nom_canal120"] = name_canal120;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal120);
    break;
  case 121:
    strcpy(name_canal121, nom_canal.c_str());
    dataT["nom_canal121"] = name_canal121;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal121);
    break;
  case 122:
    strcpy(name_canal122, nom_canal.c_str());
    dataT["nom_canal122"] = name_canal122;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal122);
    break;
  case 123:
    strcpy(name_canal123, nom_canal.c_str());
    dataT["nom_canal123"] = name_canal123;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal123);
    break;
  case 124:
    strcpy(name_canal124, nom_canal.c_str());
    dataT["nom_canal124"] = name_canal124;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal124);
    break;
  case 125:
    strcpy(name_canal125, nom_canal.c_str());
    dataT["nom_canal125"] = name_canal125;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal125);
    break;
  case 126:
    strcpy(name_canal126, nom_canal.c_str());
    dataT["nom_canal126"] = name_canal126;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal126);
    break;
  case 127:
    strcpy(name_canal127, nom_canal.c_str());
    dataT["nom_canal127"] = name_canal127;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal127);
    break;
  case 128:
    strcpy(name_canal128, nom_canal.c_str());
    dataT["nom_canal128"] = name_canal128;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal128);
    break;
  case 129:
    strcpy(name_canal129, nom_canal.c_str());
    dataT["nom_canal129"] = name_canal129;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal129);
    break;
  case 130:
    strcpy(name_canal130, nom_canal.c_str());
    dataT["nom_canal130"] = name_canal130;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal130);
    break;
  case 131:
    strcpy(name_canal131, nom_canal.c_str());
    dataT["nom_canal131"] = name_canal131;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal131);
    break;
  case 132:
    strcpy(name_canal132, nom_canal.c_str());
    dataT["nom_canal132"] = name_canal132;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal132);
    break;
  case 133:
    strcpy(name_canal133, nom_canal.c_str());
    dataT["nom_canal133"] = name_canal133;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal133);
    break;
  case 134:
    strcpy(name_canal134, nom_canal.c_str());
    dataT["nom_canal134"] = name_canal134;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal134);
    break;
  case 135:
    strcpy(name_canal135, nom_canal.c_str());
    dataT["nom_canal135"] = name_canal135;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal135);
    break;
  case 136:
    strcpy(name_canal136, nom_canal.c_str());
    dataT["nom_canal136"] = name_canal136;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal136);
    break;
  case 137:
    strcpy(name_canal137, nom_canal.c_str());
    dataT["nom_canal137"] = name_canal137;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal137);
    break;
  case 138:
    strcpy(name_canal138, nom_canal.c_str());
    dataT["nom_canal138"] = name_canal138;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal138);
    break;
  case 139:
    strcpy(name_canal139, nom_canal.c_str());
    dataT["nom_canal139"] = name_canal139;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal139);
    break;
  case 140:
    strcpy(name_canal140, nom_canal.c_str());
    dataT["nom_canal140"] = name_canal140;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal140);
    break;
  case 141:
    strcpy(name_canal141, nom_canal.c_str());
    dataT["nom_canal141"] = name_canal141;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal141);
    break;
  case 142:
    strcpy(name_canal142, nom_canal.c_str());
    dataT["nom_canal142"] = name_canal142;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal142);
    break;
  case 143:
    strcpy(name_canal143, nom_canal.c_str());
    dataT["nom_canal143"] = name_canal143;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal143);
    break;
  case 144:
    strcpy(name_canal144, nom_canal.c_str());
    dataT["nom_canal144"] = name_canal144;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal144);
    break;
  case 145:
    strcpy(name_canal145, nom_canal.c_str());
    dataT["nom_canal145"] = name_canal145;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal145);
    break;
  case 146:
    strcpy(name_canal146, nom_canal.c_str());
    dataT["nom_canal146"] = name_canal146;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal146);
    break;
  case 147:
    strcpy(name_canal147, nom_canal.c_str());
    dataT["nom_canal147"] = name_canal147;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal147);
    break;
  case 148:
    strcpy(name_canal148, nom_canal.c_str());
    dataT["nom_canal148"] = name_canal148;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal148);
    break;
  case 149:
    strcpy(name_canal149, nom_canal.c_str());
    dataT["nom_canal149"] = name_canal149;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal149);
    break;
  case 150:
    strcpy(name_canal150, nom_canal.c_str());
    dataT["nom_canal150"] = name_canal150;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal150);
    break;
  case 151:
    strcpy(name_canal151, nom_canal.c_str());
    dataT["nom_canal151"] = name_canal151;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal151);
    break;
  case 152:
    strcpy(name_canal152, nom_canal.c_str());
    dataT["nom_canal152"] = name_canal152;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal152);
    break;
  case 153:
    strcpy(name_canal153, nom_canal.c_str());
    dataT["nom_canal153"] = name_canal153;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal153);
    break;
  case 154:
    strcpy(name_canal154, nom_canal.c_str());
    dataT["nom_canal154"] = name_canal154;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal154);
    break;
  case 155:
    strcpy(name_canal155, nom_canal.c_str());
    dataT["nom_canal155"] = name_canal155;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal155);
    break;
  case 156:
    strcpy(name_canal156, nom_canal.c_str());
    dataT["nom_canal156"] = name_canal156;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal156);
    break;
  case 157:
    strcpy(name_canal157, nom_canal.c_str());
    dataT["nom_canal157"] = name_canal157;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal157);
    break;
  case 158:
    strcpy(name_canal158, nom_canal.c_str());
    dataT["nom_canal158"] = name_canal158;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal158);
    break;
  case 159:
    strcpy(name_canal159, nom_canal.c_str());
    dataT["nom_canal159"] = name_canal159;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal159);
    break;
  case 160:
    strcpy(name_canal160, nom_canal.c_str());
    dataT["nom_canal160"] = name_canal160;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal160);
    break;
  case 161:
    strcpy(name_canal161, nom_canal.c_str());
    dataT["nom_canal161"] = name_canal161;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal161);
    break;
  case 162:
    strcpy(name_canal162, nom_canal.c_str());
    dataT["nom_canal162"] = name_canal162;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal162);
    break;
  case 163:
    strcpy(name_canal163, nom_canal.c_str());
    dataT["nom_canal163"] = name_canal163;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal163);
    break;
  case 164:
    strcpy(name_canal164, nom_canal.c_str());
    dataT["nom_canal164"] = name_canal164;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal164);
    break;
  case 165:
    strcpy(name_canal165, nom_canal.c_str());
    dataT["nom_canal165"] = name_canal165;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal165);
    break;
  case 166:
    strcpy(name_canal166, nom_canal.c_str());
    dataT["nom_canal166"] = name_canal166;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal166);
    break;
  case 167:
    strcpy(name_canal167, nom_canal.c_str());
    dataT["nom_canal167"] = name_canal167;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal167);
    break;
  case 168:
    strcpy(name_canal168, nom_canal.c_str());
    dataT["nom_canal168"] = name_canal168;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal168);
    break;
  case 169:
    strcpy(name_canal169, nom_canal.c_str());
    dataT["nom_canal169"] = name_canal169;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal169);
    break;
  case 170:
    strcpy(name_canal170, nom_canal.c_str());
    dataT["nom_canal170"] = name_canal170;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal170);
    break;
  case 171:
    strcpy(name_canal171, nom_canal.c_str());
    dataT["nom_canal171"] = name_canal171;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal171);
    break;
  case 172:
    strcpy(name_canal172, nom_canal.c_str());
    dataT["nom_canal172"] = name_canal172;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal172);
    break;
  case 173:
    strcpy(name_canal173, nom_canal.c_str());
    dataT["nom_canal173"] = name_canal173;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal173);
    break;
  case 174:
    strcpy(name_canal174, nom_canal.c_str());
    dataT["nom_canal174"] = name_canal174;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal174);
    break;
  case 175:
    strcpy(name_canal175, nom_canal.c_str());
    dataT["nom_canal175"] = name_canal175;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal175);
    break;
  case 176:
    strcpy(name_canal176, nom_canal.c_str());
    dataT["nom_canal176"] = name_canal176;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal176);
    break;
  case 177:
    strcpy(name_canal177, nom_canal.c_str());
    dataT["nom_canal177"] = name_canal177;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal177);
    break;
  case 178:
    strcpy(name_canal178, nom_canal.c_str());
    dataT["nom_canal178"] = name_canal178;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal178);
    break;
  case 179:
    strcpy(name_canal179, nom_canal.c_str());
    dataT["nom_canal179"] = name_canal179;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal179);
    break;
  case 180:
    strcpy(name_canal180, nom_canal.c_str());
    dataT["nom_canal180"] = name_canal180;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal180);
    break;
  case 181:
    strcpy(name_canal181, nom_canal.c_str());
    dataT["nom_canal181"] = name_canal181;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal181);
    break;
  case 182:
    strcpy(name_canal182, nom_canal.c_str());
    dataT["nom_canal182"] = name_canal182;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal182);
    break;
  case 183:
    strcpy(name_canal183, nom_canal.c_str());
    dataT["nom_canal183"] = name_canal183;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal183);
    break;
  case 184:
    strcpy(name_canal184, nom_canal.c_str());
    dataT["nom_canal184"] = name_canal184;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal184);
    break;
  case 185:
    strcpy(name_canal185, nom_canal.c_str());
    dataT["nom_canal185"] = name_canal185;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal185);
    break;
  case 186:
    strcpy(name_canal186, nom_canal.c_str());
    dataT["nom_canal186"] = name_canal186;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal186);
    break;
  case 187:
    strcpy(name_canal187, nom_canal.c_str());
    dataT["nom_canal187"] = name_canal187;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal187);
    break;
  case 188:
    strcpy(name_canal188, nom_canal.c_str());
    dataT["nom_canal188"] = name_canal188;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal188);
    break;
  case 189:
    strcpy(name_canal189, nom_canal.c_str());
    dataT["nom_canal189"] = name_canal189;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal189);
    break;
  case 190:
    strcpy(name_canal190, nom_canal.c_str());
    dataT["nom_canal190"] = name_canal190;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal190);
    break;
  case 191:
    strcpy(name_canal191, nom_canal.c_str());
    dataT["nom_canal191"] = name_canal191;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal191);
    break;
  case 192:
    strcpy(name_canal192, nom_canal.c_str());
    dataT["nom_canal192"] = name_canal192;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal192);
    break;
  case 193:
    strcpy(name_canal193, nom_canal.c_str());
    dataT["nom_canal193"] = name_canal193;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal193);
    break;
  case 194:
    strcpy(name_canal194, nom_canal.c_str());
    dataT["nom_canal194"] = name_canal194;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal194);
    break;
  case 195:
    strcpy(name_canal195, nom_canal.c_str());
    dataT["nom_canal195"] = name_canal195;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal195);
    break;
  case 196:
    strcpy(name_canal196, nom_canal.c_str());
    dataT["nom_canal196"] = name_canal196;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal196);
    break;
  case 197:
    strcpy(name_canal197, nom_canal.c_str());
    dataT["nom_canal197"] = name_canal197;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal197);
    break;
  case 198:
    strcpy(name_canal198, nom_canal.c_str());
    dataT["nom_canal198"] = name_canal198;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal198);
    break;
  case 199:
    strcpy(name_canal199, nom_canal.c_str());
    dataT["nom_canal199"] = name_canal199;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal199);
    break;
  case 200:
    strcpy(name_canal200, nom_canal.c_str());
    dataT["nom_canal200"] = name_canal200;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal200);
    break;
  case 201:
    strcpy(name_canal201, nom_canal.c_str());
    dataT["nom_canal201"] = name_canal201;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal201);
    break;
  case 202:
    strcpy(name_canal202, nom_canal.c_str());
    dataT["nom_canal202"] = name_canal202;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal202);
    break;
  case 203:
    strcpy(name_canal203, nom_canal.c_str());
    dataT["nom_canal203"] = name_canal203;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal203);
    break;
  case 204:
    strcpy(name_canal204, nom_canal.c_str());
    dataT["nom_canal204"] = name_canal204;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal204);
    break;
  case 205:
    strcpy(name_canal205, nom_canal.c_str());
    dataT["nom_canal205"] = name_canal205;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal205);
    break;
  case 206:
    strcpy(name_canal206, nom_canal.c_str());
    dataT["nom_canal206"] = name_canal206;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal206);
    break;
  case 207:
    strcpy(name_canal207, nom_canal.c_str());
    dataT["nom_canal207"] = name_canal207;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal207);
    break;
  case 208:
    strcpy(name_canal208, nom_canal.c_str());
    dataT["nom_canal208"] = name_canal208;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal208);
    break;
  case 209:
    strcpy(name_canal209, nom_canal.c_str());
    dataT["nom_canal209"] = name_canal209;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal209);
    break;
  case 210:
    strcpy(name_canal210, nom_canal.c_str());
    dataT["nom_canal210"] = name_canal210;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal210);
    break;
  case 211:
    strcpy(name_canal211, nom_canal.c_str());
    dataT["nom_canal211"] = name_canal211;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal211);
    break;
  case 212:
    strcpy(name_canal212, nom_canal.c_str());
    dataT["nom_canal212"] = name_canal212;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal212);
    break;
  case 213:
    strcpy(name_canal213, nom_canal.c_str());
    dataT["nom_canal213"] = name_canal213;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal213);
    Serial.println(start_name_adrr2 + (addr_name * nb_name));
    Serial.println(myEEP.read(start_name_adrr2 + (addr_name * nb_name) + 1));
    break;
  case 214:
    strcpy(name_canal214, nom_canal.c_str());
    dataT["nom_canal214"] = name_canal214;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal214);
    break;
  case 215:
    strcpy(name_canal215, nom_canal.c_str());
    dataT["nom_canal215"] = name_canal215;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal215);
    break;
  case 216:
    strcpy(name_canal216, nom_canal.c_str());
    dataT["nom_canal216"] = name_canal216;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal216);
    break;
  case 217:
    strcpy(name_canal217, nom_canal.c_str());
    dataT["nom_canal217"] = name_canal217;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal217);
    break;
  case 218:
    strcpy(name_canal218, nom_canal.c_str());
    dataT["nom_canal218"] = name_canal218;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal218);
    break;
  case 219:
    strcpy(name_canal219, nom_canal.c_str());
    dataT["nom_canal219"] = name_canal219;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal219);
    break;
  case 220:
    strcpy(name_canal220, nom_canal.c_str());
    dataT["nom_canal220"] = name_canal220;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal220);
    break;
  case 221:
    strcpy(name_canal221, nom_canal.c_str());
    dataT["nom_canal221"] = name_canal221;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal221);
    break;
  case 222:
    strcpy(name_canal222, nom_canal.c_str());
    dataT["nom_canal222"] = name_canal222;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal222);
    break;
  case 223:
    strcpy(name_canal223, nom_canal.c_str());
    dataT["nom_canal223"] = name_canal223;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal223);
    break;
  case 224:
    strcpy(name_canal224, nom_canal.c_str());
    dataT["nom_canal224"] = name_canal224;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal224);
    break;
  case 225:
    strcpy(name_canal225, nom_canal.c_str());
    dataT["nom_canal225"] = name_canal225;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal225);
    break;
  case 226:
    strcpy(name_canal226, nom_canal.c_str());
    dataT["nom_canal226"] = name_canal226;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal226);
    break;
  case 227:
    strcpy(name_canal227, nom_canal.c_str());
    dataT["nom_canal227"] = name_canal227;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal227);
    break;
  case 228:
    strcpy(name_canal228, nom_canal.c_str());
    dataT["nom_canal228"] = name_canal228;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal228);
    break;
  case 229:
    strcpy(name_canal229, nom_canal.c_str());
    dataT["nom_canal229"] = name_canal229;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal229);
    break;
  case 230:
    strcpy(name_canal230, nom_canal.c_str());
    dataT["nom_canal230"] = name_canal230;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal230);
    break;
  case 231:
    strcpy(name_canal231, nom_canal.c_str());
    dataT["nom_canal231"] = name_canal231;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal231);
    break;
  case 232:
    strcpy(name_canal232, nom_canal.c_str());
    dataT["nom_canal232"] = name_canal232;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal232);
    break;
  case 233:
    strcpy(name_canal233, nom_canal.c_str());
    dataT["nom_canal233"] = name_canal233;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal233);
    break;
  case 234:
    strcpy(name_canal234, nom_canal.c_str());
    dataT["nom_canal234"] = name_canal234;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal234);
    break;
  case 235:
    strcpy(name_canal235, nom_canal.c_str());
    dataT["nom_canal235"] = name_canal235;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal235);
    break;
  case 236:
    strcpy(name_canal236, nom_canal.c_str());
    dataT["nom_canal236"] = name_canal236;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal236);
    break;
  case 237:
    strcpy(name_canal237, nom_canal.c_str());
    dataT["nom_canal237"] = name_canal237;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal237);
    break;
  case 238:
    strcpy(name_canal238, nom_canal.c_str());
    dataT["nom_canal238"] = name_canal238;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal238);
    break;
  case 239:
    strcpy(name_canal239, nom_canal.c_str());
    dataT["nom_canal239"] = name_canal239;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal239);
    break;
  case 240:
    strcpy(name_canal240, nom_canal.c_str());
    dataT["nom_canal240"] = name_canal240;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal240);
    break;
  case 241:
    strcpy(name_canal241, nom_canal.c_str());
    dataT["nom_canal241"] = name_canal241;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal241);
    break;
  case 242:
    strcpy(name_canal242, nom_canal.c_str());
    dataT["nom_canal242"] = name_canal242;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal242);
    break;
  case 243:
    strcpy(name_canal243, nom_canal.c_str());
    dataT["nom_canal243"] = name_canal243;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal243);
    break;
  case 244:
    strcpy(name_canal244, nom_canal.c_str());
    dataT["nom_canal244"] = name_canal244;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal244);
    break;
  case 245:
    strcpy(name_canal245, nom_canal.c_str());
    dataT["nom_canal245"] = name_canal245;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal245);
    break;
  case 246:
    strcpy(name_canal246, nom_canal.c_str());
    dataT["nom_canal246"] = name_canal246;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal246);
    break;
  case 247:
    strcpy(name_canal247, nom_canal.c_str());
    dataT["nom_canal247"] = name_canal247;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal247);
    break;
  case 248:
    strcpy(name_canal248, nom_canal.c_str());
    dataT["nom_canal248"] = name_canal248;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal248);
    break;
  case 249:
    strcpy(name_canal249, nom_canal.c_str());
    dataT["nom_canal249"] = name_canal249;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal249);
    break;
  case 250:
    strcpy(name_canal250, nom_canal.c_str());
    dataT["nom_canal250"] = name_canal250;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal250);
    break;
  case 251:
    strcpy(name_canal251, nom_canal.c_str());
    dataT["nom_canal251"] = name_canal251;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal251);
    break;
  case 252:
    strcpy(name_canal252, nom_canal.c_str());
    dataT["nom_canal252"] = name_canal252;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal252);
    break;
  case 253:
    strcpy(name_canal253, nom_canal.c_str());
    dataT["nom_canal253"] = name_canal253;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal253);
    break;
  case 254:
    strcpy(name_canal254, nom_canal.c_str());
    dataT["nom_canal254"] = name_canal254;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal254);
    break;
  case 255:
    strcpy(name_canal255, nom_canal.c_str());
    dataT["nom_canal255"] = name_canal255;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal255);
    break;
  case 256:
    strcpy(name_canal256, nom_canal.c_str());
    dataT["nom_canal256"] = name_canal256;
    myEEP.put(start_name_adrr2 + (addr_name * nb_name) + 1, name_canal256);
    break;
  default:
    dataT["nom_canal1"] = "bug";
    break;
  }
}

void invert_nom_canal(int canal_name)
{
  int notzero = 0;
  switch (canal_name)
  {
  case 1:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal1);
    notzero = strlen(name_canal1);
    if (notzero == 0)
    {
      sprintf(name_canal1, "canal1");
    }
    break;
  case 2:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal2);
    notzero = strlen(name_canal2);
    if (notzero == 0)
    {
      sprintf(name_canal2, "canal2");
    }
    break;
  case 3:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal3);
    notzero = strlen(name_canal3);
    if (notzero == 0)
    {
      sprintf(name_canal3, "canal3");
    }
    break;
  case 4:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal4);
    notzero = strlen(name_canal4);
    if (notzero == 0)
    {
      sprintf(name_canal4, "canal4");
    }
    break;
  case 5:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal5);
    notzero = strlen(name_canal5);
    if (notzero == 0)
    {
      sprintf(name_canal5, "canal5");
    }
    break;
  case 6:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal6);
    notzero = strlen(name_canal6);
    if (notzero == 0)
    {
      sprintf(name_canal6, "canal6");
    }
    break;
  case 7:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal7);
    notzero = strlen(name_canal7);
    if (notzero == 0)
    {
      sprintf(name_canal7, "canal7");
    }
    break;
  case 8:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal8);
    notzero = strlen(name_canal8);
    if (notzero == 0)
    {
      sprintf(name_canal8, "canal8");
    }
    break;
  case 9:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal9);
    notzero = strlen(name_canal9);
    if (notzero == 0)
    {
      sprintf(name_canal9, "canal9");
    }
    break;
  case 10:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal10);
    notzero = strlen(name_canal10);
    if (notzero == 0)
    {
      sprintf(name_canal10, "canal10");
    }
    break;
  case 11:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal11);
    notzero = strlen(name_canal11);
    if (notzero == 0)
    {
      sprintf(name_canal11, "canal11");
    }
    break;
  case 12:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal12);
    notzero = strlen(name_canal12);
    if (notzero == 0)
    {
      sprintf(name_canal12, "canal12");
    }
    break;
  case 13:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal13);
    notzero = strlen(name_canal13);
    if (notzero == 0)
    {
      sprintf(name_canal13, "canal13");
    }
    break;
  case 14:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal14);
    notzero = strlen(name_canal14);
    if (notzero == 0)
    {
      sprintf(name_canal14, "canal14");
    }
    break;
  case 15:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal15);
    notzero = strlen(name_canal15);
    if (notzero == 0)
    {
      sprintf(name_canal15, "canal15");
    }
    break;
  case 16:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal16);
    notzero = strlen(name_canal16);
    if (notzero == 0)
    {
      sprintf(name_canal16, "canal16");
    }
    break;
  case 17:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal17);
    notzero = strlen(name_canal17);
    if (notzero == 0)
    {
      sprintf(name_canal17, "canal17");
    }
    break;
  case 18:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal18);
    notzero = strlen(name_canal18);
    if (notzero == 0)
    {
      sprintf(name_canal18, "canal18");
    }
    break;
  case 19:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal19);
    notzero = strlen(name_canal19);
    if (notzero == 0)
    {
      sprintf(name_canal19, "canal19");
    }
    break;
  case 20:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal20);
    notzero = strlen(name_canal20);
    if (notzero == 0)
    {
      sprintf(name_canal20, "canal20");
    }
    break;
  case 21:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal21);
    notzero = strlen(name_canal21);
    if (notzero == 0)
    {
      sprintf(name_canal21, "canal21");
    }
    break;
  case 22:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal22);
    notzero = strlen(name_canal22);
    if (notzero == 0)
    {
      sprintf(name_canal22, "canal22");
    }
    break;
  case 23:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal23);
    notzero = strlen(name_canal23);
    if (notzero == 0)
    {
      sprintf(name_canal23, "canal23");
    }
    break;
  case 24:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal24);
    notzero = strlen(name_canal24);
    if (notzero == 0)
    {
      sprintf(name_canal24, "canal24");
    }
    break;
  case 25:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal25);
    notzero = strlen(name_canal25);
    if (notzero == 0)
    {
      sprintf(name_canal25, "canal25");
    }
    break;
  case 26:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal26);
    notzero = strlen(name_canal26);
    if (notzero == 0)
    {
      sprintf(name_canal26, "canal26");
    }
    break;
  case 27:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal27);
    notzero = strlen(name_canal27);
    if (notzero == 0)
    {
      sprintf(name_canal27, "canal27");
    }
    break;
  case 28:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal28);
    notzero = strlen(name_canal28);
    if (notzero == 0)
    {
      sprintf(name_canal28, "canal28");
    }
    break;
  case 29:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal29);
    notzero = strlen(name_canal29);
    if (notzero == 0)
    {
      sprintf(name_canal29, "canal29");
    }
    break;
  case 30:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal30);
    notzero = strlen(name_canal30);
    if (notzero == 0)
    {
      sprintf(name_canal30, "canal30");
    }
    break;
  case 31:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal31);
    notzero = strlen(name_canal31);
    if (notzero == 0)
    {
      sprintf(name_canal31, "canal31");
    }
    break;
  case 32:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal32);
    notzero = strlen(name_canal32);
    if (notzero == 0)
    {
      sprintf(name_canal32, "canal32");
    }
    break;
  case 33:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal33);
    notzero = strlen(name_canal33);
    if (notzero == 0)
    {
      sprintf(name_canal33, "canal33");
    }
    break;
  case 34:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal34);
    notzero = strlen(name_canal34);
    if (notzero == 0)
    {
      sprintf(name_canal34, "canal34");
    }
    break;
  case 35:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal35);
    notzero = strlen(name_canal35);
    if (notzero == 0)
    {
      sprintf(name_canal35, "canal35");
    }
    break;
  case 36:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal36);
    notzero = strlen(name_canal36);
    if (notzero == 0)
    {
      sprintf(name_canal36, "canal36");
    }
    break;
  case 37:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal37);
    notzero = strlen(name_canal37);
    if (notzero == 0)
    {
      sprintf(name_canal37, "canal37");
    }
    break;
  case 38:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal38);
    notzero = strlen(name_canal38);
    if (notzero == 0)
    {
      sprintf(name_canal38, "canal38");
    }
    break;
  case 39:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal39);
    notzero = strlen(name_canal39);
    if (notzero == 0)
    {
      sprintf(name_canal39, "canal39");
    }
    break;
  case 40:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal40);
    notzero = strlen(name_canal40);
    if (notzero == 0)
    {
      sprintf(name_canal40, "canal40");
    }
    break;
  case 41:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal41);
    notzero = strlen(name_canal41);
    if (notzero == 0)
    {
      sprintf(name_canal41, "canal41");
    }
    break;
  case 42:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal42);
    notzero = strlen(name_canal42);
    if (notzero == 0)
    {
      sprintf(name_canal42, "canal42");
    }
    break;
  case 43:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal43);
    notzero = strlen(name_canal43);
    if (notzero == 0)
    {
      sprintf(name_canal43, "canal43");
    }
    break;
  case 44:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal44);
    notzero = strlen(name_canal44);
    if (notzero == 0)
    {
      sprintf(name_canal44, "canal44");
    }
    break;
  case 45:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal45);
    notzero = strlen(name_canal45);
    if (notzero == 0)
    {
      sprintf(name_canal45, "canal45");
    }
    break;
  case 46:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal46);
    notzero = strlen(name_canal46);
    if (notzero == 0)
    {
      sprintf(name_canal46, "canal46");
    }
    break;
  case 47:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal47);
    notzero = strlen(name_canal47);
    if (notzero == 0)
    {
      sprintf(name_canal47, "canal47");
    }
    break;
  case 48:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal48);
    notzero = strlen(name_canal48);
    if (notzero == 0)
    {
      sprintf(name_canal48, "canal48");
    }
    break;
  case 49:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal49);
    notzero = strlen(name_canal49);
    if (notzero == 0)
    {
      sprintf(name_canal49, "canal49");
    }
    break;
  case 50:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal50);
    notzero = strlen(name_canal50);
    if (notzero == 0)
    {
      sprintf(name_canal50, "canal50");
    }
    break;
  case 51:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal51);
    notzero = strlen(name_canal51);
    if (notzero == 0)
    {
      sprintf(name_canal51, "canal51");
    }
    break;
  case 52:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal52);
    notzero = strlen(name_canal52);
    if (notzero == 0)
    {
      sprintf(name_canal52, "canal52");
    }
    break;
  case 53:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal53);
    notzero = strlen(name_canal53);
    if (notzero == 0)
    {
      sprintf(name_canal53, "canal53");
    }
    break;
  case 54:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal54);
    notzero = strlen(name_canal54);
    if (notzero == 0)
    {
      sprintf(name_canal54, "canal54");
    }
    break;
  case 55:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal55);
    notzero = strlen(name_canal55);
    if (notzero == 0)
    {
      sprintf(name_canal55, "canal55");
    }
    break;
  case 56:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal56);
    notzero = strlen(name_canal56);
    if (notzero == 0)
    {
      sprintf(name_canal56, "canal56");
    }
    break;
  case 57:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal57);
    notzero = strlen(name_canal57);
    if (notzero == 0)
    {
      sprintf(name_canal57, "canal57");
    }
    break;
  case 58:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal58);
    notzero = strlen(name_canal58);
    if (notzero == 0)
    {
      sprintf(name_canal58, "canal58");
    }
    break;
  case 59:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal59);
    notzero = strlen(name_canal59);
    if (notzero == 0)
    {
      sprintf(name_canal59, "canal59");
    }
    break;
  case 60:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal60);
    notzero = strlen(name_canal60);
    if (notzero == 0)
    {
      sprintf(name_canal60, "canal60");
    }
    break;
  case 61:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal61);
    notzero = strlen(name_canal61);
    if (notzero == 0)
    {
      sprintf(name_canal61, "canal61");
    }
    break;
  case 62:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal62);
    notzero = strlen(name_canal62);
    if (notzero == 0)
    {
      sprintf(name_canal62, "canal62");
    }
    break;
  case 63:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal63);
    notzero = strlen(name_canal63);
    if (notzero == 0)
    {
      sprintf(name_canal63, "canal63");
    }
    break;
  case 64:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal64);
    notzero = strlen(name_canal64);
    if (notzero == 0)
    {
      sprintf(name_canal64, "canal64");
    }
    break;
  case 65:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal65);
    notzero = strlen(name_canal65);
    if (notzero == 0)
    {
      sprintf(name_canal65, "canal65");
    }
    break;
  case 66:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal66);
    notzero = strlen(name_canal66);
    if (notzero == 0)
    {
      sprintf(name_canal66, "canal66");
    }
    break;
  case 67:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal67);
    notzero = strlen(name_canal67);
    if (notzero == 0)
    {
      sprintf(name_canal67, "canal67");
    }
    break;
  case 68:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal68);
    notzero = strlen(name_canal68);
    if (notzero == 0)
    {
      sprintf(name_canal68, "canal68");
    }
    break;
  case 69:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal69);
    notzero = strlen(name_canal69);
    if (notzero == 0)
    {
      sprintf(name_canal69, "canal69");
    }
    break;
  case 70:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal70);
    notzero = strlen(name_canal70);
    if (notzero == 0)
    {
      sprintf(name_canal70, "canal70");
    }
    break;
  case 71:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal71);
    notzero = strlen(name_canal71);
    if (notzero == 0)
    {
      sprintf(name_canal71, "canal71");
    }
    break;
  case 72:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal72);
    notzero = strlen(name_canal72);
    if (notzero == 0)
    {
      sprintf(name_canal72, "canal72");
    }
    break;
  case 73:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal73);
    notzero = strlen(name_canal73);
    if (notzero == 0)
    {
      sprintf(name_canal73, "canal73");
    }
    break;
  case 74:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal74);
    notzero = strlen(name_canal74);
    if (notzero == 0)
    {
      sprintf(name_canal74, "canal74");
    }
    break;
  case 75:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal75);
    notzero = strlen(name_canal75);
    if (notzero == 0)
    {
      sprintf(name_canal75, "canal75");
    }
    break;
  case 76:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal76);
    notzero = strlen(name_canal76);
    if (notzero == 0)
    {
      sprintf(name_canal76, "canal76");
    }
    break;
  case 77:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal77);
    notzero = strlen(name_canal77);
    if (notzero == 0)
    {
      sprintf(name_canal77, "canal77");
    }
    break;
  case 78:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal78);
    notzero = strlen(name_canal78);
    if (notzero == 0)
    {
      sprintf(name_canal78, "canal78");
    }
    break;
  case 79:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal79);
    notzero = strlen(name_canal79);
    if (notzero == 0)
    {
      sprintf(name_canal79, "canal79");
    }
    break;
  case 80:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal80);
    notzero = strlen(name_canal80);
    if (notzero == 0)
    {
      sprintf(name_canal80, "canal80");
    }
    break;
  case 81:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal81);
    notzero = strlen(name_canal81);
    if (notzero == 0)
    {
      sprintf(name_canal81, "canal81");
    }
    break;
  case 82:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal82);
    notzero = strlen(name_canal82);
    if (notzero == 0)
    {
      sprintf(name_canal82, "canal82");
    }
    break;
  case 83:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal83);
    notzero = strlen(name_canal83);
    if (notzero == 0)
    {
      sprintf(name_canal83, "canal83");
    }
    break;
  case 84:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal84);
    notzero = strlen(name_canal84);
    if (notzero == 0)
    {
      sprintf(name_canal84, "canal84");
    }
    break;
  case 85:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal85);
    notzero = strlen(name_canal85);
    if (notzero == 0)
    {
      sprintf(name_canal85, "canal85");
    }
    break;
  case 86:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal86);
    notzero = strlen(name_canal86);
    if (notzero == 0)
    {
      sprintf(name_canal86, "canal86");
    }
    break;
  case 87:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal87);
    notzero = strlen(name_canal87);
    if (notzero == 0)
    {
      sprintf(name_canal87, "canal87");
    }
    break;
  case 88:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal88);
    notzero = strlen(name_canal88);
    if (notzero == 0)
    {
      sprintf(name_canal88, "canal88");
    }
    break;
  case 89:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal89);
    notzero = strlen(name_canal89);
    if (notzero == 0)
    {
      sprintf(name_canal89, "canal89");
    }
    break;
  case 90:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal90);
    notzero = strlen(name_canal90);
    if (notzero == 0)
    {
      sprintf(name_canal90, "canal90");
    }
    break;
  case 91:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal91);
    notzero = strlen(name_canal91);
    if (notzero == 0)
    {
      sprintf(name_canal91, "canal91");
    }
    break;
  case 92:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal92);
    notzero = strlen(name_canal92);
    if (notzero == 0)
    {
      sprintf(name_canal92, "canal92");
    }
    break;
  case 93:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal93);
    notzero = strlen(name_canal93);
    if (notzero == 0)
    {
      sprintf(name_canal93, "canal93");
    }
    break;
  case 94:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal94);
    notzero = strlen(name_canal94);
    if (notzero == 0)
    {
      sprintf(name_canal94, "canal94");
    }
    break;
  case 95:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal95);
    notzero = strlen(name_canal95);
    if (notzero == 0)
    {
      sprintf(name_canal95, "canal95");
    }
    break;
  case 96:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal96);
    notzero = strlen(name_canal96);
    if (notzero == 0)
    {
      sprintf(name_canal96, "canal96");
    }
    break;
  case 97:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal97);
    notzero = strlen(name_canal97);
    if (notzero == 0)
    {
      sprintf(name_canal97, "canal97");
    }
    break;
  case 98:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal98);
    notzero = strlen(name_canal98);
    if (notzero == 0)
    {
      sprintf(name_canal98, "canal98");
    }
    break;
  case 99:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal99);
    notzero = strlen(name_canal99);
    if (notzero == 0)
    {
      sprintf(name_canal99, "canal99");
    }
    break;
  case 100:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal100);
    notzero = strlen(name_canal100);
    if (notzero == 0)
    {
      sprintf(name_canal100, "canal100");
    }
    break;
  case 101:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal101);
    notzero = strlen(name_canal101);
    if (notzero == 0)
    {
      sprintf(name_canal101, "canal101");
    }
    break;
  case 102:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal102);
    notzero = strlen(name_canal102);
    if (notzero == 0)
    {
      sprintf(name_canal102, "canal102");
    }
    break;
  case 103:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal103);
    notzero = strlen(name_canal103);
    if (notzero == 0)
    {
      sprintf(name_canal103, "canal103");
    }
    break;
  case 104:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal104);
    notzero = strlen(name_canal104);
    if (notzero == 0)
    {
      sprintf(name_canal104, "canal104");
    }
    break;
  case 105:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal105);
    notzero = strlen(name_canal105);
    if (notzero == 0)
    {
      sprintf(name_canal105, "canal105");
    }
    break;
  case 106:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal106);
    notzero = strlen(name_canal106);
    if (notzero == 0)
    {
      sprintf(name_canal106, "canal106");
    }
    break;
  case 107:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal107);
    notzero = strlen(name_canal107);
    if (notzero == 0)
    {
      sprintf(name_canal107, "canal107");
    }
    break;
  case 108:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal108);
    notzero = strlen(name_canal108);
    if (notzero == 0)
    {
      sprintf(name_canal108, "canal108");
    }
    break;
  case 109:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal109);
    notzero = strlen(name_canal109);
    if (notzero == 0)
    {
      sprintf(name_canal109, "canal109");
    }
    break;
  case 110:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal110);
    notzero = strlen(name_canal110);
    if (notzero == 0)
    {
      sprintf(name_canal110, "canal110");
    }
    break;
  case 111:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal111);
    notzero = strlen(name_canal111);
    if (notzero == 0)
    {
      sprintf(name_canal111, "canal111");
    }
    break;
  case 112:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal112);
    notzero = strlen(name_canal112);
    if (notzero == 0)
    {
      sprintf(name_canal112, "canal112");
    }
    break;
  case 113:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal113);
    notzero = strlen(name_canal113);
    if (notzero == 0)
    {
      sprintf(name_canal113, "canal113");
    }
    break;
  case 114:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal114);
    notzero = strlen(name_canal114);
    if (notzero == 0)
    {
      sprintf(name_canal114, "canal114");
    }
    break;
  case 115:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal115);
    notzero = strlen(name_canal115);
    if (notzero == 0)
    {
      sprintf(name_canal115, "canal115");
    }
    break;
  case 116:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal116);
    notzero = strlen(name_canal116);
    if (notzero == 0)
    {
      sprintf(name_canal116, "canal116");
    }
    break;
  case 117:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal117);
    notzero = strlen(name_canal117);
    if (notzero == 0)
    {
      sprintf(name_canal117, "canal117");
    }
    break;
  case 118:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal118);
    notzero = strlen(name_canal118);
    if (notzero == 0)
    {
      sprintf(name_canal118, "canal118");
    }
    break;
  case 119:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal119);
    notzero = strlen(name_canal119);
    if (notzero == 0)
    {
      sprintf(name_canal119, "canal119");
    }
    break;
  case 120:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal120);
    notzero = strlen(name_canal120);
    if (notzero == 0)
    {
      sprintf(name_canal120, "canal120");
    }
    break;
  case 121:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal121);
    notzero = strlen(name_canal121);
    if (notzero == 0)
    {
      sprintf(name_canal121, "canal121");
    }
    break;
  case 122:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal122);
    notzero = strlen(name_canal122);
    if (notzero == 0)
    {
      sprintf(name_canal122, "canal122");
    }
    break;
  case 123:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal123);
    notzero = strlen(name_canal123);
    if (notzero == 0)
    {
      sprintf(name_canal123, "canal123");
    }
    break;
  case 124:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal124);
    notzero = strlen(name_canal124);
    if (notzero == 0)
    {
      sprintf(name_canal124, "canal124");
    }
    break;
  case 125:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal125);
    notzero = strlen(name_canal125);
    if (notzero == 0)
    {
      sprintf(name_canal125, "canal125");
    }
    break;
  case 126:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal126);
    notzero = strlen(name_canal126);
    if (notzero == 0)
    {
      sprintf(name_canal126, "canal126");
    }
    break;
  case 127:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal127);
    notzero = strlen(name_canal127);
    if (notzero == 0)
    {
      sprintf(name_canal127, "canal127");
    }
    break;
  case 128:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal128);
    notzero = strlen(name_canal128);
    if (notzero == 0)
    {
      sprintf(name_canal128, "canal128");
    }
    break;
  case 129:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal129);
    notzero = strlen(name_canal129);
    if (notzero == 0)
    {
      sprintf(name_canal129, "canal129");
    }
    break;
  case 130:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal130);
    notzero = strlen(name_canal130);
    if (notzero == 0)
    {
      sprintf(name_canal130, "canal130");
    }
    break;
  case 131:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal131);
    notzero = strlen(name_canal131);
    if (notzero == 0)
    {
      sprintf(name_canal131, "canal131");
    }
    break;
  case 132:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal132);
    notzero = strlen(name_canal132);
    if (notzero == 0)
    {
      sprintf(name_canal132, "canal132");
    }
    break;
  case 133:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal133);
    notzero = strlen(name_canal133);
    if (notzero == 0)
    {
      sprintf(name_canal133, "canal133");
    }
    break;
  case 134:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal134);
    notzero = strlen(name_canal134);
    if (notzero == 0)
    {
      sprintf(name_canal134, "canal134");
    }
    break;
  case 135:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal135);
    notzero = strlen(name_canal135);
    if (notzero == 0)
    {
      sprintf(name_canal135, "canal135");
    }
    break;
  case 136:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal136);
    notzero = strlen(name_canal136);
    if (notzero == 0)
    {
      sprintf(name_canal136, "canal136");
    }
    break;
  case 137:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal137);
    notzero = strlen(name_canal137);
    if (notzero == 0)
    {
      sprintf(name_canal137, "canal137");
    }
    break;
  case 138:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal138);
    notzero = strlen(name_canal138);
    if (notzero == 0)
    {
      sprintf(name_canal138, "canal138");
    }
    break;
  case 139:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal139);
    notzero = strlen(name_canal139);
    if (notzero == 0)
    {
      sprintf(name_canal139, "canal139");
    }
    break;
  case 140:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal140);
    notzero = strlen(name_canal140);
    if (notzero == 0)
    {
      sprintf(name_canal140, "canal140");
    }
    break;
  case 141:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal141);
    notzero = strlen(name_canal141);
    if (notzero == 0)
    {
      sprintf(name_canal141, "canal141");
    }
    break;
  case 142:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal142);
    notzero = strlen(name_canal142);
    if (notzero == 0)
    {
      sprintf(name_canal142, "canal142");
    }
    break;
  case 143:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal143);
    notzero = strlen(name_canal143);
    if (notzero == 0)
    {
      sprintf(name_canal143, "canal143");
    }
    break;
  case 144:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal144);
    notzero = strlen(name_canal144);
    if (notzero == 0)
    {
      sprintf(name_canal144, "canal144");
    }
    break;
  case 145:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal145);
    notzero = strlen(name_canal145);
    if (notzero == 0)
    {
      sprintf(name_canal145, "canal145");
    }
    break;
  case 146:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal146);
    notzero = strlen(name_canal146);
    if (notzero == 0)
    {
      sprintf(name_canal146, "canal146");
    }
    break;
  case 147:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal147);
    notzero = strlen(name_canal147);
    if (notzero == 0)
    {
      sprintf(name_canal147, "canal147");
    }
    break;
  case 148:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal148);
    notzero = strlen(name_canal148);
    if (notzero == 0)
    {
      sprintf(name_canal148, "canal148");
    }
    break;
  case 149:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal149);
    notzero = strlen(name_canal149);
    if (notzero == 0)
    {
      sprintf(name_canal149, "canal149");
    }
    break;
  case 150:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal150);
    notzero = strlen(name_canal150);
    if (notzero == 0)
    {
      sprintf(name_canal150, "canal150");
    }
    break;
  case 151:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal151);
    notzero = strlen(name_canal151);
    if (notzero == 0)
    {
      sprintf(name_canal151, "canal151");
    }
    break;
  case 152:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal152);
    notzero = strlen(name_canal152);
    if (notzero == 0)
    {
      sprintf(name_canal152, "canal152");
    }
    break;
  case 153:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal153);
    notzero = strlen(name_canal153);
    if (notzero == 0)
    {
      sprintf(name_canal153, "canal153");
    }
    break;
  case 154:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal154);
    notzero = strlen(name_canal154);
    if (notzero == 0)
    {
      sprintf(name_canal154, "canal154");
    }
    break;
  case 155:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal155);
    notzero = strlen(name_canal155);
    if (notzero == 0)
    {
      sprintf(name_canal155, "canal155");
    }
    break;
  case 156:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal156);
    notzero = strlen(name_canal156);
    if (notzero == 0)
    {
      sprintf(name_canal156, "canal156");
    }
    break;
  case 157:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal157);
    notzero = strlen(name_canal157);
    if (notzero == 0)
    {
      sprintf(name_canal157, "canal157");
    }
    break;
  case 158:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal158);
    notzero = strlen(name_canal158);
    if (notzero == 0)
    {
      sprintf(name_canal158, "canal158");
    }
    break;
  case 159:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal159);
    notzero = strlen(name_canal159);
    if (notzero == 0)
    {
      sprintf(name_canal159, "canal159");
    }
    break;
  case 160:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal160);
    notzero = strlen(name_canal160);
    if (notzero == 0)
    {
      sprintf(name_canal160, "canal160");
    }
    break;
  case 161:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal161);
    notzero = strlen(name_canal161);
    if (notzero == 0)
    {
      sprintf(name_canal161, "canal161");
    }
    break;
  case 162:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal162);
    notzero = strlen(name_canal162);
    if (notzero == 0)
    {
      sprintf(name_canal162, "canal162");
    }
    break;
  case 163:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal163);
    notzero = strlen(name_canal163);
    if (notzero == 0)
    {
      sprintf(name_canal163, "canal163");
    }
    break;
  case 164:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal164);
    notzero = strlen(name_canal164);
    if (notzero == 0)
    {
      sprintf(name_canal164, "canal164");
    }
    break;
  case 165:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal165);
    notzero = strlen(name_canal165);
    if (notzero == 0)
    {
      sprintf(name_canal165, "canal165");
    }
    break;
  case 166:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal166);
    notzero = strlen(name_canal166);
    if (notzero == 0)
    {
      sprintf(name_canal166, "canal166");
    }
    break;
  case 167:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal167);
    notzero = strlen(name_canal167);
    if (notzero == 0)
    {
      sprintf(name_canal167, "canal167");
    }
    break;
  case 168:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal168);
    notzero = strlen(name_canal168);
    if (notzero == 0)
    {
      sprintf(name_canal168, "canal168");
    }
    break;
  case 169:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal169);
    notzero = strlen(name_canal169);
    if (notzero == 0)
    {
      sprintf(name_canal169, "canal169");
    }
    break;
  case 170:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal170);
    notzero = strlen(name_canal170);
    if (notzero == 0)
    {
      sprintf(name_canal170, "canal170");
    }
    break;
  case 171:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal171);
    notzero = strlen(name_canal171);
    if (notzero == 0)
    {
      sprintf(name_canal171, "canal171");
    }
    break;
  case 172:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal172);
    notzero = strlen(name_canal172);
    if (notzero == 0)
    {
      sprintf(name_canal172, "canal172");
    }
    break;
  case 173:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal173);
    notzero = strlen(name_canal173);
    if (notzero == 0)
    {
      sprintf(name_canal173, "canal173");
    }
    break;
  case 174:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal174);
    notzero = strlen(name_canal174);
    if (notzero == 0)
    {
      sprintf(name_canal174, "canal174");
    }
    break;
  case 175:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal175);
    notzero = strlen(name_canal175);
    if (notzero == 0)
    {
      sprintf(name_canal175, "canal175");
    }
    break;
  case 176:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal176);
    notzero = strlen(name_canal176);
    if (notzero == 0)
    {
      sprintf(name_canal176, "canal176");
    }
    break;
  case 177:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal177);
    notzero = strlen(name_canal177);
    if (notzero == 0)
    {
      sprintf(name_canal177, "canal177");
    }
    break;
  case 178:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal178);
    notzero = strlen(name_canal178);
    if (notzero == 0)
    {
      sprintf(name_canal178, "canal178");
    }
    break;
  case 179:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal179);
    notzero = strlen(name_canal179);
    if (notzero == 0)
    {
      sprintf(name_canal179, "canal179");
    }
    break;
  case 180:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal180);
    notzero = strlen(name_canal180);
    if (notzero == 0)
    {
      sprintf(name_canal180, "canal180");
    }
    break;
  case 181:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal181);
    notzero = strlen(name_canal181);
    if (notzero == 0)
    {
      sprintf(name_canal181, "canal181");
    }
    break;
  case 182:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal182);
    notzero = strlen(name_canal182);
    if (notzero == 0)
    {
      sprintf(name_canal182, "canal182");
    }
    break;
  case 183:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal183);
    notzero = strlen(name_canal183);
    if (notzero == 0)
    {
      sprintf(name_canal183, "canal183");
    }
    break;
  case 184:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal184);
    notzero = strlen(name_canal184);
    if (notzero == 0)
    {
      sprintf(name_canal184, "canal184");
    }
    break;
  case 185:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal185);
    notzero = strlen(name_canal185);
    if (notzero == 0)
    {
      sprintf(name_canal185, "canal185");
    }
    break;
  case 186:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal186);
    notzero = strlen(name_canal186);
    if (notzero == 0)
    {
      sprintf(name_canal186, "canal186");
    }
    break;
  case 187:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal187);
    notzero = strlen(name_canal187);
    if (notzero == 0)
    {
      sprintf(name_canal187, "canal187");
    }
    break;
  case 188:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal188);
    notzero = strlen(name_canal188);
    if (notzero == 0)
    {
      sprintf(name_canal188, "canal188");
    }
    break;
  case 189:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal189);
    notzero = strlen(name_canal189);
    if (notzero == 0)
    {
      sprintf(name_canal189, "canal189");
    }
    break;
  case 190:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal190);
    notzero = strlen(name_canal190);
    if (notzero == 0)
    {
      sprintf(name_canal190, "canal190");
    }
    break;
  case 191:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal191);
    notzero = strlen(name_canal191);
    if (notzero == 0)
    {
      sprintf(name_canal191, "canal191");
    }
    break;
  case 192:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal192);
    notzero = strlen(name_canal192);
    if (notzero == 0)
    {
      sprintf(name_canal192, "canal192");
    }
    break;
  case 193:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal193);
    notzero = strlen(name_canal193);
    if (notzero == 0)
    {
      sprintf(name_canal193, "canal193");
    }
    break;
  case 194:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal194);
    notzero = strlen(name_canal194);
    if (notzero == 0)
    {
      sprintf(name_canal194, "canal194");
    }
    break;
  case 195:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal195);
    notzero = strlen(name_canal195);
    if (notzero == 0)
    {
      sprintf(name_canal195, "canal195");
    }
    break;
  case 196:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal196);
    notzero = strlen(name_canal196);
    if (notzero == 0)
    {
      sprintf(name_canal196, "canal196");
    }
    break;
  case 197:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal197);
    notzero = strlen(name_canal197);
    if (notzero == 0)
    {
      sprintf(name_canal197, "canal197");
    }
    break;
  case 198:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal198);
    notzero = strlen(name_canal198);
    if (notzero == 0)
    {
      sprintf(name_canal198, "canal198");
    }
    break;
  case 199:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal199);
    notzero = strlen(name_canal199);
    if (notzero == 0)
    {
      sprintf(name_canal199, "canal199");
    }
    break;
  case 200:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal200);
    notzero = strlen(name_canal200);
    if (notzero == 0)
    {
      sprintf(name_canal200, "canal200");
    }
    break;
  case 201:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal201);
    notzero = strlen(name_canal201);
    if (notzero == 0)
    {
      sprintf(name_canal201, "canal201");
    }
    break;
  case 202:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal202);
    notzero = strlen(name_canal202);
    if (notzero == 0)
    {
      sprintf(name_canal202, "canal202");
    }
    break;
  case 203:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal203);
    notzero = strlen(name_canal203);
    if (notzero == 0)
    {
      sprintf(name_canal203, "canal203");
    }
    break;
  case 204:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal204);
    notzero = strlen(name_canal204);
    if (notzero == 0)
    {
      sprintf(name_canal204, "canal204");
    }
    break;
  case 205:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal205);
    notzero = strlen(name_canal205);
    if (notzero == 0)
    {
      sprintf(name_canal205, "canal205");
    }
    break;
  case 206:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal206);
    notzero = strlen(name_canal206);
    if (notzero == 0)
    {
      sprintf(name_canal206, "canal206");
    }
    break;
  case 207:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal207);
    notzero = strlen(name_canal207);
    if (notzero == 0)
    {
      sprintf(name_canal207, "canal207");
    }
    break;
  case 208:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal208);
    notzero = strlen(name_canal208);
    if (notzero == 0)
    {
      sprintf(name_canal208, "canal208");
    }
    break;
  case 209:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal209);
    notzero = strlen(name_canal209);
    if (notzero == 0)
    {
      sprintf(name_canal209, "canal209");
    }
    break;
  case 210:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal210);
    notzero = strlen(name_canal210);
    if (notzero == 0)
    {
      sprintf(name_canal210, "canal210");
    }
    break;
  case 211:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal211);
    notzero = strlen(name_canal211);
    if (notzero == 0)
    {
      sprintf(name_canal211, "canal211");
    }
    break;
  case 212:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal212);
    notzero = strlen(name_canal212);
    if (notzero == 0)
    {
      sprintf(name_canal212, "canal212");
    }
    break;
  case 213:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal213);
    notzero = strlen(name_canal213);
    if (notzero == 0)
    {
      sprintf(name_canal213, "canal213");
    }
    break;
  case 214:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal214);
    notzero = strlen(name_canal214);
    if (notzero == 0)
    {
      sprintf(name_canal214, "canal214");
    }
    break;
  case 215:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal215);
    notzero = strlen(name_canal215);
    if (notzero == 0)
    {
      sprintf(name_canal215, "canal215");
    }
    break;
  case 216:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal216);
    notzero = strlen(name_canal216);
    if (notzero == 0)
    {
      sprintf(name_canal216, "canal216");
    }
    break;
  case 217:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal217);
    notzero = strlen(name_canal217);
    if (notzero == 0)
    {
      sprintf(name_canal217, "canal217");
    }
    break;
  case 218:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal218);
    notzero = strlen(name_canal218);
    if (notzero == 0)
    {
      sprintf(name_canal218, "canal218");
    }
    break;
  case 219:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal219);
    notzero = strlen(name_canal219);
    if (notzero == 0)
    {
      sprintf(name_canal219, "canal219");
    }
    break;
  case 220:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal220);
    notzero = strlen(name_canal220);
    if (notzero == 0)
    {
      sprintf(name_canal220, "canal220");
    }
    break;
  case 221:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal221);
    notzero = strlen(name_canal221);
    if (notzero == 0)
    {
      sprintf(name_canal221, "canal221");
    }
    break;
  case 222:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal222);
    notzero = strlen(name_canal222);
    if (notzero == 0)
    {
      sprintf(name_canal222, "canal222");
    }
    break;
  case 223:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal223);
    notzero = strlen(name_canal223);
    if (notzero == 0)
    {
      sprintf(name_canal223, "canal223");
    }
    break;
  case 224:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal224);
    notzero = strlen(name_canal224);
    if (notzero == 0)
    {
      sprintf(name_canal224, "canal224");
    }
    break;
  case 225:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal225);
    notzero = strlen(name_canal225);
    if (notzero == 0)
    {
      sprintf(name_canal225, "canal225");
    }
    break;
  case 226:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal226);
    notzero = strlen(name_canal226);
    if (notzero == 0)
    {
      sprintf(name_canal226, "canal226");
    }
    break;
  case 227:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal227);
    notzero = strlen(name_canal227);
    if (notzero == 0)
    {
      sprintf(name_canal227, "canal227");
    }
    break;
  case 228:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal228);
    notzero = strlen(name_canal228);
    if (notzero == 0)
    {
      sprintf(name_canal228, "canal228");
    }
    break;
  case 229:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal229);
    notzero = strlen(name_canal229);
    if (notzero == 0)
    {
      sprintf(name_canal229, "canal229");
    }
    break;
  case 230:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal230);
    notzero = strlen(name_canal230);
    if (notzero == 0)
    {
      sprintf(name_canal230, "canal230");
    }
    break;
  case 231:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal231);
    notzero = strlen(name_canal231);
    if (notzero == 0)
    {
      sprintf(name_canal231, "canal231");
    }
    break;
  case 232:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal232);
    notzero = strlen(name_canal232);
    if (notzero == 0)
    {
      sprintf(name_canal232, "canal232");
    }
    break;
  case 233:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal233);
    notzero = strlen(name_canal233);
    if (notzero == 0)
    {
      sprintf(name_canal233, "canal233");
    }
    break;
  case 234:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal234);
    notzero = strlen(name_canal234);
    if (notzero == 0)
    {
      sprintf(name_canal234, "canal234");
    }
    break;
  case 235:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal235);
    notzero = strlen(name_canal235);
    if (notzero == 0)
    {
      sprintf(name_canal235, "canal235");
    }
    break;
  case 236:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal236);
    notzero = strlen(name_canal236);
    if (notzero == 0)
    {
      sprintf(name_canal236, "canal236");
    }
    break;
  case 237:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal237);
    notzero = strlen(name_canal237);
    if (notzero == 0)
    {
      sprintf(name_canal237, "canal237");
    }
    break;
  case 238:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal238);
    notzero = strlen(name_canal238);
    if (notzero == 0)
    {
      sprintf(name_canal238, "canal238");
    }
    break;
  case 239:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal239);
    notzero = strlen(name_canal239);
    if (notzero == 0)
    {
      sprintf(name_canal239, "canal239");
    }
    break;
  case 240:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal240);
    notzero = strlen(name_canal240);
    if (notzero == 0)
    {
      sprintf(name_canal240, "canal240");
    }
    break;
  case 241:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal241);
    notzero = strlen(name_canal241);
    if (notzero == 0)
    {
      sprintf(name_canal241, "canal241");
    }
    break;
  case 242:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal242);
    notzero = strlen(name_canal242);
    if (notzero == 0)
    {
      sprintf(name_canal242, "canal242");
    }
    break;
  case 243:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal243);
    notzero = strlen(name_canal243);
    if (notzero == 0)
    {
      sprintf(name_canal243, "canal243");
    }
    break;
  case 244:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal244);
    notzero = strlen(name_canal244);
    if (notzero == 0)
    {
      sprintf(name_canal244, "canal244");
    }
    break;
  case 245:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal245);
    notzero = strlen(name_canal245);
    if (notzero == 0)
    {
      sprintf(name_canal245, "canal245");
    }
    break;
  case 246:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal246);
    notzero = strlen(name_canal246);
    if (notzero == 0)
    {
      sprintf(name_canal246, "canal246");
    }
    break;
  case 247:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal247);
    notzero = strlen(name_canal247);
    if (notzero == 0)
    {
      sprintf(name_canal247, "canal247");
    }
    break;
  case 248:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal248);
    notzero = strlen(name_canal248);
    if (notzero == 0)
    {
      sprintf(name_canal248, "canal248");
    }
    break;
  case 249:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal249);
    notzero = strlen(name_canal249);
    if (notzero == 0)
    {
      sprintf(name_canal249, "canal249");
    }
    break;
  case 250:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal250);
    notzero = strlen(name_canal250);
    if (notzero == 0)
    {
      sprintf(name_canal250, "canal250");
    }
    break;
  case 251:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal251);
    notzero = strlen(name_canal251);
    if (notzero == 0)
    {
      sprintf(name_canal251, "canal251");
    }
    break;
  case 252:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal252);
    notzero = strlen(name_canal252);
    if (notzero == 0)
    {
      sprintf(name_canal252, "canal252");
    }
    break;
  case 253:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal253);
    notzero = strlen(name_canal253);
    if (notzero == 0)
    {
      sprintf(name_canal253, "canal253");
    }
    break;
  case 254:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal254);
    notzero = strlen(name_canal254);
    if (notzero == 0)
    {
      sprintf(name_canal254, "canal254");
    }
    break;
  case 255:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal255);
    notzero = strlen(name_canal255);
    if (notzero == 0)
    {
      sprintf(name_canal255, "canal255");
    }
    break;
  case 256:
    myEEP.get(start_name_adrr2 + (addr_name * canal_name) + 1, name_canal256);
    notzero = strlen(name_canal256);
    if (notzero == 0)
    {
      sprintf(name_canal256, "canal256");
    }
    break;
  default:
    dataT["nom_canal1"] = "bug";
    break;
  }
}
void go_give_nom_groupe()
{
  dataT["nom_groupe1"] = name_groupe1;
  dataT["nom_groupe2"] = name_groupe2;
  dataT["nom_groupe3"] = name_groupe3;
  dataT["nom_groupe4"] = name_groupe4;
  dataT["nom_groupe5"] = name_groupe5;
  dataT["nom_groupe6"] = name_groupe6;
  dataT["nom_groupe7"] = name_groupe7;
  dataT["nom_groupe8"] = name_groupe8;
  dataT["nom_groupe9"] = name_groupe9;
  dataT["nom_groupe10"] = name_groupe10;
  dataT["nom_groupe11"] = name_groupe11;
  dataT["nom_groupe12"] = name_groupe12;
  dataT["nom_groupe13"] = name_groupe13;
  dataT["nom_groupe14"] = name_groupe14;
  dataT["nom_groupe15"] = name_groupe15;
  dataT["nom_groupe16"] = name_groupe16;
  dataT["nom_groupe17"] = name_groupe17;
  dataT["nom_groupe18"] = name_groupe18;
  dataT["nom_groupe19"] = name_groupe19;
  dataT["nom_groupe20"] = name_groupe20;
  dataT["nom_groupe21"] = name_groupe21;
  dataT["nom_groupe22"] = name_groupe22;
  dataT["nom_groupe23"] = name_groupe23;
  dataT["nom_groupe24"] = name_groupe24;
  dataT["nom_groupe25"] = name_groupe25;
  dataT["nom_groupe26"] = name_groupe26;
  dataT["nom_groupe27"] = name_groupe27;
  dataT["nom_groupe28"] = name_groupe28;
  dataT["nom_groupe29"] = name_groupe29;
  dataT["nom_groupe30"] = name_groupe30;
  dataT["nom_groupe31"] = name_groupe31;
  dataT["nom_groupe32"] = name_groupe32;
}
void go_give_nom_canal()
{
  dataT["1"] = name_canal1;
  dataT["2"] = name_canal2;
  dataT["3"] = name_canal3;
  dataT["4"] = name_canal4;
  dataT["5"] = name_canal5;
  dataT["6"] = name_canal6;
  dataT["7"] = name_canal7;
  dataT["8"] = name_canal8;
  dataT["9"] = name_canal9;
  dataT["10"] = name_canal10;
  dataT["11"] = name_canal11;
  dataT["12"] = name_canal12;
  dataT["13"] = name_canal13;
  dataT["14"] = name_canal14;
  dataT["15"] = name_canal15;
  dataT["16"] = name_canal16;
  dataT["17"] = name_canal17;
  dataT["18"] = name_canal18;
  dataT["19"] = name_canal19;
  dataT["20"] = name_canal20;
  dataT["21"] = name_canal21;
  dataT["22"] = name_canal22;
  dataT["23"] = name_canal23;
  dataT["24"] = name_canal24;
  dataT["25"] = name_canal25;
  dataT["26"] = name_canal26;
  dataT["27"] = name_canal27;
  dataT["28"] = name_canal28;
  dataT["29"] = name_canal29;
  dataT["30"] = name_canal30;
  dataT["31"] = name_canal31;
  dataT["32"] = name_canal32;
  dataT["33"] = name_canal33;
  dataT["34"] = name_canal34;
  dataT["35"] = name_canal35;
  dataT["36"] = name_canal36;
  dataT["37"] = name_canal37;
  dataT["38"] = name_canal38;
  dataT["39"] = name_canal39;
  dataT["40"] = name_canal40;
  dataT["41"] = name_canal41;
  dataT["42"] = name_canal42;
  dataT["43"] = name_canal43;
  dataT["44"] = name_canal44;
  dataT["45"] = name_canal45;
  dataT["46"] = name_canal46;
  dataT["47"] = name_canal47;
  dataT["48"] = name_canal48;
  dataT["49"] = name_canal49;
  dataT["50"] = name_canal50;
  dataT["51"] = name_canal51;
  dataT["52"] = name_canal52;
  dataT["53"] = name_canal53;
  dataT["54"] = name_canal54;
  dataT["55"] = name_canal55;
  dataT["56"] = name_canal56;
  dataT["57"] = name_canal57;
  dataT["58"] = name_canal58;
  dataT["59"] = name_canal59;
  dataT["60"] = name_canal60;
  dataT["61"] = name_canal61;
  dataT["62"] = name_canal62;
  dataT["63"] = name_canal63;
  dataT["64"] = name_canal64;
  dataT["65"] = name_canal65;
  dataT["66"] = name_canal66;
  dataT["67"] = name_canal67;
  dataT["68"] = name_canal68;
  dataT["69"] = name_canal69;
  dataT["70"] = name_canal70;
  dataT["71"] = name_canal71;
  dataT["72"] = name_canal72;
  dataT["73"] = name_canal73;
  dataT["74"] = name_canal74;
  dataT["75"] = name_canal75;
  dataT["76"] = name_canal76;
  dataT["77"] = name_canal77;
  dataT["78"] = name_canal78;
  dataT["79"] = name_canal79;
  dataT["80"] = name_canal80;
  dataT["81"] = name_canal81;
  dataT["82"] = name_canal82;
  dataT["83"] = name_canal83;
  dataT["84"] = name_canal84;
  dataT["85"] = name_canal85;
  dataT["86"] = name_canal86;
  dataT["87"] = name_canal87;
  dataT["88"] = name_canal88;
  dataT["89"] = name_canal89;
  dataT["90"] = name_canal90;
  dataT["91"] = name_canal91;
  dataT["92"] = name_canal92;
  dataT["93"] = name_canal93;
  dataT["94"] = name_canal94;
  dataT["95"] = name_canal95;
  dataT["96"] = name_canal96;
  dataT["97"] = name_canal97;
  dataT["98"] = name_canal98;
  dataT["99"] = name_canal99;
  dataT["100"] = name_canal100;
  dataT["101"] = name_canal101;
  dataT["102"] = name_canal102;
  dataT["103"] = name_canal103;
  dataT["104"] = name_canal104;
  dataT["105"] = name_canal105;
  dataT["106"] = name_canal106;
  dataT["107"] = name_canal107;
  dataT["108"] = name_canal108;
  dataT["109"] = name_canal109;
  dataT["110"] = name_canal110;
  dataT["111"] = name_canal111;
  dataT["112"] = name_canal112;
  dataT["113"] = name_canal113;
  dataT["114"] = name_canal114;
  dataT["115"] = name_canal115;
  dataT["116"] = name_canal116;
  dataT["117"] = name_canal117;
  dataT["118"] = name_canal118;
  dataT["119"] = name_canal119;
  dataT["120"] = name_canal120;
  dataT["121"] = name_canal121;
  dataT["122"] = name_canal122;
  dataT["123"] = name_canal123;
  dataT["124"] = name_canal124;
  dataT["125"] = name_canal125;
  dataT["126"] = name_canal126;
  dataT["127"] = name_canal127;
  dataT["128"] = name_canal128;
  dataT["129"] = name_canal129;
  dataT["130"] = name_canal130;
  dataT["131"] = name_canal131;
  dataT["132"] = name_canal132;
  dataT["133"] = name_canal133;
  dataT["134"] = name_canal134;
  dataT["135"] = name_canal135;
  dataT["136"] = name_canal136;
  dataT["137"] = name_canal137;
  dataT["138"] = name_canal138;
  dataT["139"] = name_canal139;
  dataT["140"] = name_canal140;
  dataT["141"] = name_canal141;
  dataT["142"] = name_canal142;
  dataT["143"] = name_canal143;
  dataT["144"] = name_canal144;
  dataT["145"] = name_canal145;
  dataT["146"] = name_canal146;
  dataT["147"] = name_canal147;
  dataT["148"] = name_canal148;
  dataT["149"] = name_canal149;
  dataT["150"] = name_canal150;
  dataT["151"] = name_canal151;
  dataT["152"] = name_canal152;
  dataT["153"] = name_canal153;
  dataT["154"] = name_canal154;
  dataT["155"] = name_canal155;
  dataT["156"] = name_canal156;
  dataT["157"] = name_canal157;
  dataT["158"] = name_canal158;
  dataT["159"] = name_canal159;
  dataT["160"] = name_canal160;
  dataT["161"] = name_canal161;
  dataT["162"] = name_canal162;
  dataT["163"] = name_canal163;
  dataT["164"] = name_canal164;
  dataT["165"] = name_canal165;
  dataT["166"] = name_canal166;
  dataT["167"] = name_canal167;
  dataT["168"] = name_canal168;
  dataT["169"] = name_canal169;
  dataT["170"] = name_canal170;
  dataT["171"] = name_canal171;
  dataT["172"] = name_canal172;
  dataT["173"] = name_canal173;
  dataT["174"] = name_canal174;
  dataT["175"] = name_canal175;
  dataT["176"] = name_canal176;
  dataT["177"] = name_canal177;
  dataT["178"] = name_canal178;
  dataT["179"] = name_canal179;
  dataT["180"] = name_canal180;
  dataT["181"] = name_canal181;
  dataT["182"] = name_canal182;
  dataT["183"] = name_canal183;
  dataT["184"] = name_canal184;
  dataT["185"] = name_canal185;
  dataT["186"] = name_canal186;
  dataT["187"] = name_canal187;
  dataT["188"] = name_canal188;
  dataT["189"] = name_canal189;
  dataT["190"] = name_canal190;
  dataT["191"] = name_canal191;
  dataT["192"] = name_canal192;
  dataT["193"] = name_canal193;
  dataT["194"] = name_canal194;
  dataT["195"] = name_canal195;
  dataT["196"] = name_canal196;
  dataT["197"] = name_canal197;
  dataT["198"] = name_canal198;
  dataT["199"] = name_canal199;
  dataT["200"] = name_canal200;
  dataT["201"] = name_canal201;
  dataT["202"] = name_canal202;
  dataT["203"] = name_canal203;
  dataT["204"] = name_canal204;
  dataT["205"] = name_canal205;
  dataT["206"] = name_canal206;
  dataT["207"] = name_canal207;
  dataT["208"] = name_canal208;
  dataT["209"] = name_canal209;
  dataT["210"] = name_canal210;
  dataT["211"] = name_canal211;
  dataT["212"] = name_canal212;
  dataT["213"] = name_canal213;
  dataT["214"] = name_canal214;
  dataT["215"] = name_canal215;
  dataT["216"] = name_canal216;
  dataT["217"] = name_canal217;
  dataT["218"] = name_canal218;
  dataT["219"] = name_canal219;
  dataT["220"] = name_canal220;
  dataT["221"] = name_canal221;
  dataT["222"] = name_canal222;
  dataT["223"] = name_canal223;
  dataT["224"] = name_canal224;
  dataT["225"] = name_canal225;
  dataT["226"] = name_canal226;
  dataT["227"] = name_canal227;
  dataT["228"] = name_canal228;
  dataT["229"] = name_canal229;
  dataT["230"] = name_canal230;
  dataT["231"] = name_canal231;
  dataT["232"] = name_canal232;
  dataT["233"] = name_canal233;
  dataT["234"] = name_canal234;
  dataT["235"] = name_canal235;
  dataT["236"] = name_canal236;
  dataT["237"] = name_canal237;
  dataT["238"] = name_canal238;
  dataT["239"] = name_canal239;
  dataT["240"] = name_canal240;
  dataT["241"] = name_canal241;
  dataT["242"] = name_canal242;
  dataT["243"] = name_canal243;
  dataT["244"] = name_canal244;
  dataT["245"] = name_canal245;
  dataT["246"] = name_canal246;
  dataT["247"] = name_canal247;
  dataT["248"] = name_canal248;
  dataT["249"] = name_canal249;
  dataT["250"] = name_canal250;
  dataT["251"] = name_canal251;
  dataT["252"] = name_canal252;
  dataT["253"] = name_canal253;
  dataT["254"] = name_canal254;
  dataT["255"] = name_canal255;
  dataT["256"] = name_canal256;
  /* switch (nb_name)
  {
  case 1:
    dataT["nom_canal1"] = name_canal1;
    break;
  case 2:
    dataT["nom_canal2"] = name_canal2;
    break;
  case 3:
    dataT["nom_canal3"] = name_canal3;
    break;
  case 4:
    dataT["nom_canal4"] = name_canal4;
    break;
  case 5:
    dataT["nom_canal5"] = name_canal5;
    break;
  case 6:
    dataT["nom_canal6"] = name_canal6;
    break;
  case 7:
    dataT["nom_canal7"] = name_canal7;
    break;
  case 8:
    dataT["nom_canal8"] = name_canal8;
    break;
  case 9:
    dataT["nom_canal9"] = name_canal9;
    break;
  case 10:
    dataT["nom_canal10"] = name_canal10;
    break;
  case 11:
    dataT["nom_canal11"] = name_canal11;
    break;
  case 12:
    dataT["nom_canal12"] = name_canal12;
    break;
  case 13:
    dataT["nom_canal13"] = name_canal13;
    break;
  case 14:
    dataT["nom_canal14"] = name_canal14;
    break;
  case 15:
    dataT["nom_canal15"] = name_canal15;
    break;
  case 16:
    dataT["nom_canal16"] = name_canal16;
    break;
  case 17:
    dataT["nom_canal17"] = name_canal17;
    break;
  case 18:
    dataT["nom_canal18"] = name_canal18;
    break;
  case 19:
    dataT["nom_canal19"] = name_canal19;
    break;
  case 20:
    dataT["nom_canal20"] = name_canal20;
    break;
  case 21:
    dataT["nom_canal21"] = name_canal21;
    break;
  case 22:
    dataT["nom_canal22"] = name_canal22;
    break;
  case 23:
    dataT["nom_canal23"] = name_canal23;
    break;
  case 24:
    dataT["nom_canal24"] = name_canal24;
    break;
  case 25:
    dataT["nom_canal25"] = name_canal25;
    break;
  case 26:
    dataT["nom_canal26"] = name_canal26;
    break;
  case 27:
    dataT["nom_canal27"] = name_canal27;
    break;
  case 28:
    dataT["nom_canal28"] = name_canal28;
    break;
  case 29:
    dataT["nom_canal29"] = name_canal29;
    break;
  case 30:
    dataT["nom_canal30"] = name_canal30;
    break;
  case 31:
    dataT["nom_canal31"] = name_canal31;
    break;
  case 32:
    dataT["nom_canal32"] = name_canal32;
    break;
  case 33:
    dataT["nom_canal33"] = name_canal33;
    break;
  case 34:
    dataT["nom_canal34"] = name_canal34;
    break;
  case 35:
    dataT["nom_canal35"] = name_canal35;
    break;
  case 36:
    dataT["nom_canal36"] = name_canal36;
    break;
  case 37:
    dataT["nom_canal37"] = name_canal37;
    break;
  case 38:
    dataT["nom_canal38"] = name_canal38;
    break;
  case 39:
    dataT["nom_canal39"] = name_canal39;
    break;
  case 40:
    dataT["nom_canal40"] = name_canal40;
    break;
  case 41:
    dataT["nom_canal41"] = name_canal41;
    break;
  case 42:
    dataT["nom_canal42"] = name_canal42;
    break;
  case 43:
    dataT["nom_canal43"] = name_canal43;
    break;
  case 44:
    dataT["nom_canal44"] = name_canal44;
    break;
  case 45:
    dataT["nom_canal45"] = name_canal45;
    break;
  case 46:
    dataT["nom_canal46"] = name_canal46;
    break;
  case 47:
    dataT["nom_canal47"] = name_canal47;
    break;
  case 48:
    dataT["nom_canal48"] = name_canal48;
    break;
  case 49:
    dataT["nom_canal49"] = name_canal49;
    break;
  case 50:
    dataT["nom_canal50"] = name_canal50;
    break;
  case 51:
    dataT["nom_canal51"] = name_canal51;
    break;
  case 52:
    dataT["nom_canal52"] = name_canal52;
    break;
  case 53:
    dataT["nom_canal53"] = name_canal53;
    break;
  case 54:
    dataT["nom_canal54"] = name_canal54;
    break;
  case 55:
    dataT["nom_canal55"] = name_canal55;
    break;
  case 56:
    dataT["nom_canal56"] = name_canal56;
    break;
  case 57:
    dataT["nom_canal57"] = name_canal57;
    break;
  case 58:
    dataT["nom_canal58"] = name_canal58;
    break;
  case 59:
    dataT["nom_canal59"] = name_canal59;
    break;
  case 60:
    dataT["nom_canal60"] = name_canal60;
    break;
  case 61:
    dataT["nom_canal61"] = name_canal61;
    break;
  case 62:
    dataT["nom_canal62"] = name_canal62;
    break;
  case 63:
    dataT["nom_canal63"] = name_canal63;
    break;
  case 64:
    dataT["nom_canal64"] = name_canal64;
    break;
  case 65:
    dataT["nom_canal65"] = name_canal65;
    break;
  case 66:
    dataT["nom_canal66"] = name_canal66;
    break;
  case 67:
    dataT["nom_canal67"] = name_canal67;
    break;
  case 68:
    dataT["nom_canal68"] = name_canal68;
    break;
  case 69:
    dataT["nom_canal69"] = name_canal69;
    break;
  case 70:
    dataT["nom_canal70"] = name_canal70;
    break;
  case 71:
    dataT["nom_canal71"] = name_canal71;
    break;
  case 72:
    dataT["nom_canal72"] = name_canal72;
    break;
  case 73:
    dataT["nom_canal73"] = name_canal73;
    break;
  case 74:
    dataT["nom_canal74"] = name_canal74;
    break;
  case 75:
    dataT["nom_canal75"] = name_canal75;
    break;
  case 76:
    dataT["nom_canal76"] = name_canal76;
    break;
  case 77:
    dataT["nom_canal77"] = name_canal77;
    break;
  case 78:
    dataT["nom_canal78"] = name_canal78;
    break;
  case 79:
    dataT["nom_canal79"] = name_canal79;
    break;
  case 80:
    dataT["nom_canal80"] = name_canal80;
    break;
  case 81:
    dataT["nom_canal81"] = name_canal81;
    break;
  case 82:
    dataT["nom_canal82"] = name_canal82;
    break;
  case 83:
    dataT["nom_canal83"] = name_canal83;
    break;
  case 84:
    dataT["nom_canal84"] = name_canal84;
    break;
  case 85:
    dataT["nom_canal85"] = name_canal85;
    break;
  case 86:
    dataT["nom_canal86"] = name_canal86;
    break;
  case 87:
    dataT["nom_canal87"] = name_canal87;
    break;
  case 88:
    dataT["nom_canal88"] = name_canal88;
    break;
  case 89:
    dataT["nom_canal89"] = name_canal89;
    break;
  case 90:
    dataT["nom_canal90"] = name_canal90;
    break;
  case 91:
    dataT["nom_canal91"] = name_canal91;
    break;
  case 92:
    dataT["nom_canal92"] = name_canal92;
    break;
  case 93:
    dataT["nom_canal93"] = name_canal93;
    break;
  case 94:
    dataT["nom_canal94"] = name_canal94;
    break;
  case 95:
    dataT["nom_canal95"] = name_canal95;
    break;
  case 96:
    dataT["nom_canal96"] = name_canal96;
    break;
  case 97:
    dataT["nom_canal97"] = name_canal97;
    break;
  case 98:
    dataT["nom_canal98"] = name_canal98;
    break;
  case 99:
    dataT["nom_canal99"] = name_canal99;
    break;
  case 100:
    dataT["nom_canal100"] = name_canal100;
    break;
  case 101:
    dataT["nom_canal101"] = name_canal101;
    break;
  case 102:
    dataT["nom_canal102"] = name_canal102;
    break;
  case 103:
    dataT["nom_canal103"] = name_canal103;
    break;
  case 104:
    dataT["nom_canal104"] = name_canal104;
    break;
  case 105:
    dataT["nom_canal105"] = name_canal105;
    break;
  case 106:
    dataT["nom_canal106"] = name_canal106;
    break;
  case 107:
    dataT["nom_canal107"] = name_canal107;
    break;
  case 108:
    dataT["nom_canal108"] = name_canal108;
    break;
  case 109:
    dataT["nom_canal109"] = name_canal109;
    break;
  case 110:
    dataT["nom_canal110"] = name_canal110;
    break;
  case 111:
    dataT["nom_canal111"] = name_canal111;
    break;
  case 112:
    dataT["nom_canal112"] = name_canal112;
    break;
  case 113:
    dataT["nom_canal113"] = name_canal113;
    break;
  case 114:
    dataT["nom_canal114"] = name_canal114;
    break;
  case 115:
    dataT["nom_canal115"] = name_canal115;
    break;
  case 116:
    dataT["nom_canal116"] = name_canal116;
    break;
  case 117:
    dataT["nom_canal117"] = name_canal117;
    break;
  case 118:
    dataT["nom_canal118"] = name_canal118;
    break;
  case 119:
    dataT["nom_canal119"] = name_canal119;
    break;
  case 120:
    dataT["nom_canal120"] = name_canal120;
    break;
  case 121:
    dataT["nom_canal121"] = name_canal121;
    break;
  case 122:
    dataT["nom_canal122"] = name_canal122;
    break;
  case 123:
    dataT["nom_canal123"] = name_canal123;
    break;
  case 124:
    dataT["nom_canal124"] = name_canal124;
    break;
  case 125:
    dataT["nom_canal125"] = name_canal125;
    break;
  case 126:
    dataT["nom_canal126"] = name_canal126;
    break;
  case 127:
    dataT["nom_canal127"] = name_canal127;
    break;
  case 128:
    dataT["nom_canal128"] = name_canal128;
    break;
  case 129:
    dataT["nom_canal129"] = name_canal129;
    break;
  case 130:
    dataT["nom_canal130"] = name_canal130;
    break;
  case 131:
    dataT["nom_canal131"] = name_canal131;
    break;
  case 132:
    dataT["nom_canal132"] = name_canal132;
    break;
  case 133:
    dataT["nom_canal133"] = name_canal133;
    break;
  case 134:
    dataT["nom_canal134"] = name_canal134;
    break;
  case 135:
    dataT["nom_canal135"] = name_canal135;
    break;
  case 136:
    dataT["nom_canal136"] = name_canal136;
    break;
  case 137:
    dataT["nom_canal137"] = name_canal137;
    break;
  case 138:
    dataT["nom_canal138"] = name_canal138;
    break;
  case 139:
    dataT["nom_canal139"] = name_canal139;
    break;
  case 140:
    dataT["nom_canal140"] = name_canal140;
    break;
  case 141:
    dataT["nom_canal141"] = name_canal141;
    break;
  case 142:
    dataT["nom_canal142"] = name_canal142;
    break;
  case 143:
    dataT["nom_canal143"] = name_canal143;
    break;
  case 144:
    dataT["nom_canal144"] = name_canal144;
    break;
  case 145:
    dataT["nom_canal145"] = name_canal145;
    break;
  case 146:
    dataT["nom_canal146"] = name_canal146;
    break;
  case 147:
    dataT["nom_canal147"] = name_canal147;
    break;
  case 148:
    dataT["nom_canal148"] = name_canal148;
    break;
  case 149:
    dataT["nom_canal149"] = name_canal149;
    break;
  case 150:
    dataT["nom_canal150"] = name_canal150;
    break;
  case 151:
    dataT["nom_canal151"] = name_canal151;
    break;
  case 152:
    dataT["nom_canal152"] = name_canal152;
    break;
  case 153:
    dataT["nom_canal153"] = name_canal153;
    break;
  case 154:
    dataT["nom_canal154"] = name_canal154;
    break;
  case 155:
    dataT["nom_canal155"] = name_canal155;
    break;
  case 156:
    dataT["nom_canal156"] = name_canal156;
    break;
  case 157:
    dataT["nom_canal157"] = name_canal157;
    break;
  case 158:
    dataT["nom_canal158"] = name_canal158;
    break;
  case 159:
    dataT["nom_canal159"] = name_canal159;
    break;
  case 160:
    dataT["nom_canal160"] = name_canal160;
    break;
  case 161:
    dataT["nom_canal161"] = name_canal161;
    break;
  case 162:
    dataT["nom_canal162"] = name_canal162;
    break;
  case 163:
    dataT["nom_canal163"] = name_canal163;
    break;
  case 164:
    dataT["nom_canal164"] = name_canal164;
    break;
  case 165:
    dataT["nom_canal165"] = name_canal165;
    break;
  case 166:
    dataT["nom_canal166"] = name_canal166;
    break;
  case 167:
    dataT["nom_canal167"] = name_canal167;
    break;
  case 168:
    dataT["nom_canal168"] = name_canal168;
    break;
  case 169:
    dataT["nom_canal169"] = name_canal169;
    break;
  case 170:
    dataT["nom_canal170"] = name_canal170;
    break;
  case 171:
    dataT["nom_canal171"] = name_canal171;
    break;
  case 172:
    dataT["nom_canal172"] = name_canal172;
    break;
  case 173:
    dataT["nom_canal173"] = name_canal173;
    break;
  case 174:
    dataT["nom_canal174"] = name_canal174;
    break;
  case 175:
    dataT["nom_canal175"] = name_canal175;
    break;
  case 176:
    dataT["nom_canal176"] = name_canal176;
    break;
  case 177:
    dataT["nom_canal177"] = name_canal177;
    break;
  case 178:
    dataT["nom_canal178"] = name_canal178;
    break;
  case 179:
    dataT["nom_canal179"] = name_canal179;
    break;
  case 180:
    dataT["nom_canal180"] = name_canal180;
    break;
  case 181:
    dataT["nom_canal181"] = name_canal181;
    break;
  case 182:
    dataT["nom_canal182"] = name_canal182;
    break;
  case 183:
    dataT["nom_canal183"] = name_canal183;
    break;
  case 184:
    dataT["nom_canal184"] = name_canal184;
    break;
  case 185:
    dataT["nom_canal185"] = name_canal185;
    break;
  case 186:
    dataT["nom_canal186"] = name_canal186;
    break;
  case 187:
    dataT["nom_canal187"] = name_canal187;
    break;
  case 188:
    dataT["nom_canal188"] = name_canal188;
    break;
  case 189:
    dataT["nom_canal189"] = name_canal189;
    break;
  case 190:
    dataT["nom_canal190"] = name_canal190;
    break;
  case 191:
    dataT["nom_canal191"] = name_canal191;
    break;
  case 192:
    dataT["nom_canal192"] = name_canal192;
    break;
  case 193:
    dataT["nom_canal193"] = name_canal193;
    break;
  case 194:
    dataT["nom_canal194"] = name_canal194;
    break;
  case 195:
    dataT["nom_canal195"] = name_canal195;
    break;
  case 196:
    dataT["nom_canal196"] = name_canal196;
    break;
  case 197:
    dataT["nom_canal197"] = name_canal197;
    break;
  case 198:
    dataT["nom_canal198"] = name_canal198;
    break;
  case 199:
    dataT["nom_canal199"] = name_canal199;
    break;
  case 200:
    dataT["nom_canal200"] = name_canal200;
    break;
  case 201:
    dataT["nom_canal201"] = name_canal201;
    break;
  case 202:
    dataT["nom_canal202"] = name_canal202;
    break;
  case 203:
    dataT["nom_canal203"] = name_canal203;
    break;
  case 204:
    dataT["nom_canal204"] = name_canal204;
    break;
  case 205:
    dataT["nom_canal205"] = name_canal205;
    break;
  case 206:
    dataT["nom_canal206"] = name_canal206;
    break;
  case 207:
    dataT["nom_canal207"] = name_canal207;
    break;
  case 208:
    dataT["nom_canal208"] = name_canal208;
    break;
  case 209:
    dataT["nom_canal209"] = name_canal209;
    break;
  case 210:
    dataT["nom_canal210"] = name_canal210;
    break;
  case 211:
    dataT["nom_canal211"] = name_canal211;
    break;
  case 212:
    dataT["nom_canal212"] = name_canal212;
    break;
  case 213:
    dataT["nom_canal213"] = name_canal213;
    break;
  case 214:
    dataT["nom_canal214"] = name_canal214;
    break;
  case 215:
    dataT["nom_canal215"] = name_canal215;
    break;
  case 216:
    dataT["nom_canal216"] = name_canal216;
    break;
  case 217:
    dataT["nom_canal217"] = name_canal217;
    break;
  case 218:
    dataT["nom_canal218"] = name_canal218;
    break;
  case 219:
    dataT["nom_canal219"] = name_canal219;
    break;
  case 220:
    dataT["nom_canal220"] = name_canal220;
    break;
  case 221:
    dataT["nom_canal221"] = name_canal221;
    break;
  case 222:
    dataT["nom_canal222"] = name_canal222;
    break;
  case 223:
    dataT["nom_canal223"] = name_canal223;
    break;
  case 224:
    dataT["nom_canal224"] = name_canal224;
    break;
  case 225:
    dataT["nom_canal225"] = name_canal225;
    break;
  case 226:
    dataT["nom_canal226"] = name_canal226;
    break;
  case 227:
    dataT["nom_canal227"] = name_canal227;
    break;
  case 228:
    dataT["nom_canal228"] = name_canal228;
    break;
  case 229:
    dataT["nom_canal229"] = name_canal229;
    break;
  case 230:
    dataT["nom_canal230"] = name_canal230;
    break;
  case 231:
    dataT["nom_canal231"] = name_canal231;
    break;
  case 232:
    dataT["nom_canal232"] = name_canal232;
    break;
  case 233:
    dataT["nom_canal233"] = name_canal233;
    break;
  case 234:
    dataT["nom_canal234"] = name_canal234;
    break;
  case 235:
    dataT["nom_canal235"] = name_canal235;
    break;
  case 236:
    dataT["nom_canal236"] = name_canal236;
    break;
  case 237:
    dataT["nom_canal237"] = name_canal237;
    break;
  case 238:
    dataT["nom_canal238"] = name_canal238;
    break;
  case 239:
    dataT["nom_canal239"] = name_canal239;
    break;
  case 240:
    dataT["nom_canal240"] = name_canal240;
    break;
  case 241:
    dataT["nom_canal241"] = name_canal241;
    break;
  case 242:
    dataT["nom_canal242"] = name_canal242;
    break;
  case 243:
    dataT["nom_canal243"] = name_canal243;
    break;
  case 244:
    dataT["nom_canal244"] = name_canal244;
    break;
  case 245:
    dataT["nom_canal245"] = name_canal245;
    break;
  case 246:
    dataT["nom_canal246"] = name_canal246;
    break;
  case 247:
    dataT["nom_canal247"] = name_canal247;
    break;
  case 248:
    dataT["nom_canal248"] = name_canal248;
    break;
  case 249:
    dataT["nom_canal249"] = name_canal249;
    break;
  case 250:
    dataT["nom_canal250"] = name_canal250;
    break;
  case 251:
    dataT["nom_canal251"] = name_canal251;
    break;
  case 252:
    dataT["nom_canal252"] = name_canal252;
    break;
  case 253:
    dataT["nom_canal253"] = name_canal253;
    break;
  case 254:
    dataT["nom_canal254"] = name_canal254;
    break;
  case 255:
    dataT["nom_canal255"] = name_canal255;
    break;
  case 256:
    dataT["nom_canal256"] = name_canal256;
    break;

  default:
  dataT["nom_canal1"] = "bug";
    break;
  } */
}

void invertgroupe()
{

  switch (groupe)
  {
  case 1:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe1, arraygroupe, 32);
    break;
  case 2:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe2, arraygroupe, 32);
    break;
  case 3:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe3, arraygroupe, 32);
    break;
  case 4:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe4, arraygroupe, 32);
    break;
  case 5:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe5, arraygroupe, 32);
    break;
  case 6:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe6, arraygroupe, 32);
    break;
  case 7:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe7, arraygroupe, 32);
    break;
  case 8:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe8, arraygroupe, 32);
    break;
  case 9:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe9, arraygroupe, 32);
    break;
  case 10:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe10, arraygroupe, 32);
    break;
  case 11:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe11, arraygroupe, 32);
    break;
  case 12:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe12, arraygroupe, 32);
    break;
  case 13:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe13, arraygroupe, 32);
    break;
  case 14:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe14, arraygroupe, 32);
    break;
  case 15:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe15, arraygroupe, 32);
    break;
  case 16:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe16, arraygroupe, 32);
    break;
  case 17:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe17, arraygroupe, 32);
    break;
  case 18:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe18, arraygroupe, 32);
    break;
  case 19:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe19, arraygroupe, 32);
    break;
  case 20:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe20, arraygroupe, 32);
    break;
  case 21:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe21, arraygroupe, 32);
    break;
  case 22:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe22, arraygroupe, 32);
    break;
  case 23:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe23, arraygroupe, 32);
    break;
  case 24:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe24, arraygroupe, 32);
    break;
  case 25:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe25, arraygroupe, 32);
    break;
  case 26:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe26, arraygroupe, 32);
    break;
  case 27:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe27, arraygroupe, 32);
    break;
  case 28:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe28, arraygroupe, 32);
    break;
  case 29:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe29, arraygroupe, 32);
    break;
  case 30:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe30, arraygroupe, 32);
    break;
  case 31:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe31, arraygroupe, 32);
    break;
  case 32:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe32, arraygroupe, 32);
    break;
  default:
    memset(arraygroupe, 0, sizeof(arraygroupe));
    copy(groupe1, arraygroupe, 32);
    break;
  }
}