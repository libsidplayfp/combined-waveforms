/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright 2013-2020 Leandro Nini <drfiemost@users.sourceforge.net>
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


static const float EPSILON = 1e-3;

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
static std::normal_distribution<> normal_dist(1.0, 0.0002);
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
     * on a total of 32768.
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 5.58014059f;
            bestparams.distance2 = 5.4269886f;
            bestparams.stmix = 0.806023061f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.130663797f;
            bestparams.distance2 = 0.0835102722f;
            break;
        case 7: // PST
            // current score 0
            bestparams.threshold = 0.924151242f;
            bestparams.pulsestrength = 1.16428149f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.920196056f;
            bestparams.distance2 = 1.07491302f;
            bestparams.stmix = 0.793505609f;
            break;
        }
        break;
    // 6581 R2 (odd)
    case 'C':
        switch (wave)
        {
        case 3:
            // current score 2948 (446)
            bestparams.threshold = 0.881458f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.93176f;
            bestparams.stmix = 0.596385f;
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
            bestparams.topbit = 1.5743562f; // ???
            bestparams.distance1 = 0.101219185f;
            bestparams.distance2 = 0.18899554f;
            break;
        case 7:
            // current score 850 (60)
            bestparams.threshold = 0.919113517f;
            bestparams.pulsestrength = 1.62892509f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.21794045f;
            bestparams.distance2 = 1.2279855f;
            bestparams.stmix = 0.896797657f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.582165f;
            bestparams.distance2 = 2.23844433f;
            bestparams.stmix = 0.53945446f;
            break;
        case 5:
            // current score 9922 (424)
            bestparams.threshold = 0.927246153f;
            bestparams.pulsestrength = 2.37208962f;
            bestparams.distance1 = 1.00441122f;
            bestparams.distance2 = 1.20633698f;
            break;
        case 6:
            // current score 12332 (857)
            bestparams.threshold = 0.883725405f;
            bestparams.pulsestrength = 1.86024225f;
            bestparams.topbit = 1.12501097f; // ???
            bestparams.distance1 = 0.0956423283f;
            bestparams.distance2 = 0.457357466f;
            break;
        case 7:
            // current score 258 (64)
            bestparams.threshold = 0.91076839f;
            bestparams.pulsestrength = 1.2814858f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.17647922f;
            bestparams.distance2 = 1.18998444f;
            bestparams.stmix = 0.771969795f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 5.75852f;
            bestparams.stmix = 0.800831f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.122848086f;
            bestparams.distance2 = 0.0706237406f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.970328987f;
            bestparams.pulsestrength = 1.75902855f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.21010804f;
            bestparams.distance2 = 1.269436f;
            bestparams.stmix = 0.983422756f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 7.18324232f;
            bestparams.distance2 = 6.00581455f;
            bestparams.stmix = 0.841992021f;
            break;
        case 5:
            // current score 3008 (57)
            bestparams.threshold = 0.959230483f;
            bestparams.pulsestrength = 2.74101543f;
            bestparams.distance1 = 1.00775206f;
            bestparams.distance2 = 1.11247838f;
            break;
        case 6:
            // current score 432 (12)
            bestparams.threshold = 0.902768612f;
            bestparams.pulsestrength = 2.06190324f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.150018558f;
            bestparams.distance2 = 0.102936305f;
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.89575851f;
            bestparams.pulsestrength = 0.865568995f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.986006081f;
            bestparams.distance2 = 1.17148495f;
            bestparams.stmix = 0.547324002f;
            break;
        }
        break;
    // 6581 R2 (odd)
    case 'G':
        switch (wave)
        {
        case 3:
            // current score 1689 (280)
            bestparams.threshold = 0.90251f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.9147f;
            bestparams.distance2 = 1.6747f;
            bestparams.stmix = 0.62376f;
            break;
        case 5:
            // current score 6128 (130)
            bestparams.threshold = 0.93088f;
            bestparams.pulsestrength = 2.4843f;
            bestparams.distance1 = 1.0353f;
            bestparams.distance2 = 1.1484f;
            break;
        case 6:
            // current score 10575 (568)
            bestparams.threshold = 0.912125707f;
            bestparams.pulsestrength = 2.32007766f;
            bestparams.topbit = 1.10602474f; // ???
            bestparams.distance1 = 0.0538718924f;
            bestparams.distance2 = 0.251507759f;
            break;
        case 7:
            // current score 64 (2)
            bestparams.threshold = 0.91f;
            bestparams.pulsestrength = 1.192f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.0169f;
            bestparams.distance2 = 1.2f;
            bestparams.stmix = 0.637f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 3.35368633f;
            bestparams.distance2 = 2.60800719f;
            bestparams.stmix = 0.670109689f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.149554357f;
            bestparams.distance2 = 0.150783867f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.98f;
            bestparams.pulsestrength = 2.f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.1f;
            bestparams.stmix = 0.91f;
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
            bestparams.topbit = 0.001f;
            bestparams.distance1 = 4.43f;
            bestparams.distance2 = 4.433f;
            bestparams.stmix = 0.776272893f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.0912446976f;
            bestparams.distance2 = 0.101162158f;
            break;
        case 7:
            // current score 12 (4)
            bestparams.threshold = 0.920725882f;
            bestparams.pulsestrength = 1.27905869f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.03366196f;
            bestparams.distance2 = 1.19196999f;
            bestparams.stmix = 0.521854043f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 3.98271f;
            bestparams.stmix = 0.775023f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.0996442288f;
            bestparams.distance2 = 0.0730706826f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.95248f;
            bestparams.pulsestrength = 1.51f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.07153f;
            bestparams.distance2 = 1.09353f;
            bestparams.stmix = 1.f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.78504324f;
            bestparams.distance2 = 2.21891737f;
            bestparams.stmix = 0.692269444f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.108322836f;
            bestparams.distance2 = 0.096527569f;
            break;
        case 7:
            // current score 102 (10)
            bestparams.threshold = 0.931187391f;
            bestparams.pulsestrength = 1.41706371f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.14805245f;
            bestparams.distance2 = 1.35853684f;
            bestparams.stmix = 0.48f;
            break;
        }
        break;
    // 6581 R2
    case 'L':
        switch (wave)
        {
        case 3:
            // current score 1356 (109)
            bestparams.threshold = 0.983135f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 4.62089f;
            bestparams.stmix = 0.778836f;
            break;
        case 5:
            // current score 6098 (134)
            bestparams.threshold = 0.924762011f;
            bestparams.pulsestrength = 2.37176347f;
            bestparams.distance1 = 1.04673755f;
            bestparams.distance2 = 1.16099727f;
            break;
        case 6:
            // current score 1534 (107)
            bestparams.threshold = 0.927221477f;
            bestparams.pulsestrength = 2.94884133f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.0910025164f;
            bestparams.distance2 = 0.132933453f;
            break;
        case 7:
            // current score 102 (16)
            bestparams.threshold = 0.941179f;
            bestparams.pulsestrength = 1.65307f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.22544f;
            bestparams.stmix = 0.748047f;
            break;
        }
        break;  
    // 6581 R2
    case 'M':
        switch (wave)
        {
        case 3:
            // current score 352 (106)
            bestparams.threshold = 0.938881f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 2.07118f;
            bestparams.stmix = 0.579197f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.111504503f;
            bestparams.distance2 = 0.0748674423f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.979779f;
            bestparams.pulsestrength = 2.03635f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.30189f;
            bestparams.stmix = 0.923735f;
            break;
        }
        break;
    // 6581 R2
    case 'N':
        switch (wave)
        {
        case 3:
            // current score 662 (104)
            bestparams.threshold = 0.981390178f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 4.37947226f;
            bestparams.distance2 = 4.3854661f;
            bestparams.stmix = 0.772289276f;
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
            bestparams.threshold = 0.908709824f;
            bestparams.pulsestrength = 2.22933149f;
            bestparams.topbit = -0.003f; // ???
            bestparams.distance1 = 0.120273024f;
            bestparams.distance2 = 0.11959032f;
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.911848485f;
            bestparams.pulsestrength = 1.17097521f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.991214871f;
            bestparams.distance2 = 1.10500252f;
            bestparams.stmix = 0.591298461f;
            break;
        }
        break;
    // 6581 R3
    case 'O':
        switch (wave)
        {
        case 3:
            // current score 1100 (118)
            bestparams.threshold = 0.967385352f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 3.22246957f;
            bestparams.distance2 = 3.13088274f;
            bestparams.stmix = 0.739427269f;
            break;
        case 5:
            // current score 6052 (99)
            bestparams.threshold = 0.929785728f;
            bestparams.pulsestrength = 2.30605006f;
            bestparams.distance1 = 1.03718281f;
            bestparams.distance2 = 1.1534183f;
            break;
        case 6:
            // current score 1012 (54)
            bestparams.threshold = 0.919249177f;
            bestparams.pulsestrength = 2.58188939f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.131623372f;
            bestparams.distance2 = 0.158718824f;
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.901689231f;
            bestparams.pulsestrength = 1.06335056f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.04226708f;
            bestparams.distance2 = 1.29957008f;
            bestparams.stmix = 0.509225965f;
            break;
        }
        break;
    // 6581 R3
    case 'P':
        switch (wave)
        {
        case 3:
            // current score 1806 (118)
            bestparams.threshold = 0.981850088f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 4.73386049f;
            bestparams.distance2 = 5.07241964f;
            bestparams.stmix = 0.784137607f;
            break;
        case 5:
            // current score 3728 (109)
            bestparams.threshold = 0.918060482f;
            bestparams.pulsestrength = 2.61582947f;
            bestparams.distance1 = 1.03105855f;
            bestparams.distance2 = 1.0820508f;
            break;
        case 6:
            // current score 269 (74)
            bestparams.threshold = 0.944703519f;
            bestparams.pulsestrength = 4.04210186f;
            bestparams.topbit = -0.001f; // ???
            bestparams.distance1 = 0.0535937659f;
            bestparams.distance2 = 0.0294511318f;
            break;
        case 7:
            // current score 181 (17)
            bestparams.threshold = 0.919028938f;
            bestparams.pulsestrength = 1.21112931f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.14953518f;
            bestparams.distance2 = 1.21946204f;
            bestparams.stmix = 0.672066152f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 4.59079f;
            bestparams.stmix = 0.778088f;
            break;
        case 5:
            // current score 3740 (63)
            bestparams.threshold = 1.f;
            bestparams.pulsestrength = 3.62465143f;
            bestparams.distance1 = 0.986276627f;
            bestparams.distance2 = 1.09922075f;
            break;
        case 6:
            // current score 748 (58)
            bestparams.threshold = 0.912882149f;
            bestparams.pulsestrength = 2.45562696f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.128567815f;
            bestparams.distance2 = 0.148435056f;
            break;
        case 7:
            // current score 38 (14)
            bestparams.threshold = 0.901646256f;
            bestparams.pulsestrength = 0.918491125f;
            bestparams.topbit = 0.001f;
            bestparams.distance1 = 1.05738974f;
            bestparams.distance2 = 1.10902572f;
            bestparams.stmix = 0.617564797f;
            // current score 64 (3)
            //bestparams.threshold = 0.898101449f;
            //bestparams.pulsestrength = 0.797706962f;
            //bestparams.topbit = 0.f;
            //bestparams.distance1 = 0.989617884f;
            //bestparams.distance2 = 1.0587045f;
            //bestparams.stmix = 0.684196472f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.94586849f;
            bestparams.distance2 = 2.01927376f;
            bestparams.stmix = 0.600944996f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.11984051f;
            bestparams.distance2 = 0.0987310335f;
            break;
        case 7:
            // current score 274 (30)
            bestparams.threshold = 0.91457653f;
            bestparams.pulsestrength = 1.32809377f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.11845613f;
            bestparams.distance2 = 1.16926301f;
            bestparams.stmix = 0.791111946f;
            break;
        }
        break;
    // 6581 R4AR
    case 'S':
        switch (wave)
        {
        case 3:
            // current score 1724 (124)
            bestparams.threshold = 0.973066f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 3.57771f;
            bestparams.stmix = 0.747192f;
            break;
        case 5:
            // current score 6264 (226)
            bestparams.threshold = 0.900257707f;
            bestparams.pulsestrength = 1.89190149f;
            bestparams.distance1 = 1.05784476f;
            bestparams.distance2 = 1.1705128f;
            break;
        case 6:
            // current score 2006 (131)
            bestparams.threshold = 0.917820513f;
            bestparams.pulsestrength = 2.8137641f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.133098751f;
            bestparams.distance2 = 0.139843836f;
            break;
        case 7:
            // current score 148 (14)
            bestparams.threshold = 0.933337092f;
            bestparams.pulsestrength = 1.51991868f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.15172291f;
            bestparams.distance2 = 1.23372996f;
            bestparams.stmix = 0.693227589f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 3.04781175f;
            bestparams.distance2 = 3.81387138f;
            bestparams.stmix = 0.609185994f;
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
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.158760354f;
            bestparams.distance2 = 0.108530171f;
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.949945092f;
            bestparams.pulsestrength = 1.60713959f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.01901114f;
            bestparams.distance2 = 1.03737819f;
            bestparams.stmix = 0.994224012f;
            break;
        }
        break;
    // 6581 R4AR
    case 'U':
        switch (wave)
        {
        case 3:
            // current score 294 (94)
            bestparams.threshold = 0.983248f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 4.63783f;
            bestparams.stmix = 0.779401f;
            break;
        case 5:
            // current score 6454 (259)
            bestparams.threshold = 0.99398762f;
            bestparams.pulsestrength = 3.14190888f;
            bestparams.distance1 = 0.999676824f;
            bestparams.distance2 = 1.16238594f;
            break;
        case 6:
            // current score 840 (42)
            bestparams.threshold = 0.903786302f;
            bestparams.pulsestrength = 2.10020733f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 0.142474398f;
            bestparams.distance2 = 0.139588535f;
            break;
        case 7:
            // current score 6 (4)
            bestparams.threshold = 0.925804496f;
            bestparams.pulsestrength = 1.36537039f;
            bestparams.topbit = 0.f;
            bestparams.distance1 = 1.1688062f;
            bestparams.distance2 = 1.32638979f;
            bestparams.stmix = 0.509957671f;
            break;
        }
        break;
    // 8580
    case 'V':
        switch (wave)
        {
        case 3:
            // current score 1380 (169)
            bestparams.threshold = 0.96345f;
            bestparams.topbit = 0.976f;
            bestparams.distance1 = 1.751f;
            bestparams.distance2 = 2.366f;
            bestparams.stmix = 0.978f;
            break;
        case 5:
            // current score 7981 (204)
            bestparams.threshold = 0.93303f;
            bestparams.pulsestrength = 1.7025f;
            bestparams.distance1 = 1.0868f;
            bestparams.distance2 =  1.43527f;
            break;
        case 6:
            // current score 9596 (324)
            bestparams.threshold = 0.958310068f;
            bestparams.pulsestrength = 1.95269263f;
            bestparams.topbit = 0.992986143f;
            bestparams.distance1 = 0.00773835462f;
            bestparams.distance2 = 0.184085369f;
            break;
        case 7:
            // current score 2118 (54)
            bestparams.threshold = 0.946992695f;
            bestparams.pulsestrength = 1.09667945f;
            bestparams.topbit = 0.995859802f;
            bestparams.distance1 = 0.941672206f;
            bestparams.distance2 = 2.01391125f;
            bestparams.stmix = 0.563315094f;
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
            bestparams.topbit = 0.982385635f;
            bestparams.distance1 = 1.99798977f;
            bestparams.distance2 = 2.03521848f;
            bestparams.stmix = 0.98950702f;
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
            bestparams.topbit = 0.99865073f;
            bestparams.distance1 = 0.0453318208f;
            bestparams.distance2 = 0.294430673f;
            break;
        case 7:
            // current score 2149 (110)
            bestparams.threshold = 0.997233212f;
            bestparams.pulsestrength = 1.69910455f;
            bestparams.topbit = 0.995346427f;
            bestparams.distance1 = 0.861709118f;
            bestparams.distance2 = 1.48027956f;
            bestparams.stmix = 0.730670393f;
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
    std::cout << "# initial score " << bestscore << std::endl << bestparams.toString() << std::endl << std::endl;
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
            for (Param_t i = Param_t::THRESHOLD; i <= Param_t::STMIX; i++)
            {
                // PULSESTRENGTH only affects pulse
                if ((i==Param_t::PULSESTRENGTH) && ((wave & 0x04) != 0x04))
                {
                    continue;
                }

                // STMIX only affects saw/triangle mix
                if ((i==Param_t::STMIX) && ((wave & 0x03) != 0x03))
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

                    // try to avoid too small values
                    if (newValue < EPSILON)
                        newValue += GetNewRandomValue();

                    // check for parameters limits
                    if ((i == Param_t::STMIX || i == Param_t::THRESHOLD) && (newValue > 1.f)
                        /*|| (i == Param_t::DISTANCE)  && (newValue < 1.f)*/)
                    {
                        newValue = 1.f;
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
            std::cout << "# current score " << score << std::endl << p.toString() << std::endl << std::endl;
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
