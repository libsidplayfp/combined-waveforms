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
static std::normal_distribution<> normal_dist(1.0, 0.05);
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
// current score 1628 (290/28672)
bestparams.threshold = 0.974392116f;
bestparams.distance1 = 23.0050449f;
bestparams.distance2 = 3.28206158f;
            break;
        case 5: // PT
            // current score 2872 (154)
            bestparams.threshold = 0.984531879f;
            bestparams.pulsestrength = 3.03670526f;
            bestparams.distance1 = 0.99342072f;
            bestparams.distance2 = 1.11435139f;
// current score 2852 (200/32768)
bestparams.threshold = 0.952721357f;
bestparams.pulsestrength = 2.01124787f;
bestparams.distance1 = 0.963695705f;
bestparams.distance2 = 1.11461008f;
            break;
        case 6: // PS
            // current score 0
            bestparams.threshold = 0.906437993f;
            bestparams.pulsestrength = 2.11317873f;
            bestparams.distance1 = 0.130663797f;
            bestparams.distance2 = 0.0835102722f;
// current score 588 (131/28672)
bestparams.threshold = 0.727954626f;
bestparams.pulsestrength = 0.000710734806f;
bestparams.distance1 = 1.00799012f;
bestparams.distance2 = 1.00027072f;
#ifdef QUADRATIC
// current score 350 (97/28672)
bestparams.threshold = 0.732864559f;
bestparams.pulsestrength = 0.000326580601f;
bestparams.distance1 = 0.0288858805f;
bestparams.distance2 = 0.000101818761f;
#endif
            break;
        case 7: // PST
            // current score 0
            bestparams.threshold = 0.924151242f;
            bestparams.pulsestrength = 1.16428149f;
            bestparams.distance1 = 0.920196056f;
            bestparams.distance2 = 1.07491302f;
// current score 62 (5/28672)
bestparams.threshold = 0.999899983f;
bestparams.pulsestrength = 1.63314331f;
bestparams.distance1 = 1.15069818f;
bestparams.distance2 = 1.0465101f;
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
// current score 2280 (288/28672)
bestparams.threshold = 0.71365726f;
bestparams.distance1 = 5.21978188f;
bestparams.distance2 = 2.55532956f;
            break;
        case 5:
            // current score 10966 (365)
            bestparams.threshold = 0.93361032f;
            bestparams.pulsestrength = 3.0441184f;
            bestparams.distance1 = 1.01603663f;
            bestparams.distance2 = 1.19291425f;
// current score 11034 (277/32768)
bestparams.threshold = 0.954938531f;
bestparams.pulsestrength = 2.80997849f;
bestparams.distance1 = 1.03282881f;
bestparams.distance2 = 1.18058574f;
            break;
        case 6:
            // current score 5528 (297)
            bestparams.threshold = 0.906388283f;
            bestparams.pulsestrength = 3.24655199f;
            bestparams.distance1 = 0.101219185f;
            bestparams.distance2 = 0.18899554f;
// current score 4975 (666/28672)
bestparams.threshold = 0.78486836f;
bestparams.pulsestrength = 2.31583929f;
bestparams.distance1 = 0.926879525f;
bestparams.distance2 = 1.11879337f;
#ifdef QUADRATIC
// current score 7258 (552/28672)
bestparams.threshold = 0.863937199f;
bestparams.pulsestrength = 2.65305567f;
bestparams.distance1 = 0.00316168088f;
bestparams.distance2 = 0.0415615179f;
#endif
            break;
        case 7:
            // current score 842 (58)
            bestparams.threshold = 0.919801712f;
            bestparams.pulsestrength = 1.64734352f;
            bestparams.distance1 = 1.21909821f;
            bestparams.distance2 = 1.22679806f;
// current score 1088 (166/28672)
bestparams.threshold = 0.824253142f;
bestparams.pulsestrength = 0.863560319f;
bestparams.distance1 = 1.49998295f;
bestparams.distance2 = 1.22292244f;
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
// current score 1928 (241/28672)
bestparams.threshold = 0.741058528f;
bestparams.distance1 = 6.50857639f;
bestparams.distance2 = 2.88163209f;
            break;
        case 5:
            // current score 9922 (424)
            bestparams.threshold = 0.927246153f;
            bestparams.pulsestrength = 2.37208962f;
            bestparams.distance1 = 1.00441122f;
            bestparams.distance2 = 1.20633698f;
// current score 9970 (420/32768)
bestparams.threshold = 0.870049f;
bestparams.pulsestrength = 1.72719193f;
bestparams.distance1 = 1.01112795f;
bestparams.distance2 = 1.20551908f;
            break;
        case 6:
            // current score 12331 (856)
            bestparams.threshold = 0.883922815f;
            bestparams.pulsestrength = 1.86374128f;
            bestparams.distance1 = 0.0953552499f;
            bestparams.distance2 = 0.456922501f;
