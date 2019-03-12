#include <Arduino.h>

#include "OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <AutoWifi.h>

#include "SimStreamer.h"
#include "OV2640Streamer.h"
#include "OV2640.h"
#include "CRtspSession.h"


// This board has slightly different GPIO bindings (and lots more RAM)
// uncomment to use
// #define USEBOARD_GENERIC
// #define USEBOARD_TTGO_T
#define USEBOARD_TTGO_T_PLUS
// #define USEBOARD_AITHINKER

#if   defined USEBOARD_GENERIC     && defined USEBOARD_TTGO_T
#error Generic and TTGO_T cannot both be defined
#elif defined USEBOARD_GENERIC     && defined USEBOARD_TTGO_T_PLUS
#error Generic and TTGO_T_PLUS cannot both be defined
#elif defined USEBOARD_GENERIC     && defined USEBOARD_AITHINKER
#error Generic and AITHINKER cannot both be defined
#elif defined USEBOARD_TTGO_T      && defined USEBOARD_TTGO_T_PLUS
#error TTGO_T and TTGO_T_PLUS cannot both be defined
#elif defined USEBOARD_TTGO_T      && defined USEBOARD_AITHINKER
#error TTGO_T and AITHINKER cannot both be defined
#elif defined USEBOARD_TTGO_T_PLUS && defined USEBOARD_AITHINKER
#error TTGO_T_PLUS and AITHINKER cannot both be defined
#endif

// #define SOFTAP_MODE // If you want to run our own softap turn this on
#define WIFICLIENT_HARD_CODED // If you want to hard code you wifi credentials
#define ENABLE_WEBSERVER
#define ENABLE_RTSPSERVER

#ifdef WIFICLIENT_HARD_CODED
    #include "wifikeys.h"
#endif


#ifndef USEBOARD_AITHINKER
    #define ENABLE_OLED //if want use oled, turn on this macro
#endif


#if defined USEBOARD_TTGO_T
    // TTGO T-Camera ESP32 WROVER & PSRAM Camera Module ESP32-WROVER-B OV2640 Camera Module 0.96 OLED
    // Master Chip: esp32 dual-core
    // Protocol: Wi-Fi 802.11 b/g/n & Bluetooth 4.2 BLE & BR/EDR
    // Flash: 4MBytes
    // PSRAM: 8MBytes
    // Display chip: SSD1306 I2C
    // Display type: OLED
    // Display resolution: 128x64
    // PIR: AS312
    // Uart Chip: CP2104
    // Charging chip: IP5306 I2C
    // Camera: OV2640
    // Camera Resolution: 2Megapixel

    #define FACTORYRESET_BUTTON 34

    // PIR AS312
    #define PIR                 33

    // BME280
    #define BME280_SCL 22
    #define BME280_SDA 21

#elif defined USEBOARD_TTGO_T_PLUS
    // Chipset	ESPRESSIF-ESP32-DOWDQ6 240MHz Xtensa® single-/dual-core 32-bit LX6 microprocessor
    // FLASH	QSPI flash/SRAM, up to 4 x 16 MB
    // SRAM 	520 kB SRAM +  8MBytes External SPRAM
    // https://github.com/Xinyuan-LilyGo/esp32-camera-screen

    #undef FACTORYRESET_BUTTON

    // PMIC - Battery Management ESOP-8 RoHS
    // INJOINIC IP5306 & BME280
    #define IP5306_SDA 18
    #define IP5306_SCL 23
    #define BME280_SDA 18
    #define BME280_SCL 23

    // mic - MSM261S4030H0
    #define MIC_SCLK 14
    #define MIC_LCLK 32 //ws
    #define MIC_DOUT 33 //sd
    #define MIC_DIN  -1

    // SD card
    #define SD_MISO 22
    #define SD_MOSI 19
    #define SD_SCLK 21
    #define SD_CS    0

    //CP2104

#elif defined USEBOARD_AITHINKER
    #undef FACTORYRESET_BUTTON

#else
    // If your board has a GPIO which is attached to a button, uncomment the following line
    // and adjust the GPIO number as needed.  If that button is held down during boot the device
    // will factory reset.
    #define FACTORYRESET_BUTTON 32
