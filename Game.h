#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>

struct Vector2 {
	float x;
	float y;
};

class Game
{
public:
	Game();
	// Init the game
	bool Initialize();
	// Run game loop until it is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Create window with SDL
	SDL_Window* m_Window;
	// Create renderer with SDL
	SDL_Renderer* m_Renderer;
	// Keep game running
	bool m_IsRunning;
	// Ball position
	//Vector2 m_BallPos;
	// Paddle position
	Vector2 m_Paddle1Pos;
	Vector2 m_Paddle2Pos;
	// Get ticks since last
	Uint32 m_TicksCount;
	// Paddle Movement
	int m_Paddle1Dir;
	int m_Paddle2Dir;
	// Ball movement

	struct Ball {
		Vector2 m_BallPos;
		Vector2 m_BallVel;
		bool m_IsActive;
		int m_TimeOut;	
		} ball;

	// Keep Score
	int m_P1Score;
	int m_P2Score;
	
	
};