// current score 7510 (1316/28672)
bestparams.threshold = 0.551125646f;
bestparams.pulsestrength = 0.0224781483f;
bestparams.distance1 = 0.93890202f;
bestparams.distance2 = 1.02495301f;
#ifdef QUADRATIC
// current score 9218 (850/28672)
bestparams.threshold = 0.570613563f;
bestparams.pulsestrength = 0.0665763617f;
bestparams.distance1 = 0.00148640526f;
bestparams.distance2 = 0.0123987598f;
#endif
            break;
        case 7:
            // current score 258 (64)
            bestparams.threshold = 0.91076839f;
            bestparams.pulsestrength = 1.2814858f;
            bestparams.distance1 = 1.17647922f;
            bestparams.distance2 = 1.18998444f;
// current score 723 (69/28672)
bestparams.threshold = 0.999899983f;
bestparams.pulsestrength = 1.70869279f;
bestparams.distance1 = 1.29492676f;
bestparams.distance2 = 1.16611755f;
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
// current score 1391 (235/28672)
bestparams.threshold = 0.953145444f;
bestparams.distance1 = 15.0084419f;
bestparams.distance2 = 2.66968346f;
            break;
        case 5:
            // current score 3348 (146)
            bestparams.threshold = 0.91292721f;
            bestparams.pulsestrength = 1.83235359f;
            bestparams.distance1 = 1.12143898f;
            bestparams.distance2 = 1.12768865f;
// current score 2942 (126/32768)
bestparams.threshold = 0.997553706f;
bestparams.pulsestrength = 2.05550694f;
bestparams.distance1 = 1.03922045f;
bestparams.distance2 = 1.1279707f;
            break;
        case 6:
            // current score 8 (6)
            bestparams.threshold = 0.91496712f;
            bestparams.pulsestrength = 2.28155446f;
            bestparams.distance1 = 0.122848086f;
            bestparams.distance2 = 0.0706237406f;
// current score 464 (130/28672)
bestparams.threshold = 0.727631092f;
bestparams.pulsestrength = 0.00013740026f;
bestparams.distance1 = 1.00438571f;
bestparams.distance2 = 1.00012612f;
#ifdef QUADRATIC
// current score 231 (98/28672)
bestparams.threshold = 0.733087361f;
bestparams.pulsestrength = 0.000104717306f;
bestparams.distance1 = 0.0303035676f;
bestparams.distance2 = 0.000105497958f;
#endif
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.970328987f;
            bestparams.pulsestrength = 1.75902855f;
            bestparams.distance1 = 1.21010804f;
            bestparams.distance2 = 1.269436f;
// current score 60 (4/28672)
bestparams.threshold = 0.977151155f;
bestparams.pulsestrength = 1.20450652f;
bestparams.distance1 = 1.18575227f;
bestparams.distance2 = 1.08447385f;
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
// current score 1296 (165/28672)
bestparams.threshold = 0.913765669f;
bestparams.distance1 = 13.9196177f;
bestparams.distance2 = 3.15339899f;
            break;
        case 5:
            // current score 3008 (57)
            bestparams.threshold = 0.959230483f;
            bestparams.pulsestrength = 2.74101543f;
            bestparams.distance1 = 1.00775206f;
            bestparams.distance2 = 1.11247838f;
// current score 2858 (119/32768)
bestparams.threshold = 0.947046936f;
bestparams.pulsestrength = 2.06237698f;
bestparams.distance1 = 0.995746732f;
bestparams.distance2 = 1.11283243f;
            break;
        case 6:
            // current score 416 (10)
            bestparams.threshold = 0.894908309f;
            bestparams.pulsestrength = 1.83701169f;
            bestparams.distance1 = 0.117017552f;
            bestparams.distance2 = 0.0981598422f;
// current score 1208 (167/28672)
bestparams.threshold = 0.781512201f;
bestparams.pulsestrength = 0.564687252f;
bestparams.distance1 = 1.02758503f;
bestparams.distance2 = 1.00007522f;
#ifdef QUADRATIC
// current score 964 (120/28672)
bestparams.threshold = 0.734900773f;
bestparams.pulsestrength = 0.0353164896f;
bestparams.distance1 = 0.02329899f;
bestparams.distance2 = 0.000107905551f;
#endif
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.89575851f;
            bestparams.pulsestrength = 0.865568995f;
            bestparams.distance1 = 0.986006081f;
            bestparams.distance2 = 1.17148495f;
// current score 124 (13/28672)
bestparams.threshold = 0.815193534f;
bestparams.pulsestrength = 0.560088754f;
bestparams.distance1 = 1.10899627f;
bestparams.distance2 = 1.03930104f;
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
// current score 5992 (126/32768)
bestparams.threshold = 0.932746708f;
bestparams.pulsestrength = 2.07508397f;
bestparams.distance1 = 1.03668225f;
bestparams.distance2 = 1.14876997f;
#ifdef QUADRATIC
// current score 10916 (232/32768)
bestparams.threshold = 0.94307816f;
bestparams.pulsestrength = 2.32452512f;
bestparams.distance1 = 0.000155628324f;
bestparams.distance2 = 0.0316461697f;
#endif
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
// current score 3693 (521/28672)
bestparams.threshold = 0.860927045f;
bestparams.pulsestrength = 2.43506575f;
bestparams.distance1 = 0.908603609f;
bestparams.distance2 = 1.07907593f;
#ifdef QUADRATIC
// current score 6022 (585/28672)
bestparams.threshold = 0.847607434f;
bestparams.pulsestrength = 1.88896728f;
bestparams.distance1 = 0.000250048906f;
bestparams.distance2 = 0.0341336951f;
#endif
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
// current score 338 (29/28672)
bestparams.threshold = 0.741343081f;
bestparams.pulsestrength = 0.0452554375f;
bestparams.distance1 = 1.1439606f;
bestparams.distance2 = 1.05711341f;
#ifdef QUADRATIC
// current score 342 (29/28672)
bestparams.threshold = 0.73320061f;
bestparams.pulsestrength = 0.000187645594f;
bestparams.distance1 = 0.0232787784f;
bestparams.distance2 = 0.00392863061f;
#endif
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
// current score 940 (167/28672)
bestparams.threshold = 0.820039213f;
bestparams.distance1 = 2.81103563f;
bestparams.distance2 = 1.41034496f;
            break;
        case 5:
            // current score 2744 (91)
            bestparams.threshold = 0.919414461f;
            bestparams.pulsestrength = 1.49266505f;
            bestparams.distance1 = 1.12526083f;
            bestparams.distance2 = 1.15659571f;
