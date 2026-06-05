/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_common.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_COMMON_H
#define _HERSS_COMMON_H

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

// BVM May 2026, we start using the version convention MAJOR.MINOR.PATCH
const string VERSION = "3.1.02";
const string VERSION_DATE = "20260605";

// Maximum number of nodes allowed. // to save coding
#define MAX_NR_NODES 30
// Maximum number of columns (words or tokens) in one line
#define MAX_WORDS 200

// To make initialisation of array easier.
#define MAX_TRAVELTIME_HOURS 240

// Maximum number of points in a point curve
#define MAX_NR_POINTS_CURVE 50

// If MINIMIZE_OUTPUT=1 we write only the the riversystem outputfile with waterbalance and economy.
// #define MINIMIZE_OUTPUT 0

// Average Earth gravity
#define GRAVITY 9.80665

#define PI 3.14159265358979323846

#define MOUNT_EVEREST_MASL 8848.0

const string DELIMITER = " \n\t";
const string NUMERIC = "0123456789.-";

#define NOT_INIT 9999999
#define STR_NOT_INIT "ERROR_STR_NOT_INIT"

// #include <limits> std::numeric_limits<double>::max();
// More practical number to use in the code.
#define VERY_LARGE_NUMBER 999999999

const std::string DEFAULT_STRING_INIT = "ERROR_STR_NOT_INIT";

#define HERSS_DEBUG_ALL false

#define MAX_NUMBER_OF_QMIN_PERIODS 5

// Turn on and off warnings related to check of waterbalance.
#define WATERBALANCE_WARNINGS false

// Turn on and off warnings related to check of economy in the system
#define ECONOMY_WARNINGS false

// Maximum nr of generators in a powerstation
#define MAX_NR_GENERATORS 6

#define MAX_NR_CASCADED_RESERVOIRS 10

/////////////////////////////////////////////////////////////////
#define MACRO_m3s_2_Mm3(q, dt) q*dt/1000000.0
#define MACRO_Mm3_2_m3s(q, dt) q*1000000.0/dt
/////////////////////////////////////////////////////////////////

#ifdef _WIN32
   #define timegm(X) _mktime64(X) - timezone
   #define gmtime_r(X,Y) gmtime_s(Y,X)
#endif

#ifdef _WIN64
   #define timegm(X) _mktime64(X) - timezone
   #define gmtime_r(X,Y) gmtime_s(Y,X)
#endif

//-----------------------------------------------------------------------------
// Smoothly maps R -> [0,1] (differentiable alternative to clamp)
// We avoid "hard"-constraining the range of parameters [min,max]
// Instead we use the smoothClamp01 function to smoothly constrain the range of parameters to [0,1],
// and then we can scale and shift this to get the desired range.
inline double smoothClamp01(double x) {
    double y = 0.5 * (x / (1.0 + std::abs(x)) + 1.0);
    return y * y * (3.0 - 2.0 * y);
}

// We avoid using hard minimum and maximum functions,
// and instead use smooth approximations
inline double smooth_max(double a, double b) {
    return log(exp(a) + exp(b));
}

inline double smooth_min(double a, double b) {
    return -log(exp(-a) + exp(-b));
}
//-----------------------------------------------------------------------------

// This is the naming convention that needs to be used inside the topology file
// In earlier versions we used POWERSTATION, this has now changed to PSTATION
// We make it more consistent with class names.
enum NodeType
{
   RESERVOIR,
   PSTATION,
   CHANNEL
};

const size_t NODE_TYPE_COUNT = 3;

inline const char* EnumToString(NodeType v)
{
    switch (v)
    {
        case RESERVOIR:   return "RESERVOIR";
        case PSTATION:   return "PSTATION";
        case CHANNEL: return "CHANNEL";
    }
    return "VOID";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Forward declarations to make all classes visible before they are defined
class Scenario;
class GlobalConfig;
class Channel;
class SystemState;
class Herss;

class CascadedReservoirs;

#endif
