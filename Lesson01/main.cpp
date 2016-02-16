//#define paddle_game
#define plane_game
//#define not_paddle_game
//#define lesson7

#if defined not_paddle_game

#include <SDL.h>
#include <iostream>

//using namespace std;

void toggleFullscreen(SDL_Window* window, SDL_Renderer* renderer, bool &isFullscreen, int &width, int &height)
{
	SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!isFullscreen)
		SDL_SetWindowSize(window, 800, 600); //or 1024x768

	isFullscreen = !isFullscreen; //toggle isFullscreen
	SDL_GetRendererOutputSize(renderer, &width, &height);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


/*
void toggleFullscreen(SDL_Window* window, SDL_Renderer* renderer, bool &isFullscreen, int &width, int &height)
{
	SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!isFullscreen)
		SDL_SetWindowSize(window, 1920, 1080);

	//storeScreenDimensions(renderer, windowDimensions.w, windowDimensions.h);
	isFullscreen = !isFullscreen; //toggle isFullscreen

	SDL_GetRendererOutputSize(renderer, &width, &height);
}
*/
int main(int argc, char ** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Event event;
	bool quit = false;
	int counter = 0;
	bool isFullscreen = true;

	int r, g, b;
	r = g = b = 0;

	int movementStep = 5;

	int width = 0;
	int height = 0;

	//SDL_Renderer* renderer = nullptr;
	SDL_Rect rect = {};
	rect.x = rect.y = 200;
	rect.w = 500;
	rect.h = 300;

	SDL_Rect windowDimensions = {};

	int buttonCount = 0;
	//SDL_Window * screen = SDL_CreateWindow("My First Window",
	//	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
	//	SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	SDL_Window * screen = SDL_CreateWindow("My SDL Empty Window",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 680,
		SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS);

	SDL_Renderer * renderer = SDL_CreateRenderer(screen, -1, 0);

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);

	SDL_GetRendererOutputSize(renderer, &width, &height);
	std::cout << "width,height:" << width << "," << height
		<< std::endl;

	//SDL_SetWindowSize(screen, 1920, 1080);

	while (!quit)
	{
		if (buttonCount % 3 == 0)
		{
			r = 255;
			g = 0;
			b = 0;
		}
		else if (buttonCount % 3 == 1)
		{
			r = 0;
			g = 255;
			b = 0;
		}
		else if (buttonCount % 3 == 2)
		{
			r = 0;
			g = 0;
			b = 255;
		}
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);

		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				rect.y -= movementStep;
				//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				break;
				case SDLK_DOWN:
				rect.y += movementStep;
				//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				break;
			case SDLK_LEFT:
				rect.x -= movementStep;
				//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				break;
			case SDLK_RIGHT:
				rect.x += movementStep;
				//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			}// End of switch
			if (event.key.keysym.mod & KMOD_CTRL)
			{
				if (event.key.keysym.sym == SDLK_q) quit = true;
				if (event.key.keysym.sym == SDLK_f)
				{	
					toggleFullscreen(screen, renderer, isFullscreen, windowDimensions.w, windowDimensions.h);					
					/*
					if (isFullscreen)
					{
						SDL_SetWindowFullscreen(screen, 0);
						SDL_SetWindowSize(screen, 1920, 1080);
						isFullscreen = false;
						std::cout << "fullscreen" << std::endl;
					}
					else
					{
						SDL_SetWindowFullscreen(screen, SDL_WINDOW_FULLSCREEN_DESKTOP);
						isFullscreen = true;
						std::cout << "windowed" << std::endl;
					}
					*/
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
						SDL_SetRenderDrawColor(renderer, r, g, b, 255);
						++buttonCount;
						std::cout << buttonCount << ": " << r << "," << g << "," << b << std::endl;
						break;
				}
			break;
			case SDL_QUIT:
				quit = true;
				std::cout << "quit";
				break;
		}
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);

		std::cout << "windowDimensions: " << windowDimensions.w << "," << windowDimensions.h << std::endl;

		//std::cout << counter << std::endl;
		//counter++;
	}

	//SDL_Delay(3000);

	SDL_Quit();

	return 0;
}
#elif defined paddle_game
//****************Brick and ball game starts here****************

