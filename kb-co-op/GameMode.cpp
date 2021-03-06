#include "GameMode.h"
// #include <ctype.h>

void GameMode::SetupDelegates() {

}

void GameMode::CommonInit() {
  // views = new GameView[1];
  views = new GameView*[1];
  views[0] = new GameView(modeDelegate);
  currentView = views[0];

  // wordIndexHistory = new int[HISTORY_SIZE];
}

GameMode::GameMode(GameModeDelegate *del) : modeDelegate(del) {
  CommonInit();
}

void GameMode::ResetGameMode() {
  
}

void GameMode::keyPressed(char c, int upperChar, bool isP1) {
  if (currentView == NULL) { return; }

  currentView->keyPressed(c, upperChar, isP1);
}

void GameMode::draw(U8G2_ST7920_128X64_1_SW_SPI *disp, bool isDisp1) {
  if (currentView == NULL) { return; }

  currentView->draw(disp, isDisp1);
}

// GameView::GameView(bool btn1On, bool btn2On) : _btn1State(btn1On), _btn2State(btn2On) {
//   CommonInit();
// }

// --------------------
