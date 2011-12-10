#include "MedQueue.h"

MedQueue::MedQueue()
	: _cap(DATA_SIZE), _hist(HISTORY_SIZE)
{
}

MedQueue::MedQueue(int capacity, int hist)
	: _cap(capacity), _hist(hist)
{
}

void MedQueue::push(float x) {
	/* Keep median history */
	if (_dataQueue.size() > 0) {
		if (_medQueue.size() == _hist)
			_medQueue.pop();
		_medQueue.push(median());
	}

	/* If data queue is full, pop oldest element */
	if (_dataQueue.size() == _cap) {
		_dataOrdered.erase(_dataOrdered.find(_dataQueue.front()));
		_dataQueue.pop();
	}

	/* Add new data */
	_dataOrdered.insert(x);
	_dataQueue.push(x);
}

float MedQueue::median() {
	/* No data to calculate median */
	if (_dataOrdered.size() == 0)
		return 0;

	/* Return median */
	std::multiset<float>::iterator it = _dataOrdered.begin();
	for (int i=0; i<_dataOrdered.size()/2; i++)
		it++;

	return *it;
}

bool MedQueue::check(float eps) {
	if (_medQueue.size() != _hist)
		return false;
	return fabs(_medQueue.front() - median()) < eps;
}

void MedQueue::clear() {
	while (!_dataQueue.empty()) _dataQueue.pop();
	while (!_medQueue.empty()) _medQueue.pop();
	_dataOrdered.clear();
}
