#include "api.h"
#include "arg.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <Arduino.h> //Librairie Arduino
#include <ETH.h>
#include <SPI.h>
// permet de supporter ip101
#define ETH_ADDR 1
#define ETH_POWER_PIN 5
#define ETH_TYPE ETH_PHY_IP101
#include <LXESP32DMX.h>
//#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "mbedtls/md.h"
#include <AsyncElegantOTA.h>
#include "gamma.h"
#include "time.h"
#include <ESPDateTime.h>
#include <ETH.h>
#include <ESPmDNS.h>
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include <EEPROM.h>
#include <Preferences.h>
#include <esp_task_wdt.h>
#define EEPROM_SIZE 4096
#include <stdarg.h>
#include <stdio.h>
#include <ArduinoNvs.h>
#include <esp_partition.h>
#include <EEPROM_SPI_WE.h>
#include "SoftwareSerial.h"

SoftwareSerial swSer1;
#define ETH_ADDR 1
#define ETH_POWER_PIN 5
#define ETH_TYPE ETH_PHY_IP101
String urlink = "";
unsigned long previousMillis = 0;
unsigned long interval = 30000;
Preferences preferences;
int addr = 0;
const int eeprom_start = 4;
static File spiffsLogFile;
static char log_print_buffer[512];
TaskHandle_t Task1;
uint8_t level_groupe = 0;
int level_for_groupe = 1;
double on_progress_groupev2 = 0;
void serverRouting();
uint8_t oldlevel = 0;
String date = "";
unsigned long lastMs = 0;
unsigned long ms = millis();
/* const char* ssid = "SFR_F9A0";
const char* password = "fnex7aaf65wfxy9tft83"; */
const char *ssid = "vivenda";
const char *password = "wingmakers";
const char *www_username = "admin";
const char *www_password = "esp32";
AsyncWebServer httpServer(80);
bool noconnection = false;
String authFailResponse = F("Authentication rater");
int turn = 0;

/* pour tester la voie serie en echo */
/* void checkSwSerial(SoftwareSerial *ss)
{
    esp_task_wdt_reset();
  byte ch;
  while (!Serial.available())
    ;
  ss->enableTx(true);
  while (Serial.available())
  {
    ch = Serial.read();
    ss->write(ch);
  }
  ss->enableTx(false);
  // wait 1 second for the reply from SOftwareSerial if any
  delay(1000);
  if (ss->available())
  {
    Serial.print(PSTR("\nResult:"));
    while (ss->available())
    {
      ch = (byte)ss->read();
      Serial.print(ch < 0x10 ? PSTR(" 0") : PSTR(" "));
      Serial.print(ch, HEX);
    }
    Serial.println();
  }
} */
void test(int combien){
  
      if (swSer1.available()>0)
    {
      //  Serial.println(swSer1.read(),HEX);
      int ok = swSer1.read();
      Serial.print(ok);
    }

}
String sha256(String payloadStr)
{
  const char *payload = payloadStr.c_str();
  int size = 20;
  byte shaResult[size];
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
  const size_t payloadLength = strlen(payload);
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)payload, payloadLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);
  String hashStr = "";
  for (uint16_t i = 0; i < size; i++)
  {
    String hex = String(shaResult[i], HEX);
    if (hex.length() < 2)
    {
      hex = "0" + hex;
    }
    hashStr += hex;
  }
  return hashStr;
}

String stringToWrite = "test";
String stringToRead = "";
bool is_authenticated(AsyncWebServerRequest *request)
{

  Serial.println(F("Enter is_authenticated"));
  if (request->hasHeader("Cookie"))
  {
    Serial.print(F("Found cookie: "));
    String cookie = request->header("Cookie");
    Serial.println(cookie);
    String token = sha256(String(www_username) + ":" +
                          String(www_password) + ":" +
                          request->client()->remoteIP().toString());
    // token = sha256(token);
    if (cookie.indexOf("ESPSESSIONID=" + token) != -1)
    {
      Serial.println("Authentication est vraie");
      return true;
    }
  }
  Serial.println(F("Authentication est fausse"));
  return false;
}

void setup_spiffs_logging()
{
  // open file in WRITE and APPEND mode to not overwrite old content.
  // you might want to choose only WRITE (or a previous delete call)
  // to remove the old log
  spiffsLogFile = SPIFFS.open("/spiffs_log.txt", FILE_WRITE FILE_APPEND);
}

void spiffs_logging_cleanup()
{
  spiffsLogFile.close();
}

int vprintf_into_spiffs(const char *szFormat, va_list args)
{
  // write evaluated format string into buffer
  int ret = vsnprintf(log_print_buffer, sizeof(log_print_buffer), szFormat, args);

  // output is now in buffer. write to file.
  if (ret >= 0)
  {
    // debug output
    printf("[Writing to SPIFFS] %.*s", ret, log_print_buffer);
    spiffsLogFile.write((uint8_t *)log_print_buffer, (size_t)ret);
    // to be safe in case of crashes: flush the output
    spiffsLogFile.flush();
  }
  return ret;
}

