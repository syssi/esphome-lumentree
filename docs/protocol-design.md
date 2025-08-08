# Lumentree BLE Protocol Specification

## Overview

This document describes the Lumentree Solar Inverter BLE protocol specification. The protocol uses standard MODBUS RTU commands over Bluetooth Low Energy for comprehensive device monitoring and control.

## BLE Service Configuration

- **Service UUID**: `0xFFE0` (128-bit: `0000ffe0-0000-1000-8000-00805f9b34fb`)
- **Characteristic UUID**: `0xFFE1` (bidirectional - for sending commands and receiving responses)

### Connection Parameters
- **Connection Timeout**: 10 seconds
- **Scan Period**: 12 seconds
- **Max Connections**: 1 (single connection only)
- **Device Name Pattern**: Typically contains "Lumentree" or device serial number

## Protocol Implementation

### MODBUS Function Code Support

The device supports multiple MODBUS function codes for different data types:

#### Function 0x03 (Read Holding Registers) - Real-time Data
Live operational data for monitoring. Use this function for real-time sensor readings.

#### Function 0x04 (Read Input Registers) - Historical Data
Statistical and historical data storage for charts and energy statistics.

#### Function 0x06 (Write Single Register) - Configuration
Individual parameter changes for basic device settings.

#### Function 0x10 (Write Multiple Registers) - Batch Configuration
Complex settings and time synchronization. Required for advanced device configuration.

### Command Structure

Standard MODBUS RTU format over BLE:

```
[Slave Address][Function Code][Start Register High][Start Register Low][Register Count High][Register Count Low][CRC Low][CRC High]
```

#### Frame Structure Details

**Read Command (Function Code 0x03)**

| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Device address | `0x01` |
| 1 | Function Code | 1 Byte | Read Holding Registers | `0x03` |
| 2-3 | Register Address | 2 Bytes | Start register (Big Endian) | `0x0046` (Register 70) |
| 4-5 | Register Count | 2 Bytes | Number of registers to read (Big Endian) | `0x0001` (1 register) |
| 6-7 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

**Write Command (Function Code 0x10)**

| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Device address | `0x01` |
| 1 | Function Code | 1 Byte | Write Multiple Registers | `0x10` |
| 2-3 | Register Address | 2 Bytes | Start register (Big Endian) | `0x0028` (Register 40) |
| 4-5 | Register Count | 2 Bytes | Number of registers (Big Endian) | `0x0001` |
| 6 | Byte Count | 1 Byte | Number of data bytes | `0x02` |
| 7-8 | Data Bytes | 2 Bytes | Write value (Big Endian) | Value |
| 9-10 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

### Recommended Implementation Pattern

For comprehensive data collection, use a multi-request approach:

#### System Status Request (Primary)
```
01 03 00 00 00 5F [CRC]  // Read registers 0-94 (95 registers)
```

#### Battery Configuration Request
```
01 03 00 65 00 39 [CRC]  // Read registers 101-157 (57 registers)
```

#### System Control Request
```
01 03 00 A0 00 15 [CRC]  // Read registers 160-180 (21 registers)
```

**Command Format:**
- `01`: Slave address (always 1)
- `03`: Function code (Read Holding Registers) 
- `00 XX`: Start register address (16-bit big-endian)
- `00 XX`: Register count (16-bit big-endian)
- CRC: MODBUS CRC16 checksum (little-endian)

### Response Processing

1. **BLE Connection**: Connect to device with service UUID 0xFFE0
2. **Characteristic Discovery**: Find characteristic 0xFFE1
3. **Notification Subscription**: Subscribe to notifications on 0xFFE1
4. **Command Transmission**: Send read command via 0xFFE1
5. **Response Assembly**: Receive response packets via notifications on 0xFFE1
6. **CRC Validation**: Verify response integrity using MODBUS CRC16
7. **Data Extraction**: Parse register values from response

### Register Mapping

#### Device Parameter Registers (0-94) - Real-time Monitoring

