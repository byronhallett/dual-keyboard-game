#include <U8g2lib.h>
#include <usbhub.h>

#include "HIDKeyboardParser.h"
#include "GameState.h"

// pin definitions - change these to match your wiring
// E =  Green
// RW = Yellow (-> Blue for 1 display)
// RS = Orange
static const PROGMEM uint8_t D1_PIN_E = 11, D1_PIN_RW = 8, D1_PIN_RS = 5;
static const PROGMEM uint8_t D2_PIN_E = 7, D2_PIN_RW = 6, D2_PIN_RS = 9;
static const PROGMEM uint8_t CS_PIN_SD = 4, CS_PIN_USB = 3;

// const int SWITCH_PIN_1 = 9, SWITCH_PIN_2 = 10;

// ------------------------------
// USB KEYBOARD
USB Usb;
int lastUSBTaskState = 0;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);
KeyEvents keyEvents;
HIDKeyboardParser Prs(&keyEvents);

// ------------------------------
// DISPLAYS
// https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#constructor-name
// Prefix:     U8G2
// Display:    ST7920
// DispName:   128X64
// BufferSize: 1 (1,2 or F (full frame buffer))
// Commun:     SW_SPI (Software emulated, SPI (Serial))

//                           ...(rotation, clock, data, cs [, reset])
U8G2_ST7920_128X64_1_SW_SPI disp1(U8G2_R0, D1_PIN_E, D1_PIN_RW, D1_PIN_RS);
U8G2_ST7920_128X64_1_SW_SPI disp2(U8G2_R0, D2_PIN_E, D2_PIN_RW, D2_PIN_RS);
// ------------------------------

// =========
GameState game;
// =========

//SPISettings spiSettings(2e6, MSBFIRST, SPI_MODE3); // 2 MHz, mode 3

void setSPIMode(bool SDActive) {
  digitalWrite(CS_PIN_SD,  SDActive ? LOW : HIGH); // HIGH is INACTIVE
  digitalWrite(CS_PIN_USB, SDActive ? LOW : HIGH); // LOW  is   ACTIVE
}

void setup() {

  delay(1000);
  Serial.begin(115200);
  while (!Serial) yield();
  delay( 200 );
  
  // initialize PIN MODES
//  pinMode(CS_PIN_SD, OUTPUT);
//  pinMode(CS_PIN_USB, OUTPUT);
//  digitalWrite(10, HIGH);
//  digitalWrite(CS_PIN_SD, HIGH);
//  digitalWrite(CS_PIN_USB, LOW);
  // pinMode(SWITCH_PIN_1, INPUT);
  // pinMode(SWITCH_PIN_2, INPUT);

  Words::InitSDCard(CS_PIN_SD);
  
  game.SetupGameModes();
  game.update();
//  game = GameState(digitalRead(SWITCH_PIN_1), digitalRead(SWITCH_PIN_2));

  // * * * * * * * * * * * * * * * *
//  setSPIMode(true); // SDActive=true
  // * * * * * * * * * * * * * * * *

  
//  String randWord = game.getRandBalanced8CharWord();
//  Serial.print("RANDOM WORD: ");
//  Serial.println(randWord);

  // ----------------------------------
  Serial.println("Keyboard Setup Start...");
  // Keyboard setup

  // * * * * * * * * * * * * * * * *
//  setSPIMode(false); // SDActive=false
  // * * * * * * * * * * * * * * * *
 
  if (Usb.Init() == -1) { Serial.println("USB OSC did not start."); }
  delay( 200 );
  HidKeyboard.SetReportParser(0, &Prs);

  // WAIT UNTIL THE USB Task State is RUNNING...
  Serial.println("WAIT until USB is RUNNING...");
  int newUSBState = 0;
  while (lastUSBTaskState != 144) { // USB_STATE_RUNNING = 144
    Usb.Task();
    newUSBState = Usb.getUsbTaskState();
    if (newUSBState != lastUSBTaskState) { printUSBTaskState(lastUSBTaskState); }
    lastUSBTaskState = newUSBState;
  }
  Serial.println("USB.Init() finished (USB_STATE_RUNNING)");
  // ----------------------------------

  // Displays setup
  Serial.println("DISPs Setup Start...");
  disp1.begin();
  disp2.begin();
  Serial.println("disp.begin() finished");

  Serial.println("Finished Setup");
  delay( 2000 );
}

// void drawOnDisplay(U8G2_ST7920_128X64_1_SW_SPI disp, bool isDisp1) {
//   //    u8g2.drawLine(c1, r1, c2, r2);   // draw the line from (c1, r1) to (c2, r2)
// //    u8g2.setFont(u8g2_font_ncenB14_tr);

//     // SetFont
//     // ----------
//     // The 2 letters are <purpose><char set>
//     // PURPOSE
//     // t_ - Transparent gylphs with variable width
//     // m_ - Monospace/fixed width glyphs
//     // h_ - Glyphs with variable width and common height
//     // 8_ - Monospace/fixed width glyphs in a 8x8 box
//     // CHAR SET
//     // _f - The font includes up to 256 glyphs.
//     // _r - Only glyphs on the range of the ASCII codes 32 to 127 are included in the font.
//     // _u - Only glyphs on the range of the ASCII codes 32 to 95 (uppercase chars) are included in the font.
//     // _n - Only numbers and extra glyphs for writing date and time strings are included in the font.

