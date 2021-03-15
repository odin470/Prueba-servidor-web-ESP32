// Ejemplo de boceto de prueba para varios sensores de humedad / temperatura DHT 
// REQUIERE las siguientes bibliotecas de Arduino:
// - Biblioteca de sensores DHT: https://github.com/adafruit/DHT-sensor-library
// - Biblioteca de sensores unificados de Adafruit: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Pin digital conectado al sensor DHT


// ¡Descomenta el tipo de sensor que estés usando
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Inicializar el sensor DHT.
// Tenga en cuenta que las versiones anteriores de esta biblioteca tomaron un tercer parámetro opcional para
// Ajusta los tiempos para procesadores más rápidos. Este parámetro ya no es necesario
// ya que el algoritmo de lectura DHT actual se ajusta para trabajar en procesos más rápidos.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 test! CONECTADO"));

  dht.begin();
}

void loop() {
  // Espere unos segundos entre mediciones.
  delay(2000);
  // ¡La lectura de la temperatura o la humedad tarda unos 250 milisegundos!
  // Las lecturas del sensor también pueden tener hasta 2 segundos de retrazo (es un sensor muy lento)
  float h = dht.readHumidity();
  // Leer la temperatura como Celsius (por defecto)
  float t = dht.readTemperature();
  // Leer la temperatura como Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("¡Error al leer del sensor DHT11 !"));
    return;
  }

  // Calcular el índice de calor en Fahrenheit (predeterminado)
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Índice de calor: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
