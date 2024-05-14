#include <iostream>
#include "Packet.hpp"
#include "Serial.hpp"

int main(int argc, char *argv[])
{
    Packet* _test = new Packet("echo \"auto eth0\niface eth0 inet static\naddress 111.111.111.111\nnetmask 255.255.255.0\" >> /etc/network/interfaces");

    std::cout << _test->mData << std::endl;

    uint_8t* _testS = _test->Serialize();

    Packet* _des = new Packet("");
    _des->DeSerialze(_testS);
    std::cout << "\n\n" << _des->mData << std::endl;
}
