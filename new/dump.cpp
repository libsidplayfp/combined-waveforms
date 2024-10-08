/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2023 Leandro Nini <drfiemost@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

// g++ $CXXFLAGS -std=c++17 dump.cpp -o dump

#include "chips.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cassert>

using ref_vector_t = std::vector<unsigned int>;

/**
 * Read sampled values for specific waveform and chip.
 */
static ref_vector_t ReadChip(int wave, const char* chip)
{
    std::cout << "Reading wave: " << wave << " for chip " << chip << std::endl;

    std::ostringstream fileName;
    fileName << "sidwaves/" << chip << "/6581wf" << wave << "0.dat.prg";
    std::ifstream ifs(fileName.str().c_str(), std::ifstream::in);
    if (!ifs.is_open())
    {
        std::cout << "Error opening file " << fileName.str() << std::endl;
        exit(EXIT_FAILURE);
    }
    ref_vector_t result;
    char buffer[4098];
    ifs.read(buffer, 4098);
    for (int i=2; i<4098; i++)
    {
        result.push_back(static_cast<unsigned char>(buffer[i]));
    }
    return result;
}

int main(int argc, const char* argv[])
{
    for ( int wave : { 3,5,6,7 } )
    {
        std::string rows[4096 + 1];

        for (const char* chip: chips)
        {
            ref_vector_t reference = ReadChip(wave, chip);
            int i=0;
            rows[i++].append(chip).append(",");
            for (unsigned int val: reference)
            {
                rows[i++].append(std::to_string(val)).append(",");
            }
        }

        std::ostringstream fileName;
        fileName << "wave0" << wave << ".csv";
        std::cout << "Saving " << fileName.str() << std::endl;
        std::ofstream ofs(fileName.str().c_str());
        for (std::string row: rows)
        {
            ofs << row << std::endl;
        }
    }
}
