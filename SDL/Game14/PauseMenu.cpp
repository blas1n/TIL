#include "PauseMenu.h"
#include <SDL/SDL.h>
#include "Game.h"
#include "InputSystem.h"
#include "DialogBox.h"

PauseMenu::PauseMenu(Game* inGame)
	: UiScreen(inGame) {

	game->SetState(Game::GameState::EPaused);
	game->GetInputSystem()->SetRelativeMouseMode(false);
	SetTitle("PauseTitle");
	
	AddButton("ResumeButton", [this] {
		Close();
	});

	AddButton("QuitButton", [this] {
		new DialogBox(game, "QuitText", [this] {
			game->SetState(Game::GameState::EQuit);
		});
	});
}

PauseMenu::~PauseMenu() {
	game->GetInputSystem()->SetRelativeMouseMode(true);
	game->SetState(Game::GameState::EGameplay);
}

void PauseMenu::ProcessInput(const InputState& inputState) {
	UiScreen::ProcessInput(inputState);
	
	if (inputState.keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::EReleased)
		Close();
}