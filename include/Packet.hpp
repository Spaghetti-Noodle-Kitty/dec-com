#define __MAGIC  0x5A
#define __PAD    0x7C

#define uint_16t unsigned short
#define uint_8t unsigned char

#include <cstring>

/*
Packet definition:
------------+----------+---------------+------------+---------
Start byte  | End byte | Content       | Size (byte)| Type
------------+----------+---------------+------------+---------
0           | 1        | Magic (0x5A)  | 1 byte     | uint_8t
1           | 252      | Data          | 252 bytes  | uint_8t[252]
252         | 254      | Checksum      | 2 bytes    | uint_16t
------------+----------+---------------+------------+---------

*/

/// @brief Definition for the packet class, each instance is a seperate packet
class Packet {
public:
    uint_8t mMagic = __MAGIC;
    uint_8t mData[252] = {{0}};
    
    uint_8t* Serialize();
    void DeSerialze(uint_8t* Serialized);
    bool Validate(uint_16t ExpectedChecksum);
    
    Packet(char* Data) {
        // ffs i hate u strncpy, i shouldve just read the manual
        mempcpy(Packet::mData, Data, sizeof(Packet::mData));
    }
private:
    uint_16t mChecksum;
    uint_16t CreateChecksum();
    void SplitChecksum(uint_16t Checksum, uint_8t& ChecksumHigh, uint_8t& ChecksumLow);
    uint_16t CombineChecksum(uint_8t ChecksumHigh, uint_8t ChecksumLow);
};

/// @brief Serializes the Packet class with above definition
/// @return uint_8t* of the serialized Class
uint_8t* Packet::Serialize() {
    uint_8t* packet_s = new uint_8t[256];

    packet_s[0] = mMagic;
    packet_s[1] = __PAD;
    for(int i = 0; i < 252; ++i) {
        packet_s[(i+2)] = mData[i];
    }
    
    uint_8t CHigh='a';
    uint_8t CLow='b';
    uint_8t* CLowPtr = &CLow;
    uint_8t* CHighPtr = &CHigh;

    Packet::SplitChecksum(Packet::CreateChecksum(), *CHighPtr, *CLowPtr);
    packet_s[253] = CHigh;
    packet_s[254] = CLow;

    return packet_s;
}

/// @brief Deserializes the passed uint_8t* into the current instance of Packet class (validation tbd)
/// @param Serialized The uint_8t* to be deserialized
void Packet::DeSerialze(uint_8t* Serialized) {
    if (Serialized != nullptr) {
        size_t Length = strlen((char*)Serialized) + 1;
        if(Length > 2) {
            mMagic = Serialized[0];
            for(size_t i = 2; i < Length; ++i) {
                mData[i-2] = Serialized[i];
            }
        }
    }
}
/// @brief Creates a checksum by combining every uint_8t into one uint16_t
/// @return uint16_t of all uint_8ts (Hex value) in packet combined
uint_16t Packet::CreateChecksum() {
    uint_16t Checksum = 0;
    Checksum += __MAGIC;
    for(int i = 0; i < sizeof(mData); ++i) {
        Checksum += (uint_16t)mData[i];
    }
    return Checksum;
}

/// @brief Splits the Checksum into two uint_8t instances using bit-shifting
/// @param Checksum The checksum to be split
/// @param ChecksumHigh High byte of the checksum
/// @param ChecksumLow  Low byte of the checksum
void Packet::SplitChecksum(uint_16t Checksum, uint_8t& ChecksumHigh, uint_8t& ChecksumLow) {
    ChecksumHigh = (Checksum >> 8) & 0xFF;
    ChecksumLow = Checksum & 0xFF;
}

/// @brief Re-combines the checksum from its high and low bytes
/// @param ChecksumHigh High part of checksum
/// @param ChecksumLow Low part of checksum
/// @return Combined uint16_t of both checksum parts
uint_16t Packet::CombineChecksum(uint_8t ChecksumHigh, uint_8t ChecksumLow) {
    return (static_cast<uint_8t>(ChecksumHigh) << 8) | ChecksumLow;
}


/// @brief Validates packet's data during de-serialization using Magic value + checksum
/// @param ExpectedChecksum The expected checksum that the deserialized packet presents
/// @return true if packet is valid, false if packet is invalid
bool Packet::Validate(uint_16t ExpectedChecksum) {
    if(mMagic != __MAGIC) {
        return false;
    } else if (
        CreateChecksum() != ExpectedChecksum
    ) {
        return false;
    } else {
        return true;
    }
}
