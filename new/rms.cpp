/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2024 Leandro Nini <drfiemost@users.sourceforge.net>
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

// g++ $CXXFLAGS -std=c++17 rms.cpp -o rms

#include "chips.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cassert>

using ref_vector_t = std::vector<unsigned int>;

/**
 * Read sampled values for specific waveform and chip.
 */
static ref_vector_t ReadChip(int wave, const char* chip)
{
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
    std::ofstream ofs("rms.csv");

    for (const char* chip: chips)
    {
        std::cout << "Reading waves for chip " << chip << std::endl;
        ofs << chip;

        for ( int wave : { 3,5,6,7 } )
        {
            std::cout << "Wave: " << wave;

            ref_vector_t reference = ReadChip(wave, chip);
            double sum = 0.;
            for (unsigned int val: reference)
            {
                double const x = val * val;
                sum += x;
            }
            double const rms = std::sqrt(sum/4096.0);
            std::cout << " RMS: " << rms << std::endl;
            ofs << "," << rms;
        }

        ofs << std::endl;
    }
}
