# Appliances Backend

The aim of this project is to produce a (non-user-visible) backend
service that transparently connects different ("smart") household
appliances to existing frontend services.

This sounds like something every single home automation system
does. And that's true. The downside: Every system I tried to far was
either lacking support for my appliances, adapters were out of date,
limited in their funtionality, were (at least partially) incompatible
with newer versions of the home automation systems, and were in
general almost never well maintained. I tried to tie into existing
systems and adapters, but with most of the being written in NodeJS,
Java, or some obscure description language, I lost interest and
started rolling my own in C++.

I'm working on more features in whatever spare time I can invest into
this. Priority goes to devices I own and see practical value in
implementing. If you have suggestions, questions, ideas - let me know!

Code documentation follows when I have time, as well as automated
build and unit tests as well as packaging and releases. Right now its
checking out, building, running yourself if you want to try it.


## Getting it, Running it

On a Ubuntu server of your choice, install the prerequisite packages:
```bash
sudo apt-get install cmake g++ libmosquitto-dev libmodbus-dev nlohmann-json-dev libconfig++-dev
```

Get the source:
```bash
git clone https://github.com/HoldersOfStakes/appliances_backend.git
```

Build it:
```bash
mkdir build
cd build
cmake ..
make
```

Run it (possibly in `screen`, `byobu`, or similar in order to keep it alive when you close your SSH session):
```
./../bin/appliances_backend-bin
```


## Supported Appliances

The following appliances are currently (at least partially) supported:

 * Helios KWL 200/300 (via Modbus)
   * Variables: `fan_stage`, `temperature_supply_air`, `temperature_extract_air`


## Supported Interfaces

The following interfaces are currently (at least partially) supported:

 * homebridge-mqtt
   * Service types: `Fan`, `TemperatureSensor`


## To Do

 * Add configuration file support


## Known Issues

 * The homebridge<->helios mapping is fixed at the moment (will be removed with config file support)
 * The helios IP address is fixed at the moment (will be removed with config file support)
