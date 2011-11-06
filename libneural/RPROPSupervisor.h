#ifndef RPROP_SUPERVISOR_H
#define RPROP_SUPERVISOR_H

#include <vector>
#include "Supervisor.h"
#include "Matrix2D.h"

class RPROPSupervisor
	: public Supervisor
{
public:
	RPROPSupervisor();
	RPROPSupervisor(double min, double max);

	void buildData();
	void train();

private:
	std::vector<Matrix2D> n_, errorDrv_, prevDrv_;
	std::vector<std::vector<double> > delta_;
	double nMax_, nMin_, a_, b_;
};

#endif
