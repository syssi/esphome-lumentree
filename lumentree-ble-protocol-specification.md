# Lumentree BLE Protocol Specification

## Overview

The Lumentree Bluetooth Low Energy (BLE) protocol enables direct wireless communication with Lumentree solar inverters for local control and monitoring without requiring internet connectivity.

## BLE Service Configuration

### Primary Service
- **Service UUID**: `ffe0` (128-bit: `0000ffe0-0000-1000-8000-00805f9b34fb`)

### Characteristics

| Characteristic | UUID | Type | Purpose |
|----------------|------|------|---------|
| Data | `ffe1` | Write/Notify | Bidirectional communication - send commands and receive responses |

## Connection Parameters

- **Connection Timeout**: 10 seconds
- **Scan Period**: 12 seconds
- **Max Connections**: 1 (single connection only)
- **Device Name Pattern**: Typically contains "Lumentree" or device serial number

## Protocol Frame Structures

### 1. Read Command (Function Code 0x03)

#### Request Payload
| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Device address | `0x01` |
| 1 | Function Code | 1 Byte | Read Holding Registers | `0x03` |
| 2-3 | Register Address | 2 Bytes | Start register (Big Endian) | `0x0046` (Register 70) |
| 4-5 | Register Count | 2 Bytes | Number of registers to read (Big Endian) | `0x0001` (1 register) |
| 6-7 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

**Example**: Read AC Voltage (Register 70)
```
01 03 00 46 00 01 [CRC_LOW] [CRC_HIGH]
```

#### Response Payload
| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Echo of request | `0x01` |
| 1 | Function Code | 1 Byte | Echo of request | `0x03` |
| 2 | Byte Count | 1 Byte | Number of data bytes | `0x02` |
| 3-n | Data Bytes | Variable | Register values (Big Endian) | Register data |
| n+1-n+2 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

### 2. Write Command (Function Code 0x10)

#### Request Payload
| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Device address | `0x01` |
| 1 | Function Code | 1 Byte | Write Multiple Registers | `0x10` |
| 2-3 | Register Address | 2 Bytes | Start register (Big Endian) | `0x0028` (Register 40) |
| 4-5 | Register Count | 2 Bytes | Number of registers (Big Endian) | `0x0001` |
| 6 | Byte Count | 1 Byte | Number of data bytes | `0x02` |
| 7-8 | Data Bytes | 2 Bytes | Write value (Big Endian) | Value |
| 9-10 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

**Example**: Set Power (Register 40)
```
01 10 00 28 00 01 02 [VALUE_HIGH] [VALUE_LOW] [CRC_LOW] [CRC_HIGH]
```

#### Response Payload
| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Echo of request | `0x01` |
| 1 | Function Code | 1 Byte | Echo of request | `0x10` |
| 2-3 | Register Address | 2 Bytes | Echo of start address | `0x0028` |
| 4-5 | Register Count | 2 Bytes | Echo of register count | `0x0001` |
| 6-7 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

### 3. Statistics Read Command (Function Code 0x04)

#### Request Payload
| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0 | Slave Address | 1 Byte | Device address | `0x01` |
| 1 | Function Code | 1 Byte | Read Input Registers | `0x04` |
| 2-3 | Register Address | 2 Bytes | Start register (Big Endian) | `0x0050` (Register 80) |
| 4-5 | Register Count | 2 Bytes | Number of registers (Big Endian) | `0x0060` (96 registers) |
| 6-7 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

**Example**: Daily Statistics (Register 80, 96 values)
```
01 04 00 50 00 60 [CRC_LOW] [CRC_HIGH]
```

#### Response Payload
| Byte Position | Field | Length | Description | Special Note |
|---------------|-------|--------|-------------|--------------|
| 0 | Slave Address | 1 Byte | Echo of request | `0x01` |
| 1 | Function Code | 1 Byte | Echo of request | `0x04` |
| 2 | Byte Count | 1 Byte | Number of data bytes | Variable |
| 3-n | Data Bytes | Variable | 4-byte values (Big Endian) | 32-bit statistics values |
| n+1-n+2 | CRC16 | 2 Bytes | MODBUS CRC (Low, High) | Calculated |

### 4. Firmware Update Start Command