void setupDateTime()
{
  /* configuration après la connexion wifi
     vous pouvez utiliser le timeZone, le serveur et le timeout personnalisés vous pouvez utiliser le timeZone, le serveur et le timeout personnalisés*/
  // DateTime.setTimeZone(-4);
  // DateTime.setServer("asia.pool.ntp.org");
  // DateTime.begin(15 * 1000);
  DateTime.setServer("fr.pool.ntp.org");
  DateTime.setTimeZone("UTC-2");
  DateTime.begin();

  int tour = 0;
  while (!DateTime.isTimeValid())
  {
    tour++;
    Serial.println(F("Impossible d'obtenir l'heure du serveur."));

    delay(1000);
    if (tour == 10)
    {
      //ESP.restart();
     // break;
     
    }
  }
  Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
}

void logMemory()
{
  log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
}

void showTime()
{
  Serial.printf("TimeZone:      %s\n", DateTime.getTimeZone());
  Serial.printf("Up     Time:   %lu seconds\n", millis() / 1000);
  Serial.printf("Boot   Time:   %ld seconds\n", DateTime.getBootTime());
  Serial.printf("Cur    Time:   %ld seconds\n",
                DateTime.getBootTime() + millis() / 1000);
  Serial.printf("Now    Time:   %ld\n", DateTime.now());
  Serial.printf("OS     Time:   %ld\n", DateTime.osTime());
  Serial.printf("NTP    Time:   %ld\n", DateTime.ntpTime(2 * 1000L));

  Serial.printf("Local  Time:   %s\n",
                DateTime.format(DateFormatter::SIMPLE).c_str());
  Serial.println("===========");
  time_t t = time(NULL);
  Serial.printf("OS local:     %s", asctime(localtime(&t)));
  Serial.printf("OS UTC:       %s", asctime(gmtime(&t)));
}
void WiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    Serial.println("WiFi lost connection");

    break;
  }
}
EEPROM_SPI_WE myEEP = EEPROM_SPI_WE(&SPI, csPin);

void setup(void)
{
  Serial.begin(115200);
  /*   pinMode(DMX_DIRECTION_PIN, OUTPUT);
    digitalWrite(DMX_DIRECTION_PIN, HIGH); */
  pinMode(DMX_SERIAL_OUTPUT_PIN, OUTPUT);
  ESP32DMX.startOutput(DMX_SERIAL_OUTPUT_PIN);
		swSer1.begin(57600, SWSERIAL_8N1, rxuart, txuart, false);
  /*   int intToWrite = 42;
    eepromWriteInt(4, intToWrite);
    intToWrite = 2;
    eepromWriteInt(6, intToWrite); exemples write */

  /*   int intToRead = 0;
    intToRead = eepromReadInt(4);
    Serial.print("intToRead (Address = 4): ");
    Serial.println(intToRead);
    intToRead = 0;
    intToRead = eepromReadInt(6);
    Serial.print("intToRead (Address = 6): ");
    Serial.println(intToRead); exemple read*/
  while (!Serial)
    continue;

  struct SpiRamAllocator
  {
    void *allocate(size_t size)
    {
      return ps_malloc(size);
    }
    void deallocate(void *pointer)
    {
      free(pointer);
    }
  };

  using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;
  logMemory();
  byte *psdRamBuffer = (byte *)ps_malloc(2000000);
  logMemory();
  free(psdRamBuffer);
  logMemory();
  if (myEEP.init())
  {
    Serial.println("external EEPROM connected");
  }
  else
  {
    Serial.println("external EEPROM does not respond");
    while (1)
      ;
  }
  /* You can change the SPI clock speed. The default of is 8 MHz */
  // myEEP.setSPIClockSpeed(4000000); // use AFTER init()!

  /* Select the page size of your EEPROM.
   * Choose EEPROM_PAGE_SIZE_xxx,
   * with xxx = 16, 32, 64, 128 or 256
   */
  myEEP.setPageSize(EEPROM_PAGE_SIZE_64);
  delay(500);
  // Serial.println(ESP.getSketchSize());
  IPAddress ip(192, 168, 0, 92);
  IPAddress dns(1, 1, 1, 1);
  IPAddress gateway(192, 168, 0, 254);
  IPAddress subnet(255, 255, 255, 0);
  //
  /*Pour la connection au wifi il faut d'abord le configurer puis le lancer mais pour la connexion ethernet c'est l'inverse il faut d'abord lancer l’Ethernet puis le configurer
  sinon l'esp redémarre*/
  // WiFi.config(ip, gateway, subnet, dns);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_TYPE);
  ETH.config(ip, gateway, subnet, dns);
  /*   WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password); */
  Serial.println("");
  // Attente de la connexion
  int tour = 0;
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   tour++;
  //   delay(500);
  //   Serial.print(F("."));
  //   /* if (tour==100){
  //     noconnection=true;
  //   break;
  //   } */
  // }
  EEPROM.begin(EEPROM_SIZE);
  /* for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, 0);
  }

  EEPROM.commit();*/
  delay(500);

  Serial.print(F("Inizializing SPIFFS..."));
  if (SPIFFS.begin())
  {

    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail and restart"));
    esp_restart();
  }
  // utilise le gpio39 pour les chiffres aleatoire
  randomSeed(analogRead(39));
  // Devrait charger la configuration par défaut si elle est exécutée pour la première fois.
  Serial.println(F("Loading configuration..."));
  loadConfiguration(filename2, config);
  // afficher le fichier de configuration
  Serial.println(F("Imprimer le fichier de configuration..."));
  printFile(filename2);
  /* // Créer un fichier de configuration
   Serial.println(F("Sauvegarde de la configuration..."));
   saveConfiguration(filename2, config); */
  setupDateTime();
  showTime();
  date = DateTime.format(DateFormatter::DATE_ONLY);
  setup_spiffs_logging();
  // install new logging function
  esp_log_set_vprintf(&vprintf_into_spiffs);

  /*     File root = SPIFFS.open("/");
    File file = root.openNextFile();
     while(file){

        Serial.print("FILE: ");
        Serial.println(file.name());

        file = root.openNextFile();
  } */
  Serial.println(F("Set routing for http server!"));
  serverRouting();
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      1024,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      1);        /* pin task to core 0 */

  addr = eeprom_start;
  for (int i = 0; i < 256; i++)
  {
    int actual_value = EEPROM.read(addr);
    /*     Serial.println("actual_value: ");

        Serial.println(actual_value); */
    dmxbuffer[i] = actual_value;
    addr = addr + 1;
    // ALLvalue[i] = ESP32DMX.getSlot(i);
    copyDMXToOutput();
  }
  // WiFi.onEvent(WiFiEvent);
  /*   Serial.println(WiFi.dnsIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.subnetCIDR());
    Serial.println(WiFi.subnetMask());
    Serial.println(F(""));
    Serial.print(F("Connected to "));
    Serial.println(ssid);
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP()); */
  Serial.println(ETH.dnsIP());
  Serial.println(ETH.gatewayIP());
  Serial.println(ETH.subnetCIDR());
  Serial.println(ETH.subnetMask());
  Serial.println(F(""));
  Serial.print(F("Connected to "));
  // Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(ETH.localIP());
  httpServer.begin();
  Serial.println(F("HTTP server started"));
  AsyncElegantOTA.begin(&httpServer, www_username, www_password); // Démarrer ElegantOTA
  if (!MDNS.begin("esp32"))
  {
    Serial.println("Error starting mDNS");
    /*  return; */
  }
  const char *token = www_username;
  AsyncElegantOTA.setID(token);
  groupe = 1;
  go_groupe();
  // permet de réinitialiser la mémoire externe eeprom si activer
  /*         for (size_t i =  0; i < 32767; i++)
      {
        myEEP.write(i,0);
      }   */
      swSer1.onReceive(test);
      
}

