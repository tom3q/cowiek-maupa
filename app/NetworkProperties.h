#ifndef NETWORK_PROPERTIES_H
#define NETWORK_PROPERTIES_H

#include <QLinkedList>
#include "Layer.h"

class NetworkProperties
{
public:
	NetworkProperties();
	~NetworkProperties();

	double a;
	double b;
	double nMax;
	double nMin;

	QLinkedList<Layer> layers;
};

#endif
