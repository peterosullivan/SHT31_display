#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0); // hardware
//U8G2_SSD1306_64X48_ER_F_SW_I2C u8g2(U8G2_R0, D1, D2); // software


/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup(void) {
  u8g2.begin();
}

/* draw something on the display with the `firstPage()`/`nextPage()` loop*/
void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(1000);
}
