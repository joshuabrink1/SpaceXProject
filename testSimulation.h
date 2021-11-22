#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include "Payload.h"
#include "StarlinkCommunication.h"
#include "StarlinkGroundUser.h"
#include "CollectionIterator.h"
#include "StarlinkCollection.h"
#include "ConcreteSpaceCraftFactory.h"
#include "TransportEntity.h"
#include "TransportEntityCollection.h"
#include "SpaceCraft.h"
#include "SpaceCraftFactory.h"
#include "StarlinkVector.h"
#include "StarlinkOrbitingSatellite.h"
#include "TransportEntityCargo.h"
#include "TransportEntityCrew.h"
#include "Rocket.h"
#include "ISS.h"
#include "Earth.h"
#include "LowOrbit.h"
#include "Destination.h"
#include "RocketFactory.h"
#include "ConcreteRocketFactory.h"
#include "Build.h"
#include "Command.h"
#include "PayloadFactory.h"
#include "SimulationBackup.h"
#include "SimulationBackupStore.h"
#include "SpaceCraftCrewDragon.h"
#include "SpaceCraftDragon.h"
#include "ConcreteSpaceCraftFactory.h"
#include "TECargoCollection.h"
#include "TECrewCollection.h"
#include "TransportEntity.h"

using namespace std;

/*
class SatelliteFactory
{
public:
    Satellite *createSatellite() { return new Satellite(); };
};
 */
class TestSimulation
{
public:
    TestSimulation()
    {
        destinations[0] = new Earth();
        destinations[1] = new LowOrbit();
        destinations[2] = new ISS();

        groundUsers->add(new StarlinkGroundUser());
        groundUsers->add(new StarlinkGroundUser());
        groundUsers->add(new StarlinkGroundUser());

        comNetwork = new CommuncationNetwork(groundUsers);
    }

private:
    Destination **destinations = new Destination *[3];
    int backupCount = 0;
    SimulationBackupStore *backupStore = new SimulationBackupStore();
    Rocket *rocket = 0;
    SpaceCraft *spaceCraft;
    Command *buildCommand;
    StarlinkCollection *groundUsers = new StarlinkVector();
    EngineBuilder *eb = new EngineBuilder();
    CoreBuilder *cb = new CoreBuilder();
    CommuncationNetwork *comNetwork;
    StarlinkOrbitingSatellite *satelliteFactory = new StarlinkOrbitingSatellite();
    StarlinkCollection *starlinkCollection = new StarlinkVector();
    short satelliteCount = 0;

    double price;
    void setBuildTS(Command *c)
    {
        buildCommand = c;
    }
    void buildRocket(int rocketType)
    {
        rocket = buildCommand->executeBuild();

        RocketLeaf *stage2 = new RocketLeaf(cb, eb);
        RocketLeaf *stage1 = new RocketLeaf(cb, eb);

        CompositeStage *cs = new CompositeStage(stage1);
        cs->addRocketStage(stage2);

        switch (rocketType)
        {
        case (1):
            stage1->makeFalcon9Stage1();
            stage2->makeFalcon9Stage2();
            break;
        case (2):
            stage1->makeFalconHeavyStage1();
            stage2->makeFalconHeavyStage2();
            break;
        }

        rocket->addStage(cs);
    }
    void launch()
    {

        rocket->setLaunch(true);
        sleep(1);
        cout << "Rocket has reached 200 kilometers" << endl;
        sleep(1);
        cout << "Rocket has reached 400 kilometers" << endl;
        rocket->NextStage();
        if (!starlinkCollection->isEmpty())
        {
            cout << "Rocket has deployed " << satelliteCount << " into lower orbit" << endl;
            StarlinkCollection::iterator it = starlinkCollection->begin();
            while (!(it == starlinkCollection->end()))
            {
                cout << (*it)->getId() << " Satellite has been deployed" << endl;
                ++it;
            }
        }
    RETURN_FLIGHT:
        string message;
        string inSpaceMenu[3] = {"Communicate between Satellites", "Broadcast to Satellites", "Communicate to Ground Users"};
        short inSpaceIndex = getMenu(inSpaceMenu, 3);

        if (inSpaceIndex == 0)
        {
            goto RETURN_FLIGHT;
        }
        else if (inSpaceIndex == 1)
        {
            cout << "Message: ";
            cin >> message;
            for (int i = 3; i < satelliteCount + 3; i++)
            {
                comNetwork->broadcast(message, i);
            }
        }
        else if (inSpaceIndex == 2)
        {
            cout << "Message: ";
            cin >> message;
            comNetwork->broadcast(message);
        }
        else if (inSpaceIndex == 3)
        {
            cout << "Message: ";
            cin >> message;
            for (int i = 0; i < 3; i++)
            {
                comNetwork->broadcast(message, i);
            }
        }
        goto RETURN_FLIGHT;
        //
    }
    void setTripDestination(Destination *d)
    {
        rocket->setDestination(d);
    }
    int makeBackupTS()
    {
        backupStore->setMemento(new SimulationBackup(rocket, price));
        backupCount++;
        return backupCount;
    }

