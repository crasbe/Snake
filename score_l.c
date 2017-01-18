#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "snake.h"
struct properties* props; //ekeligg
struct sScore{
    char name[40];
    int punkte;
};

void sort(struct sScore *highscorearray, int length){          //not tested, same as original code(score_e.c)
    struct sScore tmp;
        for (int i = 0; i < length - 1; ++i)
        {

            for (int j = 0; j < length - i - 1; ++j)
            {
                if (highscorearray[j].punkte < highscorearray[j + 1].punkte)
                {
                    tmp = highscorearray[j];
                    highscorearray[j] = highscorearray[j+1];
                    highscorearray[j+1] = tmp;
                }
            }
        }
}

void writeData(struct sScore *highscorearray, int length){
    FILE *Fhighscore;
    Fhighscore = fopen("/Users/Evgenij/Documents/highscore/highscore/highscore_e.txt", "w");      //path is incorrect
    for (int i = 0;i < length;i++){
        fprintf(Fhighscore,"%d:",highscorearray[i].punkte);
        fprintf(Fhighscore,"%s \n",highscorearray[i].name);




    }
}

void readData(struct sScore *highscorearray, int length){
  {
  FILE* Fhighscore;
  char line[length];
  int count = 0;

  f = fopen("/Users/Evgenij/Documents/highscore/highscore/highscore_e.txt", "w");      //path is incorrect

  while (fgets( line, length, Fhighscore )){
    fscanf(Fhighscore, "%d:%s", highscorearray[count].score, highscorearray[count].name);
    count++
    }
  fclose( f );
  return 0;
  }
  }

int countScores(){

    char line[1000];
    int count = 0

    Fhighscore = fopen("/Users/Evgenij/Documents/highscore/highscore/highscore_e.txt", "r");      //path is incorrect

    if (!Fhighscore){
        printf("Could not open highscore \n" );
        return 0;
    }

    while (fgets( line, MAX_LINE_LEN, Fhighscore )){
        count++
    }


    return count ;
}

void writenewscore(struct sScore *highscorearray,char *Name, int length){
            highscorearray[length+1].punkte = props->score;
            strcpy(highscorearray[length+1].name,Name);

            sort(highscorearray);
        }

}

int main(int argc, const char * argv[]) {
    char Name[40];
    int length
    props = (struct properties*)malloc(sizeof(struct properties));
    props->score =properties.score;
    printf("Enter your name:  ");
    scanf("%s",Name);
    length = countScores()
    struct sScore highscorearray[length+1];
    readData()(highscorearray);
    writenewscore(highscorearray, Name,length));
    writeData()(highscorearray, length);
    for (int i=0;i<length;i++){
        printf("%s %d \n",highscorearray[i].name,highscorearray[i].punkte);
    }

}
