
// librerias que se neceitan instalar; ESPAsyncWebServer, Adafruit_Sensor, DHT sensor

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Reemplace con sus credenciales de red
const char* ssid = "ocorp";
const char* password = "koh2020.";

#define DHTPIN 2     // Pin digital conectado al sensor DHT

// Descomente el tipo de sensor que esta usando:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);

String readDHTTemperature() {
  // Las lecturas del sensor también pueden tener hasta 2 segundos de antigüedad (es un sensor muy lento)
  // Leer la temperatura como Celsius (por defecto)
  float t = dht.readTemperature();
  // Leer la temperatura como Fahrenheit (isFahrenheit = true)
  // flotar t = dht.readTemperature (verdadero);
  // Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if (isnan(t)) {    
    Serial.println("¡No se pudo leer el sensor DHT11 !!!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
// Las lecturas del sensor también pueden tener hasta 2 segundos de antigüedad (es un sensor muy lento)  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("¡No se pudo leer el sensor DHT11 !!!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Servidor en ESP32 con sensor DHT11 </h2>
  <span class="dht-labels">Actualiza cada 2 segundos</span> 
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperatura</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humedad</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 2000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 2000 ) ;
</script>
</html>)rawliteral";

// Reemplaza el marcador de posición con valores DHT
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  // Puerto serial para fines de depuración
  Serial.begin(115200);

  dht.begin();
  
  // Conectarse a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi ...");
  }

  // Imprimir la dirección IP local de ESP32
  Serial.println(WiFi.localIP());

  // Ruta para la raíz / página web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  // Iniciar servidor
  server.begin();
}
 
void loop(){
  
}
