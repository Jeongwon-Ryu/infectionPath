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
    FILE* fp;                   // ���� ������ 
    int pIndex, age, time;      // ȯ�� �ε���, ����, �ð� 
    int placeHist[N_HISTORY];   // place history
    int pNum;                   // ȯ�� �ѹ� 
    char place[100];            // �ڷ����� �������� �迭, ���[����] 
    int min_age, max_age;       // �ּ� ����, �ִ� ���� 
    int patient_track;          // ȯ�� ��� 
    int i, j;
    int temp1[10000], temp2[10000], val, val2;  //temp1�� detected time, temp2�� place, val�� first infector, val2�� track infector 

    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    /*
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }*/

    fp = fopen(FILE_NAME, "r"); // ������ �����ϸ�  �б� �������� ��
    //fp = fopen(argv[1], "r");
    if (fp == NULL) // ������ �������� ������ ���� ó���ϰ� ���� 
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1; 
    }

    //1-2. loading each patient informations
    while (3 == (fscanf(fp, "%d %d %d", &pIndex, &age, &time))) //���� �� �������� �и��� ���ϰ� ������ �� �κ� (3��) ���� 
	{
        for (i = 0; i < N_HISTORY; i++) // ������ �� �κ�(3��) ������ ����(5���� ���)�� �迭�� ����, i < N_HISTORY�� ������ �ݺ��ؼ� �о� �� 
		{
            fscanf(fp, "%d", &placeHist[i]); 
        }
        ifct_element = ifctele_genElement(pIndex, age, time, placeHist); // ����ü�� ���� 
        ifctdb_addTail(ifct_element);

        printf("%i ��° ȯ�� ���� ��� :", pIndex);
        
        for (j = 0; j < N_HISTORY; j++)  // j < N_HISTORY �� ������ �ݺ��ؼ� �о� �� 
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
        case MENU_EXIT: // 0. ������  
            printf("Exiting the program... Bye bye.\n");
            
            break;

        case MENU_PATIENT: // 1. ȯ�� ����  
            printf("Patient index : ");
            scanf("%d", &pNum);
            
            if (pNum <= pIndex) //�Է¹��� ȯ�� ��ȣ�� ȯ�� �ε������� ������ �Է¹��� ȯ�� ���� �����ͼ� ��� 
			{
                ifctele_printElement(ifctdb_getData(pNum));
            }
            else
			printf("[ERROR] Your input for the patient index (%i) is wrong! Input must be 0 ~ %i.\n", pNum, pIndex);
            
            break;

        case MENU_PLACE: // 2. ��ҿ��� ������ ȯ�� ���� 
            printf("Place Name : ");
            scanf("%s", &place);
            
            int num = ifctdb_len(); 
            int countPatient = 0; // ���� ������ �����ϴ� ȯ�� ���� ��Ÿ���� ���� 
            

            for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len() �� ������ �ݺ�, �Է¹��� ��Ұ� ȯ���� ���� ��ҿ� ��ġ�ϸ� ���� ȯ�� ������ ���(��ҿ� ���� �Բ�)   
            {
                if (strcmp(place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY - 1))) == 0) 
				{
                    ifctele_printElement(ifctdb_getData(i));
                	countPatient++;    
                }     
                
            }
            printf("\nThere are %i patients detected in %s\n", countPatient, place); // ���� �����ϴ� ȯ�ڸ� �� ���

            break;

        case MENU_AGE: // 3. ���� ���� ���̿� �ִ� ȯ�� ���� 
            printf("minimal age : "); //�ּҳ��� ������ min_age 
            scanf("%d", &min_age);

            printf("maximal age : "); //�ְ��� ������ max_age 
            scanf("%d", &max_age);
            
            int nPatient = 0;  //���� ������ �����ϴ� ȯ�� ���� ��Ÿ���� ���� 
            
            for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len()�� ������ �ݺ�, ���� �� ������ ��� �����ϴ� ȯ�� ������ �����ͼ� ��� 
            {
                if (min_age <= ifctele_getAge(ifctdb_getData(i)) && max_age >= ifctele_getAge(ifctdb_getData(i)))
				{
                	ifctele_printElement(ifctdb_getData(i));
                	
                	nPatient++;	
            	}    
            	
   			}
   			printf("\nThere are %i patients whose age is between %i and %i.\n\n", nPatient , min_age, max_age);  // ���� �����ϴ� ȯ�ڸ� �� ���            				
            
			break;

        case MENU_TRACK: // 4. ���� ��� 
            printf("Patient index : ");
            scanf("%d", &pIndex);
            
            for (i = 0; i < ifctdb_len(); i++) // i < ifctdb_len()���� �ݺ�, ������ �������� ���� ������ temp1 �迭�� ����  
            {
                temp1[i] = ifctele_getinfestedTime(ifctdb_getData(i));
            }

            for (i = 0; i < ifctdb_len(); i++) 
            {
                for (j = 0; j < N_HISTORY; j++) // j < N_HISTORY���� �ݺ�, ������ �������� HistPlaceIndex�� temp2 �迭�� ���� 
                {
                    temp2[i * N_HISTORY + j] = ifctele_getHistPlaceIndex(ifctdb_getData(i), j);

                }
            }

            val = trackInfester(pIndex, temp1, temp2); // ���� �����ڸ� ��Ÿ���� ���� 

            if (pIndex < 0 || pIndex >= ifctdb_len()) // �Է� ���� ȯ�� index�� ���� ���� ���� ������ ���� ó�� 
                printf("[ERROR] Your input for the patient index (%i) is wrong! Input must be 0 ~ %i.\n", pIndex, ifctdb_len()-1);

            else if (pIndex != val) //pIndex�� ���� �����ڰ� �ƴ� ��� 
            {
                do 
				{
                    val2 = trackInfester(val, temp1, temp2); //��� ������ 
                    val = val2;
                } while (val2 != val); // val2�� val�� �ٸ� �� ���� 

                printf("The first infector of %i is %i\n", pIndex, val2); //pIndex�� ������ ��ȣ ��� 
            }

            else //pIndex�� ������������ �� 
            {
                printf("\n");
                printf("%i is the first infector!!!\n", pIndex);
            }
            
            break;

        default: // �߸��� �޴� ��ȣ�� �Է¹޾��� �� ���� ó�� 
            printf("[ERROR] Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
            
            break;
        }

    } while (menu_selection != 0); // 0. ������ ������ �ݺ� 


    return 0;
}
