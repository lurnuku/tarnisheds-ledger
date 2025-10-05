#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM, Arduino IDE -> tools -> PSRAM -> OPI !!!"
#endif

#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include "epd_driver.h"
#include "SpectralLight.h"
#include "utilities.h"
#include "env.h" 
#include "webInterface.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

WebServer server(80);
uint8_t *framebuffer;

struct TodoItem {
    String text;
    bool completed;
};

const int MAX_TODOS = 20;
TodoItem todos[MAX_TODOS];
int todoCount = 0;

void drawHorizontalLine(int x1, int x2, int y, uint8_t *fb) {
    epd_fill_rect(x1, y, x2 - x1, 1, 0, fb);
}

void drawVerticalLine(int x, int y1, int y2, uint8_t *fb) {
    epd_fill_rect(x, y1, 1, y2 - y1, 0, fb);
}

void drawCornerDecoration(int x, int y, int size, bool topLeft, uint8_t *fb) {
    int h_len = size;
    int v_len = size;
    
    if (topLeft) {
        drawHorizontalLine(x, x + h_len, y, fb);
        drawVerticalLine(x, y, y + v_len, fb);
        for (int i = 0; i < 3; i++) {
            epd_draw_pixel(x + i, y + i, 0, fb);
        }
    } else {
        drawHorizontalLine(x - h_len, x, y, fb);
        drawVerticalLine(x, y - v_len, y, fb);
        for (int i = 0; i < 3; i++) {
            epd_draw_pixel(x - i, y - i, 0, fb);
        }
    }
}

void drawEldenRingBorder(uint8_t *fb) {
    int margin = 15;
    int cornerSize = 25;
    
    drawHorizontalLine(margin, EPD_WIDTH - margin, margin, fb);
    drawHorizontalLine(margin, EPD_WIDTH - margin, EPD_HEIGHT - margin, fb);
    drawVerticalLine(margin, margin, EPD_HEIGHT - margin, fb);
    drawVerticalLine(EPD_WIDTH - margin, margin, EPD_HEIGHT - margin, fb);
    
    drawCornerDecoration(margin, margin, cornerSize, true, fb);
    drawCornerDecoration(EPD_WIDTH - margin, margin, cornerSize, false, fb);
    drawCornerDecoration(margin, EPD_HEIGHT - margin, cornerSize, true, fb);
    drawCornerDecoration(EPD_WIDTH - margin, EPD_HEIGHT - margin, cornerSize, false, fb);
    
    int innerOffset = 8;
    drawHorizontalLine(margin + innerOffset, EPD_WIDTH - margin - innerOffset, margin + innerOffset, fb);
    drawHorizontalLine(margin + innerOffset, EPD_WIDTH - margin - innerOffset, EPD_HEIGHT - margin - innerOffset, fb);
    drawVerticalLine(margin + innerOffset, margin + innerOffset, EPD_HEIGHT - margin - innerOffset, fb);
    drawVerticalLine(EPD_WIDTH - margin - innerOffset, margin + innerOffset, EPD_HEIGHT - margin - innerOffset, fb);
}

void drawDividerLine(int y, uint8_t *fb) {
    int margin = 40;
    int width = EPD_WIDTH - (margin * 2);
    int x_start = margin;
    
    drawHorizontalLine(x_start, x_start + width, y, fb);
    
    for (int i = 0; i < 3; i++) {
        int dot_x1 = x_start - 10 + (i * 4);
        int dot_x2 = x_start + width + 2 + (i * 4);
        epd_fill_rect(dot_x1, y - 1, 2, 2, 0, fb);
        epd_fill_rect(dot_x2, y - 1, 2, 2, 0, fb);
    }
}

void drawCheckbox(int x, int y, bool completed, uint8_t *fb) {
    int size = 36;
    
    epd_fill_rect(x, y, size, size, 255, fb);
    epd_draw_rect(x, y, size, size, 0, fb);
    epd_draw_rect(x + 2, y + 2, size - 4, size - 4, 0, fb);

    epd_draw_line(x + 5, y + 5, x + 8, y + 5, 0, fb);
    epd_draw_line(x + 5, y + 5, x + 5, y + 8, 0, fb);

    epd_draw_line(x + size - 6, y + 5, x + size - 9, y + 5, 0, fb);
    epd_draw_line(x + size - 6, y + 5, x + size - 6, y + 8, 0, fb);

    epd_draw_line(x + 5, y + size - 6, x + 8, y + size - 6, 0, fb);
    epd_draw_line(x + 5, y + size - 6, x + 5, y + size - 9, 0, fb);

    epd_draw_line(x + size - 6, y + size - 6, x + size - 9, y + size - 6, 0, fb);
    epd_draw_line(x + size - 6, y + size - 6, x + size - 6, y + size - 9, 0, fb);
    
    if (completed) {
        int center = size / 2;
        int inner = 10;
        
        epd_draw_line(x + center, y + inner, x + center, y + size - inner, 0, fb);
        epd_draw_line(x + inner, y + center, x + size - inner, y + center, 0, fb);
        
        epd_draw_line(x + inner, y + inner, x + size - inner, y + size - inner, 0, fb);
        epd_draw_line(x + size - inner, y + inner, x + inner, y + size - inner, 0, fb);
        
        epd_draw_line(x + center + 1, y + inner, x + center + 1, y + size - inner, 0, fb);
        epd_draw_line(x + inner, y + center + 1, x + size - inner, y + center + 1, 0, fb);
    }
}