// current score 2364 (31/32768)
bestparams.threshold = 0.955886602f;
bestparams.pulsestrength = 1.48913789f;
bestparams.distance1 = 1.00709164f;
bestparams.distance2 = 1.13628459f;
            break;
        case 6:
            // current score 360 (30)
            bestparams.threshold = 0.941394627f;
            bestparams.pulsestrength = 2.23991108f;
            bestparams.distance1 = 0.149554357f;
            bestparams.distance2 = 0.150783867f;
// current score 1063 (89/28672)
bestparams.threshold = 0.936044216f;
bestparams.pulsestrength = 0.838183165f;
bestparams.distance1 = 1.20354855f;
bestparams.distance2 = 1.07675874f;
#ifdef QUADRATIC
// current score 1489 (170/28672)
bestparams.threshold = 0.963495851f;
bestparams.pulsestrength = 0.232831359f;
bestparams.distance1 = 1.11744213f;
bestparams.distance2 = 0.17362459f;
#endif
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.98f;
            bestparams.pulsestrength = 2.f;
            bestparams.distance1 = 1.1f;
// current score 0 (0/28672)
bestparams.threshold = 0.91070199f;
bestparams.pulsestrength = 0.658197105f;
bestparams.distance1 = 1.46202254f;
bestparams.distance2 = 0.98897016f;
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
// current score 1512 (278/28672)
bestparams.threshold = 0.913934469f;
bestparams.distance1 = 11.1513662f;
bestparams.distance2 = 2.1778872f;
            break;
        case 5:
            // current score 6847 (171)
            bestparams.threshold = 0.943752468f;
            bestparams.pulsestrength = 2.28453493f;
            bestparams.distance1 = 1.03244841f;
            bestparams.distance2 = 1.17106056f;
// current score 6743 (199/32768)
bestparams.threshold = 0.943232477f;
bestparams.pulsestrength = 1.7254281f;
bestparams.distance1 = 1.0196048f;
bestparams.distance2 = 1.17215312f;
            break;
        case 6:
            // current score 422 (17)
            bestparams.threshold = 0.894735754f;
            bestparams.pulsestrength = 1.81871581f;
            bestparams.distance1 = 0.0912446976f;
            bestparams.distance2 = 0.101162158f;
// current score 1504 (171/28672)
bestparams.threshold = 0.751202285f;
bestparams.pulsestrength = 0.247013241f;
bestparams.distance1 = 1.01501608f;
bestparams.distance2 = 1.00005627f;
#ifdef QUADRATIC
// current score 1895 (277/28672)
bestparams.threshold = 0.738982499f;
bestparams.pulsestrength = 0.0407376401f;
bestparams.distance1 = 0.0957824439f;
bestparams.distance2 = 0.00282897172f;
#endif
            break;
        case 7:
            // current score 12 (4)
            bestparams.threshold = 0.920725882f;
            bestparams.pulsestrength = 1.27905869f;
            bestparams.distance1 = 1.03366196f;
            bestparams.distance2 = 1.19196999f;
// current score 144 (20/28672)
bestparams.threshold = 0.917718291f;
bestparams.pulsestrength = 0.086768195f;
bestparams.distance1 = 2.30751419f;
bestparams.distance2 = 1.33606577f;
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
// current score 2812 (461/28672)
bestparams.threshold = 0.999899983f;
bestparams.distance1 = 0.217175797f;
bestparams.distance2 = 4.6845479f;
            break;
        case 5:
            // current score 1540 (102)
            bestparams.threshold = 0.9079f;
            bestparams.pulsestrength = 1.72749f;
            bestparams.distance1 = 1.12017f;
            bestparams.distance2 = 1.10793f;
// current score 1438 (84/32768)
bestparams.threshold = 0.959937572f;
bestparams.pulsestrength = 1.88582182f;
bestparams.distance1 = 1.10908091f;
bestparams.distance2 = 1.10897303f;
            break;
        case 6:
            // current score 0
            bestparams.threshold = 0.905734479f;
            bestparams.pulsestrength = 1.99118233f;
            bestparams.distance1 = 0.0996442288f;
            bestparams.distance2 = 0.0730706826f;
