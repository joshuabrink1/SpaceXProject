#include "LowOrbit.h"

string LowOrbit::getDestination()
{
	return "Low Orbit";
}

Destination* LowOrbit::clone()
{
	return new LowOrbit();
}
