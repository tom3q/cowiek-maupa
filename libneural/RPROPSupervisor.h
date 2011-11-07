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
	RPROPSupervisor(double min, double max, double a, double b);

	virtual void train();

	void setNMin(double val);
	void setNMax(double val);
	void setA(double val);
	void setB(double val);

	double getNMin() const;
	double getNMax() const;
	double getA() const;
	double getB() const;

private:
	void buildData();

	std::vector<Matrix2D> n_, errorDrv_, prevDrv_;
	std::vector<std::vector<double> > delta_;
	double nMax_, nMin_, a_, b_;
};

#endif
