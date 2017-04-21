#include <wiringPi.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <unistd.h>

using namespace std;
/*
    Thanks to http://tech.jolowe.se/arduino-home-automation-project/
    
    Packet structure:
    Bit nbr:  Name:
    01-52     Transmitter code. 26 bits, sent as 52 (every other bit is the inverse of previous)
    53-54     Group On(01), Off(10)
    55-56     On(01), Off(10) (or Dim(11)?)
    57-60     Channel. 1=1010, 2=1001, 3=0110, 4=0101
    61-64     Switch.  1=1010, 2=1001, 3=0110, 4=0101
    (65-73    Dimmer value, 16 steps. (optional))

    #                10        20        30        40        50           60
    #       1234567890123456789012345678901234567890123456789012 34 56 7890 1234
    ----------------------------------------------------------------------------
    #1 On:  1010100101101001010101100101011001010101010101010110 10 01 0101 0101
    #1 Off: 1010100101101001010101100101011001010101010101010110 10 10 0101 0101
    #2 On:  1010100101101001010101100101011001010101010101010110 10 01 0101 0110
    #2 Off: 1010100101101001010101100101011001010101010101010110 10 10 0101 0110
    #3 On:  1010100101101001010101100101011001010101010101010110 10 01 0101 1001
    #3 Off: 1010100101101001010101100101011001010101010101010110 10 10 0101 1001
    Gr On:  1010100101101001010101100101011001010101010101010110 01 01 0101 0101
    Gr Off: 1010100101101001010101100101011001010101010101010110 01 10 0101 0101
*/
class Proove
{
public:
    Proove(int gpio, int transmitter_id_dec);
    ~Proove();
    void channel_on(int channel_id);
    void channel_off(int channel_id);
    void group_on();
    void group_off();
private:
    int gpio_pin;
    const int tx_repeat = 4;

    const int tOneHigh = 250; //275
    const int tOneLow = 250; //170

    const int tZeroHigh = 250;
    const int tZeroLow = 1250;

    const int tSyncHigh = 250;
    const int tSyncLow = 2500;

    const int tPauseHigh = 250;
    const int tPauseLow = 10000;

    string transmitter_id = "";
    const string on = "0";
    const string off = "1";
    const string channel_id[4] = {"00", "01", "10", "11"};
    const string switch_id[4] = {"00", "01", "10", "11"};
    const string dim[16] = { //16 levels
        "0000"
        "0001",
        "0010",
        "0011",
        "0100",
        "0101",
        "0110",
        "0111",
        "1000",
        "1001",
        "1010",
        "1011",
        "1100",
        "1101",
        "1110",
        "1111"
    };

    void trigger(string group, string on, string channel);
    string encode(string code);
    void tx_packet(string packet);
    void tx_sync();
    void tx_l0();
    void tx_l1();
    void tx_pause();
    void tx_waveform(int high_pulse, int low_pulse);
};