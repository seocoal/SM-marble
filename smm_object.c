#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100

// 노드의 이름을 저장하는 배열 
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "강의",
       "식당",
       "실험실",
       "집",
       "실험실로이동",
       "음식찬스",
       "축제시간"
};

// 학점을 저장하는 배열 
static char smmGradeName[9][MAX_CHARNAME]={
	    "A+"
	    "A0"
	    "A-"
	    "B+"
	    "B0"
	    "B-"
	    "C+"
	    "C0"
	    "C-"
};

// 노드 타입에 대한 이름 반환 
char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}

// 학점에 대한 이름 반환 
char* smmObj_getGradeName(smmObjGrade_e grade){
      
	  return (char*)smmGradeName[grade];
}





//1. 구조체의 형식 정의 
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;




//2. 구조체의 배열 변수 정의 
//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;



//3. 관련 함수 변경 

// 노드를 생성하는 함수 
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}



//노드의 이름을 반환하는 함수 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

//노드의 타입을 반환하는 함수 
int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->type;
}

//노드의 credit을 반환하는 함수 
int smmObj_getNodeCredit(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->credit;
}

//노드의 에너지를 반환 
int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->energy;
}

//노드의 학점을 반환 
int smmObj_getNodeGrade(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->grade;
}



