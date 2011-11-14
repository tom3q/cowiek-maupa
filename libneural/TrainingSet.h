#ifndef TRAINING_SET_H
#define TRAINING_SET_H

#include <vector>

class TrainingSet
{
public:
	typedef std::vector<double> Data;

	void addData(Data &input, Data &desOutput);
	Data& getInput(int index);
	Data& getDesiredOutput(int index);
	int size();

private:
	std::vector<Data> inputs_;
	std::vector<Data> outputs_;
};

#endif
