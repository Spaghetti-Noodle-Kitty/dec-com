#pragma once

#include "Packet.hpp"

class Guest {
public:
    static bool RunCommand(char* Com, char& Out);

private:
    static Packet& BuildResponse(char* Msg, bool Code);
};

bool Guest::RunCommand(char* Com, char& Out){
    
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