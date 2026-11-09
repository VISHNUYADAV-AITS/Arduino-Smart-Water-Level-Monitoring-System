#include <LiquidCrystal.h>

// LCD pin configuration
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Ultrasonic sensor pins
const int trigPin = 5;
const int echoPin = 6;

// Water pump control pin
const int Motor_Pin = 7;

// Water level calibration (EXAMPLE values)
// Measure these distances on your actual tank.
const int EMPTY_DISTANCE_CM = 30;
const int FULL_DISTANCE_CM = 5;

// Variables
long duration;
int distance;
int Level;
bool Motor = false;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Motor_Pin, OUTPUT);

  // Initially keep pump OFF
  digitalWrite(Motor_Pin, LOW);

  // Initialize LCD
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Water Monitoring");
  delay(1500);
  lcd.clear();
}

void loop()
{
  // Generate ultrasonic trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Read echo with timeout
  duration = pulseIn(echoPin, HIGH, 30000UL);

  // Check sensor reading
  if (duration == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error   ");
    lcd.setCursor(0, 1);
    lcd.print("Check Sensor   ");
    delay(500);
    return;
  }

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  // Calculate water level percentage
  Level = map(distance,
               EMPTY_DISTANCE_CM,
               FULL_DISTANCE_CM,
               0, 100);

  // Restrict percentage between 0 and 100
  Level = constrain(Level, 0, 100);

  // Display water level
  lcd.setCursor(0, 0);
  lcd.print("Water: ");
  lcd.print(Level);
  lcd.print("%    ");

  // Pump ON below 20%
  if (Level < 20)
  {
    Motor = true;
  }

  // Pump OFF at or above 90%
  else if (Level >= 90)
  {
    Motor = false;
  }

  // Control pump
  digitalWrite(Motor_Pin, Motor ? HIGH : LOW);

  // Display pump status
  lcd.setCursor(0, 1);

  if (Motor)
  {
    lcd.print("Pump: ON       ");
  }
  else
  {
    lcd.print("Pump: OFF      ");
  }

  delay(500);
}
