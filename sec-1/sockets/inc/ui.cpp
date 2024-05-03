#include "ui.h"
#define SDL_STBIMAGE_IMPLEMENTATION
#define STBI_NO_JPEG
#include "SDL_stbimage.h"

void UiManager::setup()
{
	bool running = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	wind = SDL_CreateWindow(GameManager::GetGameManager().getIsServer() ? "Pokeman(?) Duel | SERVER" : "Pokeman(?) Duel | CLIENT",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (wind == NULL)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}

	/* Create a renderer */
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(wind, -1, render_flags);
	if (rend == NULL)
	{
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(wind);
		SDL_Quit();
	}

	if (TTF_Init() != 0)
	{
		printf("Error Initializing TTF%s\n", TTF_GetError());
		TTF_Quit();
	}

	encode = TTF_OpenFont("assets/EncodeRegular.ttf", 12);
	if (encode == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}

void UiManager::setupActive(std::list<Button>* attacks, Pokemon active)
{

	attacks->clear();

	int buttonPosXStart = 225;
	int buttonPosX = buttonPosXStart;
	int buttonPosY = 300;
	int buttonSpacingX = 120;
	int buttonSpacingY = 70;

	for (int i = 0; i < 4; i++)
	{
		std::string display = active.getAttackAt(i).getName() + " : " + std::to_string(active.getAttackAt(i).getDamage());
		attacks->push_back(Button(rend, encode, SDL_Color{ 255, 50, 50, 255 }, SDL_Color{ 255, 100, 100, 255 }, display));

		BattleEvent e;
		e.client_id = GameManager::GetGameManager().getLocalClientId();
		e.attackIndex = i;

		attacks->back().updateCallback(GameManager::acceptAttackInput, e);
		attacks->back().updateRect(100, 50, buttonPosX, buttonPosY);
		buttonPosX += buttonSpacingX;
		if ((i + 1) % 2 == 0)
		{
			buttonPosX = buttonPosXStart;
			buttonPosY += buttonSpacingY;
		}
	}
}

void UiManager::initSprites()
{
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		std::string path = ("assets/Sprites/SPRITE_" + std::to_string(i) + ".png");
		SDL_Surface* s = STBIMG_Load(path.data());
		SDL_Texture* t = STBIMG__SurfToTex(rend, s);
		std::cout << SDL_GetError() << std::endl;
		sprites.push_back(t);

		SDL_FreeSurface(s);
	}

}

//render one players active & two benched 
void UiManager::updateTeam(Player p, bool allied, float dt)
{
	SDL_Rect leaderRect;
	leaderRect.x = allied ? 250 : 350;
	leaderRect.y = 100;
	leaderRect.w = 200;
	leaderRect.h = 200;
	SDL_RenderCopy(rend, sprites[p.party[p.leader].getSprite()], NULL, &leaderRect);

	SDL_Surface* surfaceMessage = SDL_CreateRGBSurface(0, 100, 50, 32, 0, 0, 0, 0);

	surfaceMessage = TTF_RenderText_Solid(encode, std::to_string(p.party[p.leader].getHealth()).c_str(), SDL_Color{ 255, 0, 0, 255 });

	SDL_Texture* leaderHP = SDL_CreateTextureFromSurface(rend, surfaceMessage);

	SDL_Rect leaderHPRect;
	leaderHPRect.x = allied ? 500 : 0;
	leaderHPRect.y = allied ? 320 : 0;
	leaderHPRect.w = 100;
	leaderHPRect.h = 80;

	SDL_RenderCopy(rend, leaderHP, NULL, &leaderHPRect);

	int partyX = allied ? 250 : 350;
	for (int i = 0; i < 3; i++)
	{
		if (i == p.leader) continue;

		SDL_Rect partyRect;
		leaderRect.x = partyX;
		leaderRect.y = 325;
		leaderRect.w = 125;
		leaderRect.h = 125;

		partyX += allied ? -150 : 150;

		SDL_RenderCopy(rend, sprites[p.party[p.leader].getSprite()], NULL, &partyRect);
	}
}


bool UiManager::update(std::list<Button>* attacks, float dt)
{
	SDL_GetMouseState(mouseX, mouseY);
	bool mbUp = false;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mbUp = true;
			}
			break;
		}
	}
	
	SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
	SDL_RenderClear(rend);

	//UPDATE ALL OBJECTS

	for (int i = 0; i < 2; i++)
	{
		updateTeam(*(GameManager::GetGameManager().getPlayerAtIndex(0)), GameManager::GetGameManager().getIsServer(), dt);
		updateTeam(*(GameManager::GetGameManager().getPlayerAtIndex(1)), !GameManager::GetGameManager().getIsServer(), dt);
	}

	for (Button b : *attacks)
	{
		b.update(*mouseX, *mouseY, mbUp);
	}

	SDL_RenderPresent(rend);

	return running;
}