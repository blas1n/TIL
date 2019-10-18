#include "DialogBox.h"
#include <SDL/SDL.h>
#include "Game.h"
#include "Renderer.h"

DialogBox::DialogBox(Game* inGame, const std::string& text, std::function<void()> onOK)
	: UiScreen(inGame) {

	bgPos = Vector2::Zero;
	titlePos = Vector2{ 0.0f, 100.0f };
	nextButtonPos = Vector2::Zero;

	background = game->GetRenderer()->GetTexture("Assets/DialogBG.png");
	SetTitle(text, Vector3::Zero, 30);
	
	AddButton("OKButton", [onOK] {
		onOK();
	});

	AddButton("CancelButton", [this] {
		Close();
	});
}