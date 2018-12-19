#include "Game.h"
const int thiccness = 15;
const float paddleH = 100.0f;

Game::Game() :
	m_IsRunning(true),
	m_Window(nullptr),
	m_Renderer(nullptr),
	m_TicksCount(0),
	m_Paddle1Dir(0),
	m_Paddle2Dir(0),
	m_P1Score(0),
	m_P2Score(0)
	{}


bool Game::Initialize() {

	int sdlResult = SDL_Init(SDL_INIT_VIDEO); // SDL_Init returns 0 for a successful initialization, and nonzero for a failure
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	// Create Window
	m_Window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // 1st parameter: window title
		100, // 2nd parameter: x-pos of top left corner
		100, // 3rd parameter: y-pos of top left corner
		1024, // 4th parameter: width
		768, // 5th parameter: height
		0 // 6th parameter: flags, 0 means no flags set
	);

	// Create Renderer
	m_Renderer = SDL_CreateRenderer(
		m_Window,												// 1st parameter: The pointer to the window to create renderer for
		-1,														// 2nd parameter: Which graphics driver to user (-1 is let SDL decide)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC	// 3rd parameter: Flags
	);

	if (!m_Window) { // if SDL_CreateWindow fails, it returns nullptr (which would be NULL, 0, or false). (!m_Window) would be (!false) or (true), which would make the if statement execute
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Init ball position to center
	ball.m_BallPos.x = (1024.0f / 2.0f);
	ball.m_BallPos.y = (768.0f / 2.0f);
	ball.m_BallVel.x = -200.0f;
	ball.m_BallVel.y = 235.0f;

	// Init Ball w/ Active Status
	ball.m_IsActive = true;
	ball.m_TimeOut = 0;

	


	// Init paddle y to center, paddle x to center of thiccness on left edge of screen
	m_Paddle1Pos.x = thiccness / 2.0f;
	m_Paddle1Pos.y = 768.0f / 2.0f;
	m_Paddle2Pos.x = 1024 - thiccness / 2.0f;
	m_Paddle2Pos.y = 768.0f / 2.0f;
	return true;
}

