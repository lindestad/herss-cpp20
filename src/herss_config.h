/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_config.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_CONFIG_H
#define _HERSS_CONFIG_H

#include "herss_common.h"

//-----------------------------------------------------------------------
class TopologyParser {

private:
    std::vector<std::string> lines;
    // size_t currentLine = 0;
    std::string trim(const std::string& str);

public:

    bool loadFile(const std::string& filename);
    size_t getLineCount() const { return lines.size(); }

    std::string getLine(size_t index) {
        if (index < lines.size()) {
            return lines[index];
        } else {
            throw std::out_of_range("Index out of range in TopologyParser::getLine");
        }
    }
};
//----------------------------------------------------------

// We accept the following formats:
// Supported date formats
// yyyy-mm-dd
// yyyy-mm-dd-hh
// yyyymmdd
// yyyymmddhh

class Xtime {
public:
    Xtime();
    ~Xtime();
    static bool isValid(std::string &);
    static void getNowString(std::string &);

    bool setDate(string str_date);  // Returns true if date format is OK.
    void getDateString(char *str_buffer);
    string returnDateString();
    void setDate(int yyyy, int mm, int day, int hour, int min, int sec);
    void printDate();
    int changeDate(int dt);
    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMin();
    int getSec();
    int getJulianDay();
    time_t getEpoch();
    int dateDiff(Xtime *, int);
    void print(FILE *f, char *format);   // Write the current time to file descriptor f on given format
    string currentDateTime();
    bool isValid(int, int, int, int, int, int);

private:
    struct tm my_tm;
    time_t epoch;       /* Essentially a pointer to an integer value holding EPOCH seconds.
                            After 2038 this may be a problem. */

    static int isLeapYear(int);
};
//----------------------------------------------------------

// Simple time class
// See Kernighan and Ritchie page 298, ISBN 82-518-2705-1, Norwegian edition.
// Note that this may be affected by the Y2038 problem.
class DateTime {
public:
    DateTime(){};
    ~DateTime(){};
    void setDate(int year, int month, int day, int hour, int min, int sec) {
        mytm.tm_sec   = sec;            // 0 to 59
        mytm.tm_min   = min;            // 0 to 59
        mytm.tm_hour  = hour;           // 0 to 23
        mytm.tm_mday  = day;            // 1 to 31
        mytm.tm_mon   = month-1;        // 0 to 11
        mytm.tm_year  = year-1900;      // year-1900
        mytm.tm_isdst = 0;              // 0 to 1, daylight saving or not.
        epoch = timegm(&mytm);
        gmtime_r(&epoch, &mytm);
    }
    time_t getEpoch(){ return epoch;};

private:
    struct tm mytm;
    time_t epoch;       // Essentially a pointer to an integer value holding EPOCH seconds. After 2038 this may be a problem.
};

//////////////////////////////////////////////////
// NAMING OF VARIABLES
// On many variables we specify the units they are in.
// We use the following ending on the variables to indicate unit
// Euro     Euros
// masl     meters above sea level
// MWh      Mega Watt Hours
// Mm3      Million kubic meters
// m3s      square meters pr second
// MW       Mega Watt
// fr       fraction, usually between zero and one. [0,1], but may be slightly above 1 or under 0.
//////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
class Line {
public:
    Line();
    ~Line();
    string extractNextElementFromLine(string* line);
    string extractLastElementFromLine(string* line);
    int calcNrCols(string* line);     // Calculates how many columns (words/tokens) there are in the string
    int checkDigit(string line);      // Checks if the string contains digits.
    int removeWhites(string* line);
};
///////////////////////////////////////////////////////////////////////////////////////////
class GlobalConfig {
public:
    GlobalConfig();
    ~GlobalConfig();

    NodeType nodetypes[MAX_NR_NODES];  // We keep track of which nodetype each index 0,1,2,3 etc is.
    string globalfile;
    string topologyfile;
    string actionsfile;
    string pricefile;
    string outputfile;
    string inflowfile;
    string systemname;
    string start_statefile;  // Reservoir levels and water storage in channels (how to value the water in channels?).
    string out_statefile;
    string outputdir;
    string inputdir;

    TopologyParser topoparser;

    bool found_topologyfilename;
    bool found_actionsfilename;
    bool found_pricefilename;
    bool found_inflowfilename;
    bool found_systemname;
    bool found_start_statefilename;
    bool found_outputfilename;
    bool found_dt;
    bool write_nodefiles;
    bool printglobalinfo;
    bool printeconomicinfo;
    bool use_reservoir_curve;
    bool use_reservoir_geometry;

    string logfilename;
    size_t nr_nodes;
    size_t nr_pstations;
    size_t nr_reservoirs;
    size_t nr_channels;
    size_t dt;     // Delta time step in seconds
    size_t stps;   // Nr of time steps in the simulation
    size_t dt_last;

    double discount_rate;  // DISCOUNT_RATE 0.05
    double discount_factor;

    size_t actions_idnrs[MAX_NR_NODES];  // We save the idnrs pointing to nodes with actions (actions inputfile).
    size_t n_action_nodes;  // Number of nodes were we need to set actions. Could be at PSTATION or RESERVOIRS (hatch_release)

    // Number of nodes were we need to set actions.
    // Could be at PSTATION or RESERVOIRS (hatch_release)
    // We calculate this from the topology file and compare it when reading actions.
    size_t n_action_nodes_from_topology;

    size_t inflows_idnrs[MAX_NR_NODES];  // We save the idnrs pointing to nodes with inflows
    size_t n_inflow_nodes;  // Number of nodes were we need to set the inflow (RESERVOIRS)

    void DiagnoseActionFile(); // We read the header and find number of action nodes and their indexes.
    void readGlobalFile();              // Reads the global file
    void SetDirectoriesAndFilenames();
    void printGlobalInfo();
    void Diagnose();
    void DiagnoseTopologyFile();

    void checkNrSteps();  // Checks number of timesteps in the pricefile
};

#endif