void updateDisplay() {
    epd_poweron();
    epd_clear();
    
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
    
    drawEldenRingBorder(framebuffer);
    
    int32_t cursor_x = 60;
    int32_t cursor_y = 70;
    writeln((GFXfont *)&SpectralLight, "~~ Endless Burdens Of The Ashen One ~~", &cursor_x, &cursor_y, framebuffer);
    drawDividerLine(90, framebuffer);
    
    cursor_y = 140;
    int maxLines = 6;
    int displayCount = min(todoCount, maxLines);
    
    if (todoCount == 0) {
        cursor_x = 250;
        cursor_y = 300;
        writeln((GFXfont *)&SpectralLight, "...the ledger lies empty...", &cursor_x, &cursor_y, framebuffer);
    } else {
        for (int i = 0; i < displayCount; i++) {
            int checkbox_x = 50;
            int checkbox_y = cursor_y - 30;
            
            drawCheckbox(checkbox_x, checkbox_y, todos[i].completed, framebuffer);

            cursor_x = checkbox_x + 60;
    
            String todoText = todos[i].text;
            
            int maxTextLen = 33;
            
            if (todoText.length() > maxTextLen) {
                todoText = todoText.substring(0, maxTextLen - 3) + "...";
            }
            
            writeln((GFXfont *)&SpectralLight, todoText.c_str(), &cursor_x, &cursor_y, framebuffer);
            
            cursor_y += SpectralLight.advance_y - 5;
        }
        
        if (todoCount > maxLines) {
            cursor_x = 250;
            cursor_y -= 20;
            String overflow = "... +" + String(todoCount - maxLines) + " more burdens";
            writeln((GFXfont *)&SpectralLight, overflow.c_str(), &cursor_x, &cursor_y, framebuffer);
        }
    }
    
    epd_draw_grayscale_image(epd_full_screen(), framebuffer);
    epd_poweroff();
}

void handleRoot() {
    server.send_P(200, "text/html", HTML_PAGE);
}

void handleGetTodos() {
    String json = "{\"todos\":[";
    for (int i = 0; i < todoCount; i++) {
        if (i > 0) json += ",";
        json += "{\"text\":\"";
        String escaped = todos[i].text;
        escaped.replace("\"", "\\\"");
        escaped.replace("\\", "\\\\");
        json += escaped;
        json += "\",\"completed\":";
        json += todos[i].completed ? "true" : "false";
        json += "}";
    }
    json += "]}";
    server.send(200, "application/json", json);
}

void handleAdd() {
    if (server.hasArg("text") && todoCount < MAX_TODOS) {
        todos[todoCount].text = server.arg("text");
        todos[todoCount].completed = false;
        todoCount++;
        updateDisplay();
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Error");
    }
}

void handleToggle() {
    if (server.hasArg("id")) {
        int id = server.arg("id").toInt();
        if (id >= 0 && id < todoCount) {
            todos[id].completed = !todos[id].completed;
            updateDisplay();
            server.send(200, "text/plain", "OK");
            return;
        }
    }
    server.send(400, "text/plain", "Error");
}

void handleDelete() {
    if (server.hasArg("id")) {
        int id = server.arg("id").toInt();
        if (id >= 0 && id < todoCount) {
            for (int i = id; i < todoCount - 1; i++) {
                todos[i] = todos[i + 1];
            }
            todoCount--;
            updateDisplay();
            server.send(200, "text/plain", "OK");
            return;
        }
    }
    server.send(400, "text/plain", "Error");
}

void handleDeleteAll() {
    todoCount = 0;
    updateDisplay();
    server.send(200, "text/plain", "OK");
}

void setup() {
    Serial.begin(115200);
    
    epd_init();
    framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
    if (!framebuffer) {
        Serial.println("Framebuffer allocation failed!");
        while (1);
    }
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
    
    epd_poweron();
    epd_clear();
    epd_poweroff();
    
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", handleRoot);
    server.on("/todos", handleGetTodos);
    server.on("/add", HTTP_POST, handleAdd);
    server.on("/toggle", handleToggle);
    server.on("/delete", handleDelete);
    server.on("/deleteall", handleDeleteAll);
    
    server.begin();
    Serial.println("HTTP server started");
    
    updateDisplay();
}

void loop() {
    server.handleClient();
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected, reconnecting...");
        WiFi.reconnect();
        delay(5000);
    }
   
    delay(1000);
}