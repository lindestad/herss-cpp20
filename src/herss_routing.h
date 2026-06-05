/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_routing.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_ROUTING_H
#define _HERSS_ROUTING_H

#include "herss_common.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// BVM May 2026, new routing model inside the Channel class.
// We make a routing model that is suited for multi-temporal resolution.
// It is based on a series of cascaded linear reservoirs. The number of reservoirs can be set by the user.
struct ReservoirState {
    double storage_m3 = 0.0;
};

struct ReservoirStepResult {
    double storage_new_m3;
    double q_out_avg_m3s;
    double q_out_end_m3s;
};

//------------------------------------------------------------------------------------------------
// Class models a series of cascaded reservoirs that will be used in the channel class/routing.
class CascadedReservoirs {

  public:
    CascadedReservoirs(double k_traveltime_hours, size_t num_reservoirs);
    ~CascadedReservoirs();
    void setInitialStorage(std::vector<double> initial_storage_Mm3);
    double route(double q_in_m3s, double dt_seconds);
    double totalStorageM3();
    double getStorageMm3(size_t idx_linres);

  private:

    ReservoirStepResult routeOneReservoir(
        double storage_old_m3, double q_in_m3s, double k_seconds, double dt_seconds);

    // In each channel the user can choose number of cascaded linear reservoirs
    std::vector<ReservoirState> linreservoirs;
    double k_total_seconds;
    double k_res_seconds;
    double k_traveltime_hours;
    size_t num_reservoirs;


};

#endif
