#include <iostream>
#include <vector>


int main(int argc,char** argv){
    uint16_t RLZ1 = 0x7DEC;
    std::vector<uint64_t> RLZ2 = {0x0000043218574BAB,0x5431BAAB98754891};
    std::cout << "Введите кол-во тактов цикла:" << std::endl;
    int count;
    int i = 0;
    std::cin >> count;
    while(i < count){
        auto bit1 = (RLZ1&0x01)^((RLZ1>>7)&0x01);
        RLZ1 = (RLZ1>>1)|(bit1<<15);
        auto bit2 = (RLZ2.at(1) & 0x01) ^ ((RLZ2.at(1) >> 31) & 0x01);
        RLZ2[0] = (RLZ2[0] >> 1) | ((RLZ2[1] & 0x01) << 63);
        RLZ2[1] = (RLZ2[1] >> 1) | (bit2 << 63);
        auto bit = bit1 ^ bit2;
        /*
        std::cout << "Такт:" << i << '\t';
        std::cout << bit1 << '\t' << bit2 << '\t' << bit << std::endl;*/
        std::cout << bit;
        i++;
    }
    std::cout << '\n';
}