void Task1code(void *pvParameters)
{

  for (;;)
  {
    /*
                addr = eeprom_start;
        for (int i = 0; i < 256; i++)
        {
          EEPROM.write(addr, ESP32DMX.getSlot(i));
          addr = addr + 1;

          // ALLvalue[i] = ESP32DMX.getSlot(i);
        }
        EEPROM.write(0, level);
        EEPROM.commit();
         go_save();*/

    // checkSwSerial(&swSer1);
    
    if (swSer1.available()>0)
    {
      /* code */
    
     int ok;
    while(swSer1.available()) {
      //  Serial.println(swSer1.read(),HEX);
ok =     swSer1.read();
       swSer1.write(ok);
         //Serial.println(ok,HEX);
    }
    switch (ok)
    {
    case 0X02:
     Serial.println("c'est 0x02");
      break;
    
    default:
      break;
    }
    Serial.println(ok,HEX);
    }
  }
}

void loop(void)
{
  /* unsigned long currentMillis = millis();
   // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
      if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
     {
       Serial.print(millis());
       Serial.println("Reconnection au WiFi...");
       WiFi.disconnect();
       WiFi.reconnect();
       previousMillis = currentMillis;
     } */
       

  switch (level_for_groupe)
  {
  case 1:

    level_groupe = ESP32DMX.getSlot(arraygroupe[1]);

    break;
  case 2:
    level_groupe = ESP32DMX.getSlot(arraygroupe[2]);
    break;
  case 3:
    level_groupe = ESP32DMX.getSlot(arraygroupe[3]);
    break;
  case 4:
    level_groupe = ESP32DMX.getSlot(arraygroupe[4]);
    break;
  case 5:
    level_groupe = ESP32DMX.getSlot(arraygroupe[5]);
    break;
  case 6:
    level_groupe = ESP32DMX.getSlot(arraygroupe[6]);
    break;
  case 7:
    level_groupe = ESP32DMX.getSlot(arraygroupe[7]);
    break;
  case 8:
    level_groupe = ESP32DMX.getSlot(arraygroupe[8]);
    break;
  case 9:
    level_groupe = ESP32DMX.getSlot(arraygroupe[9]);
    break;
  case 10:
    level_groupe = ESP32DMX.getSlot(arraygroupe[10]);
    break;
  case 11:
    level_groupe = ESP32DMX.getSlot(arraygroupe[11]);
    break;
  case 12:
    level_groupe = ESP32DMX.getSlot(arraygroupe[12]);
    break;
  case 13:
    level_groupe = ESP32DMX.getSlot(arraygroupe[13]);
    break;
  case 14:
    level_groupe = ESP32DMX.getSlot(arraygroupe[14]);
    break;
  case 15:
    level_groupe = ESP32DMX.getSlot(arraygroupe[15]);
    break;
  case 16:
    level_groupe = ESP32DMX.getSlot(arraygroupe[16]);
    break;
  case 17:
    level_groupe = ESP32DMX.getSlot(arraygroupe[17]);
    break;
  case 18:
    level_groupe = ESP32DMX.getSlot(arraygroupe[18]);
    break;
  case 19:
    level_groupe = ESP32DMX.getSlot(arraygroupe[19]);
    break;
  case 20:
    level_groupe = ESP32DMX.getSlot(arraygroupe[20]);
    break;
  case 21:
    level_groupe = ESP32DMX.getSlot(arraygroupe[21]);
    break;
  case 22:
    level_groupe = ESP32DMX.getSlot(arraygroupe[22]);
    break;
  case 23:
    level_groupe = ESP32DMX.getSlot(arraygroupe[23]);
    break;
  case 24:
    level_groupe = ESP32DMX.getSlot(arraygroupe[24]);
    break;
  case 25:
    level_groupe = ESP32DMX.getSlot(arraygroupe[25]);
    break;
  case 26:
    level_groupe = ESP32DMX.getSlot(arraygroupe[26]);
    break;
  case 27:
    level_groupe = ESP32DMX.getSlot(arraygroupe[27]);
    break;
  case 28:
    level_groupe = ESP32DMX.getSlot(arraygroupe[28]);
    break;
  case 29:
    level_groupe = ESP32DMX.getSlot(arraygroupe[29]);
    break;
  case 30:
    level_groupe = ESP32DMX.getSlot(arraygroupe[30]);
    break;
  case 31:
    level_groupe = ESP32DMX.getSlot(arraygroupe[31]);
    break;
  case 32:
    level_groupe = ESP32DMX.getSlot(arraygroupe[32]);
    break;
  default:
    level_groupe = ESP32DMX.getSlot(arraygroupe[1]);
    break;
  }
  level = ESP32DMX.getSlot(canal);

  canal_value = level;
  DateTime.forceUpdate();
  if (millis() - ms > 15 * 1000L)
  {
    ms = millis();

    if (!DateTime.isTimeValid())
    {
      Serial.println(F("Impossible d'obtenir l'heure du serveur, réessayer.."));
      DateTime.begin();
    }
    else
    {
      date = DateTime.format(DateFormatter::DATE_ONLY);
    }
  }

  if (get_loop == true)
  {
    // if ((increment_activer==true) && (decrement_activer == true));
    if (increment_activer == true)
    {
      turn = 0;
      level = ESP32DMX.getSlot(canal);
      EEPROM.write(1, turn);
      EEPROM.write(0, level);
      EEPROM.commit();
      int Delayv2 = Delay * 1000;
      while (gamma(level) < constrain(increment, offset_min, offset_max))
      {
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        turn++;
        level++;
        dmxbuffer[canal] = gamma(level);
        copyDMXToOutput();
        EEPROM.write(1, turn);
        EEPROM.write(0, level);
        EEPROM.commit();
        Serial.println(gamma(level));
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        vTaskDelay(Delayv2);
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini"));
      get_loop = false;
      Serial.print(F("level: "));
      Serial.println(level);
      /*             dmxbuffer[canal] = offset_max;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }

    if (variation_increment_activer == true)
    {
      turn = 0;
      level = ESP32DMX.getSlot(canal);
      EEPROM.write(1, turn);
      EEPROM.write(0, level);
      EEPROM.commit();
      int Delayv2 = Delay * 1000;
      while (gamma(level) < offset_max)
      {
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        turn++;
        level++;
        EEPROM.write(1, turn);
        EEPROM.write(0, level);
        EEPROM.commit();
        dmxbuffer[canal] = gamma(level);
        copyDMXToOutput();
        EEPROM.write(1, turn);
        EEPROM.write(0, level);
        EEPROM.commit();
        Serial.println(gamma(level));
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        vTaskDelay(Delayv2);
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini1"));
      get_loop = false;
      Serial.print(F("level: "));
      Serial.println(level);
      /*       dmxbuffer[canal] = offset_max;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }
  }

  if (get_loop == true)
  {

    // if ((increment_activer==true) && (decrement_activer == true));
    if (decrement_activer == true)
    {
      turn = 0;
      level = ESP32DMX.getSlot(canal);
      EEPROM.write(1, turn);
      EEPROM.write(0, level);
      EEPROM.commit();
      int Delayv2 = Delay * 1000;
      while (gamma(level) > constrain(decrement, offset_min, offset_max))
      {
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        turn++;
        level--;
        EEPROM.write(1, turn);
        EEPROM.write(0, level);
        EEPROM.commit();
        dmxbuffer[canal] = gamma(level);
        copyDMXToOutput();
        vTaskDelay(Delayv2);
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        Serial.println(gamma(level));
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini2"));
      get_loop = false;
      Serial.print(F("level: "));
      Serial.println(level);
      /*       dmxbuffer[canal] = offset_min;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }

    if (variation_decrement_activer == true)
    {
      turn = 0;
      level = ESP32DMX.getSlot(canal);
      EEPROM.write(1, turn);
      EEPROM.write(0, level);
      EEPROM.commit();
      int Delayv2 = Delay * 1000;
      while (gamma(level) > offset_min)
      {
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        turn++;
        level--;
        EEPROM.write(1, turn);
        EEPROM.write(0, level);
        EEPROM.commit();
        dmxbuffer[canal] = gamma(level);
        copyDMXToOutput();

        vTaskDelay(Delayv2);
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        Serial.println(gamma(level));
        level = EEPROM.read(0);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini3"));
      get_loop = false;
      Serial.print(F("level: "));
      Serial.println(level);
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }
  }

  if (on_progress_activer == true)
  {
    turn = 0;
    level = ESP32DMX.getSlot(canal);
    EEPROM.write(1, turn);
    EEPROM.write(0, level);
    EEPROM.commit();
    double on_progressv2 = on_progress * 1000;
    Serial.print(F("offsetmax: "));
    Serial.println(offset_max);
    Serial.println(on_progressv2);

    while (gamma(level) < offset_max)
    {
      level = EEPROM.read(0);
      turn = EEPROM.read(1);
      turn++;
      level++;
      EEPROM.write(1, turn);
      EEPROM.write(0, level);
      EEPROM.commit();
      dmxbuffer[canal] = gamma(level);

      copyDMXToOutput();
      Serial.println(gamma(level));
      level = EEPROM.read(0);
      turn = EEPROM.read(1);
      vTaskDelay(on_progressv2);
      level = EEPROM.read(0);
      turn = EEPROM.read(1);

      if (stop_i == true)
      {
        Serial.println(F("erreur"));
        stop_i = false;
        break;
      }
    }
    Serial.println("finito");
    Serial.print("level: ");
    Serial.println(level);
    addr = eeprom_start;
    for (int i = 0; i < 256; i++)
    {
      EEPROM.write(addr, ESP32DMX.getSlot(i));
      addr = addr + 1;

      // ALLvalue[i] = ESP32DMX.getSlot(i);
    }
    EEPROM.write(0, level);
    EEPROM.commit();
  }

  if (off_progress_activer == true)
  {
    turn = 0;
    level = ESP32DMX.getSlot(canal);
    EEPROM.write(1, turn);
    EEPROM.write(0, level);
    EEPROM.commit();
    double off_progressv2 = off_progress * 1000;
    Serial.println(off_progressv2);
    Serial.print(F("offsetmin: "));
    Serial.println(offset_min);
    while (gamma(level) > offset_min)
    {
      level = EEPROM.read(0);
      turn = EEPROM.read(1);
      turn++;
      level--;
      EEPROM.write(1, turn);
      EEPROM.write(0, level);
      EEPROM.commit();
      dmxbuffer[canal] = gamma(level);
      copyDMXToOutput();
      Serial.println(gamma(level));
      level = EEPROM.read(0);
      turn = EEPROM.read(1);
      vTaskDelay(off_progressv2);
      level = EEPROM.read(0);
      turn = EEPROM.read(1);
      if (stop_i == true)
      {
        Serial.println(F("erreur"));
        stop_i = false;
        break;
      }
    }

    Serial.println(F("finito2"));
    Serial.print(F("level: "));
    Serial.println(level);
    addr = eeprom_start;
    for (int i = 0; i < 256; i++)
    {
      EEPROM.write(addr, ESP32DMX.getSlot(i));
      addr = addr + 1;

      // ALLvalue[i] = ESP32DMX.getSlot(i);
    }
    EEPROM.write(0, level);
    EEPROM.commit();
  }

  if (on_progress_groupe_activer == true)
  {
    turn = 0;
    EEPROM.write(1, turn);
    EEPROM.commit();
    double on_progress_groupev2 = on_progress_groupe * 1000;
    Serial.println(on_progress_groupev2);
    Serial.print(F("offsetmax groupe: "));
    Serial.println(offset_max_groupe);
    for (int i = 0; i < 32; i++)
    {

      Serial.print(F("i: "));
      Serial.println(i);
      int num_groupe = arraygroupe[i];
      Serial.print(F("num_groupe: "));
      Serial.println(num_groupe);
      if (num_groupe == 0)
      {
        continue;
      }
      else
      {
        while (gamma(level) < offset_max_groupe)
        {
          level = EEPROM.read(0);
          turn = EEPROM.read(1);
          Serial.print(F("turn: "));
          Serial.println(turn);
          turn++;
          level++;
          dmxbuffer[num_groupe] = gamma(level);
          for (int i = 0; i < 32; i++)
          {
            if (arraygroupe[i] == 0)
            {

              continue;
            }
            dmxbuffer[arraygroupe[i]] = gamma(level);
          }
          copyDMXToOutput();
          EEPROM.write(0, level);
          EEPROM.write(1, turn);
          EEPROM.commit();
          Serial.println(gamma(level));
          level = EEPROM.read(0);
          turn = EEPROM.read(1);
          vTaskDelay(on_progress_groupev2);
          level = EEPROM.read(0);
          turn = EEPROM.read(1);
          if (stop_i == true)
          {
            Serial.println(F("erreur"));
            stop_i = false;
            break;
          }
        }
      }
    }
    addr = eeprom_start;
    for (int i = 0; i < 256; i++)
    {
      EEPROM.write(addr, ESP32DMX.getSlot(i));
      addr = addr + 1;

      // ALLvalue[i] = ESP32DMX.getSlot(i);
    }
    EEPROM.write(0, level);
    EEPROM.commit();
  }

  if (off_progress_groupe_activer == true)
  {
    turn = 0;
    EEPROM.write(1, turn);
    EEPROM.commit();
    double off_progress_groupev2 = off_progress_groupe * 1000;
    Serial.println(off_progress_groupev2);
    Serial.print(F("offsetmin groupe: "));
    Serial.println(offset_min_groupe);

    for (int i = 0; i < 32; i++)
    {

      Serial.print(F("i: "));
      Serial.println(i);
      int num_groupe = arraygroupe[i];
      Serial.print(F("num_groupe: "));
      Serial.println(num_groupe);

      level = ESP32DMX.getSlot(num_groupe);

      Serial.print(F("level "));
      Serial.println(level);
      if (num_groupe == 0)
      {
        Serial.println("break zero");
        continue;
      }
      else
      {
        if (level == 0)
        {
          Serial.println("break zero 2");
          break;
        }

        while (gamma(level) > offset_min_groupe)
        {

          Serial.print(F("turn: "));
          Serial.println(turn);
          turn++;
          level--;
          dmxbuffer[num_groupe] = gamma(level);
          for (int i = 0; i < 32; i++)
          {
            if (arraygroupe[i] == 0)
            {

              continue;
            }
            dmxbuffer[arraygroupe[i]] = gamma(level);
          }
          copyDMXToOutput();
          EEPROM.write(0, level);
          EEPROM.write(1, turn);
          EEPROM.commit();
          Serial.println(gamma(level));
          level = EEPROM.read(0);
          turn = EEPROM.read(1);
          vTaskDelay(off_progress_groupev2);
          level = EEPROM.read(0);
          turn = EEPROM.read(1);
          if (stop_i == true)
          {
            Serial.println(F("erreur"));
            stop_i = false;
            break;
          }
        }
      }
    }
    addr = eeprom_start;
    for (int i = 0; i < 256; i++)
    {
      EEPROM.write(addr, ESP32DMX.getSlot(i));
      addr = addr + 1;

      // ALLvalue[i] = ESP32DMX.getSlot(i);
    }
    EEPROM.write(0, level);
    EEPROM.commit();
  }

  if (get_loop_groupe == true)
  {
    // if ((increment_activer==true) && (decrement_activer == true));
    if (increment_groupe_activer == true)
    {
      level_for_groupe = 1;
      turn = 0;
      EEPROM.write(1, turn);
      EEPROM.write(3, level_groupe);
      EEPROM.commit();
      int Delay_groupev2 = Delay_groupe * 1000;
      Serial.println("level_groupe: ");
      Serial.println(level_groupe);
      while (gamma(level_groupe) < constrain(increment_groupe, offset_min_groupe, offset_max_groupe))
      {
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        turn++;
        level_groupe++;
        for (int i = 0; i < 32; i++)
        {
          if (arraygroupe[i] == 0)
          {

            continue;
          }
          dmxbuffer[arraygroupe[i]] = gamma(level_groupe);
        }
        copyDMXToOutput();
        EEPROM.write(1, turn);
        EEPROM.write(3, level_groupe);
        EEPROM.commit();
        Serial.println(gamma(level_groupe));
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        vTaskDelay(Delay_groupev2);
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini"));
      get_loop_groupe = false;
      Serial.print(F("level: "));
      Serial.println(level_groupe);
      /*             dmxbuffer[canal] = offset_max;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }

    if (variation_increment_groupe_activer == true)
    {
      level_for_groupe = 1;
      turn = 0;
      EEPROM.write(1, turn);
      EEPROM.write(3, level_groupe);
      EEPROM.commit();
      int Delay_groupev2 = Delay_groupe * 1000;
      Serial.println("level_groupe: ");
      Serial.println(level_groupe);
      while (gamma(level_groupe) < offset_max_groupe)
      {
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        turn++;
        level_groupe++;
        for (int i = 0; i < 32; i++)
        {
          if (arraygroupe[i] == 0)
          {

            continue;
          }
          dmxbuffer[arraygroupe[i]] = gamma(level_groupe);
        }
        copyDMXToOutput();
        EEPROM.write(1, turn);
        EEPROM.write(3, level_groupe);
        EEPROM.commit();
        Serial.println(gamma(level_groupe));
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        vTaskDelay(Delay_groupev2);
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini"));
      get_loop_groupe = false;
      Serial.print(F("level: "));
      Serial.println(level_groupe);
      /*             dmxbuffer[canal] = offset_max;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }
  }

  if (get_loop_groupe == true)
  {

    // if ((increment_activer==true) && (decrement_activer == true));
    if (decrement_groupe_activer == true)
    {
      level_for_groupe = 1;
      turn = 0;
      EEPROM.write(1, turn);
      level_groupe = EEPROM.read(3);
      EEPROM.commit();
      Serial.println("level_groupe: ");
      Serial.println(level_groupe);
      int Delay_groupev2 = Delay_groupe * 1000;
      while (level_groupe > constrain(decrement_groupe, offset_min_groupe, offset_max_groupe))
      {

        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        turn++;
        level_groupe--;
        EEPROM.write(1, turn);
        EEPROM.write(3, level_groupe);
        EEPROM.commit();
        for (int i = 0; i < 32; i++)
        {
          if (arraygroupe[i] == 0)
          {

            continue;
          }
          dmxbuffer[arraygroupe[i]] = gamma(level_groupe);
        }
        copyDMXToOutput();
        vTaskDelay(Delay_groupev2);
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        Serial.println(gamma(level_groupe));
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini2"));
      get_loop_groupe = false;
      Serial.print(F("level: "));
      Serial.println(level_groupe);
      /*       dmxbuffer[canal] = offset_min;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }

    if (variation_decrement_groupe_activer == true)
    {
      level_for_groupe = 1;
      turn = 0;
      EEPROM.write(1, turn);
      level_groupe = EEPROM.read(3);
      EEPROM.commit();
      Serial.println("level_groupe: ");
      Serial.println(level_groupe);
      int Delay_groupev2 = Delay_groupe * 1000;
      while (gamma(level_groupe) > offset_min_groupe)
      {

        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        turn++;
        level_groupe--;
        EEPROM.write(1, turn);
        EEPROM.write(3, level_groupe);
        EEPROM.commit();
        for (int i = 0; i < 32; i++)
        {
          if (arraygroupe[i] == 0)
          {

            continue;
          }
          dmxbuffer[arraygroupe[i]] = gamma(level_groupe);
        }
        copyDMXToOutput();
        vTaskDelay(Delay_groupev2);
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        Serial.println(gamma(level_groupe));
        level_groupe = EEPROM.read(3);
        turn = EEPROM.read(1);
        if (stop_i == true)
        {
          Serial.println(F("erreur"));
          stop_i = false;
          break;
        }
      }
      Serial.println(F("fini2"));
      get_loop_groupe = false;
      Serial.print(F("level: "));
      Serial.println(level_groupe);
      /*       dmxbuffer[canal] = offset_min;
            copyDMXToOutput(); */
      addr = eeprom_start;
      for (int i = 0; i < 256; i++)
      {
        EEPROM.write(addr, ESP32DMX.getSlot(i));
        addr = addr + 1;

        // ALLvalue[i] = ESP32DMX.getSlot(i);
      }
      EEPROM.write(0, level);
      EEPROM.commit();
    }
  }

  if (redirect == true)
  {
    Serial.println(F("censé retourner a la page requete"));
    // httpServer.addHandler();
    httpServer.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
  String url ="/requete";
  request->redirect(url); });
  }
  if (redirect == true || on_progress_activer == true || off_progress_activer == true || increment_activer == true || decrement_activer == true || variation_increment_activer == true || variation_decrement_activer == true || off_progress_groupe_activer == true || on_progress_groupe_activer == true || increment_groupe_activer == true || decrement_groupe_activer == true || variation_increment_groupe_activer == true || variation_decrement_groupe_activer == true)
  {
    go_save();
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

    /*       httpServer.on("/offwait", HTTP_POST, [](AsyncWebServerRequest *request){
       request->send(200);
     }); */
  }
}
String getContentType(String filename)
{
  if (filename.endsWith(F(".htm")))
    return F("text/html");
  else if (filename.endsWith(F(".html")))
    return F("text/html");
  else if (filename.endsWith(F(".css")))
    return F("text/css");
  else if (filename.endsWith(F(".js")))
    return F("application/javascript");
  else if (filename.endsWith(F(".json")))
    return F("application/json");
  else if (filename.endsWith(F(".png")))
    return F("image/png");
  else if (filename.endsWith(F(".gif")))
    return F("image/gif");
  else if (filename.endsWith(F(".jpg")))
    return F("image/jpeg");
  else if (filename.endsWith(F(".jpeg")))
    return F("image/jpeg");
  else if (filename.endsWith(F(".ico")))
    return F("image/x-icon");
  else if (filename.endsWith(F(".xml")))
    return F("text/xml");
  else if (filename.endsWith(F(".pdf")))
    return F("application/x-pdf");
  else if (filename.endsWith(F(".zip")))
    return F("application/x-zip");
  else if (filename.endsWith(F(".gz")))
    return F("application/x-gzip");
  return F("text/plain");
}
bool handleFileRead(AsyncWebServerRequest *request, String path)
{

  Serial.print(F("handleFileRead: "));
  Serial.println(path);
  if (connecter)
  {
    if (!is_authenticated(request))
    {
      urlink = request->url();
      Serial.println(F("Go au login screen!"));
      path = "/login.html";
    }
    else
    {

      if (connecter)
      {
        if (path.endsWith("/"))
          path += F("index.html");
        else
        {
          // request->send(200, getContentType(path), path);
          request->redirect(path);
        }
      }
      else
      {
        if (path.endsWith("/"))
          path += F("index2.html");
        else
        {
          // request->send(200, getContentType(path), path);
          request->redirect(path);
        }
      }
    }
  }
  else
  {

    if (connecter)
    {

      if (path.endsWith("/"))
        path += F("index.html");
      else
      {
        // request->send(200, getContentType(path), path);
        request->redirect(path);
      }
    }
    else
    {

      if (path.endsWith("/"))
        path += F("index2.html");
      else
      {
        // request->send(200, getContentType(path), path);
        request->redirect(path);
      }
    }
  }
  String contentType = getContentType(path); // Obtenir le type MIME
  String pathWithGz = path + F(".gz");
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  { //  Si le fichier existe, soit en tant qu'archive compressée, soit en tant que fichier normal.
    bool gzipped = false;
    if (SPIFFS.exists(pathWithGz))
    {                   // S'il y a une version compressée disponible
      path += F(".gz"); // Utilisez la version compressée
      gzipped = true;
    }
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, path, contentType);
    if (gzipped)
    {
      response->addHeader("Content-Encoding", "gzip");
    }
    Serial.print(F("vrai chemin du fichier: "));
    Serial.println(path);
    request->send(response);
    return true;
  }
  Serial.println(String(F("\tfichier n'a pas etait trouver: ")) + path);
  handleNotFound(request);
  return false; //  Si le fichier n'existe pas, retournez false.
}

