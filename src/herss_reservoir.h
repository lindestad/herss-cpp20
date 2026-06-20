/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_reservoir.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_RESERVOIR_H
#define _HERSS_RESERVOIR_H

#include "arraycurve.h"
#include "herss_node.h"

/////////////////////////////////////////////////////////////////////////////////////////
class Reservoir: public Node {

    public:
    Reservoir();
    ~Reservoir();
    size_t stps;
    size_t dt;
    GlobalConfig *gc;
    double floodlevel_penalty;
    double floodlevel_cost;
    double reservoir_init_fr;
    double reservoir_init_masl;
    double reservoir_init_Mm3;
    double reservoir_init_active_Mm3;  // minus the filling at LRW
    double active_max_volume_Mm3;  // Filling at HRW minus the filling at LRW
    double res_HRW;             //  Highest regulated water level [masl]
    double res_LRW;             //  Lowest regulated water level [masl]
    double filling_at_hrw_Mm3;  // Mm3
    double filling_at_lrw_Mm3;  // Mm3
    double filling_at_hatchlevel;
    double cost_lrw;
    double res_penalty;
    bool fast_overflow;         // If true, we use the fast overflow calculation.

    // Dynamic variables that change during the simulation
    double res_Mm3;             //  Reservoir filling [Mm3]
    double res_masl;            //  Reservoir filling [masl]
    double res_fr;              //  Reservoir filling as a fraction of full.


    // Reservoir geometry, we assume a trapezoidal shape
    // we use this to scip the reservoir curve.
    bool use_reservoir_geometry;
    double width_m;
    double length_m;
    double theta; // angle of the sides of the reservoir, in degrees.
    double bottom_masl; // masl of the bottom of the reservoir.

    // We precompute this one to save time in the simulation.
    double slope_term; // tan((90.0 - theta) * PI / 180.0);

    // We precompute this one to save time in the simulation.
    double geo_denom;
    // geo_denom = length_m * (slope_term + width_m);


    bool use_reservoir_curve; // If true, we use the reservoir curves for calculating the filling and the masl.
    double res_curve_masl[MAX_NR_POINTS_CURVE];
    double res_curve_Mm3[MAX_NR_POINTS_CURVE];
    size_t nr_points_res_curve;

    bool use_overflow_curve; // If true, we use the overflow curve for calculating the overflow.
    double ovefl_curve_masl[MAX_NR_POINTS_CURVE];
    double ovefl_curve_m3s[MAX_NR_POINTS_CURVE];
    size_t nr_points_ovefl_curve;

    double minQ_hatch, maxQ_hatch, hatch_masl;

    ArrayCurve ac_res_masl_2_Mm3;
    ArrayCurve ac_res_Mm3_2_masl;
    ArrayCurve ac_ovefl_masl_2_m3s;
    ArrayCurve ac_ovefl_m3s_2_masl;

    // VIRTUAL FUNCTIONS USED IN RESERVOIR/CHANNEL/PSTATION
    int ReadNodeData(string filename);
    int ReadStateFile(string filename);
    int Simulate(size_t t);
    int initArrayCurves(void);
    //int CheckWaterBalance(void);
    int CheckWaterBalance(class Herss *herss_obj);
    int GetStartWater(void);
    int WriteStateFile(FILE *fp);

    // Functions used only in Reservoir
    void InitReservoir(void);
    double CalcOverflow();   // Mm3
    double GetStartWater_Mm3(void);
    double GetEndWater_Mm3(void);
    int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node
    double GetTunnelFLow(size_t t); // Used for reservoirs connected to a powerstation.
    double GetReservoirFraction(size_t t);

    // We use this to check if the reservoir level is valid.
    void ValidateReservoirLevelMm3(size_t t, double level_Mm3);

    // We check if the settings for the reservoir are valid.
    void ValidateReservoirSettings();


    double calcResVolume(double masl);  // Returns Mm3
    double calcResMasl(double Mm3);     // Returns masl



};

#endif
