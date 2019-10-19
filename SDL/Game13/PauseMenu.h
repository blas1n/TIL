#pragma once

#include "UiScreen.h"

class PauseMenu : public UiScreen
{
public:
	PauseMenu(Game* inGame);
	~PauseMenu();

	void ProcessInput(const InputState& inputState) override;
};