#include <SDL.h>
#include <iostream>
#include "add.h"
#include <SDL_mixer.h>
#include <SDL_image.h>

//using namespace std;

int main(int argc, char ** argv)
{
	//Demo of splitting code into multiple .cpp and .h files
	std::cout << "add 5 and 7: " << add(5, 7) << std::endl;

	bool quit = false;
	bool isBallLaunched = false;
	bool isBallHit = false;
	//bool isBallControllable = true;
	int counter = 0;

	int r, g, b;
	r = g = b = 0;

	const int PADDLE_VELOCITY = 2;
	int paddleVelocity = PADDLE_VELOCITY;

	const int BALL_VELOCITY_LIMIT = 5; //maximum ball velocity
	//int ballVelocity = 0;
	//int xBallVelocity = 1;
	//int yBallVelocity = 1;
	//int ballInitialXDirection = 0;
	//int ballInitialYDirection = 0;

	//SDL_Renderer* renderer = nullptr;
	SDL_Rect paddle = {};
	SDL_Rect ball = {};
	SDL_Rect screenRect = {};
	screenRect.x = screenRect.y = 0;

	struct xy { int x, y; };
	//const xy BALL_VELOCITY = {1, 1};
	xy ballVelocity = { 1, 1 };

	const xy BALL_ACCELERATION = { 5, 5 };
	//xy ballAcceleration = {};

	const int PADDLE_ACCELERATION = 5;
	int paddleAcceleration = PADDLE_ACCELERATION;
	//const xy PADDLE_ACCELERATION = {1,1};

	xy ballDirection = { 1, 1 };

	enum direction { STATIONARY, LEFT, RIGHT };
	direction currentPaddleDirection = STATIONARY;
	direction previousPaddleDirection = STATIONARY;

	//paddle.x = paddle.y = 600; //get and use screen dimensions. start paddle in middle of screen.
	paddle.x = 600;
	paddle.y = 400;
	paddle.w = 100;
	paddle.h = 20;

	ball.h = ball.w = 10;
	ball.x = paddle.x + paddle.w / 2 - ball.w / 2; //centralise the ball on top of the paddle
	ball.y = paddle.y - ball.h; //centralise the ball on top of the paddle

	int buttonCount = 0;

	Mix_Chunk * ballBounceSound = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Event event;
	SDL_Window * screen = SDL_CreateWindow("My First Window",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * renderer = SDL_CreateRenderer(screen, -1, 0);

	SDL_GetRendererOutputSize(renderer, &screenRect.w, &screenRect.h);
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* fighterPlane = IMG_Load("images/Plane1.png");
	SDL_Texture * fighterPlaneTexture = SDL_CreateTextureFromSurface(renderer, fighterPlane);
	SDL_RenderCopy(renderer, fighterPlaneTexture, NULL, NULL);
	SDL_RenderPresent(renderer);


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}

	ballBounceSound = Mix_LoadWAV("sounds/ball_bounce.wav");

	while (!quit)
	{
		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		if (buttonCount % 3 == 0)
		{
			r = 255;
			g = 0;
			b = 0;
		}
		else if (buttonCount % 3 == 1)
		{
			r = 0;
			g = 255;
			b = 0;
		}
		else if (buttonCount % 3 == 2)
		{
			r = 0;
			g = 0;
			b = 255;
		}
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);

		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				/*
				paddle.y -= paddleVelocity;
				if(!ballLaunched)
				{
				ball.y -= paddleVelocity;
				//ballVelocity.y = -yBallVelocity;
				}
				//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				//++buttonCount;
				*/
				break;
			case SDLK_DOWN:
				/*
				paddle.y += paddleVelocity;
				if(!ballLaunched)
				{
				ball.y += paddleVelocity;
				ballVelocity.y = yBallVelocity;
				}
				//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				//++buttonCount;
				*/
				break;
			case SDLK_LEFT:
				currentPaddleDirection = LEFT;
				if (currentPaddleDirection != previousPaddleDirection)
					paddleVelocity = 0; //stop paddle movement when its direction is changed, so that it moves off gradually in the new direction
					//if(paddle.x <= screenRect.x && paddle.x >= screenRect.x - 1)
					//{
					//    if(currentPaddleDirection == LEFT)
					//    {
					//        paddleVelocity = 0;
					//        paddleAcceleration = PADDLE_ACCELERATION * 0;
					//    }
					//}
					//else if(currentPaddleDirection)
					//{
				paddleVelocity += paddleAcceleration;
				paddle.x -= paddleVelocity;
				//}
				if (!isBallLaunched)
				{
					ball.x -= paddleVelocity;

					// set the ball's direction for launch
					ballDirection.x = -1; //ball needs to move left
					ballDirection.y = -1; //ball needs to move up
										  //ballVelocity.x = BALL_VELOCITY.x;
										  //ballVelocity.y = BALL_VELOCITY.y;
										  //ballVelocity.x = BALL_VELOCITY.x * -1;
										  //ballVelocity.y = BALL_VELOCITY.y * -1;
										  //ballAcceleration.x = BALL_ACCELERATION.x;
										  //ballAcceleration.y = BALL_ACCELERATION.y;
				}
				//else if(isBallHit) ballDirection.x = -1; //when ball hits paddle while paddle is moving left, ball moves left
				previousPaddleDirection = LEFT;
				//if(ballLaunched)
				//{
				//    ball.x -= ballVelocity;
				//    ball.y -= ballVelocity;
				//}
				//else ball.x -= paddleVelocity;
				//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				//++buttonCount;
				break;
			case SDLK_RIGHT:
				currentPaddleDirection = RIGHT;
				if (currentPaddleDirection != previousPaddleDirection)
					paddleVelocity = 0; //stop paddle movement when its direction is changed, so that it moves off gradually in the new direction
				paddleVelocity += paddleAcceleration;
				paddle.x += paddleVelocity;
				if (!isBallLaunched)
				{
					ball.x += paddleVelocity;
					// set the ball's direction for launch
					ballDirection.x = 1; //ball needs to move right
					ballDirection.y = -1; //ball needs to move up
										  //ballVelocity.x = BALL_VELOCITY.x;
										  //ballVelocity.y = BALL_VELOCITY.y;
										  //ballVelocity.y = BALL_VELOCITY.y * -1;
										  //ballAcceleration.x = BALL_ACCELERATION.x;
										  //ballAcceleration.y = BALL_ACCELERATION.y;
				}
				//else if(isBallHit) ballDirection.x = 1; //when ball hits paddle while paddle is moving right, move the ball right
				previousPaddleDirection = RIGHT;
				//if(ballLaunched)
				//{
				//    ball.x += ballVelocity;
				//    ball.y += ballVelocity;
				//}
				//else
				//   ball.x += paddleVelocity;
				//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				//++buttonCount;
				break;
			case SDLK_SPACE:
				if (currentPaddleDirection != STATIONARY) //don't launch the ball when paddle hasn't been moved
					isBallLaunched = true;
				//std::cout << "spacebar pressed" << std::endl;
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				SDL_SetRenderDrawColor(renderer, r, g, b, 255);
				++buttonCount;
				//std::cout << buttonCount << ": " << r << "," << g << "," << b << std::endl;
				break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			//std::cout << "quit" << std::endl;
			break;
		}

		//collision detection for paddle against sides of screen
		//don't allow the paddle to move further if it hits the sides of the screen
		//if(paddle.x <= screenRect.x || (paddle.x + paddle.w >= screenRect.x + screenRect.w))
		//{
		//    if(currentPaddleDirection == previousPaddleDirection)
		//    {
		//        paddleVelocity = 0;
		//        paddleAcceleration = PADDLE_ACCELERATION * 0;
		//    }
		//    else paddleAcceleration = PADDLE_ACCELERATION;
		//}
		if (isBallLaunched)
		{
			isBallHit = false;
			//calculate acceleration and speed, with speed limit
			//ballVelocity.x += BALL_ACCELERATION.x;
			//ballVelocity.y += BALL_ACCELERATION.y;
			if (ballVelocity.x >= BALL_VELOCITY_LIMIT)
				ballVelocity.x += BALL_ACCELERATION.x * 0;
			else ballVelocity.x += BALL_ACCELERATION.x;
			if (ballVelocity.y >= BALL_VELOCITY_LIMIT)
				ballVelocity.y += BALL_ACCELERATION.y * 0;
			else ballVelocity.y += BALL_ACCELERATION.y;

			ball.x += ballVelocity.x * ballDirection.x;
			ball.y += ballVelocity.y * ballDirection.y;
			//std::cout << "ballVelocity x,y: " << ballVelocity.x << "," <<
			//ballVelocity.y << std::endl;
			//std::cout << "ball x,y: " << ball.x << "," <<
			//ball.y << std::endl;

			//collision with screen top or bottom
			if (ball.y <= screenRect.y || (ball.y + ball.h) >= screenRect.h)
				//std::cout << "collided with screen top or bottom" << std::endl;
				//ballVelocity.y = -ballVelocity.y;
			{
				Mix_PlayChannel(-1, ballBounceSound, 0);
				ballDirection.y = ballDirection.y * -1;
			}

			//collision with screen left or right
			if (ball.x <= screenRect.x || (ball.x + ball.w) >= screenRect.w)
				//std::cout << "collided with screen left or right" << std::endl;
				//ballVelocity.x = -ballVelocity.x;
			{
				Mix_PlayChannel(-1, ballBounceSound, 0);
				ballDirection.x = ballDirection.x * -1;
			}

			//collision detection between ball and paddle
			//check if ball's bottom is within paddle's top and 1 pixel more than paddle's top
			//and check if ball's x position is within the range of paddle's x position and paddle's width
			if ((ball.y + ball.h >= paddle.y && ball.y + ball.h <= paddle.y + paddle.h) && ((ball.x >= paddle.x) && ball.x + ball.w <= paddle.x + paddle.w))
			{
				isBallHit = true;
				Mix_PlayChannel(-1, ballBounceSound, 0);
				ballDirection.y = ballDirection.y * -1;
				//paddle direction controls direction the ball will move in
				if (currentPaddleDirection == RIGHT) ballDirection.x = 1;
				else ballDirection.x = -1;
			}
		}

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &paddle);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &ball);
		SDL_RenderPresent(renderer);
		//std::cout << counter << std::endl;
		counter++;
		SDL_Delay(50);
	}//game loop ends

	 //SDL_Delay(3000);
	 // game code eventually goes here

	SDL_Quit();

	return 0;
}
//****************End of brick and ball game****************

//****************Start of fighter plane game****************
#elif defined plane_game

#include <SDL.h>
#include <iostream>
#include "add.h"
#include <SDL_mixer.h>
#include <SDL_image.h>

//using namespace std;

int main(int argc, char ** argv)
{
	//Demo of splitting code into multiple .cpp and .h files
	std::cout << "add 5 and 7: " << add(5, 7) << std::endl;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Event event;
	SDL_Window * screen = SDL_CreateWindow("My First Window",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * renderer = SDL_CreateRenderer(screen, -1, 0);

//	SDL_GetRendererOutputSize(renderer, &screenRect.w, &screenRect.h);
//	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* fighterPlane = IMG_Load("images/Plane1.png");
	SDL_Texture * fighterPlaneTexture = SDL_CreateTextureFromSurface(renderer, fighterPlane);
	SDL_RenderCopyEx(renderer, fighterPlaneTexture, NULL, NULL, 0 , NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);

	SDL_Delay(5000);
	return 0;
}
#endif

