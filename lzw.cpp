#include "lzw.h"
#include <map>
#include <string>


// Возвращает таблицу, содержащую все односимвольные фразы.
std::map<std::string, int> create_dictionary()
{
    std::map<std::string, int> dictionary;
    int code = 0;
    std::string str;

    for (char ch = ' '; ch <= 'z'; ch++, code++)
    {
        dictionary[str = ch] = code;
    }

    return dictionary;
}

std::map<int, std::string> create_reverse_dictionary()
{
    std::map<int, std::string> dictionary;
    int code = 0;

    for (char ch = ' '; ch <= 'z'; ch++, code++)
    {
        dictionary[code] = ch;
    }

    return dictionary;
}

void LZW_encode(std::ifstream& infile, std::ofstream& outfile)
{
    // Инициализируем словарь
    std::map<std::string, int> dictionary = create_dictionary();

    char symbol;                    // Текущий символ (Y)
    int code = dictionary.size();
    std::string current_str;        // Текущая строка (X)

    while (true)
    {
        symbol = infile.get();      // Считываем очередной символ
        if (infile.eof())
            break;

        std::map<std::string, int>::iterator it;
        if ((it = dictionary.find(current_str + symbol)) != dictionary.end())  // Если XY есть в словаре
        {
            current_str += symbol;                          // X = XY
        }
        else
        {
            dictionary[current_str + symbol] = code++;      // Заносим XY в словарь
            outfile << dictionary[current_str] << ' ';      // Записываем в файл код строки X
            current_str = symbol;                           // X = Y
        }
    }

    // Записываем последний символ
    std::map<std::string, int>::iterator it;
    if ((it = dictionary.find(current_str)) != dictionary.end())
    {
        outfile << it->second;
    }
}

void LZW_decode(std::ifstream& infile, std::ofstream& outfile)
{
    // Инициализируем словарь
    std::map<int, std::string> dictionary = create_reverse_dictionary();

    int code;                   // Очередной код из сообщения (Y)
    int old_code;               // Код (X)
    std::string out_str;

    infile >> code;
    outfile << dictionary[code];
    old_code = code;

    while (true)
    {
        infile >> code;         // Считываем очередной код
        if (infile.eof())
            break;

        int size = dictionary.size();
        if (code < size)        /// Если фраза с кодом X есть в словаре
        {
            outfile << dictionary[code];                                        /// Записываем в файл фразу с кодом X
            dictionary[size] = dictionary[old_code] + dictionary[code][0];      // Заносим фразу с кодом XY в словарь
            old_code = code;
        }
        else
        {
            out_str = dictionary[old_code] + dictionary[old_code][0];
            outfile << out_str;
            dictionary[size] = out_str;
            old_code = code;
        }
    }

    outfile << dictionary[code];
}
