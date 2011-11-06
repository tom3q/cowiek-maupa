#include <cstdio>
#include <cstdarg>
#include "NeuralNetwork.h"
#include "RPROPSupervisor.h"

using namespace std;

vector<double> makeVector(int amount, ...) {
	vector<double> result;
	va_list v;
	va_start(v, amount);
	for (int i=0; i<amount; ++i) 
		result.push_back(va_arg(v, double));
	va_end(v);
	return result;
}

double abs(double a, double b) {
	return a > b ? a-b : b-a;
}

int main() {
	NeuralNetwork net;
	net.addInputLayer(2);
	net.addLayer(2, NeuralNetwork::FUNCTION_UNIPOLAR);
	net.addLayer(1, NeuralNetwork::FUNCTION_UNIPOLAR);
	
	// XOR function
	TrainingSet trainingSet;
	trainingSet.addData(makeVector(2, 0.0, 0.0), makeVector(1, 0.0));
	trainingSet.addData(makeVector(2, 0.0, 1.0), makeVector(1, 1.0));
	trainingSet.addData(makeVector(2, 1.0, 0.0), makeVector(1, 1.0));
	trainingSet.addData(makeVector(2, 1.0, 1.0), makeVector(1, 0.0));

	RPROPSupervisor supervisor;
	supervisor.setNeuralNetwork(net);
	supervisor.setTrainingSet(trainingSet);

	int epochs = 0, dead = 0, randomized = 0;
	const double errorDelta = 0.001, deadCount = 10;
	double lastError = 0, error;

	while ((error = supervisor.totalError()) > 0.001) {
		supervisor.train();
		epochs++;

		if (abs(error, lastError) < errorDelta) {
			dead++;
			if (dead == deadCount) {
				net.randomizeConnections(5.0);
				dead = epochs = 0;
				randomized++;
			}
		}
		else {
			dead = 0;
			lastError = error;
		}
	}

	printf("XOR Exmaple\nEpochs: %d (randomized %d times)\n\n", epochs, randomized);
	printf("(0.0, 0.0): %0.4f\n", net.getOutput(makeVector(2, 0.0, 0.0))[0]);
	printf("(0.0, 1.0): %0.4f\n", net.getOutput(makeVector(2, 0.0, 1.0))[0]);
	printf("(1.0, 0.0): %0.4f\n", net.getOutput(makeVector(2, 1.0, 0.0))[0]);
	printf("(1.0, 1.0): %0.4f\n", net.getOutput(makeVector(2, 1.0, 1.0))[0]);

	return 0;
}
