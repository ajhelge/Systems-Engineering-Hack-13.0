/**
 * Author:    Alec Helgeson
 * Date:      12/7/2022
 * Purpose:   Generates a report on the list of given airports after comparing different elements of each airport.
 *            (To include the type of airport, its lattitude/longitude, its name, etc.)
*/

#include <stdlib.h>
#include <stdio.h>
#include "airport.h"

int main(void){
    int numAirports = 7;
    Airport *airportList = malloc(sizeof(Airport) * numAirports);

    Airport *DFW = createAirport("DFW", "large_airport", "Dallas Fort Worth International Airport", 32.8972331,
                                -97.0376947, 606.4, "Dallas", "US");
    Airport *FAR = createAirport("FAR", "Comercial/Military", "Hector International Airport", 46.9206389,
                                -96.8157500, 901.3, "Fargo", "US");
    Airport *PNS = createAirport("PNS", "Comercial", "Pensacola International Airport", 30.4734167,
                                -87.1866111, 120.9, "Pensacola", "US");
    Airport *OMA = createAirport("OMA", "Comercial", "Eppley Airfield", 41.3, -95.895, 984, "Omaha", "US");

    Airport *RMS = createAirport("RMS", "Military", "Ramstein Air Base", 49.436833, 7.600333, 781, "Rhineland", "DE");

    Airport *LAX = createAirport("LAX", "Comercial", "Los Angeles International Airport", 33.9425222, -118.4071611,
                                128, "Los Angeles", "US");
    Airport *JFK = createAirport("JFK", "Comercial", "John F. Kennedy International Airport", 40.639722, -73.778889,
                                13, "New York", "US");

    airportList[0] = *FAR;
    airportList[1] = *DFW;
    airportList[2] = *PNS;
    airportList[3] = *OMA;
    airportList[4] = *RMS;
    airportList[5] = *LAX;
    airportList[6] = *JFK;

    generateReports(airportList, numAirports);
    return 0;
}