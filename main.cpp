#include <iostream>
#include <vector>
#include <bitset>

int main(int argc, char** argv) {
    int c[2]{0};
    int p1[4]{0};
    int p2[8]{0};
    int p3[16]{0};
    
    std::string outline;
    uint16_t RLZ1 = 0x7DEC;
    std::vector<uint64_t> RLZ2 = {0x0000043218574BAB, 0x5431BAAB98754891};
    
    std::cout << "Введите кол-во тактов цикла:" << std::endl;
    int count, t, A, B;
    int i = 0;
    std::cin >> count;
    
    char gamma = 0;
    
    // Генерация гаммы
    while(i < count) {
        auto bit1 = (RLZ1 & 0x01) ^ ((RLZ1 >> 7) & 0x01);
        RLZ1 = (RLZ1 >> 1) | (bit1 << 15);
        
        auto bit2 = (RLZ2.at(1) & 0x01) ^ ((RLZ2.at(1) >> 31) & 0x01);
        RLZ2[0] = (RLZ2[0] >> 1) | ((RLZ2[1] & 0x01) << 63);
        RLZ2[1] = (RLZ2[1] >> 1) | (bit2 << 63);
        
        auto bit = bit1 ^ bit2;
        c[bit] += 1;
        
        if(i > 1) {
            p1[gamma & 0x3]++;
        }
        if(i > 2) {
            p2[gamma & 0x7]++;
        }
        if(i > 3) {
            p3[gamma & 0xF]++;
        }
        
        i++;
        gamma = gamma << 1 | bit;
        outline.append(std::to_string(bit));
    }
    /*
    std::cout << "Частота появления нулей: "<<(double)c[0]/count << std::endl;
    std::cout << "Частота появления единиц: "<<(double)c[0]/count << std::endl;
    std::cout << "Частоты появления двубайтовых последовательностей" << std::endl;
    std::cout << (double)p1[0]/count<<' '<< (double)p1[1]/count<<' '<< (double)p1[2]/count<<' '<< (double)p2[3]/count <<std::endl;*/
    std::cout << "\n=== Битовые последовательности ===\n";
    
    // 1-битные (c)
    std::cout << "\n1-битные (c):\n";
    std::cout << "0: " << c[0] << "\n";
    std::cout << "1: " << c[1] << "\n";
    
    // 2-битные (p1)
    std::cout << "\n2-битные (p1):\n";
    for (int i = 0; i < 4; ++i) {
        std::bitset<2> pattern(i);
        std::cout << pattern << ": " << p1[i] << "\n";
    }
    
    // 3-битные входные (p2)
    std::cout << "\n3-битные входные (p2):\n";
    for (int i = 0; i < 8; ++i) {
        std::bitset<3> pattern(i);
        std::cout << pattern << ": " << p2[i] << "\n";
    }
    
    // 4-битные входные (p3)
    std::cout << "\n4-битные входные (p3):\n";
    for (int i = 0; i < 16; ++i) {
        std::bitset<4> pattern(i);
        std::cout << pattern << ": " << p3[i] << "\n";
    }
    // Вычисление АКФ для t от 1 до 31
    for(t = 1; t <= 31; t++) {
        A = 0;
        B = 0;
        
        std::string akf_line = outline.substr(0, outline.length() - t);
        for(int i = 0; i < akf_line.length(); i++) {
            if(akf_line[i] == outline[i + t]) {
                A++;
            } else {
                B++;
            }
        }
        
        std::cout << "t = " << t << ": " << (double)(A - B)/(A + B) << std::endl;
    }
    
    return 0;
}