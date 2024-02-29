// Variables globales
int movimiento = 34;
int tiempoTotal = 0;
int tiempoRestante = 0;
float temperaturaMaxima = 25;
float temperaturaMinima = 14;
float tempC;
int DURACION;
int DISTANCIA;
bool act = false; //Variable para el sensor de movimiento
char *motor = "off";
char *flauta = "off";
char *sistema = "off";
const char *esp32Server = "http://192.168.200.1:8080";
const char *RelayMotorOn = "http://192.168.200.53:8053/RELAY=ON";
const char *RelayBombilloOn = "http://192.168.200.70:8070/RELAY=ON";
const char *RelayMotorOff = "http://192.168.200.53:8053/RELAY=OFF";
const char *RelayBombilloOff = "http://192.168.200.70:8070/RELAY=OFF";
