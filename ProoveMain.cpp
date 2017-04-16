#include "Proove.h"
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Code ist mostly from: http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
 * @author iain
*/
class InputParser
{
public:
    InputParser (int &argc, char **argv)
    {
        for (int i=1; i < argc; ++i)
        {
            this->tokens.push_back(string(argv[i]));
        }
    }

    const string& getCmdOption(const string &option) const
    {
        vector<string>::const_iterator itr;
        itr =  find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end())
        {
            return *itr;
        }

        static const string empty_string("");
        
        return empty_string;
    }

    bool cmdOptionExists(const string &option) const
    {
        return find(this->tokens.begin(), this->tokens.end(), option)
                != this->tokens.end();
    }
private:
    vector<string> tokens;
};

int main(int argc, char **argv){
    InputParser input(argc, argv);

    if(input.cmdOptionExists("-g") && input.cmdOptionExists("-c")){
        throw "Can't use -g and -i at the same time.";
    }

    if (input.getCmdOption("-p").empty()){
        throw "-p can not be ommited";
    }

    int gpio = stoi(input.getCmdOption("-p"));
    Proove* p = new Proove(gpio);

    if(input.cmdOptionExists("-c"))
    {
        int channel = stoi(input.getCmdOption("-c"));
        if(input.cmdOptionExists("-o"))
        {
            p->channel_off(channel);
        }
        else
        {
            p->channel_on(channel);
        }
    }
    else if(input.cmdOptionExists("-g"))
    {
        if(input.cmdOptionExists("-o"))
        {
            p->group_off();
        }
        else
        {
            p->group_on();
        }
    }

    return 0;
}