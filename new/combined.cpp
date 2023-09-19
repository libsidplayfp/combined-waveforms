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
            // current score 690 (136/32768)
            bestparams.threshold = 1.0130074f;
            bestparams.pulsestrength = 2.4708786f;
            bestparams.distance1 = 0.0610449091f;
            bestparams.distance2 = 0.0978600159f;
            break;
        case 6: // PS
            // current score 8248 (545/32768)
            bestparams.threshold = 2.11463952f;
            bestparams.pulsestrength = 10.3241644f;
            bestparams.distance1 = 0.238633528f;
            bestparams.distance2 = 0.125765577f;
            break;
        case 7: // PST
            // current score 2537 (64/32768)
            bestparams.threshold = 1.32549036f;
            bestparams.pulsestrength = 3.35835981f;
            bestparams.distance1 = 0.108314693f;
            bestparams.distance2 = 0.309225291f;
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
// current score 10021 (385/32768)
bestparams.threshold = 0.823114872f;
bestparams.topbit = 1.29229462f;
bestparams.distance1 = 2.96363974f;
bestparams.distance2 = 6.07001877f;
            break;
        case 5: // PT
            // current score 2016 (141/32768) *
            bestparams.threshold = 0.938275278f;
            bestparams.pulsestrength = 1.70019507f;
            bestparams.distance1 = 1.10584641f;
            bestparams.distance2 = 1.11688411f;
// current score 3246 (175/32768)
bestparams.threshold = 0.912571907f;
bestparams.pulsestrength = 1.40682948f;
bestparams.distance1 = 0.148781762f;
bestparams.distance2 = 0.203292117f;
            break;
        case 6: // PS
            // current score 13205 (599/32768)
            bestparams.threshold = 1.9980768f;
            bestparams.pulsestrength = 8.31666946f;
            bestparams.distance1 = 0.307338983f;
            bestparams.distance2 = 0.196080133f;
            break;
        case 7: // PST
            // current score 4088 (106/32768) *
            bestparams.threshold = 1.14416945f;
            bestparams.pulsestrength = 3.07632709f;
            bestparams.distance1 = 0.674530327f;
            bestparams.distance2 = 1.17008042f;
// current score 4307 (96/32768)
bestparams.threshold = 1.29502332f;
bestparams.pulsestrength = 3.24021482f;
bestparams.distance1 = 0.0112341065f;
bestparams.distance2 = 0.357879192f;
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
// current score 6393 (334/32768)
bestparams.threshold = 0.913319409f;
bestparams.topbit = 1.45449018f;
bestparams.distance1 = 7.60523796f;
bestparams.distance2 = 20.4204884f;
            break;
        case 5: // PT
            // current score 5911 (176/32768) *
            bestparams.threshold = 0.996065319f;
            bestparams.pulsestrength = 2.00260067f;
            bestparams.distance1 = 1.0930692f;
            bestparams.distance2 = 1.1597296f;
