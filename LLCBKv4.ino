#include <WiFi.h>
#include <EEPROM.h>

#include "definitions.hpp"
#include "animations.hpp"

// Pin Definitions
#define BRAKE_PIN 32
#define RIGHT_PIN 33
#define LEFT_PIN 27
#define FORCE_STATIC_PIN 25
#define FORCE_OVERRIDE_PIN 26
#define EEPROM_SIZE 11
#define MIC_PIN 34

// WiFi Settings
#define SSID "Powercreep-LR34"
#define PASSWORD "K4Tech.net"

WiFiServer server(80);
String header;
anims an;
demo dm;

// Function Protoypes
void ledRun(/*void* parameter*/);
void webServerHandler(void* parameter);
void generateWebPage(WiFiClient& client);
void timerTask(void* parameter);
void inputHandler(void* parameter);

void init() {
  Serial.begin(115200);

  btStop();

  FastLED.addLeds<WS2812, 16, GRB>(globals.lLeds, globals.totalLEDs);
  FastLED.addLeds<WS2812, 17, GRB>(globals.rLeds, globals.totalLEDs);

  // GPIO INIT
  pinMode(BRAKE_PIN, INPUT_PULLDOWN);
  pinMode(RIGHT_PIN, INPUT_PULLDOWN);
  pinMode(LEFT_PIN, INPUT_PULLDOWN);
  pinMode(MIC_PIN, INPUT);
  pinMode(FORCE_STATIC_PIN, INPUT_PULLDOWN);
  pinMode(FORCE_OVERRIDE_PIN, INPUT_PULLDOWN);

  // AP Creation
  Serial.println("Configuring AP..");
  WiFi.softAP(SSID, PASSWORD);
  Serial.print("AP hosted at "); 
  Serial.println(WiFi.softAPIP());
  server.begin();

  EEPROM.begin(EEPROM_SIZE);
  globals.desiredStyle = EEPROM.read(0);
  globals.web.bands.Band0 = EEPROM.read(1);
  globals.web.bands.Band1 = EEPROM.read(2);
  globals.web.bands.Band2 = EEPROM.read(3);
  globals.web.bands.Band3 = EEPROM.read(4);
  globals.web.bands.Band4 = EEPROM.read(5);
  globals.web.bands.Band5 = EEPROM.read(6);
  globals.web.bands.Band6 = EEPROM.read(7);
  globals.web.bands.Band7 = EEPROM.read(8);
  globals.web.bands.Band8 = EEPROM.read(9);
  globals.web.overrideStyle = EEPROM.read(10);

  // Task Creation
  // xTaskCreatePinnedToCore(
  //   ledRun,
  //   "LEDRun",
  //   10000,
  //   NULL,
  //   2,
  //   NULL,
  //   0
  // );

  xTaskCreatePinnedToCore(
    webServerHandler,
    "WebServer",
    10000,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    timerTask,
    "Timer",
    4096,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    inputHandler,
    "InputHandler",
    10000,
    NULL,
    1,
    NULL,
    0
  );
}

