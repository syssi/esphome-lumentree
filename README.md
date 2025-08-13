# esphome-lumentree

![GitHub actions](https://github.com/syssi/esphome-lumentree/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-lumentree)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-lumentree)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-lumentree)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor a Lumentree Solar Inverter via BLE

## Supported devices

* Lumentree SUNT-6.0kW-H Hybrid
* Lumentree Solar Inverters with BLE connectivity

## Requirements

* [ESPHome 2024.6.0 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32 board

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-lumentree@main
```

or just use the `esp32-ble-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-lumentree.git
cd esphome-lumentree

# Create a secrets.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

# Optional
mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
esphome run esp32-ble-example.yaml

```

## Example response all sensors enabled

```
[lumentree_ble:150]: Register data frame received
[lumentree_ble:151]:   01.03.BE.03.00.11.16.01.02.50.32.34.31.30.31.39.30.37.30.00.05.12.42.20.11.15.18.FF.C4.09.92.00.00.09.92.13.60.13.60.01.A9.00.00.00.FE.00.3C.00.A5.00.00.05.6A.06.40.00.00.55.AA.00.00.00.00.00.00.00.00.00.00.00.8C.00.02.00.00.00.00.00.01.00.00.00.00.00.00.00.5F.00.21.00.00.00.00.00.00.00.00.00.46.00.00.00.00.00.63.00.01.00.00.00.14.00.C8.00.00.00.00.00.F0.00.00.00.0A.00.01.FF.DC.00.A3.01.AF.00.49.00.00.00.00.00.00.00.01.00.00.00.00.00.00.01.B1.00.3C.01.18.00.00.00.00.00.00.0
[lumentree_ble:262]: Received Read Holding Registers response (0x03)
[lumentree_ble:270]: Decoding 190 register bytes for request type 0 (function 0x03)
[lumentree_ble:307]: Decoding System Status registers (0-94)
[lumentree_ble:312]: Serial Number: P241019070
[text_sensor:069]: 'lumentree-inverter serial number': Sending state 'P241019070'
[sensor:104]: 'lumentree-inverter device type': Sending state 768.00000  with 0 decimals of accuracy
[binary_sensor:026]: 'lumentree-inverter pv2 support': New state is OFF
[sensor:104]: 'lumentree-inverter device power rating code': Sending state 5.00000  with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter device power rating': Sending state 6000.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter battery voltage': Sending state 54.00000 V with 2 decimals of accuracy
[sensor:104]: 'lumentree-inverter battery current': Sending state -0.60000 A with 2 decimals of accuracy
[sensor:104]: 'lumentree-inverter ac output voltage': Sending state 245.00000 V with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter ac input voltage': Sending state 245.00000 V with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter ac output frequency': Sending state 49.60000 Hz with 2 decimals of accuracy
[sensor:104]: 'lumentree-inverter ac input frequency': Sending state 49.60000 Hz with 2 decimals of accuracy
[sensor:104]: 'lumentree-inverter ac power': Sending state 425.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter pv voltage': Sending state 254.00000 V with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter pv power': Sending state 165.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter device temperature': Sending state 38.60000 °C with 1 decimals of accuracy
[binary_sensor:026]: 'lumentree-inverter battery connected': New state is ON
[sensor:104]: 'lumentree-inverter battery status': Sending state 1.00000  with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter battery state of charge': Sending state 99.00000 % with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter grid power': Sending state 20.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter grid export': Sending state 200.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter ac output apparent power': Sending state 0.00000 VA with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter grid ct power': Sending state 10.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter battery power': Sending state -36.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter load power': Sending state 0.00000 W with 0 decimals of accuracy
[text_sensor:069]: 'lumentree-inverter operation mode': Sending state 'Hybrid Mode'
[select:015]: 'lumentree-inverter operation mode': Sending state Hybrid Mode (index 1)
[binary_sensor:026]: 'lumentree-inverter grid connected': New state is OFF
[sensor:104]: 'lumentree-inverter grid connection status': Sending state 0.00000  with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter pv2 voltage': Sending state 433.00000 V with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter pv2 power': Sending state 280.00000 W with 0 decimals of accuracy
[sensor:104]: 'lumentree-inverter device type image': Sending state 1.00000  with 0 decimals of accuracy
[text_sensor:069]: 'lumentree-inverter device model': Sending state 'Unknown (Type=768, Power=5, Engine=0)'

[lumentree_ble:150]: Register data frame received
[lumentree_ble:151]:   01.03.72.16.44.15.E0.15.E0.01.40.00.00.00.64.00.64.00.00.00.00.00.00.00.14.00.14.00.00.00.00.00.00.00.00.14.B4.14.50.00.00.00.00.00.00.00.00.00.00.00.00.00.01.00.00.00.00.00.00.00.00.01.67.00.01.00.00.00.01.00.00.00.01.00.00.00.00.08.FC.09.37.00.00.05.32.00.00.00.3C.00.14.00.38.00.34.00.00.00.5A.00.3C.00.01.00.00.00.00.15.E0.13.24.15.E0.14.50.00.64.EA.A4 (119)
[lumentree_ble:262]: Received Read Holding Registers response (0x03)
[lumentree_ble:270]: Decoding 114 register bytes for request type 1 (function 0x03)
[lumentree_ble:428]: Decoding Battery Configuration registers (101-157)
[lumentree_ble:438]: Equalization Voltage: 57.00 V
[lumentree_ble:441]: Charging Target Voltage: 56.00 V
[lumentree_ble:444]: Float Charge Voltage: 56.00 V
[lumentree_ble:447]: Battery Capacity: 320 Ah
[lumentree_ble:450]: Low Voltage Cutoff: 0.00 V
[lumentree_ble:453]: Battery Voltage Restore: 0.00 V
[lumentree_ble:456]: Target Voltage 5: 53.00 V
[lumentree_ble:459]: Target Voltage 6: 52.00 V
[lumentree_ble:462]: Charge from AC: Disabled
[switch:055]: 'lumentree-inverter ac charging': Sending state OFF
[lumentree_ble:467]: AC Output Frequency: 50Hz
[lumentree_ble:470]: AC Output Voltage Setting: 1 V
[lumentree_ble:473]: Maximum Current Setting 1: 60 A
[lumentree_ble:476]: Maximum Current Setting 2: 20 A
[lumentree_ble:479]: Maximum Current Setting 3: 56 A
[lumentree_ble:482]: Maximum Current Setting 4: 52 A
[lumentree_ble:485]: Equalized Charge Interval: 90 days
[lumentree_ble:488]: Balanced Charging Duration: 60 minutes
[lumentree_ble:491]: Work Mode: 1
[lumentree_ble:494]: Starter Generator: Disabled
[lumentree_ble:497]: Target Voltage 1: 56.00 V
[lumentree_ble:500]: Target Voltage 2: 49.00 V
[lumentree_ble:503]: Target Voltage 3: 56.00 V
[lumentree_ble:506]: Target Voltage 4: 52.00 V
[lumentree_ble:509]: Maximum Discharge Current: 100 A
[lumentree_ble:629]: Requesting System Control registers (160-180)

[lumentree_ble:150]: Register data frame received
[lumentree_ble:151]:   01.03.2A.19.08.0D.0C.11.0C.00.00.00.00.00.01.00.01.00.00.00.01.00.00.00.00.00.00.00.00.05.32.06.A4.06.A4.09.37.00.35.00.34.07.D0.17.6F.1A.0C (47)
[lumentree_ble:262]: Received Read Holding Registers response (0x03)
[lumentree_ble:270]: Decoding 42 register bytes for request type 2 (function 0x03)
[lumentree_ble:525]: Decoding System Control registers (160-180)
[lumentree_ble:535]: Real Time Clock: 6408 (Unix timestamp)
[lumentree_ble:538]: Sleep Mode: Disabled
[lumentree_ble:541]: Overload Auto Start: Enabled
[lumentree_ble:544]: Overtemperature Protection Auto: Enabled
[lumentree_ble:547]: Beep: Silent
[lumentree_ble:550]: Backlight: 1
[lumentree_ble:553]: Online Mode: Offline
[switch:055]: 'lumentree-inverter output': Sending state OFF
[lumentree_ble:558]: Maximum Current 5: 53 A
[lumentree_ble:561]: Maximum Current 6: 52 A

[lumentree_ble:150]: Register data frame received
[lumentree_ble:151]:   01.04.10.00.8C.00.46.00.02.00.00.00.5F.00.21.00.00.00.00.36.F5 (21)
[lumentree_ble:264]: Received Read Input Registers response (0x04)
[lumentree_ble:270]: Decoding 16 register bytes for request type 3 (function 0x04)
[lumentree_ble:698]: Decoding Daily Statistics registers (0-7)
[sensor:104]: 'lumentree-inverter today pv production': Sending state 14.00000 kWh with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter today essential load': Sending state 7.00000 kWh with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter today grid consumption': Sending state 0.20000 kWh with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter today total consumption': Sending state 0.00000 kWh with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter today battery charging': Sending state 9.50000 kWh with 1 decimals of accuracy
[sensor:104]: 'lumentree-inverter today battery discharge': Sending state 3.30000 kWh with 1 decimals of accuracy
```

## Troubleshooting

### Connection issues

- Ensure your ESP32 device is within BLE range (typically 10-100m)
- Only one device can connect to the inverter at a time
- Check that the inverter's BLE is enabled

### Missing sensor data

- Some registers may return 0 or invalid values depending on inverter state
- Grid-tied values are only meaningful when grid is connected
- PV values are only meaningful during daylight hours

## Known limitations

- Single connection only - the inverter accepts only one BLE connection at a time
- Limited to ESP32 devices (BLE not available on ESP8266)
- Protocol reverse-engineered and may be incomplete for all inverter variants

## References

- [Lumentree BLE Protocol Specification](https://github.com/syssi/esphome-lumentree/blob/main/docs/protocol-design.md)
- https://github.com/MacGH23/lumentree-rs232-lib
