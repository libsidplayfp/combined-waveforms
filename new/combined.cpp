/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2013-2022 Leandro Nini <drfiemost@users.sourceforge.net>
 * Copyright 2007-2010 Antti Lankila
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

#include <cassert>
#include <ctime>
#include <cstring>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <random>

#include "parameters.h"


static const float EPSILON = 1e-4;

#ifdef __MINGW32__
// MinGW's std::random_device is a PRNG seeded with a constant value
// so we use system time as a random seed.
#include <chrono>
inline long getSeed()
{
    using namespace std::chrono;
    const auto now_ms = time_point_cast<std::chrono::milliseconds>(system_clock::now());
    return now_ms.time_since_epoch().count();
}
#else
inline long getSeed()
{
    return std::random_device{}();
}
#endif

static std::default_random_engine prng(getSeed());
static std::normal_distribution<> normal_dist(1.0, 0.1);
static std::normal_distribution<> normal_dist2(0.5, 0.2);

static double GetRandomValue()
{
    return normal_dist(prng);
}

static float GetNewRandomValue()
{
    return static_cast<float>(normal_dist2(prng));
}

static void Optimize(const ref_vector_t &reference, int wave, const char* chip)
{
    Parameters bestparams;

    /*
     * The score here reported is the acoustic error.
     * In parentheses the number of mispredicted bits
     * on a total of 32768.
     */
#if 1
    if (strcmp(chip, "locu128_6581_cbm_4383") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 1858 (204/32768)
            bestparams.threshold = 0.886832297f;
            bestparams.distance1 = 2.14438701f;
            bestparams.distance2 = 9.51839447f;
            break;
        case 5: // PT
            // current score 850 (60/32768)
            bestparams.threshold = 0.984093368f;
            bestparams.pulsestrength = 2.36193347f;
            bestparams.distance1 = 1.01945996f;
            bestparams.distance2 = 1.06785512f;
            break;
        case 6: // PS
            // current score 10760 (593/32768)
            bestparams.threshold = 1.82748342f;
            bestparams.pulsestrength = 8.60987568f;
            bestparams.distance1 = 1.17361319f;
            bestparams.distance2 = 1.09112144f;
            break;
        case 7: // PST
            // current score 2633 (79/32768)
            bestparams.threshold = 1.29421425f;
            bestparams.pulsestrength = 4.31972408f;
            bestparams.distance1 = 0.83322084f;
            bestparams.distance2 = 1.12437904f;
            break;
        }
    }

    else if (strcmp(chip, "0784") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 14149 (434/32768)
            bestparams.threshold = 0.972974837f;
            bestparams.distance1 = 5.24314833f;
            bestparams.distance2 = 40.3477287f;
            break;
        case 5: // PT
            // current score 2016 (141/32768)
            bestparams.threshold = 0.938275278f;
            bestparams.pulsestrength = 1.70019507f;
            bestparams.distance1 = 1.10584641f;
            bestparams.distance2 = 1.11688411f;
            break;
        case 6: // PS
            // current score 16438 (825/32768)
            bestparams.threshold = 1.95848787f;
            bestparams.pulsestrength = 9.22996807f;
            bestparams.distance1 = 1.19547772f;
            bestparams.distance2 = 1.10592592f;
            break;
        case 7: // PST
            // current score 4088 (106/32768)
            bestparams.threshold = 1.14416945f;
            bestparams.pulsestrength = 3.07632709f;
            bestparams.distance1 = 0.674530327f;
            bestparams.distance2 = 1.17008042f;
            break;
        }
    }

    else if (strcmp(chip, "3084") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 11698 (644/32768)
            bestparams.threshold = 0.660794199f;
            bestparams.distance1 = 1.32682252f;
            bestparams.distance2 = 3.66723275f;
            break;
        case 5: // PT
            // current score 5911 (176/32768)
            bestparams.threshold = 0.996065319f;
            bestparams.pulsestrength = 2.00260067f;
            bestparams.distance1 = 1.0930692f;
            bestparams.distance2 = 1.1597296f;
            break;
        case 6: // PS
            // current score 22853 (888/32768)
            bestparams.threshold = 1.92156518f;
            bestparams.pulsestrength = 7.8455534f;
            bestparams.distance1 = 1.22920787f;
            bestparams.distance2 = 1.14945805f;
            break;
        case 7: // PST
            // current score 6379 (142/32768)
            bestparams.threshold = 1.10986507f;
            bestparams.pulsestrength = 2.369416f;
            bestparams.distance1 = 0.698016405f;
            bestparams.distance2 = 1.25517154f;
            break;
        }
    }

    else if (strcmp(chip, "cbm3384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 20316 (1555/32768)
            bestparams.threshold = 0.0123020485f;
            bestparams.distance1 = 0.000481111667f;
            bestparams.distance2 = 1.05254078f;
            break;
        case 5: // PT
            // current score 4936 (64/32768)
            bestparams.threshold = 0.944592297f;
            bestparams.pulsestrength = 2.04414487f;
            bestparams.distance1 = 1.0138588f;
            bestparams.distance2 = 1.12991118f;
            break;
        case 6: // PS
            // current score 24673 (1491/32768)
            bestparams.threshold = 2.29614663f;
            bestparams.pulsestrength = 10.9457226f;
            bestparams.distance1 = 1.22529554f;
            bestparams.distance2 = 1.1281817f;
            break;
        case 7: // PST
            // current score 8324 (191/32768)
            bestparams.threshold = 1.13159347f;
            bestparams.pulsestrength = 2.58528447f;
            bestparams.distance1 = 0.729154527f;
            bestparams.distance2 = 1.25284457f;
            break;
        }
    }

    else if (strcmp(chip, "cbm4383") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 9127 (1462/32768)
            bestparams.threshold = 0.00404749066f;
            bestparams.distance1 = 9.99999975e-05f;
            bestparams.distance2 = 12.5628147f;
            break;
        case 5: // PT
            // current score 3370 (157/32768)
            bestparams.threshold = 0.954228163f;
            bestparams.pulsestrength = 2.24112558f;
            bestparams.distance1 = 0.961981118f;
            bestparams.distance2 = 1.10502172f;
            break;
        case 6: // PS
            // current score 24420 (1120/32768)
            bestparams.threshold = 2.07865024f;
            bestparams.pulsestrength = 9.54263973f;
            bestparams.distance1 = 1.21039879f;
            bestparams.distance2 = 1.1256249f;
            break;
        case 7: // PST
            // current score 6906 (168/32768)
            bestparams.threshold = 1.09114969f;
            bestparams.pulsestrength = 2.55320096f;
            bestparams.distance1 = 0.642633319f;
            bestparams.distance2 = 1.2205565f;
            break;
        }
    }

    else if (strcmp(chip, "r4ar378914") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 9710 (558/32768)
            bestparams.threshold = 0.779444575f;
            bestparams.distance1 = 1.38047099f;
            bestparams.distance2 = 5.92329025f;
            break;
        case 5: // PT
            // current score 5067 (154/32768)
            bestparams.threshold = 0.946132541f;
            bestparams.pulsestrength = 2.04653716f;
            bestparams.distance1 = 0.991122723f;
            bestparams.distance2 = 1.11967027f;
            break;
        case 6: // PS
            // current score 25414 (1048/32768)
            bestparams.threshold = 1.96959639f;
            bestparams.pulsestrength = 8.46444702f;
            bestparams.distance1 = 1.22678828f;
            bestparams.distance2 = 1.13746059f;
            break;
        case 7: // PST
            // current score 5755 (142/32768)
            bestparams.threshold = 1.10361195f;
            bestparams.pulsestrength = 2.61093307f;
            bestparams.distance1 = 0.721655667f;
            bestparams.distance2 = 1.21978724f;
            break;
        }
    }

    else if (strcmp(chip, "r4ar448614") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 28257 (1735/32768)
            bestparams.threshold = 0.102088384f;
            bestparams.distance1 = 0.102473356f;
            bestparams.distance2 = 2.11288095f;
            break;
        case 5: // PT
            // current score 4392 (83/32768)
            bestparams.threshold = 0.949247003f;
            bestparams.pulsestrength = 2.34519076f;
            bestparams.distance1 = 0.980778635f;
            bestparams.distance2 = 1.10463774f;
            break;
        case 6: // PS
            // current score 23788 (1476/32768)
            bestparams.threshold = 2.18728018f;
            bestparams.pulsestrength = 10.1908789f;
            bestparams.distance1 = 1.2324692f;
            bestparams.distance2 = 1.12931967f;
            break;
        case 7: // PST
            // current score 7787 (187/32768)
            bestparams.threshold = 1.08715856f;
            bestparams.pulsestrength = 2.7700963f;
            bestparams.distance1 = 0.614550114f;
            bestparams.distance2 = 1.20550799f;
            break;
        }
    }

    else if (strcmp(chip, "r4ar528614") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 22364 (1681/32768)
            bestparams.threshold = 0.000156631984f;
            bestparams.distance1 = 9.99999975e-05f;
            bestparams.distance2 = 59.9391479f;
            break;
        case 5: // PT
            // current score 5586 (147/32768)
            bestparams.threshold = 0.965520382f;
            bestparams.pulsestrength = 1.97317994f;
            bestparams.distance1 = 1.03463221f;
            bestparams.distance2 = 1.17572582f;
            break;
        case 6: // PS
            // current score 28016 (1317/32768)
            bestparams.threshold = 1.9388175f;
            bestparams.pulsestrength = 7.28115606f;
            bestparams.distance1 = 1.29100823f;
            bestparams.distance2 = 1.18405747f;
            break;
        case 7: // PST
            // current score 8387 (205/32768)
            bestparams.threshold = 1.06987619f;
            bestparams.pulsestrength = 2.56502175f;
            bestparams.distance1 = 0.592541218f;
            bestparams.distance2 = 1.24261856f;
            break;
        }
    }

    else if (strcmp(chip, "r30486s") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 4959 (470/32768)
            bestparams.threshold = 0.825091541f;
            bestparams.distance1 = 1.66668344f;
            bestparams.distance2 = 8.88740063f;
            break;
        case 5: // PT
            // current score 5596 (138/32768)
            bestparams.threshold = 0.889705539f;
            bestparams.pulsestrength = 1.46961653f;
            bestparams.distance1 = 1.0133251f;
            bestparams.distance2 = 1.15594172f;
            break;
        case 6: // PS
            // current score 22739 (770/32768)
            bestparams.threshold = 1.60104156f;
            bestparams.pulsestrength = 6.3162694f;
            bestparams.distance1 = 1.15619385f;
            bestparams.distance2 = 1.13926661f;
            break;
        case 7: // PST
            // current score 5402 (122/32768)
            bestparams.threshold = 1.03484344f;
            bestparams.pulsestrength = 2.08253074f;
            bestparams.distance1 = 0.674197793f;
            bestparams.distance2 = 1.24544847f;
            break;
        }
    }

    else if (strcmp(chip, "r34785") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 4026 (285/32768)
            bestparams.threshold = 0.917309165f;
            bestparams.distance1 = 2.65143633f;
            bestparams.distance2 = 13.3040199f;
            break;
        case 5: // PT
            // current score 682 (45/32768)
            bestparams.threshold = 1.02050364f;
            bestparams.pulsestrength = 2.72869492f;
            bestparams.distance1 = 1.02362359f;
            bestparams.distance2 = 1.06529796f;
            break;
        case 6: // PS
            // current score 12313 (741/32768)
            bestparams.threshold = 1.94709873f;
            bestparams.pulsestrength = 9.60707474f;
            bestparams.distance1 = 1.17691827f;
            bestparams.distance2 = 1.08982253f;
            break;
        case 7: // PST
            // current score 2848 (84/32768)
            bestparams.threshold = 1.12691724f;
            bestparams.pulsestrength = 3.10190463f;
            bestparams.distance1 = 0.734457731f;
            bestparams.distance2 = 1.14173222f;
            break;
        }
    }

    else if (strcmp(chip, "r34885") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 7739 (496/32768)
            bestparams.threshold = 0.652302086f;
            bestparams.distance1 = 1.30279803f;
            bestparams.distance2 = 3.48698401f;
            break;
        case 5: // PT
            // current score 3174 (110/32768)
            bestparams.threshold = 0.949834585f;
            bestparams.pulsestrength = 2.3867557f;
            bestparams.distance1 = 0.96540755f;
            bestparams.distance2 = 1.08990455f;
            break;
        case 6: // PS
            // current score 22631 (732/32768)
            bestparams.threshold = 2.41232967f;
            bestparams.pulsestrength = 14.9673033f;
            bestparams.distance1 = 1.08695972f;
            bestparams.distance2 = 1.06505466f;
            break;
        case 7: // PST
            // current score 6006 (155/32768)
            bestparams.threshold = 1.04216397f;
            bestparams.pulsestrength = 2.66450763f;
            bestparams.distance1 = 0.527853131f;
            bestparams.distance2 = 1.17376161f;
            break;
        }
    }

    else {
        std::cout << "Unrecognized chip" << std::endl;
        exit(-1);
    }