void ledRun(/*void* parameter*/) {
  while (!globals.shutdown) {
    Serial.println(globals.desiredStyle);
    Serial.println(globals.runningStyle);
    Serial.println(globals.desiredMode);
    Serial.println(globals.runningMode);


    switch (globals.desiredStyle) {
      case 1:
        switch (globals.desiredMode) {
          case 10:
            globals.runningStyle = 1;
            globals.runningMode = 10;
            an.style1.mode10();
            break;
          case 15:
            globals.runningStyle = 1;
            globals.runningMode = 15;
            an.style1.mode15();
            break;
          case 16:
            globals.runningStyle = 1;
            globals.runningMode = 16;
            an.style1.mode16();
            break;
          case 17:
            globals.runningStyle = 1;
            globals.runningMode = 17;
            an.style1.mode17();
            break;
          case 20:
            globals.runningStyle = 1;
            globals.runningMode = 20;
            an.style1.mode20();
            break;
          case 25:
            globals.runningStyle = 1;
            globals.runningMode = 25;
            an.style1.mode25();
            break;
          case 26:
            globals.runningStyle = 1;
            globals.runningMode = 26;
            an.style1.mode26();
            break;
          case 27:
            globals.runningStyle = 1;
            globals.runningMode = 27;
            an.style1.mode27();
            break;
        }
        break;
      case 2:
        switch (globals.desiredMode) {
          case 10:
            globals.runningStyle = 2;
            globals.runningMode = 10;
            globals.compBrakeAnim = false;
            an.style2.mode10();
            break;
          case 15:
            globals.runningStyle = 2;
            globals.runningMode = 15;
            globals.compBrakeAnim = false;
            an.style2.mode15();
            break;
          case 16:
            globals.runningStyle = 2;
            globals.runningMode = 16;
            globals.compBrakeAnim = false;
            an.style2.mode16();
            break;
          case 17:
            globals.runningStyle = 2;
            globals.runningMode = 17;
            globals.compBrakeAnim = false;
            an.style2.mode17();
            break;
          case 20:
            globals.runningStyle = 2;
            globals.runningMode = 20;
            an.style2.mode20();
            break;
          case 25:
            globals.runningStyle = 2;
            globals.runningMode = 25;
            an.style2.mode25();
            break;
          case 26:
            globals.runningStyle = 2;
            globals.runningMode = 26;
            an.style2.mode26();
            break;
          case 27:
            globals.runningStyle = 2;
            globals.runningMode = 27;
            an.style2.mode27();
            break;
        }
        break;
      case 3:
        switch (globals.desiredMode) {
          case 10:
            globals.runningStyle = 3;
            globals.runningMode = 10;
            globals.compBrakeAnim = false;
            an.style3.mode10();
            break;
          case 15:
            globals.runningStyle = 3;
            globals.runningMode = 15;
            globals.compBrakeAnim = false;
            an.style3.mode15();
            break;
          case 16:
            globals.runningStyle = 3;
            globals.runningMode = 16;
            globals.compBrakeAnim = false;
            an.style3.mode16();
            break;
          case 17:
            globals.runningStyle = 3;
            globals.runningMode = 17;
            globals.compBrakeAnim = false;
            an.style3.mode17();
            break;
          case 20:
            globals.runningStyle = 3;
            globals.runningMode = 20;
            an.style3.mode20();
            break;
          case 25:
            globals.runningStyle = 3;
            globals.runningMode = 25;
            an.style3.mode25();
            break;
          case 26:
            globals.runningStyle = 3;
            globals.runningMode = 26;
            an.style3.mode26();
            break;
          case 27:
            globals.runningStyle = 3;
            globals.runningMode = 27;
            an.style3.mode27();
            break;
        }
        break;
      case 4:
        switch (globals.desiredMode) {
          case 10:
            globals.runningStyle = 4;
            globals.runningMode = 10;
            globals.compBrakeAnim = false;
            an.style4.mode10();
            break;
          case 15:
            globals.runningStyle = 4;
            globals.runningMode = 15;
            globals.compBrakeAnim = false;
            an.style4.mode15();
            break;
          case 16:
            globals.runningStyle = 4;
            globals.runningMode = 16;
            globals.compBrakeAnim = false;
            an.style4.mode16();
            break;
          case 17:
            globals.runningStyle = 4;
            globals.runningMode = 17;
            globals.compBrakeAnim = false;
            an.style4.mode17();
            break;
          case 20:
            globals.runningStyle = 4;
            globals.runningMode = 20;
            an.style4.mode20();
            break;
          case 25:
            globals.runningStyle = 4;
            globals.runningMode = 25;
            an.style4.mode25();
            break;
          case 26:
            globals.runningStyle = 4;
            globals.runningMode = 26;
            an.style4.mode26();
            break;
          case 27:
            globals.runningStyle = 4;
            globals.runningMode = 27;
            an.style4.mode27();
            break;
        }
        break;
      case 5:
        switch (globals.desiredMode) {
          case 10:
            globals.runningStyle = 5;
            globals.runningMode = 10;
            globals.compBrakeAnim = false;
            an.style5.mode10();
            break;
          case 15:
            globals.runningStyle = 5;
            globals.runningMode = 15;
            globals.compBrakeAnim = false;
            //an.style5.mode15();
            break;
          case 16:
            globals.runningStyle = 5;
            globals.runningMode = 16;
            globals.compBrakeAnim = false;
            //an.style5.mode16();
            break;
          case 17:
            globals.runningStyle = 5;
            globals.runningMode = 17;
            globals.compBrakeAnim = false;
            //an.style5.mode17();
            break;
          case 20:
            globals.runningStyle = 5;
            globals.runningMode = 20;
            //an.style5.mode20();
            break;
          case 25:
            globals.runningStyle = 5;
            globals.runningMode = 25;
            //an.style5.mode25();
            break;
          case 26:
            globals.runningStyle = 5;
            globals.runningMode = 26;
            //an.style5.mode26();
            break;
          case 27:
            globals.runningStyle = 5;
            globals.runningMode = 27;
            //an.style5.mode27();
            break;
        }
        break;
      case 6:
        globals.runningStyle = 6;
        globals.runningMode = 10;
        an.style6.mode10();
        break;
      case 7:
        globals.runningStyle = 7;
        globals.runningMode = 10;
        an.style7.mode10();
        break;
      case 8:
        globals.runningStyle = 8;
        globals.runningMode = 10;
        //an.style8.mode10();
        break;
      case 9:
        globals.runningStyle = 9;
        globals.runningMode = 10;
        an.style9.mode10();
        break;
      case 10:
        globals.runningStyle = 10;
        globals.runningMode = 10;
        an.style10.mode10();
        break;
      case 11:
        globals.runningStyle = 11;
        globals.runningMode = 10;
        dm.mode10();
        break;
      case 12:
        globals.runningStyle = 12;
        globals.runningMode = 10;
        an.style1.mode0();
        break;
      case 13:
        globals.runningStyle = 13;
        globals.runningMode = 10;
        an.style13.mode10();
        break;
      case 14:
        globals.runningStyle = 14;
        globals.runningMode = 10;
        an.style14.mode10();
        break;
      case 15:
        globals.runningStyle = 15;
        globals.runningMode = 10;
        an.style15.mode10();
        break;
      case 16:
        globals.runningStyle = 16;
        globals.runningMode = 10;
        an.style16.mode10();
        break;
    }

    delay(5);
  }
}

