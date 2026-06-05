/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_channel.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_CHANNEL_H
#define _HERSS_CHANNEL_H

#include "herss_node.h"
#include "herss_routing.h"

/////////////////////////////////////////////////////////////////////////////////////////
class Channel: public Node {
    public:
    Channel();
    ~Channel();
    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;
    Channel(Channel&&) noexcept = default;
    Channel& operator=(Channel&&) noexcept = default;
    size_t stps;
    size_t dt;
    GlobalConfig *gc;

    CascadedReservoirs *casc_reservoirs;
    // We use a series of cascaded reservoirs to model the routing in the channel.

    std::vector<double> initial_storage_linres_Mm3;

    double K_traveltime_hours;  // Travel time in hours, used in the cascaded reservoir routing model.
    size_t num_cascaded_reservoirs;  // Number of cascaded reservoirs used in the routing model.

    double decay;
    double waterflow_m3[MAX_TRAVELTIME_HOURS];  // Keeps track of how much water that is stored in the channel. [m3]
    double init_waterflow_m3[MAX_TRAVELTIME_HOURS];  // Keeps track of how much water that is stored in the channel. [m3]

    int ReadNodeData(string filename);
    int ReadStateFile(string filename);
    int Simulate(size_t t);
    int initArrayCurves(void);
    int CheckWaterBalance(class Herss *herss_obj);
    double GetStartWater_Mm3(void);
    double GetEndWater_Mm3(void);
    int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node
    double GetTunnelFLow(size_t t);
    int WriteStateFile(FILE *fp);
    int SetStartState(void);
    void PrintChannelWater(void);
    void ValidateChannelSettings();  // We check if the settings for the channel are valid. For example, that the travel time is not longer than the maximum allowed.

private:
    std::unique_ptr<CascadedReservoirs> casc_reservoirs_owner;

};

#endif
