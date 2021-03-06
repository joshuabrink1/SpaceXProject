//
// Created by Leonardo on 2021/11/07.
//

#ifndef SPACEXPROJECT_ENGINEBUILDER_H
#define SPACEXPROJECT_ENGINEBUILDER_H

#include "StageBuilder.h"
#include "MerlinEngine.h"
#include "VacuumMerlinEngine.h"

/**
 * @class EngineBuilder
 * Concrete Builder to create engines based on product type
 */

class EngineBuilder : public  StageBuilder
{
public:

    EngineBuilder();
    ~EngineBuilder();
    void buildCores(int stageNumber, string rocketName) override;
    void buildEngines(int stageNumber, string rocketName) override;
    Engine** getResult();
    int getNumEngines(){return arrSize;};
private:
    void resetBuilder();
    int arrSize;
    Engine** engineArray;
};


#endif //SPACEXPROJECT_ENGINEBUILDER_H
