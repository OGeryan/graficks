#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

typedef std::string string;
struct Line_draw
{
	int x_a;
	int y_a;
	int x_b;
	int y_b;
	
	int cl_a;
	int cl_b;
	int cl_c;
};

std::vector<Line_draw> All_Lines;

int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 500;

int main ()
{	 
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Window* Window = NULL;
	
	SDL_Renderer* Renderer = NULL;
	
	Window = SDL_CreateWindow("GRAFICKS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	
	TTF_Font* Font1 = NULL;
	Font1 = TTF_OpenFont("arial.ttf", 14);
	
	//ESTABLISH COLORS
	SDL_Color CRed = {255, 0, 0, 255};
	SDL_Color CGreen = {0, 255, 0, 255};
	SDL_Color CBlue = {0, 0, 255, 255};
	SDL_Color CWhite = {255, 255, 255, 255};
	string CLR_RTXT;
	string CLR_GTXT;
	string CLR_BTXT;
	string GRD_TXT;
	
	//MAKE SURFACES AND TEXTURES
	SDL_Surface* MSGSurface_r;
	SDL_Texture* MSG_r;
	SDL_Surface* MSGSurface_g;
	SDL_Texture* MSG_g;
	SDL_Surface* MSGSurface_b;
	SDL_Texture* MSG_b;
	SDL_Surface* MSGSurface_s;
	SDL_Texture* MSG_s;
	
	
	//MAKE RECTANGLES 
	SDL_Rect msg_r_1;
	msg_r_1.x = 5;
	msg_r_1.y = WINDOW_HEIGHT - 60;
	SDL_Rect msg_r_2;
	msg_r_2.x = 5;
	msg_r_2.y = WINDOW_HEIGHT - 40;
	SDL_Rect msg_r_3;
	msg_r_3.x = 5;
	msg_r_3.y = WINDOW_HEIGHT - 20;
	SDL_Rect msg_r_4;
	msg_r_4.x = WINDOW_WIDTH - 60;
	msg_r_4.y = WINDOW_HEIGHT - 20;

	string SaveData;
	
	int MX = 250, MY = 250;
	int MX_END, MY_END;
	int CL_R = 255, CL_G = 255, CL_B = 255;
	int CL_FAC = 0;
		
	int GridSize = 25;
	int GridFac = 1;
		
	bool IsSnapping = false;
	
	const Uint8* keys = SDL_GetKeyboardState( NULL );
	
	bool Running = true;
	bool Drawing = false;
	bool ShowUI = true;
	
	bool IsClampY = false;
	bool IsClampX = false;
	while(Running)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e)) //GAMELOOP UPDATE
		{
			if(e.type == SDL_MOUSEMOTION) //UPDATE MOUSE POSITION
			{
				SDL_GetMouseState(&MX_END, &MY_END);
			}
			
			if(e.type == SDL_MOUSEBUTTONDOWN) //NOUSE INPUT
			{
				if(e.button.button == SDL_BUTTON_LEFT) //DRAWING
				{
					if(Drawing == false) //START LINE IF THERE ISN'T ONE
					{
						SDL_GetMouseState(&MX, &MY);
						Drawing = true;
					}
					else //DRAW IF LINES IS STARTED
					{
						All_Lines.push_back({MX, MY,  (!IsClampX) ? MX_END : MX, (!IsClampY) ? MY_END : MY, CL_R, CL_G, CL_B});
						if(IsClampX || IsClampY)
						{
							MX = (!IsClampX) ? MX_END : MX;
							MY = (!IsClampY) ? MY_END : MY;
						}
						else
						{
							SDL_GetMouseState(&MX, &MY);
						}
						IsClampX = false;
						IsClampY = false;
					}
				}
				if(e.button.button == SDL_BUTTON_RIGHT) //STOP DRAWING
				{
					Drawing = false;
					IsClampX = false;
					IsClampY = false;
				}
				if(e.button.button == SDL_BUTTON_MIDDLE) //RESET COLOR
				{
					switch(CL_FAC)
					{
						case 0:
							CL_R = 255;
							break;
						case 1:
							CL_G = 255;
							break;
						case 2:
							CL_B = 255;
							break;
					}
				}
			}
			if(e.type == SDL_MOUSEWHEEL) //COLOR SWITCH
			{
				if(e.wheel.y > 0) //MOUSE WHEEL INPUT
				{
					switch(CL_FAC)//SWITCH BETWEEN COLORS
					{
						case 0:
							CL_R += 5;
							if(CL_R > 255) //IF RED
							{
								CL_R = 0;
							}
						break;
						case 1:
						CL_G += 5;
							if(CL_G > 255) //IF GREEN
							{
								CL_G = 0;
							}
						break;
						case 2:
						CL_B += 5;
							if(CL_B > 255) //IF BLUE
							{
								CL_B = 0;
							}
						break;
					}
				}
				if(e.wheel.y < 0) //MOUSE WHEEL INPUT
				{
					switch(CL_FAC) //SWITCH BETWEEN COLORS
					{
						case 0:
							CL_R -= 5;
							if(CL_R < 0) //IF RED
							{
								CL_R = 255;
							}
						break;
						case 1:
						CL_G -= 5;
							if(CL_G < 0) //If GREEN
							{
								CL_G = 255;
							}
						break;
						case 2:
						CL_B -= 5;
							if(CL_B < 0) //IF BLUE
							{
								CL_B = 255;
							}
						break;
					}
				}
			}
			if(e.type == SDL_KEYDOWN) //KEYBOARD INPUT
			{
				if(e.key.keysym.sym == SDLK_SPACE) //SPACEBAR
				{
					Drawing = false;
					IsClampX = false;
					IsClampY = false;
				}
				if(e.key.keysym.sym == SDLK_TAB) //TAB
				{
					CL_FAC++;
					if(CL_FAC > 2)
					{
						CL_FAC = 0;
					}
				}
				if(e.key.keysym.sym == SDLK_r) //r for RESET
				{
					Drawing = false;
					All_Lines.clear();
					SDL_RenderClear(Renderer);
					SDL_RenderPresent(Renderer);
				}
				if(e.key.keysym.sym == SDLK_z) //z for UNDO
				{
					if(All_Lines.size() > 0)
					{
						All_Lines.pop_back();
					}
				}
				if(e.key.keysym.sym == SDLK_h)
				{
					ShowUI = !ShowUI;
				}
				if(e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_LEFT)
				{
					IsClampY = !IsClampY;
					IsClampX = false;
				}
				if(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)
				{
					IsClampX = !IsClampX;
					IsClampY = false;
				}
				if(e.key.keysym.sym == SDLK_g)
				{
					GridFac++;
					if(GridFac > 2)
					{
						GridFac = 0;
					}
					switch(GridFac)
					{
						case 0:
							GridSize = 10;
							break;
						case 1:
							GridSize = 25;
							break;
						case 2:
							GridSize = 50;
							break;
					}
				}
				if(e.key.keysym.sym == SDLK_s)
				{
					fstream outfile("SAVE.data", std::ofstream::out | std::ofstream::trunc );
					outfile.flush();
					outfile.close();
					outfile.open("SAVE.data");
					SaveData = "";
					for(const Line_draw d : All_Lines)
					{
						SaveData.append(std::to_string(d.x_a));
						SaveData.append(",");
						SaveData.append(std::to_string(d.y_a));
						SaveData.append(",");
						SaveData.append(std::to_string(d.x_b));
						SaveData.append(",");
						SaveData.append(std::to_string(d.y_b));
						SaveData.append(",");
						SaveData.append(std::to_string(d.cl_a));
						SaveData.append(",");
						SaveData.append(std::to_string(d.cl_b));
						SaveData.append(",");
						SaveData.append(std::to_string(d.cl_c));
						SaveData.append(",");
					}
					outfile << SaveData;
					outfile.close();
				}
				if(e.key.keysym.sym == SDLK_l)
				{
					All_Lines.clear();
					string ReadData = "";
					ifstream readfile;
					readfile.open("SAVE.data");
					if(readfile)
					{
						readfile >> ReadData;
					}
					string Data = ReadData;
					printf(Data.c_str());
					int p1, p2;
					p1 = Data.find(',');
					p2 = 0;
					int num = 0;
					std::vector<int> nums;
					int count = 0;
					while(p1 != string::npos)
					{
						istringstream(Data.substr(p2, p1 - p2)) >> num;
						printf("%d;", num);
						nums.push_back(num);
						p2 = p1 + 1;
						p1 = Data.find(',', p2);
						if(nums.size() % 7 == 0)
						{
							Line_draw line = {nums[0 + (count * 7)], nums[1 + (count * 7)], nums[2 + (count * 7)], nums[3 + (count * 7)], nums[4 + (count * 7)], nums[5 + (count * 7)], nums[6 + (count * 7)]};
							count++;
							All_Lines.push_back(line);
						}
					}
				}
			}
			if(e.type == SDL_QUIT) //QUITTING GAME/STOPPING GAMELOOP
			{
				Running = false;
			}
			if(keys[SDL_SCANCODE_LCTRL])
			{
				IsSnapping = true;
			}
			else
			{
				IsSnapping = false;
			}
		}
		
		if(IsSnapping)
		{
			MX = MX	- (MX % GridSize);
			MY = MY - (MY % GridSize);
			MX_END = MX_END - (MX_END % GridSize);
			MY_END = MY_END - (MY_END % GridSize);
		}
		
		//THESE MAKE THE NUMBER VALUES
		CLR_RTXT = std::to_string(CL_R).append((CL_FAC == 0) ? "*" : "");
		CLR_GTXT = std::to_string(CL_G).append((CL_FAC == 1) ? "*" : "");
		CLR_BTXT = std::to_string(CL_B).append((CL_FAC == 2) ? "*" : "");
		
		GRD_TXT = "Grid: ";
		GRD_TXT.append(std::to_string(GridSize));
		if(IsSnapping)
		{
			GRD_TXT = GRD_TXT + "*";
		}
		
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); //NEEDED BEFORE DRAWING
		SDL_RenderClear(Renderer);
		
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255); //DEFAULT DRAW COLOR
			
			MSGSurface_r = TTF_RenderText_Solid(Font1, CLR_RTXT.c_str(), CRed);//MAKE A RED NUMBER, ASSIGNED TO A RECTANGLE ON THE BOTTOM RIGHT OF THE SCREEN
			MSG_r = SDL_CreateTextureFromSurface(Renderer, MSGSurface_r);///MAKE THE NUMBER A TEXTURE
			MSGSurface_g = TTF_RenderText_Solid(Font1, CLR_GTXT.c_str(), CGreen);//MAKE A GREEN NUMBER, ASSIGNED TO A RECTANGLE ON THE BOTTOM RIGHT OF THE SCREEN
			MSG_g = SDL_CreateTextureFromSurface(Renderer, MSGSurface_g); //~
			MSGSurface_b = TTF_RenderText_Solid(Font1, CLR_BTXT.c_str(), CBlue);//MAKE A BLUE NUMBER, ASSIGNED TO A RECTANGLE ON THE BOTTOM RIGHT OF THE SCREEN
			MSG_b = SDL_CreateTextureFromSurface(Renderer, MSGSurface_b);//~
			
			MSGSurface_s = TTF_RenderText_Solid(Font1, GRD_TXT.c_str(), CWhite);
			MSG_s = SDL_CreateTextureFromSurface(Renderer, MSGSurface_s);
			

			msg_r_1.w = MSGSurface_r->w;//DON'T STRETCH NUMBER TEXTURE
			msg_r_1.h = MSGSurface_r->h;//~
			msg_r_2.w = MSGSurface_g->w;//~
			msg_r_2.h = MSGSurface_g->h;//~
			msg_r_3.w = MSGSurface_b->w;//~
			msg_r_3.h = MSGSurface_b->h;//~
			msg_r_4.w = MSGSurface_s->w;//~
			msg_r_4.h = MSGSurface_s->h;//~
		
		for(const Line_draw& l : All_Lines) //RE-RENDER ALL LINES
		{
			SDL_SetRenderDrawColor(Renderer, l.cl_a, l.cl_b, l.cl_c, 255); //GET COLOR
			SDL_RenderDrawLine(Renderer,l.x_a, l.y_a, l.x_b, l.y_b); //GET POSITION
		}
		if(Drawing == true) //CURRENT LINE
		{
			SDL_SetRenderDrawColor(Renderer, CL_R, CL_G, CL_B, 255);
			SDL_RenderDrawLine(Renderer, MX, MY, (!IsClampX) ? MX_END : MX, (!IsClampY) ? MY_END : MY);
		}
		if(ShowUI)
		{
			//THESE RENDER THE NUMBERS AS TEXTURE
			SDL_RenderCopy(Renderer, MSG_r, NULL, &msg_r_1);
			SDL_RenderCopy(Renderer, MSG_g, NULL, &msg_r_2);
			SDL_RenderCopy(Renderer, MSG_b, NULL, &msg_r_3);
			SDL_RenderCopy(Renderer, MSG_s, NULL, &msg_r_4);
			
			//FREE THE SURFACES, SO THAT THE DATA ISN'T KEPT
			SDL_FreeSurface(MSGSurface_r);
			SDL_FreeSurface(MSGSurface_g);
			SDL_FreeSurface(MSGSurface_b);
			SDL_FreeSurface(MSGSurface_s);
			SDL_DestroyTexture(MSG_r);
			SDL_DestroyTexture(MSG_g);
			SDL_DestroyTexture(MSG_b);
			SDL_DestroyTexture(MSG_s);
		}
		if(IsClampX || IsClampY)
		{}
		else
		{
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
			SDL_RenderDrawPoint(Renderer, MX_END, MY_END);	
			SDL_RenderDrawPoint(Renderer, MX_END + 1, MY_END);	
			SDL_RenderDrawPoint(Renderer, MX_END, MY_END + 1);	
			SDL_RenderDrawPoint(Renderer, MX_END - 1, MY_END);	
			SDL_RenderDrawPoint(Renderer, MX_END, MY_END - 1);	
		}
			SDL_RenderPresent(Renderer); //RENDER ALL lINES
	}
}
