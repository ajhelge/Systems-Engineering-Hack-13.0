/**
 * Author: Alec Helgeson
 * Date: 11/10/2022
 * Purpose: To calculate the estimated travel time between an itinerary of airports from source to destination.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "airport.h"

/**
 * Program Functions
*/

Airport* createAirport(const char* gpsId, const char* type, const char* name, double latitude,
                        double longitude, int elevationFeet, const char* city, const char* countryAbbrv){

                            if((gpsId == NULL) || (type == NULL) || (name == NULL) || (city == NULL) || (countryAbbrv == NULL)){
                                return NULL;
                            }

                            Airport* airport = (Airport *) malloc(sizeof(Airport) * 1);
                            initAirport(airport, gpsId, type, name, latitude, longitude, elevationFeet, city, countryAbbrv);
                            return airport;
}

void initAirport(Airport* airport, const char* gpsId, const char* type, const char* name,
                 double latitude, double longitude, int elevationFeet, const char* city, const char* countryAbbrv){

                            airport->gpsId = (char *) malloc(sizeof(char) * (strlen(gpsId) + 1));
                            strcpy(airport->gpsId, gpsId);

                            airport->type = (char *) malloc(sizeof(char) * (strlen(type) + 1));
                            strcpy(airport->type, type);

                            airport->name = (char *) malloc(sizeof(char) * (strlen(name)) + 1);
                            strcpy(airport->name, name);

                            airport->latitude = latitude;

                            airport->longitude = longitude;

                            airport->elevationFeet = elevationFeet;

                            airport->city = (char *) malloc(sizeof(char) * (strlen(city) + 1));
                            strcpy(airport->city, city);

                            airport->countryAbbrv = (char *) malloc(sizeof(char) * (strlen(countryAbbrv) + 1));
                            strcpy(airport->countryAbbrv, countryAbbrv);
                            return;
}

char *airportToString(const Airport* a){
    if (a == NULL) {
        return NULL;
    }

    char tempBuffer[3000];
    sprintf(tempBuffer, ">>>%10s, %20s, %50s, %8.3lf, %8.3lf, %5d, %30s, %2s<<<", a->gpsId, a->type, a->name, a->latitude, a->longitude,
    a->elevationFeet, a->city, a->countryAbbrv);
    char *airportString = (char *)malloc(sizeof(char) * (strlen(tempBuffer) + 1));
    strcpy(airportString, tempBuffer);
    return airportString;
}

double getAirDistance(const Airport* origin, const Airport* destination){
    if((origin == NULL) || (destination == NULL)){
        return -100000;
    }
    else if((origin->latitude > 90) || (origin->latitude < (-90)) || (origin->longitude > 180) || (origin->longitude < (-180))){
        return -100000;
    }
    else if((destination->latitude > 90) || (destination->latitude < (-90)) || (destination->longitude > 180) || (destination->longitude < (-180))){
        return -100000;
    }
    double orgLat = origin->latitude, orgLong = origin->longitude, desLat = destination->latitude, desLong = destination->longitude;
    double orgLatRad = 0, orgLongRad = 0, desLatRad = 0, desLongRad = 0;
    double distance, delta, earthRadius = 6371;

    orgLatRad = (orgLat/180 * M_PI);
    orgLongRad = (orgLong/180 * M_PI);
    desLatRad = (desLat/180 * M_PI);
    desLongRad = (desLong/180 * M_PI);
    delta = desLongRad - orgLongRad;

    distance = acos(  sin(orgLatRad) * sin(desLatRad)  +  cos(orgLatRad) * cos(desLatRad) * cos(delta)  ) * earthRadius;
    return distance;
}

double getEstimatedTravelTime(const Airport* stops, int size, double aveKmsPerHour, double aveLayoverTimeHrs){
    if((stops == NULL) || (size < 2) || (aveKmsPerHour < 50) || (aveLayoverTimeHrs < 0)){
        return -1;
    }

    double estTravelTime = 0;
    double totalDistanceTraveled = 0;

    for(int src = 0; src < (size - 1); src++){
        totalDistanceTraveled += getAirDistance(&stops[src], &stops[src + 1]);
    }

    estTravelTime = (totalDistanceTraveled / aveKmsPerHour) + (aveLayoverTimeHrs * (size - 2));
    return estTravelTime;
}

