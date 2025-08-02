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

or just use the `esp32-example.yaml` as proof of concept:

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
# If you use a esp8266 run the esp8266-example.yaml
esphome run esp32-example.yaml

```

## Example response all sensors enabled

```
[D][lumentree:031] System Status Register 0: 0x0000
[D][lumentree:031] System Status Register 2: 0x4C54  # Device Type Code
[D][lumentree:031] System Status Register 3-7: LT-INV  # Device Model Name (ASCII)
[D][lumentree:031] System Status Register 11: 5680  # Battery Voltage: 56.80V
[D][lumentree:031] System Status Register 12: 1250  # Battery Current: 12.50A
[D][lumentree:031] System Status Register 13: 2300  # AC Output Voltage: 230.0V
[D][lumentree:031] System Status Register 18: 1200  # AC Output Power: 1200W
[D][lumentree:031] System Status Register 20: 420   # PV Input Voltage: 420V
[D][lumentree:031] System Status Register 22: 800   # PV Input Power: 800W
[D][lumentree:031] System Status Register 24: 250   # Device Temperature: 25.0°C
[D][lumentree:031] System Status Register 50: 85    # Battery SOC: 85%
[D][lumentree:031] System Status Register 53: -500  # Grid Input Power: -500W (exporting)
[D][lumentree:031] System Status Register 67: 700   # Family Load Power: 700W
[D][lumentree:031] System Status Register 68: 1     # Operation Mode: 1 (Normal)
[D][lumentree:031] System Status Register 70: 8     # Grid Connection Status: 8 (Connected)
```

## Configuration

The component fetches data from registers 0-95 on connection, providing comprehensive system monitoring.

### Binary sensors

```yaml
binary_sensor:
  - platform: lumentree
    grid_connected:
      name: "${name} grid connected"
    battery_connected:
      name: "${name} battery connected"
```

### Sensors

```yaml
sensor:
  - platform: lumentree
    battery_voltage:
      name: "${name} battery voltage"
    battery_current:
      name: "${name} battery current"
    battery_power:
      name: "${name} battery power"
    battery_soc:
      name: "${name} battery state of charge"
    ac_voltage:
      name: "${name} ac voltage"
    ac_current:
      name: "${name} ac current"
    ac_power:
      name: "${name} ac power"
    pv_voltage:
      name: "${name} pv voltage"
    pv_power:
      name: "${name} pv power"
    grid_power:
      name: "${name} grid power"
    load_power:
      name: "${name} load power"
    device_temperature:
      name: "${name} device temperature"
    power_generation_today:
      name: "${name} power generation today"
    power_generation_total:
      name: "${name} power generation total"
```

### Text sensors

```yaml
text_sensor:
  - platform: lumentree
    device_model:
      name: "${name} device model"
    operation_mode:
      name: "${name} operation mode"
    grid_status:
      name: "${name} grid status"
```

## Protocol

The Lumentree BLE protocol uses standard MODBUS commands over BLE:

- **Service UUID**: `ffe0`
- **Write Characteristic**: `ffe1` 
- **Notify Characteristic**: `fd03`

Key register mappings:
- Registers 0-95: System status and real-time data
- Register 11: Battery voltage (V÷100)
- Register 12: Battery current (A÷100) 
- Register 18: AC output power (W)
- Register 20: PV input voltage (V)
- Register 22: PV input power (W)
- Register 50: Battery state of charge (%)
- Register 67: Load power consumption (W)
- Register 70: Grid connection status (≥7 = connected)

The component automatically reads the first 90 registers on connection and decodes the values according to the Lumentree protocol specification.

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
- https://github.com/philippoo66/py-lumentree

## Debugging

If this component doesn't work out of the box for your device please update your configuration to increase the log level to see details about the BLE traffic:

```yaml
logger:
  level: DEBUG
```

