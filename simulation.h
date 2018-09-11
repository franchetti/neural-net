#pragma once
SDL_Texture* car_texture = NULL;

class Simulation {
	float last_angle = 275;
	vector<pair<SDL_Point, float>> road;
	SDL_Point car_position;
	float car_velocity = 5;
	float car_angle = 0;
	float car_max_velocity = 5;
	float car_max_angle =   30;
	vector<pair<pair<float,float>, float>> car_sensor_angles = { 
		pair<pair<float,float>,float>(pair<float,float>(0,45),0) ,
		pair<pair<float,float>,float>(pair<float,float>(15,45),0),
		pair<pair<float,float>,float>(pair<float,float>(-15,45),0),
		pair<pair<float,float>,float>(pair<float,float>(45,45),0),
		pair<pair<float,float>,float>(pair<float,float>(-45,45),0),
		pair<pair<float,float>,float>(pair<float,float>(75,45),0),
		pair<pair<float,float>,float>(pair<float,float>(-75,45),0),
	
	};
	
public:
	bool car_crashed = false;
	Simulation(string path, SDL_Renderer* renderer) {
		if (car_texture == NULL) {
			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load(path.c_str());
			if (loadedSurface == NULL)
			{
				printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
			}
			else
			{
				SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
				//Create texture from surface pixels
				car_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

				if (car_texture == NULL)
				{
					printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
				}

				//Get rid of old loaded surface
				SDL_FreeSurface(loadedSurface);
			}
		}
	
		generate_road();
	}

	void generate_road();
	void draw(SDL_Rect area, SDL_Renderer* renderer, double zoom);
	float run(NeuralNet* net);
};

void Simulation::generate_road() {
	if (road.size() == 0) {
		SDL_Point start;
		start.x = 0;
		start.y = 0;
		road.push_back(pair<SDL_Point, float>(start, 50));
	}
	
	float angle = Random::random_value(last_angle-15, last_angle+15);
	if (angle > 360)angle -= 360;
	if (angle < -360)angle += 360;
	last_angle = angle;
	float distance = Random::random_value(10, 30);
	SDL_Point new_road;
	new_road.x = road[road.size() - 1].first.x + cos(to_rad(angle))*distance;
	new_road.y = road[road.size() - 1].first.y + sin(to_rad(angle))*distance;
	float width = Random::random_value(road[road.size() - 1].second-2.5, road[road.size() - 1].second+2.5);
	if (width < 20)width = 20;
	if (width > 45)width = 45;
	road.push_back(pair<SDL_Point, float>(new_road,width));
	if (road.size() > 25) {
		road.erase(road.begin());
	}
}

void Simulation::draw(SDL_Rect area, SDL_Renderer* renderer, double zoom) {
	writeTxt("Car velocity: " + to_string(car_velocity), 10, 5, 100, 25, 0, renderer);
	writeTxt("Car angle: " + to_string(car_angle), 10, 30, 100, 25, 0, renderer);
	

	SDL_Point center = car_position;
	SDL_RenderDrawLine(renderer, 0 - center.x*zoom + area.w / 2, -10000 - center.x*zoom + area.w / 2, 0 - center.x*zoom + area.w / 2, 10000 - center.x*zoom + area.w / 2);
	SDL_RenderDrawLine(renderer, -10000 - center.x*zoom + area.w / 2, 0 - center.x*zoom + area.w / 2, 10000 - center.x*zoom + area.w / 2, 0 - center.x*zoom + area.w / 2);

	float angle = angular_coefficient(road[1].first, road[0].first);
	SDL_Point a;
	a.x = road[0].first.x + sqrt(pow(road[0].second, 2) / (1 + 1 / pow(angle, 2)));
	a.y = -1 / angle*(a.x - road[0].first.x) + road[0].first.y;
	SDL_Point b;
	b.x = road[0].first.x - sqrt(pow(road[0].second, 2) / (1 + 1 / pow(angle, 2)));
	b.y = -1 / angle*(b.x - road[0].first.x) + road[0].first.y;

	for (int r = 1; r < road.size(); r++) {
		
		float angle = angular_coefficient(road[r].first, road[r-1].first);
		SDL_Point a1;
		a1.x = road[r].first.x + sqrt(pow(road[r].second, 2) / (1 + 1 / pow(angle, 2)));
		a1.y = -1 / angle*(a1.x - road[r].first.x) + road[r].first.y;
		SDL_Point b1;
		b1.x = road[r].first.x - sqrt(pow(road[r].second, 2) / (1 + 1 / pow(angle, 2)));
		b1.y = -1 / angle*(b1.x - road[r].first.x) + road[r].first.y;


		SDL_RenderDrawLine(renderer, a.x*zoom - center.x*zoom + area.w / 2, a.y*zoom - center.y*zoom + area.h / 2, a1.x*zoom - center.x*zoom + area.w / 2, a1.y*zoom - center.y*zoom + area.h / 2);
		SDL_RenderDrawLine(renderer, b.x*zoom - center.x*zoom + area.w / 2, b.y*zoom - center.y*zoom + area.h / 2, b1.x*zoom - center.x*zoom + area.w / 2, b1.y*zoom - center.y*zoom + area.h / 2);
		
		SDL_RenderDrawLine(renderer, road[r].first.x*zoom - center.x*zoom + area.w / 2, road[r].first.y*zoom - center.y*zoom + area.h / 2, road[r-1].first.x*zoom - center.x*zoom + area.w / 2, road[r-1].first.y*zoom - center.y*zoom + area.h / 2);

		a = a1;
		b = b1;
	}

	SDL_Rect car1;
	car1.w = 20*zoom;
	car1.h = 20*zoom;
	car1.x = (car_position.x - car1.w/zoom / 2)*zoom - center.x*zoom + area.w / 2;
	car1.y = (car_position.y - car1.h/zoom / 2)*zoom - center.y*zoom + area.h / 2;
	
	
	
	SDL_RenderCopyEx(renderer, car_texture, NULL, &car1, car_angle, NULL, SDL_FLIP_NONE);
	for (auto s : car_sensor_angles) {
		SDL_RenderDrawLine(renderer, car_position.x*zoom - center.x*zoom + area.w / 2, car_position.y*zoom - center.y*zoom + area.h / 2, car_position.x* zoom+ cos(to_rad(car_angle + s.first.first-90))*s.first.second*zoom - center.x*zoom + area.w / 2, car_position.y*zoom + sin(to_rad(car_angle + s.first.first-90))*s.first.second*zoom - center.y*zoom + area.h / 2);
		if (s.second != 0) {
			SDL_Rect sensor = {
				car_position.x* zoom + cos(to_rad(car_angle + s.first.first - 90))*s.first.second*zoom - center.x*zoom + area.w / 2-3*zoom,
				car_position.y*zoom + sin(to_rad(car_angle + s.first.first - 90))*s.first.second*zoom - center.y*zoom + area.h / 2-3*zoom,
				6*zoom,
				6*zoom};
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
			SDL_RenderFillRect(renderer,&sensor);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}
	}

}

