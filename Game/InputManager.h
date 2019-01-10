#pragma once
#include "Game.h"

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void);

// 生キーが押されているか
bool IsRawKeyDown(int key);

// 生キーが離されているか
bool IsRawKeyUp(int key);

// 生キーを押した直後か
bool IsRawKeyPressed(int key);

// 生キーを離した直後か
bool IsRawKeyReleased(int key);

// キーが押されているか
bool IsKeyDown(int key);

// キーが離されているか
bool IsKeyUp(int key);

// キーを押した直後か
bool IsKeyPressed(int key);

// キーを離した直後か
bool IsKeyReleased(int key);