    void restoreTS(SimulationBackup *backup)
    {
        this->price = backup->getPrice();
        this->rocket = backup->getMyRocket();
    }

    void beginCountdown()
    {

        for (int i = 5; i > 0; i--)
        {
            cout << "Launch in " << i << endl;
            sleep(1);
        }

        launch();
    }

    int getMenu(string *menuArr, int arrSize)
    {
        cout << "0 - Go Back" << endl;
        int index = 0;
        for (int i = 0; i < arrSize; i++)
        {
            cout << (i + 1) << " - " << menuArr[i] << endl;
        }
        cout << "Command: ";
        cin >> index;
        cout << endl;
        return index;
    }

public:
    void start()
    {
        random_device rd;
        default_random_engine eng(rd());
        uniform_real_distribution<double> distr(0, 1000);

    MAIN_MENU:
        int choice = 0;
        cout << "MAIN MENU" << endl;
        if (rocket == nullptr)
            cout << "0 - Build Rocket" << endl;
        else
            cout << "0 - Edit Rocket" << endl;

        cout << "1 - Save Rocket" << endl;
        cout << "2 - Begin Launch" << endl;
        cout << "3 - Restore Previous Simulation" << endl;
        cout << "4 - Exit Simulation" << endl;
        cout << "Command: ";
        cin >> choice;
        cout << endl;

        if (choice == 0)
        {
        BUILD_MENU:
            if (rocket == nullptr)
            {
                RocketFactory *factory;

                starlinkCollection = new StarlinkVector();

                cout << "SELECT ROCKET TYPE" << endl;
                string rocketMenu[2] = {"Falcon 9", "Falcon Heavy"};
                short rocketIndex = getMenu(rocketMenu, 2);

                if (rocketIndex == 0)
                {
                    goto MAIN_MENU;
                }
                else if (rocketIndex == 1)
                {
                    factory = new Falcon9Factory();
                }
                else if (rocketIndex == 2)
                {
                    factory = new FalconHeavyFactory();
                }

                setBuildTS(new Build(factory, distr(eng)));
                this->buildRocket(rocketIndex);

            CONFIGURE_ROCKET:
                cout << "CONFIGURE ROCKET" << endl;
                string configMenu[3] = {"Set Destination", "Add Satellites", "Build Space Craft"};
                short configIndex = getMenu(configMenu, 3);
                if (configIndex == 0)
                    goto MAIN_MENU;

                else if (configIndex == 1)
                {
                    string destinationMenu[3] = {"Low Orbit", "International Space Station", "Earth"};
                    short destinationIndex = getMenu(destinationMenu, 3);

                    if (destinationIndex == 0)
                    {
                        goto BUILD_MENU;
                    }

                    destinationIndex--;

                    setTripDestination(destinations[destinationIndex]);
                    goto CONFIGURE_ROCKET;
                }
                else if (configIndex == 2)
                {
                    satelliteCount = 0;
                    cout << "How many satellites? (1-60) ";
                    cin >> satelliteCount;

                    for (int i = 0; i < satelliteCount; i++)
                    {
                        starlinkCollection->add(satelliteFactory->clone());
                    }

                    cout << satelliteCount << " Satellites Added" << endl;
                    cout << endl;
                    goto CONFIGURE_ROCKET;
                }
                else if (configIndex == 3)
                {
                CONFIGURE_SPACECRAFT:
                    short typeMenuIndex;

                    SpaceCraftFactory *spaceCraftFactory;
                    cout << "SELECT SPACE CRAFT TYPE" << endl;
                    string typeMenu[2] = {"Dragon", "Crew Dragon"};
                    typeMenuIndex = getMenu(typeMenu, 2);
                    if (typeMenuIndex == 0)
                        goto CONFIGURE_ROCKET;
                    if (typeMenuIndex == 1)
                    {
                        spaceCraftFactory = new DragonFactory();
                    }
                    else if (typeMenuIndex == 2)
                    {
                        spaceCraftFactory = new CrewDragonFactory();
                    }
                    cout << "Choose a Price (double): ";
                    double p;
                    cin >> p;
                    cout << endl;
                    cout << "Choose a Capacity (int): ";
                    int cap;
                    cin >> cap;

                    spaceCraft = spaceCraftFactory->createSpaceCraft(p, cap);

                    string spaceCraftMenu[2] = {"Add Cargo"};
                    short spaceCraftIndex;
                    if (typeMenuIndex == 2)
                    {
                        spaceCraftMenu[1] = "Add Crew";
                        spaceCraftIndex = getMenu(spaceCraftMenu, 2);
                    }
                    else
                    {
                        spaceCraftIndex = getMenu(spaceCraftMenu, 1);
                    }

                    if (spaceCraftIndex == 0)
                        goto CONFIGURE_ROCKET;
                    else if (spaceCraftIndex == 1)
                    {
                        int numAddCargo;
                        cout << "Please enter the number of Cargo to add: ";
                        cin >> numAddCargo;
                        // spaceCraft->setTEC(new TransportEntityCollection)
                        TransportEntityCollection *tec = new TECargoCollection();
                        for (int i = 0; i < numAddCargo; ++i)
                        {
                            TransportEntity *addCargo = new Cargo();
                            tec->add(addCargo);
                        }
                        spaceCraft->setTEC(tec);
                    }
                    else if (spaceCraftIndex == 2)
                    {

                        int numAddCrew;
                        cout << "Please enter the number of Crew to add: ";
                        cin >> numAddCrew;
                        TransportEntityCollection *tec = new TECrewCollection();
                        for (int i = 0; i < numAddCrew; ++i)
                        {
                            TransportEntity *addCrew = new Crew();
                            tec->add(addCrew);
                        }
                        spaceCraft->setTEC(tec);
                    }
                }

                // goto CONFIGURE_SPACECRAFT;
            }
            // goto CONFIGURE_ROCKET;

            else
            {
                cout << "EDIT ROCKET" << endl;
                string editMenu[2] = {"Change destination", "Edit Spacecraft"};
                short editIndex = getMenu(editMenu, 2);

                if (editIndex == 0)
                    goto MAIN_MENU;
                else if (editIndex == 1)
                {
                    string destinationMenu[3] = {"Low Orbit", "International Space Station", "Earth"};
                    short destinationIndex = getMenu(destinationMenu, 3);

                    if (destinationIndex == 0)
                        goto BUILD_MENU;

                    destinationIndex--;

                    setTripDestination(destinations[destinationIndex]);
                }
                else if (editIndex == 2)
                {
                    cout << "EDIT SPACECRAFT" << endl;
                    string editSCMenu[2] = {"Edit Crew", "Edit Cargo"};
                    short editSCIndex = getMenu(editSCMenu, 2);

                    if (editSCIndex == 0)
                        goto BUILD_MENU;
                    else if (editSCIndex == 1)
                    {
                        cout << "CREW OPTIONS" << endl;
                        string cOptions[3] = {"Display Crew", "Add Crew", "Remove Crew"};
                        short cOptionsIndex = getMenu(cOptions, 3);
                        if (cOptionsIndex == 0)
                            goto BUILD_MENU;
                        else if (cOptionsIndex == 1)
                        {
                            if (spaceCraft != nullptr && spaceCraft->getTEC() != nullptr)
                            {
                                spaceCraft->getTEC()->display();
                            }
                            else
                                cout << "SpaceCraft has not been created or no transport entity collection has been created" << endl;
                        }
                        else if (cOptionsIndex == 2)
                        {
                            int numAddCrew;
                            cout << "Please enter the number of Crew to add: ";
                            cin >> numAddCrew;
                            TECrewCollection *tec;
                            for (int i = 0; i < numAddCrew; ++i)
                            {
                                TransportEntityCrew *addCrew = new TransportEntityCrew();
                                tec->add(addCrew);
                            }
                            TransportEntityCollection *temp = spaceCraft->getTEC();
                            spaceCraft->setTEC(tec);
                            delete temp;
                        }
                        else if (cOptionsIndex == 3)
                        {
                            TransportEntityCollection *temp = spaceCraft->getTEC();
                            spaceCraft->setTEC(nullptr);
                            delete temp;
                        }
                        // Edit crew
                    }
                    else if (editSCIndex == 2)
                    {
                        cout << "CARGO OPTIONS" << endl;
                        string cOptions[3] = {"Display Cargo", "Add Cargo", "Remove Cargo"};
                        short cOptionsIndex = getMenu(cOptions, 3);
                        if (cOptionsIndex == 0)
                            goto BUILD_MENU;
                        else if (cOptionsIndex == 1)
                        {
                            if (spaceCraft != nullptr && spaceCraft->getTEC() != nullptr)
                            {
                                spaceCraft->getTEC()->display();
                            }
                            else
                                cout << "SpaceCraft has not been created or no transport entity collection has been created" << endl;
                        }
                        else if (cOptionsIndex == 2)
                        {
                            int numAddCargo;
                            cout << "Please enter the number of Cargo to add: ";
                            cin >> numAddCargo;
                            TECrewCollection *tec;
                            for (int i = 0; i < numAddCargo; ++i)
                            {
                                TransportEntityCargo *addCargo = new TransportEntityCargo();
                                tec->add(addCargo);
                            }
                            TransportEntityCollection *temp = spaceCraft->getTEC();

                            spaceCraft->setTEC(tec);
                            delete temp;
                        }
                        else if (cOptionsIndex == 3)
                        {
                            TransportEntityCollection *temp = spaceCraft->getTEC();
                            spaceCraft->setTEC(nullptr);
                            delete temp;
                        }
                    }
                }
            }

            goto MAIN_MENU;
        }
        else if (choice == 1)
        {
            cout << "Saving rocket at " << makeBackupTS() << endl;
            goto MAIN_MENU;
        }
        else if (choice == 2)
        {

            if (rocket == nullptr)
            {
                cout << "Need to build rocket before launching" << endl;
                goto MAIN_MENU;
            }
            // cout << "0 - Go Back" << endl;

            // string destinationMenu[3] = {"Low Orbit", "International Space Station", "Earth"};
            // short destinationIndex = getMenu(destinationMenu, 3);

            // if (destinationIndex == 0)
            //     goto MAIN_MENU;

            // destinationIndex--;

            // setTripDestination(destinations[destinationIndex]);

            makeBackupTS();

            beginCountdown();
        }
        else if (choice == 3)
        {
            short backupIndex = 0;
            cout << "0 - Go Back" << endl;
            cout << "Choose backup:" << endl;
            // for (int i = 0; i < backupCount; i++)
            //{
            // cout << (i + 1) << " - " << "Backup ";
            //cout << (1) << " - " << "Backup ";
            // cout << backupStore->getAt(i)->myRocket->destination->name;
            //cout << backupStore->getMemento()->getMyRocket()->getDestination();
            //}


            for (int i = 0; i < backupCount; ++i)
            {
                cout << (i+1) << " - Backup" << endl;
            }

            cin >> backupIndex;

            if (backupIndex == 0)
            {
                goto MAIN_MENU;
            }

            backupIndex--;

            // rocket = backupStore->getAt(backupIndex)->myRocket;
            rocket = backupStore->getMemento(backupIndex)->getMyRocket();
            // price = backupStore->getAt(backupIndex)->price;
            price = backupStore->getMemento(backupIndex)->getPrice();
            goto MAIN_MENU;
        }
        else
        {
            return;
        }
    }
};
