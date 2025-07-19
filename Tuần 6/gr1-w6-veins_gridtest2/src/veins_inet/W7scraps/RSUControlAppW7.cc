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

#include "RSUControlAppW7.h"
#include "Constant.h"

#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Register_Class(RSUControlAppW7);

void RSUControlAppW7::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        sendBeacon= new cMessage("send Beacon");
    }
    else if (stage == 1) {
        // Initializing members that require initialized other modules goes here
    }


}

void RSUControlAppW7::finish()
{
    //Duoc goi khi RSU ket thuc cong viec
    DemoBaseApplLayer::finish();
    // statistics recording goes here
}

void RSUControlAppW7::onBSM(DemoSafetyMessage* bsm)
{
    //for my own simulation circle
}

void RSUControlAppW7::onWSM(BaseFrame1609_4* wsm)
{
    // Your application has received a data message from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples

    cPacket* enc = wsm->getEncapsulatedPacket();
    count++;

    if(TraCIDemo11pMessage* bc = dynamic_cast<TraCIDemo11pMessage*>(enc)){
        //if(strcmp(Constant::FIRST, bc->getDemoData()) == 0){
            EV << "my message = " <<bc->getDemoData()<<" from "<<bc->getSenderAddress()<<endl;
            if(sendBeacon != NULL){
                if (sendBeacon->isScheduled())
                {
                    cancelEvent(sendBeacon);
                }

                TraCIDemo11pMessage* rsuBeacon = new TraCIDemo11pMessage();

                double vehSpd = std::stod(bc->getDemoData());
                std::string ret;

                if(vehSpd > 10 && count == 10){
                    ret = "SLOW DI NAO";
                } else{
                    ret = mergeContent(bc->getSenderAddress());
                }
                rsuBeacon->setDemoData(ret);
                rsuBeacon->setSenderAddress(myId);
                BaseFrame1609_4* WSM = new BaseFrame1609_4();
                WSM->encapsulate(rsuBeacon);
                populateWSM(WSM);
                send(WSM,lowerLayerOut);
            }
        //}
    }
}

void RSUControlAppW7::onWSA(DemoServiceAdvertisment* wsa)
{
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void RSUControlAppW7::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);
}

void RSUControlAppW7::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    // the vehicle has moved. Code that reacts to new positions goes here.
    // member variables such as currentPosition and currentSpeed are updated in the parent class

}
