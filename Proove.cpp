#include "Proove.h"

using namespace std;

Proove::Proove(int gpio_pin)
{
    this->gpio_pin = gpio_pin;
    if(wiringPiSetupGpio() == -1){
        printf("%s \n", "Failed to setup wiringPi");
    }
    pinMode(this->gpio_pin, OUTPUT);
}

Proove::~Proove()
{
}

void Proove::channel_on(int channel_id)
{
    this->trigger(this->off, this->on, this->channel_id[channel_id]);
}

void Proove::channel_off(int channel_id)
{
    this->trigger(this->off, this->off, this->channel_id[channel_id]);
}

void Proove::group_on()
{
    this->trigger(this->on, this->on, this->channel_id[0]);
}

void Proove::group_off()
{
    this->trigger(this->on, this->off, this->channel_id[0]);
}

void Proove::trigger(string group, string state, string channel)
{
    stringstream ss;
	ss << this->transmitter_id;
	ss << group;
	ss << state;
	ss << this->channel_id[0];
    ss << channel;
	string data = ss.str();

    string packet = this->encode(data);
    for(int i = 0; i < this->tx_repeat; i++)
    {
        this->tx_packet(packet);
    }
}

string Proove::encode(string data)
{
    stringstream ss;
	for(char& c : data)
    {
        ss << c;
        if(c == '0')
        {
            ss << '1';
        }
        else
        {
            ss << '0';
        }
    }
	
    return ss.str();
}

void Proove::tx_packet(string packet)
{
    this->tx_sync();
    for(char& c : packet)
    {
        if(c == '0')
        {
            this->tx_l0();
        }
        else
        {
            this->tx_l1();
        }
    }
}

void Proove::tx_sync()
{
    this->tx_waveform(this->tSyncHigh, this->tSyncLow);
}

void Proove::tx_l0()
{
    this->tx_waveform(this->tZeroHigh, this->tZeroLow);
}

void Proove::tx_l1()
{
    this->tx_waveform(this->tOneHigh, this->tOneLow);
}

void Proove::tx_pause()
{
    this->tx_waveform(this->tPauseHigh, this->tPauseLow);
}

void Proove::tx_waveform(int high_pulse, int low_pulse)
{
    digitalWrite(this->gpio_pin, HIGH);
    usleep(high_pulse);
    digitalWrite(this->gpio_pin, LOW);
    usleep(low_pulse);
}