// スマトコスケッチ
// ESPr Developer（ESP-WROOM-02開発ボード）
// https://www.switch-science.com/catalog/2500/
// TFT液晶with基板 [M-Z18SPI-2P]
// http://www.aitendo.com/product/11590
// 参考サイト:ESP8266にカラーTFT液晶をSPI接続してみる
// http://blog.boochow.com/article/425389092.html

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <ESP8266WiFi.h>

// プッシュスイッチのIOポート番号
#define SW_PIN 0

// TFT設定関連
#define TFT_CS     15
#define TFT_RST    5
#define TFT_DC     4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#define TFT_SCLK 13
#define TFT_MOSI 11

// タイマー残秒数
int16_t timer_remain;

// タイマーカウントダウン可否
bool enable;

void setup() {
  // シリアルボーレート設定
  Serial.begin(115200);

  // 初期化開始
  Serial.print("Setup start");

  // Wi-Fi無効
  WiFi.mode(WIFI_OFF);

  // IO設定(スイッチなのでINPUT)
  pinMode(SW_PIN, INPUT);

  // タイマーカウントダウン不許可
  enable = false;

  // TFT初期化
  tft.initR(INITR_BLUETAB);   // initialize a ST7735S chip, black tab
  tft.setTextWrap(false); // Allow text to run off right edge
  tft.fillScreen(ST7735_BLACK);

  // タイマー残秒数 2時間
  timer_remain = 2 * 60 * 60;

  // TFTに初期化画面を描画する
  tft.fillScreen(ST7735_BLUE);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(3);
  tft.println("Press");
  tft.println("Switch");

  // 初期化終了
  Serial.print("Setup end");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(SW_PIN) == 0) {
    // スイッチが押されているのでトグルする
    if (enable) {
      enable = false;
    } else {
      enable = true;
    }
  }

  if (enable) {
    // タイマー許可
    if (timer_remain >= 0) {
      // 現カウントを表示する
      int16_t hh = timer_remain / 60 /60;
      int16_t mm = timer_remain / 60 - (hh * 60);
      int16_t ss =  timer_remain - (mm * 60) - (hh * 60 * 60);
      char strTime[9];

      // 画面消す
      tft.fillScreen(ST7735_BLUE);
      // 初期位置設定
      tft.setCursor(0, 30);

      // 文字初期を白
      tft.setTextColor(ST7735_WHITE);

      // 文字サイズ：2
      tft.setTextSize(2);

      // 文字描画
      tft.println("Time=");

      ////////////////////////////////
      // 文字サイズ：4
      tft.setTextSize(4);
      tft.print(" ");
      // 文字サイズ：5
      tft.setTextSize(5);
      sprintf(strTime, "%02d", hh); 
      tft.println(strTime);
      ////////////////////////////////
      // 文字サイズ：4
      tft.setTextSize(4);
      tft.print(":");
      // 文字サイズ：5
      tft.setTextSize(5);
      sprintf(strTime, "%02d", mm); 
      tft.println(strTime);
      ////////////////////////////////
      // 文字サイズ：4
      tft.setTextSize(4);
      tft.print(":");
      // 文字サイズ：5
      tft.setTextSize(5);
      sprintf(strTime, "%02d", ss); 
      tft.println(strTime);
      // 1秒待つ
      delay(1000);
    }
    if (timer_remain != 0) {
      // まだ残時間があるので残時間を減算
      timer_remain -= 1;
    }
  } else {
    // 500ms待つ
    delay(500);
  }
}
