#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NewPing.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define TRIGGER_PIN  9
#define ECHO_PIN     10
NewPing sonar(TRIGGER_PIN, ECHO_PIN, 100);

enum Mode {LOCKED, DIST_METER, REACTION, FLAPPY};
Mode currentMode = LOCKED;

int passStep = 0; 
int successCounter = 0;
long reactionStart = 0;

// Flappy Variables (Confined to bottom half)
int birdY = 48;
int wallX = 128;
int gapY = 45;
int score = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  randomSeed(analogRead(0));
}

void loop() {
  int d = sonar.ping_cm();
  if (d == 0) d = 100;
  
  display.clearDisplay();
  // Drawing the crack boundary
  display.drawLine(0, 31, 128, 31, 1); 

  switch (currentMode) {
    case LOCKED:      handleLock(d); break;
    case DIST_METER:  runDist(d); break;
    case REACTION:    runReact(d); break;
    case FLAPPY:      runFlappy(d); break;
  }

  display.display();
  delay(30);
}

void handleLock(int d) {
    display.setTextSize(1);
    display.setCursor(0, 35);
    
    // Bottom-half instructions
    if (passStep == 0) display.print("10:Dist 30:Reac 20:Flap");
    else if (passStep == 1) display.print("PATH: DIST -> 30cm");
    else if (passStep == 2) display.print("PATH: REAC -> 10cm");
    else if (passStep == 3) display.print("PATH: FLAP -> 20cm");

    display.setCursor(0, 50);
    display.print("Dist: "); display.print(d); display.print("cm");

    // STICKY LOGIC BRANCHING
    if (passStep == 0) {
      if (d > 9 && d < 11) checkCounter(1);      // Start Dist Path
      else if (d > 29 && d < 31) checkCounter(2); // Start React Path
      else if (d > 19 && d < 21) checkCounter(3); // Start Flap Path
      else successCounter = 0;
    } 
    else if (passStep == 1 && d > 29 && d < 31) finishStep(DIST_METER);
    else if (passStep == 2 && d > 9 && d < 11) finishStep(REACTION);
    else if (passStep == 3 && d > 19 && d < 21) finishStep(FLAPPY);
    else if (d > 60) { passStep = 0; successCounter = 0; } // Reset on pull-away

    // Progress Bar at the absolute bottom
    display.fillRect(0, 62, map(successCounter, 0, 20, 0, 128), 2, 1);
}

void checkCounter(int nextStep) {
  successCounter++;
  if (successCounter > 20) { passStep = nextStep; successCounter = 0; delay(800); }
}

void finishStep(Mode targetMode) {
  successCounter++;
  if (successCounter > 20) { currentMode = targetMode; passStep = 0; successCounter = 0; }
}

// --- APP 1: DISTANCE ---
void runDist(int d) {
  display.setCursor(0, 35); display.print("DISTANCE:");
  display.setTextSize(2); display.setCursor(40, 45);
  display.print(d); display.print("cm");
  if (d < 5 && d > 0) { currentMode = LOCKED; delay(1000); }
}

// --- APP 2: REACTION ---
void runReact(int d) {
  display.setCursor(0, 35);
  if (reactionStart == 0) {
    display.print("STAY STILL... >40cm");
    if (d > 40) reactionStart = millis() + random(1000, 3000);
  } else if (millis() < reactionStart) {
    display.print("READY...");
  } else {
    display.setTextSize(2); display.print("GO!!");
    if (d < 20 && d > 0) {
      long score_time = millis() - reactionStart;
      display.clearDisplay(); display.setCursor(0, 45);
      display.setTextSize(1); display.print("TIME: "); display.print(score_time);
      display.display(); delay(3000);
      reactionStart = 0; currentMode = LOCKED;
    }
  }
}

// --- APP 3: FLAPPY (BOTTOM ONLY) ---
void runFlappy(int d) {
  birdY = map(d, 10, 20, 32, 58);
  birdY = constrain(birdY, 32, 58);

  display.fillRect(20, birdY, 4, 4, 1); 
  display.fillRect(wallX, 32, 8, gapY - 32, 1); 
  display.fillRect(wallX, gapY + 14, 8, 64 - (gapY + 14), 1); 

  wallX -= 4;
  if (wallX < -10) { wallX = 128; gapY = random(35, 50); score++; }

  if (wallX < 24 && wallX > 16 && (birdY < gapY || birdY > gapY + 10)) {
    display.setCursor(40, 45); display.print("CRASH!");
    display.display(); delay(2000);
    score = 0; wallX = 128; currentMode = LOCKED;
  }
  display.setCursor(0, 55); display.print("S:"); display.print(score);
  if (d < 5 && d > 0) currentMode = LOCKED; 
}