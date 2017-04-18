/***********************************************************************************************************/
// El programa representa a un Bot que lee todos los mensajes publicados y se da a conocer cuando en alguno 
// de los mensajes aparecen las palabras clave: "arduino", "telegram" y "bot".
//
// Se precisan las siguientes librerías (buscarlas con Arduino IDE en Programa/Incluir libreria/Gestionar librerias):
// UniversalTelegramBot
// ArduinoJson
/***********************************************************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

/***********************************************************************************************************/

// Parametros de conexion al Punto de Acceso
char ssid[] = "xxxxxxxxxxxxxxxxxxxxxx";
char pass[] = "yyyyyyyy";

// Token del BOT de Telegram (crear bot con botfather)
#define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

/***********************************************************************************************************/

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; // Tiempo entre lecturas de mensajes 1s
long Bot_lasttime;
bool Start = false;

/***********************************************************************************************************/

// Funcion correspondiente a la lectura de los mensajes
void handleNewMessages(int numNewMessages)
{
    for(int i=0; i<numNewMessages; i++)
    {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        if ((StringHasWord(text, " arduino")) && (StringHasWord(text, " telegram")) && (StringHasWord(text, " bot ")))
        {
            bot.sendMessage(chat_id, "Hola humanos, soy un Bot localizado en un ESP8266. Me doy a conocer cuando alguien escribe en un mensaje las palabras claves: bot, telegram y arduino", "");
        }
    }
}

// Funcion para determinar si un String contiene una cierta palabra
uint8_t StringHasWord(String s, String w)
{
    int maxl = s.length() - w.length();
    int lgsearch = w.length();

    for (int i = 0; i <= maxl; i++)
    {
        if (s.substring(i, i + lgsearch) == w)
            return 1;
    }

    return 0;
}

/***********************************************************************************************************/

void setup()
{
    Serial.begin(115200);
    
    // Reinicia la interfaz de conexion en caso de que estubiera conectado a una red
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    
    // Conexion a la red
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    
    // Esperar la conexión
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Conectado a la red
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
}

void loop()
{
    // El tiempo para consultar si hay mensajes nuevos ha transcurrido
    if (millis() > Bot_lasttime + Bot_mtbs)
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while(numNewMessages)
        {
            // Mientras haya nuevos mensajes, se tratan en el manejador
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        Bot_lasttime = millis();
    }
}
