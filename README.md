# LoRa

Repositorio de proyectos relacionados con LoRa

# WorkshopOSHWDem18

Documentación del taller "Introducción a las comunicaciones LoRa y LoRaWAN"

Incluye:

* La presentación del taller
* El proyecto de la BricoLabs ESP8266 LoRa Shield
  * Se partió del trabajo de Gustavo Reynaga y su [WeMos_Shield_BricoLabs](https://github.com/hulkco/LoRa/tree/master/WeMos_Shield_Bricolabs) y fué completamente modificado su pinado para adaptarse al taller
* Los ejemplos utilizados durante el taller
  * LoRa P2P
    * Basado en el ejemplo de la librería [arduino-LoRa](https://github.com/sandeepmistry/arduino-LoRa)
  * TTN Node
    * Basado en el ejemplo de la librería [arduino-lmic](https://github.com/matthijskooijman/arduino-lmic)
  * TTN Gateway with Sensors
    * Basado en el increible proyecto [ESP-1ch-Gateway-v5.0](https://github.com/things4u/ESP-1ch-Gateway-v5.0)

### Prerequisitos

El taller se realizó con la última versión disponible de Arduino en su momento 1.8.7

### Instalación
Los ejemplos son sketchs de arduino de otros autores modificados para funcionar con nuestra shield y el taller, incluyen las librerías utilizadas que compilan sin errores.

La BricoLabs ESP8266 LoRa Shield presenta un problema no detectado hasta su montaje. Los pines D3 y D4 son utilizados por el módulo LoRa y al estar conectados impiden en muchos casos tanto la programación como el boot del ESP8266. Para el taller se ha evitado la conexión entre estos pines de la shield y el ESP8266. Con esta modificación los ejemplos parecen funcionar sin mucho problema.

También para el taller se ha cortado el solder jumper JP1 quedando desconectado el dio2 del módulo LoRa y el pin D0 del ESP8266.

### Materiales taller

* BricoLabs ESP8266 LoRa Shield
  * Módulo LoRa RFM95W 868MHz: https://goo.gl/Lq5xBn o https://goo.gl/x9Ruqk
  * Conector antena SMA: https://goo.gl/uX1qVK
  * Antena SMA 868MHz https://goo.gl/3MmXzr
  * Condensador 1uF 0805: https://goo.gl/9gNtP3
  * Condensador 10uF 0805: https://goo.gl/FJRFJw
* ESP8266 WeMos D1 mini  https://goo.gl/JqPN6Y
* Shield Temperatura y humedad SHT30 https://goo.gl/2b3S51


## Contribuye!

Si ves algun error o quieres contribuir con algun ejemplo o modificación envíanos un pull request!

## Autores
Proyectos Originales:
* **Gustavo Reynaga** - *WeMos_Shield_BricoLabs* - [hulkco](https://github.com/hulkco)

* **Sandeep Mistry** - *arduino-LoRa* - [sandeepmistry](https://github.com/sandeepmistry)

* **Matthijs Kooijman** - *arduino-lmic* - [matthijskooijman](https://github.com/matthijskooijman)

* **Maarten Westenberg** - *ESP-1ch-Gateway-v5.0* - [things4u](https://github.com/things4u)

Modificaciones y ñapas:
* **Alejandro Taracido @TCRobotics** - * BricoLabs ESP8266 LoRa Shield, taller, y modificación de ejemplos * - [TCRobotics](https://github.com/TCRobotics)

Mira también la lista de [contribuidores](https://github.com/your/project/contributors) que participaron en el proyecto.

## Licencia

Cada ejemplo está licenciado según las licencias de los autores

## Grupo de Telegram
Después del taller se ha creado un grupo de Telegram para los interesados en TTN y LoRa de Coruña https://t.me/joinchat/AGoVpU_xfHY2CJOtXUelTg

## Agradecimientos

* A los autores de los proyectos utilizados para este taller, sin su trabajo, todo esto no sería posible.
* A TTN (The Things Network) y su comunidad, por su red y todo lo que he podido aprender.
* A BricoLabs por la organización de la OSHWDem y la ayuda en el taller y su aprovisionamiento de materiales.
* A Victor (Benchosei), Óscar (dev4migo) y Sergio (Salvari) por su ayuda con los problemas detectados el día anterior de taller xD
* A Victor (Benchosei), Óscar (dev4migo), Rafa (caligari), Santi Castro y Carlos Miranda por su ayuda durante la realización del taller.
* A Manuel Miramontes por su ayuda en el deploy de los gateways TTN de BricoLabs.
* A los asistentes del taller, por aguantarme y conseguir los objetivos del taller!
