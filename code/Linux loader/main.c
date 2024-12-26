#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#define BLOCK_SIZE  4096
BYTE blockData[BLOCK_SIZE];
bool    endBlocks = false;
bool IsFirstBlock = true;
BYTE SegLowByte = 0;
BYTE SegHighByte = 1;   //For test

BYTE OffsetLowByte = 0;
BYTE OffsetHighByte = 0;

BYTE chkSum = 0;
int  BlocksSend = 0;
int  NumBlocks = 0;
DWORD RoundUpTo4K(DWORD size) {
    return (size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
}
int getStr(BYTE* data)
{
    int i = 0;
    while(1)
    {
        if (_kbhit()) {  // Check if a key has been pressed
            data[i] = _getch();  // Read the key without blocking
            putch(data[i]);
            if(data[i] == 0x0d)
            {
                printf("You pressed: %.*s\n", i, data);
                break;
            }
            i++;
            //break;  // Exit the loop after a key press
        }
    }
    return i;
}
int main(int argc, char *argv[]) {

    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    HANDLE hFile;
    LARGE_INTEGER fileSize;
    DWORD roundedSize;

    blockData[0] = 0xCB;    //retf for test
    // Open the serial port
    hSerial = CreateFile(
        "\\\\.\\COM20", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port\n");
        return 1;
    }

    // Set device parameters (9600 baud, 1 start bit, 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 5;
    timeouts.ReadTotalTimeoutConstant = 5;
    timeouts.ReadTotalTimeoutMultiplier = 5 ;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Send data to the serial port
    char data[256];
    DWORD bytes_written;
    if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
        printf("Error writing to serial port\n");
        CloseHandle(hSerial);
        return 1;
    }

    //printf("Data sent successfully:%i\n",bytes_written);
    
    //Sleep(10);
    // Check if data is available
    DWORD dwEventMask = EV_RXFLAG;
    if (!SetCommMask(hSerial, EV_RXCHAR)) {
        printf("Error setting CommMask\n");
        CloseHandle(hSerial);
        return 1;
    }
    
    //Sleep(5);
    BYTE read_buf[256];
    DWORD bytes_read;
    DWORD bytesToWrite;

    if(argc != 2)
    {
        printf("Usage: main <Name of file to send>\n");
        return 1;
    }
  
    // Open the file
    hFile = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Could not open file (error %d)\n", GetLastError());
        return 1;
    }

    // Get the file size
    if (!GetFileSizeEx(hFile, &fileSize)) {
        printf("Could not get file size (error %d)\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    // Round up to the nearest 4KB
    roundedSize = RoundUpTo4K((DWORD)fileSize.QuadPart);
    NumBlocks = roundedSize/BLOCK_SIZE;
    printf("Original size: %lld byte(s)\n", fileSize.QuadPart);
    


    DWORD   fileDataPtr = 0;
    BYTE    *fileData = malloc(roundedSize);
    memset(fileData,0,roundedSize);
    if(fileData == NULL)
    {
        printf("Error malloc\n");
        return 1;
    }
    DWORD   bytesRead = 0;
    if(!ReadFile(hFile, fileData, (DWORD)fileSize.QuadPart, &bytesRead, NULL))
    {
        printf("ReadFile failed (error %d)\n", GetLastError());
        return 1;
    }
    if(bytesRead != (DWORD)fileSize.QuadPart)
    {
        printf("File read wrong\n");
        return 1;
    }


    printf("Rounded size: %u bytes -> %u block(s)\n", roundedSize, NumBlocks);












    //Clear all buffers
        
    if (!PurgeComm(hSerial,PURGE_RXCLEAR | PURGE_TXCLEAR)) {
        printf("Error clearing buffers!\n");
        CloseHandle(hSerial);
        return 1;
        }
        
    //bytesToWrite = getStr(data);

    //printf("Written %d bytes\n",bytesToWrite);
    //if (!WriteFile(hSerial, data, bytesToWrite, &bytes_written, NULL)) {
    //    printf("Error writing to serial port\n");
    //    CloseHandle(hSerial);
    //    return 1;
    //}
    //printf("Loop\n");

    //Sleep(100);
    while(1)
    {
        if (!ReadFile(hSerial, read_buf, 256, &bytes_read, NULL)) {
        printf("Error reading from serial port");
        CloseHandle(hSerial);
        return 1;
        }
        if(bytes_read == 1 && read_buf[0] == 'S')   //Arduino wants a block
        {
            if(NumBlocks == BlocksSend)
            {
                data[0] = 'E';  //Say it's the end
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }
                printf("Done\n");
                return 0;
            }else if(!IsFirstBlock){

                data[0] = 'r';  //Say we redy
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }
            }

            
            if(IsFirstBlock)
            {
                data[0] = 'r';  //Say we redy
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }


                printf("Init begin\n");
                data[0] = 't';  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                data[0] = SegLowByte;  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }
                //printf("Init st 2\n");
                
                while(read_buf[0] != 'd')
                {
                    if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                    printf("Error reading from serial port\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                    //printf("Waiting redy\n");
                    //printf("Got %c\n", read_buf[0]);
                }
                //printf("Init st 3\n");
                data[0] = 'T';  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                data[0] = SegHighByte;  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                while(read_buf[0] != 'd')
                {
                    if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                    printf("Error reading from serial port\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                    //printf("Waiting redy\n");
                }



                //printf("Init st 4\n");
                data[0] = 'o';  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                data[0] = OffsetLowByte;  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                while(read_buf[0] != 'd')
                {
                    if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                    printf("Error reading from serial port\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                    //printf("Waiting redy\n");
                }
                //printf("Init st 5\n");
                data[0] = 'O';  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                data[0] = OffsetHighByte;  //Segment low byte
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                while(read_buf[0] != 'd')
                {
                    if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                    printf("Error reading from serial port\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                    //printf("Waiting redy\n");
                }
                //Sleep(5);
                printf("Init st 6\n");
                data[0] = 'S';  //Size in blocks
                bytes_written = 0;
                while(bytes_written != 1)
                {
                    if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }
                }
                
                
                data[0] = NumBlocks;
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }

                while(read_buf[0] != 'd')
                {
                    if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                    printf("Error reading from serial port\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                    //printf("Waiting redy\n");
                }
                
                printf("Init done\n");
                IsFirstBlock = false;

            }
            if(NumBlocks != BlocksSend)
            
            {  //Not the end
                //LOAD blockData here
                //update ednBlocks here
                chkSum = 0;
                if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                printf("Error reading from serial port\n");
                CloseHandle(hSerial);
                return 1;
            }
            if(bytes_read == 1 && read_buf[0] == 'S')   //Arduino wants a block
            {
                data[0] = 'r';
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }
            }
        
            data[0] = 'n';  //Say thata data is on the way
            BlocksSend++;   //increment number of blocks send
                if (!WriteFile(hSerial, data, 1, &bytes_written, NULL)) {
                    printf("Error writing to serial port\n");
                    CloseHandle(hSerial);
                    return 1;   
                }


             printf("Waiting redy to send\n");
            while(read_buf[0] != 'r')
            {
                if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                printf("Error reading from serial port\n");
                CloseHandle(hSerial);
                return 1;
            }
                //printf("Waiting redy to send got %c\n",read_buf[0]);
            }
            printf("Sending data\n");
            for(int i = 0;i < BLOCK_SIZE;i++)
            {
                chkSum += fileData[fileDataPtr];
                if (!WriteFile(hSerial, &fileData[fileDataPtr], 1, &bytes_written, NULL)) {
                printf("Error writing to serial port\n");
                CloseHandle(hSerial);
                return 1;   

            }
                fileDataPtr++;
                if(i % 256 == 0)
                {
                    printf("#");
                }
                //Sleep(1);
            }

                printf("\n");
            if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                printf("Error reading from serial port\n");
                CloseHandle(hSerial);
                return 1;
            }
            if(read_buf[0] != 'c')
            {
                printf("Error data didnt send or corupted on the way!\n");
                printf("Filed to send block(s)!\nSend %d bytes\n",bytes_written);
                printf("Got char %c\n",read_buf[0]);
                return 1;
            }else if(read_buf[0] == 'c')
            {
                data[0] = chkSum;
                //printf("Cheksum send\n");
                if (!WriteFile(hSerial, &data[0], 1, &bytes_written, NULL)) {
                printf("Error writing to serial port\n");
                CloseHandle(hSerial);
                return 1;   

            }
            Sleep(5);
            if (!ReadFile(hSerial, read_buf, 1, &bytes_read, NULL)) {
                printf("Error reading from serial port\n");
                CloseHandle(hSerial);
                return 1;
            }
             if(read_buf[0] != 'd' && read_buf[0] != 'e')
            {
                printf("Error chekSum failed!\n");
                printf("Got char %c\n",read_buf[0]);
                return 1;
            }else if(read_buf[0] == 'd'){
                //printf("Send one block!\n");
            }else{
                
                return 1;
            }

                
            }else{
                printf("Unknown code got %c\n",data[0]);
                return 1;
            }
        }   
    }
    
     
     
    }
    //Sleep(1000);
    //printf("Read %ld bytes. Received message: %x\n", bytes_read, read_buf[0]);
    //printf("Read %ld bytes. Received message: %x\n", bytes_read, read_buf[1]);
    //printf("Read %ld bytes. Received message: %x\n", bytes_read, read_buf[2]);
    //printf("Read %ld bytes. Received message: %x\n", bytes_read, read_buf[3]);
    free(fileData);
    // Close the serial port
    CloseHandle(hSerial);
     // Close the file
    CloseHandle(hFile);
    return 0;
}