#### Request Payload
| Byte Position | Field | Length | Description | Example Value |
|---------------|-------|--------|-------------|---------------|
| 0-3 | Command Header | 4 Bytes | Update start command | `0x20100438` |
| 4-7 | Firmware CRC32 | 4 Bytes | CRC32 of firmware file | Calculated |
| 8 | Block Count | 1 Byte | Number of transfer blocks | `0x04` |
| 9-12 | Additional Parameters | 4 Bytes | Update-specific parameters | Variable |

### 5. WiFi Configuration (PeiWang)

#### Request Payload (String Format)
```
WIFI:[SSID]:[PASSWORD]
```

**Example**:
```
WIFI:MyNetwork:MyPassword123
```

#### Response Payload (String Format)
| Response | Description |
|----------|-------------|
| `OK` | WiFi configuration successful |
| `ERROR` | WiFi configuration failed |

## Register Map

### System Status Registers (0-95)

| Register | Name | Type | Description | Scaling/Units |
|----------|------|------|-------------|---------------|
| 0 | System Register Base | R | Base address register | - |
| 2 | Device Type Code | R | Device identification code | Hex value |
| 3-7 | Device Model Name | R | Device name (5 registers, ASCII hex) | ASCII text |
| 8 | Device Version/Type | R | Device classification identifier | Hex value |
| 11 | Battery Voltage | R | Real-time battery voltage | V ÷ 100 |
| 12 | Battery Current | R | Battery charge/discharge current | A ÷ 100 |
| 13 | AC Output Voltage | R | AC output voltage | V ÷ 10 |
| 15 | AC Input Voltage | R | Grid input voltage | V ÷ 10 |
| 16 | AC Output Frequency | R | Output frequency | Hz ÷ 100 |
| 17 | AC Input Frequency | R | Grid input frequency | Hz ÷ 100 |
| 18 | AC Output Power | R | Current AC output power | W (direct) |
| 20 | PV Input Voltage | R | Solar panel voltage | V (direct) |
| 22 | PV Input Power | R | Solar panel power | W (direct) |
| 24 | Device Temperature | R | Internal temperature | °C ÷ 10 |
| 37 | Battery Status | R | Battery connection status | Code (2=No Battery) |
| 40 | Power Setting | R/W | Inverter output power | W (direct) |
| 50 | Battery SOC | R | State of charge percentage | % (direct) |
| 53 | Grid Input Power | R | Grid power (bidirectional) | W (signed) |
| 54 | Grid Export Power | R | Power exported to grid | W (direct) |
| 58 | AC Output VA | R | AC output apparent power | VA (direct) |
| 59 | Grid CT Power | R | Current transformer measured power | W (direct) |
| 61 | Battery Power | R | Battery power (bidirectional) | W (signed) |
| 67 | Family Load Power | R | Load consumption | W (direct) |
| 68 | Operation Mode | R | Current operation mode | Code (0=UPS, 1=Normal) |
| 70 | Grid Connection Status | R | Grid availability status | Code (≥7=Connected) |
| 72 | PV Input Voltage 2 | R | Second PV input voltage | V (direct) |
| 74 | PV Input Power 2 | R | Second PV input power | W (direct) |
| 94 | Device Type Image | R | Device type for UI display | Code |

**Note**: Registers not listed (1, 9-10, 14, 19, 21, 23, 25-36, 38-39, 41-49, 51-52, 55-57, 60, 62-66, 69, 71, 73, 75-93, 95) may contain additional sensor data, status flags, timestamps, or reserved space. Complete mapping requires official documentation.

### Battery Configuration (100-157)

