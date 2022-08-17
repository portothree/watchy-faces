#include "beastie.h"

void Beastie::drawWatchFace() {
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(0, 0, daemon_img, 200, 200, GxEPD_WHITE);
    drawWDay();
    drawDate();
    drawTime();
    drawSteps();
  //drawTemperature();
    drawBattery();
    drawX();
    for (uint8_t i=0; i<3; i++) {
        // Reduce ghosting
        display.display(true);
    }
}

void Beastie::drawWDay() {
    display.setFont(&conso10pt7b);
    display.setTextColor(GxEPD_WHITE);
    int16_t  x1, y1;
    uint16_t w, h;
    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(String(dayOfWeek), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(151 - w/2, 67);
    display.println(String(dayOfWeek));
}

void Beastie::drawDate() {
    display.setFont(&conso12pt7b);
    display.setTextColor(GxEPD_WHITE);
    int16_t  x1, y1;
    uint16_t w, h;
    String monthStr = String(currentTime.Month);
    String dayStr = String(currentTime.Day);
    monthStr = currentTime.Month < 10 ? "0" + monthStr : monthStr;
    dayStr = currentTime.Day < 10 ? "0" + dayStr : dayStr;
    String dateStr = dayStr + "/" + monthStr;
    display.getTextBounds(String(dateStr), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(151 - w/2, 88);
    display.println(String(dateStr));
}

void Beastie::drawTime() {
    display.setFont(&conso17pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(111, 119);
    display.fillRoundRect(111, 95, 85, 29, 4, GxEPD_WHITE);
    if (currentTime.Hour < 10) {
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if (currentTime.Minute < 10) {
        display.print("0");
    }
    display.print(currentTime.Minute);
}

void Beastie::drawSteps() {
    display.setFont(&conso11pt7b);
    display.setTextColor(GxEPD_WHITE);
    if (currentTime.Hour == 23 && currentTime.Minute == 59) {
        sensor.resetStepCounter();
    }
    int16_t  x1, y1;
    uint16_t w, h;
    uint32_t stepCount = sensor.getCounter();
    char stepStr[32];
    itoa(stepCount, stepStr, 10);
    int stepStrL = strlen(stepStr);
    memset(stepStr, '0', 5);
    itoa(stepCount, stepStr + max(5-stepStrL, 0), 10);
    display.getTextBounds(String(stepStr), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(152 - w/2, 143);
    display.println(String(stepStr));
}

void Beastie::drawTemperature() {
    display.setFont(&conso10pt7b);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(4, 18);
    uint8_t temperatureRTC = RTC.temperature() / 4;
    if (settings.weatherUnit == "imperial") {
        temperatureRTC = temperatureRTC * (9/5) + 32;
    }
    if (temperatureRTC < 10) {
        display.print("0");
    }
    display.print(temperatureRTC);
    display.setCursor(22, 11);
    display.print(".");
    display.setCursor(29, 16);
    if (settings.weatherUnit == "imperial") {
        display.print("f");
    } else {
        display.print("c");
    }
}

void Beastie::drawBattery() {
    float BATTV = getBatteryVoltage() - 3.60;
    int batt_w = constrain(((33.33 * BATTV) + 0.9), 0, 20);
    display.fillRoundRect(138, 150, 30, 10, 5, GxEPD_WHITE);
    display.fillRoundRect(140, 152, 26, 6, 4, GxEPD_BLACK);
    if (BATTV > 0) {
        if (batt_w % 2 != 0) {
            display.fillRoundRect(153 - (batt_w/2)-1, 154, batt_w, 2, 3, GxEPD_WHITE);
        }
        display.fillRoundRect(153 - batt_w/2, 154, batt_w, 2, 3, GxEPD_WHITE);
    }
}

void Beastie::drawX() {
    display.setFont(&conso11pt7b);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(149, 50);
    display.print("x");
}