void generateReports(Airport *airports, int n){

    printf("The Airport list's original order:\n");
    printAirports(airports, n);

    printf("\nSorted by gps ID:\n");
    qsort(airports, n, sizeof(Airport), cmpByGPSId);
    printAirports(airports, n);

    printf("\nSorted by Type:\n");
    qsort(airports, n, sizeof(Airport), cmpByType);
    printAirports(airports, n);

    printf("\nSorted by Name:\n");
    qsort(airports, n, sizeof(Airport), cmpByName);
    printAirports(airports, n);

    printf("\nSorted by Name (Descending Order):\n");
    qsort(airports, n, sizeof(Airport), cmpByNameDesc);
    printAirports(airports, n);

    printf("\nSorted by Country then City:\n");
    qsort(airports, n, sizeof(Airport), cmpByCountryCity);
    printAirports(airports, n);

    printf("\nSorted North to South:\n");
    qsort(airports, n, sizeof(Airport), cmpByLatitude);
    printAirports(airports, n);

    printf("\nSorted West to East:\n");
    qsort(airports, n, sizeof(Airport), cmpByLongitude);
    printAirports(airports, n);
    //not done
    printf("\nSorted by distance from Lincoln Airport:\n");
    printf("Here Lies Alec's self-confidence, as he did not figure out the Lincoln distance comparison on time...\n");

    qsort(airports, n, sizeof(Airport), cmpByLongitude);
    printf("\nWest to East median:\n   %s\n", airportToString(&airports[n / 2]));

    Airport NYkey;
    initAirport(&NYkey, "", "", "", 0, 0, 0, "New York", "US");
    Airport *NYairport = NULL;
    qsort(airports, n, sizeof(Airport), cmpByCountryCity);
    NYairport = bsearch(&NYkey, airports, n, sizeof(Airport), cmpByCountryCity);
    if(NYairport != NULL){
        printf("\nAn airport in New York City: \n");
        printAirports(NYairport, 1);
    }
    else{
        printf("\nNo New York airports found...\n");
    }

    Airport key;
    initAirport(&key, "", "large_airport", "", 0, 0, 0, "", "");
    Airport *typeAirport = NULL;
    qsort(airports, n, sizeof(Airport), cmpByType);
    typeAirport = bsearch(&key, airports, n, sizeof(Airport), cmpByType);
    if(typeAirport != NULL){
        printf("\nAn airport of type \"large_airport\": \n");
        printAirports(typeAirport, 1);
    }
    else{
        printf("\nNo large airports found...\n");
    }
}

void printAirports(Airport *airports, int n){
    for(int i = 0; i < n; i++){
        char *airportS = airportToString(&airports[i]);
        printf("%d: %s\n", (i + 1), airportS);
    }
}

/**
 * Comparators
*/

int cmpByGPSId(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    return strcmp(y->gpsId, z->gpsId);
}

int cmpByType(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    return strcmp(y->type, z->type);
}

int cmpByName(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    return strcmp(y->name, z->name);
}

int cmpByNameDesc(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    return -(strcmp(y->name, z->name));
}

int cmpByCountryCity(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    if((strcmp(y->countryAbbrv, z->countryAbbrv)) > 0){
        return 1;
    }
    else if((strcmp(y->countryAbbrv, z->countryAbbrv)) < 0){
        return -1;
    }
    else{
        if((strcmp(y->city, z->city)) > 0){
            return 1;
        }
        else if((strcmp(y->city, z->city)) < 0){
            return -1;
        }
        else{
            return 0;
        }
    }
}

int cmpByLatitude(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    if((y->latitude) < (z->latitude)){
        return 1;
    }
    else if(((y->latitude) > (z->latitude))){
        return 0;
    }
    else{
        return -1;
    }
}

int cmpByLongitude(const void* a, const void* b){
    const Airport *y = (const Airport *)a;
    const Airport *z = (const Airport *)b;
    if((y->longitude) > (z->longitude)){
        return 1;
    }
    else if(((y->longitude) < (z->longitude))){
        return 0;
    }
    else{
        return -1;
    }
}

int cmpByLincolnDistance(const void* a, const void* b){
    // const Airport *y = (const Airport *)a;
    // const Airport *z = (const Airport *)b;
    return 0;
}