// current score 460 (127/28672)
bestparams.threshold = 0.733189285f;
bestparams.pulsestrength = 0.0608687997f;
bestparams.distance1 = 1.00428832f;
bestparams.distance2 = 1.00009477f;
#ifdef QUADRATIC
// current score 250 (89/28672)
bestparams.threshold = 0.734301805f;
bestparams.pulsestrength = 0.0428034291f;
bestparams.distance1 = 0.016789645f;
bestparams.distance2 = 0.000103888917f;
#endif
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.95248f;
            bestparams.pulsestrength = 1.51f;
            bestparams.distance1 = 1.07153f;
            bestparams.distance2 = 1.09353f;
// current score 60 (4/28672)
bestparams.threshold = 0.968275785f;
bestparams.pulsestrength = 0.920366883f;
bestparams.distance1 = 1.15777135f;
bestparams.distance2 = 1.11236846f;
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
// current score 1859 (327/28672)
bestparams.threshold = 0.78706491f;
bestparams.distance1 = 2.57177067f;
bestparams.distance2 = 1.47947466f;
            break;
        case 5:
            // current score 8086 (386)
            bestparams.threshold = 1.f;
            bestparams.pulsestrength = 3.06605577f;
            bestparams.distance1 = 0.981742382f;
            bestparams.distance2 = 1.1532563f;
// current score 8094 (392/32768)
bestparams.threshold = 0.949815691f;
bestparams.pulsestrength = 1.84632218f;
bestparams.distance1 = 0.980218053f;
bestparams.distance2 = 1.15307987f;
            break;
        case 6:
            // current score 2608 (140)
            bestparams.threshold = 0.898440778f;
            bestparams.pulsestrength = 1.99839222f;
            bestparams.distance1 = 0.108322836f;
            bestparams.distance2 = 0.096527569f;
// current score 3504 (284/28672)
bestparams.threshold = 0.737879455f;
bestparams.pulsestrength = 0.0878225341f;
bestparams.distance1 = 1.0354116f;
bestparams.distance2 = 1.00075126f;
#ifdef QUADRATIC
// current score 5462 (490/28672)
bestparams.threshold = 0.965190947f;
bestparams.pulsestrength = 0.0691156834f;
bestparams.distance1 = 1.22363687f;
bestparams.distance2 = 0.730414212f;
#endif
            break;
        case 7:
            // current score 102 (10)
            bestparams.threshold = 0.931187391f;
            bestparams.pulsestrength = 1.41706371f;
            bestparams.distance1 = 1.14805245f;
            bestparams.distance2 = 1.35853684f;
// current score 192 (18/28672)
bestparams.threshold = 0.780435681f;
bestparams.pulsestrength = 0.125344962f;
bestparams.distance1 = 1.16760302f;
bestparams.distance2 = 1.06152129f;
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
// current score 1372 (188/28672)
bestparams.threshold = 0.737709999f;
bestparams.distance1 = 2.90239716f;
bestparams.distance2 = 1.66237426f;
            break;
        case 5:
            // current score 6098 (134)
            bestparams.threshold = 0.924762011f;
            bestparams.pulsestrength = 2.37176347f;
            bestparams.distance1 = 1.04673755f;
            bestparams.distance2 = 1.16099727f;
// current score 6042 (124/32768)
bestparams.threshold = 0.926745653f;
bestparams.pulsestrength = 2.04700971f;
bestparams.distance1 = 0.993469119f;
bestparams.distance2 = 1.15294385f;
            break;
        case 6:
            // current score 1362 (127)
            bestparams.threshold = 0.924177945f;
            bestparams.pulsestrength = 2.76564121f;
            bestparams.distance1 = 0.0548680872f;
            bestparams.distance2 = 0.137630284f;
// current score 5667 (452/28672)
bestparams.threshold = 0.738824189f;
bestparams.pulsestrength = 0.164307505f;
bestparams.distance1 = 1.23515034f;
bestparams.distance2 = 1.10335743f;
#ifdef QUADRATIC
// current score 5737 (524/28672)
bestparams.threshold = 0.726974905f;
bestparams.pulsestrength = 0.126440957f;
bestparams.distance1 = 0.113696262f;
bestparams.distance2 = 0.0169145931f;
#endif
            break;
        case 7:
            // current score 84 (12)
            bestparams.threshold = 0.933185399f;
            bestparams.pulsestrength = 1.5136503f;
            bestparams.distance1 = 1.14671242f;
            bestparams.distance2 = 1.22675645f;
// current score 339 (30/28672)
bestparams.threshold = 0.799417555f;
bestparams.pulsestrength = 0.548412979f;
bestparams.distance1 = 1.14222479f;
bestparams.distance2 = 1.05515838f;
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
// current score 836 (173/28672)
bestparams.threshold = 0.918681979f;
bestparams.distance1 = 7.65736389f;
bestparams.distance2 = 2.18060994f;
            break;
        case 5:
            // current score 2434 (139)
            bestparams.threshold = 0.906191885f;
            bestparams.pulsestrength = 1.59146726f;
            bestparams.distance1 = 1.127689f;
            bestparams.distance2 = 1.1314106f;