// current score 7315 (188/32768)
bestparams.threshold = 1.0226984f;
bestparams.pulsestrength = 2.02403545f;
bestparams.distance1 = 0.114521369f;
bestparams.distance2 = 0.274634987f;
            break;
        case 6: // PS
            // current score 19364 (826/32768)
            bestparams.threshold = 2.02650046f;
            bestparams.pulsestrength = 6.55439091f;
            bestparams.distance1 = 0.52970469f;
            bestparams.distance2 = 0.355786055f;
            break;
        case 7: // PST
            // current score 5658 (105/32768)
            bestparams.threshold = 1.25902987f;
            bestparams.pulsestrength = 1.67054915f;
            bestparams.distance1 = 0.424322635f;
            bestparams.distance2 = 1.03241634f;
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
// current score 16820 (1031/32768)
bestparams.threshold = 0.000224893636f;
bestparams.topbit = 0.000224897463f;
bestparams.distance1 = 0.000115541166f;
bestparams.distance2 = 1.84193969f;
            break;
        case 5: // PT
            // current score 3682 (44/32768)
            bestparams.threshold = 0.983347535f;
            bestparams.pulsestrength = 2.34515882f;
            bestparams.distance1 = 0.0261887871f;
            bestparams.distance2 = 0.176363945f;
            break;
        case 6: // PS
            // current score 21119 (1159/32768)
            bestparams.threshold = 2.41408348f;
            bestparams.pulsestrength = 10.2276049f;
            bestparams.distance1 = 0.358307987f;
            bestparams.distance2 = 0.240197971f;
            break;
        case 7: // PST
            // current score 7880 (153/32768)
            bestparams.threshold = 1.21470439f;
            bestparams.pulsestrength = 2.40497684f;
            bestparams.distance1 = 0.0799665004f;
            bestparams.distance2 = 0.598565221f;
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
// current score 5537 (924/32768)
bestparams.threshold = 0.00673561823f;
bestparams.topbit = 0.0067387647f;
bestparams.distance1 = 0.00215783017f;
bestparams.distance2 = 9.49551773f;
            break;
        case 5: // PT
            // current score 2152 (109/32768)
            bestparams.threshold = 0.98445785f;
            bestparams.pulsestrength = 2.30505061f;
            bestparams.distance1 = 0.0125515247f;
            bestparams.distance2 = 0.166096732f;
            break;
        case 6: // PS
            // current score 20084 (913/32768)
            bestparams.threshold = 2.08749008f;
            bestparams.pulsestrength = 8.03567791f;
            bestparams.distance1 = 0.364282787f;
            bestparams.distance2 = 0.258649141f;
            break;
        case 7: // PST
            // current score 6773 (128/32768)
            bestparams.threshold = 1.14698815f;
            bestparams.pulsestrength = 1.75253236f;
            bestparams.distance1 = 0.12823686f;
            bestparams.distance2 = 0.768049777f;
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
// current score 5504 (312/32768)
bestparams.threshold = 0.973038077f;
bestparams.topbit = 1.43141603f;
bestparams.distance1 = 5.40211439f;
bestparams.distance2 = 47.9917068f;
            break;
        case 5: // PT
            // current score 4711 (124/32768)
            bestparams.threshold = 1.00073922f;
            bestparams.pulsestrength = 2.23375988f;
            bestparams.distance1 = 0.0484863259f;
            bestparams.distance2 = 0.198355511f;
            break;
        case 6: // PS
            // current score 22212 (880/32768)
            bestparams.threshold = 1.96295238f;
            bestparams.pulsestrength = 6.79839182f;
            bestparams.distance1 = 0.42308706f;
            bestparams.distance2 = 0.307685643f;
            break;
        case 7: // PST
            // current score 5454 (108/32768)
            bestparams.threshold = 1.1500113f;
            bestparams.pulsestrength = 1.70165455f;
            bestparams.distance1 = 0.161344022f;
            bestparams.distance2 = 0.803988993f;
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
// current score 25195 (1197/32768)
bestparams.threshold = 0.0993857682f;
bestparams.topbit = 0.105061948f;
bestparams.distance1 = 0.0556670353f;
bestparams.distance2 = 2.12972975f;
            break;
        case 5: // PT
            // current score 3620 (65/32768)
            bestparams.threshold = 0.99788028f;
            bestparams.pulsestrength = 2.50648594f;
            bestparams.distance1 = 0.043751359f;
            bestparams.distance2 = 0.164767623f;
            break;
        case 6: // PS
            // current score 19624 (1177/32768)
            bestparams.threshold = 2.35510826f;
            bestparams.pulsestrength = 10.1756306f;
            bestparams.distance1 = 0.353252262f;
            bestparams.distance2 = 0.22332482f;
            break;
        case 7: // PST
            // current score 7366 (157/32768)
            bestparams.threshold = 1.23311782f;
            bestparams.pulsestrength = 2.23575926f;
            bestparams.distance1 = 0.140250757f;
            bestparams.distance2 = 0.680407405f;
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
// current score 18860 (1155/32768)
bestparams.threshold = 0.000317011203f;
bestparams.topbit = 0.000317018101f;
bestparams.distance1 = 0.000216722008f;
bestparams.distance2 = 15.1529646f;
            break;
        case 5: // PT
            // current score 5586 (147/32768) *
            bestparams.threshold = 0.965520382f;
            bestparams.pulsestrength = 1.97317994f;
            bestparams.distance1 = 1.03463221f;
            bestparams.distance2 = 1.17572582f;
