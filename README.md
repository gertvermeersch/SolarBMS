# SolarBMS

## Introduction

This project aims to provide me with a status LCD and basic BMS for my solar panel/battery/charger/inverter setup.
It shows Volts, current, watt, charging/consumption over the day. It also shows the status of the relay that enables the inverter and features a visual charge bar.
I'm aware that cutting the power with a relay isn't the best solution. I might add support for some power MOSFETs instead in the future.

## Hardware

- ESP12E
- a voltage divider consisting of a 10kOhm and 20kOhm resistor
- Shunt resistor
- Opamps for the shunt
- ST7920 128x64 display 
- ADS1015 Analog to digital converter
- 100A automotive Relay

## Software

- Arduino sketch
- Angular app (future)