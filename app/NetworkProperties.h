#ifndef NETWORK_PROPERTIES_H
#define NETWORK_PROPERTIES_H

#include <QLinkedList>
#include "Layer.h"

class NetworkProperties
{
public:
	NetworkProperties();
	~NetworkProperties();

	int a;
	int b;
	int nMax;
	int nMin;

	QLinkedList<Layer> layers;
};

#endif
