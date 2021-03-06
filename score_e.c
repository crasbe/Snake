//
//  main.c
//  highscore
//
//  Created by Evgenij Salnikov on 15/01/2017.
//  Copyright � 2017 Evgenij Salnikov. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "snake.h"
struct properties* props; //ekeligg
struct sScore{
    char name[20];
    int punkte;
};

void neusortieren(struct sScore *highscorearray,char *Name);
void dateiAuslesen(struct sScore *highscorearray);
void sortieren(struct sScore *highscorearray);
void schreiben(struct sScore *highscorearray);

void sortieren(struct sScore *highscorearray){
    int length = 10;
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
void schreiben(struct sScore *highscorearray){
    FILE *Fhighscore;
    char zahl[30];
    Fhighscore = fopen("/Users/Evgenij/Documents/highscore/highscore/highscore_e.txt", "w");      //path is incorrect
    for (int i = 0;i < 10;i++){
        fprintf(Fhighscore,"%05d ",highscorearray[i].punkte);
        fprintf(Fhighscore,"%-20s",highscorearray[i].name);



    }
}



void dateiAuslesen(struct sScore *highscorearray){
    char c[50];
    FILE *Fhighscore;
    Fhighscore = fopen("/Users/Evgenij/Documents/highscore/highscore/highscore_e.txt", "r");      //path is incorrect

    for (int i= 0;i<10;i++){
        if (fgets(c, 27, Fhighscore)==NULL) break;
        sscanf(c, "%d %s",&highscorearray[i].punkte,highscorearray[i].name);
}
    fclose(Fhighscore);


}
//void neusortieren(struct sScore *highscorearray,char *Name){                  old version saved in case of bugs with the new one
  //  int index=-1;
  //  int value=-1;
   // for (int i = 0; i<9;i++){
    //    printf("%d\n",highscorearray[i].punkte); ////////Probelm hier Lasse mach!!
      //  if ((highscorearray[i].punkte <= highscorearray[i+1].punkte)){
       //     index = i;
        //    printf("%d !",index);
   //     }
   // }
  //  if (index>=0){
    //    if ((highscorearray[index].punkte <= props->score)){
      //      highscorearray[index].punkte = props->score;
        //    strcpy(highscorearray[index].name,Name);
    //        printf("1");
    //    }
  //  }
  //  sortieren(highscorearray);
//}




void neusortieren(struct sScore *highscorearray,char *Name){
    sortieren(highscorearray);
        if ((highscorearray[10].punkte <= props->score)){
            highscorearray[10].punkte = props->score;
            strcpy(highscorearray[10].name,Name);

            sortieren(highscorearray);
        }

}

int main(int argc, const char * argv[]) {
    char Name[20];
    props = (struct properties*)malloc(sizeof(struct properties));
    props->score =19;
    printf("Gebe deinen Name ein:");
    scanf("%s",Name);
    struct sScore highscorearray[10];
    dateiAuslesen(highscorearray);
    neusortieren(highscorearray, Name);
    schreiben(highscorearray);
    for (int i=0;i<10;i++){
        printf("%s %d \n",highscorearray[i].name,highscorearray[i].punkte);
    }

}
