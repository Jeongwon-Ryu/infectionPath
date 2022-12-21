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
    int Index; //환자 index 
    int age; // 나이 
    int inf_detected_time; // 감염 시점 
    int place_t[N_HISTORY]; //장소 이동 경로, enum을 이용했기 때문에 정수로 저장됨 
} ifs_ele_t; // 구조체 이름 

static ifs_ele_t ifsarray[20];
static int ifs_cnt; 

int ifctele_getAge(void* obj) {
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;

    return (strPtr->age); // 나이 반환 
}


char* ifctele_getPlaceName(int placeIndex) {
    return countryName[placeIndex]; // 해당 번호의 장소이름을 반환 
}

//int는 정수, unsigned int는 양수와 0만 표현 가능
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]) {
    int i;
    ifs_ele_t* strPtr = NULL;
    strPtr = (ifs_ele_t*)malloc(sizeof(ifs_ele_t) * 1); // 구조체를 포인터로 선언하고 메모리도 할당 
    // 포인터에서 대입되어 있는 값을 접근하려면 ->로 접근 
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
//환자 정보 
void ifctele_printElement(void* obj){
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;
    printf("------------------------------------------------------------------------------------\n");
    printf("Patient index : %i\n", strPtr->Index);
    printf("Patient Age : %i\n", strPtr->age);
    printf("Detected time : %i\n", strPtr->inf_detected_time);
    printf("Path History :");
    
    int i;
	//Path History 
	for (i = 0; i < N_HISTORY; i++) // 이동 장소와 시점 출력, i < N_HISTORY까지 반복 
    {
        printf("%s(%i)", ifctele_getPlaceName(ifctele_getHistPlaceIndex(obj, i)), strPtr->inf_detected_time-(N_HISTORY-1));
        
        if (i >= 0 && i < N_HISTORY-1) // 앞의 네 개의 장소는 ->와 함께 출력  
		    printf("-> ");
		    strPtr->inf_detected_time++; //detected_time +1씩 더함 Ex. (1->2->3->4->5) 이런 식으로 
		
    }
    printf("\n");
    printf("-------------------------------------------------------------------------------\n\n");

}

int ifctele_getHistPlaceIndex(void* obj, int index) {
    int i;
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;

    return strPtr->place_t[index]; //해당 index의 place_t 반환 
}

unsigned int ifctele_getinfestedTime(void* obj) {  
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;


    return ((strPtr->inf_detected_time)); //infector detected time 반환 
}

int trackInfester(int patient_no, int* detected_time, int* place) { //감염자의 경로와 전파자의 경로를 추적해서 최초 전파자를 찾음 

    int row = 0; //행 
    int col = 0; //열 
    int num = 0;
    int i, j;
	 
    for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len()까지 반복 
    {
        if (patient_no == i) 
            continue;
        else
        {
            for (j = 0; j < N_HISTORY - 2; j++) 
            {
                if (detected_time[patient_no] - (N_HISTORY - 1 - j) == detected_time[i]) // 입력받은 patient_no의 경로의 장소와 시점이 전파자의 detected_time[i]와 같으면 
                {
                    if (place[patient_no * N_HISTORY + j] == place[i * N_HISTORY + 4])
                    {
                        printf("-->[Tracking] patient %i is infected by %i (time : %i, place : %s)\n", patient_no, i, detected_time[patient_no] - (N_HISTORY - 1 - j), ifctele_getPlaceName(place[patient_no * N_HISTORY + j]));
                        return i;
                    }
                }
                else if (detected_time[patient_no] - (N_HISTORY - 1 - j) == detected_time[i] - 1) // 입력받은 patient_no의 경로의 장소와 시점이 전파자의 detected_time[i]-1 과 같으면 
                {
                    if (place[patient_no * N_HISTORY + j] == place[i * N_HISTORY + 3])
                    {
                        printf("-->[Tracking] patient %i is infected by %i (time : %i, place : %s)\n", patient_no, i, detected_time[patient_no] - (N_HISTORY - 1 - j), ifctele_getPlaceName(place[patient_no * N_HISTORY + j]));
                        return i;
                    }
                }
                else
                    num++;
            }
        }
    }

    if (num == (ifctdb_len() - 1) * (N_HISTORY - 2)) 

        return patient_no;

}
