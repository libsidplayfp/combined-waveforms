/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2013-2023 Leandro Nini <drfiemost@users.sourceforge.net>
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

static std::normal_distribution<> normal_dist(1.0, 0.001);
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
            // current score 614 (104/32768)
            bestparams.threshold = 1.0124892f;
            bestparams.pulsestrength = 2.45542145f;
            bestparams.distance1 = 0.055455029f;
            bestparams.distance2 = 0.0990793332f;
            break;
        case 6: // PS
            // current score 8135 (575/32768)
            bestparams.threshold = 2.14896345f;
            bestparams.pulsestrength = 10.5400085f;
            bestparams.topbit = 1.0216713f;
            bestparams.distance1 = 0.244498149f;
            bestparams.distance2 = 0.126134038f;
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
            // current score 12974 (691/32768)
            bestparams.threshold = 2.06900835f;
            bestparams.pulsestrength = 8.69191265f;
            bestparams.topbit = 1.05807245f;
            bestparams.distance1 = 0.338294297f;
            bestparams.distance2 = 0.196878001f;
            break;
        case 7: // PST
            // current score 4088 (106/32768) *
            bestparams.threshold = 1.14416945f;
            bestparams.pulsestrength = 3.07632709f;
            bestparams.distance1 = 0.674530327f;
            bestparams.distance2 = 1.17008042f;
