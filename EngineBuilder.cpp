//
// Created by Leonardo on 2021/11/07.
//

#include "EngineBuilder.h"
void EngineBuilder::~EngineBuilder()
{
    if(engineArray)
    {
        for (Engines* e: engineArray)
        {
            delete e;
        }
    }
    delete [] engineArray;
}


void EngineBuilder::buildEngines(int stageNumber, string rocketName)
{
    // check to see if the cores are instantiated if not then instantiate them
    if(engineArray)
    {
        for (Engines* e: engineArray)
        {
            delete e;
        }
    }
    if(stageNumber == 1)
    {
        //if we already have the coresArray we need to reset it so delete the old
        if(rocketName == "falcon9")
        {
            engineArray = new Engine*[9];
            for (int i = 0; i < 9; ++i)
            {
                engineArray = new Engine();
            }
        }
        else
        {
            engineArray = new Engine*[27];
            for (int i = 0; i < 27; ++i)
            {
                engineArray = new Engine();
            }
        }
    }
    else
    {
        if(rocketName == "falcon9")
        {
            engineArray = new Engine*[1];
            engineArray[0] = new Engine("Vacuum");
        }
        else
        {
            engineArray = new Engine*[1];
            engineArray[0] = new Engine("Vacuum");
        }
    }
}

void EngineBuilder::buildCores(int stageNumber)
{
    cout << "This shouldn't be called with this object EngineBuilder.cpp, this should be engine creation only" << endl;
}

Engine** EngineBuilder::getResult()
{
    return engineArray;
}