// current score 2356 (101/32768)
bestparams.threshold = 0.941510081f;
bestparams.pulsestrength = 1.61072075f;
bestparams.distance1 = 1.05882442f;
bestparams.distance2 = 1.13106549f;
            break;
        case 6:
            // current score 0
            bestparams.threshold = 0.893231869f;
            bestparams.pulsestrength = 1.70082629f;
            bestparams.distance1 = 0.111504503f;
            bestparams.distance2 = 0.0748674423f;
// current score 460 (127/28672)
bestparams.threshold = 0.727923632f;
bestparams.pulsestrength = 0.00532891555f;
bestparams.distance1 = 1.00216448f;
bestparams.distance2 = 1.00006902f;
#ifdef QUADRATIC
// current score 223 (94/28672)
bestparams.threshold = 0.733136892f;
bestparams.pulsestrength = 0.00494264998f;
bestparams.distance1 = 0.0281301681f;
bestparams.distance2 = 0.000100072481f;
#endif
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.979779f;
            bestparams.pulsestrength = 2.03635f;
            bestparams.distance1 = 1.30189f;
// current score 56 (3/28672)
bestparams.threshold = 0.782271028f;
bestparams.pulsestrength = 0.017604202f;
bestparams.distance1 = 1.09065044f;
bestparams.distance2 = 0.969712913f;
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
// current score 1410 (187/28672)
bestparams.threshold = 0.899008334f;
bestparams.distance1 = 11.6505499f;
bestparams.distance2 = 2.89900804f;
            break;
        case 5:
            // current score 4656 (74)
            bestparams.threshold = 0.945089161f;
            bestparams.pulsestrength = 2.48777676f;
            bestparams.distance1 = 1.02335358f;
            bestparams.distance2 = 1.14071643f;
// current score 4406 (118/32768)
bestparams.threshold = 0.949076235f;
bestparams.pulsestrength = 2.00739908f;
bestparams.distance1 = 1.00364292f;
bestparams.distance2 = 1.13973677f;
            break;
        case 6:
            // current score 584 (18)
            bestparams.threshold = 0.908909619f;
            bestparams.pulsestrength = 2.23209023f;
            bestparams.distance1 = 0.12028072f;
            bestparams.distance2 = 0.119499192f;
// current score 2048 (192/28672)
bestparams.threshold = 0.74251771f;
bestparams.pulsestrength = 0.143947542f;
bestparams.distance1 = 1.02514362f;
bestparams.distance2 = 1.00037169f;
#ifdef QUADRATIC
// current score 5635 (544/28672)
bestparams.threshold = 0.995977461f;
bestparams.pulsestrength = 0.185731843f;
bestparams.distance1 = 2.42163539f;
bestparams.distance2 = 0.587991834f;
#endif
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.911848485f;
            bestparams.pulsestrength = 1.17097521f;
            bestparams.distance1 = 0.991214871f;
            bestparams.distance2 = 1.10500252f;
// current score 184 (13/28672)
bestparams.threshold = 0.769324243f;
bestparams.pulsestrength = 0.367783606f;
bestparams.distance1 = 1.03127813f;
bestparams.distance2 = 1.01245713f;
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
// current score 1372 (188/28672)
bestparams.threshold = 0.737600684f;
bestparams.distance1 = 2.90159321f;
bestparams.distance2 = 1.66177297f;
            break;
        case 5:
            // current score 6020 (101)
            bestparams.threshold = 0.929734766f;
            bestparams.pulsestrength = 2.31088519f;
            bestparams.distance1 = 1.03067636f;
            bestparams.distance2 = 1.15108597f;
// current score 5724 (85/32768)
bestparams.threshold = 0.937753856f;
bestparams.pulsestrength = 2.0206368f;
bestparams.distance1 = 1.01295638f;
bestparams.distance2 = 1.14537656f;
            break;
        case 6:
            // current score 944 (60)
            bestparams.threshold = 0.916872084f;
            bestparams.pulsestrength = 2.47265148f;
            bestparams.distance1 = 0.137618273f;
            bestparams.distance2 = 0.179932177f;
// current score 4025 (316/28672)
bestparams.threshold = 0.727271855f;
bestparams.pulsestrength = 0.000143648853f;
bestparams.distance1 = 1.02342808f;
bestparams.distance2 = 1.00505447f;
#ifdef QUADRATIC
// current score 7249 (624/28672)
bestparams.threshold = 0.857491732f;
bestparams.pulsestrength = 0.000121455705f;
bestparams.distance1 = 1.39466298f;
bestparams.distance2 = 0.495716512f;
#endif
            break;
        case 7:
            // current score 2 (2)
            bestparams.threshold = 0.901689231f;
            bestparams.pulsestrength = 1.06335056f;
            bestparams.distance1 = 1.04226708f;
            bestparams.distance2 = 1.29957008f;
// current score 256 (19/28672)
bestparams.threshold = 0.767733753f;
bestparams.pulsestrength = 0.0994318053f;
bestparams.distance1 = 1.13837802f;
bestparams.distance2 = 1.06411004f;
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
// current score 2398 (345/28672)
bestparams.threshold = 0.774881363f;
bestparams.distance1 = 4.1222415f;
bestparams.distance2 = 1.86707234f;
            break;
        case 5:
            // current score 3724 (107)
            bestparams.threshold = 0.917284548f;
            bestparams.pulsestrength = 2.60221553f;
            bestparams.distance1 = 1.03195667f;
            bestparams.distance2 = 1.08213603f;
