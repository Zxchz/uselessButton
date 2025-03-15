# RFID Activated Useless Button

Have you ever seen the useless button? It is simple a box with a switch on top, that no matter how many times you flick it the other way, a robot finger comes out and flicks the switch back. We wanted to digitalize it


https://github.com/user-attachments/assets/0d6bbbc7-a22e-4a04-8851-632b309e0263



## Demo

Here is a demo of the complete project, you use an RFID tag to press the button, and then the arrow comes in and unpresses the button.

https://github.com/user-attachments/assets/adb7a521-a1ec-4838-a90c-4c247318c6cb



## Components Needed

- Arduino (Uno or similar)
- 16x2 LCD Display
- MFRC522 RFID Reader module
- LED (any color)
- 220-330Ω resistor (for the LED)
- 10 kΩ resistor
- Breadboard
- Jumper wires

## Circuit Diagram

[Insert circuit diagram/schematic here]

## Wiring Instructions

### RFID Module Connections
| RFID Pin | Arduino Pin |
|----------|-------------|
| SDA (NSS) | 10 |
| SCK | 13 |
| MOSI | 11 |
| MISO | 12 |
| GND | GND |
| RST | 5 |
| VCC | 3.3V |

### LCD Connections
| LCD Pin | Connection |
|---------|------------|
| 1 (VSS) | GND |
| 2 (VDD) | 5V |
| 3 (VO) | Potentiometer wiper (middle pin) |
| 4 (RS) | Arduino Pin 8 |
| 5 (R/W) | GND |
| 6 (E) | Arduino Pin 9 |
| 11 (DB4) | Arduino Pin 2 |
| 12 (DB5) | Arduino Pin 3 |
| 13 (DB6) | Arduino Pin 4 |
| 14 (DB7) | Arduino Pin 6 |
| 15 (A, Backlight +) | 5V |
| 16 (K, Backlight -) | GND |

### LED Connection
| Component | Connection |
|-----------|------------|
| LED Anode (longer leg) | Arduino Pin 7 |
| LED Cathode (shorter leg) | 220-330 ohm resistor → GND |

### Potentiometer Connections
| Potentiometer Pin | Connection |
|-------------------|------------|
| Left terminal | 5V |
| Middle terminal (wiper) | LCD pin 3 (VO) |
| Right terminal | GND |

## Component Details

### Resistors
- **LED Resistor (220-330 ohm)**: This resistor limits the current flowing through the LED to prevent damage. The exact value isn't critical within this range.
- **Resistor (10 kΩ)**: Used to adjust the LCD contrast.

## Software Installation

1. Install the Arduino IDE if you haven't already: [arduino.cc/en/software](https://arduino.cc/en/software)
2. Install the required libraries via the Arduino Library Manager:
   - MFRC522 (for the RFID reader)
   - LiquidCrystal (usually included with Arduino IDE)
3. Copy the provided code into a new Arduino sketch
4. Connect your Arduino to your computer via USB
5. Select the correct board and port in the Arduino IDE
6. Upload the code to your Arduino

## How It Works

1. The system starts in a "Deactivated" state with a full-width button displayed on the LCD.
2. When the specific RFID card (Write it in in line 18 of the code) is scanned:
   - The state changes to "Activated"
   - The LED turns on
   - The button animates a press
3. After a brief delay, an arrow animation appears:
   - The arrow moves from the left side of the screen
   - When it reaches the button, it "presses" it
   - The state immediately changes to "Deactivated" and the LED turns off
   - The arrow retreats back off the screen

## Customization

- To change the expected RFID card, update the `expectedUID` array in the code with your card's UID.
- You can modify animation speeds by changing the delay values.
- Different animations can be created by modifying the `animatePress()` and `animateDeactivation()` functions.

## Troubleshooting

- **LCD display is blank or shows only boxes**: Make sure everything is plugged into the correct places.
- **RFID not detecting cards**: Check connections, particularly RST and NSS pins.
- **LED doesn't light up**: Check LED polarity (longer leg should connect to pin 7) and resistor connection.
- **Slow/stuttering animations**: Verify all connections are secure and the Arduino is receiving adequate power.

## Future Improvements

- Add a buzzer for audio feedback
- Create a proper enclosure for the project