// current score 7314 (179/32768)
bestparams.threshold = 1.00954437f;
bestparams.pulsestrength = 1.9268142f;
bestparams.distance1 = 0.157903984f;
bestparams.distance2 = 0.35697192f;
            break;
        case 6: // PS
            // current score 21337 (1258/32768)
            bestparams.threshold = 1.80203927f;
            bestparams.pulsestrength = 4.74579906f;
            bestparams.distance1 = 0.500079989f;
            bestparams.distance2 = 0.52558279f;
            break;
        case 7: // PST
            // current score 7736 (155/32768)
            bestparams.threshold = 1.1686492f;
            bestparams.pulsestrength = 1.87631691f;
            bestparams.distance1 = 0.131389365f;
            bestparams.distance2 = 0.844650745f;
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
// current score 3555 (324/32768)
bestparams.threshold = 0.877322257f;
bestparams.topbit = 1.11349654f;
bestparams.distance1 = 2.14537621f;
bestparams.distance2 = 9.08618164f;
            break;
        case 5: // PT
            // current score 4734 (124/32768)
            bestparams.threshold = 0.943426073f;
            bestparams.pulsestrength = 1.8501184f;
            bestparams.distance1 = 0.0302912984f;
            bestparams.distance2 = 0.221833065f;
            break;
        case 6: // PS
            // current score 19755 (821/32768)
            bestparams.threshold = 1.59060228f;
            bestparams.pulsestrength = 5.29649925f;
            bestparams.distance1 = 0.254997313f;
            bestparams.distance2 = 0.282227874f;
            break;
        case 7: // PST
            // current score 5132 (99/32768)
            bestparams.threshold = 1.12174881f;
            bestparams.pulsestrength = 1.56275725f;
            bestparams.distance1 = 0.187281072f;
            bestparams.distance2 = 0.854561031f;
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
// current score 2362 (341/32768)
bestparams.threshold = 0.796405137f;
bestparams.topbit = 1.25515425f;
bestparams.distance1 = 2.45860958f;
bestparams.distance2 = 5.07782269f;
            break;
        case 5: // PT
            // current score 582 (57/32768)
            bestparams.threshold = 1.01866758f;
            bestparams.pulsestrength = 2.69177628f;
            bestparams.distance1 = 0.0233543925f;
            bestparams.distance2 = 0.0850229636f;
            break;
        case 6: // PS
            // current score 9506 (621/32768)
            bestparams.threshold = 2.05713248f;
            bestparams.pulsestrength = 9.49412537f;
            bestparams.distance1 = 0.274534792f;
            bestparams.distance2 = 0.147642791f;
            break;
        case 7: // PST
            // current score 2823 (67/32768)
            bestparams.threshold = 1.32814288f;
            bestparams.pulsestrength = 3.24630189f;
            bestparams.distance1 = 0.163979709f;
            bestparams.distance2 = 0.352188557f;
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
// current score 7286 (397/32768)
bestparams.threshold = 0.759519219f;
bestparams.topbit = 1.28535891f;
bestparams.distance1 = 2.08408093f;
bestparams.distance2 = 4.26385403f;
            break;
        case 5: // PT
            // current score 1956 (36/32768)
            bestparams.threshold = 0.992383003f;
            bestparams.pulsestrength = 2.49721408f;
            bestparams.distance1 = 0.0148989018f;
            bestparams.distance2 = 0.14348942f;
            break;
        case 6: // PS
            // current score 19913 (774/32768)
            bestparams.threshold = 2.22848773f;
            bestparams.pulsestrength = 11.5227165f;
            bestparams.distance1 = 0.180363834f;
            bestparams.distance2 = 0.130718917f;
            break;
        case 7: // PST
            // current score 5575 (118/32768)
            bestparams.threshold = 1.15620351f;
            bestparams.pulsestrength = 2.5087378f;
            bestparams.distance1 = 0.0456474312f;
            bestparams.distance2 = 0.433534175f;
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

                // TOPBIT only affects saw
                if ((i==Param_t::TOPBIT) && ((wave & 0x02) != 0x02))
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
