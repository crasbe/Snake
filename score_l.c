#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "snake.h"
#include "sdlhelper.h"
#include "menu_c.h"

#define namelength 20           // max length of playername
#define scorelistlength 1000    //max length of scorelist
#define MAX 25                  // maximum line length
#define SCORESSHOWN 10

struct sScore{
    char name[namelength];
    int punkte;
};

void sort(struct sScore *highscorearray, int length){                                         //not tested, same as original code(score_e.c)
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

void writeData(struct sScore *highscorearray, int length, FILE* highscorefile){              //works well
    for (int i = 0;i < length;i++){
        fprintf(highscorefile,"%i:",highscorearray[i].punkte);
        fprintf(highscorefile,"%s \n",highscorearray[i].name);
    }

    rewind(highscorefile);
}

void readData(struct sScore *highscorearray, FILE *highscorefile){
    char line[MAX];
    int count = 0;

    while (fgets(line, MAX, highscorefile) !=0){
        sscanf(line, "%i:%s",&highscorearray[count].punkte, highscorearray[count].name);
        count++;
    }

    rewind(highscorefile); // back to start

  }

int countScores(FILE *highscorefile){                                                       //works well
    int count = 0;
    char line[scorelistlength];

    if (!highscorefile){
        printf("Could not open highscore \n" );
        return -2;
    }

    while (fgets( line, scorelistlength, highscorefile ) != 0){
        count++;
    }

    rewind(highscorefile); // put curser back to start

    return count;
}

void writenewscore(struct sScore *highscorearray,char *Name, int length, int newscore){     // dont know couse of problems with the r/w functions
    highscorearray[length -1].punkte = newscore;
    strcpy(highscorearray[length -1].name,Name);

    sort(highscorearray, length+1);
}

unsigned int drawscore( struct sScore *highscorearray) {

    for(int i=1;i<SCORESSHOWN+1;i++){
        printf("%i: %s \t %i\n",i, highscorearray[i-1].name, highscorearray[i-1].punkte);
    }

	return 0;
}


int score(struct properties* props) {
	FILE *highscorefile = fopen("highscore_l.txt", "r+");


    char Name[namelength];
    int length;
    props->score = 20;                                                                      // need the real score form game.c
    printf("Enter your name:  ");
    scanf("%s",Name);
    printf("\n");

    length = countScores(highscorefile) +1;
    if (length < 0){        // if the file could not opend
        printf("highscorefile isnt there");
        return -1;
    }

    struct sScore* highscorearray;
    highscorearray = (struct sScore*)malloc((length)*sizeof(struct sScore));

    readData(highscorearray, highscorefile);
    writenewscore(highscorearray, Name,length, props->score);
    writeData(highscorearray, length, highscorefile);
    drawscore(highscorearray);

    return 3;

}