// current score 4299 (95/32768)
bestparams.threshold = 1.27460456f;
bestparams.pulsestrength = 3.12633324f;
bestparams.topbit = 0.976682305f;
bestparams.distance1 = 0.0108803604f;
bestparams.distance2 = 0.364160866f;
            break;
        }
    }

    else if (strcmp(chip, "3084") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 6329 (332/32768)
            bestparams.threshold = 0.918491125f;
            bestparams.topbit = 1.45740879f;
            bestparams.distance1 = 7.97798014f;
            bestparams.distance2 = 20.3139534f;
            break;
        case 5: // PT
            // current score 5911 (176/32768) *
            bestparams.threshold = 0.996065319f;
            bestparams.pulsestrength = 2.00260067f;
            bestparams.distance1 = 1.0930692f;
            bestparams.distance2 = 1.1597296f;
// current score 7299 (186/32768)
bestparams.threshold = 1.02371943f;
bestparams.pulsestrength = 2.03441763f;
bestparams.distance1 = 0.111022435f;
bestparams.distance2 = 0.273567319f;
            break;
        case 6: // PS
            // current score 19251 (820/32768)
            bestparams.threshold = 2.03611517f;
            bestparams.pulsestrength = 6.61680031f;
            bestparams.topbit = 1.00762045f;
            bestparams.distance1 = 0.532329381f;
            bestparams.distance2 = 0.353334934f;
            break;
        case 7: // PST
            // current score 5520 (101/32768)
            bestparams.threshold = 1.19494736f;
            bestparams.pulsestrength = 1.56723189f;
            bestparams.topbit = 0.876089394f;
            bestparams.distance1 = 0.33571592f;
            bestparams.distance2 = 1.00845397f;
            break;
        }
    }

    else if (strcmp(chip, "cbm3384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 16820 (1031/32768)
            bestparams.threshold = 0.000224893636f;
            bestparams.topbit = 0.000224897463f;
            bestparams.distance1 = 0.000115541166f;
            bestparams.distance2 = 1.84193969f;
            break;
        case 5: // PT
            // current score 3620 (42/32768)
            bestparams.threshold = 0.984425008f;
            bestparams.pulsestrength = 2.35668468f;
            bestparams.distance1 = 0.0199570525f;
            bestparams.distance2 = 0.175396249f;
            break;
        case 6: // PS
            // current score 21119 (1159/32768)
            bestparams.threshold = 2.41408348f;
            bestparams.pulsestrength = 10.2276049f;
            bestparams.distance1 = 0.358307987f;
            bestparams.distance2 = 0.240197971f;
            break;
        case 7: // PST
            // current score 7752 (151/32768)
            bestparams.threshold = 1.19250798f;
            bestparams.pulsestrength = 2.32080412f;
            bestparams.topbit = 0.955280125f;
            bestparams.distance1 = 0.0681763813f;
            bestparams.distance2 = 0.604984641f;
            break;
        }
    }

    else if (strcmp(chip, "cbm4383") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 5537 (924/32768)
            bestparams.threshold = 0.00673561823f;
            bestparams.topbit = 0.0067387647f;
            bestparams.distance1 = 0.00215783017f;
            bestparams.distance2 = 9.49551773f;
            break;
        case 5: // PT
            // current score 2130 (131/32768)
            bestparams.threshold = 1.00092328f;
            bestparams.pulsestrength = 2.42803788f;
            bestparams.distance1 = 0.0113755139f;
            bestparams.distance2 = 0.162516415f;
            break;
        case 6: // PS
            // current score 19304 (1054/32768)
            bestparams.threshold = 2.42779779f;
            bestparams.pulsestrength = 9.93910408f;
            bestparams.topbit = 1.12610471f;
            bestparams.distance1 = 0.411725849f;
            bestparams.distance2 = 0.245940804f;
            break;
        case 7: // PST
            // current score 6364 (107/32768)
            bestparams.threshold = 1.01210797f;
            bestparams.pulsestrength = 1.34227395f;
            bestparams.topbit = 0.786518633f;
            bestparams.distance1 = 0.0586184449f;
            bestparams.distance2 = 0.824515998f;
            break;
        }
    }

    else if (strcmp(chip, "r4ar378914") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 5504 (312/32768)
            bestparams.threshold = 0.973038077f;
            bestparams.topbit = 1.43141603f;
            bestparams.distance1 = 5.40211439f;
            bestparams.distance2 = 47.9917068f;
            break;
        case 5: // PT
            // current score 4621 (104/32768)
            bestparams.threshold = 0.978124142f;
            bestparams.pulsestrength = 2.08345437f;
            bestparams.distance1 = 0.0454150252f;
            bestparams.distance2 = 0.203794882f;
            break;
        case 6: // PS
            // current score 22212 (880/32768)
            bestparams.threshold = 1.96295238f;
            bestparams.pulsestrength = 6.79839182f;
            bestparams.distance1 = 0.42308706f;
            bestparams.distance2 = 0.307685643f;
            break;
        case 7: // PST
            // current score 5404 (100/32768)
            bestparams.threshold = 1.09994781f;
            bestparams.pulsestrength = 1.55916071f;
            bestparams.topbit = 0.93129617f;
            bestparams.distance1 = 0.137331873f;
            bestparams.distance2 = 0.820938587f;
            break;
        }
    }

    else if (strcmp(chip, "r4ar448614") == 0)
    {
        switch (wave)
        {
        case 3: // ST
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
            // current score 7346 (155/32768)
            bestparams.threshold = 1.21678829f;
            bestparams.pulsestrength = 2.18165922f;
            bestparams.topbit = 0.989545345f;
            bestparams.distance1 = 0.142729312f;
            bestparams.distance2 = 0.684979916f;
            break;
        }
    }

    else if (strcmp(chip, "r4ar528614") == 0)
    {
        switch (wave)
        {
        case 3: // ST
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
// current score 7298 (177/32768)
bestparams.threshold = 1.00947511f;
bestparams.pulsestrength = 1.92809784f;
bestparams.distance1 = 0.154805347f;
bestparams.distance2 = 0.356464893f;
            break;
        case 6: // PS
            // current score 21337 (1258/32768)
            bestparams.threshold = 1.80203927f;
            bestparams.pulsestrength = 4.74579906f;
            bestparams.distance1 = 0.500079989f;
            bestparams.distance2 = 0.52558279f;
            break;
        case 7: // PST
            // current score 7455 (126/32768)
            bestparams.threshold = 1.05178881f;
            bestparams.pulsestrength = 1.42655933f;
            bestparams.topbit = 0.805379808f;
            bestparams.distance1 = 0.13179633f;
            bestparams.distance2 = 0.978171349f;
            break;
        }
    }

    else if (strcmp(chip, "r30486s") == 0)
    {
        switch (wave)
        {
        case 3: // ST
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
            // current score 19352 (763/32768)
            bestparams.threshold = 1.66494179f;
            bestparams.pulsestrength = 5.62705326f;
            bestparams.topbit = 1.03760982f;
            bestparams.distance1 = 0.291590303f;
            bestparams.distance2 = 0.283631504f;
            break;
        case 7: // PST
            // current score 5068 (94/32768)
            bestparams.threshold = 1.09762526f;
            bestparams.pulsestrength = 1.52196741f;
            bestparams.topbit = 0.975265801f;
            bestparams.distance1 = 0.151528224f;
            bestparams.distance2 = 0.841949463f;
            break;
        }
    }

    else if (strcmp(chip, "r34785") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2298 (339/32768)
            bestparams.threshold = 0.776678205f;
            bestparams.topbit = 1.18439901f;
            bestparams.distance1 = 2.25732255f;
            bestparams.distance2 = 5.12803745f;
            break;
        case 5: // PT
            // current score 582 (57/32768)
            bestparams.threshold = 1.01866758f;
            bestparams.pulsestrength = 2.69177628f;
            bestparams.distance1 = 0.0233543925f;
            bestparams.distance2 = 0.0850229636f;
            break;
        case 6: // PS
            // current score 9242 (679/32768)
            bestparams.threshold = 2.20329857f;
            bestparams.pulsestrength = 10.5146885f;
            bestparams.topbit = 1.04501438f;
            bestparams.distance1 = 0.277294368f;
            bestparams.distance2 = 0.143747061f;
            break;
        case 7: // PST
            // current score 2799 (71/32768)
            bestparams.threshold = 1.35652959f;
            bestparams.pulsestrength = 3.21098137f;
            bestparams.topbit = 1.09051275f;
            bestparams.distance1 = 0.16658926f;
            bestparams.distance2 = 0.370252877f;
            break;
        }
    }

    else if (strcmp(chip, "r34885") == 0)
    {
        switch (wave)
        {
        case 3: // ST
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
            // current score 18924 (892/32768)
            bestparams.threshold = 2.57584476f;
            bestparams.pulsestrength = 13.8990936f;
            bestparams.topbit = 1.17231143f;
            bestparams.distance1 = 0.202597454f;
            bestparams.distance2 = 0.128030822f;
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

    else if (strcmp(chip, "6581R4AR-3488_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2207 (302/32768)
            bestparams.threshold = 0.769770384f;
            bestparams.pulsestrength = 1.f;
            bestparams.topbit = 1.19125676f;
            bestparams.distance1 = 2.24802995f;
            bestparams.distance2 = 4.92881823f;
            break;
        case 5: // PT
            // current score 3832 (76/32768)
            bestparams.threshold = 0.946048796f;
            bestparams.pulsestrength = 1.83248079f;
            bestparams.distance1 = 0.0637977049f;
            bestparams.distance2 = 0.207975447f;
            break;
        case 6: // PS
            // current score 20496 (988/32768)
            bestparams.threshold = 1.14159644f;
            bestparams.pulsestrength = 3.50420499f;
            bestparams.topbit = 0.748402119f;
            bestparams.distance1 = 0.00319250347f;
            bestparams.distance2 = 0.218578994f;
            break;
        case 7: // PST
            // current score 5006 (102/32768)
            bestparams.threshold = 1.08452392f;
            bestparams.pulsestrength = 1.81916571f;
            bestparams.topbit = 0.904740691f;
            bestparams.distance1 = 0.0277621783f;
            bestparams.distance2 = 0.585185289f;
            break;
        }
    }

    else if (strcmp(chip, "broken0384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 20337 (1579/32768)
            bestparams.threshold = 0.000637792516f;
            bestparams.topbit = 1.56725872f;
            bestparams.distance1 = 0.00036806846f;
            bestparams.distance2 = 1.51800942f;
            break;
        case 5: // PT
            // current score 5204 (244/32768)
            bestparams.threshold = 0.926222563f;
            bestparams.pulsestrength = 1.97369242f;
            bestparams.distance1 = 0.0908993334f;
            bestparams.distance2 = 0.23501052f;
            break;
        case 6: // PS
            // current score 31015 (2181/32768)
            bestparams.threshold = 1.2328074f;
            bestparams.pulsestrength = 3.9719491f;
            bestparams.topbit = 0.73079139f;
            bestparams.distance1 = 0.00156516861f;
            bestparams.distance2 = 0.314677745f;
            break;
        case 7: // PST
            // current score 9874 (201/32768)
            bestparams.threshold = 1.08558261f;
            bestparams.pulsestrength = 1.52781796f;
            bestparams.topbit = 0.857638359f;
            bestparams.distance1 = 0.152927235f;
            bestparams.distance2 = 1.02657032f;
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
