/*-------------CONFIG--------------------*/
const char*  ssid      = "ESP32-AP";
const char*  password  = "25041999";
const IPAddress ip(192, 168, 200, 53); // Dirección IP estática para esta placa
//const IPAddress ip(192, 168, 200, 70); // Dirección IP estática para esta placa
const IPAddress gateway(192, 168, 200, 1); // Dirección IP del gateway
const IPAddress subnet(255, 255, 255, 0); // Máscara de subred
const String Relay_Password = "26962904";
const int    Relay_PIN      = 0;
#define RELAY 0 
String Pagina = R"====(<form method='POST' action='/actualizar' enctype='multipart/form-data'><input type='file' name='update'><br><input type='submit' value='actualizar'></form>)====";
WiFiUDP udp; // Crear una instancia de WiFiUDP
