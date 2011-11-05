#ifndef TRAINING_SET_H
#define TRAINING_SET_H

#include <vector>
#include <boost\tuple\tuple.hpp>

class TrainingSet
{
public:
	typedef std::vector<double> Data;

	void addData(Data &input, Data &desOutput);
	Data& getInput(int index);
	Data& getDesiredOutput(int index);
	int size();

private:
	std::vector<boost::tuple<Data, Data > > data_;
};

#endif
