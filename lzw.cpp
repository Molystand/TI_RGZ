#include "lzw.h"
#include <map>
#include <string>


// Возвращает таблицу, содержащую все односимвольные фразы и их коды.
std::map<std::string, int> create_dictionary()
{
    std::map<std::string, int> dictionary;
    int code = 0;
    std::string str;

    for (char ch = ' '; ch <= 'z'; ch++, code++)
    {
        dictionary[str = ch] = code;
    }
    dictionary["\n"] = code;

    return dictionary;
}

// Возвращает таблицу, содержащую коды всех односимвольных фраз и сами эти фразы.
std::map<int, std::string> create_reverse_dictionary()
{
    std::map<int, std::string> dictionary;
    int code = 0;

    for (char ch = ' '; ch <= 'z'; ch++, code++)
    {
        dictionary[code] = ch;
    }
    dictionary[code] = '\n';

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
        /*infile.read((char*)&symbol, sizeof(symbol));//*/symbol = infile.get();      // Считываем очередной символ
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
            outfile.write((char*)&dictionary[current_str], sizeof(dictionary[current_str]));//outfile << dictionary[current_str] << ' ';      // Записываем в файл код строки X
            current_str = symbol;                           // X = Y
        }
    }

    // Записываем последний символ
    std::map<std::string, int>::iterator it;
    if ((it = dictionary.find(current_str)) != dictionary.end())
    {
        outfile.write((char*)&it->second, sizeof(it->second));//outfile << it->second;
    }
}

void LZW_decode(std::ifstream& infile, std::ofstream& outfile)
{
    // Инициализируем словарь
    std::map<int, std::string> dictionary = create_reverse_dictionary();

    int code;                   // Очередной код из сообщения (Y)
    int old_code;               // Код (X)
    std::string out_str;

    infile.read((char*)&code, sizeof(code));//infile >> code;
    /*outfile.write((char*)&dictionary[code], sizeof(dictionary[code]));//*/outfile << dictionary[code];
    old_code = code;

    while (true)
    {
        infile.read((char*)&code, sizeof(code));//infile >> code;         // Считываем очередной код
        if (infile.eof())
            break;

        int size = dictionary.size();
        if (code < size)        /// Если фраза с кодом X есть в словаре
        {
            /*outfile.write((char*)&dictionary[code], sizeof(dictionary[code]));//*/outfile << dictionary[code];                                        /// Записываем в файл фразу с кодом X
            dictionary[size] = dictionary[old_code] + dictionary[code][0];      // Заносим фразу с кодом XY в словарь
            old_code = code;
        }
        else
        {
            out_str = dictionary[old_code] + dictionary[old_code][0];
            /*outfile.write((char*)&out_str, sizeof(out_str));//*/outfile << out_str;
            dictionary[size] = out_str;
            old_code = code;
        }
    }

    /*outfile.write((char*)&dictionary[code], sizeof(dictionary[code]));//*/outfile << dictionary[code];
}
