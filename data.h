// Variables globales
int movimiento = 34;
int tiempoTotal = 0;
int tiempoRestante = 0;
float temperaturaMaxima = 25;
float temperaturaMinima = 14;
float tempC;
int DURACION;
int DISTANCIA;
String answer;
bool act = false; //Variable para el sensor de movimiento
char *motor = "off";
char *flauta = "off";
char *sistema = "off";
const int requestInterval = 1500;
const char *esp32Server = "http://192.168.200.1";
const char *RelayMotorOn = "http://192.168.200.53/RELAY=ON";
const char *RelayBombilloOn = "http://192.168.200.70/RELAY=ON";
const char *RelayMotorOff = "http://192.168.200.53/RELAY=OFF";
const char *RelayBombilloOff = "http://192.168.200.70/RELAY=OFF";