//     // Our fonts:
//     // https://github.com/olikraus/u8g2/wiki/fntgrpgeoff#tenstamps
//     // https://github.com/olikraus/u8g2/wiki/fntgrpgeoff#tenthinnerguys
//     // FONT MODE
//     // https://github.com/olikraus/u8g2/wiki/u8g2reference#setfontmode

// //    u8g2.setFont(u8g2_font_helvR24_te); // tf, tr, tn, te?
// //    u8g2.setFont(u8g2_font_helvB18_tf); // tf, tr, tn, te?
//     disp.setFont(u8g2_font_tenthinnerguys_tf);
//     disp.setFontPosTop();
    
// //    disp.drawStr(4,7,line1Str.c_str());
//     disp.drawStr(4,7, !isDisp1 ? "The Best..." : "...Typing Game");
    

// //    u8g2.setFont(u8g2_font_crox5tb_tf); // mf, mr
//     disp.setFont(u8g2_font_tenstamps_mu); // mu=monospace,uppercase
// //    disp.drawStr(4,22,game.line2Str.c_str());
// //    disp.drawStr(4,22,"FIX ME");
//     disp.drawStr(4,22,isDisp1 ? game.player1Str() : game.player2Str() );
// //    disp.drawStr(4,22,game.player1Str());

//     disp.setFont(u8g2_font_tenthinnerguys_tf);
//     disp.drawStr(4,45,"On Display for all");

// //    disp.setFont(u8g2_font_roentgen_nbp_tf);
// //    disp.drawStr(109,0,String(wait_progress).c_str());

// //    disp.setFont(u8g2_font_roentgen_nbp_tf);
// //    disp.drawStr(81,56, ("BTN="+String(isDisp1 ? game.btn2State() : game.btn1State())).c_str());
// }

static void printUSBTaskState (uint8_t state) {
  switch (state) {
    case 16: Serial.println("   USB_DETACHED"); break;
    case 0x11: Serial.println("   USB_INITIALIZE"); break;
    case 18: Serial.println("   USB_WAIT_FOR_DEVICE"); break;
    case 32: Serial.println("   USB_SETTLE"); break;
    case 64: Serial.println("   USB_WAIT_RESET_COMPLETE"); break;
    case 80: Serial.println("   USB_WAIT_SOF"); break;
    case 81: Serial.println("   USB_WAIT_RESET"); break;
    case 144: Serial.println("   USB_RUNNING"); break;
    case 160: Serial.println("   USB_STATE_ERROR"); break;
    default: Serial.print("   USB_UNKNOWN - "); Serial.println(state); break;
  }
}

void loop() {
  
  Usb.Task();
  int newState = Usb.getUsbTaskState();
  if (newState != lastUSBTaskState) {
    printUSBTaskState(newState);  
    lastUSBTaskState = newState;
  }

  
//  Usb.setUsbTaskState(17); // 0x11 - USB_DETACHED_SUBSTATE_INITIALIZE
  

  // -------------------
  // Page buffer mode (Picture Loop)
  
  // ==============================
//  game.update(digitalRead(SWITCH_PIN_1), digitalRead(SWITCH_PIN_2));
  game.update();

  // Never Redraw both displays in the same loop!
  // Let's reserve time for the Usb.Task() to run
  // It's more important that our data is correct, rather than the displays are UpToDate
  // - - - - - - - - - - -
  // TODO: Try different methods to draw the displays...
  
  if (game.shouldRedrawDisp1()) {
//    Serial.println("Draw Display 1");
    game.displayDrawingStateChanged(true, true);
    disp1.firstPage();
    do {
      Usb.Task();
      // drawOnDisplay(disp1, true);
      game.draw(&disp1, true);
    } while ( disp1.nextPage() );
    game.displayDrawingStateChanged(true, false);
  }

  // TODO: If we are running a Usb.Task() during the display draw loop, the data could change, resulting in inconsistent data displayed
  //       We need to store the data while the displays are updating
  
  if (game.shouldRedrawDisp2()) {
//    Serial.println("Draw Display 2");
    game.displayDrawingStateChanged(false, true);
    disp2.firstPage();
    do {
      Usb.Task();
      // drawOnDisplay(disp2, false);
      game.draw(&disp2, false);
    } while ( disp2.nextPage() ); 
    game.displayDrawingStateChanged(false, false);
  }
}

// ---------------------

void KeyEvents::KeyStateChanged(char c,  bool isDown) {
  Serial.print(c);
//  Serial.print(" ");
//  Serial.println((isDown ? "DN":"UP"));
//  int upperC = toupper(c);
//  Serial.print(upperC);
//  Serial.println(c);
  game.keyStateChanged(c, isDown);

//  String randWord = game.getRandBalanced8CharWord();
//  Serial.println(randWord);
}

void KeyEvents::ControlKeyStateChanged(MODIFIERKEYS beforeMod, MODIFIERKEYS afterMod) {
//  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
//    Serial.println("LeftCtrl changed");
//  }
//  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
//    Serial.println("LeftShift changed");
//  }
//  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
//    Serial.println("LeftAlt changed");
//  }
//  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
//    Serial.println("LeftGUI changed");
//  }
//
//  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
//    Serial.println("RightCtrl changed");
//  }
//  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
//    Serial.println("RightShift changed");
//  }
//  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
//    Serial.println("RightAlt changed");
//  }
//  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
//    Serial.println("RightGUI changed");
//  }
}

// ---------------------
