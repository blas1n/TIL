#pragma once

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

private:
	struct SDL_Window* window;
	bool isRunning;
};

