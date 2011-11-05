#include "TrainingSet.h"

typedef std::vector<double> Data;

void TrainingSet::addData(Data &input, Data &desOutput) {
	data_.push_back(boost::make_tuple(input, desOutput));
}

Data& TrainingSet::getInput(int index) {
	return data_[index].get<0>();
}

Data& TrainingSet::getDesiredOutput(int index) {
	return data_[index].get<1>();
}

int TrainingSet::size() {
	return data_.size();
}