#endif




#ifdef ENABLE_OLED
    #ifdef USEBOARD_TTGO_T_PLUS
        #undef ENABLE_OLED
        
        // #define USER_SETUP_LOADED 
        // // #define ST7789_DRIVER
        // // //  #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
        // // //  #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red
        // // // For ST7789, ST7735 and ILI9163 ONLY, define the pixel width and height in portrait orientation
        // // // #define TFT_WIDTH  80
        // #define TFT_WIDTH  128
        // // // #define TFT_WIDTH  240 // ST7789 240 x 240 and 240 x 320
        // // // #define TFT_HEIGHT 160
        // #define TFT_HEIGHT 128
        // // // #define TFT_HEIGHT 240 // ST7789 240 x 240
        // // // #define TFT_HEIGHT 320 // ST7789 240 x 320
        // #define TFT_MISO   22
        // #define TFT_MOSI   19
        // #define TFT_SCLK   21
        // #define TFT_CS     12  // Chip select control pin D8
        // #define TFT_DC     15  // Data Command control pin
        // #define TFT_RST    -1  // Reset pin (could connect to NodeMCU RST, see next line)
        // #define TFT_BL     -1  
        // #define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
        // #define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
        // #define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
        // #define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
        // #define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
        // #define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
        // #define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

        // #define SMOOTH_FONT
        // #define SPI_FREQUENCY  27000000
        // #define SPI_TOUCH_FREQUENCY  2500000
        // // #define TFT_SDA_READ

        // #define ESP32_PARALLEL


        // #include <TFT_eSPI.h>
        // #include <SPI.h>

        // TFT_eSPI tft = TFT_eSPI();

        // IPS Panel ST7789/1.3 Inch
        #define ST7789_MISO   22
        #define ST7789_MOSI   19
        #define ST7789_SCLK   21
        #define ST7789_DC     15
        #define ST7789_TFT_CS 12
        #define ST7789_TFT_BK  2
        #define ST7789_SD_CS   0

    #else
        #include "SSD1306.h"
        #define OLED_ADDRESS 0x3c

        #if defined USEBOARD_TTGO_T
            #define I2C_SDA 21
            #define I2C_SCL 22
        #else
            #define I2C_SDA 14
            #define I2C_SCL 13
        #endif

        SSD1306Wire display(OLED_ADDRESS, I2C_SDA, I2C_SCL, GEOMETRY_128_32);
        bool hasDisplay; // we probe for the device at runtime
    #endif
#endif

OV2640 cam;

#ifdef ENABLE_WEBSERVER
    WebServer server(80);
#endif

#ifdef ENABLE_RTSPSERVER
    WiFiServer rtspServer(8554);
#endif


#ifdef SOFTAP_MODE
    IPAddress apIP = IPAddress(192, 168, 1, 1);
#endif


#ifdef ENABLE_WEBSERVER
    void handle_jpg_stream(void)
    {
        WiFiClient client = server.client();
        String response = F("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n");
        // String response = "HTTP/1.1 200 OK\r\n";
        // response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
        server.sendContent(response);

        while (1)
        {
            cam.run();
            if (!client.connected())
                break;
            response = "--frame\r\nContent-Type: image/jpeg\r\n\r\n";
            server.sendContent(response);

            client.write((char *)cam.getfb(), cam.getSize());
            server.sendContent("\r\n");
            if (!client.connected())
                break;
        }
    }

    void handle_jpg(void)
    {
        WiFiClient client = server.client();

        cam.run();
        if (!client.connected())
        {
            return;
        }
        String response = F("HTTP/1.1 200 OK\r\nContent-disposition: inline; filename=capture.jpg\r\nContent-type: image/jpeg\r\n\r\n");
        // String response = "HTTP/1.1 200 OK\r\n";
        // response += "Content-disposition: inline; filename=capture.jpg\r\n";
        // response += "Content-type: image/jpeg\r\n\r\n";
        server.sendContent(response);
        client.write((char *)cam.getfb(), cam.getSize());
    }

    void handleNotFound()
    {
        String message = F("Server is running!\n\n");
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";
        server.send(200, "text/plain", message);
    }
