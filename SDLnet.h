#pragma once

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>

#include "net.h"

SDL_Window* netWindow;
SDL_Renderer* netRenderer;
const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 400;
TTF_Font* Sans;
SDL_Event event;
bool quit = false;

void writeTxt(const string &text, int x, int y, int width, int height, double angle, SDL_Renderer* renderer) {


	SDL_Color Black = { 0, 0, 0 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), Black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = width; // controls the width of the rect
	Message_rect.h = height; // controls the height of the rect

							 //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

							 //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

	SDL_RenderCopyEx(renderer, Message, nullptr, &Message_rect, angle, nullptr, SDL_FLIP_NONE); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		netWindow = SDL_CreateWindow("Neural net", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		// SDL_SetWindowFullscreen(netWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (netWindow == nullptr)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			netRenderer = SDL_CreateRenderer(netWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (netRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(netRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_SetRenderDrawBlendMode(netRenderer, SDL_BLENDMODE_BLEND);
				TTF_Init();
				IMG_Init(IMG_INIT_JPG);
				Sans = TTF_OpenFont("..\\Resources\\Sans.ttf", 16); //this opens a font style and sets a size
														//Initialize PNG loading

			}
		}
	}

	return success;
}

#include "simulation.h"

class SDLnet {

	SDL_Rect logo_area{};
	SDL_Rect net_area{}; float net_zoom = 1;
	SDL_Rect log_area{}; float log_scrol = 0; float old_size = 0;
	SDL_Rect graph_area{}; float graph_zoom = 1;
	SDL_Rect simulation_area{}; float simulation_zoom = 1;
	SDL_Rect option_area{};
	float wheel = 0;
public:
	vector<string> log;
	SDLnet() {
		int w, h;
		SDL_GetWindowSize(netWindow, &w, &h);
		net_area.x = 200;
		net_area.y = 100 + (h - 100) / 3;
		net_area.w = (w - 200) / 2;
		net_area.h = (h - 100) / 3 * 2;

		simulation_area.x = 200 + (w - 200) / 2;
		simulation_area.y = 100 + (h - 100) / 3;
		simulation_area.w = (w - 200) / 2;
		simulation_area.h = (h - 100) / 3 * 2;

		graph_area.x = 200;
		graph_area.y = 100;
		graph_area.w = w - 200;
		graph_area.h = (h - 100) / 3;

		log_area.x = 0;
		log_area.y = 100;
		log_area.w = 200;
		log_area.h = h - 102;

		logo_area.x = 0;
		logo_area.y = 0;
		logo_area.w = 200;
		logo_area.h = 100;

		option_area.x = 200;
		option_area.y = 0;
		option_area.w = w - 200;
		option_area.h = 100;

	}

	void draw_logo(SDL_Rect area, SDL_Renderer* renderer);
	void draw_net(NeuralNet* net, SDL_Rect area, SDL_Renderer* renderer, double zoom = 1);
	void draw_graph(vector<double> points, SDL_Rect area, SDL_Renderer* renderer, double zoom = 1);
	void draw_simulation(Simulation * simulation, SDL_Rect area, SDL_Renderer* renderer, double zoom = 1);
	void draw_log(vector<string>* log, SDL_Rect area, SDL_Renderer* renderer, int starting_line = 0);
	void draw_option(SDL_Rect area, SDL_Renderer* renderer);
	void draw(NeuralNet* net, Simulation * simulation, vector<double> points, SDL_Renderer* renderer);

	void zoom(float x) {
		wheel = x;
	}

	int button() {

		int mx, my;
		SDL_GetMouseState(&mx, &my);
		SDL_Rect button;
		button.h = 70;
		button.w = 70;
		button.y = option_area.y + 15;
		button.x = option_area.x + 15;

		SDL_Rect button1;
		button1.h = 70;
		button1.w = 70;
		button1.y = option_area.y + 15;
		button1.x = option_area.x + 100;


		if (point_inside_rect(SDL_Point{ mx, my }, button)) {
			return 10;
		}
		else if (point_inside_rect(SDL_Point{ mx, my }, button1)) {
			return -10;
		}


		return 0;
	}
};

