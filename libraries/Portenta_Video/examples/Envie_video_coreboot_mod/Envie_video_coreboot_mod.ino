#include "Portenta_Video.h"
#include "image_320x240_argb8888.h"
#include "SDRAM.h"
#include "mbed.h"

struct edid recognized_edid;

mbed::DigitalOut video_on(PK_2);
mbed::DigitalOut video_rst(PJ_3);

void spettacolino() {
  int i;

  for(i=0; i<3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void clearAndHome(void)
{
  Serial1.write(0x1b); // ESC
  Serial1.print(F("[2J")); // clear screen
  Serial1.write(0x1b); // ESC
  Serial1.print(F("[H")); // cursor to home
}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  printf("%s %d\n", __func__, __LINE__);
  spettacolino();
  clearAndHome();

  delay(1000);
  video_on = 1;
  delay(10);
  video_rst = 1;
  delay(10);

  int ret = -1;
  video_on = 0;
  delay(10);
  video_rst = 0;
  delay(100);
  while (ret < 0) {

    video_on = 0;
    delay(10);
    video_rst = 0;
    delay(100);

    video_on = 1;
    delay(100);
    video_rst = 1;

    printf("%s %d\n", __func__, __LINE__);
    ret = anx7625_init(0);
    printf("%s %d\n", __func__, __LINE__);
  }

  delay(100);
  printf("%s %d\n", __func__, __LINE__);
  anx7625_dp_get_edid(0, &recognized_edid);
  printf("%s %d\n", __func__, __LINE__);
  anx7625_dp_start(0, &recognized_edid, EDID_MODE_640x480_60Hz);
  printf("%s %d\n", __func__, __LINE__);
  delay(100);

  SDRAM.begin(getFramebufferEnd());
  while (1) {
    stm32_LCD_DrawImage((void*)texture_raw, (void *)getNextFrameBuffer(), 300, 300, DMA2D_INPUT_RGB565);
    stm32_LCD_DrawImage((void*)texture_raw, (void *)getNextFrameBuffer(), 300, 300, DMA2D_INPUT_RGB565);
  }
  printf("%s %d\n", __func__, __LINE__);
}

int i = 0;

void loop() {
  delay(1000);
  printf("%s %d\n", __func__, __LINE__);

  i = random(0, 0xFFFFFF);
  printf("now: %d\n", millis());

  delay(1000);
}
