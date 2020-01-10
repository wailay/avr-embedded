#include "memoire_24.h"
#include "uart.h"
#include "debug.h"
#include "commun.h"
//Ce code est juste pour tester ma boucle dexecution vite fait,
// un refactor est necessaire.

using namespace commun;

void readSize(uint8_t fileSize, uint8_t readFileSize = 0, UART* uart = uart()){
uart.receptionUART();
readFileSize = uart->receptionUART(); 
fileSize = readFileSize;
}

void readSize(uint16_t fileSize, uint8_t readFileSize, UART* uart){
    readFileSize = uart->receptionUART();
    fileSize <<= 8;
    readFileSize = uart->receptionUART();
    fileSize += readFileSize;
}
void readData(uint8_t& readData, UART* uart){
    readData = uart->receptionUART();
}
void writeData(uint8_t readData, uint16_t& address, Memoire24CXXX* mem){
    mem->ecriture(address,readData);
    delai_ms(5);
    address++;
}
void writeData(uint16_t readData, uint16_t& address, Memoire24CXXX* mem){
    mem->ecriture(address,(uint8_t)(readData >> 8));
    address++;
    delai_ms(5);
    mem->ecriture(adress,(uint8_t)readData);
    address++;
    }


int main(){
    UART uart;
    Memoire24CXXX mem;
    DEBUG_PRINT("START \n");
    // DEBUG_PRINT_INT(0b0);
    
//    unsigned char readFileSize = 0;
    uint16_t fileSize = 0;
    uint16_t address = 0x00;
    uint8_t readData;
    
    //On sait que les deux premier octets du fichier permettent de connaitre la taille du fichier.
    readSize(fileSize,,uart);
    //Ecrire la taille du byte code
    writeData(fileSize,address,&mem);   
    uart.transmissionUART(0b01);
    for(int i = 2; i <= fileSize; i++){
        readData(readData,&uart);
        writeData(readData,address,&mem);
    }
   
    DEBUG_PRINT("FIN DE L'ECRITURE DANS LA MEMOIRE\n");
    
    return 1;
}
