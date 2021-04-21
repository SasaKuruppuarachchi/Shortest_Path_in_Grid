/*
This is a program written for a grid following robot for finding shortest path between two predefined locations

program is written for a robot with Atmega2560 micro controller. 
This is used for the grid solver robot came the first runner-up in the SLRC 2016. Program performed reliably.

Credits - Sasa
*/

#include <EEPROM.h>

#include<stdio.h>

#define MAXNODES 16
#define ROW 4
#define COL 4
typedef struct node{
	int x;
	int y;
}node;

int grid[ROW][COL];
int costMatrix[MAXNODES][MAXNODES];
String pathMatrix[MAXNODES][MAXNODES];
int nodeMatrix[MAXNODES];
node unsettledNodes[36];
int unsettledNodeCounter = 0;
node settledNodes[36];
int settledNodeCounter = 0;
node startNode;
const int MAXVALUE = 9999;
void setup()
{
	Serial.begin(9600);
        Serial.println();        Serial.println();        Serial.println();
        Serial.println("****************************************");
        getGrid();
	createMap(); //create cost matrix
	printMap();	
	

}

void calculateCost(int i,int row, int col){
	if(row>=0&&col>=0&&row<ROW&&col<COL&&grid[row][col]==1){
		int j = row*ROW+col;
		costMatrix[i][j] = 1;
		pathMatrix[i][j] = String(String(i)+","+String(j));
					     }
	
}
void getGrid(){
	int k=0;
        for(int i=0;i<ROW;i++){
          for(int j=0;j<COL;j++){
            grid[i][j] = EEPROM.read(k++);
         }
        }
        
	
}

String trimString(String path){
  while(path.charAt(path.length()-1)!=',') path = path.substring(0,path.length()-1);
  path = path.substring(0,path.length()-1);
  return path;
}
void createMap(){
	int i=0,j=0;
	int nodeMatrixCounter = 0;	
	//getting the grid to an array
	for(i=0;i<ROW;i++){
		for(j=0;j<COL;j++){
			nodeMatrix[nodeMatrixCounter++] = grid[i][j];
				}
			}
        Serial.println("Nodes");
			
	for(i=0;i<nodeMatrixCounter;i++) {
              Serial.print(nodeMatrix[i]); Serial.print("\t");
                                      }
	Serial.println();Serial.println();
        //initializing the path matrix;
        for(i=0;i<MAXNODES;i++){
          for(j=0;j<MAXNODES;j++)pathMatrix[i][j] = String(-1);
        }
        
	//calculating the cost mar=trix
	for(i=0;i<MAXNODES;i++){
		for(j=0;j<MAXNODES;j++){
			costMatrix[i][j] = MAXVALUE;
					}		
		}

   
	int row,col;
	for(i=0;i<MAXNODES;i++){
		if(nodeMatrix[i]==1){
			row = i/ROW;
			col = i%ROW;
			calculateCost(i,row-1,col);
			calculateCost(i,row+1,col);
			calculateCost(i,row,col-1);
			calculateCost(i,row,col+1);
				    }
				}
	int k=0;
	for (i = 0; i < MAXNODES; i++) {
                for (j = 0; j < MAXNODES; j++) {
                        for (k = 0; k < MAXNODES; k++) {
                                if (costMatrix[j][k] > costMatrix[j][i] + costMatrix[i][k]&&k!=j) {
                                        costMatrix[j][k] = costMatrix[j][i] + costMatrix[i][k];
                                        if(pathMatrix[j][k]==-1) pathMatrix[j][k] = String(trimString(pathMatrix[j][i])+","+pathMatrix[i][k]);
                                        else {
                                          pathMatrix[j][k] = trimString(pathMatrix[j][k]);
                                          pathMatrix[j][k]+= String(pathMatrix[j][i]+","+pathMatrix[i][k]);
                                            }
                                }
                        }
                }
        }
}

void printMap(){
int i=0;
int j=0;
Serial.println("Cost Matrix");
for(i=0;i<MAXNODES;i++){
	for(j=0;j<MAXNODES;j++) {
          Serial.print(costMatrix[i][j]);
          Serial.print("\t");
                                }
	Serial.println();
}
	Serial.println();Serial.println();
Serial.println("Shortest Path Matrix");
for(i=0;i<MAXNODES;i++){
	for(j=0;j<MAXNODES;j++) {
          Serial.print(pathMatrix[i][j]);
          Serial.print("\t");
                                }
	Serial.println();
}
}


void loop(){}
