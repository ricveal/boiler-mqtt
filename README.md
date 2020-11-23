# Boiler MQTT

In Europe, the boiler is usually controlled with a thermostat which can be
simplified as a switch or relay. This is a personal project to create an MQTT
controlled relay that could replace the operative part of a thermostat.

Caution! A full thermostat replacement requires additional components:

- A thermometer to check the temperature
- A logic unit to control the system, a.k.a switch on or off the relay according
  the desired temperature and the actual temperature of the thermometer.

Because of my personal requirements, I built this MQTT Switch using a Wemo D1 Mini
as main board with the relay shield which provides me an easy hardware
environment with an ESP8266 board and a relay, configurable with Arduino IDE.

Of course, you can adapt my hardware choices but notice that, if you do that,
software changes may apply as well.

The following steps will guide you to use my specific stack and I will consider
that the Arduino IDE is familiar to you.

This project uses the following libraries:

- ESP8266WiFi
- [PubSubClient](http://pubsubclient.knolleary.net)

The first one, is installed when you set up the Arduino IDE to work with ESP8266
boards, i.e Wemo boards. This is your first step:
[Prepare your IDE to work with Wemos](https://www.instructables.com/Wemos-ESP8266-Getting-Started-Guide-Wemos-101/)

This library will allow us to connect our board to a WiFi Network.

Then, you should install the PubSubClient library which will allow us to connect
and interact with an MQTT network which will be configured in the WiFi Network.

The scope of this project is not to configure an MQTT broker so if this is new
to you, I would recommend you to check before continuing some resources focused
on this topic like, for example,
[this](https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/)

Anyway, I strongly recommend you to configure your broker with authentication.
It's not difficult and adds minimal security to your network.

Once you have a working secured MQTT broker and your Arduino IDE ready to work
with Wemos, you can upload to your board the `boiler.ino` file.

Please, replace the following values with those according to your network and
configuration:

- CLIENT_ID: sets up the name the device will have on the MQTT network. It's
  value is totally up to you but remember, it should be unique.
- MQTT_USER and MQTT_PASSWORD: MQTT credentials
- ip: The IP the device will have on your network. It must be unique.
- gateway: The IP of the router.
- subnet: Your network mask. Probably 255.255.255.0
- ssid: The name of your WiFi network.
- password: your WiFi password.
- mqtt_server: the IP of the MQTT server.

Once you connect the board with your values, it will connect to your network and
MQTT broker and will start to send and receive MQTT messages.

Mainly, the client will publish on `stat/<CLIENT_ID>/POWER` the status of the
switch (ON / OFF) and will subscribe to `cmnd/<CLIENT_ID>/POWER`, listening new
messages and changing its state. The payload waited is ON / OFF too.

If you are using Mosquitto, you can test the system subscribing to the stat
topic and publish messages on `cmnd/<CLIENT_ID>/POWER` and the switch should
respond you according to your requests.