void webServerHandler(void* parameter) {
  while (true) {
    WiFiClient client = server.available();

    if (client) {
      String currentLine = "";
      unsigned long connectionStart = millis();
      const unsigned long timeout = 2000; // 2 second timeout

      while (client.connected() && (millis() - connectionStart < timeout)) {
        if (client.available()) {
          char c = client.read();
          header += c;

          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              // Handle style changes
              if (header.startsWith("GET /Style")) {
                int styleNum = header.substring(header.indexOf("Style") + 5).toInt();
                if (styleNum >= 1 && styleNum <= 16) {
                  globals.desiredStyle = styleNum;
                }
                
              } else if (header.startsWith("GET /Tab")) {
                int tabNum = header.substring(header.indexOf("Tab") + 3).toInt();
                if (tabNum >= 1 && tabNum <= 2) {
                  globals.web.tab = tabNum;
                }
                
              } else if (header.startsWith("GET /Settings?")) {
                // Extract parameters after '?'
                int paramsStart = header.indexOf("?") + 1;
                String params = header.substring(paramsStart, header.indexOf(" HTTP"));
                
                // Process each parameter
                while (params.length() > 0) {
                  int ampIndex = params.indexOf("&");
                  String param = (ampIndex == -1) ? params : params.substring(0, ampIndex);
                  
                  int equalIndex = param.indexOf("=");
                  if (equalIndex != -1) {
                    String key = param.substring(0, equalIndex);
                    int value = param.substring(equalIndex + 1).toInt();
                    
                    // Update band values
                    if (key == "B0") globals.web.bands.Band0 = value;
                    else if (key == "B1") globals.web.bands.Band1 = value;
                    else if (key == "B2") globals.web.bands.Band2 = value;
                    else if (key == "B3") globals.web.bands.Band3 = value;
                    else if (key == "B4") globals.web.bands.Band4 = value;
                    else if (key == "B5") globals.web.bands.Band5 = value;
                    else if (key == "B6") globals.web.bands.Band6 = value;
                    else if (key == "B7") globals.web.bands.Band7 = value;
                    else if (key == "B8") globals.web.bands.Band8 = value;
                  }
                  
                  if (ampIndex == -1) break;
                  params = params.substring(ampIndex + 1);
                }
                
                client.println("<html><head><script>window.location.href = '/Tab2';</script></head><body></body></html>");
                
              } else if (header.indexOf("GET /SetDefault") >= 0) {
                // Save current settings to EEPROM
                EEPROM.write(0, globals.desiredStyle);
                EEPROM.write(1, globals.web.bands.Band0);
                EEPROM.write(2, globals.web.bands.Band1);
                EEPROM.write(3, globals.web.bands.Band2);
                EEPROM.write(4, globals.web.bands.Band3);
                EEPROM.write(5, globals.web.bands.Band4);
                EEPROM.write(6, globals.web.bands.Band5);
                EEPROM.write(7, globals.web.bands.Band6);
                EEPROM.write(8, globals.web.bands.Band7);
                EEPROM.write(9, globals.web.bands.Band8);
                EEPROM.commit();
                
                client.print("<html><head><script>window.location.href = \"/Tab");
                client.print(globals.web.tab);
                client.println("\";</script></head><body></body></html>");
              } else if (header.indexOf("GET /SetOverride") >= 0) {
                globals.web.overrideStyle = globals.desiredStyle;

                EEPROM.write(10, globals.web.overrideStyle);
                EEPROM.commit();

                client.print("<html><head><script>window.location.href = \"/Tab");
                client.print(globals.web.tab);
                client.println("\";</script></head><body></body></html>");
              }

              // Generate and send the web page
              generateWebPage(client);
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }
        }
      }
      header = "";
      client.stop();
    }
    delay(2); // Small delay to prevent watchdog issues
  }
}

