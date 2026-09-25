/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2013-2026 Leandro Nini <drfiemost@users.sourceforge.net>
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

static std::normal_distribution<> normal_dist(1.0, 0.02);
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
    if (std::strcmp(chip, "locu128_6581_cbm_4383") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 751 (390/32768) [RMS: -13.56]
            bestparams.threshold = 0.885195196f;
            bestparams.topbit = 1.48950899f;
            bestparams.distance1 = 2.02920771f;
            bestparams.distance2 = 3.7329123f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 612 (102/32768) [RMS: -15.35]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.01262534f;
            bestparams.pulsestrength = 2.46070528f;
            bestparams.distance1 = 0.0537485816f;
            bestparams.distance2 = 0.0986242667f;
            break;
        case 6: // PS
            // current score 5 (1536/32768) [RMS: -15.71]
            bestparams.threshold = 0.74411875f;
            bestparams.pulsestrength = 0.232185543f;
            bestparams.topbit = 0.062505953f;
            bestparams.distance1 = 0.0717892349f;
            bestparams.distance2 = 0.000250642159f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 0 (138/32768) [RMS: -25.46]
            bestparams.threshold = 1.10582423f;
            bestparams.pulsestrength = 1.94850934f;
            bestparams.topbit = 0.578988612f;
            bestparams.distance1 = 0.0783150643f;
            bestparams.distance2 = 0.300926387f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581_0784") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 269 (503/32768) [RMS: -13.55]
            bestparams.threshold = 0.86450088f;
            bestparams.topbit = 1.24369597f;
            bestparams.distance1 = 3.12171197f;
            bestparams.distance2 = 3.52461076f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 2016 (141/32768) [RMS: -13.82]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.938275278f;
            bestparams.pulsestrength = 1.70019507f;
            bestparams.distance1 = 1.10584641f;
            bestparams.distance2 = 1.11688411f;
            break;
        case 6: // PS
            // current score 0 (960/32768) [RMS: -12.01]
            bestparams.threshold = 1.36234152f;
            bestparams.pulsestrength = 5.8827219f;
            bestparams.topbit = 1.09117806f;
            bestparams.distance1 = 0.0813409165f;
            bestparams.distance2 = 0.0651179627f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 0 (197/32768) [RMS: -19.59]
            bestparams.threshold = 1.22868216f;
            bestparams.pulsestrength = 1.54707289f;
            bestparams.topbit = 0.148325935f;
            bestparams.distance1 = 1.32943034f;
            bestparams.distance2 = 1.40600038f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581_3084") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 483 (450/32768) [RMS: -13.17]
            bestparams.threshold = 0.8754071f;
            bestparams.topbit = 5.05079842f;
            bestparams.distance1 = 4.10759544f;
            bestparams.distance2 = 3.69861078f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5781 (198/32768) [RMS: -11.68]
            bestparams.threshold = 0.999375761f;
            bestparams.pulsestrength = 2.03652263f;
            bestparams.distance1 = 1.05754781f;
            bestparams.distance2 = 1.15805364f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 144 (1083/32768) [RMS: -10.64]
            bestparams.threshold = 1.20781219f;
            bestparams.pulsestrength = 4.51915121f;
            bestparams.topbit = 1.3795054f;
            bestparams.distance1 = 0.0633469224f;
            bestparams.distance2 = 0.0802473426f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 3 (257/32768) [RMS: -18.58]
            bestparams.threshold = 1.71266961f;
            bestparams.pulsestrength = 2.02621317f;
            bestparams.topbit = 0.00574938022f;
            bestparams.distance1 = 1.57895577f;
            bestparams.distance2 = 1.37034345f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "cbm3384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 242 (970/32768) [RMS: -12.63]
            bestparams.threshold = 0.830523849f;
            bestparams.topbit = 8.53260136f;
            bestparams.distance1 = 3.1122036f;
            bestparams.distance2 = 2.65378499f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 3618 (42/32768) [RMS: -11.21]
            bestparams.threshold = 0.977547884f;
            bestparams.pulsestrength = 2.30878043f;
            bestparams.distance1 = 0.0191228352f;
            bestparams.distance2 = 0.176398084f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 361 (1126/32768) [RMS: -9.13]
            bestparams.threshold = 1.23311937f;
            bestparams.pulsestrength = 4.07948732f;
            bestparams.topbit = 1.58626962f;
            bestparams.distance1 = 0.13691774f;
            bestparams.distance2 = 0.155669421f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 124 (278/32768) [RMS: -18.31]
            bestparams.threshold = 1.07696235f;
            bestparams.pulsestrength = 1.72518933f;
            bestparams.topbit = 1.75790918f;
            bestparams.distance1 = 0.345677555f;
            bestparams.distance2 = 0.434917927f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "cbm4383") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 256 (897/32768) [RMS: -13.52]
            bestparams.threshold = 0.892982721f;
            bestparams.topbit = 1.09948468f;
            bestparams.distance1 = 4.73459387f;
            bestparams.distance2 = 4.82480574f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 2130 (131/32768) [RMS: -11.93]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.00092328f;
            bestparams.pulsestrength = 2.42803788f;
            bestparams.distance1 = 0.0113755139f;
            bestparams.distance2 = 0.162516415f;
            break;
        case 6: // PS
            // current score 337 (1020/32768) [RMS: -10.22]
            bestparams.threshold = 1.09252477f;
            bestparams.pulsestrength = 3.45962834f;
            bestparams.topbit = 1.48039985f;
            bestparams.distance1 = 0.0714268684f;
            bestparams.distance2 = 0.100377731f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 56 (233/32768) [RMS: -20.35]
            bestparams.threshold = 1.10278761f;
            bestparams.pulsestrength = 2.43588948f;
            bestparams.topbit = 1.87595975f;
            bestparams.distance1 = 0.178646505f;
            bestparams.distance2 = 0.224552304f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R4AR_3789_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 905 (712/32768) [RMS: -13.57]
            bestparams.threshold = 0.988546371f;
            bestparams.topbit = 1.10214388f;
            bestparams.distance1 = 8.60669518f;
            bestparams.distance2 = 23.9268799f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 4621 (104/32768) [RMS: -11.74]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.978124142f;
            bestparams.pulsestrength = 2.08345437f;
            bestparams.distance1 = 0.0454150252f;
            bestparams.distance2 = 0.203794882f;
            break;
        case 6: // PS
            // current score 113 (1067/32768) [RMS: -10.38]
            bestparams.threshold = 1.10930324f;
            bestparams.pulsestrength = 3.50717831f;
            bestparams.topbit = 1.47075057f;
            bestparams.distance1 = 0.0881707147f;
            bestparams.distance2 = 0.104346052f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 51 (263/32768) [RMS: -22.10]
            bestparams.threshold = 0.797409892f;
            bestparams.pulsestrength = 0.375682682f;
            bestparams.topbit = 1.73399341f;
            bestparams.distance1 = 0.139801368f;
            bestparams.distance2 = 0.321204275f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R4AR_4486_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2102 (1759/32768) [RMS: -9.87]
            bestparams.threshold = 0.0945058465f;
            bestparams.topbit = 0.922308445f;
            bestparams.distance1 = 0.0948866159f;
            bestparams.distance2 = 1.90828049f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 3604 (63/32768) [RMS: -11.20]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.998088539f;
            bestparams.pulsestrength = 2.51015329f;
            bestparams.distance1 = 0.0422255732f;
            bestparams.distance2 = 0.164421782f;
            break;
        case 6: // PS
            // current score 607 (1164/32768) [RMS: -9.61]
            bestparams.threshold = 1.03459346f;
            bestparams.pulsestrength = 3.16868901f;
            bestparams.topbit = 1.43029153f;
            bestparams.distance1 = 0.0646577701f;
            bestparams.distance2 = 0.102762282f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 126 (263/32768) [RMS: -18.39]
            bestparams.threshold = 0.99378103f;
            bestparams.pulsestrength = 1.42474997f;
            bestparams.topbit = 1.50541234f;
            bestparams.distance1 = 0.254800051f;
            bestparams.distance2 = 0.43316409f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R4AR_5286_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 242 (663/32768) [RMS: -13.18]
            bestparams.threshold = 0.85966903f;
            bestparams.topbit = 2.52508402f;
            bestparams.distance1 = 3.67300773f;
            bestparams.distance2 = 3.32991076f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5586 (147/32768) [RMS: -10.06]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.965520382f;
            bestparams.pulsestrength = 1.97317994f;
            bestparams.distance1 = 1.03463221f;
            bestparams.distance2 = 1.17572582f;
            break;
        case 6: // PS
            // current score 469 (1221/32768) [RMS: -8.83]
            bestparams.threshold = 1.19410479f;
            bestparams.pulsestrength = 3.69806218f;
            bestparams.topbit = 1.61734545f;
            bestparams.distance1 = 0.157596424f;
            bestparams.distance2 = 0.177035213f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 76 (539/32768) [RMS: -20.44]
            bestparams.threshold = 1.10539722f;
            bestparams.pulsestrength = 1.44411063f;
            bestparams.topbit = 0.302006781f;
            bestparams.distance1 = 0.515361965f;
            bestparams.distance2 = 0.703935504f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R3_0486_S") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 406 (764/32768) [RMS: -13.55]
            bestparams.threshold = 0.79111582f;
            bestparams.topbit = 1.06053483f;
            bestparams.distance1 = 1.97922957f;
            bestparams.distance2 = 2.67848182f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 4590 (124/32768) [RMS: -11.40]
            bestparams.threshold = 0.941692829f;
            bestparams.pulsestrength = 1.80072665f;
            bestparams.distance1 = 0.033124879f;
            bestparams.distance2 = 0.232303441f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 211 (1036/32768) [RMS: -10.33]
            bestparams.threshold = 1.09182823f;
            bestparams.pulsestrength = 3.42711949f;
            bestparams.topbit = 1.41560268f;
            bestparams.distance1 = 0.0792779177f;
            bestparams.distance2 = 0.102292374f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 57 (333/32768) [RMS: -20.29]
            bestparams.threshold = 1.52983034f;
            bestparams.pulsestrength = 2.31868196f;
            bestparams.topbit = 0.0413390137f;
            bestparams.distance1 = 0.959919035f;
            bestparams.distance2 = 0.858174801f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R3_4785") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 311 (595/32768) [RMS: -15.83]
            bestparams.threshold = 0.776902437f;
            bestparams.topbit = 2.68711281f;
            bestparams.distance1 = 2.17907f;
            bestparams.distance2 = 1.64271462f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 582 (57/32768) [RMS: -14.98]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 1.01866758f;
            bestparams.pulsestrength = 2.69177628f;
            bestparams.distance1 = 0.0233543925f;
            bestparams.distance2 = 0.0850229636f;
            break;
        case 6: // PS
            // current score 0 (694/32768) [RMS: -12.33]
            bestparams.threshold = 1.5300988f;
            bestparams.pulsestrength = 7.5939908f;
            bestparams.topbit = 0.984048605f;
            bestparams.distance1 = 0.0752864107f;
            bestparams.distance2 = 0.0527844131f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 0 (150/32768) [RMS: -24.50]
            bestparams.threshold = 1.1543535f;
            bestparams.pulsestrength = 2.04660106f;
            bestparams.topbit = 0.998426616f;
            bestparams.distance1 = 0.281716466f;
            bestparams.distance2 = 0.310142517f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R3_4885") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 257 (480/32768) [RMS: -12.63]
            bestparams.threshold = 0.811963379f;
            bestparams.topbit = 7.0373621f;
            bestparams.distance1 = 2.8535459f;
            bestparams.distance2 = 2.44088554f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 1956 (36/32768) [RMS: -11.88]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.992383003f;
            bestparams.pulsestrength = 2.49721408f;
            bestparams.distance1 = 0.0148989018f;
            bestparams.distance2 = 0.14348942f;
            break;
        case 6: // PS
            // current score 172 (866/32768) [RMS: -10.13]
            bestparams.threshold = 1.0444051f;
            bestparams.pulsestrength = 3.06324339f;
            bestparams.topbit = 1.40519512f;
            bestparams.distance1 = 0.0720482841f;
            bestparams.distance2 = 0.108887099f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 56 (199/32768) [RMS: -20.65]
            bestparams.threshold = 0.893137574f;
            bestparams.pulsestrength = 1.27701271f;
            bestparams.topbit = 1.54157186f;
            bestparams.distance1 = 0.090017207f;
            bestparams.distance2 = 0.229917094f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R4AR_3488_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 255 (455/32768) [RMS: -13.57]
            bestparams.threshold = 0.80822444f;
            bestparams.topbit = 2.20796704f;
            bestparams.distance1 = 2.83114409f;
            bestparams.distance2 = 2.34842539f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 3518 (72/32768) [RMS: -11.95]
            bestparams.threshold = 0.963632345f;
            bestparams.pulsestrength = 2.06904531f;
            bestparams.distance1 = 0.0287600756f;
            bestparams.distance2 = 0.183034822f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 216 (910/32768) [RMS: -10.66]
            bestparams.threshold = 0.878465831f;
            bestparams.pulsestrength = 1.91333973f;
            bestparams.topbit = 1.2073791f;
            bestparams.distance1 = 0.00125265087f;
            bestparams.distance2 = 0.0837968513f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 65 (204/32768) [RMS: -20.71]
            bestparams.threshold = 0.827495337f;
            bestparams.pulsestrength = 0.933227003f;
            bestparams.topbit = 1.36435914f;
            bestparams.distance1 = 0.0481203347f;
            bestparams.distance2 = 0.223367706f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581_1585") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 1408 (1831/32768) [RMS: -9.88]
            bestparams.threshold = 0.101508208f;
            bestparams.topbit = 0.142596543f;
            bestparams.distance1 = 0.101549007f;
            bestparams.distance2 = 1.89587104f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 1933 (96/32768) [RMS: -13.75]
            bestparams.distFunc = Parameters::linearDistance;
            bestparams.threshold = 0.984207988f;
            bestparams.pulsestrength = 1.83862209f;
            bestparams.distance1 = 0.151734218f;
            bestparams.distance2 = 0.202220336f;
            break;
        case 6: // PS
            // current score 0 (1403/32768) [RMS: -12.65]
            bestparams.threshold = 1.00993466f;
            bestparams.pulsestrength = 3.15449047f;
            bestparams.topbit = 0.720546722f;
            bestparams.distance1 = 0.000118476979f;
            bestparams.distance2 = 0.0512445085f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 0 (199/32768) [RMS: -23.01]
            bestparams.threshold = 0.953606069f;
            bestparams.pulsestrength = 1.11801636f;
            bestparams.topbit = 0.524759948f;
            bestparams.distance1 = 0.0325532295f;
            bestparams.distance2 = 0.465396047f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R4AR_3586_S") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 878 (407/32768) [RMS: -13.47]
            bestparams.threshold = 0.893505216f;
            bestparams.topbit = 1.48727369f;
            bestparams.distance1 = 2.0544138f;
            bestparams.distance2 = 3.9500246f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 2993 (151/32768) [RMS: -12.51]
            bestparams.threshold = 0.972008884f;
            bestparams.pulsestrength = 1.71443033f;
            bestparams.distance1 = 0.141484126f;
            bestparams.distance2 = 0.257483304f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 3 (998/32768) [RMS: -10.98]
            bestparams.threshold = 1.07824457f;
            bestparams.pulsestrength = 3.56869817f;
            bestparams.topbit = 1.29889834f;
            bestparams.distance1 = 0.0312500745f;
            bestparams.distance2 = 0.0689333752f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 0 (123/32768) [RMS: -17.67]
            bestparams.threshold = 1.0823282f;
            bestparams.pulsestrength = 1.65954721f;
            bestparams.topbit = 1.01421452f;
            bestparams.distance1 = 0.0934818462f;
            bestparams.distance2 = 0.599186718f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580R5_5092_25") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1193 (168/32768) [RMS: -13.30]
            bestparams.threshold = 0.6865291f;
            bestparams.topbit = 0.941219449f;
            bestparams.distance1 = 1.20599532f;
            bestparams.distance2 = 2.1035006f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5649 (251/32768) [RMS: -6.46]
            bestparams.threshold = 0.947981834f;
            bestparams.pulsestrength = 1.1519047f;
            bestparams.distance1 = 1.02821982f;
            bestparams.distance2 = 1.66400278f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 7620 (454/32768) [RMS: -7.02]
            bestparams.distFunc = Parameters::quadraticDistance;
            bestparams.threshold = 0.963866293f;
            bestparams.pulsestrength = 1.22095084f;
            bestparams.topbit = 1.01380754f;
            bestparams.distance1 = 0.0110885892f;
            bestparams.distance2 = 0.381492466f;
            break;
        case 7: // PST
            // current score 3689 (115/32768) [RMS: -11.89]
            bestparams.threshold = 0.975387692f;
            bestparams.pulsestrength = 0.206782967f;
            bestparams.topbit = 0.988257587f;
            bestparams.distance1 = 0.980601788f;
            bestparams.distance2 = 9.14362907f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580R5_5092_25_2") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1048 (120/32768) [RMS: -13.56]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.814103305f;
            bestparams.topbit = 1.17548299f;
            bestparams.distance1 = 1.88967574f;
            bestparams.distance2 = 2.32063961f;
            break;
        case 5: // PT
            // current score 3670 (140/32768) [RMS: -6.41]
            bestparams.distFunc = Parameters::exponentialDistance;
            bestparams.threshold = 0.990784764f;
            bestparams.pulsestrength = 1.18064904f;
            bestparams.distance1 = 1.04774177f;
            bestparams.distance2 = 1.72867715f;
            break;
        case 6: // PS
            // current score 9244 (407/32768) [RMS: -6.95]
            bestparams.threshold = 0.982130826f;
            bestparams.pulsestrength = 1.19545388f;
            bestparams.topbit = 0.984055102f;
            bestparams.distance1 = 0.0200770069f;
            bestparams.distance2 = 0.456685066f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 356 (19/32768) [RMS: -11.95]
            bestparams.threshold = 0.961569846f;
            bestparams.pulsestrength = 0.9459517f;
            bestparams.topbit = 1.03952408f;
            bestparams.distance1 = 1.0483191f;
            bestparams.distance2 = 1.69999886f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_3493") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2182 (244/32768) [RMS: -13.09]
            bestparams.threshold = 0.732075274f;
            bestparams.topbit = 1.01875746f;
            bestparams.distance1 = 1.65055382f;
            bestparams.distance2 = 3.44771957f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5735 (232/32768) [RMS: -7.15]
            bestparams.threshold = 0.936719835f;
            bestparams.pulsestrength = 1.17875373f;
            bestparams.distance1 = 1.04700363f;
            bestparams.distance2 = 1.50305116f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 10895 (435/32768) [RMS: -7.53]
            bestparams.threshold = 0.944479704f;
            bestparams.pulsestrength = 1.19168735f;
            bestparams.topbit = 0.990218341f;
            bestparams.distance1 = 0.00204254151f;
            bestparams.distance2 = 0.296270579f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8848 (111/32768) [RMS: -12.56]
            bestparams.threshold = 0.943110585f;
            bestparams.pulsestrength = 1.0835638f;
            bestparams.topbit = 1.02020848f;
            bestparams.distance1 = 0.95966351f;
            bestparams.distance2 = 1.51834857f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_5092") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1167 (130/32768) [RMS: -13.56]
            bestparams.threshold = 0.812157929f;
            bestparams.topbit = 1.19008696f;
            bestparams.distance1 = 1.8724792f;
            bestparams.distance2 = 2.3072772f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 4773 (132/32768) [RMS: -7.13]
            bestparams.threshold = 0.979222834f;
            bestparams.pulsestrength = 1.15944064f;
            bestparams.distance1 = 1.06649458f;
            bestparams.distance2 = 1.58736694f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 9499 (349/32768) [RMS: -7.68]
            bestparams.threshold = 0.967251718f;
            bestparams.pulsestrength = 1.20654142f;
            bestparams.topbit = 0.966849685f;
            bestparams.distance1 = 0.00760078849f;
            bestparams.distance2 = 0.314019769f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 10131 (133/32768) [RMS: -12.21]
            bestparams.threshold = 1.06831551f;
            bestparams.pulsestrength = 0.120533176f;
            bestparams.topbit = 1.20669949f;
            bestparams.distance1 = 1.95325541f;
            bestparams.distance2 = 6.4570384f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_0590") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2143 (187/32768) [RMS: -13.31]
            bestparams.threshold = 0.688183069f;
            bestparams.topbit = 0.929571509f;
            bestparams.distance1 = 1.21250761f;
            bestparams.distance2 = 2.13566232f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8480 (213/32768) [RMS: -7.47]
            bestparams.threshold = 0.955921412f;
            bestparams.pulsestrength = 1.13047683f;
            bestparams.distance1 = 1.09507132f;
            bestparams.distance2 = 1.51376963f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 10798 (447/32768) [RMS: -7.84]
            bestparams.threshold = 0.925701499f;
            bestparams.pulsestrength = 1.09079564f;
            bestparams.topbit = 0.977671206f;
            bestparams.distance1 = 0.000129987471f;
            bestparams.distance2 = 0.285061598f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 7247 (117/32768) [RMS: -13.46]
            bestparams.threshold = 0.897638917f;
            bestparams.pulsestrength = 0.602467358f;
            bestparams.topbit = 1.01111174f;
            bestparams.distance1 = 1.12252307f;
            bestparams.distance2 = 1.67404807f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_1087") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1615 (134/32768) [RMS: -13.55]
            bestparams.threshold = 0.791922331f;
            bestparams.topbit = 1.27795017f;
            bestparams.distance1 = 1.77714765f;
            bestparams.distance2 = 2.21664143f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7898 (162/32768) [RMS: -8.63]
            bestparams.threshold = 0.9482705f;
            bestparams.pulsestrength = 1.21793139f;
            bestparams.distance1 = 1.04166055f;
            bestparams.distance2 = 1.37272894f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 9804 (337/32768) [RMS: -9.12]
            bestparams.threshold = 0.954935849f;
            bestparams.pulsestrength = 1.28759611f;
            bestparams.topbit = 1.00321376f;
            bestparams.distance1 = 0.000331178948f;
            bestparams.distance2 = 0.151375741f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 3184 (55/32768) [RMS: -14.58]
            bestparams.threshold = 0.949159145f;
            bestparams.pulsestrength = 0.894956648f;
            bestparams.topbit = 1.06276321f;
            bestparams.distance1 = 1.06268573f;
            bestparams.distance2 = 1.47704351f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_1088") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 10660 (353/32768) [RMS: -12.85]
            bestparams.threshold = 0.853578329f;
            bestparams.topbit = 1.09615636f;
            bestparams.distance1 = 1.8819375f;
            bestparams.distance2 = 6.80794907f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 10635 (289/32768) [RMS: -7.43]
            bestparams.threshold = 0.929835618f;
            bestparams.pulsestrength = 1.12836814f;
            bestparams.distance1 = 1.10453653f;
            bestparams.distance2 = 1.48065746f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 12255 (554/32768) [RMS: -7.97]
            bestparams.threshold = 0.911938608f;
            bestparams.pulsestrength = 1.2278074f;
            bestparams.topbit = 0.996440411f;
            bestparams.distance1 = 0.000117214302f;
            bestparams.distance2 = 0.18948476f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 6913 (127/32768) [RMS: -13.23]
            bestparams.threshold = 0.938004673f;
            bestparams.pulsestrength = 1.21178246f;
            bestparams.topbit = 1.04827631f;
            bestparams.distance1 = 0.915959001f;
            bestparams.distance2 = 1.42698038f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_1489") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 4837 (388/32768) [RMS: -10.54]
            bestparams.threshold = 0.89762634f;
            bestparams.topbit = 56.7594185f;
            bestparams.distance1 = 7.68995237f;
            bestparams.distance2 = 12.0754194f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9266 (508/32768) [RMS: -6.03]
            bestparams.threshold = 0.87147671f;
            bestparams.pulsestrength = 1.44887495f;
            bestparams.distance1 = 1.05899632f;
            bestparams.distance2 = 1.43786001f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 13168 (718/32768) [RMS: -6.34]
            bestparams.threshold = 0.89255774f;
            bestparams.pulsestrength = 1.75615835f;
            bestparams.topbit = 1.2253896f;
            bestparams.distance1 = 0.0245045591f;
            bestparams.distance2 = 0.12982437f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 6702 (300/32768) [RMS: -11.14]
            bestparams.threshold = 0.91124934f;
            bestparams.pulsestrength = 0.909965038f;
            bestparams.topbit = 0.963609755f;
            bestparams.distance1 = 1.07445884f;
            bestparams.distance2 = 1.82399702f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_1891") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 3401 (283/32768) [RMS: -11.79]
            bestparams.threshold = 0.74335587f;
            bestparams.topbit = 1.13261592f;
            bestparams.distance1 = 1.83344603f;
            bestparams.distance2 = 3.90392399f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9150 (263/32768) [RMS: -7.54]
            bestparams.threshold = 0.918804288f;
            bestparams.pulsestrength = 1.1401931f;
            bestparams.distance1 = 1.07224596f;
            bestparams.distance2 = 1.44912469f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 13940 (609/32768) [RMS: -7.89]
            bestparams.threshold = 0.901862085f;
            bestparams.pulsestrength = 1.11271441f;
            bestparams.topbit = 1.02348149f;
            bestparams.distance1 = 0.000376841635f;
            bestparams.distance2 = 0.220544845f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8422 (180/32768) [RMS: -13.40]
            bestparams.threshold = 0.982669771f;
            bestparams.pulsestrength = 0.213502586f;
            bestparams.topbit = 0.991373479f;
            bestparams.distance1 = 0.771452665f;
            bestparams.distance2 = 2.62002873f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_3190") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2593 (269/32768) [RMS: -11.61]
            bestparams.threshold = 0.742079914f;
            bestparams.topbit = 1.16795468f;
            bestparams.distance1 = 1.82698667f;
            bestparams.distance2 = 3.90259051f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7136 (302/32768) [RMS: -6.95]
            bestparams.threshold = 0.920148611f;
            bestparams.pulsestrength = 1.2706455f;
            bestparams.distance1 = 1.03514659f;
            bestparams.distance2 = 1.45814693f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 14360 (668/32768) [RMS: -7.38]
            bestparams.threshold = 0.911647439f;
            bestparams.pulsestrength = 1.19287789f;
            bestparams.topbit = 1.00216305f;
            bestparams.distance1 = 0.000113861912f;
            bestparams.distance2 = 0.257546455f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8600 (135/32768) [RMS: -12.27]
            bestparams.threshold = 0.943421066f;
            bestparams.pulsestrength = 1.19525087f;
            bestparams.topbit = 1.0747292f;
            bestparams.distance1 = 0.970244825f;
            bestparams.distance2 = 1.48792744f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_3491") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1935 (229/32768) [RMS: -12.82]
            bestparams.threshold = 0.720933437f;
            bestparams.topbit = 0.997237265f;
            bestparams.distance1 = 1.59829557f;
            bestparams.distance2 = 3.3607018f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8480 (236/32768) [RMS: -7.90]
            bestparams.threshold = 0.924642026f;
            bestparams.pulsestrength = 1.19979942f;
            bestparams.distance1 = 1.07368398f;
            bestparams.distance2 = 1.39958048f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 12414 (523/32768) [RMS: -8.28]
            bestparams.threshold = 0.922902048f;
            bestparams.pulsestrength = 1.24408174f;
            bestparams.topbit = 1.07340896f;
            bestparams.distance1 = 0.000197364454f;
            bestparams.distance2 = 0.16440165f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 3808 (88/32768) [RMS: -13.98]
            bestparams.threshold = 0.96112895f;
            bestparams.pulsestrength = 1.36136329f;
            bestparams.topbit = 1.13906264f;
            bestparams.distance1 = 0.971457958f;
            bestparams.distance2 = 1.35724473f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_3987") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2029 (283/32768) [RMS: -12.97]
            bestparams.threshold = 0.705426931f;
            bestparams.topbit = 0.92870903f;
            bestparams.distance1 = 1.47875774f;
            bestparams.distance2 = 3.15420222f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9212 (287/32768) [RMS: -8.02]
            bestparams.threshold = 0.903500497f;
            bestparams.pulsestrength = 1.02719498f;
            bestparams.distance1 = 1.06971335f;
            bestparams.distance2 = 1.4370302f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 11109 (503/32768) [RMS: -8.48]
            bestparams.threshold = 0.933880389f;
            bestparams.pulsestrength = 1.29445052f;
            bestparams.topbit = 1.06563056f;
            bestparams.distance1 = 0.000236776366f;
            bestparams.distance2 = 0.152991742f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 5112 (140/32768) [RMS: -14.02]
            bestparams.threshold = 0.866591275f;
            bestparams.pulsestrength = 0.113579206f;
            bestparams.topbit = 0.877181113f;
            bestparams.distance1 = 1.1728934f;
            bestparams.distance2 = 2.75143433f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_4388") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 2274 (288/32768) [RMS: -12.93]
            bestparams.threshold = 0.727870882f;
            bestparams.topbit = 0.981630623f;
            bestparams.distance1 = 1.62720287f;
            bestparams.distance2 = 3.45849872f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7427 (188/32768) [RMS: -9.01]
            bestparams.threshold = 0.947672546f;
            bestparams.pulsestrength = 1.29177153f;
            bestparams.distance1 = 1.0831207f;
            bestparams.distance2 = 1.32575297f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 14028 (521/32768) [RMS: -9.28]
            bestparams.threshold = 0.973695457f;
            bestparams.pulsestrength = 1.51140547f;
            bestparams.topbit = 1.06569493f;
            bestparams.distance1 = 0.0182949118f;
            bestparams.distance2 = 0.109501146f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 5198 (86/32768) [RMS: -14.96]
            bestparams.threshold = 0.992993474f;
            bestparams.pulsestrength = 1.39050341f;
            bestparams.topbit = 1.10221159f;
            bestparams.distance1 = 0.909341216f;
            bestparams.distance2 = 1.34693623f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_4589") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 12084 (360/32768) [RMS: -12.76]
            bestparams.threshold = 0.711074412f;
            bestparams.topbit = 0.947770417f;
            bestparams.distance1 = 1.55405724f;
            bestparams.distance2 = 3.37904644f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7797 (249/32768) [RMS: -7.60]
            bestparams.threshold = 0.923860133f;
            bestparams.pulsestrength = 1.2507503f;
            bestparams.distance1 = 1.05845523f;
            bestparams.distance2 = 1.40350294f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 14820 (634/32768) [RMS: -7.99]
            bestparams.threshold = 0.918689728f;
            bestparams.pulsestrength = 1.22249699f;
            bestparams.topbit = 1.04088986f;
            bestparams.distance1 = 0.010150237f;
            bestparams.distance2 = 0.190041497f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 9803 (220/32768) [RMS: -13.15]
            bestparams.threshold = 0.882457912f;
            bestparams.pulsestrength = 0.0400544927f;
            bestparams.topbit = 0.932223499f;
            bestparams.distance1 = 1.36063206f;
            bestparams.distance2 = 4.08809948f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_4790") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1920 (242/32768) [RMS: -13.04]
            bestparams.threshold = 0.725565016f;
            bestparams.topbit = 0.995874524f;
            bestparams.distance1 = 1.61511159f;
            bestparams.distance2 = 3.41737127f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8512 (236/32768) [RMS: -8.10]
            bestparams.threshold = 0.921056628f;
            bestparams.pulsestrength = 1.1018368f;
            bestparams.distance1 = 1.07269633f;
            bestparams.distance2 = 1.42056799f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 10286 (424/32768) [RMS: -8.59]
            bestparams.threshold = 0.943577528f;
            bestparams.pulsestrength = 1.23169756f;
            bestparams.topbit = 1.04314649f;
            bestparams.distance1 = 0.0142333535f;
            bestparams.distance2 = 0.175569296f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 4026 (133/32768) [RMS: -13.99]
            bestparams.threshold = 0.829947531f;
            bestparams.pulsestrength = 0.383184969f;
            bestparams.topbit = 0.859575093f;
            bestparams.distance1 = 1.12513435f;
            bestparams.distance2 = 1.78050268f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_4887") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score score 741 (76/32768) [RMS: -13.56]
            bestparams.threshold = 0.812351167f;
            bestparams.topbit = 1.1727736f;
            bestparams.distance1 = 1.87459648f;
            bestparams.distance2 = 2.31578159f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7199 (192/32768) [RMS: -9.23]
            bestparams.threshold = 0.917997837f;
            bestparams.pulsestrength = 1.01248944f;
            bestparams.distance1 = 1.05761552f;
            bestparams.distance2 = 1.37529826f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 9856 (332/32768) [RMS: -9.45]
            bestparams.threshold = 0.968754232f;
            bestparams.pulsestrength = 1.29909098f;
            bestparams.topbit = 1.00669801f;
            bestparams.distance1 = 0.00962483883f;
            bestparams.distance2 = 0.146850556f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 4809 (60/32768) [RMS: -15.03]
            bestparams.threshold = 0.941834152f;
            bestparams.pulsestrength = 0.991132736f;
            bestparams.topbit = 1.06401193f;
            bestparams.distance1 = 0.995310068f;
            bestparams.distance2 = 1.41105855f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "8580_5092_2") == 0)
    {
        is8580 = true;

        switch (wave)
        {
        case 3: // ST
            // current score 1359 (150/32768) [RMS: -13.34]
            bestparams.threshold = 0.841939628f;
            bestparams.topbit = 1.1484369f;
            bestparams.distance1 = 1.66275322f;
            bestparams.distance2 = 4.84815454f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5191 (238/32768) [RMS: -7.30]
            bestparams.threshold = 0.926346302f;
            bestparams.pulsestrength = 1.11291993f;
            bestparams.distance1 = 1.0417254f;
            bestparams.distance2 = 1.50533426f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 11562 (454/32768) [RMS: -7.91]
            bestparams.threshold = 0.926925302f;
            bestparams.pulsestrength = 0.934966981f;
            bestparams.topbit = 0.985450864f;
            bestparams.distance1 = 0.0299737118f;
            bestparams.distance2 = 0.384106278f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 6685 (62/32768) [RMS: -12.91]
            bestparams.threshold = 0.945638657f;
            bestparams.pulsestrength = 1.00875294f;
            bestparams.topbit = 1.11989963f;
            bestparams.distance1 = 1.0154649f;
            bestparams.distance2 = 1.50759816f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "broken0384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 754 (2056/32768) [RMS: -18.87]
            bestparams.threshold = 0.714277208f;
            bestparams.topbit = 0.00729158986f;
            bestparams.distance1 = 2.12244034f;
            bestparams.distance2 = 1.66707671f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5190 (238/32768) [RMS: -9.73]
            bestparams.threshold = 0.924780309f;
            bestparams.pulsestrength = 1.96809769f;
            bestparams.distance1 = 0.0888123438f;
            bestparams.distance2 = 0.234606609f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 860 (1292/32768) [RMS: -8.29]
            bestparams.threshold = 1.0222646f;
            bestparams.pulsestrength = 3.66975141f;
            bestparams.topbit = 1.36549771f;
            bestparams.distance1 = 0.00202895817f;
            bestparams.distance2 = 0.0821412951f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 85 (370/32768) [RMS: -16.34]
            bestparams.threshold = 0.942416549f;
            bestparams.pulsestrength = 1.35665071f;
            bestparams.topbit = 1.54389608f;
            bestparams.distance1 = 0.154163897f;
            bestparams.distance2 = 0.472713977f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "brokenr4ar3488") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2848 (1646/32768) [RMS: -9.30]
            bestparams.threshold = 0.00245361845f;
            bestparams.topbit = 0.399462789f;
            bestparams.distance1 = 0.00235818862f;
            bestparams.distance2 = 1.94512761f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 10938 (229/32768) [RMS: -9.21]
            bestparams.threshold = 0.971203208f;
            bestparams.pulsestrength = 1.92458713f;
            bestparams.distance1 = 0.0430820882f;
            bestparams.distance2 = 0.34782514f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 668 (2038/32768) [RMS: -9.52]
            bestparams.threshold = 1.30860806f;
            bestparams.pulsestrength = 4.26479483f;
            bestparams.topbit = 0.763755977f;
            bestparams.distance1 = 0.184508532f;
            bestparams.distance2 = 0.209104061f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 12 (351/32768) [RMS: -15.57]
            bestparams.threshold = 1.17640698f;
            bestparams.pulsestrength = 1.56606185f;
            bestparams.topbit = 1.75867808f;
            bestparams.distance1 = 0.602180004f;
            bestparams.distance2 = 0.819929481f;
            bestparams.distFunc = Parameters::linearDistance;
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
#if 0
            // print the rate of wrong bits
            std::cout << score.wrongBitsRate() << std::endl;
#endif
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
