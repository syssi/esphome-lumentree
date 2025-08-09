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
TBD.
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

## Debugging

If this component doesn't work out of the box for your device please update your configuration to increase the log level to see details about the BLE traffic:

```yaml
logger:
  level: DEBUG
```