void SDLnet::draw_logo(SDL_Rect area, SDL_Renderer* renderer) {
	SDL_RenderSetViewport(renderer, &area);
	SDL_Rect rect;
	rect.x = 1;
	rect.y = 1;
	rect.h = area.h - 1;
	rect.w = area.w - 1;
	SDL_RenderDrawRect(renderer, &rect);
	writeTxt("NEURAL", 30, 0, area.w - 60, 30, 0, renderer);
	writeTxt("NET", 60, 35, area.w - 120, 30, 0, renderer);
	writeTxt("Created by: ", 5, 65, area.w - 120, 30, 0, renderer);
	writeTxt("Emilio Dalla Torre", area.w - 110, 65, 100, 15, 0, renderer);
}

void SDLnet::draw_graph(vector<double> points, SDL_Rect area, SDL_Renderer* renderer, double zoom) {
	if (!points.empty()) {
		int max_y = 1;
		for (double point : points) {
			if (point > max_y) max_y = static_cast<int>(point);
		}
		int scale = area.h / max_y;
		SDL_RenderSetViewport(renderer, &area);
		SDL_Rect rect;
		rect.x = 1;
		rect.y = 1;
		rect.h = area.h - 1;
		rect.w = area.w - 1;
		SDL_RenderDrawRect(renderer, &rect);
		double y1 = area.h - (points[0] * scale);
		for (int i = 1; i < points.size(); i++) {
			double y2 = area.h - (points[i] * scale);
			writeTxt(remove_zero(to_string(points[i - 1])), area.w * (i - 1) / points.size() * zoom, y1 * zoom, 30 * zoom, 30 * zoom, 0, renderer);
			SDL_RenderDrawLine(renderer, area.w * (i - 1) / points.size() * zoom, y1 * zoom, area.w * i / points.size() * zoom, y2 * zoom);
			y1 = y2;
		}
	}
}

void SDLnet::draw_net(NeuralNet* net, SDL_Rect area, SDL_Renderer* renderer, double zoom) {
	SDL_RenderSetViewport(renderer, &area);
	SDL_Rect rect;
	rect.x = 1;
	rect.y = 1;
	rect.h = area.h - 1;
	rect.w = area.w - 1;
	SDL_RenderDrawRect(renderer, &rect);
	int x = 10;
	int y = 10;
	SDL_Rect neuron;
	neuron.w = (area.w - 20 - net->layers.size() * 20) / (net->layers.size()) *zoom;
	int max_neurons = 0;
	for (auto& layer : net->layers) {
		int neurons = 0;
		for (auto& neuron : layer.neurons) {
			neurons++;
		}
		if (neurons > max_neurons)max_neurons = neurons;
	}
	neuron.h = (area.h - 20 - max_neurons * 10) / (max_neurons)*zoom;
	if (neuron.h < neuron.w) {
		neuron.w = neuron.h;
	}
	else
	{
		neuron.h = neuron.w;

	}



	neuron.x = x;
	for (int layer = 0; layer < net->layers.size(); layer++) {
		y = 10 + (area.h - 10 - net->layers[layer].neurons.size()*(10 + neuron.h)) / 2;
		for (auto& n : net->layers[layer].neurons) {
			neuron.y = y;
			SDL_RenderDrawRect(renderer, &neuron);
			writeTxt(remove_zero(to_string(n.value)), neuron.x + 10, (neuron.y + neuron.h / 2 - 10), (neuron.w - 20)*zoom, neuron.h / 2 * zoom, 0, renderer);
			y += neuron.w + 10;
			int line_ending = 0;
			if (1 + layer < net->layers.size())
				line_ending = 10 + (area.h - 10 - net->layers[1 + layer].neurons.size()*(10 + neuron.h)) / 2 + neuron.h / 2;
			for (auto& weight : n.weights) {
				SDL_RenderDrawLine(renderer, neuron.x + neuron.w, neuron.y + neuron.h / 2, neuron.x + neuron.w * 2, line_ending);
				line_ending += neuron.h + 10;
			}
		}

		x += neuron.w * 2;
		neuron.x = x;
	}


}

void SDLnet::draw_simulation(Simulation * simulation, SDL_Rect area, SDL_Renderer* renderer, double zoom) {
	SDL_RenderSetViewport(renderer, &area);
	SDL_Rect rect;
	rect.x = 1;
	rect.y = 1;
	rect.h = area.h - 1;
	rect.w = area.w - 1;
	SDL_RenderDrawRect(renderer, &rect);
	simulation->draw(area, renderer, zoom);
}