void handleNotFound(AsyncWebServerRequest *request)
{
  String message = "page introuvable\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";
  for (uint8_t i = 0; i < request->args(); i++)
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(500, "text/plain", message);
}
void handleLogin(AsyncWebServerRequest *request)
{
  if (connecter)
  {
    Serial.println("Handle login");
    String msg;
    if (request->hasHeader("Cookie"))
    {
      // Print cookies
      Serial.print(F("cookie trouver: "));
      String cookie = request->header("Cookie");
      Serial.println(cookie);
    }
    if (request->hasArg("username") && request->hasArg("password"))
    {
      Serial.print("parametres trouver: ");
      if (request->arg("username") == String(www_username) && request->arg("password") == String(www_password))
      {

        AsyncWebServerResponse *response = request->beginResponse(308); // Envoie une redirection 301
        response->addHeader("Location", urlink);
        response->addHeader("Cache-Control", "no-cache");
        String token = sha256(String(www_username) + ":" + String(www_password) + ":" + request->client()->remoteIP().toString());
        Serial.print("Token: ");
        Serial.println(token);
        response->addHeader("Set-Cookie", "ESPSESSIONID=" + token);
        request->send(response);
        Serial.println("Login reussi");
        return;
      }
      else
        msg = "mauvais nom d'utilisateur ou mot de passe, veuillez recommencer";
      Serial.println(F("Login rater"));
      AsyncWebServerResponse *response = request->beginResponse(301); // Envoie une redirection 301
      response->addHeader("Location", "/login.html?msg=" + msg);
      response->addHeader("Cache-Control", "no-cache");
      request->send(response);
      return;
    }
  }
  else
  {

    request->send(200, "text/plain", "l'authentification n'est pas activer");

    // request->send(301,"text/plain","l'authentification n'est pas activer");
    return;
  }
}
/**
 * Gestion de la déconnexion (il suffit de retirer le jeton correct et de rediriger vers le formulaire de connexion)
 */

