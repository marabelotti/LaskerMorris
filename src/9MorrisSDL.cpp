#include "9MorrisSDL.h"
#include <iostream>

NineMorrisSDL::NineMorrisSDL(int screen_dim) :  Game_9Morris(Game::Human, Game::AI), screen_width(screen_dim), screen_height(screen_dim)
{
	if (!init())
	{
		std::cerr << "Something went wrong with creating the SDL setup\n";
	}

	if (!background())
	{
		std::cerr << "Something went wrong while rendering the image of the board\n";
	}

	init_positions();
}

void NineMorrisSDL::init_positions()
{
	double u = screen_width / 17;
	std::vector<double> x_coords = {u / 2, u * 8, u * 15, u * 15, u * 15, u * (8), u / 2, u / 2, u * (3.5), u * 8, u * 12, u * 12, u * 12, u * 8, u * (3.5), u * 3.5, u * (6.5), u * 8, u * (9.5), u * (9.5), u * (9.5), u * 8, u * (6.5), u * (6.5)};
	std::vector<double> y_coords = {u / 2, u / 2, u / 2, u * (8), u * 15.5, u * 15.5, u * 15.5, u * (8), u * 3.5, u * 3.5, u * 3.5, u * (8), u * 12.5, u * 12.5, u * 12.5, u * 8, u * (6.5), u * 6.5, u * 6.5, u * 8, u * (9.5), u * 9.5, u * 9.5, u * (8)};

	for (int el = 0; el < NUMBER_OF_POSITIONS; el++)
	{
		PositionSDL temp(x_coords[el], y_coords[el]);
		posSDL.push_back(temp);
	}
}