| Register | Name | Type | Description | Scaling/Values |
|----------|------|------|-------------|----------------|
| 101 | Equalization Voltage | R/W | Battery equalization voltage | V × 100 |
| 102 | Charging Target Voltage | R/W | Primary charge voltage target | V × 100 |
| 103 | Float Charge Voltage | R/W | Maintenance charge voltage | V × 100 |
| 104 | Battery Capacity | R/W | Battery ampere-hour capacity | Ah |
| 114 | Low Voltage Cutoff | R/W | Battery protection disconnect | V × 100 |
| 115 | Battery Voltage Restore | R/W | LVD recovery voltage | V × 100 |
| 117 | Target Voltage 5 | R/W | Additional voltage target 5 | V × 100 |
| 118 | Target Voltage 6 | R/W | Additional voltage target 6 | V × 100 |
| 120 | Charge from AC | R/W | AC charging enable | 0=Disable, 1=Enable |
| 123 | AC Output Frequency | R/W | Output frequency setting | 0=50Hz, 1=60Hz |
| 125 | AC Output Voltage | R/W | AC output voltage level | Voltage setting |
| 143 | Max Current Setting 1 | R/W | Current limit 1 | Amperes |
| 144 | Max Current Setting 2 | R/W | Current limit 2 | Amperes |
| 145 | Max Current Setting 3 | R/W | Current limit 3 | Amperes |
| 146 | Max Current Setting 4 | R/W | Current limit 4 | Amperes |
| 148 | Equalized Charge Interval | R/W | Days between equalization | Days (0-90) |
| 149 | Balanced Charging Duration | R/W | Equalization time | Minutes (0-90) |
| 150 | Work Mode | R/W | Inverter operating mode | Mode selection |
| 152 | Starter Generator | R/W | Generator control | 0=Disable, 1=Enable |
| 153 | Target Voltage 1 | R/W | Voltage target 1 | V × 100 |
| 154 | Target Voltage 2 | R/W | Voltage target 2 | V × 100 |
| 155 | Target Voltage 3 | R/W | Voltage target 3 | V × 100 |
| 156 | Target Voltage 4 | R/W | Voltage target 4 | V × 100 |
| 157 | Max Discharge Current | R/W | Maximum discharge limit | Amperes (80-120A) |

### System Control Registers (160-180)

| Register | Name | Type | Description | Values/Range |
|----------|------|------|-------------|--------------|
| 160 | Real Time Clock | R/W | System time setting | Unix timestamp format |
| 163 | Sleep Mode Set | R/W | Power saving mode | 0=Disable, 1=Enable |
| 165 | Overload Auto Start | R/W | Overload protection setting | 0=Disable, 1=Enable |
| 166 | Overtemp Protection Auto | R/W | Temperature protection | 0=Disable, 1=Enable |
| 167 | Beep | R/W | Audio notification control | 0=Silent, 1=Enable |
| 168 | Backlight | R/W | Display backlight control | Setting value |
| 170 | Online Mode | R/W | Network connectivity mode | 0=Offline, 1=Online |
| 177 | Max Current 5 | R/W | Additional current limit 5 | Amperes |
| 178 | Max Current 6 | R/W | Additional current limit 6 | Amperes |

### Lithium Battery Management System (185-197)

| Register | Name | Type | Description | Scaling/Values |
|----------|------|------|-------------|----------------|
| 185 | Lithium Battery Protocol | R | BMS communication protocol | Protocol identifier |
| 186 | BMS Charging Voltage | R | BMS recommended charge voltage | V × 100 |
| 187 | BMS Discharge Voltage | R | BMS cutoff voltage | V × 100 |
| 188 | BMS Charging Current Limit | R | BMS charge current limit | Amperes |
| 189 | BMS Discharge Current Limit | R | BMS discharge current limit | Amperes |
| 190 | Real-time Capacity | R | Current battery capacity | Percentage (%) |
| 191 | Real-time Voltage | R | Current battery voltage | V × 100 |
| 192 | Real-time Current | R | Current battery current | Amperes |
| 193 | Real-time Temperature | R | Battery temperature | °C × 10 |
| 196 | Lithium Battery Alarm Pos | R | BMS alarm position indicator | Position code |
| 197 | Lithium Battery Fault Loc | R | BMS fault location indicator | Location code |

### Security and Advanced Settings (201-203)

| Register | Name | Type | Description | Values/Range |
|----------|------|------|-------------|--------------|
| 201 | Password Setting 1 | R/W | Security access control 1 | Password value |
| 202 | Password Setting 2 | R/W | Security access control 2 | Password value |
| 203 | Password Setting 3 | R/W | Security access control 3 | Password value |

## Multi-Register Operations

### Typical Read Ranges

