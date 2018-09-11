#include "genetic.h"

int main(int argc, char* args[])
{
	init();

	vector<int> model = { 7, 4, 2 };
	Genetic genetic(100,model);
	int file = 0;
	//for (auto& o : genetic.objects) {
	//	o.second.second.load_net("C:/Users/Riccardo-PC/Source/Repos/NNv0.0/NNv0.0/Saves/netsave#" + to_string(file) + ".net");
	//	file++;
	//}

	while (quit!=true) {
		genetic.itinerate(50);
	}

	file = 0;
	for (auto& o : genetic.objects) {
		o.second.second.save_net("C:/Users/Riccardo-PC/Source/Repos/NNv0.0/NNv0.0/Saves/netsave#" + to_string(file) + ".net");
		file++;
	}


	return 0;
}