bool NineMorrisSDL::init()
{	
	if(TTF_Init()==-1) {
    	printf("TTF_Init: %s\n", TTF_GetError());
	}else if(!(font=TTF_OpenFont( "./fonts/CaviarDreams.ttf", 100 ))){
		printf("SDL could not initialize font! SDL Error: %s\n", SDL_GetError());
	}
	
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create window
		gWindow = SDL_CreateWindow("Game of Nine Men's Morris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool NineMorrisSDL::background()
{
	// Loading success flag
	bool success = true;

	// Load PNG texture
	gTexture = background_surface_to_texture("./graphic/Board");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image of Board!\n");
		success = false;
	}

	return success;
}

SDL_Texture *NineMorrisSDL::background_surface_to_texture(std::string path)
{
	// The final texture
	SDL_Texture *newTexture = NULL;

	// Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

NineMorrisSDL::~NineMorrisSDL()
{
	close();
}

void NineMorrisSDL::close()
{
	// we also need to free loaded images

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Destroy fonts
	TTF_CloseFont( font );
    font = NULL;

	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


void NineMorrisSDL::visual_game()
{
	bool quit = false;

	SDL_Event e;

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render texture to screen
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

	// Render buttons
	for (int i = 0; i < NUMBER_OF_POSITIONS; ++i)
	{
		render_position(i);
	}

	// Update screen
	render_information("Welcome to the best game of your life!");
	SDL_RenderPresent(gRenderer);

	bool turn = true;

	while (!quit)

	{
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			// Handle button events
			for (int i = 0; i < NUMBER_OF_POSITIONS; ++i)
			{
				posSDL[i].handleEvent(&e);
			}
		}

		if(fresh_mills!=0){
			message="Human, you created one or more mills, please remove piece";
			check_if_mills_and_remove(turn);
			if (fresh_mills==0){
				check_game_phase();
				if(!game_over()){
					turn=!turn;
					get_message(turn);
				}
			}
		}else if (!game_over() and !quit and make_a_move(turn)){	
			if (fresh_mills==0){
				if(!turn){
					check_game_phase();
				}
				turn=!turn;
				get_message(turn);
			}	
		}

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		// Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		// Render buttons
		for (int i = 0; i < NUMBER_OF_POSITIONS; ++i)
		{
			render_position(i);
		}
		render_information(message);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
}

bool NineMorrisSDL::make_a_move_placing(bool turn)
{
	for (int i = 0; i < NUMBER_OF_POSITIONS; i++)
	{
		if (posSDL[i].mSprite == POSITION_SPRITE_MOUSE_UP)
		{
			if (values_are_okay(turn, i))
			{
				place_piece(i, players[turn].player_color);
				posSDL[i].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return true;
			}
			posSDL[i].mSprite = POSITION_SPRITE_MOUSE_OUT;
		}
	}
	return false;
}

bool NineMorrisSDL::make_a_move_moving(bool turn)
{

	int pos_in;
	int pos_fin;
	int cont = 0;

	for (int i = 0; i < NUMBER_OF_POSITIONS; i++)
	{
		if (cont == 0 and (posSDL[i].mSprite == POSITION_SPRITE_MOUSE_UP or posSDL[i].mSprite == POSITION_SPRITE_MOUSE_DOWN))
		{
			pos_in = i;
			cont = cont + 1;
		}
		else if (cont == 1 and (posSDL[i].mSprite == POSITION_SPRITE_MOUSE_UP or posSDL[i].mSprite == POSITION_SPRITE_MOUSE_DOWN))
		{
			pos_fin = i;
			cont = cont + 1;
			if (values_are_okay(turn, pos_in, pos_fin))
			{
				move_piece(pos_fin, pos_in, players[turn].player_color);
				posSDL[pos_in].mSprite = POSITION_SPRITE_MOUSE_OUT;
				posSDL[pos_fin].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return true;
			}
			else if (values_are_okay(turn, pos_fin, pos_in))
			{
				move_piece(pos_in, pos_fin, players[turn].player_color);
				posSDL[pos_in].mSprite = POSITION_SPRITE_MOUSE_OUT;
				posSDL[pos_fin].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return true;
			}
			else
			{
				std::cout << cont << " " << pos_in << " " << pos_fin << "\n";
				std::cout << values_are_okay(turn, pos_fin, pos_in) << "\n";
				std::cout << values_are_okay(turn, pos_in, pos_fin) << "\n";
				std::cout << "The selected positions are invalid\n";
				posSDL[pos_in].mSprite = POSITION_SPRITE_MOUSE_OUT;
				posSDL[pos_fin].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return false;
			}
		}
	}

	return false;
}

bool NineMorrisSDL::make_a_move_flying(bool turn)
{
	int pos_in;
	int pos_fin;
	int cont = 0;

	for (int i = 0; i < NUMBER_OF_POSITIONS; i++)
	{
		if (cont == 0 and (posSDL[i].mSprite == POSITION_SPRITE_MOUSE_UP or posSDL[i].mSprite == POSITION_SPRITE_MOUSE_DOWN))
		{
			pos_in = i;
			cont = cont + 1;
		}
		else if (cont == 1 and (posSDL[i].mSprite == POSITION_SPRITE_MOUSE_UP or posSDL[i].mSprite == POSITION_SPRITE_MOUSE_DOWN))
		{
			pos_fin = i;
			cont = cont + 1;
			if (values_are_okay(turn, pos_in, pos_fin))
			{
				move_piece(pos_fin, pos_in, players[turn].player_color);
				posSDL[pos_in].mSprite = POSITION_SPRITE_MOUSE_OUT;
				posSDL[pos_fin].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return true;
			}
			else if (values_are_okay(turn, pos_fin, pos_in))
			{
				move_piece(pos_in, pos_fin, players[turn].player_color);
				posSDL[pos_in].mSprite = POSITION_SPRITE_MOUSE_OUT;
				posSDL[pos_fin].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return true;
			}
			else
			{
				std::cout << "The selected positions are invalid\n";
				posSDL[pos_in].mSprite = POSITION_SPRITE_MOUSE_OUT;
				posSDL[pos_fin].mSprite = POSITION_SPRITE_MOUSE_OUT;
				return false;
			}
		}
	}

	return false;
}

void NineMorrisSDL::check_if_mills_and_remove(bool turn){

	for (int i = 0; i < NUMBER_OF_POSITIONS; i++)
	{
		if (posSDL[i].mSprite == POSITION_SPRITE_MOUSE_UP)
		{
			if (Board9Morris::positions[i].piece_on_position() == (Board::PieceType) -(int)players[turn].player_color)
			{	
				remove_piece(i, players[turn].player_color);
				posSDL[i].mSprite = POSITION_SPRITE_MOUSE_OUT;
			}else{
				posSDL[i].mSprite = POSITION_SPRITE_MOUSE_OUT;
			}
		}
	}
}

void NineMorrisSDL::render_position(int i)
{
	SDL_Rect fillRect = {posSDL[i].mPosition.x, posSDL[i].mPosition.y, posSDL[i].pos_width, posSDL[i].pos_height};
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	Uint8 a, r, g, b;
	a = 128;
	switch (posSDL[i].mSprite)
	{

	case POSITION_SPRITE_MOUSE_OVER_MOTION:
		r = 255;
		g = 178;
		b = 102;
		SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
		SDL_RenderFillRect(gRenderer, &fillRect);
		break;

	case POSITION_SPRITE_MOUSE_DOWN:
		r = 255;
		g = 102;
		b = 102;
		SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
		SDL_RenderFillRect(gRenderer, &fillRect);
		break;

	case POSITION_SPRITE_MOUSE_UP:
		r = 255;
		g = 102;
		b = 102;
		SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
		SDL_RenderFillRect(gRenderer, &fillRect);
		break;

	default:
		break;
	}

	SDL_Rect piece= {posSDL[i].mPosition.x, posSDL[i].mPosition.y, posSDL[i].pos_width, posSDL[i].pos_height};
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	a=255;
	switch(positions[i].piece_on_position())
	{
		case Black_piece:
			r = 124;
			g = 218;
			b = 121;
			SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
			SDL_RenderFillRect(gRenderer, &piece);
			break;

		case White_piece:
			r = 255;
			g = 178;
			b = 102;
			SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
			SDL_RenderFillRect(gRenderer, &piece);
			break;
		
		default:
			break;
	}
}

void NineMorrisSDL::render_information(std::string message){
	SDL_Color color={0,0,0,255};
	SDL_Surface* text_surface;

	SDL_Texture* text_texture=NULL;
	if(!(text_surface=TTF_RenderText_Blended(font,message.c_str(),color))) {
		//handle error here, perhaps print TTF_GetError at least
	} else {
		text_texture=SDL_CreateTextureFromSurface( gRenderer, text_surface );
		//perhaps we can reuse it, but I assume not for simplicity.
		SDL_Rect Message_rect={100,0,500,40}; 
		SDL_RenderCopy(gRenderer, text_texture, NULL, &Message_rect);
		SDL_FreeSurface(text_surface);
		SDL_DestroyTexture(text_texture);
	}
}


void NineMorrisSDL::get_message(bool turn){
	if(!game_over()){
		switch (turn)
		{
		case true:
			message="Strong AI, please ";
			break;
		
		case false:
			message="Human, please ";
			break;
		}
		
		switch (give_game_phase() )
		{
			case Placing:
			message=message+"place a piece";
				break;
			case Moving:
			message=message+"move a piece";
				break;
			case Flying:
				if(pawn_state[2]==3){
					message=message+"fly a piece";
				}else{
					message=message+"move a piece";
				}
				break;
		}
	}else{
		message="The game is over! ";
		switch (give_winner() )
		{
			case Board::White:
			message+="Human won";
				break;
			case Board::Black:
			message+=" AI won";

			default:
				assert(give_winner()!=Board::Empty);
				break;
		}
	}
}

NineMorrisSDL::PositionSDL::PositionSDL(int x_coord, int y_coord)
{
	mPosition.x = x_coord;
	mPosition.y = y_coord;

	mSprite = POSITION_SPRITE_MOUSE_OUT;
}

void NineMorrisSDL::PositionSDL::handleEvent(SDL_Event *e)
{
	// If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Check if mouse is in button
		bool inside = true;

		// Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		// Mouse is right of the button
		else if (x > mPosition.x + pos_width)
		{
			inside = false;
		}
		// Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		// Mouse below the button
		else if (y > mPosition.y + pos_height)
		{
			inside = false;
		}

		if (!inside)
		{
			if (mSprite == POSITION_SPRITE_MOUSE_OVER_MOTION)
			{
				mSprite = POSITION_SPRITE_MOUSE_OUT;
			}
		}
		else
		{
			// Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				if (mSprite == POSITION_SPRITE_MOUSE_OUT)
				{
					mSprite = POSITION_SPRITE_MOUSE_OVER_MOTION;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				mSprite = POSITION_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mSprite = POSITION_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}
