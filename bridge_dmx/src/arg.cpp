#include "arg.h"
#include <LXESP32DMX.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "api.h"
#include <gamma.h>

// init des variables
int nb_name = 0;
bool cmd_all = false;
String name = "";
bool rip = false;
bool connecter = true;
DynamicJsonDocument dataX(32768);
DynamicJsonDocument dataV(16384);
DynamicJsonDocument dataT(16384);
DynamicJsonDocument dataG(16384);
bool connecter2 = true;
String final = "";
int args;
int canal = 1;
String message_canal = "";
int valeur_canal = 0;
String message_valeur_canal = "";
String off = "";
String message_off = "";
String on = "";
String message_on = "";
double on_progress = 0;
double off_progress = 0;
String message_on_progress = "";
String message_off_progress = "";
String ramidom = "";
String message_random = "";
int increment = 0;
String message_increment = "";
int decrement = 0;
String message_decrement = "";
String save = "";
String message_save = "";
String on_save = "";
String message_on_save = "";
String id = "";
const char *esp32 = "esp32";
String message_id = "";
String supp = "";
String message_supp = "";
String CMD = "";
String message_CMD = "";
String var = "";
String message_var = "";
String message_state = "";
String state = "";
bool on_activer = false;
bool off_activer = false;
bool decrement_activer = false;
bool increment_activer = false;
int valeur_canalv2 = 0;
bool on_progress_activer = false;
bool off_progress_activer = false;
uint8_t offset_max = 255;
uint8_t offset_min = 0;
uint8_t offset_max_groupe = 255;
uint8_t offset_min_groupe = 0;
String message_offset_max = "";
String message_offset_min = "";
bool on_save_activer = false;
bool save_activer = false;
String message_stop = "";
String stop = "";
double gamma_val = 1.0;
String message_gamma = "";
double Delay = 0.5;
String message_delay = "";
String restore = "";
String message_restore = "";
String download = "";
String message_download = "";
String upload = "";
String message_upload = "";
bool ready_download = false;
bool download_activer = false;
int groupe = 1;
String message_groupe = "";
String lire_groupe = "";
String message_lire_groupe = "";
int arraygroupe[33] = {};
int add_in_groupe = 1;
String message_add_in_groupe = "";
int supp_in_groupe = 1;
String message_supp_in_groupe = "";
String message_on_groupe = "";
String on_groupe = "";
String message_off_groupe = "";
String off_groupe = "";
double Delay_groupe = 0.5;
String message_delay_groupe = "";
String message_decrement_groupe = "";
String message_increment_groupe = "";
String message_on_progress_groupe = "";
String message_off_progress_groupe = "";
String message_valeur_canal_groupe = "";
String message_var_groupe = "";
String message_offset_max_groupe = "";
String message_offset_min_groupe = "";
double on_progress_groupe = 0;
double off_progress_groupe = 0;
bool off_progress_groupe_activer = false;
bool on_progress_groupe_activer = false;
bool decrement_groupe_activer = false;
bool increment_groupe_activer = false;
int increment_groupe = 0;
int decrement_groupe = 0;
String valeur_canal_groupe = "";
String message_canal_value_groupe = "";
String var_groupe = "";
String message_dimmer = "";
float dimmer = 100;
String message_nom_groupe = "";
String nom_groupe = "";
bool save_groupe = false;
bool save_canal = false;
String message_nom_canal = "";
String nom_canal = "";

