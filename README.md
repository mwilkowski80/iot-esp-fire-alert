# iot-esp-fire-alert

This is an IoT component of the project available here https://github.com/mwilkowski80/fire-alert-server.

## How it is implemented?

This project is implemented using a NodeMCU v2 device, [Adafruit PIR sensor](https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/) and a small LED.

PIR sensor is connected to 3V, GND and D7. LED is connected to D6 (+) and G (-).

## Getting started

Wire Your circuit as explained above.

Git clone the https://github.com/mwilkowski80/iot-iotcommons project.

Adjust the link to the `iot-iotcommons` local project in `platformio.ini` file.

Create the `PrivateSettings.h` file from the `PrivateSettings.h.template`. 

Install the [platformio](https://platformio.org/) locally. I suggest:

```
python3 -m venv platformio
source platformio/bin/activate
pip install --upgrade pip setuptools wheel
pip install platformio
```

You may want to adjust the links to platformio in `bin` scripts if You want to use them to deploy the software to Your NodeMCU device.

Deploy and enjoy :).

## Known limitations

Various paths are heavily local (coupled to my local computer).
