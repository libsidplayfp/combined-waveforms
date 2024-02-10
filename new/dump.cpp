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

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cassert>

typedef std::vector<unsigned int> ref_vector_t;

const char* chips[] =
{
    // R2
    "locu128_6581_cbm_4383",
    "cbm4383",
    "broken0384",
    "0784",
    "3084",
    "cbm3384",
    "6581_1585",
    // R3
    "r34785",
    "r34885",
    "r30486s",
    // R4
    "r4ar448614",
    "r4ar528614",
    "6581R4AR-3488_14",
    "brokenr4ar3488",
    "r4ar378914",
    "brokenr4ar378914",
    // 8580
    "8580R5_5092_25",
    "8580R5_5092_25_2",
    "8580_0590",
    "8580_3493",
    "8580_5092",
    "8580_1087",
    "8580_1088",
    "8580_1489",
    "8580_1891",
    "8580_3190",
    "8580_3491",
    "8580_3987",
    "8580_4388",
    "8580_4589"
};

std::string rows[4096];

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
        exit(-1);
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
    if (argc != 2)
    {
        std::cout << "Usage " << argv[0] << " <waveform>" << std::endl;
        exit(-1);
    }

    const int wave = atoi(argv[1]);
    assert(wave == 3 || wave == 5 || wave == 6 || wave == 7);

    for (const char* chip: chips)
    {
        ref_vector_t reference = ReadChip(wave, chip);
        int i=0;
        //rows[i++].append(chip).append(",");
        for (unsigned int val: reference)
        {
            rows[i++].append(std::to_string(val)).append(",");
        }
    }

    std::ostringstream fileName;
    fileName << "wave0" << wave << ".csv";
    std::ofstream ofs(fileName.str().c_str());
    for (std::string row: rows)
    {
        ofs << row << std::endl;
    }
}
