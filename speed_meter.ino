#include <M5StickC.h>

int64_t circumference = 2096; // ADJUST: 700 x 23C tire typical value [mm]
int64_t circumference_hour = circumference * 60 * 60;
int64_t last_time;
volatile int speed;

hw_timer_t *tim0 = NULL;
enum {RUN, STOP};
volatile int state = STOP;

// Display speed
void dispSpeed(int s)
{
  if (speed > 999) return;  // Ignore over ranged speed
  int i = s / 10; // Integral part
  int d = s % 10; // Decimal part
  M5.Lcd.setCursor(0, 0, 7);
  M5.Lcd.printf("%02d", i);
  M5.Lcd.setCursor(75, 0, 6);
  M5.Lcd.printf("%d", d);
  M5.Lcd.setCursor(105, 0, 2);
  M5.Lcd.print("km/h");
}

// Monitor stop state
// If the status is stopped, clear the speed
void IRAM_ATTR checkStopState()
{
  if (state == STOP) {
    speed = 0;
  }
  state = STOP;
}

void setup() {
  M5.begin();

  // Set reed switch input
  // Pull-up and set interrupt
  // Reed switch is directly connected between G26 and GND
  // Reed switch has no polarity
  pinMode(GPIO_NUM_26, INPUT_PULLUP);
  attachInterrupt(GPIO_NUM_26, magnetPassed_Handler, FALLING);
  last_time = esp_timer_get_time();

  // Init LCD display
  M5.Lcd.setRotation(1);
  dispSpeed(0);

  // Timer setting
  // Monitor stop state
  tim0 = timerBegin(0, 80, true); // two second
  timerAttachInterrupt(tim0, &checkStopState, true);
  timerAlarmWrite(tim0, (1 * 1000000), true); // 1sec
  timerAlarmEnable(tim0);
 }

void magnetPassed_Handler()
{
  // Stop GPIO interrupt tentatively to avoid re-entrant
  // I'm not sure this code works well or not
  // There may be a better way to do the same thing
  detachInterrupt(GPIO_NUM_26);
  
  int64_t cur_time = esp_timer_get_time();
  int diff_time = cur_time - last_time;
  
  if (diff_time > 70000) { // Ignore over ranged speed
    // Update speed x 10
    speed = (double)(circumference_hour * 10) / (double)diff_time;
    last_time = cur_time;
    state = RUN;
    Serial.println(diff_time); // debug
  }
  
  // Wait chattering time
  delay(100);
  // Set and re-start GPIO interrupt
  attachInterrupt(GPIO_NUM_26, magnetPassed_Handler, FALLING);
}

void loop() {
  delay(200);
  dispSpeed(speed);
}
