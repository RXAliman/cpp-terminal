#include <iostream>
#include <conio.h>
#include <time.h>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

class System {
public:
    //System Variables
    bool isRunning;
    const int max_ch;
    string _cmdStr;
    //Constructor
    System(int max) : max_ch(max) { }
};

class Commands : public System {
public:
    //Operational Elements
    typedef void (Commands::*_cmdType)();
    typedef struct {
        string command;
        int index;
    } cmdArrIndex;
    //Private Variables
    vector<_cmdType> cmdArr;
    vector<cmdArrIndex> cmd;
    //Constructors
    Commands() : System(35) { }
    Commands(int max) : System(max) { }
    //Initializing Function
    void initCommandArray() {
        cmdArr.push_back(&Commands::Close);
        cmdArr.push_back(&Commands::Clear);
        cmdArr.push_back(&Commands::DateTime);
        cmdArr.push_back(&Commands::Help);
        cmdArr.push_back(&Commands::HelloWorld);
        cmdArr.push_back(&Commands::Terminal_);
        cmdArr.push_back(&Commands::RollDice);

        cmd.push_back(cmdArrIndex{"CLEAR",1});
        cmd.push_back(cmdArrIndex{"CLOSE",0});
        cmd.push_back(cmdArrIndex{"DATETIME",2});
        cmd.push_back(cmdArrIndex{"HELP",3});
        cmd.push_back(cmdArrIndex{"HELLOWORLD",4});
        cmd.push_back(cmdArrIndex{"ROLLDICE",6});
        cmd.push_back(cmdArrIndex{"TERMINAL",5});
    }
    //Execute Command Functions
    bool findCmd() {
        for (auto i : cmd)
            if (i.command == _cmdStr)
            {
                (this->*cmdArr[i.index])();
                return true;
            }
        return false;
    }
    //Command Functions
    void Close() { isRunning = false; }
    void Clear() { system("CLS"); }
    void DateTime() {
        string datetime;
        char buf[80];
        struct tm tstruct;
        time_t now = time(0);
        tstruct = *localtime(&now);
        strftime(buf,sizeof(buf),"%B %d, %Y | %I:%M:%S %p\n",&tstruct);
        datetime = buf;
        cout << datetime;
    }
    void Help() {
        cout << "Available Commands:\n";
        for (auto i : cmd)
            cout << "   " << i.command << endl;
    }
    void HelloWorld() { cout << "\"Hello, World!\"\n"; }
    void RollDice() {
        for (int a=1; a<=3; a++)
        {
            cout << "\rRolling Dice" << string(a,'.');
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        cout << "\nIt's " << rand()%6+1 << endl;
    }
    void Terminal_() { cout << "Terminal <c> Rovic Xavier 2022\n"; }
};

class Terminal : private Commands {
    //Private Functions
    void inputPrompt() {
        int c, ch = 0;
        _cmdStr = "";
        cout << "> ";
        while ((c=getch()) != 13) {
            if (c == 8 && ch != 0) {
                int int_t = 0;
                string temp = "";
                for (auto i : _cmdStr) int_t++;
                for (int a=0; a<int_t-1; a++) temp += _cmdStr[a];
                _cmdStr = temp;
                cout << "\r> " << _cmdStr << " \b";
                ch--;
            }
            else if (c == 27) {
                if (_cmdStr != "") {
                    _cmdStr = "";
                    cout << '\r' << string(max_ch,' ');
                    cout << "\r> ";
                    ch = 0;
                }
                else {
                    cout << '\r' << string(max_ch,' ');
                    cout << "\r> CLOSE";
                    _cmdStr = "CLOSE";
                    ch = 5;
                }
            }
            else if (ch < max_ch - 2) {
                switch (c) {
                case int('A') ... int('Z'):
                    cout << char(c);
                    _cmdStr += char(c);
                    ch++;
                    break;
                case int('a') ... int('z'):
                    cout << char(c-32);
                    _cmdStr += char(c-32);
                    ch++;
                    break;
                default:
                    break;
                }
            }
        }
        cout << endl;
    }
    void readCommand() {
        static int calls = 0;
        if (_cmdStr == "") return;
        if (!findCmd()) {
            if (calls == 10) {
                cout << "Pro Tip: Use \'HELP\'\n";
                calls = 0;
            }
            else {
                cout << "Error: Unknown Command\n";
                calls++;
            }
        }
    }
    //Initializing Function
    void initTerminal() {
        isRunning = true;
        cout << string(max_ch,'=');
        cout << "\n" << string(max_ch/2-4,' ') << "Terminal";
        cout << "\n" << string(max_ch/2-8,' ') << "<c> Rovic Xavier";
        cout << "\n" << string(max_ch/2-6,' ') << "Made in 2022\n";
        cout << string(max_ch,'=') << endl;
        initCommandArray();
    }
public:
    //Constructors
    Terminal() { initTerminal(); }
    Terminal(int char_count_max) : Commands(char_count_max) {
        if (char_count_max < 35) {
            cout << "Error: Max Character Count Too Low\n";
            isRunning = false;
        }
        else initTerminal();
    }
    //Accessor
    bool isOpen() { return isRunning; }
    //Terminal Engine
    void run() {
        inputPrompt();
        readCommand();
    }
    //Destructor
    ~Terminal() { cout << "Closing Terminal...\n"; }
};

int main() {
    srand(time(0));
    Terminal tmx;
    while (tmx.isOpen())
        tmx.run();
}
