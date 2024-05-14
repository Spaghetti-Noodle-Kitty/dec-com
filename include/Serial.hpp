#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstring>

#define __PORT "/dev/ttyUSB0"

class Serial {
public:
    static int* InitTTY(char* TTYPath);
    static bool SendPacket(int& TTYREF);
};
/// @brief Initializes passed TTY to Serial connection using 9600 Baud
/// @param TTYPath A path to the TTY, ignores the nullptr in default and uses /dev/ttyUSB0
/// @return intptr to the initialized TTY
int* Serial::InitTTY(char* TTYPath = nullptr) {
    int SP = open(__PORT, O_RDWR);

    if(SP < 0) {
        return nullptr;
    } else {
        struct termios tty;
        memset(&tty, 0, sizeof(tty));
        if (tcgetattr(SP, &tty) != 0) {
            return nullptr;
        }

        cfsetospeed(&tty, B9600);
        cfsetispeed(&tty, B9600);

        tty.c_cflag |= (CLOCAL | CREAD);
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_oflag &= ~OPOST;

        tcsetattr(SP, TCSANOW, &tty);

        return &SP;
    }
}

/// @brief Send a serialized packet over the TTY connection
/// @param TTYRef intptr to previously initialized TTY
/// @return true if packet has been sent, false if packet sending failed
bool Serial::SendPacket(int& TTYRef) {

}