void Game::RunLoop() {
	while (m_IsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::Shutdown() {
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	SDL_Quit();
}
void Game::ProcessInput() {
	SDL_Event event;


	// Check if user clicks X on window
	while (SDL_PollEvent(&event)) { // While there are events in the queue
		switch (event.type) {		// .type  returns the type of event. If it is SDL_QUIT (x button clicked), m_IsRunning is set to false and the program closes
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		}
	}

	// Create a pointer named state that points to an array of the state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// Check if user hits esc on keyboard
	if (state[SDL_SCANCODE_ESCAPE]) {
		m_IsRunning = false;
	}

	// Check for paddle movement, up with W key, down with S key
	// It is "-= and +=" not " =" because this is moving the paddle up and down, not setting the y pos to 1 or -1!
	m_Paddle1Dir = 0;
	//m_Paddle2Dir = 0;
	if (state[SDL_SCANCODE_W]) {
		m_Paddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		m_Paddle1Dir += 1;
	}


	if (state[SDL_SCANCODE_I]) {
		m_Paddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K]){
		m_Paddle2Dir += 1;
	}
	
	
	// CPU Paddle 2 Movement
	// Make CPU follow ball
	if (ball.m_IsActive) {

		if ((ball.m_BallPos.y > m_Paddle2Pos.y - paddleH / 2) && (ball.m_BallPos.y < m_Paddle2Pos.y + paddleH / 2)) {
			m_Paddle2Dir = 0;
		}
		else if (ball.m_BallPos.y > m_Paddle2Pos.y) { // moving up
			m_Paddle2Dir += 1;
		}
		else if (ball.m_BallPos.y < m_Paddle2Pos.y) { // moving up
			m_Paddle2Dir -= 1;
		}
	}
}



void Game::UpdateGame() {

	// SDL_TICKS_PASSED: once parameter A >= parameter B, returns true
	// The while loop condition is !false until the current tick count is greater than the old tick count by 16ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16)) {}

	// deltaTime is the difference in ticks since the last frame. All of this says how much time has elapsed since the exact same location of the previous frame.
	// (converted from milliseconds to seconds)
	float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;

	// Prevent tick count from continuing when paused
	while (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Next, update tick count for next frame
	m_TicksCount = SDL_GetTicks();

	// Update paddle position, don't allow paddle to move offscreen
	m_Paddle1Pos.y += m_Paddle1Dir * 300.0f * deltaTime; // This 300.0f is just to adjust the speed of the paddle movement
	m_Paddle2Pos.y += m_Paddle2Dir * 300.0f * deltaTime;
	// Paddle 1
	if (m_Paddle1Pos.y < thiccness + (paddleH / 2.0f)) { // PaddlePos is the center of the Paddle, not the top left corner like the SDL_Rect
		m_Paddle1Pos.y = thiccness + (paddleH / 2.0f);
	}
	if (m_Paddle1Pos.y > (768 - thiccness - paddleH / 2.0f)) {
		m_Paddle1Pos.y = (768 - thiccness - paddleH / 2.0f);
	}

	// Paddle 2
	if (m_Paddle2Pos.y < thiccness + (paddleH / 2.0f)) {
		m_Paddle2Pos.y = thiccness + (paddleH / 2.0f);
	}
	if (m_Paddle2Pos.y > (768 - thiccness - paddleH / 2.0f)) {
		m_Paddle2Pos.y = (768 - thiccness - paddleH / 2.0f);
	}


	if (!ball.m_IsActive) {
		ball.m_TimeOut++;
		if (ball.m_TimeOut > 300) {
			ball.m_IsActive = true;
			ball.m_BallVel.x = 0;
			ball.m_BallVel.y = 0;
			ball.m_TimeOut = 0;
		}
	}

	// Update Ball Position
		if (ball.m_IsActive) 
		{
			ball.m_BallPos.x += ball.m_BallVel.x * deltaTime;
			ball.m_BallPos.y += ball.m_BallVel.y * deltaTime;
		}


	// Update ball position and check for collision
	int Paddle1Top = m_Paddle1Pos.y + paddleH / 2.0f;
	int Paddle1Bottom = m_Paddle1Pos.y - paddleH / 2.0f;
	int Paddle2Top = m_Paddle2Pos.y + paddleH / 2.0f;
	int Paddle2Bottom = m_Paddle2Pos.y - paddleH / 2.0f;

	// Check Collision - w/ walls, paddles, and other balls
	if (((ball.m_BallPos.x <= thiccness + thiccness / 2.0f) && ((ball.m_BallPos.y <= Paddle1Top) && (ball.m_BallPos.y >= Paddle1Bottom)) && (ball.m_BallVel.x < 0.0f))) { // Paddle 1
		ball.m_BallPos.x = thiccness + thiccness / 2.0f;
		ball.m_BallVel.x *= -1;
	}
	if (((ball.m_BallPos.x >= 1024 - thiccness - thiccness / 2.0f) && ((ball.m_BallPos.y <= Paddle2Top) && (ball.m_BallPos.y >= Paddle2Bottom)) && (ball.m_BallVel.x > 0.0f))) { // Paddle 2
		ball.m_BallPos.x = 1024 - thiccness - thiccness / 2.0f;
		ball.m_BallVel.x *= -1;
	}
	if ((ball.m_BallPos.y <= thiccness + thiccness / 2.0f) && (ball.m_BallVel.y < 0.0f)) {  // Top Wall
		ball.m_BallPos.y = thiccness + thiccness / 2.0f;
		ball.m_BallVel.y *= -1;
	}
	if ((ball.m_BallPos.y >= (768 - thiccness - thiccness / 2.0f)) && (ball.m_BallVel.y > 0.0f)) {  // Bottom Wall
		ball.m_BallPos.y = (768 - thiccness - thiccness / 2.0f);
		ball.m_BallVel.y *= -1;
	}

	// Scoring
	if ((ball.m_BallPos.x < 0) && (ball.m_BallVel.x < 0)) 	// Check left side
	{
		ball.m_BallPos.x = 1024 / 2;
		ball.m_BallPos.y = 768 / 2;
		ball.m_BallVel.x = 0;
		ball.m_BallVel.y = 0;
		ball.m_IsActive = false;
		m_P2Score++;	
		std::cout << "P1 Score: " << m_P1Score << "     P2 Score: " << m_P2Score << std::endl;
	}
		

	if ((ball.m_BallPos.x > 1024) && (ball.m_BallVel.x > 0)) 
	{	// Check right side
		ball.m_BallPos.x = 1024 / 2;
		ball.m_BallPos.y = 768 / 2;
		int tempX = ball.m_BallVel.x;
		int tempY = ball.m_BallVel.y;
		ball.m_BallVel.x = 0;
		ball.m_BallVel.y = 0;
		ball.m_IsActive = false;
		m_P1Score++;
		std::cout << "P1 Score: " << m_P1Score << "     P2 Score: " << m_P2Score << std::endl;
	}
	
	
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(
		m_Renderer,			// Name of renderer pointer
		13,					// R
		83,					// G
		0,					// B
		255					// A
	);
	SDL_RenderClear(m_Renderer);	// Clear back buffer (clear old buffer before drawing the new one)


									// Game should be drawn after the back buffer is cleared, but before the front and back buffers are swapped
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	SDL_Rect TopWall{
		0,				// x pos of top left corner
		0,				// y pos of top left corner
		1024,			// width
		thiccness		// height
	};
	SDL_Rect BottomWall{
		0,
		768 - thiccness,
		1024,
		thiccness
	};
	SDL_Rect theBall{
		// SDL_Rect objects drawn by top left x and y coordinate, but BallPos gives center of ball. Have to account for that.
		// static_cast used to convert the floats to ints, because SDL_Rect requires ints as parameters
		static_cast<int>(ball.m_BallPos.x - (thiccness / 2)),
		static_cast<int>(ball.m_BallPos.y - (thiccness / 2)),
		thiccness,
		thiccness
	};
	SDL_Rect Paddle1{
		static_cast<int>(m_Paddle1Pos.x - thiccness / 2),
		static_cast<int>(m_Paddle1Pos.y - (paddleH / 2)),
		thiccness,
		paddleH
	};
	SDL_Rect Paddle2{
		static_cast<int>(m_Paddle2Pos.x - thiccness / 2),
		static_cast<int>(m_Paddle2Pos.y - (paddleH / 2)),
		thiccness,
		paddleH
	};
	SDL_RenderFillRect(m_Renderer, &TopWall);
	SDL_RenderFillRect(m_Renderer, &BottomWall);
	SDL_RenderFillRect(m_Renderer, &Paddle1);
	SDL_RenderFillRect(m_Renderer, &Paddle2);
	if (ball.m_IsActive) {
		SDL_RenderFillRect(m_Renderer, &theBall);
	}

	SDL_RenderPresent(m_Renderer);  // Swap front and back buffers 

}