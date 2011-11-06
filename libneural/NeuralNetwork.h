#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <boost\tuple\tuple.hpp>
#include <boost\shared_ptr.hpp>

#include "Neuron.h"
#include "Matrix2D.h"

typedef std::vector<Neuron> Neurons;
typedef boost::tuple<Matrix2D, Neurons> tuple;

class Supervisor;

class NeuralNetwork
{
public:
	NeuralNetwork();
	~NeuralNetwork();

	void addInputLayer(int inputs);
	void addLayer(int nCount, int actFunction);

	std::vector<double> getOutput(std::vector<double> &input);
	Matrix2D& connectionMatrix(int layer);
	Neurons& neurons(int layer);
	int layerCount() const;
	int outputNeuronCount() const;

	bool isCurrentSupervisor(Supervisor *supervisor);
	void setCurrentSupervisor(Supervisor *supervisor);

	void randomizeConnections(double range);

	// activation funtion types
	const static int FUNCTION_UNIPOLAR = 0;
	const static int FUNCTION_BIPOLAR = 1;
	const static int FUNCTION_LINEAR = 2;

private:
	int inputCount_, prevInputCount_;
	Supervisor* supervisor_;
	std::vector<tuple> layers_;
};

#endif