void handleLogout(AsyncWebServerRequest *request)
{

  if (connecter)
  {
    Serial.println("Deconnection");
    AsyncWebServerResponse *response = request->beginResponse(301); // Sends 301 redirect
    response->addHeader("Location", "/login.html?msg=tu n'es plus connecter");
    response->addHeader("Cache-Control", "no-cache");
    response->addHeader("Set-Cookie", "ESPSESSIONID=0");
    request->send(response);
    return;
  }
  else
  {

    request->send(200, "text/plain", "l'authentification n'est pas activer");
    // request->send(301,"text/plain","l'authentification n'est pas activer");
    return;
  }
}
// Check if header is present and correct
void handlerequete(AsyncWebServerRequest *request);
void handleFileUpload(AsyncWebServerRequest *request, String filename3, size_t index, uint8_t *data, size_t len, bool final);
void handleInfo(AsyncWebServerRequest *request);
// void handlecanal(AsyncWebServerRequest *request);

void serverRouting()
{

  // Point final de repos externe (hors authentification))
  // handleGenericArgs
  // saveConfiguration("/config.json", config);
  httpServer.on("/requete", HTTP_ANY, handlerequete);
  httpServer.on("/info", HTTP_ANY, handleInfo);
  httpServer.on("/array_groupe", HTTP_ANY, go_lire_groupe_post);

  // httpServer.on("/canal", HTTP_GET, handlecanal);

  httpServer.on("/login", HTTP_POST, handleLogin);
  httpServer.on("/logout", HTTP_GET, handleLogout);
  httpServer.on("/login", HTTP_GET, handleLogout);
  httpServer.on("/checkactiver", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                  // permet de gerer le checkactivber
                  connecter = true;
                   go_save();
                  Serial.println("c'est vrai");
                  request->send(204); });
  httpServer.on("/checkdesactiver", HTTP_GET, [](AsyncWebServerRequest *request)
                {
  //permet de gerer le checkdesactivber
    //request->send(401);
 connecter =false;
  go_save();
 Serial.println("c'est faux");
 request->send(204);
 return connecter; });

  httpServer.on("/upload", HTTP_GET, [](AsyncWebServerRequest *request) { // si le client demande la page de téléchargement
    if (!handleFileRead(request, "/upload.html"))                         // l'envoyer s'il existe
      handleNotFound(request);                                            // sinon, répondre avec une erreur 404 (Not Found)
  });
  httpServer.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
                {                 
    if (!handleFileRead(request, "/new_index.html"))                
      handleNotFound(request); });
  /*
    httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
      if (!handleFileRead(request, "/new_index.html"))
        handleNotFound(request); });
   */

  httpServer.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->send(200); },
      [](AsyncWebServerRequest *request, const String &filename3, size_t index, uint8_t *data, size_t len, bool final)
      {
        handleFileUpload(request, filename3, index, data, len, final);
      });

  httpServer.onNotFound([](AsyncWebServerRequest *request) { // Si le client demande n'importe quel URI
    if (!handleFileRead(request, request->url()))
    {                          // l'envoyer s'il existe
      handleNotFound(request); // sinon, répondre avec une erreur 404 (Not Found)
      Serial.println(F("rien trouver"));
    }
  });
  Serial.println(F("Set cache!"));
  // Servez un fichier sans cache pour que chaque tuile soit téléchargée.
  httpServer.serveStatic("/requete?CMD=groupe&download", SPIFFS, "/requete?CMD=groupe&download").setCacheControl("no-cache, no-store, must-revalidate");
  // Servez toutes les autres pages avec un cache long pour que le navigateur les mette en cache.
  httpServer.serveStatic("/", SPIFFS, "/").setCacheControl("max-age=31536000");
  httpServer.serveStatic("/favicon.ico", SPIFFS, "/favico.ico", "max-age=31536000");
}