// current score 3512 (101/32768)
bestparams.threshold = 0.946881771f;
bestparams.pulsestrength = 2.75035858f;
bestparams.distance1 = 1.00806272f;
bestparams.distance2 = 1.07748377f;
            break;
        case 6:
            // current score 267 (73)
            bestparams.threshold = 0.944150627f;
            bestparams.pulsestrength = 4.02703094f;
            bestparams.distance1 = 0.0538220257f;
            bestparams.distance2 = 0.0294455308f;
// current score 1681 (456/28672)
bestparams.threshold = 0.63802737f;
bestparams.pulsestrength = 0.0159527659f;
bestparams.distance1 = 1.00593352f;
bestparams.distance2 = 1.00032222f;
#ifdef QUADRATIC
// current score 1016 (383/28672)
bestparams.threshold = 0.639436066f;
bestparams.pulsestrength = 0.00733965635f;
bestparams.distance1 = 0.0199663416f;
bestparams.distance2 = 0.000150085427f;
#endif
            break;
        case 7:
            // current score 181 (17)
            bestparams.threshold = 0.919028938f;
            bestparams.pulsestrength = 1.21112931f;
            bestparams.distance1 = 1.14953518f;
            bestparams.distance2 = 1.21946204f;
// current score 378 (32/28672)
bestparams.threshold = 0.854620993f;
bestparams.pulsestrength = 1.03441525f;
bestparams.distance1 = 1.12384593f;
bestparams.distance2 = 1.07144833f;
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
// current score 1380 (184/28672)
bestparams.threshold = 0.742948055f;
bestparams.distance1 = 3.00511265f;
bestparams.distance2 = 1.68366838f;
            break;
        case 5:
            // current score 3740 (63)
            bestparams.threshold = 1.f;
            bestparams.pulsestrength = 3.62465143f;
            bestparams.distance1 = 0.986276627f;
            bestparams.distance2 = 1.09922075f;
// current score 3420 (101/32768)
bestparams.threshold = 0.94345057f;
bestparams.pulsestrength = 2.39676905f;
bestparams.distance1 = 0.957454324f;
bestparams.distance2 = 1.09214425f;
            break;
        case 6:
            // current score 680 (48)
            bestparams.threshold = 0.916414738f;
            bestparams.pulsestrength = 2.49077201f;
            bestparams.distance1 = 0.106750675f;
            bestparams.distance2 = 0.147990197f;
// current score 4118 (345/28672)
bestparams.threshold = 0.788462877f;
bestparams.pulsestrength = 0.593668163f;
bestparams.distance1 = 1.10185242f;
bestparams.distance2 = 1.02701366f;
#ifdef QUADRATIC
// current score 7309 (613/28672)
bestparams.threshold = 0.947275758f;
bestparams.pulsestrength = 0.173419625f;
bestparams.distance1 = 1.4866519f;
bestparams.distance2 = 0.535331249f;
#endif
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
// current score 238 (21/28672)
bestparams.threshold = 0.809811771f;
bestparams.pulsestrength = 0.757989109f;
bestparams.distance1 = 1.0658381f;
bestparams.distance2 = 1.0204711f;
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
// current score 1814 (179/28672)
bestparams.threshold = 0.787465274f;
bestparams.distance1 = 11.1375971f;
bestparams.distance2 = 3.71234536f;
            break;
        case 5:
            // current score 7211 (272)
            bestparams.threshold = 0.928046405f;
            bestparams.pulsestrength = 2.5883441f;
            bestparams.distance1 = 1.01187634f;
            bestparams.distance2 = 1.15885961f;
// current score 7209 (286/32768)
bestparams.threshold = 0.883275807f;
bestparams.pulsestrength = 1.99424374f;
bestparams.distance1 = 1.00317788f;
bestparams.distance2 = 1.15872335f;
            break;
        case 6:
            // current score 23276 (420)
            bestparams.threshold = 0.872620344f;
            bestparams.pulsestrength = 2.25908351f;
            bestparams.distance1 = 0.11984051f;
            bestparams.distance2 = 0.0987310335f;
// current score 21990 (1811/28672)
bestparams.threshold = 0.999899983f;
bestparams.pulsestrength = 1.03265238f;
bestparams.distance1 = 1.18585944f;
bestparams.distance2 = 0.100247279f;
#ifdef QUADRATIC
// current score 4682 (474/28672)
bestparams.threshold = 0.658890069f;
bestparams.pulsestrength = 0.229525238f;
bestparams.distance1 = 0.0194784869f;
bestparams.distance2 = 0.00017845718f;
#endif
            break;
        case 7:
            // current score 274 (30)
            bestparams.threshold = 0.91457653f;
            bestparams.pulsestrength = 1.32809377f;
            bestparams.distance1 = 1.11845613f;
            bestparams.distance2 = 1.16926301f;
// current score 612 (63/28672)
bestparams.threshold = 0.744126081f;
bestparams.pulsestrength = 0.327689111f;
bestparams.distance1 = 1.27223182f;
bestparams.distance2 = 1.12326503f;
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
// current score 1456 (249/28672)
bestparams.threshold = 0.818934202f;
bestparams.distance1 = 5.720613f;
bestparams.distance2 = 1.99011862f;
            break;
        case 5:
            // current score 6264 (226)
            bestparams.threshold = 0.900257707f;
            bestparams.pulsestrength = 1.89190149f;
            bestparams.distance1 = 1.05784476f;
            bestparams.distance2 = 1.1705128f;
