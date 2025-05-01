#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

// Функция для отображения ASCII-графика
void drawAsciiChart(const size_t counts[256], const std::string& title, int max_height = 20) {
    // Найдем максимальное значение для масштабирования
    size_t max_count = *std::max_element(counts, counts + 256);
    if (max_count == 0) return; // Нечего отображать
    
    std::cout << "\n" << title << ":\n";
    
    // Горизонтальная ось (байты)
    std::cout << "      ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << std::setw(2) << i << " ";
    }
    std::cout << "\n      -------------------------------------------------\n";
    
    // Вертикальные столбцы
    for (int level = max_height; level > 0; --level) {
        std::cout << std::setw(4) << (level * max_count / max_height) << " |";
        for (int byte = 0; byte < 16; ++byte) {
            bool has_value = false;
            for (int nibble = 0; nibble < 16; ++nibble) {
                int idx = byte * 16 + nibble;
                if (counts[idx] * max_height / max_count >= level) {
                    has_value = true;
                    break;
                }
            }
            std::cout << (has_value ? "██" : "  ") << " ";
        }
        std::cout << "|\n";
    }
    
    // Нижняя граница
    std::cout << "      -------------------------------------------------\n";
}

int main(int argc, char** argv) {
    uint16_t RLZ1 = 0x7DEC;
    std::vector<uint64_t> RLZ2 = {0x0000043218574BAB, 0x5431BAAB98754891};

    std::ifstream source_file;
    std::ofstream out_file;
    
    // Счётчики частоты байтов
    size_t input_byte_counts[256] = {0};
    size_t masked_byte_counts[256] = {0};
    
    source_file.open("input.txt", std::ios::binary);
    out_file.open("output.txt", std::ios::binary);
    
    char s;
    while (source_file.read(&s, 1)) {
        uint8_t input_byte = static_cast<uint8_t>(s);
        input_byte_counts[input_byte]++;
        
        // Ваш алгоритм маскирования
        int count = 7;
        int i = 0;
        char gamma = 0;
        
        while (i < count) {
            auto bit1 = (RLZ1 & 0x01) ^ ((RLZ1 >> 7) & 0x01);
            RLZ1 = (RLZ1 >> 1) | (bit1 << 15);
            
            auto bit2 = (RLZ2.at(1) & 0x01) ^ ((RLZ2.at(1) >> 31) & 0x01);
            RLZ2[0] = (RLZ2[0] >> 1) | ((RLZ2[1] & 0x01) << 63);
            RLZ2[1] = (RLZ2[1] >> 1) | (bit2 << 63);
            
            auto bit = bit1 ^ bit2;
            gamma = (gamma << 1) | bit;
            i++;
        }
        
        char mask = gamma ^ s;
        out_file.write(&mask, 1);
        
        uint8_t masked_byte = static_cast<uint8_t>(mask);
        masked_byte_counts[masked_byte]++;
    }
    
    source_file.close();
    out_file.close();
    
    // Выводим ASCII-графики
    drawAsciiChart(input_byte_counts, "Исходное распределение байтов");
    drawAsciiChart(masked_byte_counts, "Распределение после маскирования");
    
    // Текстовый вывод статистики
    std::cout << "\nПодробная статистика:\n";
    std::cout << "Байт | Исходные | Маскированные\n";
    std::cout << "-----+----------+-------------\n";
    for (int i = 0; i < 256; ++i) {
        if (input_byte_counts[i] > 0 || masked_byte_counts[i] > 0) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << i 
                      << " | " << std::dec << std::setw(8) << input_byte_counts[i]
                      << " | " << std::setw(10) << masked_byte_counts[i] << "\n";
        }
    }
    
    return 0;
}