| Description | Start Register | Count | Purpose |
|-------------|----------------|-------|---------|
| Complete System Status | 0 | 95 | Full system overview |
| Battery Settings | 100 | 57 | Complete battery configuration |
| Device Settings | 2056 | 22 | Advanced device parameters |
| Daily Statistics | 80 | 96 | Daily power curve |
| Historical Data | 96 | 96 | Historical data |
| Error Records | 2040 | 80 | Alarm and error logs |

### Advanced Commands

#### Factory Reset
- **Command**: `01100C30000155550D0A`
- **Purpose**: Reset all settings to factory defaults

#### Time-based Control (Register 130)
- **Parameters**: 9 parameters for time-based charging
- **Activation**: Parameter 5 = 1 (enables time-based control)
- **End Time**: Parameter 9 = 2359 (23:59)

## CRC16 MODBUS Calculation

- **Algorithm**: CRC16 MODBUS
- **Polynomial**: 0xA001
- **Initialization**: 0xFFFF
- **Position**: At the end of each frame (Low Byte, High Byte)

## Data Types and Scaling

### Voltage Values
- **Scaling**: V × 100
- **Example**: 5680 = 56.80V

### Current Values
- **Unit**: Amperes (direct values)
- **Range**: Typically 0-120A

### Power Values
- **Unit**: Watts (direct values)

### Temperature Values
- **Scaling**: °C × 10
- **Example**: 250 = 25.0°C

### Capacity Values
- **Unit**: Percentage (%) or Ampere-hours (Ah)

## Error Handling

### Connection Errors
- Automatic reconnection attempts
- Timeout handling (10 seconds)

### Data Validation
- CRC16 checking on all frames
- Response completeness verification

### Error Response Codes
| Code | Description |
|------|-------------|
| 0x81 | Illegal Function |
| 0x82 | Illegal Data Address |
| 0x83 | Illegal Data Value |
| 0x84 | Slave Device Failure |

## Security Considerations

- **Range**: Physical proximity required (10-100m BLE range)
- **Authentication**: No apparent authentication in basic BLE
- **Pairing**: Device pairing may be required on first connection
- **Access Control**: Password registers (201-203) for enhanced security

## Implementation Notes

### BLE Packet Size Limitations
- **Standard Commands**: Up to 40 characters in one packet
- **Large Commands**: >40 characters require 20-byte packet segmentation
- **Firmware Updates**: 16-byte blocks for Bluetooth transfer

### Response Assembly
- Multiple BLE packets are concatenated into complete responses
- CRC validation determines response completeness
- Special handling for different response types (WiFi, firmware, etc.)

## Limitations

- **Single Connection**: Only one client can connect simultaneously
- **Range**: Limited by BLE signal strength
- **Proprietary Protocol**: Reverse-engineered, potentially incomplete
- **Hardware Dependent**: Specific implementation for Lumentree devices

## Common Use Cases

### Basic Monitoring
1. Read system status (registers 0-95)
2. Read battery voltage (register 191)
3. Read power output (register 40)

### Battery Configuration
1. Set charging voltage (register 102)
2. Set current limits (registers 143-146)
3. Configure protection settings (registers 114-115)

### Data Logging
1. Read daily statistics (function code 0x04, register 80)
2. Read historical data (function code 0x04, register 96)
3. Read error logs (function code 0x04, register 2040)

### System Control
1. Set work mode (register 150)
2. Configure AC output (registers 123, 125)
3. Enable/disable features (registers 120, 152, 163)

## Implementation Example Sequences

### Connect and Read Basic Status
1. Scan for BLE devices with service UUID `ffe0`
2. Connect to target device
3. Enable notifications on characteristic `ffe1`
4. Send read command: `01 03 00 00 00 5F [CRC]` (read registers 0-95)
5. Receive response via notifications on `ffe1`
6. Parse response data

### Set Battery Charging Voltage
1. Calculate voltage value: 56.80V = 5680
2. Convert to hex: 0x1630
3. Build command: `01 10 00 66 00 01 02 16 30 [CRC]`
4. Send via characteristic `ffe1`
5. Receive confirmation via characteristic `ffe1`

### Read Daily Power Statistics
1. Build statistics command: `01 04 00 50 00 60 [CRC]`
2. Send command
3. Receive multi-packet response
4. Concatenate packets until CRC validates
5. Parse 4-byte statistical values