#pragma once
#include <fstream>


void LZW_encode(std::ifstream& infile, std::ofstream& outfile);

void LZW_decode(std::ifstream& infile, std::ofstream& outfile);
