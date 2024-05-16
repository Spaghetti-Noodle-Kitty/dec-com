# dec-com
An attempt at creating a packet-based serial data transmission utility

## The basic premise
> * Create a Host-Guest communication ability via Serialports
> * Communication is validated using checksums and a Magic

### But why?
> Quite simply to learn and to ease the configuration of VM-Guests from a Host machine

### Prerequisites
> * A virtual serial port on both the Host and the Guest
> * Knowing both of their /dev/ttyUSB names
> * A lot of luck, as this is still strongly WIP

### Disclaimer
> I do not take any responsibilty for what may happen on your system after attempting to use this.
> I do not take any responsibilty for stuff breaking or not working as expectedly.
> You are allowed to modify and use this code as per the MIT license