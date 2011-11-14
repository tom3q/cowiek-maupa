#include "TrainingSet.h"

typedef std::vector<double> Data;

void TrainingSet::addData(Data &input, Data &desOutput) {
	inputs_.push_back(input);
	outputs_.push_back(desOutput);
}

Data& TrainingSet::getInput(int index) {
	return inputs_[index];
}

Data& TrainingSet::getDesiredOutput(int index) {
	return outputs_[index];
}

int TrainingSet::size() {
	return inputs_.size();
}
}
