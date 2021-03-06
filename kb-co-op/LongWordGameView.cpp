#include "LongWordGameView.h"
// #include <ctype.h>

void LongWordGameView::setLongWordDelegate(LongWordDelegate *newDel) {
  longWordDelegate = newDel;
}

void LongWordGameView::CommonInit() {
  
}

LongWordGameView::LongWordGameView(GameModeDelegate *del) : GameView(del) {
  CommonInit();
}

void LongWordGameView::keyPressed(char c, int upperChar, bool isP1) {

}

// FONTS
// https://github.com/olikraus/u8g2/wiki/fntgrpgeoff#tenstamps
// https://github.com/olikraus/u8g2/wiki/fntgrpgeoff#tenthinnerguys
// FONT MODE
// https://github.com/olikraus/u8g2/wiki/u8g2reference#setfontmode
void LongWordGameView::draw(U8G2_ST7920_128X64_1_SW_SPI *disp, bool isDisp1) {

  
//   char *charWindow = viewDelegate->getCharacterWindow(isDisp1);
  char* curWord = longWordDelegate->getCurrentWord();
//  char* curWord = "LONGWORD";
  
  // extern const uint8_t u8g2_font_tenstamps_mu[] U8G2_FONT_SECTION("u8g2_font_tenstamps_mu");
  disp->setDrawColor(1);
  disp->setFontPosTop();
  disp->setFont(u8g2_font_tenstamps_mu); // height=15,mu=monospace,uppercase
  // disp->drawStr(4,22,"LONGWORD");
  disp->drawStr(4,22,curWord);

  if (!isDisp1) {
    drawNHeartsHeight8(disp, 12);
  } else {
    drawNHearts(disp, 7);
  }

  drawTimeCounter(disp, 3); // secsRemaining=8

  // disp->setFont(u8g2_font_open_iconic_human_2x_t);
  // disp->setDrawColor(1);
  // disp.drawGlyph(1, 1, 0x0042);
  // // disp->drawStr(1,1,"B  B  B"); // 66=heart='B'=0x0042
  // disp->setDrawColor(0);
  // disp->drawStr(72,4,"B  B");

  // disp->setFont(u8g2_font_open_iconic_human_2x_t); // height=16
  // disp->setFont(u8g2_font_open_iconic_human_4x_t); // height=32
  // disp->setFont(u8g2_font_open_iconic_human_6x_t); // height=48
  // disp->setFont(u8g2_font_open_iconic_human_8x_t); // height=64

  // disp->setFont(u8g2_font_open_iconic_arrow_4x_t); // height=32
  // disp->setFont(u8g2_font_open_iconic_check_4x_t); // height=32

  // disp->setFont(u8g2_font_7Segments_26x42_mn); // height=42
  // disp->setFont(u8g2_font_iconquadpix_m_all); // height=42
  
  
  
  
}

// LongWordGameView::LongWordGameView(bool btn1On, bool btn2On) : _btn1State(btn1On), _btn2State(btn2On) {
//   CommonInit();
// }

// --------------------
