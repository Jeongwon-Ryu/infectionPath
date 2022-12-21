//
//  ifct_element.h
//  infection path
//
//  Created by Juyeop Kim on 2022/10/20.
//

#ifndef ifct_element_h
#define ifct_element_h

#define N_PLACE                     40
#define N_HISTORY                   5
#define MAX_PLACENAME               100

void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]); //gen patient Element
char* ifctele_getPlaceName(int placeIndex);           //get PlaceName which equals placeIndex
int ifctele_getHistPlaceIndex(void* obj, int index);  //get Place history
unsigned int ifctele_getinfestedTime(void* obj);      //get patient infested time
int ifctele_getAge(void* obj);                        //get age
void ifctele_printElement(void* obj);                 //print element
#endif /* ifct_element_h */
