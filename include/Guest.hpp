#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Packet.hpp"
#include "Serial.hpp"

class Guest {
public:
    static bool RunCommand(char* Com, char*& Out);
    static void SendResponse(Packet* RespPacket);

private:
    static Packet& BuildResponse(char* Msg, bool Code);
};

/// @brief Runs the passed command on the linux shell
/// @param Com Command parameter
/// @param Out Reference to a char-array for output
/// @return Was command ran?
bool Guest::RunCommand(char* Com, char*& Out){
    FILE* pipe = popen(Com, "r");
    if(!pipe) return false;

    char Buff[128];
    while(!feof(pipe)){
        if(fgets(Buff, 128, pipe) != nullptr)
            strncat(Out, Buff, strlen(Out));
    }
    pclose(pipe);
    return true;
}

/// @brief Builds a response Packet with format (Status) (Output)
/// @param Msg The output of a ran command
/// @param Code Did command run successfully (OK / NOK)
/// @return Reference to the built response Packet
Packet& Guest::BuildResponse(char* Msg, bool Code){
    uint_8t Message[252] = {{0}};
    if(Code) {
        mempcpy(Message, "OK ", 3);
    } else {
        mempcpy(Message, "NOK ", 4);
    }
    mempcpy(Message, Msg, sizeof(Message) - 4);
    
    Packet* P = new Packet(Msg);
    return *P;
}


/// @brief Sends the response back to Host using initialized TTY
/// @param ResponsePacket Packet containing the response to the Host
void Guest::SendResponse(Packet* ResponsePacket){
    uint_8t* resp = ResponsePacket->Serialize();

    int* ref = Serial::InitTTY();
    Serial::SendPacket(*ref);

}