| Register | Name | Type | Scaling | Description |
|----------|------|------|---------|-------------|
| 0-7 | Device ID/Serial Number | Read | Direct | Device identification string |
| 8 | Product Model | Read | Direct | Device model identifier |
| 11 | Battery Voltage | Read | ÷100 | Battery voltage in volts |
| 12 | Discharge Current | Read | ÷100 | Battery discharge current in amperes |
| 13 | Output Voltage | Read | ÷10 | AC output voltage |
| 15 | Input Voltage | Read | ÷10 | AC input voltage |
| 16 | Output Frequency | Read | ÷100 | AC output frequency in Hz |
| 17 | Input Frequency | Read | ÷100 | AC input frequency in Hz |
| 18 | Output Power | Read | Direct | AC output power in watts |
| 20 | PV Input Voltage | Read | Direct | Solar panel voltage |
| 22 | PV Input Power | Read | Direct | Solar panel power |
| 24 | IGBT Temperature | Read | ÷10 | Internal temperature in °C |
| 37 | Battery Status | Read | Direct | Battery connection status code |
| 50 | Battery SOC | Read | Direct | State of charge percentage |
| 53 | Input Power | Read | Signed | Grid input power in watts |
| 54 | Input Current | Read | Direct | AC input current in amperes |
| 58 | Output VA | Read | Direct | AC output apparent power in VA |
| 59 | Grid CT Power | Read | Direct | Grid CT power measurement in watts |
| 61 | Discharge Power | Read | Signed | Battery discharge power in watts |
| 67 | Family Load | Read | Direct | Total load consumption in watts |
| 68 | Work Mode | Read | Direct | Operating mode code |
| 70 | System Version | Read | Direct | Firmware version indicator |
| 72 | PV2 Input Voltage | Read | Direct | Second PV input voltage |
| 74 | PV2 Input Power | Read | Direct | Second PV input power |
| 94 | Device Type Image | Read | Direct | Device type for display |

#### Battery Configuration Registers (100-157) - Settings & Limits

| Register | Name | Type | Scaling | Description |
|----------|------|------|---------|-------------|
| 100 | Battery Type | Read/Write | Direct | Battery type selection (0=User, 1=AGM, 2=Lithium) |
| 101 | Equalizing Charge Voltage | Read/Write | ÷100 | Equalization voltage in volts |
| 102 | Boost Charge Voltage | Read/Write | ÷100 | Maximum charging voltage |
| 103 | Floating Charge Voltage | Read/Write | ÷100 | Float charge voltage |
| 104 | Battery Capacity | Read/Write | Direct | Battery capacity in Ah |
| 106 | Maximum Charge Current | Read/Write | Direct | Max charge current limit in amperes |
| 107 | Maximum Discharge Current | Read/Write | Direct | Max discharge current limit in amperes |
| 114 | Low Voltage Cutoff | Read/Write | ÷100 | Battery low voltage protection |
| 115 | Battery Recovery Voltage | Read/Write | ÷100 | Battery voltage recovery level |
| 120 | Charge From AC | Read/Write | Direct | AC charging enable (0=Disabled, 1=Enabled) |
| 148 | Equalized Charge Interval | Read/Write | Direct | Maintenance charge interval in days |
| 149 | Balanced Charging Duration | Read/Write | Direct | Balance charge duration in minutes |
| 157 | Max Discharge Current Limit | Read/Write | Direct | System discharge current limit |

#### System Control Registers (150-180) - Device Settings

| Register | Name | Type | Scaling | Description |
|----------|------|------|---------|-------------|
| 123 | AC Output Frequency Setting | Read/Write | Direct | Output frequency (0=50Hz, 1=60Hz) |
| 125 | AC Output Voltage | Read/Write | Direct | Output voltage level (0=220V, 1=230V, 2=240V) |
| 150 | Work Mode Setting | Read/Write | Direct | Operating mode (0=UPS, 1=Hybrid) |
| 160 | Real Time Setting | Read/Write | Time | System clock synchronization |
| 163 | Sleep Mode Setting | Read/Write | Direct | Power saving mode (0/1) |
| 165 | Overload Auto Restart | Read/Write | Direct | Overload protection auto-restart |
| 166 | Overtemp Auto Restart | Read/Write | Direct | Overtemperature protection auto-restart |
| 167 | Beep Control | Read/Write | Direct | Audio alert control |
| 168 | Backlight Control | Read/Write | Direct | Display backlight control |
| 170 | Connection Mode | Read/Write | Direct | Communication mode setting |

#### Lithium Battery Management System (185-197)

| Register | Name | Type | Scaling | Description |
|----------|------|------|---------|-------------|
| 185 | Lithium Battery Protocol | Read | Direct | BMS communication protocol identifier |
| 186 | BMS Charging Voltage | Read | ÷100 | BMS recommended charge voltage in volts |
| 187 | BMS Discharge Voltage | Read | ÷100 | BMS cutoff voltage in volts |
| 188 | BMS Charging Current Limit | Read | Direct | BMS charge current limit in amperes |
| 189 | BMS Discharge Current Limit | Read | Direct | BMS discharge current limit in amperes |
| 190 | Real-time Capacity | Read | Direct | Current battery capacity in percentage |
| 191 | Real-time Voltage | Read | ÷100 | Current battery voltage in volts |
| 192 | Real-time Current | Read | Direct | Current battery current in amperes |
| 193 | Real-time Temperature | Read | ÷10 | Battery temperature in °C |
| 196 | Lithium Battery Alarm Position | Read | Direct | BMS alarm position indicator |
| 197 | Lithium Battery Fault Location | Read | Direct | BMS fault location indicator |

#### Security and Advanced Settings (201-203)

