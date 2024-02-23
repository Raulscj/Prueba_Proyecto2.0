/*-------------CONFIG--------------------*/
const char*  ssid      = "radscj";
const char*  password  = "26962904";
const String Relay_Password = "26962904";
const int    Relay_PIN      = 0;
#define RELAY 0 
String Pagina = R"====(<form method='POST' action='/actualizar' enctype='multipart/form-data'><input type='file' name='update'><br><input type='submit' value='actualizar'></form>)====";
WiFiUDP udp; // Crear una instancia de WiFiUDP
