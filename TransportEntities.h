#ifndef TRANSPORTENTITIES_H
#define TRANSPORTENTITIES_H

#include <string>

using namespace std;

class TransportEntities
{
public:
	TransportEntities();
	~TransportEntities();

private:

};


class Cargo :  public TransportEntities
{
public:
	Cargo();
	Cargo(string Description);
	~Cargo();

	string getDescription();

private:
	string Description;
};


class Crew : public TransportEntities
{
public:
	Crew();
	Crew(string name, string jobDescription);
	~Crew();

	string getName();
	string getJobDescription();

private:
	string name;
	string jobDescription;
};


#endif
