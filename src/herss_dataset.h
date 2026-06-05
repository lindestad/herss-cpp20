/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_dataset.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_DATASET_H
#define _HERSS_DATASET_H

#include "herss_config.h"

///////////////////////////////////////////////////////////////////////////////////////////
class Dataset {
public:
    Dataset(GlobalConfig *gconfig);
    ~Dataset();
    size_t stps;               // Number of timesteps used.
    size_t nr_nodes;           // We allocate one inflow and action series per node. Not used in all of them, but makes it easier.
    GlobalConfig *gc;
    double *price;          // We assume all nodes located in same price area. So we need only one price series.
    double restprice;
    double **inflow;        // One series for each node. We can point to these series from other objects.
    double **action;        // One series for each node. Could change in the future.
    int *year;
    int *month;
    int *day;
    int *hour;
    map<string, int> datestring2idx; // Map between string yyyymmdd and an index to the arrays.
    map<int, string> idx2datestring; // Map between index and a date string yyyymmdd.
    string str_startdate;                            // Startdate of data
    string str_enddate;                              // End date of data
    std::vector<std::string> action_colnames;
    std::vector<int> delta_t;

    void readPricefile();
    void readInflowFile();
    void readActionsFile();
    void readAllData();
    void multi_temporal_resolution();
    int getDeltaT(size_t timestep);  // Get delta_t for a specific timestep

private:
    std::vector<double> price_storage;
    std::vector<std::vector<double>> inflow_storage;
    std::vector<std::vector<double>> action_storage;
    std::vector<double*> inflow_rows;
    std::vector<double*> action_rows;
    std::vector<int> year_storage;
    std::vector<int> month_storage;
    std::vector<int> day_storage;
    std::vector<int> hour_storage;
};

#endif
