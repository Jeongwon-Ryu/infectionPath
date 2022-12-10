//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int* detected_time, int* place);
int main(int argc, const char* argv[]) {

    int menu_selection;
    void* ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    int pNum;
    char place[100];
    int min_age, max_age;
    int patient_track;
    int i, j;
    int temp1[10000], temp2[10000], val, val2;

    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    /*
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }*/

    fp = fopen("patientInfo_sample.txt", "r");
    //fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }

    //1-2. loading each patient informations
    while (3 == (fscanf(fp, "%d %d %d", &pIndex, &age, &time))) {
        for (i = 0; i < 5; i++) {
            fscanf(fp, "%d", &placeHist[i]);
        }
        ifct_element = ifctele_genElement(pIndex, age, time, placeHist);
        ifctdb_addTail(ifct_element);

        printf("%i 번째 환자 감염 경로 :", pIndex);
        for (j = 0; j < 5; j++) {
            printf("%s  ", ifctele_getPlaceName(placeHist[j]));
        }
        printf("\n");
    }

    //1-3. FILE pointer close
    fclose(fp);

    do {
        //printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("\n1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");

        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);

        switch (menu_selection)
        {
        case MENU_EXIT:
            printf("Exiting the program... Bye bye.\n");
            break;

        case MENU_PATIENT:
            printf("Patient index : ");
            scanf("%d", &pNum);
            if (pNum <= pIndex) {
                ifctele_printElement(ifctdb_getData(pNum));
            }
            break;

        case MENU_PLACE:
            printf("Place Name : ");
            scanf("%s", &place);
            for (i = 0; i < pIndex; i++)
            {
                if (strcmp(place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY - 1))) == 0)
                    ifctele_printElement(ifctdb_getData(i));
            }
            if (ifctdb_len()==0)
            {
                printf("There are %d patients detected in %d", pIndex, placeHist[i]);
            }
            break;

        case MENU_AGE:
            printf("minimal age : "); //최소나이 변수를 min_age 
            scanf("%d", &min_age);

            printf("maximal age : "); //최고나이 변수를 max_age 
            scanf("%d", &max_age);

            for (i = 0; i < ifctdb_len(); i++)
            {
                if (min_age <= ifctele_getAge(ifctdb_getData(i)) && max_age >= ifctele_getAge(ifctdb_getData(i)))
                    ifctele_printElement(ifctdb_getData(i));
            }
            break;

        case MENU_TRACK:
            printf("Patient index : ");
            scanf("%d", &patient_track);
            for (i = 0; i < ifctdb_len(); i++)
            {
                temp1[i] = ifctele_getinfestedTime(ifctdb_getData(i));
            }

            for (i = 0; i < ifctdb_len(); i++)
            {
                for (j = 0; j < N_HISTORY; j++)
                {
                    temp2[i * N_HISTORY + j] = ifctele_getHistPlaceIndex(ifctdb_getData(i), j);

                }
            }

            val = trackInfester(pIndex, temp1, temp2);

            if (pIndex < 0 || pIndex >= ifctdb_len())
                printf("Patient index error\n");

            else if (pIndex != val)
            {
                do {
                    val2 = trackInfester(val, temp1, temp2);
                    val = val2;
                } while (val2 != val);

                printf("The first infector of %i is %i\n", pIndex, val2);
            }

            else
            {
                printf("\n");
                printf("The first infector is %i\n", pIndex);
            }
            break;

        default:
            printf("Menu index error\n", menu_selection);
            break;
        }

    } while (menu_selection != 0);


    return 0;
}

int trackInfester(int patient_no, int* detected_time, int* place)
{
    int row = 0;
    int col = 0;
    int num = 0;
    int i, j;
    
    for (i = 0; i < ifctdb_len(); i++)
    {
        if (patient_no == i)
            continue;
        else
        {
            for (j = 0; j < N_HISTORY - 2; j++)
            {
                if (detected_time[patient_no] - (N_HISTORY - 1 - j) == detected_time[i])
                {
                    if (place[patient_no * N_HISTORY + j] == place[i * N_HISTORY + 4])
                    {
                        printf("-->[Tracking] patient %i is infected by %i (time : %i, place : %s)\n", patient_no, i, detected_time[patient_no] - (N_HISTORY - 1 - j), ifctele_getPlaceName(place[patient_no * N_HISTORY + j]));
                        return i;
                    }
                }
                else if (detected_time[patient_no] - (N_HISTORY - 1 - j) == detected_time[i] - 1)
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