void generateWebPage(WiFiClient& client) {
  // Send HTML header and styling
  client.println("<!DOCTYPE html><html><head>");
  client.println("<title>PowerCreep-LR34</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("<style>");
  client.println("body{background-color:#000;color:#aaa;font-family:Arial,sans-serif;text-align:center;overflow:visible}");
  client.println("h1{margin-top:40px;font-size:45px;margin-bottom:-5px}");
  client.println("h2{font-size:20px;margin-bottom:-5px}");
  client.println(".subheading{margin-top:10px}");
  client.println(".ss{margin-top:40px;margin-bottom:5px}");
  client.println(".button{display:inline-block;width:150px;height:50px;background-color:#000;border:2px solid #444;color:#555;margin:5px;text-decoration:none;line-height:50px}");
  client.println(".button2{display:inline-block;width:150px;height:50px;background-color:#000;border:2px solid #8ef;color:#8ef;margin:5px;text-decoration:none;line-height:50px}");
  client.println(".standalone-button{display:block;width:200px;height:50px;background-color:#000;border:2px solid #999;color:#fff;margin:20px auto;text-decoration:none;line-height:50px}");
  client.println(".divider{border-top:1px solid #444;margin:20px auto;width:80%}");
  client.println("input[type=number]{background-color:#222;color:#fff;border:1px solid #444;padding:5px;width:60px;margin:5px}");
  client.println("input[type=submit]{background-color:#000;border:2px solid #8ef;color:#8ef;padding:10px 20px;margin:20px;cursor:pointer}");
  client.println("</style></head><body>");

  // Header
  client.println("<h1>PowerCreep</h1>");
  client.println("<h2 class=\"subheading\">Lighting System - R34</h2>");
  client.println("<hr class=\"divider\">");

  // Navigation tabs
  client.println("<div>");
  client.println(globals.web.tab == 1 ? 
    "<a class=\"button2\" href=\"/Tab1\">Style</a>" : 
    "<a class=\"button\" href=\"/Tab1\">Style</a>");
  client.println(globals.web.tab == 2 ? 
    "<a class=\"button2\" href=\"/Tab2\">Settings</a>" : 
    "<a class=\"button\" href=\"/Tab2\">Settings</a>");
  client.println("</div>");
  client.println("<hr class=\"divider\">");

  if (globals.web.tab == 1) {
    // Style selection page
    client.println("<h2 class=\"ss\">Select a style</h2>");
    
    // Define style buttons in pairs
    const struct {
      int id;
      const char* name;
    } styles[] = {
      {1, "Static (Classic)"}, {2, "Static (Whole)"},
      {3, "Animated"}, {4, "Nismo"},
      {5, "Test"}, {6, "Lightshow"},
      {7, "Music (Peaks)"}, {8, "Music (No Peaks)"},
      {9, "Sinelon"}, {10, "Concentric"},
      {11, "Demo"}, {12, "Blackout"},
      {13, "Diagonal"}, {14, "Spiral"},
      {15, "Inwards"}, {16, "Outwards"}
    };

    for (int i = 0; i < 16; i += 2) {
      client.println("<div>");
      for (int j = 0; j < 2; j++) {
        int index = i + j;
        client.printf("<a class=\"%s\" href=\"/Style%d\">%s</a>",
          (globals.desiredStyle == styles[index].id) ? "button2" : "button",
          styles[index].id,
          styles[index].name);
      }
      client.println("</div>");
    }
    
  } else {
    // Settings page
    client.println("<h2 class=\"ss\">Music Sensitivity</h2>");
    client.println("<form method=\"get\" action=\"/Settings\">");
    
    // Generate band controls
    const char* bandLabels[] = {"Band 0", "Band 1", "Band 2", "Band 3", "Band 4",
                               "Band 5", "Band 6", "Band 7", "Band 8"};
    uint8_t* bandValues[] = {&globals.web.bands.Band0, &globals.web.bands.Band1,
                            &globals.web.bands.Band2, &globals.web.bands.Band3,
                            &globals.web.bands.Band4, &globals.web.bands.Band5,
                            &globals.web.bands.Band6, &globals.web.bands.Band7,
                            &globals.web.bands.Band8};
    
    for (int i = 0; i < 9; i++) {
      client.println("<div>");
      client.printf("<label for=\"B%d\">%s:</label>", i, bandLabels[i]);
      client.printf("<input type=\"number\" id=\"B%d\" name=\"B%d\" min=\"0\" max=\"255\" value=\"%d\" step=\"1\" required>",
                   i, i, *bandValues[i]);
      client.println("</div>");
    }
    
    client.println("<input type=\"submit\" value=\"Submit\">");
    client.println("</form>");
  }

  // Set Default button
  client.println("<div style=\"display:flex;justify-content:center;gap:20px;\">");
  client.println("<a class=\"standalone-button\" style=\"margin:20px 0\" href=\"/SetDefault\">Set Default</a>");
  client.println("<a class=\"standalone-button\" style=\"margin:20px 0\" href=\"/SetOverride\">Set Manual Override</a>");
  client.println("</div>");
  client.println("</body></html>");
}

void timerTask(void* parameter) {
  const unsigned long DEMO_INTERVAL = 20;        // Check interval
  const unsigned long STYLE_DURATION = 5000;     // 6 seconds per style
  const unsigned long DEMO_DURATION = 49 * STYLE_DURATION; // Total demo duration (33 styles)
  
  unsigned long lastDemoCheck = 0;
  unsigned long demoStartTime = 0;

  while (true) {
    unsigned long currentMillis = millis();

    // Handle demo timing
    if (globals.desiredStyle == 11) {
      // Initialize start time if just entering demo mode
      if (demoStartTime == 0) {
        demoStartTime = currentMillis;
      }

      if (currentMillis - lastDemoCheck >= DEMO_INTERVAL) {
        lastDemoCheck = currentMillis;
        
        // Calculate time within the demo cycle
        unsigned long demoTime = (currentMillis - demoStartTime) % DEMO_DURATION;
        uint8_t newDemoIndex = demoTime / STYLE_DURATION;
        
        if (globals.demoIndex != newDemoIndex) {
          globals.halt = true;
          globals.demoIndex = newDemoIndex;
        }
      }
    } else {
      // Reset demo timing when exiting demo mode
      demoStartTime = 0;
      globals.demoIndex = 0;
    }

    // Check for state changes and set halt flag
    if ((globals.desiredMode != globals.runningMode) || (globals.desiredStyle != globals.runningStyle)) {
      globals.halt = true;
    }

    delay(1); // Small delay to prevent watchdog issues
  }
}

void inputHandler(void* parameter) {
  unsigned long lastPulseTimeRight = 0;
  unsigned long lastPulseTimeLeft = 0;
  const unsigned long pulseInterval = 100;  // Adjust as needed
  const unsigned long pulseTimeout = 1000;  // Adjust as needed

  while (true) {
    unsigned long currentMillis = millis();
    
    // Check indicators state
    bool rightActive = digitalRead(RIGHT_PIN) == HIGH;
    bool leftActive = digitalRead(LEFT_PIN) == HIGH;
    bool brakeActive = digitalRead(BRAKE_PIN) == HIGH;
    bool forceStaticActive = digitalRead(FORCE_STATIC_PIN) == HIGH;
    bool forceOverrideActive = digitalRead(FORCE_OVERRIDE_PIN) == HIGH;

    if (rightActive) {
      lastPulseTimeRight = currentMillis;
    }

    if (leftActive) {
      lastPulseTimeLeft = currentMillis;
    }

    bool rightPulsing = (currentMillis - lastPulseTimeRight) <= pulseInterval;
    bool leftPulsing = (currentMillis - lastPulseTimeLeft) <= pulseInterval;

    if (forceStaticActive) {
      if (!globals.backupStaticState) { // Switch 1 just activated
        globals.backupStyle = globals.desiredStyle;
        globals.desiredStyle = 1;
      }
    } else if (forceOverrideActive) {
      if (!globals.backupOverrideState) { // Switch 2 just activated
        globals.backupStyle = globals.desiredStyle;
        globals.desiredStyle = globals.web.overrideStyle;
      }
    } else if (globals.backupStaticState || globals.backupOverrideState) { // A switch was just deactivated, restore previous style
      globals.desiredStyle = globals.backupStyle;
    }

    // Update previous switch states
    globals.backupStaticState = forceStaticActive;
    globals.backupOverrideState = forceOverrideActive;

    // Update desired mode based on inputs
    if (!brakeActive) {
      if (leftPulsing && !rightPulsing) {
        globals.desiredMode = 15;  // Right indicator
      } else if (rightPulsing && !leftPulsing) {
        globals.desiredMode = 16;  // Left indicator
      } else if (rightPulsing && leftPulsing) {
        globals.desiredMode = 17;  // Both indicators
      } else {
        globals.desiredMode = 10;  // Normal mode
      }
    } else {
      if (leftPulsing && !rightPulsing) {
        globals.desiredMode = 25;  // Brake + Right indicator
      } else if (rightPulsing && !leftPulsing) {
        globals.desiredMode = 26;  // Brake + Left indicator
      } else if (rightPulsing && leftPulsing) {
        globals.desiredMode = 27;  // Brake + Both indicators
      } else {
        globals.desiredMode = 20;  // Brake only
      }
    }

    delay(10);
  }
}

void setup() {
  init();
}

void loop() {
  ledRun();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
