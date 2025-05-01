import matplotlib
# Используем бэкенд, который не требует GUI
matplotlib.use('Agg')  # Устанавливаем бэкенд до импорта pyplot
import matplotlib.pyplot as plt
import numpy as np
import os

def plot_byte_frequencies(original_file, masked_file, output_dir='.'):
    # Проверка существования файлов
    if not os.path.exists(original_file):
        print(f"Ошибка: файл {original_file} не найден")
        return
    if not os.path.exists(masked_file):
        print(f"Ошибка: файл {masked_file} не найден")
        return
    
    # Чтение файлов и подсчет частот байтов
    with open(original_file, 'rb') as f:
        original_data = f.read()
    with open(masked_file, 'rb') as f:
        masked_data = f.read()
    
    # Подсчет частот
    original_counts = np.zeros(256, dtype=int)
    for byte in original_data:
        original_counts[byte] += 1
        
    masked_counts = np.zeros(256, dtype=int)
    for byte in masked_data:
        masked_counts[byte] += 1
    
    # Создание графиков
    plt.figure(figsize=(12, 8))
    
    # График исходных данных
    plt.subplot(2, 1, 1)
    plt.bar(range(256), original_counts, color='blue', width=1.0)
    plt.title('Частота байтов в исходном файле')
    plt.xlabel('Значение байта')
    plt.ylabel('Частота')
    plt.xlim(0, 255)
    plt.grid(True, axis='y', alpha=0.3)
    
    # Подписи осей X в hex
    ticks = [0, 32, 64, 96, 128, 160, 192, 224, 255]
    plt.xticks(ticks, [f'0x{x:02X}' for x in ticks])
    
    # График маскированных данных
    plt.subplot(2, 1, 2)
    plt.bar(range(256), masked_counts, color='red', width=1.0)
    plt.title('Частота байтов в маскированном файле')
    plt.xlabel('Значение байта')
    plt.ylabel('Частота')
    plt.xlim(0, 255)
    plt.grid(True, axis='y', alpha=0.3)
    plt.xticks(ticks, [f'0x{x:02X}' for x in ticks])
    
    plt.tight_layout()
    
    # Сохранение в файл вместо показа
    output_path = os.path.join(output_dir, 'byte_frequencies.png')
    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    plt.close()  # Закрываем figure для освобождения памяти
    
    print(f"Графики сохранены в файл: {output_path}")

if __name__ == "__main__":
    # Пример использования с обработкой ошибок
    try:
        plot_byte_frequencies('input.txt', 'output.txt')
    except Exception as e:
        print(f"Произошла ошибка: {str(e)}")