| Register | Name | Type | Scaling | Description |
|----------|------|------|---------|-------------|
| 201 | Password Setting 1 | Read/Write | Direct | Security access control 1 |
| 202 | Password Setting 2 | Read/Write | Direct | Security access control 2 |
| 203 | Password Setting 3 | Read/Write | Direct | Security access control 3 |

#### Status Code Mappings

- **Battery Status (Register 37)**: 0=Error, 1=Connected, 2=No Battery
- **Work Mode (Register 68)**: 0=UPS Mode, 1=Hybrid Mode
- **Grid Status (Register 70)**: ≥7=Connected, <7=Disconnected

### CRC16 MODBUS Calculation

- **Algorithm**: CRC16 MODBUS
- **Polynomial**: 0xA001
- **Initialization**: 0xFFFF
- **Position**: At the end of each frame (Low Byte, High Byte)

### Data Types and Scaling

#### Voltage Values
- **Scaling**: V × 100 (stored as integer, divide by 100 for actual voltage)
- **Example**: 5680 = 56.80V

#### Current Values
- **Unit**: Amperes (direct values)
- **Range**: Typically 0-120A

#### Power Values
- **Unit**: Watts (direct values)
- **Signed Values**: Grid power and battery power can be negative (export/discharge)

#### Temperature Values
- **Scaling**: °C × 10 (stored as integer, divide by 10 for actual temperature)
- **Example**: 250 = 25.0°C

#### AC Voltage/Frequency
- **AC Voltage Scaling**: V × 10 (divide by 10 for actual voltage)
- **AC Frequency Scaling**: Hz × 100 (divide by 100 for actual frequency)

#### Capacity Values
- **Unit**: Percentage (%) or Ampere-hours (Ah) - direct values

### Error Handling

#### Connection Errors
- Automatic reconnection attempts
- Timeout handling (10 seconds)
- Single connection limit enforcement

#### Data Validation
- CRC16 checking on all frames
- Response completeness verification
- Packet assembly for fragmented responses

#### Error Response Codes
| Code | Description |
|------|-------------|
| 0x81 | Illegal Function |
| 0x82 | Illegal Data Address |
| 0x83 | Illegal Data Value |
| 0x84 | Slave Device Failure |

### Function Code Usage

#### Real-time vs Historical Data
- **Function 0x03**: Returns live operational data suitable for real-time monitoring
- **Function 0x04**: Contains historical/statistical data for energy analysis
- Register addresses may overlap but contain different data types
- Use Function 0x03 for sensor readings and real-time monitoring

### Multi-Register Operations

#### Typical Read Ranges

| Description | Start Register | Count | Purpose |
|-------------|----------------|-------|---------|
| Complete System Status | 0 | 95 | Full system overview |
| Battery Settings | 100 | 57 | Complete battery configuration |
| Historical Data | 80 | 96 | Daily power statistics |
| Error Records | 2040 | 80 | Alarm and error logs |

### Protocol Limitations

- **Single Connection**: Only one BLE client can connect at a time
- **BLE Range**: Typical Bluetooth Low Energy range 10-100 meters
- **Packet Size**: Standard commands up to 40 characters, larger commands require 20-byte segmentation
- **Write Operations**: Function 0x10 support required for full configuration capability
- **Register Coverage**: Complete implementation requires all register ranges
- **Security**: Physical proximity required, password registers available for access control

## Implementation Guidelines

### BLE Connection Management

1. Connect to device using service UUID `0xFFE0`
2. Discover characteristic `0xFFE1` for bidirectional communication
3. Subscribe to notifications on characteristic `0xFFE1`
4. Implement automatic reconnection on disconnect

### Data Collection Pattern

1. Use 30-second polling interval for optimal performance
2. Send three sequential read commands per cycle:
   - System Status: registers 0-94 (95 registers)
   - Battery Configuration: registers 101-157 (57 registers)
   - System Control: registers 160-180 (21 registers)
3. Process responses and validate CRC checksums
4. Parse register values according to scaling factors
5. Total of 173 registers per complete cycle

### Response Processing

1. Assemble fragmented responses from BLE notifications
2. Validate MODBUS CRC16 checksums
3. Apply scaling factors per register specification
4. Handle signed values where indicated
5. Implement timeout and retry logic for failed requests

## Implementation Extensions

### Essential Features
1. **Function 0x10 Support**: Write Multiple Registers for device configuration
   - Time synchronization (register 160)
   - System settings (registers 165-168)
   - Battery type selection (register 100)

2. **Complete Register Coverage**:
   - Advanced settings (registers 123, 125)
   - Full battery configuration (registers 106-107)
   - Time-based control (registers 130-184)

### Advanced Features
3. **Function 0x04 Support**: Historical data for energy statistics
   - Daily/monthly energy summaries
   - Time-series data visualization
   - Energy efficiency tracking

4. **Configuration Management**:
   - Battery type and capacity settings
   - Charge/discharge time periods
   - Power limit configurations

5. **System Features**:
   - Error logging and fault history
   - Multi-device support (master/slave)
   - Security and access control
