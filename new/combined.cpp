/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2013-2024 Leandro Nini <drfiemost@users.sourceforge.net>
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

static std::normal_distribution<> normal_dist(1.0, 0.005);
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

    bool is8580 = false;

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
            // current score 1858 (204/32768) [RMS: 62.49]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.886832297f;
            bestparams.distance1 = 2.14438701f;
            bestparams.distance2 = 9.51839447f;
            break;
        case 5: // PT
            // current score 612 (102/32768) [RMS: 43.71]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.01262534f;
            bestparams.pulsestrength = 2.46070528f;
            bestparams.distance1 = 0.0537485816f;
            bestparams.distance2 = 0.0986242667f;
            break;
        case 6: // PS
            // current score 8135 (575/32768) [RMS: 75.10]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 2.14896345f;
            bestparams.pulsestrength = 10.5400085f;
            bestparams.topbit = 1.0216713f;
            bestparams.distance1 = 0.244498149f;
            bestparams.distance2 = 0.126134038f;
            break;
        case 7: // PST
            // current score 2505 (63/32768) [RMS: 24.37]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.29061747f;
            bestparams.pulsestrength = 3.15377498f;
            bestparams.topbit = 0.9754318f;
            bestparams.distance1 = 0.0968349651f;
            bestparams.distance2 = 0.318573922f;
            break;
        }
    }

    else if (strcmp(chip, "6581_0784") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 10021 (385/32768) [RMS: 65.16]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.823114872f;
            bestparams.topbit = 1.29229462f;
            bestparams.distance1 = 2.96363974f;
            bestparams.distance2 = 6.07001877f;
            break;
        case 5: // PT
            // current score 2016 (141/32768) [RMS: 52.18]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.938275278f;
            bestparams.pulsestrength = 1.70019507f;
            bestparams.distance1 = 1.10584641f;
            bestparams.distance2 = 1.11688411f;
            break;
        case 6: // PS
            // current score 12765 (629/32768) [RMS: 87.66]
            bestparams.threshold = 2.09155488f;
            bestparams.pulsestrength = 8.82649231f;
            bestparams.topbit = 1.10415828f;
            bestparams.distance1 = 0.328211099f;
            bestparams.distance2 = 0.196435586f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 4088 (106/32768) [RMS: 31.40]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 1.14416945f;
            bestparams.pulsestrength = 3.07632709f;
            bestparams.distance1 = 0.674530327f;
            bestparams.distance2 = 1.17008042f;
            break;
        }
    }

    else if (strcmp(chip, "6581_3084") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 6329 (332/32768) [RMS: 72.16]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.918491125f;
            bestparams.topbit = 1.45740879f;
            bestparams.distance1 = 7.97798014f;
            bestparams.distance2 = 20.3139534f;
            break;
        case 5: // PT
            // current score 5781 (198/32768) [RMS: 66.75]
            bestparams.threshold = 0.999375761f;
            bestparams.pulsestrength = 2.03652263f;
            bestparams.distance1 = 1.05754781f;
            bestparams.distance2 = 1.15805364f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 19251 (820/32768) [RMS: 96.08]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 2.03611517f;
            bestparams.pulsestrength = 6.61680031f;
            bestparams.topbit = 1.00762045f;
            bestparams.distance1 = 0.532329381f;
            bestparams.distance2 = 0.353334934f;
            break;
        case 7: // PST
            // current score 5468 (97/32768) [RMS: 40.86]
            bestparams.threshold = 1.14943659f;
            bestparams.pulsestrength = 1.46092212f;
            bestparams.topbit = 0.848984182f;
            bestparams.distance1 = 0.281330794f;
            bestparams.distance2 = 1.01946712f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (strcmp(chip, "cbm3384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 16820 (1031/32768) [RMS: 87.00]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.000224893636f;
            bestparams.topbit = 0.000224897463f;
            bestparams.distance1 = 0.000115541166f;
            bestparams.distance2 = 1.84193969f;
            break;
        case 5: // PT
            // current score 3620 (42/32768) [RMS: 70.36]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.984425008f;
            bestparams.pulsestrength = 2.35668468f;
            bestparams.distance1 = 0.0199570525f;
            bestparams.distance2 = 0.175396249f;
            break;
        case 6: // PS
            // current score 20269 (1394/32768) [RMS: 102.93]
            bestparams.threshold = 2.72176981f;
            bestparams.pulsestrength = 11.8026724f;
            bestparams.topbit = 1.12436867f;
            bestparams.distance1 = 0.414662331f;
            bestparams.distance2 = 0.239115238f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 7752 (151/32768) [RMS: 43.90]
            bestparams.distFunc = Parameters::linearDistance;
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
            // current score 5537 (924/32768) [RMS: 79.93]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.00673561823f;
            bestparams.topbit = 0.0067387647f;
            bestparams.distance1 = 0.00215783017f;
            bestparams.distance2 = 9.49551773f;
            break;
        case 5: // PT
            // current score 2130 (131/32768) [RMS: 64.83]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.00092328f;
            bestparams.pulsestrength = 2.42803788f;
            bestparams.distance1 = 0.0113755139f;
            bestparams.distance2 = 0.162516415f;
            break;
        case 6: // PS
            // current score 19304 (1054/32768) [RMS: 96.13]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 2.42779779f;
            bestparams.pulsestrength = 9.93910408f;
            bestparams.topbit = 1.12610471f;
            bestparams.distance1 = 0.411725849f;
            bestparams.distance2 = 0.245940804f;
            break;
        case 7: // PST
            // current score 6364 (107/32768) [RMS: 39.55]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.01210797f;
            bestparams.pulsestrength = 1.34227395f;
            bestparams.topbit = 0.786518633f;
            bestparams.distance1 = 0.0586184449f;
            bestparams.distance2 = 0.824515998f;
            break;
        }
    }

    else if (strcmp(chip, "6581R4AR_3789_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 5504 (312/32768) [RMS: 72.74]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.973038077f;
            bestparams.topbit = 1.43141603f;
            bestparams.distance1 = 5.40211439f;
            bestparams.distance2 = 47.9917068f;
            break;
        case 5: // PT
            // current score 4621 (104/32768) [RMS: 66.23]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.978124142f;
            bestparams.pulsestrength = 2.08345437f;
            bestparams.distance1 = 0.0454150252f;
            bestparams.distance2 = 0.203794882f;
            break;
        case 6: // PS
            // current score 22207 (880/32768) [RMS: 96.91]
            bestparams.threshold = 1.96628845f;
            bestparams.pulsestrength = 6.81508446f;
            bestparams.topbit = 1.00600147f;
            bestparams.distance1 = 0.423710018f;
            bestparams.distance2 = 0.307503849f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 5404 (100/32768) [RMS: 40.99]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.09994781f;
            bestparams.pulsestrength = 1.55916071f;
            bestparams.topbit = 0.93129617f;
            bestparams.distance1 = 0.137331873f;
            bestparams.distance2 = 0.820938587f;
            break;
        }
    }

    else if (strcmp(chip, "6581R4AR_4486_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 25195 (1197/32768) [RMS: 80.06]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.0993857682f;
            bestparams.topbit = 0.105061948f;
            bestparams.distance1 = 0.0556670353f;
            bestparams.distance2 = 2.12972975f;
            break;
        case 5: // PT
            // current score 3604 (63/32768) [RMS: 70.47]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.998088539f;
            bestparams.pulsestrength = 2.51015329f;
            bestparams.distance1 = 0.0422255732f;
            bestparams.distance2 = 0.164421782f;
            break;
        case 6: // PS
            // current score 19624 (1177/32768) [RMS: 101.84]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 2.35510826f;
            bestparams.pulsestrength = 10.1756306f;
            bestparams.distance1 = 0.353252262f;
            bestparams.distance2 = 0.22332482f;
            break;
        case 7: // PST
            // current score 7250 (153/32768) [RMS: 43.42]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.20486581f;
            bestparams.pulsestrength = 2.13962531f;
            bestparams.topbit = 0.961478889f;
            bestparams.distance1 = 0.138547704f;
            bestparams.distance2 = 0.68967092f;
            break;
        }
    }

    else if (strcmp(chip, "6581R4AR_5286_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 18860 (1155/32768) [RMS: 79.93]6581R4AR_5286_14
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.00316550909f;
            bestparams.topbit = 0.00317018107f;
            bestparams.distance1 = 0.00221686065f;
            bestparams.distance2 = 10.0225477f;
            break;
        case 5: // PT
            // current score 5586 (147/32768) [RMS: 80.44]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.965520382f;
            bestparams.pulsestrength = 1.97317994f;
            bestparams.distance1 = 1.03463221f;
            bestparams.distance2 = 1.17572582f;
            break;
        case 6: // PS
            // current score 21336 (1258/32768) [RMS: 106.95]
            bestparams.threshold = 1.80564773f;
            bestparams.pulsestrength = 4.75714445f;
            bestparams.topbit = 1.00152075f;
            bestparams.distance1 = 0.50254482f;
            bestparams.distance2 = 0.525642395f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 7382 (124/32768) [RMS: 49.47]
            bestparams.threshold = 1.03704965f;
            bestparams.pulsestrength = 1.37006736f;
            bestparams.topbit = 0.771614373f;
            bestparams.distance1 = 0.130179495f;
            bestparams.distance2 = 1.02845287f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (strcmp(chip, "6581R3_0486_S") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 3555 (324/32768) [RMS: 73.98]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.877322257f;
            bestparams.topbit = 1.11349654f;
            bestparams.distance1 = 2.14537621f;
            bestparams.distance2 = 9.08618164f;
            break;
        case 5: // PT
            // current score 4590 (124/32768) [RMS: 68.90]
            bestparams.threshold = 0.941692829f;
            bestparams.pulsestrength = 1.80072665f;
            bestparams.distance1 = 0.033124879f;
            bestparams.distance2 = 0.232303441f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 19352 (763/32768) [RMS: 96.91]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.66494179f;
            bestparams.pulsestrength = 5.62705326f;
            bestparams.topbit = 1.03760982f;
            bestparams.distance1 = 0.291590303f;
            bestparams.distance2 = 0.283631504f;
            break;
        case 7: // PST
            // current score 5068 (94/32768) [RMS: 41.69]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.09762526f;
            bestparams.pulsestrength = 1.52196741f;
            bestparams.topbit = 0.975265801f;
            bestparams.distance1 = 0.151528224f;
            bestparams.distance2 = 0.841949463f;
            break;
        }
    }

    else if (strcmp(chip, "6581R3_4785") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2298 (339/32768) [RMS: 63.96]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.776678205f;
            bestparams.topbit = 1.18439901f;
            bestparams.distance1 = 2.25732255f;
            bestparams.distance2 = 5.12803745f;
            break;
        case 5: // PT
            // current score 582 (57/32768) [RMS: 45.61]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.01866758f;
            bestparams.pulsestrength = 2.69177628f;
            bestparams.distance1 = 0.0233543925f;
            bestparams.distance2 = 0.0850229636f;
            break;
        case 6: // PS
            // current score 9242 (679/32768) [RMS: 79.56]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 2.20329857f;
            bestparams.pulsestrength = 10.5146885f;
            bestparams.topbit = 1.04501438f;
            bestparams.distance1 = 0.277294368f;
            bestparams.distance2 = 0.143747061f;
            break;
        case 7: // PST
            // current score 2767 (66/32768) [RMS: 26.39]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.28576732f;
            bestparams.pulsestrength = 2.84452748f;
            bestparams.topbit = 1.04538679f;
            bestparams.distance1 = 0.151578978f;
            bestparams.distance2 = 0.389423102f;
            break;
        }
    }

    else if (strcmp(chip, "6581R3_4885") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 7286 (397/32768) [RMS: 75.32]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.759519219f;
            bestparams.topbit = 1.28535891f;
            bestparams.distance1 = 2.08408093f;
            bestparams.distance2 = 4.26385403f;
            break;
        case 5: // PT
            // current score 1956 (36/32768) [RMS: 65.23]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.992383003f;
            bestparams.pulsestrength = 2.49721408f;
            bestparams.distance1 = 0.0148989018f;
            bestparams.distance2 = 0.14348942f;
            break;
        case 6: // PS
            // current score 18924 (892/32768) [RMS: 94.14]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 2.57584476f;
            bestparams.pulsestrength = 13.8990936f;
            bestparams.topbit = 1.17231143f;
            bestparams.distance1 = 0.202597454f;
            bestparams.distance2 = 0.128030822f;
            break;
        case 7: // PST
            // current score 5575 (118/32768) [RMS: 36.88]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.15620351f;
            bestparams.pulsestrength = 2.5087378f;
            bestparams.distance1 = 0.0456474312f;
            bestparams.distance2 = 0.433534175f;
            break;
        }
    }

    else if (strcmp(chip, "6581R4AR_3488_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2207 (302/32768) [RMS: 64.27]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.769770384f;
            bestparams.topbit = 1.19125676f;
            bestparams.distance1 = 2.24802995f;
            bestparams.distance2 = 4.92881823f;
            break;
        case 5: // PT
            // current score 3518 (72/32768) [RMS: 64.69]
            bestparams.threshold = 0.963632345f;
            bestparams.pulsestrength = 2.06904531f;
            bestparams.distance1 = 0.0287600756f;
            bestparams.distance2 = 0.183034822f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 20496 (988/32768) [RMS: 93.51]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.14159644f;
            bestparams.pulsestrength = 3.50420499f;
            bestparams.topbit = 0.748402119f;
            bestparams.distance1 = 0.00319250347f;
            bestparams.distance2 = 0.218578994f;
            break;
        case 7: // PST
            // current score 5006 (102/32768) [RMS: 35.64]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.08452392f;
            bestparams.pulsestrength = 1.81916571f;
            bestparams.topbit = 0.904740691f;
            bestparams.distance1 = 0.0277621783f;
            bestparams.distance2 = 0.585185289f;
            break;
        }
    }

    else if (strcmp(chip, "6581_1585") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 8719 (948/32768) [RMS: 70.29]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.174544901f;
            bestparams.topbit = 0.180504948f;
            bestparams.distance1 = 0.107921958f;
            bestparams.distance2 = 2.36725044f;
            break;
        case 5: // PT
            // current score 1933 (96/32768) [RMS: 52.54]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.984207988f;
            bestparams.pulsestrength = 1.83862209f;
            bestparams.distance1 = 0.151734218f;
            bestparams.distance2 = 0.202220336f;
            break;
        case 6: // PS
            // current score 17068 (1170/32768) [RMS: 86.36]
            bestparams.threshold = 1.48120451f;
            bestparams.pulsestrength = 6.19636726f;
            bestparams.topbit = 0.831328928f;
            bestparams.distance1 = 0.000226263714f;
            bestparams.distance2 = 0.144217432f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 4075 (76/32768) [RMS: 30.81]
            bestparams.threshold = 1.02086127f;
            bestparams.pulsestrength = 1.57034767f;
            bestparams.topbit = 0.865189075f;
            bestparams.distance1 = 0.0384464264f;
            bestparams.distance2 = 0.529835522f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (strcmp(chip, "6581R4AR_3586_S") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 1887 (215/32768) [RMS: 64.97]
            bestparams.threshold = 0.94858247f;
            bestparams.pulsestrength = 1.f;
            bestparams.topbit = 1.05520427f;
            bestparams.distance1 = 2.20595884f;
            bestparams.distance2 = 20.6003361f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 2993 (151/32768) [RMS: 60.65]
            bestparams.threshold = 0.972008884f;
            bestparams.pulsestrength = 1.71443033f;
            bestparams.distance1 = 0.141484126f;
            bestparams.distance2 = 0.257483304f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 18550 (1118/32768) [RMS: 92.80]
            bestparams.threshold = 2.67324972f;
            bestparams.pulsestrength = 11.9622126f;
            bestparams.topbit = 1.22654665f;
            bestparams.distance1 = 0.399144709f;
            bestparams.distance2 = 0.207783923f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 4911 (91/32768) [RMS: 36.56]
            bestparams.threshold = 1.15800464f;
            bestparams.pulsestrength = 1.93585241f;
            bestparams.topbit = 0.940164089f;
            bestparams.distance1 = 0.0932772979f;
            bestparams.distance2 = 0.64203608f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580R5_5092_25") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1195 (169/32768)
            bestparams.threshold = 0.687141538f;
            bestparams.pulsestrength = 1.f;
            bestparams.topbit = 0.940658867f;
            bestparams.distance1 = 1.20837665f;
            bestparams.distance2 = 2.11195111f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5985 (253/32768)
            bestparams.threshold = 0.949803114f;
            bestparams.pulsestrength = 1.16611159f;
            bestparams.distance1 = 1.02045071f;
            bestparams.distance2 = 1.65621758f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 7620 (454/32768)
            bestparams.distFunc = Parameters::quadraticDistance;
            bestparams.threshold = 0.963866293f;
            bestparams.pulsestrength = 1.22095084f;
            bestparams.topbit = 1.01380754f;
            bestparams.distance1 = 0.0110885892f;
            bestparams.distance2 = 0.381492466f;
            break;
        case 7: // PST
            // current score 3701 (117/32768)
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.976761818f;
            bestparams.pulsestrength = 0.202727556f;
            bestparams.topbit = 0.988633931f;
            bestparams.distance1 = 0.939373314f;
            bestparams.distance2 = 9.37139416f;
            break;
        }
    }

    else if (strcmp(chip, "8580R5_5092_25_2") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1048 (120/32768)
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.814103305f;
            bestparams.topbit = 1.17548299f;
            bestparams.distance1 = 1.88967574f;
            bestparams.distance2 = 2.32063961f;
            break;
        case 5: // PT
            // current score 3670 (140/32768)
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.990784764f;
            bestparams.pulsestrength = 1.18064904f;
            bestparams.distance1 = 1.04774177f;
            bestparams.distance2 = 1.72867715f;
            break;
        case 6: // PS
            // current score 9312 (398/32768)
            bestparams.distFunc = Parameters::quadraticDistance;
            bestparams.threshold = 0.980230451f;
            bestparams.pulsestrength = 1.17020738f;
            bestparams.topbit = 0.987197578f;
            bestparams.distance1 = 0.0191217829f;
            bestparams.distance2 = 0.472027928f;
            break;
        case 7: // PST
            // current score 2997 (74/32768)
            bestparams.threshold = 0.907606602f;
            bestparams.pulsestrength = 0.555641115f;
            bestparams.topbit = 1.22714567f;
            bestparams.distance1 = 1.1921041f;
            bestparams.distance2 = 1.83306551f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_3493") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2190 (246/32768)
            bestparams.threshold = 0.731061876f;
            bestparams.topbit = 1.01355672f;
            bestparams.distance1 = 1.64468837f;
            bestparams.distance2 = 3.43933249f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5735 (232/32768)
            bestparams.threshold = 0.936719835f;
            bestparams.pulsestrength = 1.17875373f;
            bestparams.distance1 = 1.04700363f;
            bestparams.distance2 = 1.50305116f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 10921 (434/32768)
            bestparams.threshold = 0.945879936f;
            bestparams.pulsestrength = 1.19615328f;
            bestparams.topbit = 0.991938472f;
            bestparams.distance1 = 0.00203184155f;
            bestparams.distance2 = 0.295688927f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8880 (114/32768)
            bestparams.threshold = 0.944432676f;
            bestparams.pulsestrength = 1.11728537f;
            bestparams.topbit = 1.02444243f;
            bestparams.distance1 = 0.946709514f;
            bestparams.distance2 = 1.49661911f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_5092") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1167 (130/32768)
            bestparams.threshold = 0.812157929f;
            bestparams.topbit = 1.19008696f;
            bestparams.distance1 = 1.8724792f;
            bestparams.distance2 = 2.3072772f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 4773 (132/32768)
            bestparams.threshold = 0.979222834f;
            bestparams.pulsestrength = 1.15944064f;
            bestparams.distance1 = 1.06649458f;
            bestparams.distance2 = 1.58736694f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 9554 (349/32768)
            bestparams.threshold = 0.972246945f;
            bestparams.pulsestrength = 1.21901131f;
            bestparams.topbit = 0.972710431f;
            bestparams.distance1 = 0.00752384309f;
            bestparams.distance2 = 0.312743247f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 10131 (133/32768)
            bestparams.threshold = 1.06831551f;
            bestparams.pulsestrength = 0.120533176f;
            bestparams.topbit = 1.20669949f;
            bestparams.distance1 = 1.95325541f;
            bestparams.distance2 = 6.4570384f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_0590") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2143 (187/32768)
            bestparams.threshold = 0.688183069f;
            bestparams.topbit = 0.929571509f;
            bestparams.distance1 = 1.21250761f;
            bestparams.distance2 = 2.13566232f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8588 (209/32768)
            bestparams.threshold = 0.956455648f;
            bestparams.pulsestrength = 1.13230097f;
            bestparams.distance1 = 1.08532834f;
            bestparams.distance2 = 1.51373041f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 11002 (452/32768)
            bestparams.threshold = 0.92552191f;
            bestparams.pulsestrength = 1.08765888f;
            bestparams.topbit = 0.977773845f;
            bestparams.distance1 = 0.000131223627f;
            bestparams.distance2 = 0.286722243f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 7252 (115/32768)
            bestparams.threshold = 0.90436697f;
            bestparams.pulsestrength = 0.606920123f;
            bestparams.topbit = 1.02331626f;
            bestparams.distance1 = 1.115134f;
            bestparams.distance2 = 1.67853987f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_1087") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1627 (137/32768)
            bestparams.threshold = 0.795011938f;
            bestparams.topbit = 1.54905677f;
            bestparams.distance1 = 1.79432333f;
            bestparams.distance2 = 2.24898171f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7898 (162/32768)
            bestparams.threshold = 0.9482705f;
            bestparams.pulsestrength = 1.21793139f;
            bestparams.distance1 = 1.04166055f;
            bestparams.distance2 = 1.37272894f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 9804 (337/32768)
            bestparams.threshold = 0.954935849f;
            bestparams.pulsestrength = 1.28759611f;
            bestparams.topbit = 1.00321376f;
            bestparams.distance1 = 0.000331178948f;
            bestparams.distance2 = 0.151375741f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 3184 (56/32768)
            bestparams.threshold = 0.945096612f;
            bestparams.pulsestrength = 0.905796111f;
            bestparams.topbit = 1.06510091f;
            bestparams.distance1 = 1.05054963f;
            bestparams.distance2 = 1.4661454f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_1088") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 10788 (354/32768) [RMS: 58.31]
            bestparams.threshold = 0.841851234f;
            bestparams.topbit = 1.09233654f;
            bestparams.distance1 = 1.85262764f;
            bestparams.distance2 = 6.22224379f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 10635 (289/32768) [RMS: 108.81]
            bestparams.threshold = 0.929835618f;
            bestparams.pulsestrength = 1.12836814f;
            bestparams.distance1 = 1.10453653f;
            bestparams.distance2 = 1.48065746f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 12255 (554/32768) [RMS: 102.27]
            bestparams.threshold = 0.911938608f;
            bestparams.pulsestrength = 1.2278074f;
            bestparams.topbit = 0.996440411f;
            bestparams.distance1 = 0.000117214302f;
            bestparams.distance2 = 0.18948476f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 6995 (139/32768) [RMS: 55.78]
            bestparams.threshold = 0.932317019f;
            bestparams.pulsestrength = 1.2068342f;
            bestparams.topbit = 1.03892183f;
            bestparams.distance1 = 0.891974986f;
            bestparams.distance2 = 1.42451835f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_1489") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 4837 (388/32768) [RMS: 76.07]
            bestparams.threshold = 0.89762634f;
            bestparams.topbit = 56.7594185f;
            bestparams.distance1 = 7.68995237f;
            bestparams.distance2 = 12.0754194f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9298 (506/32768) [RMS: 128.15]
            bestparams.threshold = 0.867885351f;
            bestparams.pulsestrength = 1.4511894f;
            bestparams.distance1 = 1.07057536f;
            bestparams.distance2 = 1.43333757f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 13168 (718/32768) [RMS: 123.35]
            bestparams.threshold = 0.89255774f;
            bestparams.pulsestrength = 1.75615835f;
            bestparams.topbit = 1.2253896f;
            bestparams.distance1 = 0.0245045591f;
            bestparams.distance2 = 0.12982437f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 6702 (300/32768) [RMS: 71.01]
            bestparams.threshold = 0.91124934f;
            bestparams.pulsestrength = 0.909965038f;
            bestparams.topbit = 0.963609755f;
            bestparams.distance1 = 1.07445884f;
            bestparams.distance2 = 1.82399702f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_1891") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 3401 (283/32768)
            bestparams.threshold = 0.74335587f;
            bestparams.topbit = 1.13261592f;
            bestparams.distance1 = 1.83344603f;
            bestparams.distance2 = 3.90392399f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9242 (255/32768)
            bestparams.threshold = 0.924806535f;
            bestparams.pulsestrength = 1.20028079f;
            bestparams.distance1 = 1.07056773f;
            bestparams.distance2 = 1.43234241f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 13940 (609/32768)
            bestparams.threshold = 0.901862085f;
            bestparams.pulsestrength = 1.11271441f;
            bestparams.topbit = 1.02348149f;
            bestparams.distance1 = 0.000376841635f;
            bestparams.distance2 = 0.220544845f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8423 (181/32768)
            bestparams.threshold = 0.987342596f;
            bestparams.pulsestrength = 0.215089902f;
            bestparams.topbit = 0.995823205f;
            bestparams.distance1 = 0.78425771f;
            bestparams.distance2 = 2.62625265f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_3190") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2593 (269/32768)
            bestparams.threshold = 0.742079914f;
            bestparams.topbit = 1.16795468f;
            bestparams.distance1 = 1.82698667f;
            bestparams.distance2 = 3.90259051f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7136 (302/32768)
            bestparams.threshold = 0.920148611f;
            bestparams.pulsestrength = 1.2706455f;
            bestparams.distance1 = 1.03514659f;
            bestparams.distance2 = 1.45814693f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 14360 (668/32768)
            bestparams.threshold = 0.911647439f;
            bestparams.pulsestrength = 1.19287789f;
            bestparams.topbit = 1.00216305f;
            bestparams.distance1 = 0.000113861912f;
            bestparams.distance2 = 0.257546455f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8601 (135/32768)
            bestparams.threshold = 0.940942466f;
            bestparams.pulsestrength = 1.19135451f;
            bestparams.topbit = 1.07283616f;
            bestparams.distance1 = 0.968848467f;
            bestparams.distance2 = 1.48712325f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_3491") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1946 (235/32768)
            bestparams.threshold = 0.719091594f;
            bestparams.topbit = 0.998161793f;
            bestparams.distance1 = 1.58747554f;
            bestparams.distance2 = 3.35766935f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8536 (228/32768)
            bestparams.threshold = 0.926853895f;
            bestparams.pulsestrength = 1.20506012f;
            bestparams.distance1 = 1.06335676f;
            bestparams.distance2 = 1.3995806f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 12477 (511/32768)
            bestparams.threshold = 0.914696157f;
            bestparams.pulsestrength = 1.22517538f;
            bestparams.topbit = 1.07253551f;
            bestparams.distance1 = 0.000195453656f;
            bestparams.distance2 = 0.162984177f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 3859 (98/32768)
            bestparams.threshold = 0.953816235f;
            bestparams.pulsestrength = 1.34997332f;
            bestparams.topbit = 1.12209225f;
            bestparams.distance1 = 0.954176545f;
            bestparams.distance2 = 1.35542691f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_3987") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2082 (290/32768)
            bestparams.threshold = 0.707245409f;
            bestparams.topbit = 0.923609674f;
            bestparams.distance1 = 1.4772234f;
            bestparams.distance2 = 3.17645168f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9212 (287/32768)
            bestparams.threshold = 0.903500497f;
            bestparams.pulsestrength = 1.02719498f;
            bestparams.distance1 = 1.06971335f;
            bestparams.distance2 = 1.4370302f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 11109 (503/32768)
            bestparams.threshold = 0.933880389f;
            bestparams.pulsestrength = 1.29445052f;
            bestparams.topbit = 1.06563056f;
            bestparams.distance1 = 0.000236776366f;
            bestparams.distance2 = 0.152991742f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 5224 (152/32768)
            bestparams.threshold = 0.860679209f;
            bestparams.pulsestrength = 0.109913498f;
            bestparams.topbit = 0.871102452f;
            bestparams.distance1 = 1.16131163f;
            bestparams.distance2 = 2.75297952f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_4388") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2274 (288/32768)
            bestparams.threshold = 0.727870882f;
            bestparams.topbit = 0.981630623f;
            bestparams.distance1 = 1.62720287f;
            bestparams.distance2 = 3.45849872f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7549 (192/32768)
            bestparams.threshold = 0.933463514f;
            bestparams.pulsestrength = 1.24167061f;
            bestparams.distance1 = 1.07614005f;
            bestparams.distance2 = 1.32810688f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 14028 (521/32768)
            bestparams.threshold = 0.973695457f;
            bestparams.pulsestrength = 1.51140547f;
            bestparams.topbit = 1.06569493f;
            bestparams.distance1 = 0.0182949118f;
            bestparams.distance2 = 0.109501146f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 5262 (87/32768)
            bestparams.threshold = 0.980877638f;
            bestparams.pulsestrength = 1.36146891f;
            bestparams.topbit = 1.07633293f;
            bestparams.distance1 = 0.907621384f;
            bestparams.distance2 = 1.34616482f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_4589") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 12084 (360/32768)
            bestparams.threshold = 0.711074412f;
            bestparams.topbit = 0.947770417f;
            bestparams.distance1 = 1.55405724f;
            bestparams.distance2 = 3.37904644f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7835 (255/32768)
            bestparams.threshold = 0.916386724f;
            bestparams.pulsestrength = 1.23537338f;
            bestparams.distance1 = 1.04378891f;
            bestparams.distance2 = 1.40279818f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 14943 (640/32768)
            bestparams.threshold = 0.921671867f;
            bestparams.pulsestrength = 1.22090995f;
            bestparams.topbit = 1.04608607f;
            bestparams.distance1 = 0.0103900218f;
            bestparams.distance2 = 0.193279341f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 9867 (227/32768)
            bestparams.threshold = 0.888757348f;
            bestparams.pulsestrength = 0.0402527526f;
            bestparams.topbit = 0.945833206f;
            bestparams.distance1 = 1.31806123f;
            bestparams.distance2 = 4.1472621f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_4790") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1920 (242/32768)
            bestparams.threshold = 0.725565016f;
            bestparams.topbit = 0.995874524f;
            bestparams.distance1 = 1.61511159f;
            bestparams.distance2 = 3.41737127f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8580 (230/32768)
            bestparams.threshold = 0.922645688f;
            bestparams.pulsestrength = 1.10116744f;
            bestparams.distance1 = 1.07542741f;
            bestparams.distance2 = 1.42268991f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 10304 (429/32768)
            bestparams.threshold = 0.948176563f;
            bestparams.pulsestrength = 1.24627411f;
            bestparams.topbit = 1.04925656f;
            bestparams.distance1 = 0.0146395834f;
            bestparams.distance2 = 0.174901351f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 4036 (138/32768)
            bestparams.threshold = 0.827258289f;
            bestparams.pulsestrength = 0.377220988f;
            bestparams.topbit = 0.85699594f;
            bestparams.distance1 = 1.12109971f;
            bestparams.distance2 = 1.78383458f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_4887") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 745 (77/32768) [RMS: 53.74]
            bestparams.threshold = 0.816124022f;
            bestparams.topbit = 1.31208789f;
            bestparams.distance1 = 1.92347884f;
            bestparams.distance2 = 2.35027933f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7199 (192/32768) [RMS: 88.43]
            bestparams.threshold = 0.917997837f;
            bestparams.pulsestrength = 1.01248944f;
            bestparams.distance1 = 1.05761552f;
            bestparams.distance2 = 1.37529826f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 9864 (333/32768) [RMS: 86.29]
            bestparams.threshold = 0.970038712f;
            bestparams.pulsestrength = 1.30298805f;
            bestparams.topbit = 1.00844693f;
            bestparams.distance1 = 0.0097996993f;
            bestparams.distance2 = 0.146854922f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 4809 (60/32768) [RMS: 45.37]
            bestparams.threshold = 0.941834152f;
            bestparams.pulsestrength = 0.991132736f;
            bestparams.topbit = 1.06401193f;
            bestparams.distance1 = 0.995310068f;
            bestparams.distance2 = 1.41105855f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "8580_5092_2") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1359 (150/32768)
            bestparams.threshold = 0.841939628f;
            bestparams.topbit = 1.1484369f;
            bestparams.distance1 = 1.66275322f;
            bestparams.distance2 = 4.84815454f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5235 (226/32768)
            bestparams.threshold = 0.931995928f;
            bestparams.pulsestrength = 1.12565029f;
            bestparams.distance1 = 1.04701793f;
            bestparams.distance2 = 1.50431061f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 11625 (472/32768)
            bestparams.threshold = 0.919857144f;
            bestparams.pulsestrength = 0.921489298f;
            bestparams.topbit = 0.977529645f;
            bestparams.distance1 = 0.0296642669f;
            bestparams.distance2 = 0.384828329f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 6693 (63/32768)
            bestparams.threshold = 0.955013871f;
            bestparams.pulsestrength = 1.03108287f;
            bestparams.topbit = 1.1251868f;
            bestparams.distance1 = 1.02317023f;
            bestparams.distance2 = 1.50494277f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (strcmp(chip, "broken0384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 20337 (1579/32768) [RMS: 88.57]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.000637792516f;
            bestparams.topbit = 1.56725872f;
            bestparams.distance1 = 0.00036806846f;
            bestparams.distance2 = 1.51800942f;
            break;
        case 5: // PT
            // current score 5194 (240/32768) [RMS: 83.54]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.924824238f;
            bestparams.pulsestrength = 1.96749473f;
            bestparams.distance1 = 0.0891806409f;
            bestparams.distance2 = 0.234794483f;
            break;
        case 6: // PS
            // current score 31015 (2181/32768) [RMS: 114.99]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.2328074f;
            bestparams.pulsestrength = 3.9719491f;
            bestparams.topbit = 0.73079139f;
            bestparams.distance1 = 0.00156516861f;
            bestparams.distance2 = 0.314677745f;
            break;
        case 7: // PST
            // current score 9874 (201/32768) [RMS: 52.30]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.08558261f;
            bestparams.pulsestrength = 1.52781796f;
            bestparams.topbit = 0.857638359f;
            bestparams.distance1 = 0.152927235f;
            bestparams.distance2 = 1.02657032f;
            break;
        }
    }

    else if (strcmp(chip, "brokenr4ar3488") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 25216 (1567/32768)
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.0424066633f;
            bestparams.topbit = 2.43467259f;
            bestparams.distance1 = 0.000421410281f;
            bestparams.distance2 = 2.81357718f;
            break;
        case 5: // PT
            // current score 10956 (235/32768)
            bestparams.threshold = 0.969468355f;
            bestparams.pulsestrength = 1.91159272f;
            bestparams.distance1 = 0.042297896f;
            bestparams.distance2 = 0.349661946f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 22701 (1148/32768)
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.99167538f;
            bestparams.pulsestrength = 4.0302434f;
            bestparams.topbit = 1.22495222f;
            bestparams.distance1 = 1.01453114f;
            bestparams.distance2 = 0.844035387f;
            break;
        case 7: // PST
            // current score 7200 (132/32768)
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.1455301f;
            bestparams.pulsestrength = 1.33257663f;
            bestparams.topbit = 0.960132778f;
            bestparams.distance1 = 0.381222129f;
            bestparams.distance2 = 1.3617624f;
            break;
        }
    }

    else {
        std::cout << "Unrecognized chip" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif
    if (bestparams.distance2 == 0.f)
        bestparams.distance2 = bestparams.distance1;

    // Calculate current score
    score_t bestscore = bestparams.Score(wave, is8580, reference, true, 4096 * 255);
    std::cout << "# initial score " << std::dec
        << bestscore << std::endl
        << bestparams.toString() << std::endl << std::endl;
    if (bestscore.audible_error == 0)
        exit(EXIT_SUCCESS);

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
                exit(EXIT_SUCCESS);
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
    if (argc != 3)
    {
        std::cout << "Usage " << argv[0] << " <waveform> <chip>" << std::endl;
        exit(EXIT_FAILURE);
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