// current score 6180 (182/32768)
bestparams.threshold = 0.882423699f;
bestparams.pulsestrength = 1.61440325f;
bestparams.distance1 = 1.03783429f;
bestparams.distance2 = 1.17249346f;
            break;
        case 6:
            // current score 1800 (121)
            bestparams.threshold = 0.922910988f;
            bestparams.pulsestrength = 2.7897532f;
            bestparams.distance1 = 0.0856195092f;
            bestparams.distance2 = 0.147621214f;
// current score 6059 (539/28672)
bestparams.threshold = 0.773076594f;
bestparams.pulsestrength = 0.458957344f;
bestparams.distance1 = 1.25925088f;
bestparams.distance2 = 1.11294782f;
#ifdef QUADRATIC
// current score 6094 (506/28672)
bestparams.threshold = 0.743406296f;
bestparams.pulsestrength = 0.262110412f;
bestparams.distance1 = 0.0836437792f;
bestparams.distance2 = 0.018961234f;
#endif
            break;
        case 7:
            // current score 148 (14)
            bestparams.threshold = 0.933337092f;
            bestparams.pulsestrength = 1.51991868f;
            bestparams.distance1 = 1.15172291f;
            bestparams.distance2 = 1.23372996f;
// current score 407 (30/28672)
bestparams.threshold = 0.776278853f;
bestparams.pulsestrength = 0.430746824f;
bestparams.distance1 = 1.08254051f;
bestparams.distance2 = 1.04694223f;
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
// current score 1146 (194/28672)
bestparams.threshold = 0.879309416f;
bestparams.distance1 = 3.79632092f;
bestparams.distance2 = 1.84771121f;
            break;
        case 5:
            // current score 6138 (221)
            bestparams.threshold = 0.991526306f;
            bestparams.pulsestrength = 2.80080104f;
            bestparams.distance1 = 0.993945718f;
            bestparams.distance2 = 1.19684732f;
// current score 6108 (189/32768)
bestparams.threshold = 0.970621347f;
bestparams.pulsestrength = 1.69028699f;
bestparams.distance1 = 1.06219876f;
bestparams.distance2 = 1.21033943f;
            break;
        case 6:
            // current score 610 (15)
            bestparams.threshold = 0.9087286f;
            bestparams.pulsestrength = 2.26664352f;
            bestparams.distance1 = 0.158760354f;
            bestparams.distance2 = 0.108530171f;
// current score 1424 (169/28672)
bestparams.threshold = 0.762821257f;
bestparams.pulsestrength = 0.385563523f;
bestparams.distance1 = 1.00569153f;
bestparams.distance2 = 1.00022411f;
#ifdef QUADRATIC
// current score 1173 (127/28672)
bestparams.threshold = 0.735352457f;
bestparams.pulsestrength = 0.0260615852f;
bestparams.distance1 = 0.0295856707f;
bestparams.distance2 = 0.000101976053f;
#endif
            break;
        case 7:
            // current score 0
            bestparams.threshold = 0.949945092f;
            bestparams.pulsestrength = 1.60713959f;
            bestparams.distance1 = 1.01901114f;
            bestparams.distance2 = 1.03737819f;
// current score 135 (17/28672)
bestparams.threshold = 0.929782033f;
bestparams.pulsestrength = 1.59475529f;
bestparams.distance1 = 1.1103152f;
bestparams.distance2 = 1.04106855f;
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
// current score 1298 (194/28672)
bestparams.threshold = 0.786892831f;
bestparams.distance1 = 3.44096828f;
bestparams.distance2 = 1.85426927f;
            break;
        case 5:
            // current score 6398 (287)
            bestparams.threshold = 0.996706426f;
            bestparams.pulsestrength = 3.12744141f;
            bestparams.distance1 = 0.999570251f;
            bestparams.distance2 = 1.16731513f;
// current score 6142 (175/32768)
bestparams.threshold = 0.98870635f;
bestparams.pulsestrength = 2.08824015f;
bestparams.distance1 = 1.04811096f;
bestparams.distance2 = 1.16984022f;
            break;
        case 6:
            // current score 754 (38)
            bestparams.threshold = 0.908193409f;
            bestparams.pulsestrength = 2.19632339f;
            bestparams.distance1 = 0.0940685794f;
            bestparams.distance2 = 0.119425967f;
// current score 2648 (227/28672)
bestparams.threshold = 0.784013271f;
bestparams.pulsestrength = 0.608920038f;
bestparams.distance1 = 1.01388061f;
bestparams.distance2 = 1.00038803f;
#ifdef QUADRATIC
// current score 2736 (245/28672)
bestparams.threshold = 0.736335278f;
bestparams.pulsestrength = 0.0851064995f;
bestparams.distance1 = 0.0301715117f;
bestparams.distance2 = 0.000850095006f;
#endif
            break;
        case 7:
            // current score 6 (4)
            bestparams.threshold = 0.925804496f;
            bestparams.pulsestrength = 1.36537039f;
            bestparams.distance1 = 1.1688062f;
            bestparams.distance2 = 1.32638979f;
