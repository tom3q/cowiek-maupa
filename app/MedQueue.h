#ifndef MEDQUEUE_H
#define MEDQUEUE_H

#include <set>
#include <queue>

const int DATA_SIZE = 11;
const int HISTORY_SIZE = 31;

class MedQueue
{
public:
	MedQueue();
	MedQueue(int capacity, int hist);

	void push(float);
	float median();
	bool check(float);
	void clear();

private:
	std::multiset<float> _dataOrdered;
	std::queue<float> _dataQueue, _medQueue;
	int _cap, _hist;
};

#endif
