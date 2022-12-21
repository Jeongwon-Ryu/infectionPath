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
#define FILE_NAME  "patientInfo_sample.txt"


int trackInfester(int patient_no, int* detected_time, int* place);
int main(int argc, const char* argv[]) {

    int menu_selection;
    void* ifct_element;
    FILE* fp;                   // 파일 포인터 
    int pIndex, age, time;      // 환자 인덱스, 나이, 시간 
    int placeHist[N_HISTORY];   // place history
    int pNum;                   // 환자 넘버 
    char place[100];            // 자료형이 문자형인 배열, 장소[갯수] 
    int min_age, max_age;       // 최소 나이, 최대 나이 
    int patient_track;          // 환자 경로 
    int i, j;
    int temp1[10000], temp2[10000], val, val2;  //temp1는 detected time, temp2는 place, val은 first infector, val2는 track infector 

    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    /*
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }*/

    fp = fopen(FILE_NAME, "r"); // 파일이 존재하면  읽기 전용으로 엶
    //fp = fopen(argv[1], "r");
    if (fp == NULL) // 파일이 존재하지 않으면 에러 처리하고 종료 
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1; 
    }

    //1-2. loading each patient informations
    while (3 == (fscanf(fp, "%d %d %d", &pIndex, &age, &time))) //변수 간 공백으로 분리된 파일과 파일의 앞 부분 (3개) 읽음 
	{
        for (i = 0; i < N_HISTORY; i++) // 파일의 앞 부분(3개) 이후의 정수(5개의 장소)를 배열에 저장, i < N_HISTORY일 때까지 반복해서 읽어 옴 
		{
            fscanf(fp, "%d", &placeHist[i]); 
        }
        ifct_element = ifctele_genElement(pIndex, age, time, placeHist); // 구조체에 넣음 
        ifctdb_addTail(ifct_element);

        printf("%i 번째 환자 감염 경로 :", pIndex);
        
        for (j = 0; j < N_HISTORY; j++)  // j < N_HISTORY 일 때까지 반복해서 읽어 옴 
		{
            printf("%s  ", ifctele_getPlaceName(placeHist[j]));
        }
        printf("\n");
    }

    //1-3. FILE pointer close
    fclose(fp); 

    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("\n1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");

        printf("Select a menu : ");
        scanf("%d", &menu_selection);
        fflush(stdin);

        switch (menu_selection)
        {
        case MENU_EXIT: // 0. 나가기  
            printf("Exiting the program... Bye bye.\n");
            
            break;

        case MENU_PATIENT: // 1. 환자 정보  
            printf("Patient index : ");
            scanf("%d", &pNum);
            
            if (pNum <= pIndex) //입력받은 환자 번호가 환자 인덱스보다 작으면 입력받은 환자 정보 가져와서 출력 
			{
                ifctele_printElement(ifctdb_getData(pNum));
            }
            else
			printf("[ERROR] Your input for the patient index (%i) is wrong! Input must be 0 ~ %i.\n", pNum, pIndex);
            
            break;

        case MENU_PLACE: // 2. 장소에서 감염된 환자 정보 
            printf("Place Name : ");
            scanf("%s", &place);
            
            int num = ifctdb_len(); 
            int countPatient = 0; // 다음 조건을 만족하는 환자 수를 나타내는 변수 
            

            for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len() 일 때까지 반복, 입력받은 장소가 환자의 감염 장소와 일치하면 관련 환자 정보를 출력(장소와 시점 함께)   
            {
                if (strcmp(place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY - 1))) == 0) 
				{
                    ifctele_printElement(ifctdb_getData(i));
                	countPatient++;    
                }     
                
            }
            printf("\nThere are %i patients detected in %s\n", countPatient, place); // 조건 만족하는 환자명 수 출력

            break;

        case MENU_AGE: // 3. 나이 범위 사이에 있는 환자 정보 
            printf("minimal age : "); //최소나이 변수를 min_age 
            scanf("%d", &min_age);

            printf("maximal age : "); //최고나이 변수를 max_age 
            scanf("%d", &max_age);
            
            int nPatient = 0;  //다음 조건을 만족하는 환자 수를 나타내는 변수 
            
            for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len()일 때까지 반복, 다음 두 조건을 모두 만족하는 환자 정보를 가져와서 출력 
            {
                if (min_age <= ifctele_getAge(ifctdb_getData(i)) && max_age >= ifctele_getAge(ifctdb_getData(i)))
				{
                	ifctele_printElement(ifctdb_getData(i));
                	
                	nPatient++;	
            	}    
            	
   			}
   			printf("\nThere are %i patients whose age is between %i and %i.\n\n", nPatient , min_age, max_age);  // 조건 만족하는 환자명 수 출력            				
            
			break;

        case MENU_TRACK: // 4. 감염 경로 
            printf("Patient index : ");
            scanf("%d", &pIndex);
            
            for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len()까지 반복, 접근한 데이터의 감염 시점을 temp1 배열에 저장  
            {
                temp1[i] = ifctele_getinfestedTime(ifctdb_getData(i));
            }

            for (i = 0; i < ifctdb_len(); i++) 
            {
                for (j = 0; j < N_HISTORY; j++) // j < N_HISTORY까지 반복, 접근한 데이터의 HistPlaceIndex를 temp2 배열에 저장 
                {
                    temp2[i * N_HISTORY + j] = ifctele_getHistPlaceIndex(ifctdb_getData(i), j);

                }
            }

            val = trackInfester(pIndex, temp1, temp2); // 최초 전파자를 나타내는 변수 

            if (pIndex < 0 || pIndex >= ifctdb_len()) // 입력 받은 환자 index가 범위 내에 있지 않으면 에러 처리 
                printf("[ERROR] Your input for the patient index (%i) is wrong! Input must be 0 ~ %i.\n", pIndex, ifctdb_len()-1);

            else if (pIndex != val) //pIndex가 최초 전파자가 아닐 경우 
            {
                do 
				{
                    val2 = trackInfester(val, temp1, temp2); //경로 전파자 
                    val = val2;
                } while (val2 != val); // val2가 val과 다를 때 까지 

                printf("The first infector of %i is %i\n", pIndex, val2); //pIndex와 전파자 번호 출력 
            }

            else //pIndex가 최초전파자일 때 
            {
                printf("\n");
                printf("%i is the first infector!!!\n", pIndex);
            }
            
            break;

        default: // 잘못된 메뉴 번호를 입력받았을 때 에러 처리 
            printf("[ERROR] Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
            
            break;
        }

    } while (menu_selection != 0); // 0. 나가기 전까지 반복 


    return 0;
}