#endif
    if (bestparams.distance2 == 0.f)
        bestparams.distance2 = bestparams.distance1;

    const bool is8580 = false;

    // Calculate current score
    score_t bestscore = bestparams.Score(wave, is8580, reference, true, 4096 * 255);
    std::cout << "# initial score " << std::dec
        << bestscore << std::endl
        << bestparams.toString() << std::endl << std::endl;
    if (bestscore.audible_error == 0)
        exit(0);

    /*
     * Start the Monte Carlo loop: we randomly alter parameters
     * and calculate the new score until we find the best fitting
     * waveform compared to the sampled data.
     */
    Parameters p = bestparams;
    for (;;)
    {
        // loop until at least one parameter has changed
        bool changed = false;
        while (!changed)
        {
            for (Param_t i = Param_t::THRESHOLD; i <= Param_t::DISTANCE2; i++)
            {
                // PULSESTRENGTH only affects pulse
                if ((i==Param_t::PULSESTRENGTH) && ((wave & 0x04) != 0x04))
                {
                    continue;
                }

                // change a parameter with 50% proability
                if (GetRandomValue() > 1.)
                {
                    const float oldValue = bestparams.GetValue(i);

                    //std::cout << newValue << " -> ";
                    float newValue = static_cast<float>(GetRandomValue()*oldValue);
                    //float newValue = oldValue + GetRandomValue();
                    //std::cout << newValue << std::endl;

                    // avoid negative values
                    if (newValue <= 0.f)
                    {
                        newValue = EPSILON;
                    }
                    // try to avoid too small values
                    else if (newValue < EPSILON)
                        newValue += GetNewRandomValue();

                    // check for parameters limits
                    //if (((i == Param_t::THRESHOLD) || (i == Param_t::PULSESTRENGTH))
                    //    && (newValue >= 1.f))
                    //{
                    //    newValue = 1.f - EPSILON;
                    //}

                    p.SetValue(i, newValue);
                    changed = changed || oldValue != newValue;
                }
            }
        }

        // check new score
        const score_t score = p.Score(wave, is8580, reference, false, bestscore.audible_error);
        if (bestscore.isBetter(score))
        {
            // accept if improvement
            std::cout << "# current score " << std::dec
                << score << std::endl
                << p.toString() << std::endl << std::endl;
            if (score.audible_error == 0)
                exit(0);
            //p.reset();
            bestparams = p;
            bestscore = score;
        }
        else if (score.audible_error == bestscore.audible_error)
        {
            // print the rate of wrong bits
            std::cout << score.wrongBitsRate() << std::endl;

            // no improvement but use new parameters as base to increase the "entropy"
            bestparams = p;
        }
    }
}

/**
 * Split a file into lines.
 */
static std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

/**
 * Read sampled values for specific waveform and chip.
 */
static ref_vector_t ReadChip(int wave, const char* chip)
{
    std::cout << "Reading wave: " << wave << std::endl;

    std::ostringstream fileName;
    fileName << "sidwaves/" << chip << "/6581wf" << wave << "0.dat.prg";
    std::ifstream ifs(fileName.str().c_str(), std::ifstream::in);
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
    if (argc != 3)
    {
        std::cout << "Usage " << argv[0] << " <waveform> <chip>" << std::endl;
        exit(-1);
    }

    const int wave = atoi(argv[1]);
    assert(wave == 3 || wave == 5 || wave == 6 || wave == 7);

    const char* chip = argv[2];

    ref_vector_t reference = ReadChip(wave, chip);

#ifndef NDEBUG
    for (ref_vector_t::iterator it = reference.begin(); it != reference.end(); ++it)
        std::cout << (*it) << std::endl;
#endif

    srand(time(0));

    Optimize(reference, wave, chip);
}
