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
    int Index; //ȯ�� index 
    int age; // ���� 
    int inf_detected_time; // ���� ���� 
    int place_t[N_HISTORY]; //��� �̵� ���, enum�� �̿��߱� ������ ������ ����� 
} ifs_ele_t; // ����ü �̸� 

static ifs_ele_t ifsarray[20];
static int ifs_cnt; 

int ifctele_getAge(void* obj) {
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;

    return (strPtr->age); // ���� ��ȯ 
}


char* ifctele_getPlaceName(int placeIndex) {
    return countryName[placeIndex]; // �ش� ��ȣ�� ����̸��� ��ȯ 
}

//int�� ����, unsigned int�� ����� 0�� ǥ�� ����
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]) {
    int i;
    ifs_ele_t* strPtr = NULL;
    strPtr = (ifs_ele_t*)malloc(sizeof(ifs_ele_t) * 1); // ����ü�� �����ͷ� �����ϰ� �޸𸮵� �Ҵ� 
    // �����Ϳ��� ���ԵǾ� �ִ� ���� �����Ϸ��� ->�� ���� 
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
//ȯ�� ���� 
void ifctele_printElement(void* obj){
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;
    printf("------------------------------------------------------------------------------------\n");
    printf("Patient index : %i\n", strPtr->Index);
    printf("Patient Age : %i\n", strPtr->age);
    printf("Detected time : %i\n", strPtr->inf_detected_time);
    printf("Path History :");
    
    int i;
	//Path History 
	for (i = 0; i < N_HISTORY; i++) // �̵� ��ҿ� ���� ���, i < N_HISTORY���� �ݺ� 
    {
        printf("%s(%i)", ifctele_getPlaceName(ifctele_getHistPlaceIndex(obj, i)), strPtr->inf_detected_time-(N_HISTORY-1));
        
        if (i >= 0 && i < N_HISTORY-1) // ���� �� ���� ��Ҵ� ->�� �Բ� ���  
		    printf("-> ");
		    strPtr->inf_detected_time++; //detected_time +1�� ���� Ex. (1->2->3->4->5) �̷� ������ 
		
    }
    printf("\n");
    printf("-------------------------------------------------------------------------------\n\n");

}

int ifctele_getHistPlaceIndex(void* obj, int index) {
    int i;
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;

    return strPtr->place_t[index]; //�ش� index�� place_t ��ȯ 
}

unsigned int ifctele_getinfestedTime(void* obj) {  
    ifs_ele_t* strPtr = (ifs_ele_t*)obj;


    return ((strPtr->inf_detected_time)); //infector detected time ��ȯ 
}

int trackInfester(int patient_no, int* detected_time, int* place) { //�������� ��ο� �������� ��θ� �����ؼ� ���� �����ڸ� ã�� 

    int row = 0; //�� 
    int col = 0; //�� 
    int num = 0;
    int i, j;
	 
    for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len()���� �ݺ� 
    {
        if (patient_no == i) 
            continue;
        else
        {
            for (j = 0; j < N_HISTORY - 2; j++) 
            {
                if (detected_time[patient_no] - (N_HISTORY - 1 - j) == detected_time[i]) // �Է¹��� patient_no�� ����� ��ҿ� ������ �������� detected_time[i]�� ������ 
                {
                    if (place[patient_no * N_HISTORY + j] == place[i * N_HISTORY + 4])
                    {
                        printf("-->[Tracking] patient %i is infected by %i (time : %i, place : %s)\n", patient_no, i, detected_time[patient_no] - (N_HISTORY - 1 - j), ifctele_getPlaceName(place[patient_no * N_HISTORY + j]));
                        return i;
                    }
                }
                else if (detected_time[patient_no] - (N_HISTORY - 1 - j) == detected_time[i] - 1) // �Է¹��� patient_no�� ����� ��ҿ� ������ �������� detected_time[i]-1 �� ������ 
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
