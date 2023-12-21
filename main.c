#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"


//board configuration parameters(���� ���� ���� �Ű�������) 
static int board_nr; // ���� ��� ���� 
static int food_nr;  // ���� ��� ���� 
static int festival_nr; // festival ��� ���� 

static int player_nr; // �÷��̾��� ��  

// �÷��̾��� ����ü ���� 
typedef struct player{
	    int energy;  // ������ 
	    int position;  // ������ġ 
	    char name[MAX_CHARNAME];  // �̸� 
	    int accumCredit; // �������� 
	    int flag_graduate;  // ���� ���θ� flag 
}player_t;

static player_t*cur_player; // ���� �÷��̾��� ����ü�� �迭 
// static player_t cur_player[MAX_PLAYER]; 


#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PKAYER];
static char palyer_name[MAX_PLAYER][MAx_CHARNAME];
#endif 


//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif



void printGrades(int player) //player�� ���� ��� ��� 
{
	int i;
	void *gradePtr;
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
	}
}

void printPlayerStatus(void) //���� ���۽ÿ� ��� �÷��̾��� ���¸� ����ϴ� �Լ�  
{
     int i;
     
     printf("\n\n===============PLAYER STATUS===============\n");
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n", 
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
     printf("===============PLAYER STATUS===============\n\n\n");
     
}

void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name:", i); //��E���� ���硾�� 
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position
         //player_position[i] = 0;
         
         cur_player[i].position = 0;
         
         //set energy
         //player_energy[i] = initEnergy;
         
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0; // ��������flag 
     }
}


int rolldie(int player) // �ֻ��� ������ 
{
    char c;
    printf("Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player); //�������� 
#endif
    
    return (rand()%MAX_DIE + 1); //1~MAX_DIE������ ���� �������� ��� 
}


//action code when a player stays at a node
void actionNode(int player)
{
	
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );  
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr ); // ����� Ÿ�԰������� 
    char *name = smmObj_getNodeName( boardPtr ); // �ش� ����� �̸� �ҷ����� 
    void *gradePtr;
    char randomFoodCard;
    
    switch(type)
    {
    		
    		
        //case lecture
        case SMMNODE_TYPE_LECTURE:
        	
            (cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr )); // ������ ������ ���ϱ� 
            (cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr )); // ����� �������� �����ϱ� 
             
			char response;
			 
			 // ���� ���� ���ο� ���� ����
			printf("Are you going to join %s class? (Y=0/N=1): \n", smmObj_getNodeName( boardPtr ));
			scanf("%d", &response);
			
			// ������ �����Ѵٸ� 
			if(response == 0){
				
			 //grade generation
            gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit( boardPtr ), 0, (smmObjGrade_e)(rand() % (smmObjGrade_C0 + 1)) );
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
            
			printf("You join the lecture! \n"); 
			
			// ���� ��� 
            printGrades(player);
        }
            // ������ ���������ʴ´ٸ� drop �޼��� ��� 
            else 
			{
				printf("Drop the lecture.\n");
            }
            break;
            
        //case Restaurant   
        case SMMNODE_TYPE_RESTAURANT:
        	{
        		(cur_player[player].energy += smmObj_getNodeEnergy( boardPtr )); //����������� ������ ��� 
        		
        		printf("Here is the restaurant! Get %d energy! \n", smmObj_getNodeEnergy( boardPtr )); // ���� �������� ������� ��� 
        		
        		break;
            }
        
        //case Festival 
        case SMMNODE_TYPE_FESTIVAL:
    	{
			char randomFestCard=rand()%smmdb_len(LISTNO_FESTCARD); // ������ �����Ѵٸ�, �������� ī�带 ����� �̼����� 
    		printf("Participate to snow festival! \n ");
    		printf("MISSION: %s \n", smmObj_getNodeName(smmdb_getData(LISTNO_FESTCARD, randomFestCard)));
    		break;
    		
        }
        
        //case Laboratory 
        case SMMNODE_TYPE_LABORATORY:
        	printf("This is not a LABORATORY CLASS! \n"); //����ǿ� ������ ���, �׳� �������� 
        	break;
        
        //case Home
        case SMMNODE_TYPE_HOME:
        	{
			
        	(cur_player[player].energy += smmObj_getNodeEnergy( boardPtr )); //���� �����Ѵٸ�, ������ ���� 
        	break;
        	
			}
		
		//case Gotolab	
		case SMMNODE_TYPE_GOTOLAB:
			// cur_player[player].position= LABORATORY   //LABORATORY ���� �̵� 
			printf("You Move to the LABORATORY! \n");
			
			
		//case Foodchance	
		case SMMNODE_TYPE_FOODCHANCE:
		{
		    randomFoodCard=rand()%smmdb_len(LISTNO_FOODCARD); //�������� foodcard �̱� 
			printf("You got a food chance!\n");
			printf("You choose %d and get %d energy! \n", smmObj_getNodeName(smmdb_getData(LISTNO_FOODCARD, randomFoodCard)), smmObj_getNodeEnergy( boardPtr ));
			//���� foodcard�� �� ī���� �������� ���
			 
			break;
		}
        	
        	
            
        default:
            break;
    }
}


// ������ �������� ����ϴ� �Լ�  
void printTurn(int player){
	
	printf("Next turn is %s\n", cur_player[player].name);
} 


//make player go "step" on the board
void goForward(int player, int step)
{
    void *boardPtr;
    cur_player[player].position += step;
    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
     
    
    printf("%s's destination is %s !!\n", 
            cur_player[player].name, smmObj_getNodeName(boardPtr));
    		
}


// �������ɿ��θ� Ȯ���ϴ� �Լ�(GRADUATE CREDIT���� ū�� ���θ� Ȯ��) 
// ���� ���������ϴٸ� 1�� ����ϰ�, ���� ������ �Ұ��ϴٸ� 0�� ���

#if 0
void isGraduated(int player_nr){
	
	   if (accumCredit>=GRADUATE_CREDIT){
	     	printf("Player %d graduated! \n", turn);
		    return 1;
	    }
	    return 0;
    }
#endif



int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
        
    }
    fclose(fp);
    
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
 

    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy)==2) //read a food parameter set
    {
        //store the parameter set
        void *boardObj= smmObj_genObject(name, smmObjType_card , 0, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, boardObj);
        food_nr++;
        
    }
    fclose(fp);
    
    printf("Total number of food cards : %i\n", food_nr);
    
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name)==1) //read a festival card string
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_card,0,0,0,0);
        smmdb_addTail(LISTNO_FESTCARD, boardObj);
        festival_nr++;
        
    }
    fclose(fp);
    
    printf("Total number of festival cards : %i\n", festival_nr);
    

    

    
    //2. Player configuration ---------------------------------------------------------------------------------

    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    //player�� ���� ���� �Ҵ� 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    //player�� ������ ���� 
    generatePlayers(player_nr, initEnergy);
    
    
   
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //check if anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        
        //4-2. ������ �������� ���
        printTurn(turn);
        
        
        //4-3. die rolling (if not in experiment)
        die_result=rolldie(turn);
	    printf("The result is %d!\n", die_result);

        
        //4-3. ��ĭ�� �̵�+ go forward(���������� ����)
        goForward(turn, die_result);


		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        
        //4-5. check for game over condition
        #if 0
		isgraduated(turn);
		#endif
        
        
        //4-6. Move to next turn
        turn=(turn+1)%player_nr;
        
    }
    
    //Free allocated memory for player information 
    free(cur_player);
    printf("==========GRADUATED!==========");
    
    return 0;
    
}
