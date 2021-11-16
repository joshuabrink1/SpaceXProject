#ifndef SATELLITE_H
#define SATELLITE_H
#include <iostream>
using namespace std;
#include "SatelliteIterator.h"
/**
 * THE LEDS :
 *  Prototype participant in the prototype
 * 
 *  TODO: define a pure virtual clone()
 */
class FalconRockets;
class SatelliteIterator;
class Satellite
{
protected:
    bool isLaunched; 
public:
    Satellite(/* args */);
    bool launchSatellite( FalconRockets* transport);
    virtual Satellite* clone()=0;
    virtual SatelliteIterator* createIterator()=0;
    ~Satellite();
};


#endif