void handlerequete(AsyncWebServerRequest *request)
{ // Handler pour requete

  messageclear();
  // verifie les identifiant
  if (connecter)
  {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();
  }
  args = request->args();
  for (int i = 0; i < args; i++)
  {
    Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str(), "param[%s]: %s\n", request->getParam(i)->value().c_str());
  }

  // verifie quel est a la commande demannder
  if (request->hasArg("CMD"))
  {
    CMD = request->getParam("CMD")->value().c_str();

    message_CMD.clear();
    Serial.println(CMD);

    if (CMD == "canaux")
    {
      save_canal = true;
      message_CMD += F("commande les canaux");

      if (request->hasArg("canal"))
      {
        canal = request->getParam("canal")->value().toInt();

        message_canal.clear();
        if (1 <= canal && canal < 256)
        {
          go_canal();
          message_canal += F("votre numero de canal est ");
          message_canal += canal;
        }
        else
        {
          message_canal = F("le numero canal n'est pas bon");
        }
      }
      if (request->hasArg("value_canal"))
      {
        valeur_canal = request->getParam("value_canal")->value().toInt();
        int valeur_canalv2 = map(valeur_canal, 0, 100, 0, 255);
        valeur_canal = valeur_canalv2;
        Serial.println(valeur_canal);
        message_valeur_canal.clear();
        if (0 <= valeur_canal && valeur_canal <= 255)
        {

          go_valeur();
          message_valeur_canal += F(" la valeur du canal est ");
          message_valeur_canal += valeur_canal;
          message_valeur_canal += F(" pour le canal ");
          message_valeur_canal += canal;
          go_save();
        }
        else
        {
          message_valeur_canal = F("la valeur donner au canal n'est pas bonne");
        }
      }

      if (request->hasArg("off"))
      {

        off = request->getParam("off")->value();
        message_off.clear();
        Serial.println(off);

        if (off == NULL)
        {
          off_activer = true;
          Serial.println(off_activer = true);
        }
        else
        {
          message_off = F("il ne faut pas mettre de valeur apres off");
        }
      }
      if (request->hasArg("on"))
      {

        on = request->getParam("on")->value();
        Serial.println(on);
        message_on.clear();
        if (on == NULL)
        {

          on_activer = true;
          Serial.println(on_activer = true);
        }
        else
        {
          message_on = F("il ne faut pas mettre de valeur apres on");
        }
      }

      if (on_activer == true)
      {
        go_on();
        go_save();
      }

      if (off_activer == true)
      {

        go_off();
        go_save();
      }

      if (request->hasArg("on_progress"))
      {

        on_progress = request->getParam("on_progress")->value().toDouble();
        Serial.println(on_progress);
        message_on_progress.clear();

        if ((0.004 <= on_progress) && (on_progress <= 1.0))
        {

          message_on_progress += F("allume progressivement avec un delay de ");
          message_on_progress += String(on_progress, 4);
          message_on_progress += F("s");
          message_on_progress += F(" entre chaque valeur au canal ");
          message_on_progress += canal;
          go_on_progresson_progress();
        }
        else
        {
          message_on_progress = "la valeur de on_progress n'est pas bonne";
        }
      }
      if (request->hasArg("off_progress"))
      {

        off_progress = request->getParam("off_progress")->value().toDouble();
        Serial.println(off_progress);
        message_off_progress.clear();

        if ((0.004 <= off_progress) && (off_progress <= 1.0))
        {
          /* code */

          message_off_progress += "eteind progressivement avec un delay de ";
          message_off_progress += String(off_progress, 4);
          message_off_progress += "s";
          message_off_progress += " entre chaque valeur au canal ";
          message_off_progress += canal;
          go_off_progresson_progress();
        }
        else
        {
          message_off_progress = "la valeur de off_progress n'est pas bonne";
        }
      }
      if (request->hasArg("random"))
      {

        ramidom = request->getParam("random")->value();
        message_random.clear();
        Serial.println(ramidom);

        if (ramidom == NULL)
        {
          go_random();
          message_random += "met une valeur aleatoire au canal ";
          message_random += canal;
        }
        else
        {
          message_random = "il ne faut pas mettre de valeur apres random";
        }
      }
      if (request->hasArg("increment"))
      {

        increment = request->getParam("increment")->value().toInt();
        message_increment.clear();
        int valeur_incrementv2 = map(increment, 0, 100, 0, 255);
        increment = valeur_incrementv2;
        Serial.println(increment);
        if (0 <= increment && increment <= 255)
        {
          increment_activer = true;
          message_increment += "incremente jusqu'a la valeur ";
          message_increment += increment;
          message_increment += " du canal ";
          message_increment += canal;
          go_increment();
        }
        else
        {
          message_increment = "la valeur d'increment n'est pas bonne";
        }
      }

      if (request->hasArg("decrement"))
      {

        decrement = request->getParam("decrement")->value().toInt();
        message_decrement.clear();
        int valeur_decrementv2 = map(decrement, 0, 100, 0, 255);
        decrement = valeur_decrementv2;

        Serial.println(decrement);

        if (0 <= decrement && decrement <= 255)
        {
          /* code */
          decrement_activer = true;
          message_decrement.clear();
          message_decrement += "decremente jusqu'a la valeur ";
          message_decrement += decrement;
          message_decrement += " du canal ";
          message_decrement += canal;
          go_decrement();
        }
        else
        {
          message_decrement = "la valeur de decrement n'est pas bonne";
        }
      }

      if (request->hasArg("on_save"))
      {

        on_save = request->getParam("on_save")->value();
        message_on_save.clear();
        Serial.println(on_save);

        if (on_save == NULL)
        {
          on_save_activer = true;
          /* code */
          save_activer = true;
          message_on_save = "allume aux valeurs sauvegarder precedement";
          loadConfiguration("/config.json", config);
          printFile("/config.json");
          go_on_save();
        }
        else
        {
          message_on_save = "il ne faut pas mettre de valeur apres on_save";
        }
      }

      if (request->hasArg("variation"))
      {

        var = request->getParam("variation")->value();
        message_var.clear();
        Serial.println(var);

        if (var == NULL)
        {
          go_variation();
        }
        else
        {
          message_var = "il ne faut pas mettre de valeur apres variation";
        }
      }

      if (request->hasArg("offset_max"))
      {

        offset_max = request->getParam("offset_max")->value().toInt();
        message_offset_max.clear();
        Serial.println(offset_max);
        int offset_maxv2 = map(offset_max, 0, 100, 1, 255);
        offset_max = offset_maxv2;
        Serial.println(offset_max);
        if (1 <= offset_max && offset_max <= 255)
        {
          /* code */
          // decrement_activer=true;

          message_offset_max += "l'offset_max est de  ";
          message_offset_max += offset_max;
          message_offset_max += " au canal ";
          message_offset_max += canal;
          // go_save();
          // go_decrement();
        }
        else
        {
          message_offset_max = "la valeur de l'offset_max n'est pas bonne";
        }
      }

      if (request->hasArg("offset_min"))
      {

        offset_min = request->getParam("offset_min")->value().toInt();
        message_offset_min.clear();
        Serial.println(offset_min);
        int offset_minv2 = map(offset_min, 0, 100, 0, 254);
        offset_min = offset_minv2;
        Serial.println(offset_min);
        if (0 <= offset_min && offset_min <= 254)
        {
          /* code */
          // decrement_activer=true;

          message_offset_min += "l'offset_min est de  ";
          message_offset_min += offset_min;
          message_offset_min += " au canal ";
          message_offset_min += canal;
          // go_save();
          //  go_decrement();
        }
        else
        {
          message_offset_min = "la valeur de l'offset_min n'est pas bonne";
        }
      }

      /*       if (request->hasArg("gamma"))
            {
              gamma_val = request->getParam("gamma")->value().toDouble();

              message_gamma.clear();
              Serial.println(gamma_val);
              if (0.1 <= gamma_val && gamma_val <= 3.0)
              {
                go_gamma();
                message_gamma += "la valeur de la correction gamma ";
                message_gamma += double(gamma_val);
              }
              else
              {
                message_gamma = "la valeur de la correction gamma n'est pas bonne";
              }
            }

            if (request->hasArg("stop"))
            {

              stop = request->getParam("stop")->value();
              message_stop.clear();
              Serial.println(stop);

              if (stop == NULL)
              {
                go_stop();
                message_stop = "la variation a ete stopper";
              }
              else
              {
                message_stop = "il ne faut pas mettre de valeur apres stop";
              }
            } */

      if (request->hasArg("state"))
      {

        state = request->getParam("state")->value();
        message_state.clear();
        Serial.println(state);

        if (state == NULL)
        {
          go_state();
        }
        else
        {
          message_state = "il ne faut pas mettre de valeur apres state";
        }
      }

      if (request->hasArg("delay"))
      {
        Delay = request->getParam("delay")->value().toDouble();

        message_delay.clear();
        if (0.004 <= Delay && Delay <= 1.0)
        {
          // go_Delay();
          message_delay += "la valeur du delay ";
          message_delay += String(Delay, 4);
        }
        else
        {
          message_delay = "la valeur du Delay n'est pas bonne";
        }
      }

      if (rip == true)
      {
        message_supp = "tout a etait supprimer";
        rip = false;
      }
      else
      {
        message_supp.clear();
      }

      save_canal = false;
      dataX.clear();
      dataT.clear();
      // envoie les messgaes en fonction des requete demander

      dataT["message_canal"] = message_canal;
      dataT["message_delay"] = message_delay;
      dataT["message_decrement"] = message_decrement;
      dataT["message_increment"] = message_increment;
      dataT["message_off"] = message_off;
      dataT["message_on"] = message_on;
      dataT["message_on_progress"] = message_on_progress;
      dataT["message_off_progress"] = message_off_progress;
      dataT["message_on_save"] = message_on_save;
      dataT["message_random"] = message_random;
      dataT["message_value_canal"] = message_valeur_canal;
      dataT["message_variation"] = message_var;
      dataT["offset_max"] = message_offset_max;
      dataT["offset_min"] = message_offset_min;
      dataX["canaux"] = dataT;
    }

    if (CMD == "\0")
    {
      dataX.clear();
      message_CMD.clear();
      message_CMD += "quel est le type de commande";
    }
    if (CMD == "groupe")
    {
      dataX.clear();
      dataT.clear();
      save_groupe = true;

      message_CMD += "commande les groupes";
      if (request->hasArg("groupe"))
      {
        groupe = request->getParam("groupe")->value().toInt();

        message_groupe.clear();
        if (1 <= groupe && groupe <= 32)
        {
          go_groupe();
          message_groupe += F("votre numero de groupe est ");
          message_groupe += groupe;
        }
        else
        {
          message_groupe = F("le numero groupe n'est pas bon");
        }
      }
      if (request->hasArg("lire_groupe"))
      {
        lire_groupe = request->getParam("lire_groupe")->value();

        message_lire_groupe.clear();
        if (lire_groupe == "\0")
        {

          message_lire_groupe += F("voici les canaux dans ce groupe: ");
          go_groupe();
          go_lire_groupe();
        }
        else
        {
          message_lire_groupe = F("ne pas mettre de valeur apres lire_groupe");
        }
      }
      if (request->hasArg("add_in_groupe"))
      {
        add_in_groupe = request->getParam("add_in_groupe")->value().toInt();

        message_add_in_groupe.clear();
        if (1 <= add_in_groupe && add_in_groupe <= 256)
        {

          go_add_in_groupe();
        }
        else
        {
          message_add_in_groupe = "le numero canal(groupe) n'est pas bon";
        }
      }
      if (request->hasArg("supp_in_groupe"))
      {
        supp_in_groupe = request->getParam("supp_in_groupe")->value().toInt();

        message_supp_in_groupe.clear();
        if (1 <= supp_in_groupe /* && supp_in_groupe <= 256 */)
        {
          /*           for (int i = 0; i < 32; i++)
                    {
                      if (!arraygroupe[i] == supp_in_groupe)
                      {
                        message_supp_in_groupe.clear();
                        message_supp_in_groupe += F("le numero de canal à supprimer n'existe pas ");
                      }
                    } */
          message_supp_in_groupe += F("le numero de canal supprimer est ");
          message_supp_in_groupe += supp_in_groupe;
          message_supp_in_groupe += F(" et il est supprimer du groupe ");
          message_supp_in_groupe += groupe;
          go_supp_in_groupe();
          go_save();
          // go_groupe();
        }
        else
        {
          message_supp_in_groupe = "le numero canal(groupe) n'est pas bon";
        }
      }

      if (request->hasArg("on_groupe"))
      {
        on_groupe = request->getParam("on_groupe")->value();

        message_on_groupe.clear();
        if (on_groupe == "\0")
        {
          go_on_groupe();
          message_on_groupe += F("les cannaux numéro ");
          for (int i = 0; i < 32; i++)
          {
            int true_value = 0;
            true_value = arraygroupe[i];
            if (true_value != 0)
            {
              message_on_groupe += int(arraygroupe[i]);
            }

            if (i == 32)
            {

              break;
            }

            message_on_groupe += F(",");
          }
          message_on_groupe += F(" sont allumées");
          go_save();
        }
        else
        {
          message_on_groupe = "il ne faut pas mettre de valeur a on_groupe";
        }
      }

      if (request->hasArg("off_groupe"))
      {
        off_groupe = request->getParam("off_groupe")->value();

        message_off_groupe.clear();
        if (off_groupe == "\0")
        {
          go_off_groupe();
          message_off_groupe += F("les cannaux numéro ");
          for (int i = 0; i < 32; i++)
          {
            int true_value = 0;
            true_value = arraygroupe[i];
            if (true_value != 0)
            {
              message_off_groupe += int(arraygroupe[i]);
            }

            if (i == 32)
            {

              break;
            }

            message_off_groupe += F(",");
          }
          message_off_groupe += F(" sont éteins");
          go_save();
        }
        else
        {
          message_off_groupe = "il ne faut pas mettre de valeur a on_groupe";
        }
      }

      if (request->hasArg("delay_groupe"))
      {
        Delay_groupe = request->getParam("delay_groupe")->value().toDouble();

        message_delay_groupe.clear();
        if (0.004 <= Delay_groupe && Delay_groupe <= 1.0)
        {
          // go_Delay();
          message_delay_groupe += "la valeur du delay pour le groupe ";
          message_delay_groupe += String(Delay_groupe, 4);
        }
        else
        {
          message_delay_groupe = "la valeur du Delay pour le groupe n'est pas bonne";
        }
      }

      if (request->hasArg("on_progress_groupe"))
      {

        on_progress_groupe = request->getParam("on_progress_groupe")->value().toDouble();
        Serial.println(on_progress_groupe);
        message_on_progress_groupe.clear();

        if ((0.004 <= on_progress_groupe) && (on_progress_groupe <= 1.0))
        {

          message_on_progress_groupe += F("allume progressivement avec un delay de ");
          message_on_progress_groupe += String(on_progress_groupe, 4);
          message_on_progress_groupe += F("s");
          message_on_progress_groupe += F(" entre chaque valeur du groupe ");
          message_on_progress_groupe += groupe;
          go_on_progresson_progress_groupe();
        }
        else
        {
          message_on_progress_groupe = "la valeur de on_progress n'est pas bonne";
        }
      }
      if (request->hasArg("off_progress_groupe"))
      {

        off_progress_groupe = request->getParam("off_progress_groupe")->value().toDouble();
        Serial.println(off_progress_groupe);
        message_off_progress_groupe.clear();

        if ((0.004 <= off_progress_groupe) && (off_progress_groupe <= 1.0))
        {
          /* code */

          message_off_progress_groupe += F("eteind progressivement avec un delay de ");
          message_off_progress_groupe += String(off_progress_groupe, 4);
          message_off_progress_groupe += F("s");
          message_off_progress_groupe += F(" entre chaque valeur du groupe ");
          message_off_progress_groupe += groupe;
          go_off_progresson_progress_groupe();
        }
        else
        {
          message_off_progress_groupe = "la valeur de off_progress n'est pas bonne";
        }
      }

      if (request->hasArg("increment_groupe"))
      {

        increment_groupe = request->getParam("increment_groupe")->value().toInt();
        message_increment_groupe.clear();
        int valeur_increment_groupev2 = map(increment_groupe, 0, 100, 0, 255);
        increment_groupe = valeur_increment_groupev2;
        Serial.println(increment_groupe);
        if (0 <= increment_groupe && increment_groupe <= 255)
        {
          increment_groupe_activer = true;
          message_increment_groupe += "incremente jusqu'a la valeur ";
          message_increment_groupe += increment_groupe;
          message_increment_groupe += " du groupe ";
          message_increment_groupe += groupe;
          go_increment_groupe();
        }
        else
        {
          message_increment_groupe = "la valeur d'increment n'est pas bonne";
        }
      }

      if (request->hasArg("decrement_groupe"))
      {

        decrement_groupe = request->getParam("decrement_groupe")->value().toInt();
        message_decrement_groupe.clear();
        int valeur_decrement_groupev2 = map(decrement_groupe, 0, 100, 0, 255);
        decrement_groupe = valeur_decrement_groupev2;

        Serial.println(decrement_groupe);

        if (0 <= decrement_groupe && decrement_groupe <= 255)
        {
          /* code */
          decrement_groupe_activer = true;
          message_decrement_groupe.clear();
          message_decrement_groupe += "decremente jusqu'a la valeur ";
          message_decrement_groupe += decrement_groupe;
          message_decrement_groupe += " du groupe ";
          message_decrement_groupe += groupe;
          go_decrement_groupe();
        }
        else
        {
          message_decrement_groupe = "la valeur de decrement n'est pas bonne";
        }
      }

      if (request->hasArg("valeur_canal_groupe"))
      {
        valeur_canal_groupe = request->getParam("valeur_canal_groupe")->value();
        Serial.println(valeur_canal_groupe);
        message_valeur_canal_groupe.clear();
        if (valeur_canal_groupe == NULL)
        {
          go_groupe();
          go_valeur_canal_groupe();
        }
        else
        {
          message_valeur_canal_groupe = F("il ne faut pas mettre de valeur a valeur_canal_groupe");
        }
      }

      if (request->hasArg("canal_value_groupe"))
      {
        canal_value_groupe = request->getParam("canal_value_groupe")->value().toInt();
        int canal_value_groupev2 = map(canal_value_groupe, 0, 100, 0, 255);
        canal_value_groupe = canal_value_groupev2;
        Serial.println(canal_value_groupe);
        message_canal_value_groupe.clear();
        if (0 <= canal_value_groupe && canal_value_groupe <= 255)
        {

          go_canal_value_groupe();
          // go_save();
        }
        else
        {
          message_canal_value_groupe = F("la valeur n'est pas bonne");
        }
      }
      if (request->hasArg("var_groupe"))
      {

        var_groupe = request->getParam("var_groupe")->value();
        message_var_groupe.clear();
        Serial.println(var_groupe);

        if (var_groupe == NULL)
        {
          go_variation_groupe();
        }
        else
        {
          message_var_groupe = "il ne faut pas mettre de valeur apres var_groupe";
        }
      }

      if (request->hasArg("dimmer"))
      {
        dimmer = request->getParam("dimmer")->value().toFloat();
        float dimmerv2 = dimmer / 100;
        dimmer = dimmerv2;
        Serial.println(dimmer);
        message_dimmer.clear();
        if (0.01 <= dimmer && dimmer <= 1)
        {

          go_dimmer();
          // go_save();
        }
        else
        {
          message_dimmer = F("la valeur n'est pas bonne");
        }
      }

      if (request->hasArg("offset_max_groupe"))
      {

        offset_max_groupe = request->getParam("offset_max_groupe")->value().toInt();
        message_offset_max_groupe.clear();
        Serial.println(offset_max_groupe);
        int offset_max_groupev2 = map(offset_max_groupe, 0, 100, 1, 255);
        offset_max_groupe = offset_max_groupev2;
        Serial.println(offset_max_groupe);
        if (1 <= offset_max_groupe && offset_max_groupe <= 255)
        {
          /* code */
          // decrement_activer=true;

          message_offset_max_groupe += "l'offset_max_groupe est de  ";
          message_offset_max_groupe += offset_max_groupe;
          message_offset_max_groupe += " au groupe ";
          message_offset_max += groupe;
          go_save();
          // go_decrement();
        }
        else
        {
          message_offset_max_groupe = "la valeur de l'offset_max_groupe n'est pas bonne";
        }
      }

      if (request->hasArg("offset_min_groupe"))
      {

        offset_min_groupe = request->getParam("offset_min_groupe")->value().toInt();
        message_offset_min_groupe.clear();
        Serial.println(offset_min_groupe);
        int offset_min_groupev2 = map(offset_min_groupe, 0, 100, 0, 254);
        offset_min_groupe = offset_min_groupev2;
        Serial.println(offset_min_groupe);
        if (0 <= offset_min_groupe && offset_min_groupe <= 254)
        {
          /* code */
          // decrement_activer=true;

          message_offset_min_groupe += "l'offset_min est de  ";
          message_offset_min_groupe += offset_min_groupe;
          message_offset_min_groupe += " au groupe ";
          message_offset_min_groupe += groupe;
          go_save();
          // go_decrement();
        }
        else
        {
          message_offset_min_groupe = "la valeur de l'offset_min_groupe n'est pas bonne";
        }
      }

      if (request->hasArg("nom_groupe1"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe1")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 1;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 1;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe1 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe2"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe2")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 2;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 2;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe2 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe3"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe3")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 3;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 3;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe3 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe4"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe4")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 4;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 4;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe4 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe5"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe5")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 5;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 5;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe5 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe6"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe6")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 6;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 6;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe6 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe7"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe7")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 7;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 7;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe7 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe8"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe8")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 8;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 8;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe8 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe9"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe9")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) &&
            (longeurmax <= 32))
        {
          nb_name = 9;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 9;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe9 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe10"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe10")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 10;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 10;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe10 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe11"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe11")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 11;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 11;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe11 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe12"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe12")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 12;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 12;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe12 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe13"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe13")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 13;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 13;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe13 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe14"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe14")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 14;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 14;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe14 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe15"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe15")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 15;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 15;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe15 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe16"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe16")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 16;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 16;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe16 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe17"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe17")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 17;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 17;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe17 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe18"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe18")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 18;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 18;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe18 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe19"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe19")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 19;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 19;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe19 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe20"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe20")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 20;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 20;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe20 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe21"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe21")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 21;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 21;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe21 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe22"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe22")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 22;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 22;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe22 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe23"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe23")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 23;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 23;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe23 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe24"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe24")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 24;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 24;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe24 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe25"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe25")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 25;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 25;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe25 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe26"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe26")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 26;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 26;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe26 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe27"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe27")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 27;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 27;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe27 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe28"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe28")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 28;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 28;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe28 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe29"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe29")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 29;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 29;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe29 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe30"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe30")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 30;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 30;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe30 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe31"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe31")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 31;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 31;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe31 "] = message_nom_groupe;
        }
      }
      if (request->hasArg("nom_groupe32"))
      {
        message_nom_groupe.clear();
        nom_groupe = request->getParam("nom_groupe32")->value().c_str();
        int longeurmax = nom_groupe.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 32;
          go_nom_groupe();
        }
        else if (nom_groupe == NULL)
        {
          nb_name = 32;
          go_give_nom_groupe();
        }
        else
        {
          message_nom_groupe = F("nom trop long");
          dataT["nom_groupe32 "] = message_nom_groupe;
        }
      }

      if (request->hasArg("nom_canal1"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal1")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 1;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 1;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal1 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal2"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal2")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 2;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 2;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal2 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal3"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal3")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 3;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 3;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal3 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal4"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal4")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 4;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 4;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal4 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal5"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal5")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 5;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 5;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal5 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal6"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal6")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 6;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 6;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal6 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal7"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal7")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 7;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 7;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal7 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal8"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal8")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 8;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 8;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal8 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal9"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal9")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 9;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 9;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal9 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal10"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal10")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 10;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 10;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal10 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal11"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal11")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 11;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 11;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal11 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal12"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal12")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 12;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 12;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal12 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal13"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal13")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 13;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 13;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal13 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal14"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal14")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 14;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 14;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal14 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal15"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal15")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 15;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 15;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal15 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal16"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal16")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 16;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 16;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal16 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal17"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal17")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 17;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 17;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal17 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal18"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal18")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 18;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 18;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal18 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal19"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal19")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 19;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 19;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal19 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal20"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal20")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 20;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 20;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal20 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal21"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal21")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 21;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 21;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal21 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal22"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal22")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 22;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 22;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal22 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal23"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal23")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 23;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 23;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal23 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal24"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal24")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 24;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 24;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal24 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal25"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal25")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 25;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 25;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal25 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal26"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal26")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 26;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 26;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal26 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal27"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal27")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 27;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 27;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal27 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal28"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal28")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 28;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 28;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal28 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal29"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal29")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 29;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 29;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal29 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal30"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal30")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 30;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 30;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal30 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal31"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal31")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 31;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 31;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal31 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal32"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal32")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 32;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 32;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal32 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal33"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal33")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 33;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 33;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal33 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal34"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal34")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 34;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 34;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal34 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal35"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal35")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 35;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 35;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal35 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal36"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal36")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 36;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 36;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal36 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal37"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal37")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 37;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 37;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal37 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal38"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal38")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 38;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 38;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal38 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal39"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal39")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 39;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 39;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal39 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal40"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal40")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 40;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 40;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal40 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal41"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal41")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 41;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 41;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal41 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal42"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal42")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 42;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 42;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal42 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal43"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal43")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 43;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 43;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal43 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal44"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal44")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 44;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 44;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal44 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal45"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal45")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 45;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 45;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal45 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal46"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal46")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 46;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 46;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal46 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal47"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal47")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 47;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 47;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal47 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal48"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal48")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 48;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 48;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal48 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal49"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal49")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 49;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 49;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal49 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal50"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal50")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 50;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 50;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal50 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal51"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal51")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 51;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 51;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal51 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal52"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal52")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 52;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 52;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal52 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal53"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal53")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 53;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 53;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal53 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal54"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal54")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 54;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 54;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal54 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal55"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal55")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 55;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 55;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal55 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal56"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal56")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 56;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 56;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal56 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal57"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal57")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 57;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 57;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal57 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal58"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal58")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 58;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 58;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal58 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal59"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal59")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 59;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 59;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal59 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal60"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal60")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 60;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 60;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal60 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal61"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal61")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 61;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 61;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal61 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal62"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal62")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 62;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 62;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal62 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal63"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal63")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 63;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 63;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal63 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal64"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal64")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 64;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 64;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal64 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal65"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal65")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 65;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 65;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal65 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal66"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal66")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 66;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 66;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal66 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal67"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal67")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 67;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 67;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal67 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal68"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal68")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 68;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 68;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal68 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal69"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal69")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 69;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 69;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal69 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal70"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal70")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 70;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 70;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal70 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal71"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal71")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 71;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 71;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal71 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal72"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal72")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 72;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 72;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal72 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal73"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal73")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 73;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 73;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal73 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal74"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal74")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 74;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 74;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal74 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal75"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal75")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 75;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 75;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal75 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal76"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal76")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 76;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 76;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal76 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal77"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal77")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 77;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 77;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal77 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal78"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal78")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 78;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 78;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal78 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal79"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal79")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 79;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 79;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal79 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal80"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal80")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 80;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 80;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal80 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal81"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal81")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 81;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 81;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal81 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal82"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal82")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 82;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 82;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal82 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal83"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal83")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 83;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 83;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal83 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal84"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal84")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 84;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 84;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal84 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal85"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal85")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 85;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 85;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal85 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal86"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal86")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 86;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 86;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal86 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal87"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal87")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 87;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 87;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal87 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal88"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal88")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 88;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 88;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal88 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal89"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal89")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 89;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 89;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal89 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal90"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal90")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 90;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 90;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal90 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal91"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal91")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 91;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 91;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal91 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal92"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal92")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 92;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 92;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal92 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal93"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal93")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 93;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 93;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal93 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal94"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal94")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 94;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 94;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal94 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal95"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal95")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 95;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 95;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal95 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal96"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal96")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 96;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 96;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal96 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal97"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal97")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 97;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 97;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal97 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal98"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal98")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 98;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 98;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal98 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal99"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal99")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 99;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 99;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal99 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal100"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal100")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 100;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 100;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal100 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal101"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal101")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 101;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 101;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal101 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal102"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal102")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 102;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 102;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal102 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal103"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal103")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 103;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 103;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal103 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal104"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal104")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 104;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 104;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal104 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal105"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal105")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 105;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 105;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal105 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal106"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal106")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 106;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 106;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal106 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal107"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal107")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 107;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 107;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal107 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal108"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal108")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 108;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 108;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal108 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal109"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal109")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 109;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 109;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal109 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal110"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal110")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 110;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 110;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal110 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal111"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal111")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 111;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 111;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal111 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal112"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal112")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 112;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 112;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal112 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal113"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal113")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 113;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 113;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal113 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal114"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal114")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 114;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 114;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal114 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal115"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal115")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 115;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 115;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal115 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal116"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal116")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 116;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 116;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal116 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal117"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal117")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 117;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 117;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal117 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal118"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal118")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 118;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 118;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal118 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal119"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal119")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 119;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 119;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal119 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal120"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal120")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 120;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 120;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal120 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal121"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal121")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 121;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 121;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal121 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal122"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal122")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 122;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 122;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal122 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal123"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal123")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 123;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 123;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal123 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal124"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal124")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 124;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 124;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal124 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal125"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal125")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 125;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 125;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal125 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal126"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal126")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 126;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 126;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal126 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal127"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal127")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 127;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 127;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal127 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal128"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal128")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 128;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 128;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal128 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal129"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal129")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 129;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 129;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal129 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal130"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal130")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 130;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 130;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal130 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal131"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal131")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 131;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 131;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal131 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal132"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal132")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 132;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 132;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal132 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal133"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal133")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 133;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 133;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal133 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal134"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal134")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 134;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 134;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal134 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal135"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal135")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 135;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 135;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal135 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal136"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal136")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 136;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 136;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal136 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal137"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal137")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 137;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 137;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal137 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal138"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal138")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 138;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 138;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal138 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal139"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal139")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 139;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 139;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal139 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal140"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal140")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 140;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 140;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal140 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal141"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal141")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 141;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 141;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal141 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal142"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal142")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 142;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 142;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal142 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal143"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal143")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 143;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 143;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal143 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal144"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal144")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 144;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 144;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal144 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal145"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal145")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 145;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 145;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal145 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal146"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal146")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 146;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 146;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal146 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal147"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal147")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 147;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 147;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal147 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal148"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal148")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 148;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 148;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal148 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal149"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal149")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 149;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 149;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal149 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal150"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal150")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 150;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 150;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal150 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal151"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal151")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 151;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 151;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal151 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal152"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal152")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 152;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 152;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal152 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal153"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal153")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 153;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 153;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal153 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal154"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal154")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 154;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 154;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal154 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal155"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal155")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 155;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 155;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal155 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal156"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal156")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 156;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 156;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal156 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal157"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal157")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 157;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 157;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal157 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal158"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal158")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 158;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 158;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal158 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal159"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal159")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 159;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 159;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal159 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal160"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal160")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 160;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 160;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal160 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal161"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal161")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 161;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 161;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal161 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal162"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal162")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 162;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 162;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal162 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal163"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal163")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 163;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 163;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal163 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal164"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal164")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 164;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 164;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal164 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal165"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal165")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 165;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 165;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal165 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal166"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal166")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 166;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 166;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal166 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal167"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal167")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 167;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 167;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal167 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal168"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal168")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 168;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 168;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal168 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal169"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal169")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 169;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 169;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal169 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal170"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal170")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 170;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 170;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal170 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal171"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal171")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 171;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 171;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal171 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal172"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal172")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 172;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 172;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal172 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal173"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal173")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 173;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 173;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal173 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal174"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal174")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 174;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 174;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal174 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal175"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal175")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 175;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 175;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal175 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal176"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal176")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 176;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 176;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal176 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal177"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal177")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 177;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 177;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal177 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal178"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal178")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 178;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 178;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal178 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal179"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal179")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 179;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 179;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal179 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal180"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal180")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 180;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 180;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal180 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal181"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal181")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 181;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 181;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal181 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal182"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal182")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 182;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 182;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal182 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal183"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal183")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 183;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 183;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal183 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal184"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal184")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 184;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 184;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal184 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal185"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal185")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 185;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 185;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal185 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal186"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal186")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 186;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 186;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal186 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal187"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal187")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 187;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 187;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal187 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal188"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal188")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 188;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 188;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal188 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal189"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal189")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 189;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 189;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal189 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal190"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal190")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 190;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 190;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal190 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal191"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal191")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 191;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 191;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal191 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal192"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal192")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 192;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 192;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal192 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal193"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal193")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 193;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 193;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal193 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal194"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal194")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 194;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 194;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal194 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal195"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal195")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 195;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 195;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal195 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal196"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal196")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 196;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 196;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal196 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal197"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal197")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 197;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 197;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal197 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal198"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal198")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 198;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 198;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal198 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal199"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal199")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 199;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 199;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal199 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal200"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal200")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 200;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 200;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal200 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal201"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal201")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 201;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 201;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal201 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal202"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal202")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 202;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 202;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal202 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal203"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal203")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 203;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 203;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal203 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal204"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal204")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 204;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 204;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal204 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal205"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal205")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 205;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 205;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal205 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal206"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal206")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 206;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 206;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal206 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal207"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal207")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 207;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 207;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal207 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal208"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal208")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 208;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 208;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal208 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal209"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal209")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 209;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 209;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal209 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal210"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal210")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 210;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 210;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal210 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal211"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal211")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 211;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 211;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal211 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal212"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal212")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 212;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 212;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal212 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal213"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal213")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 213;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 213;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal213 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal214"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal214")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 214;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 214;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal214 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal215"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal215")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 215;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 215;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal215 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal216"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal216")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 216;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 216;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal216 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal217"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal217")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 217;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 217;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal217 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal218"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal218")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 218;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 218;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal218 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal219"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal219")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 219;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 219;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal219 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal220"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal220")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 220;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 220;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal220 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal221"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal221")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 221;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 221;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal221 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal222"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal222")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 222;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 222;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal222 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal223"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal223")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 223;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 223;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal223 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal224"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal224")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 224;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 224;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal224 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal225"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal225")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 225;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 225;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal225 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal226"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal226")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 226;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 226;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal226 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal227"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal227")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 227;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 227;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal227 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal228"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal228")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 228;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 228;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal228 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal229"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal229")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 229;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 229;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal229 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal230"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal230")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 230;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 230;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal230 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal231"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal231")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 231;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 231;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal231 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal232"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal232")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 232;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 232;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal232 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal233"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal233")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 233;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 233;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal233 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal234"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal234")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 234;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 234;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal234 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal235"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal235")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 235;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 235;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal235 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal236"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal236")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 236;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 236;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal236 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal237"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal237")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 237;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 237;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal237 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal238"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal238")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 238;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 238;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal238 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal239"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal239")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 239;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 239;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal239 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal240"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal240")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 240;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 240;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal240 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal241"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal241")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 241;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 241;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal241 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal242"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal242")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 242;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 242;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal242 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal243"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal243")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 243;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 243;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal243 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal244"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal244")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 244;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 244;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal244 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal245"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal245")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 245;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 245;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal245 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal246"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal246")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 246;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 246;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal246 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal247"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal247")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 247;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 247;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal247 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal248"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal248")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 248;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 248;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal248 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal249"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal249")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 249;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 249;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal249 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal250"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal250")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 250;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 250;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal250 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal251"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal251")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 251;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 251;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal251 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal252"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal252")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 252;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 252;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal252 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal253"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal253")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 253;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 253;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal253 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal254"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal254")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 254;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 254;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal254 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal255"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal255")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 255;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 255;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal255 "] = message_nom_canal;
        }
      }
      if (request->hasArg("nom_canal256"))
      {
        message_nom_canal.clear();
        nom_canal = request->getParam("nom_canal256")->value().c_str();
        int longeurmax = nom_canal.length();
        if ((longeurmax >= 1) && (longeurmax <= 32))
        {
          nb_name = 256;
          go_nom_canal();
        }
        else if (nom_canal == NULL)
        {
          nb_name = 256;
          go_give_nom_canal();
        }
        else
        {
          message_nom_canal = F("nom trop long");
          dataT["nom_canal256 "] = message_nom_canal;
        }
      }

      save_groupe = false;

      // envoie les messages en fonction des requête demander
      dataT["message_dimmer"] = message_dimmer;
      dataT["message_on_groupe"] = message_on_groupe;
      dataT["message_off_groupe"] = message_off_groupe;
      dataT["message_delay_groupe"] = message_delay_groupe;
      dataT["message_decrement_groupe"] = message_decrement_groupe;
      dataT["message_increment_groupe"] = message_increment_groupe;
      dataT["message_on_progress_groupe"] = message_on_progress_groupe;
      dataT["message_off_progress_groupe"] = message_off_progress_groupe;
      dataT["message_canal_value_groupe"] = message_canal_value_groupe;
      dataT["message_valeur_canal_groupe"] = message_valeur_canal_groupe;
      dataT["message_var_groupe"] = message_var_groupe;
      dataT["message_offset_max_groupe"] = message_offset_max_groupe;
      dataT["message_offset_min_groupe"] = message_offset_min_groupe;
      dataT["message_supp_in_groupe"] = message_supp_in_groupe;
      dataT["message_add_in_groupe"] = message_add_in_groupe;
      dataT["message_lire_groupe"] = message_lire_groupe;
      dataT["message_groupe"] = message_groupe;
      dataX["groupe"] = dataT;
    }
    if (CMD == "ALL")
    {
      cmd_all = true;
      dataX.clear();
      dataT.clear();
      dataT["dimmer"] = "permet d'appliquer au groupe un offset de 0 a 100";
      dataT["on_groupe"] = "permet d’allumer tous les canaux d'un groupe";
      dataT["off_groupe"] = "permet d’éteindre tous les canaux d'un groupe";
      dataT["delay_groupe"] = "permet de définir un delay pour l'increment ou le decrement des groupes";
      dataT["decrement_groupe"] = "permet de decrementer les groupe";
      dataT["increment_groupe"] = "permet de d’incrémenter les groupe";
      dataT["on_progress_groupe"] = "permet de d’allumer progressivement les groupes";
      dataT["off_progress_groupe"] = "permet de d’éteindre progressivement les groupes";
      dataT["canal_value_groupe"] = "permet de d’ajouter une valeur au groupe";
      dataT["valeur_canal_groupe"] = "permet de de voir les valeur d'un groupe";
      dataT["var_groupe"] = "permet de faire varier les groupes";
      dataT["offset_max_groupe"] = "permet de d’jouter un offset max aux groupes";
      dataT["offset_min_groupe"] = "permet de d’jouter un offset min aux groupes";
      dataT["supp_in_groupe"] = "permet de de supprimer une valeur d'un groupe";
      dataT["add_in_groupe"] = "permet de d’jouter une valeur dans un groupe";
      dataT["lire_groupe"] = "permet de lire un groupe";
      dataT["CMD=groupe"] = "permet de commander les groupes";
      dataT["groupe"] = "permet de choisir le groupe";
      dataT["gamma"] = "permet de choisir la valeur gamma";
      dataT["stop"] = "permet de stop toutes les actions en cours (ex:les increments)";
      dataT["id"] = "permet de voir l'id de l'appareil";
      dataT["upload"] = "permet d'envoyer un ficher a l'esp";
      dataT["download"] = "permet de télécharger un ficher";
      dataT["restore"] = "permet de restaurer un ficher a l'esp(pas encore utiliser)";
      dataT["delete_all"] = "permet de supprimer toutes les données";
      dataT["save"] = "permet de sauvegarder des données";
      dataT["canal_value"] = "permet de connaître la valeur du canal";
      dataT["state"] = "permet de connaître l’état du canal";
      dataT["canal"] = "permet de choisir le canal";
      dataT["delay"] = "permet de changer le delay pour l'increment et le decrement du canal";
      dataT["decrement"] = "permet de decrementer le canal";
      dataT["increment"] = "permet d’incrémenter le canal";
      dataT["off"] = "permet d’éteindre le canal";
      dataT["on"] = "permet d'allumer le canal";
      dataT["on_progress"] = "permet d'allumer progressivement le canal";
      dataT["off_progress"] = "permet d’éteindre progressivement le canal";
      dataT["random"] = "permet d'allumer le canal a une valeur aléatoire";
      dataT["value_canal"] = "permet de changer la valeur du canal";
      dataT["variation"] = "permet de faire varier le canal";
      dataT["offset_max"] = "permet d'ajouter un offset_max aux canal";
      dataT["offset_min"] = "permet d'ajouter un offset_min aux canal";
      dataT["CMD=canaux"] = "permet de commander les canaux";
      dataX["toutes les commandes"] = dataT;
    }
  }

  if (request->hasArg("stop"))
  {

    stop = request->getParam("stop")->value();
    message_stop.clear();
    Serial.println(stop);

    if (stop == NULL)
    {
      go_stop();
      message_stop = "la variation a ete stopper";
    }
    else
    {
      message_stop = "il ne faut pas mettre de valeur apres stop";
    }
  }
  if (request->hasArg("delete_all"))
  {

    supp = request->getParam("delete_all")->value();
    message_supp.clear();
    Serial.println(supp);

    if (supp == NULL)
    {

      rip = true;
      allclear();
    }
    else
    {
      message_supp = "il ne faut pas mettre de valeur apres delete_all";
    }
  }

  if (request->hasArg("restore"))
  {

    restore = request->getParam("restore")->value();
    Serial.println(restore);
    message_restore.clear();
    if (restore == NULL)
    {

      // restore_activer=true;
      // Serial.println(restore_activer=true);
      message_restore = "restore les informations du fichier";
      go_restore();
    }
    else
    {
      message_restore = "il ne faut pas mettre de valeur apres restore";
    }
  }

  if (request->hasArg("download"))
  {

    download = request->getParam("download")->value();
    Serial.println(download);
    message_download.clear();
    if (download == NULL)
    { // download_activer=true;
      // Serial.println(download_activer=true);
      message_download = "download le fichier de configuration";
      go_download();
    }
    else
    {
      message_download = "il ne faut pas mettre de valeur apres download";
    }
  }

  if (request->hasArg("upload"))
  {

    upload = request->getParam("upload")->value();
    Serial.println(upload);
    message_upload.clear();
    if (upload == NULL)
    {

      // upload_activer=true;
      // Serial.println(upload_activer=true);
      message_upload = "upload le fichier de configuration";
      request->redirect("/upload");
      // go_upload();
    }
    else
    {
      message_upload = "il ne faut pas mettre de valeur apres upload";
    }
  }

  if (request->hasArg("save"))
  {

    save = request->getParam("save")->value();
    message_save.clear();
    Serial.println(save);

    if (save == NULL)
    {
      go_save();
    }
    else
    {
      message_save = "il ne faut pas mettre de valeur apres save";
    }
  }

  if (request->hasArg("id"))
  {
    id = request->getParam("id")->value();
    message_id.clear();
    Serial.println(id);

    if (id == NULL)
    {
      id = String((uint32_t)ESP.getEfuseMac(), HEX);
      message_id += id;
      dataX["message_id"] = message_id;
    }
    else
    {
      message_id = "appareil non reconnue";
    }
  }
  if (request->hasArg("gamma"))
  {
    gamma_val = request->getParam("gamma")->value().toDouble();

    message_gamma.clear();
    Serial.println(gamma_val);
    if (0.1 <= gamma_val && gamma_val <= 3.0)
    {
      go_gamma();
      message_gamma += "la valeur de la correction gamma ";
      message_gamma += double(gamma_val);
      go_save();
    }
    else
    {
      message_gamma = "la valeur de la correction gamma n'est pas bonne";
    }
  }
  if (request->hasArg("on_save"))
  {

    on_save = request->getParam("on_save")->value();
    message_on_save.clear();
    Serial.println(on_save);

    if (on_save == NULL)
    {
      on_save_activer = true;
      /* code */
      save_activer = true;
      message_on_save = "allume aux valeurs sauvegarder precedement";
      loadConfiguration("/config.json", config);
      printFile("/config.json");
      go_on_save();
    }
    else
    {
      message_on_save = "il ne faut pas mettre de valeur apres on_save";
    }
  }

  final.clear();
  if (ready_download == true)
  {

    name += filename2;
    name += "-";
    name += date;
    name += ".json";
    download_activer = true;
    CopiaPega(request, filename2, name);
  }
  ready_download = false;
  off_activer = false;
  on_activer = false;
  save_effectuer = false;
  // envoie les messages en fonction des requête demander

  if (cmd_all == false)
  {
    dataG["gamma"] = message_gamma;
    dataG["message_stop"] = message_stop;
    dataG["message_id"] = message_id;
    dataG["message_upload"] = message_upload;
    dataG["message_dl"] = message_download;
    dataG["message_restore"] = message_restore;
    dataG["message_del"] = message_supp;
    dataG["message_save"] = message_save;
    dataG["message_CMD"] = message_CMD;
    dataX["general"] = dataG;
    dataV["state_on"] = on_state;
    dataV["canal_value"] = canal_value;
    dataX["state"] = dataV;
  }

  serializeJsonPretty(dataX, final);
  if (!download_activer == true)
  {
    request->send(200, "application/json", final);
    download_activer = false;
  }
  else
  {
    download_activer = false;
  }

  Serial.println(F("finish de la requête"));
}
