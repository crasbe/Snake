#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "snake.h"

#define namelength 20           // max length of playername
#define scorelistlength 1000    //max length of scorelist


struct properties* props; //ekeligg
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

void writeData(struct sScore *highscorearray, int length){      //works well
    FILE *Fhighscore;
    Fhighscore = fopen("/home/arkantdos/Desktop/Snake/highscore_l.txt", "w");      //path is incorrect
    for (int i = 0;i < length;i++){
        fprintf(Fhighscore,"%i:",highscorearray[i].punkte);
        fprintf(Fhighscore,"%s \n",highscorearray[i].name);
    }
}

void readData(struct sScore *highscorearray, int length){       //the problem ist to read the data

    FILE* Fhighscore;
    char line[length];
    int count = 0;

    Fhighscore = fopen("/home/arkantdos/Desktop/Snake/highscore_l.txt", "w");      //path is incorrect

    while (fgets( line, length, Fhighscore ) !=0){
        fscanf(Fhighscore,"%i:%s",&highscorearray[count].punkte,highscorearray[count].name);         count++;
    }
    fclose(Fhighscore);
    for (int i=0;i<length;i++){                                                 //tool to test the funktion
        printf("%s %d \n",highscorearray[i].name,highscorearray[i].punkte);
    }
  }


int countScores(){              //works well

    FILE* Fhighscore;

    int count = 0;
    char line[scorelistlength];


    Fhighscore = fopen("/home/arkantdos/Desktop/Snake/highscore_l.txt", "r");      //path is incorrect

    if (!Fhighscore){
        printf("Could not open highscore \n" );
        return -1;
    }

    while (fgets( line, scorelistlength, Fhighscore ) != 0){
        count++;
    }

    printf("%i\n",count);           // only for testing
    return count ;
}

void writenewscore(struct sScore *highscorearray,char *Name, int length){   // dont know couse of problems with the r/w functions
    highscorearray[length+1].punkte = props->score;
    strcpy(highscorearray[length+1].name,Name);

    sort(highscorearray, length);
}

int main(int argc, const char * argv[]) {
    char Name[namelength];
    int length;
    props = (struct properties*)malloc(sizeof(struct properties));
    props->score = 20;
    printf("Enter your name:  ");
    scanf("%s",Name);

    length = countScores();
    if (length < 0){        // if the file could not be open end funktion
        return 0;
    }

    struct sScore highscorearray[length+1];
    readData(highscorearray,length);
    writenewscore(highscorearray, Name,length);
    writeData(highscorearray, length);

    //for (int i=0;i<length;i++){                                                 //tool to test the funktion
     //   printf("%s %d \n",highscorearray[i].name,highscorearray[i].punkte);
   // }

}
