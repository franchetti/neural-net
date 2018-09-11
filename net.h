#include "functions.h"

struct Weight {
public:
	double value = 0; //valore del collegamento ocn il neurone
};

struct Neuron {
public:	
	double value = 0; //attuale valore del neurone outtenuto tramite la funzione sigmoid quindi è un numero compreso tra 0 e 1
	vector<Weight> weights; //collegamenti con gli altri neuroni
};

struct Layer {
public:
	vector<Neuron> neurons;
};

class NeuralNet {

public:

	vector<Layer> layers;

	int generate_net(vector<int> model, float min = -1, float max = 1, float bias = 1);
	int load_inputs(vector<double>* inputs, bool normalize_inputs);
	int load_net(string path);
	int save_net(string path);
	vector<double> calculate_output();


	void mutate() {
		int l = Random::random_value(0, layers.size() - 1);
		int n = Random::random_value(0, layers[l].neurons.size() - 1);
		int w = Random::random_value(0, layers[l].neurons[n].weights.size() - 1);
		layers[l].neurons[n].weights[w].value = Random::random_value(-1, 1);

	}
	NeuralNet crossingover(NeuralNet b, float prob = 50) {
		NeuralNet net = *this;
		for (int l = 0; l < layers.size(); l++) {
			for (int n = 0; n < layers[l].neurons.size(); n++) {
				for (int w = 0; w < layers[l].neurons[n].weights.size(); w++) {
					if (rand() % 100 >= prob) {
						net.layers[l].neurons[n].weights[w] = b.layers[l].neurons[n].weights[w];
					}
				}
			}
		}
		return net;
	}
};

int NeuralNet::generate_net(vector<int> model, float min, float max , float bias) {

	for (int l = 0; l < model.size(); l++){
		Layer layer;
		for (int n = 0; n < model[l]; n++) {
			Neuron neuron;
			for (int w = 0; l != model.size() - 1 && w < model[l + 1] ; w++) {
				Weight weight;
				weight.value = Random::random_value(min, max);
				neuron.weights.push_back(weight);
			}
			layer.neurons.push_back(neuron);
		}
		if (l < model.size() - 1) {
			//Bias
			Neuron neuron;
			neuron.value = bias;
			for (int w = 0; w < model[l + 1]; w++) {
				Weight weight;
				weight.value = Random::random_value(min, max);
				neuron.weights.push_back(weight);
			}
			layer.neurons.push_back(neuron);
		}
		layers.push_back(layer);
    }

	return 1;
}

int NeuralNet::load_inputs(vector<double>* inputs, bool normalize_inputs) {
	for (int neuron = 0; neuron < layers[0].neurons.size() - 1 && neuron < (*inputs).size(); neuron++) {
		double input = (*inputs)[neuron];
		//se input non è compreso tra 0 e 1 allora riducilo a un numero tra 0 e 1 se 
		if (normalize_inputs == true) {
			if (input < 0 || input > 1) {
				input = sigmoid(input);
			}
		}
		layers[0].neurons[neuron].value = input;
	}
	return 1;
}

int NeuralNet::load_net(string path) {
	ifstream in(path);
	for (auto& layer : layers) {
		for (auto& neuron : layer.neurons) {
			for (auto& weight : neuron.weights) {
				in >> weight.value;

			}

		}
	}
	
	return 1;
}

int NeuralNet::save_net(string path) {
	ofstream out(path);
	for (auto& layer : layers) {
		for (auto& neuron : layer.neurons) {
			for (auto& weight : neuron.weights) {
				out << weight.value;
				out << " ";
			}
			out << endl;
		}
	}
	

	return 1;
}

vector<double> NeuralNet::calculate_output() {

	//calcolo del valore di ogni valore seguendo la formula value = sigmoid della sommatoria di ogni neurone collegato ad esso per la sua weight
	for (int layer = 1; layer < layers.size(); layer++) {
		for (int neuron = 0; neuron < layers[layer].neurons.size(); neuron++) {
			if (layer < layers.size() - 1) {//esclude il bias
				if (neuron != layers[layer].neurons.size() - 1) {
					double sum=0;
					for (int previus_neuron = 0; previus_neuron < layers[layer - 1].neurons.size(); previus_neuron++) {
						sum += layers[layer - 1].neurons[previus_neuron].value * layers[layer - 1].neurons[previus_neuron].weights[neuron].value;
					}
					layers[layer].neurons[neuron].value = sigmoid(sum);
				}
			}
			else
			{
				double sum = 0;
				for (int previus_neuron = 0; previus_neuron < layers[layer - 1].neurons.size(); previus_neuron++) {
					sum += layers[layer - 1].neurons[previus_neuron].value * layers[layer - 1].neurons[previus_neuron].weights[neuron].value;
				}
				layers[layer].neurons[neuron].value = sigmoid(sum);
			}
		}
	}
	//creazione del vettore output
	vector<double> output;
	//inizializzazione output copiando i valori dei neuroni nell ultimo layer
	output.resize(layers[layers.size() - 1].neurons.size());
	for (int neuron = 0; neuron < layers[layers.size() - 1].neurons.size(); neuron++) {
		output[neuron] = layers[layers.size() - 1].neurons[neuron].value;
	}
	return output;
}