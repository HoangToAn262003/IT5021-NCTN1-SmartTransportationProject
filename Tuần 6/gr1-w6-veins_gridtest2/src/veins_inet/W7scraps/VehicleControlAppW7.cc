//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "VehicleControlAppW7.h"
#include "Constant.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Register_Class(VehicleControlAppW7);

void VehicleControlAppW7::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {

        int idDebug = getId();
        sendBeacon= new cMessage("send Beacon");
        if(idDebug == 15){
            mobility = TraCIMobilityAccess().get(getParentModule());
            traciVehicle = mobility->getVehicleCommandInterface();
            subscribedServiceId = -1;
            currentOfferedServiceId = 7;

            wsaInterval = 5;
            beaconInterval = 1;
        }
    }
    else if (stage == 1) {

        if (sendBeacon->isScheduled())
        {
            cancelEvent(sendBeacon);
        }
        scheduleAt(simTime() + 0.1, sendBeacon);
        EV<<"Send to RSU. Waiting for response from RSU!";
    }
}

void VehicleControlAppW7::finish()
{
    DemoBaseApplLayer::finish();
    //EV<<"Reach destination over here"<<endl;
    // statistics recording goes here
}

void VehicleControlAppW7::onBSM(DemoSafetyMessage* bsm)
{
    //for my own simulation circle
}

void VehicleControlAppW7::onWSM(BaseFrame1609_4* wsm)
{
    // Your application has received a data message from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void VehicleControlAppW7::onWSA(DemoServiceAdvertisment* wsa)
{
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void VehicleControlAppW7::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);
    // this method is for self messages (mostly timers)
    // it is important to call the DemoBaseApplLayer function for BSM and WSM transmission

    std:string curLane = traciVehicle->getLaneId();
    std:string curEdge = traciVehicle->getRoadId();

    //if(msg == sendBeacon)
    {
        TraCIDemo11pMessage* carBeacon = new TraCIDemo11pMessage();
        std::string content = "Lane: " + curLane + "; Road: " + curEdge;

        carBeacon->setDemoData(content.c_str());
        carBeacon->setSenderAddress(myId);

        BaseFrame1609_4* WSM = new BaseFrame1609_4();
        WSM->encapsulate(carBeacon);
        populateWSM(WSM);
        send(WSM,lowerLayerOut);
        return;
    }
}

void VehicleControlAppW7::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    // the vehicle has moved. Code that reacts to new positions goes here.
    // member variables such as currentPosition and currentSpeed are updated in the parent class

}

void VehicleControlAppW7::handleLowerMsg(cMessage* msg)
{
    BaseFrame1609_4* WSM = check_and_cast<BaseFrame1609_4*>(msg);
    cPacket* enc = WSM->getEncapsulatedPacket();
    if(TraCIDemo11pMessage* bc = dynamic_cast<TraCIDemo11pMessage*>(enc)){
        char *ret = mergeContent(myId);

        //const char *content = bc->getDemoData();
        if(strcmp(ret, bc->getDemoData()) == 0){
            if(strcmp(traciVehicle->getRoadId(), "E14") == 0){
                EV << "Vehicle ID " + myId + "is currently at " + traciVehicle->getRoadId()<< endl;
            }
        }
    }
    else{
        EV<<"In "<<myId<<". At "<<simTime()
                <<" no TraCIDemo11Message but from "
                <<msg->getSenderModuleId()<<endl;
    }
}


