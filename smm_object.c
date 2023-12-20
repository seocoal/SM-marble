#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "����",
       "�Ĵ�",
       "�����",
       "��",
       "����Ƿ��̵�",
       "��������",
       "�����ð�"
};


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


char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}


char* smmObj_getGradeName(smmObjGrade_e grade){
      
	  return (char*)smmGradeName[grade];
}





//1. ����ü�� ���� ���� 
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_t objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;




//2. ����ü�� �迭 ���� ���� 
//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;



//3. ���� �Լ� ���� 
//object generation

void* smmObj_genObject(char* name, smmObjType_t objType, int type, int credit, int energy, smmObjGrade_e grade)
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



//3. ���� �Լ� ���� 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

//3. ���� �Լ� ���� 
int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->type;
}

int smmObj_getNodeCredit(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->credit;
}

int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->energy;
}

int smmObj_getNodeGrade(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->grade;
}



