# Lumentree BLE Protocol Implementation

## Overview

This document describes the implementation of the Lumentree Solar Inverter BLE protocol in ESPHome.

## BLE Service Configuration

- **Service UUID**: `0xFFE0`
- **Characteristic UUID**: `0xFFE1` (bidirectional - for sending commands and receiving responses)

## Protocol Implementation

### Command Structure

The implementation uses standard MODBUS RTU commands over BLE:

```
[Slave Address][Function Code][Start Register High][Start Register Low][Register Count High][Register Count Low][CRC Low][CRC High]
```

### Default Read Command

The component automatically sends a read command for registers 0-89 (90 registers total):

```
01 03 00 00 00 5A [CRC]
```

- `01`: Slave address (always 1)
- `03`: Function code (Read Holding Registers)
- `00 00`: Start register (0)
- `00 5A`: Register count (90 decimal = 0x5A hex)
- CRC: MODBUS CRC16 checksum

### Response Processing

1. **BLE Connection**: Connect to device with service UUID 0xFFE0
2. **Characteristic Discovery**: Find characteristic 0xFFE1
3. **Notification Subscription**: Subscribe to notifications on 0xFFE1
4. **Command Transmission**: Send read command via 0xFFE1
5. **Response Assembly**: Receive response packets via notifications on 0xFFE1
6. **CRC Validation**: Verify response integrity using MODBUS CRC16
7. **Data Extraction**: Parse register values from response

### Register Mapping

Key registers implemented in the component:

| Register | Name | Type | Scaling | Description |
|----------|------|------|---------|-------------|
| 2 | Device Type Code | Read | Direct | Device identification |
| 3-7 | Device Model Name | Read | ASCII | Device name (5 registers) |
| 11 | Battery Voltage | Read | ÷100 | Battery voltage in volts |
| 12 | Battery Current | Read | ÷100 | Battery current in amperes |
| 13 | AC Output Voltage | Read | ÷10 | AC output voltage |
| 18 | AC Output Power | Read | Direct | AC output power in watts |
| 20 | PV Input Voltage | Read | Direct | Solar panel voltage |
| 22 | PV Input Power | Read | Direct | Solar panel power |
| 24 | Device Temperature | Read | ÷10 | Internal temperature |
| 37 | Battery Status | Read | Code | Battery connection status |
| 50 | Battery SOC | Read | Direct | State of charge percentage |
| 53 | Grid Power | Read | Signed | Grid power (+ import, - export) |
| 61 | Battery Power | Read | Signed | Battery power (+ charge, - discharge) |
| 67 | Load Power | Read | Direct | Load consumption |
| 68 | Operation Mode | Read | Code | Operating mode (0=UPS, 1=Normal) |
| 70 | Grid Status | Read | Code | Grid connection (≥7 = connected) |

### Error Handling

1. **Connection Errors**: Automatic reconnection on disconnect
2. **CRC Validation**: Discard invalid responses
3. **Timeout Handling**: Request timeout and retry logic  
4. **Incomplete Responses**: Buffer management for partial packets

### Limitations

- **Single Connection**: Only one BLE client can connect at a time
- **ESP32 Only**: BLE not available on ESP8266 platforms
- **Range Limited**: Typical BLE range 10-100 meters
- **Read Only**: Current implementation is monitoring only

## Implementation Details

### C++ Component Structure

- `Lumentree`: Main component class inheriting from BLEClientNode and PollingComponent
- `gattc_event_handler()`: Handles BLE GATT events
- `send_read_command()`: Constructs and sends MODBUS commands
- `process_response()`: Assembles and validates responses
- `decode_register_data()`: Parses register values and publishes to sensors

### Python Configuration

- `__init__.py`: Component registration and configuration schema
- `sensor.py`: Sensor entity definitions
- `binary_sensor.py`: Binary sensor entity definitions  
- `text_sensor.py`: Text sensor entity definitions

### Update Cycle

1. Component starts with 30-second polling interval
2. On each update cycle, sends read command for registers 0-89
3. Processes response and updates all configured sensors
4. Logs detailed register values for debugging

## Usage

```yaml
esp32_ble_tracker:

ble_client:
  - mac_address: "AA:BB:CC:DD:EE:FF"
    id: client0

lumentree:
  - ble_client_id: client0
    id: inverter0
    update_interval: 30s

sensor:
  - platform: lumentree
    lumentree_id: inverter0
    battery_voltage:
      name: "Battery Voltage"
    ac_power:
      name: "AC Power"
    # ... other sensors
```

## Future Enhancements

1. **Write Commands**: Implementation of control functions
2. **Advanced Statistics**: Daily/monthly energy data
3. **Configuration Parameters**: Battery settings, grid parameters
4. **Error Logging**: Historical alarm and fault data
5. **Firmware Updates**: OTA update capability via BLE