#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string>
#include <cstring>
#include <map>
#include <cstdlib>

using namespace std;
int fd=0;

map<std::string,std::string>listCommands={
    {"FF00FF00","spotify"},
    {"FE01FF00","pkill spotify"},
    {"FD02FF00","playerctl play-pause"},
    {"FC03FF00","playerctl next"},
    {"FB04FF00","playerctl previous"},
    {"FA05FF00","amixer set Master 10%+"},
    {"F906FF00","amixer set Master 10%-"},
    {"F807FF00","shutdown now"},
};


int openSerialPort(const char*path)
{
    fd=open(path,O_RDWR|O_NOCTTY|O_SYNC);

    if(fd<=0)
    {
        cerr<<"Could not opening serial port!\n";
        return -1;
    }


    struct termios tty;
    memset(&tty,0,sizeof(tty));

    if(tcgetattr(fd,&tty)!=0)
    {
        cerr<<"tcgetattr failed!\n";
        return -1;
    }

    cfsetospeed(&tty, B9600); // Baud rate: 9600
    cfsetispeed(&tty, B9600);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits per byte
    tty.c_iflag &= ~IGNBRK;                     // Disable break processing
    tty.c_lflag = 0;                            // No signaling chars, no echo, no canonical processing
    tty.c_oflag = 0;                            // No remapping, no delays
    tty.c_cc[VMIN]  = 1;                        // Read doesn't block
    tty.c_cc[VTIME] = 1;  

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // Enable receiver and ignore modem control lines
    tty.c_cflag &= ~(PARENB | PARODD); // No parity bit
    tty.c_cflag &= ~CSTOPB; // Only one stop bit
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        cerr << "Error from tcsetattr!" << endl;
        return -1;
    }

    return 0;
}

int isValidText(int bytesRead,char *buffer)
{
    for(int i=0;i<=bytesRead;i++)
    {
        if(buffer[i]!=10)
            return 1;
    }
    return 0;
}


int command(const char *signal)
{
    std::string key(signal);

    pid_t pid = fork();
    if(pid==0)
    {
        if (listCommands.find(key) != listCommands.end()) 
        {
            printf("%s : %s\n",key.c_str(), listCommands[key].c_str());

            int returnValue=system(listCommands[key].c_str());
        } else 
        {
            printf("Key(%s) not found in listCommands.\n",key.c_str());
        }
        
        exit(0);
    }

    return 0;
}

int main()
{
    const char *path="/dev/ttyACM0";
    if(openSerialPort(path)==-1)
    {
        return -1;
    }
    printf("Connected to the Arduino!\n");
    char buffer[256];
    char signal[9];memset(signal,0,sizeof(signal));
    int signalLength=0;
    
    while (true) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(fd, buffer, sizeof(buffer) - 1);

        if (bytesRead > 0 && isValidText(bytesRead, buffer) == 1) {
            int bufferLength = strlen(buffer);
            int i=0;

            /// 
            while (i < bufferLength) 
            {
                
                if ((buffer[i] >= '0' && buffer[i] <= '9') || (buffer[i] >= 'A' && buffer[i] <= 'F')) {
                    signal[signalLength++] = buffer[i++];
                } else 
                {
                    i++; 
                }

                // signal fully reconstructed
                if (signalLength == 8) {
                    signal[8] = '\0';
                    //printf("Reconstructed signal: %s\n", signal);
                    if(command(signal)==1)
                    {
                        printf("Command failed\n", signal);
                    }
                    // Reset signal buffer
                    memset(signal, 0, 9);
                    signalLength = 0;
                }
            }
        } else if (bytesRead < 0) {
            std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
        }
    }

    close(fd);
    printf("Done");
    return 0;
}