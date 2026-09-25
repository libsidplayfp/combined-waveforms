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

#define CHANGE_DEV

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

static double stddev = 0.0001;

static std::default_random_engine prng(getSeed());

static std::normal_distribution<> normal_dist(1.0, stddev);
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
            // current score 169 (843/32768) [RMS: -14.61]
            bestparams.threshold = 0.946056068f;
            bestparams.topbit = 26.9527836f;
            bestparams.distance1 = 6.38644743f;
            bestparams.distance2 = 3.61852479f;
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
            // current score 5 (1535/32768) [RMS: -15.71]
            bestparams.threshold = 0.760607481f;
            bestparams.pulsestrength = 0.407531887f;
            bestparams.topbit = 0.0588437207f;
            bestparams.distance1 = 0.0994859114f;
            bestparams.distance2 = 0.000200334835f;
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
            // current score 483 (448/32768) [RMS: -13.17]
            bestparams.threshold = 0.890794635f;
            bestparams.topbit = 5.24449015f;
            bestparams.distance1 = 4.62662935f;
            bestparams.distance2 = 4.23713732f;
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
            // current score 144 (1078/32768) [RMS: -10.63]
            bestparams.threshold = 1.19699228f;
            bestparams.pulsestrength = 4.41826534f;
            bestparams.topbit = 1.38852525f;
            bestparams.distance1 = 0.0604045726f;
            bestparams.distance2 = 0.0815515071f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 3 (250/32768) [RMS: -18.45]
            bestparams.threshold = 1.69891477f;
            bestparams.pulsestrength = 1.99595749f;
            bestparams.topbit = 0.00578480121f;
            bestparams.distance1 = 1.57269251f;
            bestparams.distance2 = 1.37823129f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "cbm3384") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 242 (964/32768) [RMS: -12.63]
            bestparams.threshold = 0.841265261f;
            bestparams.topbit = 10.0567818f;
            bestparams.distance1 = 3.29666662f;
            bestparams.distance2 = 2.94648147f;
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
            // current score 361 (1125/32768) [RMS: -9.14]
            bestparams.threshold = 1.23285437f;
            bestparams.pulsestrength = 4.07586765f;
            bestparams.topbit = 1.58522534f;
            bestparams.distance1 = 0.137179226f;
            bestparams.distance2 = 0.155834228f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 124 (277/32768) [RMS: -18.28]
            bestparams.threshold = 1.06343484f;
            bestparams.pulsestrength = 1.6654166f;
            bestparams.topbit = 1.73890376f;
            bestparams.distance1 = 0.341213256f;
            bestparams.distance2 = 0.438477188f;
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
            // current score 337 (1019/32768) [RMS: -10.22]
            bestparams.threshold = 1.09263694f;
            bestparams.pulsestrength = 3.46004915f;
            bestparams.topbit = 1.47997165f;
            bestparams.distance1 = 0.0714207962f;
            bestparams.distance2 = 0.100400507f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 56 (230/32768) [RMS: -20.32]
            bestparams.threshold = 1.06856573f;
            bestparams.pulsestrength = 2.14276409f;
            bestparams.topbit = 1.85001004f;
            bestparams.distance1 = 0.190060407f;
            bestparams.distance2 = 0.248597205f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "6581R4AR_3789_14") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 209 (1115/32768) [RMS: -17.60]
            bestparams.threshold = 0.931225896f;
            bestparams.topbit = 0.923819304f;
            bestparams.distance1 = 5.42964125f;
            bestparams.distance2 = 3.36991525f;
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
            // current score 113 (1064/32768) [RMS: -10.37]
            bestparams.threshold = 1.10936975f;
            bestparams.pulsestrength = 3.50789928f;
            bestparams.topbit = 1.47000027f;
            bestparams.distance1 = 0.088235341f;
            bestparams.distance2 = 0.104341723f;
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
            // current score 3532 (65/32768) [RMS: -11.18]
            bestparams.threshold = 0.9854213f;
            bestparams.pulsestrength = 2.42496276f;
            bestparams.distance1 = 0.0374390036f;
            bestparams.distance2 = 0.165902242f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 475 (1033/32768) [RMS: -9.03]
            bestparams.threshold = 1.09942484f;
            bestparams.pulsestrength = 3.33780169f;
            bestparams.topbit = 1.39295673f;
            bestparams.distance1 = 0.0854635611f;
            bestparams.distance2 = 0.150579691f;
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
            // current score 242 (649/32768) [RMS: -13.12]
            bestparams.threshold = 0.854352772f;
            bestparams.topbit = 5.2726512f;
            bestparams.distance1 = 3.57101917f;
            bestparams.distance2 = 2.95887995f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 5580 (147/32768) [RMS: -10.06]
            bestparams.threshold = 0.96532774f;
            bestparams.pulsestrength = 1.97139263f;
            bestparams.distance1 = 1.03410971f;
            bestparams.distance2 = 1.17577839f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 453 (1409/32768) [RMS: -8.74]
            bestparams.threshold = 1.10632443f;
            bestparams.pulsestrength = 3.13599825f;
            bestparams.topbit = 1.72166479f;
            bestparams.distance1 = 0.135031074f;
            bestparams.distance2 = 0.181071475f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 76 (521/32768) [RMS: -20.32]
            bestparams.threshold = 1.00029778f;
            bestparams.pulsestrength = 1.38226914f;
            bestparams.topbit = 0.815651596f;
            bestparams.distance1 = 0.288819402f;
            bestparams.distance2 = 0.480299562f;
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
            // current score 211 (1032/32768) [RMS: -10.32]
            bestparams.threshold = 1.09103811f;
            bestparams.pulsestrength = 3.41786051f;
            bestparams.topbit = 1.42359722f;
            bestparams.distance1 = 0.0797557309f;
            bestparams.distance2 = 0.102569386f;
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
            // current score 172 (834/32768) [RMS: -10.08]
            bestparams.threshold = 1.02504992f;
            bestparams.pulsestrength = 2.92979741f;
            bestparams.topbit = 1.43259883f;
            bestparams.distance1 = 0.0623142533f;
            bestparams.distance2 = 0.108534746f;
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
            // current score 255 (452/32768) [RMS: -13.56]
            bestparams.threshold = 0.806933343f;
            bestparams.topbit = 2.21477556f;
            bestparams.distance1 = 2.81543303f;
            bestparams.distance2 = 2.35701466f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 3516 (70/32768) [RMS: -11.95]
            bestparams.threshold = 0.962648809f;
            bestparams.pulsestrength = 2.06163311f;
            bestparams.distance1 = 0.0289246459f;
            bestparams.distance2 = 0.183156878f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 6: // PS
            // current score 204 (936/32768) [RMS: -10.75]
            bestparams.threshold = 0.852842271f;
            bestparams.pulsestrength = 1.68666494f;
            bestparams.topbit = 1.18602562f;
            bestparams.distance1 = 0.00126632315f;
            bestparams.distance2 = 0.0858733431f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 57 (179/32768) [RMS: -20.13]
            bestparams.threshold = 0.973592043f;
            bestparams.pulsestrength = 1.39669251f;
            bestparams.topbit = 1.48668134f;
            bestparams.distance1 = 0.229719117f;
            bestparams.distance2 = 0.351197541f;
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
            // current score 78 (927/32768) [RMS: -20.08]
            bestparams.threshold = 0.95052433f;
            bestparams.topbit = 0.176136687f;
            bestparams.distance1 = 6.97077751f;
            bestparams.distance2 = 3.79802465f;
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
            // current score 923 (135/32768) [RMS: -13.49]
            bestparams.threshold = 0.762481153f;
            bestparams.topbit = 1.05701602f;
            bestparams.distance1 = 1.20563996f;
            bestparams.distance2 = 2.19636106f;
            bestparams.distFunc = Parameters::exponentialDistance;
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
            // current score 9235 (403/32768) [RMS: -6.95]
            bestparams.threshold = 0.982675791f;
            bestparams.pulsestrength = 1.19733763f;
            bestparams.topbit = 0.984847903f;
            bestparams.distance1 = 0.0200310666f;
            bestparams.distance2 = 0.455898285f;
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
            // current score 10795 (435/32768) [RMS: -7.53]
            bestparams.threshold = 0.946380913f;
            bestparams.pulsestrength = 1.2029047f;
            bestparams.topbit = 0.997707188f;
            bestparams.distance1 = 0.00201191567f;
            bestparams.distance2 = 0.290461034f;
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
            // current score 844 (136/32768) [RMS: -13.45]
            bestparams.threshold = 0.724700212f;
            bestparams.topbit = 1.11193025f;
            bestparams.distance1 = 1.3567102f;
            bestparams.distance2 = 2.25340843f;
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
            // current score 9434 (347/32768) [RMS: -7.68]
            bestparams.threshold = 0.967761934f;
            bestparams.pulsestrength = 1.20811367f;
            bestparams.topbit = 0.968534827f;
            bestparams.distance1 = 0.0075986716f;
            bestparams.distance2 = 0.313736677f;
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
            // current score 2134 (184/32768) [RMS: -13.31]
            bestparams.threshold = 0.702459991f;
            bestparams.topbit = 0.975484133f;
            bestparams.distance1 = 1.25884247f;
            bestparams.distance2 = 2.25034642f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 8468 (209/32768) [RMS: -7.47]
            bestparams.threshold = 0.955855966f;
            bestparams.pulsestrength = 1.13067317f;
            bestparams.distance1 = 1.09514272f;
            bestparams.distance2 = 1.51367176f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 10781 (438/32768) [RMS: -7.84]
            bestparams.threshold = 0.926709116f;
            bestparams.pulsestrength = 1.09496796f;
            bestparams.topbit = 0.97637248f;
            bestparams.distance1 = 0.000130610671f;
            bestparams.distance2 = 0.283982545f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 7245 (116/32768) [RMS: -13.46]
            bestparams.threshold = 0.899005055f;
            bestparams.pulsestrength = 0.601432741f;
            bestparams.topbit = 1.01393855f;
            bestparams.distance1 = 1.12622178f;
            bestparams.distance2 = 1.67641866f;
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
            // current score 1473 (168/32768) [RMS: -13.47]
            bestparams.threshold = 0.710448802f;
            bestparams.topbit = 1.11656249f;
            bestparams.distance1 = 1.30863619f;
            bestparams.distance2 = 2.02344704f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 7890 (168/32768) [RMS: -8.62]
            bestparams.threshold = 0.946508229f;
            bestparams.pulsestrength = 1.21882367f;
            bestparams.distance1 = 1.0407542f;
            bestparams.distance2 = 1.37056279f;
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
            // current score 9242 (504/32768) [RMS: -6.03]
            bestparams.threshold = 0.871706188f;
            bestparams.pulsestrength = 1.44852948f;
            bestparams.distance1 = 1.05926013f;
            bestparams.distance2 = 1.43830109f;
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
            // current score 3395 (280/32768) [RMS: -11.81]
            bestparams.threshold = 0.740850687f;
            bestparams.topbit = 1.1147213f;
            bestparams.distance1 = 1.80291951f;
            bestparams.distance2 = 3.85912538f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 5: // PT
            // current score 9034 (259/32768) [RMS: -7.54]
            bestparams.threshold = 0.917611182f;
            bestparams.pulsestrength = 1.13679028f;
            bestparams.distance1 = 1.07353628f;
            bestparams.distance2 = 1.44951916f;
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
            // current score 14328 (667/32768) [RMS: -7.38]
            bestparams.threshold = 0.911578476f;
            bestparams.pulsestrength = 1.19318783f;
            bestparams.topbit = 1.00221062f;
            bestparams.distance1 = 0.000113086062f;
            bestparams.distance2 = 0.257361323f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8599 (134/32768) [RMS: -12.27]
            bestparams.threshold = 0.942918181f;
            bestparams.pulsestrength = 1.19498122f;
            bestparams.topbit = 1.0760572f;
            bestparams.distance1 = 0.970883131f;
            bestparams.distance2 = 1.48736346f;
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
            // current score 14787 (632/32768) [RMS: -7.99]
            bestparams.threshold = 0.918511629f;
            bestparams.pulsestrength = 1.22237325f;
            bestparams.topbit = 1.04048359f;
            bestparams.distance1 = 0.0102269938f;
            bestparams.distance2 = 0.189951181f;
            bestparams.distFunc = Parameters::quadraticDistance;
            break;
        case 7: // PST
            // current score 8827 (207/32768) [RMS: -13.52]
            bestparams.threshold = 0.887825787f;
            bestparams.pulsestrength = 0.042996794f;
            bestparams.topbit = 0.889274299f;
            bestparams.distance1 = 1.31400216f;
            bestparams.distance2 = 4.05096912f;
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
            // current score 8496 (234/32768) [RMS: -8.10]
            bestparams.threshold = 0.921019256f;
            bestparams.pulsestrength = 1.10160947f;
            bestparams.distance1 = 1.07253897f;
            bestparams.distance2 = 1.42064273f;
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
            // current score 9849 (333/32768) [RMS: -9.45]
            bestparams.threshold = 0.969898582f;
            bestparams.pulsestrength = 1.30233467f;
            bestparams.topbit = 1.00785899f;
            bestparams.distance1 = 0.00962228701f;
            bestparams.distance2 = 0.146903187f;
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
            // current score 5167 (234/32768) [RMS: -7.30]
            bestparams.threshold = 0.925685465f;
            bestparams.pulsestrength = 1.11378157f;
            bestparams.distance1 = 1.0397594f;
            bestparams.distance2 = 1.50381863f;
            bestparams.distFunc = Parameters::exponentialDistance;
            break;
        case 6: // PS
            // current score 11560 (453/32768) [RMS: -7.91]
            bestparams.threshold = 0.926167309f;
            bestparams.pulsestrength = 0.932631016f;
            bestparams.topbit = 0.984126568f;
            bestparams.distance1 = 0.0298364628f;
            bestparams.distance2 = 0.384995401f;
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
            // current score 860 (1288/32768) [RMS: -8.28]
            bestparams.threshold = 1.02248156f;
            bestparams.pulsestrength = 3.66920304f;
            bestparams.topbit = 1.36571658f;
            bestparams.distance1 = 0.00203792308f;
            bestparams.distance2 = 0.0826661736f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        case 7: // PST
            // current score 60 (412/32768) [RMS: -18.00]
            bestparams.threshold = 0.886677682f;
            bestparams.pulsestrength = 1.30265021f;
            bestparams.topbit = 1.82846642f;
            bestparams.distance1 = 0.131817579f;
            bestparams.distance2 = 0.299035341f;
            bestparams.distFunc = Parameters::linearDistance;
            break;
        }
    }

    else if (std::strcmp(chip, "brokenr4ar3488") == 0)
    {
        switch (wave)
        {
        case 3: // ST
            // current score 2720 (1639/32768) [RMS: -9.32]
            bestparams.threshold = 0.00322450767f;
            bestparams.topbit = 0.742047548f;
            bestparams.distance1 = 0.00322453398f;
            bestparams.distance2 = 1.98249948f;
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
            // current score 668 (2037/32768) [RMS: -9.52]
            bestparams.threshold = 1.30886745f;
            bestparams.pulsestrength = 4.26684999f;
            bestparams.topbit = 0.764546275f;
            bestparams.distance1 = 0.184561312f;
            bestparams.distance2 = 0.208998889f;
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
    int loops = 0;
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

#ifdef CHANGE_DEV
            loops = 0;
            stddev = 0.0001;
            std::normal_distribution<>::param_type param(1.0, stddev);
            continue;
#endif
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

#ifdef CHANGE_DEV
        loops++;
        if (loops > 10000)
        {
            stddev *= 2;
            if (stddev > 0.5)
                exit(EXIT_FAILURE);
            std::normal_distribution<>::param_type param(1.0, stddev);
            normal_dist.param(param);
            loops = 0;
        }
#endif
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
