#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

// RFID module pins
// LED pin
#define LED_PIN 7
#define SS_PIN 10     // RFID NSS (CS)
#define RST_PIN 5     // RFID Reset

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// LCD wiring (4-bit mode):
// RS -> pin 8, E -> pin 9, DB4 -> pin 2, DB5 -> pin 3, DB6 -> pin 4, DB7 -> pin 6
LiquidCrystal lcd(8, 9, 2, 3, 4, 6);


const byte expectedUID[4] = {0x24, 0xFC, 0xA5, 0xA7}; // ADD THE RFID OF YOUR CARD HERE

// Layout settings
const int stateCol = 0;       // Start column for state text (row 0)
const int buttonCol = 10;     // Start column for button (row 1)
const int buttonFullWidth = 6; // Button drawn as 6 full blocks

// Display the state on row 0 ("Activated" or "Deactivated")
void showState(const char* state) {
  lcd.setCursor(stateCol, 0);
  lcd.print(state);
  // Clear remaining characters on row 0 (16 columns total)
  int len = strlen(state);
  for (int i = len; i < 16; i++) {
    lcd.print(" ");
  }
}

// Draw the button on row 1 starting at buttonCol using 'width' full blocks.
// Draw the button on row 1 starting at buttonCol using 'width' full blocks.
// Now shrinks from left to right (earlier blocks disappear first)
void drawButton(int width) {
  // Clear button area (columns buttonCol to 15)
  for (int col = buttonCol; col < 16; col++) {
    lcd.setCursor(col, 1);
    lcd.print(" ");
  }
  
  // Start drawing from the right side
  int startCol = buttonCol + buttonFullWidth - width;
  lcd.setCursor(startCol, 1);
  for (int i = 0; i < width; i++) {
    lcd.write(byte(255));  // 0xFF is the full-block character
  }
}

// Animate the button press:
// - Start full (6 blocks)
// - Shrink to 3 blocks (fully pressed)
// - Then rebound to 4 blocks
void animatePress() {
  drawButton(buttonFullWidth);  // Show full button (6 blocks)
  delay(300);
  
  // Press down
  drawButton(5);
  delay(120);
  drawButton(4);
  delay(120);
  drawButton(3);  // Fully pressed state
  delay(600);     // Hold pressed state
  
  // Release
  drawButton(4);  // Rebound slightly
  delay(300);
}

// Animate deactivation with arrow interaction:
void animateDeactivation() {
  const char arrowStr[] = "---->"; // The arrow string (length 5)
  int arrowLength = 5;
  
  // Move arrow from off-screen to right before the button (FASTER)
  for (int pos = -arrowLength+1; pos <= buttonCol - arrowLength; pos++) {
    // Clear arrow area
    for (int col = 0; col < buttonCol; col++) {
      lcd.setCursor(col, 1);
      lcd.print(" ");
    }
    
    // Only print the visible part of the arrow
    if (pos < 0) {
      // Arrow is partially off screen
      int visibleLength = arrowLength + pos;
      lcd.setCursor(0, 1);
      lcd.print(&arrowStr[arrowLength - visibleLength]);
    } else {
      // Arrow is fully on screen
      lcd.setCursor(pos, 1);
      lcd.print(arrowStr);
    }
    delay(150); // FASTER approach (was 250)
  }
  
  // Arrow reaches button - press button and arrow moves with it
  // Change the state to "Deactivated" immediately when button is pressed
  showState("Deactivated");
  
  // Turn off LED immediately when button is pressed
  digitalWrite(LED_PIN, LOW);
  
  drawButton(5);  // Button presses one block in
  
  // Move arrow right one position as it "pushes" the button
  for (int col = 0; col < buttonCol; col++) {
    lcd.setCursor(col, 1);
    lcd.print(" ");
  }
  lcd.setCursor(buttonCol - arrowLength + 1, 1);
  lcd.print(arrowStr);
  delay(600); // KEEP SAME SPEED for button press
  
  // Release the button - button goes back to full and arrow moves back
  drawButton(buttonFullWidth);
  
  // Move arrow back to original position
  for (int col = 0; col < buttonCol; col++) {
    lcd.setCursor(col, 1);
    lcd.print(" ");
  }
  lcd.setCursor(buttonCol - arrowLength, 1);
  lcd.print(arrowStr);
  delay(400); // KEEP SAME SPEED for button release
  
  // Arrow moves back and disappears off screen (FASTER)
  for (int pos = buttonCol - arrowLength; pos >= -arrowLength; pos--) {
    // Clear arrow area
    for (int col = 0; col < buttonCol; col++) {
      lcd.setCursor(col, 1);
      lcd.print(" ");
    }
    
    // Only draw arrow if part of it is still visible
    if (pos < 0 && pos > -arrowLength) {
      // Arrow is partially off screen
      int visibleLength = arrowLength + pos;
      lcd.setCursor(0, 1);
      lcd.print(&arrowStr[arrowLength - visibleLength]);
    } else if (pos >= 0) {
      // Arrow is fully on screen
      lcd.setCursor(pos, 1);
      lcd.print(arrowStr);
    }
    delay(150); // FASTER retreat (was 250)
  }
}

// Compare the scanned RFID UID with the expected UID.
bool checkUID() {
  if (mfrc522.uid.size != 4) return false;
  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != expectedUID[i])
      return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.begin(16, 2);
  
  // Set up LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Initially off
  
  // Initially, show "Deactivated" state with full button.
  showState("Deactivated");
  drawButton(buttonFullWidth);
  
  Serial.println("Scan RFID card with UID 24 FC A5 A7");
}

void loop() {
  // Check for a new RFID card.
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    return;
    
  if (checkUID()) {
    // Correct card scanned: set state to "Activated" and animate button press.
    showState("Activated");
    
    // Turn on LED when activated
    digitalWrite(LED_PIN, HIGH);
    
    animatePress();
    delay(1500); // Wait a bit longer before arrow animation starts
    
    // Animation and deactivation
    animateDeactivation();
    // No need to turn off LED here as it's already done in the animateDeactivation function
  }
  
  mfrc522.PICC_HaltA();
  delay(500);
}