// current score 193 (16/28672)
bestparams.threshold = 0.850530267f;
bestparams.pulsestrength = 0.909227252f;
bestparams.distance1 = 1.10618293f;
bestparams.distance2 = 1.0431186f;
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
#ifdef QUADRATIC
// current score 1257 (164/32768)
bestparams.threshold = 0.691515148f;
bestparams.distance1 = 0.115770422f;
bestparams.distance2 = 0.819613338f;
#endif
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
// current score 7991 (212/32768)
bestparams.threshold = 0.93500334f;
bestparams.pulsestrength = 1.05977178f;
bestparams.distance1 = 1.08629429f;
bestparams.distance2 = 1.43518543f;
#ifdef QUADRATIC
// current score 9113 (198/32768)
bestparams.threshold = 0.955464482f;
bestparams.pulsestrength = 1.33896255f;
bestparams.distance1 = 0.000220529852f;
bestparams.distance2 = 0.183474064f;
#endif
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
// current score 12566 (394/32768)
bestparams.threshold = 0.920648575f;
bestparams.pulsestrength = 0.943601072f;
bestparams.distance1 = 1.13034654f;
bestparams.distance2 = 1.41881108f;
#ifdef QUADRATIC
//current score 10379 (362/32768)
bestparams.threshold = 0.921799242f;
bestparams.pulsestrength = 1.10167611f;
bestparams.distance1 = 0.000543073867f;
bestparams.distance2 = 0.18010284f;
#endif
            break;
        case 7:
            // current score 2118 (54)
            bestparams.threshold = 0.94699f;
            bestparams.pulsestrength = 1.09668f;
            bestparams.distance1 = 0.94167f;
            bestparams.distance2 = 2.0139f;
// current score 2066 (54/32768)
bestparams.threshold = 0.897909403f;
bestparams.pulsestrength = 1.72159398f;
bestparams.distance1 = 1.00274765f;
bestparams.distance2 = 1.46455169f;
// current score 2092 (60/32768)
bestparams.threshold = 0.90921098f;
bestparams.pulsestrength = 0.979807794f;
bestparams.distance1 = 0.942194462f;
bestparams.distance2 = 1.40958893f;
#ifdef QUADRATIC
// current score 2663 (72/32768)
bestparams.threshold = 0.978958666f;
bestparams.pulsestrength = 0.358672857f;
bestparams.distance1 = 0.101479679f;
bestparams.distance2 = 1.01369643f;
#endif
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
// current score 1759 (235/32768)
bestparams.threshold = 0.727274895f;
bestparams.distance1 = 1.62215316f;
bestparams.distance2 = 3.44898725f;
#ifdef QUADRATIC
// current score 9591 (319/32768)
bestparams.threshold = 0.655504584f;
bestparams.distance1 = 0.0351161323f;
bestparams.distance2 = 0.632410944f;
#endif
            break;
        case 5:
            // current score 8646 (214)
            bestparams.threshold = 0.936479628f;
            bestparams.pulsestrength = 1.86489666f;
            bestparams.distance1 = 1.08213437f;
            bestparams.distance2 = 1.47512901f;
// current score 8582 (222/32768)
bestparams.threshold = 0.950187087f;
bestparams.pulsestrength = 1.19043314f;
bestparams.distance1 = 1.0927738f;
bestparams.distance2 = 1.48095214f;
#ifdef QUADRATIC
// current score 10504 (274/32768)
bestparams.threshold = 0.938506305f;
bestparams.pulsestrength = 1.23100233f;
bestparams.distance1 = 0.00112735783f;
bestparams.distance2 = 0.285014719f;
#endif
            break;
        case 6:
            // current score 12409 (501)
            bestparams.threshold = 0.921457112f;
            bestparams.pulsestrength = 1.63515782f;
            bestparams.distance1 = 0.0453318208f;
            bestparams.distance2 = 0.294430673f;
// current score 16688 (610/32768)
bestparams.threshold = 0.944962323f;
bestparams.pulsestrength = 0.875410736f;
bestparams.distance1 = 1.26995218f;
bestparams.distance2 = 1.61216533f;
#ifdef QUADRATIC
// current score 13035 (520/32768)
bestparams.threshold = 0.950229704f;
bestparams.pulsestrength = 1.13534844f;
bestparams.distance1 = 0.0525861941f;
bestparams.distance2 = 0.293604106f;
#endif
            break;
        case 7:
            // current score 2109 (103)
            bestparams.threshold = 0.994506955f;
            bestparams.pulsestrength = 1.68373728f;
            bestparams.distance1 = 0.868044257f;
            bestparams.distance2 = 1.48022985f;
// current score 8317 (151/32768)
bestparams.threshold = 0.910725236f;
bestparams.pulsestrength = 1.10669744f;
bestparams.distance1 = 0.896611392f;
bestparams.distance2 = 1.45393038f;
#ifdef QUADRATIC
// current score 9487 (143/32768)
bestparams.threshold = 0.876457989f;
bestparams.pulsestrength = 0.813472748f;
bestparams.distance1 = 0.00255764835f;
bestparams.distance2 = 0.33945635f;
#endif
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
