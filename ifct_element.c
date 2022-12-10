//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE + 1][MAX_PLACENAME] =
{ "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

typedef struct ifs_ele {
    int Index;
    int age;
    int inf_detected_time;
    int place_t[N_HISTORY];

} ifs_ele_t;

static ifs_ele_t ifsarray[20];
static int ifs_cnt; 

int ifctele_getAge(void* obj) {
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;

    return (strPtr->age);
}


char* ifctele_getPlaceName(int placeIndex) {

    return countryName[placeIndex];
}

void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]) {
    int i;
    ifs_ele_t* strPtr = NULL;
    strPtr = (ifs_ele_t*)malloc(sizeof(ifs_ele_t) * 1);

    strPtr->Index = index;
    strPtr->age = age;
    strPtr->inf_detected_time = detected_time;

    //ifsarray[ifs_cnt].Index = index;
    //ifsarray[ifs_cnt].age = age;
    //ifsarray[ifs_cnt].inf_detected_time = detected_time;
    for (i = 0; i < N_HISTORY; i++)
        strPtr->place_t[i] = history_place[i];
        
    return strPtr;
}

void ifctele_printElement(void* obj) {
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;
    printf("------------------------------------------------------------------------------------\n");
    printf("Patient index : %i\n", strPtr->Index);
    printf("Age : %i\n", strPtr->age);
    printf("Detected time : %i\n", strPtr->inf_detected_time);
    printf("Path History :");
    ifctele_getHistPlaceIndex(obj, strPtr->Index);
    printf("\n");

}

int ifctele_getHistPlaceIndex(void* obj, int index) {
    int i;
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;

    for (i = 0; i < 5; i++) {
        printf("%s (%d) ", ifctele_getPlaceName(strPtr->place_t[i]), ifctele_getinfestedTime(obj) - (4 - i));
        printf("%s", ifctele_getPlaceName(strPtr->place_t[i]));
        if (i != 4)
            printf("-> ");
    }
}

unsigned int ifctele_getinfestedTime(void* obj) {
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;


    return ((strPtr->inf_detected_time));
}
