/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_powerstation.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_POWERSTATION_H
#define _HERSS_POWERSTATION_H

#include "arraycurve.h"
#include "herss_node.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CHANGE BY OVE - Initialize the Generator struct
struct Generator {
        std::vector<double> turb_virkn_Q;       // Turbine efficiency curve Q values
        std::vector<double> turb_virkn_psnt;    // Turbine efficiency curve psnt values
        ArrayCurve eff_curve;
        std::vector<double> action;             // Actions for this generator
        double headlosscoef;                    // Head loss coefficient for this generator/penstock
        double max_discharge;                   // Maximum discharge for this generator
    };

class Powerstation: public Node {
    public:
    Powerstation();
    ~Powerstation();
    size_t stps;
    size_t dt;
    double init_Power;
    GlobalConfig *gc;

    double static_gen_efficiency;
    double headlosscoef;
    double powstat_masl;
    double powstat_startstop;
    bool shared_penstock;  // CHANGE BY OVE: true = shared penstock, false = separate penstocks

    size_t nr_generators;  // We can get this out of the vector, but its nice to have.
    std::vector<Generator> generators; // CHANGE BY OVE: Implemented vectors to hold generators in a powerstation.

    // BVM 15 oct 2024.
    // In a river runoff powerstation, it is possible to empty the intake in one timestep.
    // Since we do not allow this, and we override the action.
    // We give a marginal penalty so that we see a change in the VF.
    double aggressive_actions_cost;

    int ReadNodeData(string filename);
    int ReadStateFile(string filename);
    int Simulate(size_t t);
    // int initArrayCurves(void);
    //int CheckWaterBalance(void);
    int CheckWaterBalance(class Herss *herss_obj);
    double GetStartWater_Mm3(void);
    double GetEndWater_Mm3(void);
    int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node
    double GetTunnelFLow(size_t t);
    int WriteStateFile(FILE *fp);
    double CalcAdjustmenCosts(void); // Only for Powerstation
    void ValidatePowerstationSettings();  // We check if the settings for the powerstation are valid. For example, that the number of generators is not higher than the maximum allowed, and that the headloss coefficient is not negative.

};

#endif
