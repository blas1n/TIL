#pragma once

#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class Button {
public:
	Button(const std::string& inName, class Font* inFont,
		std::function<void()> inOnClick,
		const Vector2& inPosition, const Vector2& inDimensions);
	~Button();

	void SetName(const std::string& inName);

	bool ContainsPoint(const Vector2& point) const;
	void OnClick();

	inline class Texture* GetNameTexture() { return nameTexture; }
	inline const Vector2& GetPosition() const { return position; }

	inline bool GetHighlighted() const { return highlighted; }
	inline void SetHighlighted(bool sel) { highlighted = sel; }

private:
	std::function<void()> onClick;
	std::string name;

	Texture* nameTexture;
	Font* font;

	Vector2 position;
	Vector2 dimensions;

	bool highlighted;
};

class UiScreen {
public:
	UiScreen(class Game* inGame);
	virtual ~UiScreen();

	virtual void Update(float deltaTime) {}
	virtual void Draw(class Shader* shader);
	virtual void ProcessInput(const struct InputState& inputState);
	
	enum class UiState {
		EActive,
		EClosing
	};

	void Close();

	inline UiState GetState() const { return state; };
	
	void SetTitle(const std::string& text,
		const Vector3& color = Color::White,
		int pointSize = 40);

	void AddButton(const std::string& name, std::function<void()> onClick);

protected:
	void DrawTexture(Shader* shader, class Texture* texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f, bool flipY = false);

	Game* game;

	class Font* font;

	Texture* title;
	Texture* background;
	Texture* buttonOn;
	Texture* buttonOff;

	Vector2 titlePos;
	Vector2 nextButtonPos;
	Vector2 bgPos;

	std::vector<Button*> buttons;
	UiState state;
};