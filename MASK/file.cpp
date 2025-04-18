#include <fstream>
#include <iostream>
#include <vector>

int main(int argc,char** argv){
    uint16_t RLZ1 = 0x7DEC;
    std::vector<uint64_t> RLZ2 = {0x0000043218574BAB,0x5431BAAB98754891};
    std::ifstream source_file;
    std::ofstream out_file;
    source_file.open("input.txt",std::ios::binary);
    char s;
    out_file.open("output.txt",std::ios::binary);
    while(source_file.read(&s,1)){
        int count = 7;
        int i = 0;
        char gamma = 0;
        while(i <count){
            auto bit1 = (RLZ1&0x01)^((RLZ1>>7)&0x01);
            RLZ1 = (RLZ1>>1)|(bit1<<15);
            auto bit2 = (RLZ2.at(1) & 0x01) ^ ((RLZ2.at(1) >> 31) & 0x01);
            RLZ2[0] = (RLZ2[0] >> 1) | ((RLZ2[1] & 0x01) << 63);
            RLZ2[1] = (RLZ2[1] >> 1) | (bit2 << 63);
            auto bit = bit1 ^ bit2;
            gamma = (gamma << 1) | bit;
            i++;
        }
        char mask = gamma ^ s;
        
        out_file.write(&mask,1);
        
    }
    source_file.close();
    out_file.close();
    return 0;
}