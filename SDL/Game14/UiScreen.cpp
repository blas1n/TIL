#include "UiScreen.h"
#include "InputSystem.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"

UiScreen::UiScreen(Game* inGame)
	: game(inGame),
	font(nullptr),
	title(nullptr),
	background(nullptr),
	buttonOn(nullptr),
	buttonOff(nullptr),
	titlePos(0.0f, 300.0f),
	nextButtonPos(0.0f, 200.0f),
	bgPos(0.0f, 250.0f),
	buttons(),
	state(UiState::EActive) {

	game->PushUi(this);
	font = game->GetFont("Assets/Carlito-Regular.ttf");
	buttonOn = game->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
	buttonOff = game->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

UiScreen::~UiScreen() {
	if (title) {
		title->Unload();
		delete title;
	}

	for (auto button : buttons)
		delete button;

	buttons.clear();
}

void UiScreen::Draw(Shader* shader) {
	if (background)
		DrawTexture(shader, background, bgPos);
	if (title)
		DrawTexture(shader, title, titlePos);

	for (auto button : buttons) {
		Texture* tex = button->GetHighlighted() ? buttonOn : buttonOff;
		DrawTexture(shader, tex, button->GetPosition());
		DrawTexture(shader, button->GetNameTexture(), button->GetPosition());
	}
}

void UiScreen::ProcessInput(const InputState& inputState) {
	if (buttons.empty()) return;
	
	bool isClick = false;

	if (inputState.controllers.size() > 0)
		isClick = inputState.controllers[0].GetButtonState(SDL_CONTROLLER_BUTTON_A) == ButtonState::EReleased;
	else
		isClick = inputState.mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::EReleased;

	auto mousePos = inputState.mouse.GetPosition();
	mousePos.x -= game->GetRenderer()->GetScreenWidth() * 0.5f;
	mousePos.y = game->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

	for (auto button : buttons) {
		const auto isContain = button->ContainsPoint(mousePos);
		button->SetHighlighted(isContain);

		if (isContain && isClick)
			button->OnClick();
	}
}

void UiScreen::Close() {
	state = UiState::EClosing;
}

void UiScreen::SetTitle(const std::string& text, const Vector3& color /*= Color::White*/, int pointSize /*= 40*/) {
	if (title) {
		title->Unload();
		delete title;
		title = nullptr;
	}

	title = font->RenderText(text, color, pointSize);
}

void UiScreen::AddButton(const std::string& name, std::function<void()> onClick) {
	Vector2 dims{
		static_cast<float>(buttonOn->GetWidth()),
		static_cast<float>(buttonOn->GetHeight()),
	};

	auto button = new Button(name, font, onClick, nextButtonPos, dims);
	buttons.emplace_back(button);

	nextButtonPos.y -= buttonOff->GetHeight() + 20.0f;
}

void UiScreen::DrawTexture(Shader* shader, Texture* texture,
	const Vector2& offset /*= Vector2::Zero*/, float scale /*= 1.0f*/, bool flipY /*= false*/) {

	const auto yScale = texture->GetHeight() * scale;

	const auto scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		flipY ? yScale * -1.0f : yScale,
		1.0f
	);

	const auto transMat = Matrix4::CreateTranslation(Vector3{ offset.x, offset.y, 0.0f });
	
	shader->SetUniformValue("uWorldTransform", scaleMat * transMat);
	texture->SetActive();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Button::Button(const std::string& inName, class Font* inFont,
	std::function<void()> inOnClick,
	const Vector2& inPosition, const Vector2& inDimensions)
	: onClick(inOnClick),
	name(inName),
	nameTexture(nullptr),
	font(inFont),
	position(inPosition),
	dimensions(inDimensions),
	highlighted(false) {

	SetName(name);
}

Button::~Button() {
	if (nameTexture) {
		nameTexture->Unload();
		delete nameTexture;
	}
}

void Button::SetName(const std::string& inName) {
	name = inName;

	if (nameTexture) {
		nameTexture->Unload();
		delete nameTexture;
		nameTexture = nullptr;
	}

	nameTexture = font->RenderText(name);
}

bool Button::ContainsPoint(const Vector2& point) const {
	return
		point.x >= (position.x - dimensions.x * 0.5f) &&
		point.x <= (position.x + dimensions.x * 0.5f) &&
		point.y >= (position.y - dimensions.y * 0.5f) &&
		point.y <= (position.y + dimensions.y * 0.5f);
}

void Button::OnClick() {
	if (onClick)
		onClick();
}