#endif


void lcdMessage(String msg)
{
  #ifdef ENABLE_OLED
    if(hasDisplay) {
        display.clear();
        display.drawString(128 / 2, 32 / 2, msg);
        display.display();
    }
  #endif
}


void setup()
{
    #ifdef ENABLE_OLED
        hasDisplay = display.init();
        if(hasDisplay) {
            display.flipScreenVertically();
            display.setFont(ArialMT_Plain_16);
            display.setTextAlignment(TEXT_ALIGN_CENTER);
        }
    #endif



    lcdMessage("booting");

    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    // #ifdef USEBOARD_TTGO_T_PLUS
    //     //https://github.com/Bodmer/TFT_eSPI/blob/master/examples/Generic/TFT_Screen_Capture/TFT_Screen_Capture.ino
    //     Serial.println("starting TFT");
    //     // tft.begin();
    //     tft.init();
    //     tft.setRotation(1);
    //     tft.fillScreen(TFT_WHITE);
    //     tft.setTextColor(TFT_BLACK, TFT_WHITE); // Set text colour to white and background to black
    //     tft.setTextDatum(TC_DATUM);  // Top Centre datum
    //     tft.setTextFont(1);        // Select font 0 which is the Adafruit font
    //     int xpos = tft.width() / 2;  // Centre of screen
    //     // tft.drawString("Original Adafruit font!", xpos, 5);
    //     tft.drawString("Font size 2", xpos, 14, 2); // Draw text centre at position xpos, 14 using font 2
    //     tft.println("Touch corners as indicated");
    // #endif


#ifdef USEBOARD_TTGO_T_PLUS
camera_config_t esp32cam_ttgo_t_plus_config {
    .pin_pwdn = -1,
    .pin_reset = -1,

    .pin_xclk = 4,

    .pin_sscb_sda = 18,
    .pin_sscb_scl = 23,

    .pin_d7 = 36,
    .pin_d6 = 37,
    .pin_d5 = 38,
    .pin_d4 = 39,
    .pin_d3 = 35,
    .pin_d2 = 26,
    .pin_d1 = 13,
    .pin_d0 = 34,
    .pin_vsync = 5,
    .pin_href = 27,
    .pin_pclk = 25,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality = 12, //0-63 lower numbers are higher quality
    .fb_count = 2 // if more than one i2s runs in continous mode.  Use only with jpeg
};

#endif

    int camInit =
#if   defined USEBOARD_TTGO_T
        cam.init(esp32cam_ttgo_t_config);
#elif defined USEBOARD_TTGO_T_PLUS
        cam.init(esp32cam_ttgo_t_plus_config);
#elif defined USEBOARD_AITHINKER
        cam.init(esp32cam_aithinker_config);
#else
        cam.init(esp32cam_config);
#endif

    Serial.printf("Camera init returned %d\n", camInit);

    IPAddress ip;


#ifdef SOFTAP_MODE
    const char *hostname = "devcam";
    // WiFi.hostname(hostname); // FIXME - find out why undefined
    lcdMessage("starting softAP");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    bool result = WiFi.softAP(hostname, "12345678", 1, 0);
    if (!result)
    {
        Serial.println("AP Config failed.");
        return;
    }
    else
    {
        Serial.println("AP Config Success.");
        Serial.print("AP MAC: ");
        Serial.println(WiFi.softAPmacAddress());

        ip = WiFi.softAPIP();
    }
#else
    WiFi.mode(WIFI_MODE_STA);

    #ifdef WIFICLIENT_HARD_CODED
        lcdMessage(ssid);
        
        #ifdef FIXED_IP
            String macAddStr = WiFi.macAddress();

            Serial.print("MAC address: ");
            Serial.println("" + macAddStr + "");

            bool macFound   = false;
            int  numMacs    = sizeof(ipmacs) / sizeof(ipmacs[0]);
            for (int m = 0; m < numMacs; ++m) {
                IpMac currIpMAc = ipmacs[m];
                Serial.print(F("Checking MAC address "));
                Serial.print(m+1);
                Serial.print(F(" / "));
                Serial.print(numMacs);
                Serial.print(F(": "));
                Serial.println(currIpMAc.Mac);
                if(macAddStr == currIpMAc.Mac) {
                    Serial.println("MAC address found!. using fixed IP");
                    Serial.print(F("Setting fixed IP "));
                    Serial.print(currIpMAc.local_ip);
                    Serial.print(F(" Gateway "));
                    Serial.print(currIpMAc.gateway);
                    Serial.print(F(" Subnet "));
                    Serial.println(currIpMAc.subnet);

                    WiFi.config(currIpMAc.local_ip, currIpMAc.gateway, currIpMAc.subnet);
                    macFound = true;
                    break;
                }
            }

            if (! macFound) {
                Serial.println("MAC address not found. using DHCP");
            }
        #endif

        Serial.print(F("Connecting to WiFi '"));
        Serial.print(String(ssid));
        Serial.print("'...");

        WiFi.begin(ssid, password);

        int waitcount = 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            waitcount++;
            if ( waitcount > 20 ) {
                Serial.println("");
                Serial.println("Restarting");
                delay(500);
                ESP.restart();
            }
        }

        Serial.println("");
        Serial.println(F("Connected to the WiFi network"));

        ip = WiFi.localIP();

        Serial.println(F("WiFi connected"));
        Serial.println(ip);
    #else
        WiFi.mode(WIFI_AP_STA);

        AutoWifi a;

        #ifdef FACTORYRESET_BUTTON
            pinMode(FACTORYRESET_BUTTON, INPUT);
            if(!digitalRead(FACTORYRESET_BUTTON))     // 1 means not pressed
                a.resetProvisioning();
                lcdMessage("Reset");
        #endif

        if(!a.isProvisioned())
            lcdMessage("Setup wifi!");
            // lcdMessage(String("!join ") + a.getSSID());
        else
            lcdMessage(String("join ") + a.getSSID());

        a.startWifi();

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(F("."));
        }
        
        ip = WiFi.localIP();
        Serial.println(F("WiFi connected"));
        Serial.println(ip);
    #endif
