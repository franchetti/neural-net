#pragma once

#include "SDLnet.h"
class Genetic {
	SDLnet sdlnet;
	vector<double> graph;
	float iteration = 1;
	float time_warp = 1;
	float sleep=0;
public:
	vector<pair<float,pair<Simulation, NeuralNet>>> objects;
	Genetic(float population, vector<int> model) {
		for (int i = 0; i < population; i++) {
			Simulation s("C:/car.png", netRenderer);
			NeuralNet net;
			net.generate_net(model);
			pair<Simulation, NeuralNet> b(s, net);
			objects.push_back(pair<float, pair<Simulation, NeuralNet>>(0,b));
		}
	}
	void itinerate(float prob = 50);
};

void Genetic::itinerate(float prob) {
	sdlnet.log.push_back("Iteration: " + remove_zero(to_string(iteration)) );
	Timer t;
	float total_fitness = 0;
	float average_fitness = 0;
	for (auto& i : objects) {
		float fit = 0;
		while (!i.second.first.car_crashed) {
			for (int t = 0; t < time_warp; t++) {
				fit += i.second.first.run(&i.second.second);
				
			}
			while (SDL_PollEvent(&event)) {
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE: 
					if(!quit)sdlnet.log.push_back("The application will close as soon as the itineration has finished");
					quit = true;

				}
				switch (event.wheel.type)
				{
				case SDL_MOUSEWHEEL:
					sdlnet.zoom(event.wheel.y);
					break;
				}
				switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						time_warp+=sdlnet.button();
						if (time_warp < 0)time_warp = 0;
					}
				}
			}



			sdlnet.draw(&i.second.second, &i.second.first, graph, netRenderer);
		}
		
		i.first = fit;
		total_fitness += fit;
		
	}
	sort(objects.begin(), objects.end(), [](pair<float, pair<Simulation, NeuralNet>> a, pair<float, pair<Simulation, NeuralNet>> b) { return a.first > b.first; });
	if (objects[objects.size() - 1].first < 0) {
		float fit = -objects[objects.size() - 1].first;
		for (auto& i : objects) {
			i.first += fit;
			total_fitness += fit;
		}
	}
	average_fitness = total_fitness / objects.size();
	vector<int> temp;
	int s = 1;
	do {
		for (int i = 0; i < objects.size(); i++) {
			int size = objects[i].first / total_fitness * 100 * s;
			for (int t = 0; t < size; t++) {
				temp.push_back(i);
			}
		}
		s *= 10;
	} while (temp.size() < 2);

	vector<pair<float, pair<Simulation, NeuralNet>>> new_objects;
	if (temp.size() != 0) {
		int tries = 0;
		for (auto& i : objects) {
			int index1 = 0, index2 = 0;
			tries = 0;
			do {
				index1 = temp[Random::random_value(0, temp.size() - 1)];
				index2 = temp[Random::random_value(0, temp.size() - 1)];
				tries++;

			} while (index1 == index2 && tries < temp.size() / 2);


			Simulation s("C:/car.png", netRenderer);

			pair<Simulation, NeuralNet> b(s, objects[index1].second.second.crossingover(objects[index2].second.second, prob));
			new_objects.push_back(pair<float, pair<Simulation, NeuralNet>>(0, b));

			if (Random::random_value(0, 100) <= 1) {
				if (new_objects.size() > 0)
					new_objects[new_objects.size() - 1].second.second.mutate();
			}



		}
		if (tries == temp.size() / 2)cout << "There is no varity left\n";
		objects = new_objects;
	}
	else
	{
		cout << "error: all dead\n";
	}
	iteration++;
	sdlnet.log.push_back("Time taken: " + remove_zero(to_string(t.get_time())));
	graph.push_back(average_fitness);
}