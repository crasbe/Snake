#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "snake.h"

#define namelength 20           // max length of playername
#define scorelistlength 1000    //max length of scorelist

#define MAX 25 // maximum line length


//struct properties* props; //ekeligg
struct sScore{
    char name[namelength];
    int punkte;
};

void sort(struct sScore *highscorearray, int length){          //not tested, same as original code(score_e.c)
    struct sScore tmp;
    for (int i = 0; i < length - 1; ++i){

        for (int j = 0; j < length - i - 1; ++j){
            if (highscorearray[j].punkte < highscorearray[j + 1].punkte){
                tmp = highscorearray[j];
                highscorearray[j] = highscorearray[j+1];
                highscorearray[j+1] = tmp;
            }
        }
    }
}

void writeData(struct sScore *highscorearray, int length, FILE* highscorefile){      //works well
    //FILE *Fhighscore;
    //Fhighscore = fopen("highscore_l.txt", "w");      //path is incorrect
    for (int i = 0;i < length;i++){
        fprintf(highscorefile,"%i:",highscorearray[i].punkte);
        fprintf(highscorefile,"%s \n",highscorearray[i].name);
    }
    
    rewind(highscorefile);
}

void readData(struct sScore *highscorearray, FILE *highscorefile){       //the problem ist to read the data

    //FILE* Fhighscore;
    char line[MAX];
    int count = 0;

    //Fhighscore = fopen("highscore_l.txt", "r");      //path is incorrect

    while (fgets(line, MAX, highscorefile) !=0){
        //fscanf(highscorefile,"%i:%s",&highscorearray[count].punkte,highscorearray[count].name);
        sscanf(line, "%i:%s",&highscorearray[count].punkte, highscorearray[count].name);
        count++;
    }
    //fclose(Fhighscore);
    //for (int i=0;i<length;i++){                                                 //tool to test the funktion
    //    printf("%s %d \n",highscorearray[i].name,highscorearray[i].punkte);
    //}
    
    rewind(highscorefile); // back to start
    
  }


int countScores(FILE *highscorefile){              //works well

    //FILE* Fhighscore;

    int count = 0;
    char line[scorelistlength];


    //Fhighscore = fopen("highscore_l.txt", "r");      //path is incorrect

    if (!highscorefile){
        printf("Could not open highscore \n" );
        return -1;
    }

    while (fgets( line, scorelistlength, highscorefile ) != 0){
        count++;
    }

    printf("%i\n",count);           // only for testing
    
    rewind(highscorefile); // put curser back to start
    
    return count;
}

void writenewscore(struct sScore *highscorearray,char *Name, int length, int newscore){   // dont know couse of problems with the r/w functions
    highscorearray[length].punkte = newscore;
    strcpy(highscorearray[length].name,Name);

    sort(highscorearray, length+1);
}

//int main(int argc, const char * argv[]) {
int score(struct properties* props) {
	FILE *highscorefile = fopen("highscore_l.txt", "r+");
	
	
    char Name[namelength];
    int length;
    //props = (struct properties*)malloc(sizeof(struct properties));
    props->score = 20;
    printf("Enter your name:  ");
    scanf("%s",Name);

    length = countScores(highscorefile);
    if (length < 0){        // if the file could not be open end funktion
        return -1;
    }

    //struct sScore highscorearray[length+2];
    struct sScore* highscorearray;
    highscorearray = (struct sScore*)malloc((length+1)*sizeof(struct sScore));
    
    readData(highscorearray, highscorefile);
    writenewscore(highscorearray, Name,length, props->score);
    writeData(highscorearray, length+1, highscorefile);

    for (int i=0;i<length+1;i++){                                                 //tool to test the funktion
		printf("%s %d \n",highscorearray[i].name,highscorearray[i].punkte);
    }
   
   return -1;

}
