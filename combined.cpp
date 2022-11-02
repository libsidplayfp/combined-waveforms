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

static void Optimize(const ref_vector_t &reference, int wave, char chip)
{
    Parameters bestparams;

    /*
     * The score here reported is the acoustic error.
     * In parentheses the number of mispredicted bits
     * on a total of 32768 (28672 for combinations
     * including saw on 6581).
     */
    switch (chip)
    {
    // 6581 R2
    case 'B':
        switch (wave)
        {
        case 3: // ST
            // current score 152 (57)
            bestparams.threshold = 0.988547385f;

            bestparams.distance1 = 5.58014059f;
            bestparams.distance2 = 5.4269886f;

            break;
        case 5: // PT
            // current score 2872 (154)
            bestparams.threshold = 0.984531879f;
            bestparams.pulsestrength = 3.03670526f;
            bestparams.distance1 = 0.99342072f;
            bestparams.distance2 = 1.11435139f;
            break;
        case 6: // PS
            // current score 0
            bestparams.threshold = 0.906437993f;
            bestparams.pulsestrength = 2.11317873f;

            bestparams.distance1 = 0.130663797f;
            bestparams.distance2 = 0.0835102722f;
            break;
        case 7: // PST
            // current score 0
            bestparams.threshold = 0.924151242f;
            bestparams.pulsestrength = 1.16428149f;

            bestparams.distance1 = 0.920196056f;
            bestparams.distance2 = 1.07491302f;

            break;
        }
        break;
    // 6581 R2 (odd)
    case 'C':
        switch (wave)
        {
        case 3:
            // current score 2890 (446)
            bestparams.threshold = 0.879240036f;

            bestparams.distance1 = 1.90632415f;
            bestparams.distance2 = 1.95846224f;

            break;
        case 5:
            // current score 10966 (365)
            bestparams.threshold = 0.93361032f;
            bestparams.pulsestrength = 3.0441184f;
            bestparams.distance1 = 1.01603663f;
            bestparams.distance2 = 1.19291425f;
            break;
        case 6:
            // current score 5528 (297)
            bestparams.threshold = 0.906388283f;
            bestparams.pulsestrength = 3.24655199f;

            bestparams.distance1 = 0.101219185f;
            bestparams.distance2 = 0.18899554f;
            break;
        case 7:
            // current score 842 (58)
            bestparams.threshold = 0.919801712f;
            bestparams.pulsestrength = 1.64734352f;

            bestparams.distance1 = 1.21909821f;
            bestparams.distance2 = 1.22679806f;

            break;
        }
        break;
    // 6581 R2 (odd)
    case 'D':
        switch (wave)
        {
        case 3:
            // current score 1958 (302)
            bestparams.threshold = 0.861116648f;

            bestparams.distance1 = 1.582165f;
            bestparams.distance2 = 2.23844433f;

            break;
        case 5:
            // current score 9922 (424)
            bestparams.threshold = 0.927246153f;
            bestparams.pulsestrength = 2.37208962f;
            bestparams.distance1 = 1.00441122f;
            bestparams.distance2 = 1.20633698f;
            break;
        case 6:
            // current score 12331 (856)
            bestparams.threshold = 0.883922815f;
            bestparams.pulsestrength = 1.86374128f;

            bestparams.distance1 = 0.0953552499f;
            bestparams.distance2 = 0.456922501f;
            break;
        case 7:
            // current score 258 (64)
            bestparams.threshold = 0.91076839f;
            bestparams.pulsestrength = 1.2814858f;

            bestparams.distance1 = 1.17647922f;
            bestparams.distance2 = 1.18998444f;

            break;
        }
        break;
    // 6581 R2
    case 'E':
        switch (wave)
        {
        case 3:
            // current score 297 (98)
            bestparams.threshold = 0.989183f;

            bestparams.distance1 = 5.75852f;

            break;
        case 5:
            // current score 3348 (146)
            bestparams.threshold = 0.91292721f;
            bestparams.pulsestrength = 1.83235359f;
            bestparams.distance1 = 1.12143898f;
            bestparams.distance2 = 1.12768865f;
            break;
        case 6:
            // current score 8 (6)
            bestparams.threshold = 0.91496712f;
            bestparams.pulsestrength = 2.28155446f;

            bestparams.distance1 = 0.122848086f;
            bestparams.distance2 = 0.0706237406f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.970328987f;
            bestparams.pulsestrength = 1.75902855f;

            bestparams.distance1 = 1.21010804f;
            bestparams.distance2 = 1.269436f;

            break;
        }
        break;
    // 6581 R2
    case 'F':
        switch (wave)
        {
        case 3:
            // current score 240 (64)
            bestparams.threshold = 0.99254179f;

            bestparams.distance1 = 7.18324232f;
            bestparams.distance2 = 6.00581455f;

            break;
        case 5:
            // current score 3008 (57)
            bestparams.threshold = 0.959230483f;
            bestparams.pulsestrength = 2.74101543f;
            bestparams.distance1 = 1.00775206f;
            bestparams.distance2 = 1.11247838f;
            break;
        case 6:
            // current score 416 (10)
            bestparams.threshold = 0.894908309f;
            bestparams.pulsestrength = 1.83701169f;

            bestparams.distance1 = 0.117017552f;
            bestparams.distance2 = 0.0981598422f;
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.89575851f;
            bestparams.pulsestrength = 0.865568995f;

            bestparams.distance1 = 0.986006081f;
            bestparams.distance2 = 1.17148495f;

            break;
        }
        break;
    // 6581 R2 (odd)
    case 'G':
        switch (wave)
        {
        case 3:
            // current score 1687 (278)
            bestparams.threshold = 0.90522f;
            bestparams.distance1 = 1.97506f;
            bestparams.distance2 = 1.66937f;
// current score 1941 (327/28672)
bestparams.threshold = 0.862147212f;
bestparams.distance1 = 10.8962431f;
bestparams.distance2 = 2.50848103f;
            break;
        case 5:
            // current score 6128 (130)
            bestparams.threshold = 0.93088f;
            bestparams.pulsestrength = 2.4843f;
            bestparams.distance1 = 1.0353f;
            bestparams.distance2 = 1.1484f;
// current score 6372 (146/32768)
bestparams.threshold = 0.913688719f;
bestparams.pulsestrength = 2.91472721f;
bestparams.distance1 = 1.00917649f;
bestparams.distance2 = 1.14808357f;
            break;
        case 6:
            // current score 10567 (567)
            bestparams.threshold = 0.912142f;
            bestparams.pulsestrength = 2.32076f;
            bestparams.distance1 = 0.053906f;
            bestparams.distance2 = 0.25143f;
// current score 5630 (1073/28672)
bestparams.threshold = 0.498705059f;
bestparams.pulsestrength = 0.000455357862f;
bestparams.distance1 = 0.843253314f;
bestparams.distance2 = 1.058056f;
            break;
        case 7:
            // current score 36 (12)
            bestparams.threshold = 0.901f;
            bestparams.pulsestrength = 1.0845f;
            bestparams.distance1 = 1.056f;
            bestparams.distance2 = 1.1848f;
// current score 306 (35/28672)
bestparams.threshold = 0.858491957f;
bestparams.pulsestrength = 4.21265459f;
bestparams.distance1 = 1.61194706f;
bestparams.distance2 = 1.12226105f;
            break;
        }
        break;
    // 6581 R2
    case 'H':
        switch (wave)
        {
        case 3:
            // current score 230 (78)
            bestparams.threshold = 0.97577709f;

            bestparams.distance1 = 3.35368633f;
            bestparams.distance2 = 2.60800719f;

            break;
        case 5:
            // current score 2744 (91)
            bestparams.threshold = 0.919414461f;
            bestparams.pulsestrength = 1.49266505f;
            bestparams.distance1 = 1.12526083f;
            bestparams.distance2 = 1.15659571f;
            break;
        case 6:
            // current score 360 (30)
            bestparams.threshold = 0.941394627f;
            bestparams.pulsestrength = 2.23991108f;

            bestparams.distance1 = 0.149554357f;
            bestparams.distance2 = 0.150783867f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.98f;
            bestparams.pulsestrength = 2.f;

            bestparams.distance1 = 1.1f;

            break;
        }
        break;
    // 6581 R2
    case 'I':
        switch (wave)
        {
        case 3:
            // current score 184 (68)
            bestparams.threshold = 0.982663572f;

            bestparams.distance1 = 4.43f;
            bestparams.distance2 = 4.433f;

            break;
        case 5:
            // current score 6847 (171)
            bestparams.threshold = 0.943752468f;
            bestparams.pulsestrength = 2.28453493f;
            bestparams.distance1 = 1.03244841f;
            bestparams.distance2 = 1.17106056f;
            break;
        case 6:
            // current score 422 (17)
            bestparams.threshold = 0.894735754f;
            bestparams.pulsestrength = 1.81871581f;

            bestparams.distance1 = 0.0912446976f;
            bestparams.distance2 = 0.101162158f;
            break;
        case 7:
            // current score 12 (4)
            bestparams.threshold = 0.920725882f;
            bestparams.pulsestrength = 1.27905869f;

            bestparams.distance1 = 1.03366196f;
            bestparams.distance2 = 1.19196999f;

            break;
        }
        break;
    // 6581 R2
    case 'J':
        switch (wave)
        {
        case 3:
            // current score 148 (61)
            bestparams.threshold = 0.979544f;

            bestparams.distance1 = 3.98271f;

            break;
        case 5:
            // current score 1540 (102)
            bestparams.threshold = 0.9079f;
            bestparams.pulsestrength = 1.72749f;
            bestparams.distance1 = 1.12017f;
            bestparams.distance2 = 1.10793f;
            break;
        case 6:
            // current score 0
            bestparams.threshold = 0.905734479f;
            bestparams.pulsestrength = 1.99118233f;

            bestparams.distance1 = 0.0996442288f;
            bestparams.distance2 = 0.0730706826f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.95248f;
            bestparams.pulsestrength = 1.51f;

            bestparams.distance1 = 1.07153f;
            bestparams.distance2 = 1.09353f;

            break;
        }
        break;
    // 6581 R2
    case 'K':
        switch (wave)
        {
        case 3:
            // current score 1299 (150)
            bestparams.threshold = 0.931232035f;

            bestparams.distance1 = 1.78504324f;
            bestparams.distance2 = 2.21891737f;

            break;
        case 5:
            // current score 8086 (386)
            bestparams.threshold = 1.f;
            bestparams.pulsestrength = 3.06605577f;
            bestparams.distance1 = 0.981742382f;
            bestparams.distance2 = 1.1532563f;
            break;
        case 6:
            // current score 2608 (140)
            bestparams.threshold = 0.898440778f;
            bestparams.pulsestrength = 1.99839222f;

            bestparams.distance1 = 0.108322836f;
            bestparams.distance2 = 0.096527569f;
            break;
        case 7:
            // current score 102 (10)
            bestparams.threshold = 0.931187391f;
            bestparams.pulsestrength = 1.41706371f;

            bestparams.distance1 = 1.14805245f;
            bestparams.distance2 = 1.35853684f;

            break;
        }
        break;
    // 6581 R2
    case 'L':
        switch (wave)
        {
        case 3:
            // current score 1320 (97)
            bestparams.threshold = 0.991488576f;

            bestparams.distance1 = 6.7075367f;
            bestparams.distance2 = 4.29520941f;

            break;
        case 5:
            // current score 6098 (134)
            bestparams.threshold = 0.924762011f;
            bestparams.pulsestrength = 2.37176347f;
            bestparams.distance1 = 1.04673755f;
            bestparams.distance2 = 1.16099727f;
            break;
        case 6:
            // current score 1362 (127)
            bestparams.threshold = 0.924177945f;
            bestparams.pulsestrength = 2.76564121f;

            bestparams.distance1 = 0.0548680872f;
            bestparams.distance2 = 0.137630284f;
            break;
        case 7:
            // current score 84 (12)
            bestparams.threshold = 0.933185399f;
            bestparams.pulsestrength = 1.5136503f;

            bestparams.distance1 = 1.14671242f;
            bestparams.distance2 = 1.22675645f;

            break;
        }
        break;
    // 6581 R2
    case 'M':
        switch (wave)
        {
        case 3:
            // current score 348 (104)
            bestparams.threshold = 0.9364627f;

            bestparams.distance1 = 2.03176761f;
            bestparams.distance2 = 2.10565162f;

            break;
        case 5:
            // current score 2434 (139)
            bestparams.threshold = 0.906191885f;
            bestparams.pulsestrength = 1.59146726f;
            bestparams.distance1 = 1.127689f;
            bestparams.distance2 = 1.1314106f;
            break;
        case 6:
            // current score 0
            bestparams.threshold = 0.893231869f;
            bestparams.pulsestrength = 1.70082629f;

            bestparams.distance1 = 0.111504503f;
            bestparams.distance2 = 0.0748674423f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.979779f;
            bestparams.pulsestrength = 2.03635f;

            bestparams.distance1 = 1.30189f;

            break;
        }
        break;
    // 6581 R2
    case 'N':
        switch (wave)
        {
        case 3:
            // current score 610 (90)
            bestparams.threshold = 0.992348015f;

            bestparams.distance1 = 7.33235741f;
            bestparams.distance2 = 3.72023797f;

            break;
        case 5:
            // current score 4656 (74)
            bestparams.threshold = 0.945089161f;
            bestparams.pulsestrength = 2.48777676f;
            bestparams.distance1 = 1.02335358f;
            bestparams.distance2 = 1.14071643f;
            break;
        case 6:
            // current score 584 (18)
            bestparams.threshold = 0.908909619f;
            bestparams.pulsestrength = 2.23209023f;

            bestparams.distance1 = 0.12028072f;
            bestparams.distance2 = 0.119499192f;
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.911848485f;
            bestparams.pulsestrength = 1.17097521f;

            bestparams.distance1 = 0.991214871f;
            bestparams.distance2 = 1.10500252f;

            break;
        }
        break;
    // 6581 R3
    case 'O':
        switch (wave)
        {
        case 3:
            // current score 1000 (92)
            bestparams.threshold = 0.991642594f;

            bestparams.distance1 = 6.90697622f;
            bestparams.distance2 = 3.85472918f;

            break;
        case 5:
            // current score 6020 (101)
            bestparams.threshold = 0.929734766f;
            bestparams.pulsestrength = 2.31088519f;
            bestparams.distance1 = 1.03067636f;
            bestparams.distance2 = 1.15108597f;
            break;
        case 6:
            // current score 944 (60)
            bestparams.threshold = 0.916872084f;
            bestparams.pulsestrength = 2.47265148f;

            bestparams.distance1 = 0.137618273f;
            bestparams.distance2 = 0.179932177f;
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.901689231f;
            bestparams.pulsestrength = 1.06335056f;

            bestparams.distance1 = 1.04226708f;
            bestparams.distance2 = 1.29957008f;

            break;
        }
        break;
    // 6581 R3
    case 'P':
        switch (wave)
        {
        case 3:
            // current score 1750 (111)
            bestparams.threshold = 0.990397394f;

            bestparams.distance1 = 6.70575047f;
            bestparams.distance2 = 6.62938929f;

            break;
        case 5:
            // current score 3724 (107)
            bestparams.threshold = 0.917284548f;
            bestparams.pulsestrength = 2.60221553f;
            bestparams.distance1 = 1.03195667f;
            bestparams.distance2 = 1.08213603f;
            break;
        case 6:
            // current score 267 (73)
            bestparams.threshold = 0.944150627f;
            bestparams.pulsestrength = 4.02703094f;

            bestparams.distance1 = 0.0538220257f;
            bestparams.distance2 = 0.0294455308f;
            break;
        case 7:
            // current score 181 (17)
            bestparams.threshold = 0.919028938f;
            bestparams.pulsestrength = 1.21112931f;

            bestparams.distance1 = 1.14953518f;
            bestparams.distance2 = 1.21946204f;

            break;
        }
        break;
    // 6581 R3
    case 'Q':
        switch (wave)
        {
        case 3:
            // current score 488 (90)
            bestparams.threshold = 0.982932f;

            bestparams.distance1 = 4.59079f;

            break;
        case 5:
            // current score 3740 (63)
            bestparams.threshold = 1.f;
            bestparams.pulsestrength = 3.62465143f;
            bestparams.distance1 = 0.986276627f;
            bestparams.distance2 = 1.09922075f;
            break;
        case 6:
            // current score 680 (48)
            bestparams.threshold = 0.916414738f;
            bestparams.pulsestrength = 2.49077201f;

            bestparams.distance1 = 0.106750675f;
            bestparams.distance2 = 0.147990197f;
            break;
        case 7:
            // current score 38 (14)
            bestparams.threshold = 0.902495265f;
            bestparams.pulsestrength = 0.937372625f;

            bestparams.distance1 = 1.06008101;
            bestparams.distance2 = 1.10936856f;

            // current score 64 (3)
            //bestparams.threshold = 0.898101449f;
            //bestparams.pulsestrength = 0.797706962f;

            //bestparams.distance1 = 0.989617884f;
            //bestparams.distance2 = 1.0587045f;

            break;
        }
        break;
    // 6581 R4AR
    case 'R':
        switch (wave)
        {
        case 3:
            // current score 2070 (341)
            bestparams.threshold = 0.888629317f;

            bestparams.distance1 = 1.94586849f;
            bestparams.distance2 = 2.01927376f;

            break;
        case 5:
            // current score 7211 (272)
            bestparams.threshold = 0.928046405f;
            bestparams.pulsestrength = 2.5883441f;
            bestparams.distance1 = 1.01187634f;
            bestparams.distance2 = 1.15885961f;
            break;
        case 6:
            // current score 23276 (420)
            bestparams.threshold = 0.872620344f;
            bestparams.pulsestrength = 2.25908351f;

            bestparams.distance1 = 0.11984051f;
            bestparams.distance2 = 0.0987310335f;
            break;
        case 7:
            // current score 274 (30)
            bestparams.threshold = 0.91457653f;
            bestparams.pulsestrength = 1.32809377f;

            bestparams.distance1 = 1.11845613f;
            bestparams.distance2 = 1.16926301f;

            break;
        }
        break;
    // 6581 R4AR
    case 'S':
        switch (wave)
        {
        case 3:
            // current score 1664 (110)
            bestparams.threshold = 0.984836519f;

            bestparams.distance1 = 4.88818884f;
            bestparams.distance2 = 3.9673593f;

            break;
        case 5:
            // current score 6264 (226)
            bestparams.threshold = 0.900257707f;
            bestparams.pulsestrength = 1.89190149f;
            bestparams.distance1 = 1.05784476f;
            bestparams.distance2 = 1.1705128f;
            break;
        case 6:
            // current score 1800 (121)
            bestparams.threshold = 0.922910988f;
            bestparams.pulsestrength = 2.7897532f;

            bestparams.distance1 = 0.0856195092f;
            bestparams.distance2 = 0.147621214f;
            break;
        case 7:
            // current score 148 (14)
            bestparams.threshold = 0.933337092f;
            bestparams.pulsestrength = 1.51991868f;

            bestparams.distance1 = 1.15172291f;
            bestparams.distance2 = 1.23372996f;

            break;
        }
        break;
    // 6581 R4AR
    case 'T':
        switch (wave)
        {
        case 3:
            // current score 290 (101)
            bestparams.threshold = 0.965168953f;

            bestparams.distance1 = 3.04781175f;
            bestparams.distance2 = 3.81387138f;

            break;
        case 5:
            // current score 6138 (221)
            bestparams.threshold = 0.991526306f;
            bestparams.pulsestrength = 2.80080104f;
            bestparams.distance1 = 0.993945718f;
            bestparams.distance2 = 1.19684732f;
            break;
        case 6:
            // current score 610 (15)
            bestparams.threshold = 0.9087286f;
            bestparams.pulsestrength = 2.26664352f;

            bestparams.distance1 = 0.158760354f;
            bestparams.distance2 = 0.108530171f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.949945092f;
            bestparams.pulsestrength = 1.60713959f;

            bestparams.distance1 = 1.01901114f;
            bestparams.distance2 = 1.03737819f;

            break;
        }
        break;
    // 6581 R4AR
    case 'U':
        switch (wave)
        {
        case 3:
            // current score 282 (90)
            bestparams.threshold = 0.984771669f;

            bestparams.distance1 = 4.88738728f;
            bestparams.distance2 = 4.53328466f;

            break;
        case 5:
            // current score 6398 (287)
            bestparams.threshold = 0.996706426f;
            bestparams.pulsestrength = 3.12744141f;
            bestparams.distance1 = 0.999570251f;
            bestparams.distance2 = 1.16731513f;
            break;
        case 6:
            // current score 754 (38)
            bestparams.threshold = 0.908193409f;
            bestparams.pulsestrength = 2.19632339f;

            bestparams.distance1 = 0.0940685794f;
            bestparams.distance2 = 0.119425967f;
            break;
        case 7:
            // current score 6 (4)
            bestparams.threshold = 0.925804496f;
            bestparams.pulsestrength = 1.36537039f;

            bestparams.distance1 = 1.1688062f;
            bestparams.distance2 = 1.32638979f;

            break;
        }
        break;
    // 8580
    case 'V':
        switch (wave)
        {
        case 3:
            // current score 1300 (184)
            bestparams.threshold = 0.94344f;
            bestparams.distance1 = 1.6347f;
            bestparams.distance2 = 2.51537f;
// current score 928 (135/32768)
bestparams.threshold = 0.715788841f;
bestparams.distance1 = 1.32999945f;
bestparams.distance2 = 2.2172699f;

            break;
        case 5:
            // current score 7981 (204)
            bestparams.threshold = 0.93303f;
            bestparams.pulsestrength = 1.7025f;
            bestparams.distance1 = 1.0868f;
            bestparams.distance2 =  1.43527f;
// current score 7891 (236/32768)
bestparams.threshold = 0.835852802f;
bestparams.pulsestrength = 1.7671895f;
bestparams.distance1 = 1.11400747f;
bestparams.distance2 = 1.43516386f;
            break;
        case 6:
            // current score 9596 (324)
            bestparams.threshold = 0.95831f;
            bestparams.pulsestrength = 1.95269f;
            bestparams.distance1 = 0.0077384f;
            bestparams.distance2 = 0.18408f;
// current score 12559 (389/32768)
bestparams.threshold = 0.820425332f;
bestparams.pulsestrength = 1.62089145f;
bestparams.distance1 = 1.14658892f;
bestparams.distance2 = 1.42470372f;
            break;
        case 7:
            // current score 2118 (54)
            bestparams.threshold = 0.94699f;
            bestparams.pulsestrength = 1.09668f;
            bestparams.distance1 = 0.941672206f;
            bestparams.distance2 = 2.01391125f;
// current score 2066 (54/32768)
bestparams.threshold = 0.897909403f;
bestparams.pulsestrength = 1.72159398f;
bestparams.distance1 = 1.00274765f;
bestparams.distance2 = 1.46455169f;

            bestparams.distance1 = 0.94167f;
            bestparams.distance2 = 2.0139f;
            break;
        }
        break;
    // 6582
    case 'W':
        switch (wave)
        {
        case 3:
            // current score 2200 (241)
            bestparams.threshold = 0.948939443f;

            bestparams.distance1 = 1.99798977f;
            bestparams.distance2 = 2.03521848f;

            break;
        case 5:
            // current score 8646 (214)
            bestparams.threshold = 0.936479628f;
            bestparams.pulsestrength = 1.86489666f;
            bestparams.distance1 = 1.08213437f;
            bestparams.distance2 = 1.47512901f;
            break;
        case 6:
            // current score 12409 (501)
            bestparams.threshold = 0.921457112f;
            bestparams.pulsestrength = 1.63515782f;

            bestparams.distance1 = 0.0453318208f;
            bestparams.distance2 = 0.294430673f;
            break;
        case 7:
            // current score 2109 (103)
            bestparams.threshold = 0.994506955f;
            bestparams.pulsestrength = 1.68373728f;

            bestparams.distance1 = 0.868044257f;
            bestparams.distance2 = 1.48022985f;

            break;
        }
        break;
    default:
        break;
    }

    if (bestparams.distance2 == 0.f)
        bestparams.distance2 = bestparams.distance1;

    const bool is8580 = chip >= 'V';

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
                    if ((i == Param_t::THRESHOLD) && (newValue >= 1.f))
                    {
                        newValue = 1.f - EPSILON;
                    }

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
static ref_vector_t ReadChip(int wave, char chip)
{
    std::cout << "Reading chip: " << chip << std::endl;

    std::ostringstream fileName;
    fileName << "sidwaves/WAVE" << wave << ".CSV";
    std::ifstream ifs(fileName.str().c_str(), std::ifstream::in);
    std::string line;
    ref_vector_t result;
    while (getline(ifs, line).good())
    {
        std::vector<std::string> values = split(line, ',');
        result.push_back(atoi(values[chip - 'A'].c_str()));
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

    const char chip = argv[2][0];
    assert(chip >= 'A' && chip <= 'Z');

    ref_vector_t reference = ReadChip(wave, chip);

#ifndef NDEBUG
    for (ref_vector_t::iterator it = reference.begin(); it != reference.end(); ++it)
        std::cout << (*it) << std::endl;
#endif

    srand(time(0));

    Optimize(reference, wave, chip);
}
