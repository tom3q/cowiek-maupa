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

void printVector(vector<double>& v) {
	for (unsigned int i=0; i<v.size(); i++)
		printf("%0.4f\t", v[i]);
	printf("\n");
}

double abs(double a, double b) {
	return a > b ? a-b : b-a;
}

int main() {
	NeuralNetwork net;
	net.addInputLayer(2);
	net.addLayer(2, NeuralNetwork::FUNCTION_UNIPOLAR);
	net.addLayer(1, NeuralNetwork::FUNCTION_UNIPOLAR);
	
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

	while ((error = supervisor.totalError()) > 0.01) {
		supervisor.train();
		epochs++;

		if (abs(error, lastError) < errorDelta) {
			dead++;
			if (dead == deadCount) {
				// Tutaj nast¹pi ponowne losowanie krawêdzi, gdy¿ przez
				// d³u¿szy czas nie poprawi³ siê b³¹d uczenia sieci.
				// Dodatkowo nale¿y wyczyœciæ dane zgromadzone u supervisora,
				// poniewa¿ inaczej sieæ ci¹gle bêdzie trwaæ w 'martwym
				// punkcie'. 
				//
				// Mam pewn¹ koncepcjê jak ukryæ koniecznoœæ rêcznego 
				// czyszczenia danych supervisora i wprawadzê j¹ w ¿ycie
				// niebawem.

				net.randomizeConnections(5.0);
				supervisor.buildData();
				dead = epochs = 0;
				randomized++;
			}
		}
		else {
			dead = 0;
			lastError = error;
		}
	}

	printf("Epochs: %d (randomized %d times)\n\n", epochs, randomized);
	printf("(0.0, 0.0): %0.4f\n", net.getOutput(makeVector(2, 0.0, 0.0))[0]);
	printf("(0.0, 1.0): %0.4f\n", net.getOutput(makeVector(2, 0.0, 1.0))[0]);
	printf("(1.0, 0.0): %0.4f\n", net.getOutput(makeVector(2, 1.0, 0.0))[0]);
	printf("(1.0, 1.0): %0.4f\n", net.getOutput(makeVector(2, 1.0, 1.0))[0]);

	return 0;
}