float Simulation::run(NeuralNet* net) {
	float cicle = 0;
			vector<double> inputs;
			for (auto s : car_sensor_angles) {
					inputs.push_back(s.second);
			}
			net->load_inputs(&inputs, false);
			vector<double> output = net->calculate_output();
			car_velocity = newmap(output[0], 0, 1, 0, car_max_velocity);
			car_angle += newmap(output[1], 0, 1, -car_max_angle, car_max_angle);
			
			if (car_angle > 360)car_angle -= 360;
			if (car_angle < -360)car_angle += 360;
			
			if (!car_crashed) {
				if (distance(car_position, road[road.size() - 1].first) < 50) {
					for (int i = 0; i < 5; i++)
						generate_road();
				}
				if ((distance(car_position, road[road.size() - 1].first) > 200)||
					(car_angle+275)-last_angle>150 ||
					(car_angle + 275)-last_angle<-150
					) {
					car_crashed = true;
				}

				car_position.x += cos(to_rad(car_angle - 90))*car_velocity;
				car_position.y += sin(to_rad(car_angle - 90))*car_velocity;

				for (auto& s : car_sensor_angles) {
					SDL_Point sensor = { car_position.x + cos(to_rad(car_angle + s.first.first -90))*s.first.second,car_position.y + sin(to_rad(car_angle + s.first.first -90))*s.first.second };



					float angle = angular_coefficient(road[1].first, road[0].first);
					SDL_Point a;
					a.x = road[0].first.x + sqrt(pow(road[0].second, 2) / (1 + 1 / pow(angle, 2)));
					a.y = -1 / angle*(a.x - road[ 0].first.x) + road[0].first.y;
					SDL_Point b;
					b.x = road[0].first.x - sqrt(pow(road[0].second, 2) / (1 + 1 / pow(angle, 2)));
					b.y = -1 / angle*(b.x - road[ 0].first.x) + road[0].first.y;

					for (int r = 1; r < road.size(); r++) {
						float angle = angular_coefficient(road[r].first, road[r-1].first);
						SDL_Point a1;
						a1.x = road[r ].first.x + sqrt(pow(road[r ].second, 2) / (1 + 1 / pow(angle, 2)));
						a1.y = -1 / angle*(a1.x - road[r ].first.x) + road[r ].first.y;
						SDL_Point b1;
						b1.x = road[r ].first.x - sqrt(pow(road[r ].second, 2) / (1 + 1 / pow(angle, 2)));
						b1.y = -1 / angle*(b1.x - road[r ].first.x) + road[r ].first.y;


						if (do_intersect(a, a1, sensor, car_position)){
							s.second = intersection(car_position, sensor, a, a1);
							break;
						
						}
						if (do_intersect(b, b1, sensor, car_position)) {
							s.second = intersection(car_position, sensor, b, b1);
							break;
						}
						if(!do_intersect(a, a1, sensor, car_position) && !do_intersect(b, b1, sensor, car_position))
						{
							s.second = 0;
						}


						a = a1;
						b = b1;

					}
					


				}
			}
			cicle+=car_velocity*10/distance(car_position,road[road.size()-1].first);
		
		return cicle;
}