void SDLnet::draw_log(vector<string>* log, SDL_Rect area, SDL_Renderer* renderer, int starting_line) {
	SDL_RenderSetViewport(renderer, &area);
	SDL_Rect rect;
	rect.x = 1;
	rect.y = 1;
	rect.h = area.h - 1;
	rect.w = area.w - 1;
	SDL_RenderDrawRect(renderer, &rect);
	writeTxt("LOG", 30, 0, area.w - 60, 30, 0, renderer);
	int y = 50;
	for (int s = starting_line; s < (*log).size(); s++) {

		if ((*log)[s].length() * 10 > area.w - 20) {
			string str = (*log)[s];
			int max_c = (area.w - 20) / 10;
			while (str.length() != 0) {
				if (str[0] == ' ')str.erase(0, 1);
				string sub = str.substr(0, max_c);
				auto found = sub.find_last_of(' ');
				sub = str.substr(0, found);
				writeTxt(sub, 10, y, sub.length() * 10, 15, 0, renderer);
				y += 25;
				str.erase(0, found);
			}
		}
		else
		{
			writeTxt((*log)[s], 10, y, (*log)[s].length() * 10, 15, 0, renderer);
			y += 25;
		}
		if (y > area.h && log->size() > old_size) { log_scrol++; old_size = log->size(); break; }
	}

}

void SDLnet::draw_option(SDL_Rect area, SDL_Renderer* renderer) {
	SDL_RenderSetViewport(renderer, &area);
	SDL_Rect rect;
	rect.x = 1;
	rect.y = 1;
	rect.h = area.h - 1;
	rect.w = area.w - 1;
	SDL_RenderDrawRect(renderer, &rect);

	SDL_Rect button;
	button.h = 70;
	button.w = 70;
	button.y = 15;
	button.x = 15;
	SDL_RenderDrawRect(renderer, &button);
	writeTxt("FF", 25, 35, 50, 30, 0, renderer);

	button.x = 100;
	SDL_RenderDrawRect(renderer, &button);
	writeTxt("SD", 110, 35, 50, 30, 0, renderer);
}

void SDLnet::draw(NeuralNet* net, Simulation* simulation, vector<double> points, SDL_Renderer* renderer) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	draw_net(net, net_area, renderer, net_zoom);
	if (point_inside_rect(SDL_Point{ (int)mx,(int)my }, net_area)) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		if (wheel != 0) {

			if (wheel > 0) { net_zoom += .1; }
			else { net_zoom -= .1; if (net_zoom <= 0)net_zoom = 1; }

			wheel = 0;
		}
	}

	draw_simulation(simulation, simulation_area, renderer, simulation_zoom);
	if (point_inside_rect(SDL_Point{ (int)mx,(int)my }, simulation_area)) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		if (wheel != 0) {
			if (wheel > 0) { simulation_zoom += .1; }
			else {
				simulation_zoom -= .1;
				if (simulation_zoom <= 0) simulation_zoom = 1;
			}
			wheel = 0;
		}
	}

	draw_graph(points, graph_area, renderer, graph_zoom);
	if (point_inside_rect(SDL_Point{ (int)mx,(int)my }, graph_area)) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		if (wheel != 0) {

			if (wheel > 0) { graph_zoom += .1; }
			else { graph_zoom -= .1; if (graph_zoom <= 0)graph_zoom = 1; }
			wheel = 0;
		}
	}

	draw_log(&log, log_area, renderer, log_scrol);
	if (point_inside_rect(SDL_Point{ (int)mx,(int)my }, log_area)) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		if (wheel != 0) {

			if (wheel > 0) { log_scrol++; if (log_scrol >= log.size())log_scrol = log.size() - 1; }
			else { log_scrol--; if (log_scrol < 0)log_scrol = 0; }
			wheel = 0;
		}
	}

	draw_logo(logo_area, renderer);
	if (point_inside_rect(SDL_Point{ (int)mx,(int)my }, logo_area)) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	draw_option(option_area, renderer);
	if (point_inside_rect(SDL_Point{ (int)mx,(int)my }, option_area)) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	SDL_RenderPresent(renderer);
}