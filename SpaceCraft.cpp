#include "SpaceCraft.h"

SpaceCraft::SpaceCraft() {
	this->cost = 0;
	this->capacity = 0;
	tec = nullptr;
}

SpaceCraft::SpaceCraft(double Cost, int Capacity) {
	this->cost = Cost;
	this->capacity = Capacity;
	tec = nullptr;
}

SpaceCraft::~SpaceCraft() {

}

double SpaceCraft::getCost() {
	return this->cost;
}

int SpaceCraft::getCapacity() {
	return this->capacity;
}

void SpaceCraft::setTEC(TransportEntityCollection *trec)
{
    tec = trec;
}

TransportEntityCollection* SpaceCraft::getTEC()
{
    return tec;
}



Dragon::Dragon() {

}

Dragon::Dragon(double Cost, int Capacity) : SpaceCraft (Cost,Capacity) {
	cout << "Creating dragon at cost " << fixed << setprecision(2) << getCost() << " with capacity: "<< getCapacity() << endl;
}

Dragon::~Dragon() {

}


DragonCrew::DragonCrew() {

}

DragonCrew::DragonCrew(double Cost, int Capacity) : SpaceCraft(Cost, Capacity) {
	cout << "Creating Crew dragon at cost " << fixed << setprecision(2) << getCost() << " with capacity: " << getCapacity() << endl;
}

DragonCrew::~DragonCrew() {


}