#endif

    lcdMessage(ip.toString());

    // String macStr = WiFi.macAddress();
    // Serial.print("MAC address: ");
    // Serial.println(macStr);

#ifdef ENABLE_WEBSERVER
    server.on("/", HTTP_GET, handle_jpg_stream);
    server.on("/jpg", HTTP_GET, handle_jpg);
    server.onNotFound(handleNotFound);
    server.begin();
#endif

#ifdef ENABLE_RTSPSERVER
    rtspServer.begin();
#endif
}

CStreamer *streamer;
CRtspSession *session;
WiFiClient client; // FIXME, support multiple clients

void loop()
{
#ifdef ENABLE_WEBSERVER
    server.handleClient();
#endif

#ifdef ENABLE_RTSPSERVER
    uint32_t msecPerFrame = 100;
    static uint32_t lastimage = millis();

    // If we have an active client connection, just service that until gone
    // (FIXME - support multiple simultaneous clients)
    if(session) {
        session->handleRequests(0); // we don't use a timeout here,
        // instead we send only if we have new enough frames

        uint32_t now = millis();
        if(now > lastimage + msecPerFrame || now < lastimage) { // handle clock rollover
            session->broadcastCurrentFrame(now);
            lastimage = now;

            // check if we are overrunning our max frame rate
            now = millis();
            if(now > lastimage + msecPerFrame)
                printf("warning exceeding max frame rate of %d ms\n", now - lastimage);
        }

        if(session->m_stopped) {
            delete session;
            delete streamer;
            session = NULL;
            streamer = NULL;
        }
    }
    else {
        client = rtspServer.accept();

        if(client) {
            //streamer = new SimStreamer(&client, true);             // our streamer for UDP/TCP based RTP transport
            streamer = new OV2640Streamer(&client, cam);             // our streamer for UDP/TCP based RTP transport

            session = new CRtspSession(&client, streamer); // our threads RTSP session and state
        }
    }
#endif
}
