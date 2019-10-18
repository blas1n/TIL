#pragma once

#include "UiScreen.h"

class DialogBox : public UiScreen {
public:
	DialogBox(Game* inGame, const std::string& text, std::function<void()> onOK);
};