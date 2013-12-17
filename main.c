#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "bot.h"
#include "player1.h"
#include "player2_2.h"
#include "player3.h"
#include "player4.h"

int timesleep=000;

typedef struct ELEM_POINT {
    int y,x,alive;
} ELEM_POINT;

void updatemap(char** map, int maxn, int maxm, ELEM_POINT *Monster, int cmonsters, ELEM_POINT *Gold, int cgolds, ELEM_POINT *Player, int cplayers, int indexplayer, int *score);
void updatemap2(char** map, int maxn, int maxm, ELEM_POINT *Monster, int cmonsters, ELEM_POINT *Gold, int cgolds, ELEM_POINT *Player, int cplayers, int *score);

int factorial(int x) {
    return (x==1) ? 1 : x*factorial(x-1);
}

int main(int argc, char **argv) {
    int i,j,maxn,maxm;
    const char str[]="../test/2/input20.txt";//14 15
    char bool_show=1;
    freopen(str,"rt",stdin);
    scanf("%d%d\n",&maxn,&maxm);
    char **map;
    map=(char **)malloc(maxn*sizeof(char *));
    for (i=0;i<maxn;i++) map[i]=(char *)malloc(maxm*sizeof(char));
    ELEM_POINT *Gold, *Monster, *Player;
    int cgolds=0,cmonsters=0,cplayers=0;
    for (i=0;i<maxn;i++) {
        char *string=(char *)malloc((maxm+2)*sizeof(char));
        gets(string);
        for (j=0;j<maxm;j++) {
            map[i][j]=string[j];
            if (map[i][j]>='0' && map[i][j]<='9') cplayers++;
            if (map[i][j]=='*') cgolds++;
            if (map[i][j]=='@') cmonsters++;
        }
    }
    fclose(stdin);
    Player=(ELEM_POINT *)malloc(cplayers*sizeof(ELEM_POINT));
    Gold=(ELEM_POINT *)malloc(cgolds*sizeof(ELEM_POINT));
    Monster=(ELEM_POINT *)malloc(cmonsters*sizeof(ELEM_POINT));
    int *score=(int *)malloc(cplayers*sizeof(int));
    for (i=0;i<cplayers;i++) score[i]=0;
    int numbergame;
    for (numbergame=0;numbergame<factorial(cplayers);numbergame++) {
        printf("Number of game = %d\n",numbergame);
        //usleep(3*1000*1000);

        int move=0,indexgold,indexmonster,indexplayer;

        //set position
        int iteration=-1;
        int number_of_iteration=0;
        int *massive_iteration=(int *)malloc(cplayers*sizeof(int)) ;
        while (iteration!=numbergame) {
            iteration++;
            while (1) {
                int index_of_iteration, tmp_number_of_iteration=number_of_iteration;
                for (index_of_iteration=cplayers-1;index_of_iteration>=0;index_of_iteration--) {
                    massive_iteration[index_of_iteration]=tmp_number_of_iteration%cplayers;
                    tmp_number_of_iteration/=cplayers;
                }
                int flagin=1;
                for (i=0;i<cplayers-1;i++) {
                    for (j=i+1;j<cplayers;j++) {
                        if (massive_iteration[i]==massive_iteration[j]) {
                            flagin=0;
                            break;
                        }
                    }
                    if (flagin==0) break;
                }
                number_of_iteration++;
                if (flagin==1) break;
            }
        }

        freopen(str,"rt",stdin);
        scanf("%d%d\n",&maxn,&maxm);
        int cgolds=0,cmonsters=0,cplayers=0;
        for (i=0;i<maxn;i++) {
            for (j=0;j<maxm;j++) {
                map[i][j]='.';
            }
        }

        for (i=0;i<maxn;i++) {
            char *string=(char *)malloc((maxm+2)*sizeof(char));
            gets(string);
            for (j=0;j<maxm;j++) {
                map[i][j]=string[j];
                if (map[i][j]>='0' && map[i][j]<='9') {
                    cplayers++;
                    Player[ map[i][j]-'0' ].y=i;
                    Player[ map[i][j]-'0' ].x=j;
                    Player[ map[i][j]-'0' ].alive=map[i][j];
                }
                if (map[i][j]=='*') cgolds++;
                if (map[i][j]=='@') cmonsters++;
            }
        }
        fclose(stdin);

        for (i=0;i<cplayers;i++) {
            map[Player[i].y][Player[i].x]=Player[massive_iteration[i]].alive;
        }
        free(massive_iteration);

        while (1) {
            cgolds=0;
            cmonsters=0;
            indexgold=0;
            indexmonster=0;
            indexplayer=0;
            system("clear");
            for (i=0;i<maxn;i++) {
                for (j=0;j<maxm;j++) {

                    if (bool_show) {


                        if (map[i][j]>='0' && map[i][j]<='9') printf("\033[33m%c",map[i][j]);
                        if (map[i][j]=='#') printf("\033[35m%c",map[i][j]);
                        if (map[i][j]=='*') printf("\033[31m%c",map[i][j]);
                        if (map[i][j]=='@') printf("\033[36m%c",map[i][j]);
                        if (map[i][j]=='.') printf("\033[37m%c",map[i][j]);//37
                        printf("\033[37m");

                        //printf("%c",map[i][j]);
                    }
                    if (map[i][j]>='0' && map[i][j]<='9') {
                        Player[ map[i][j]-'0' ].y=i;
                        Player[ map[i][j]-'0' ].x=j;
                        Player[ map[i][j]-'0' ].alive=map[i][j];
                    }
                    if (map[i][j]=='*') {
                        Gold[indexgold].y=i;
                        Gold[indexgold].x=j;
                        Gold[indexgold].alive=1;
                        indexgold++;
                    }
                    if (map[i][j]=='@') {
                        Monster[indexmonster].y=i;
                        Monster[indexmonster].x=j;
                        Monster[indexmonster].alive=1;
                        indexmonster++;
                    }
                }
                if (bool_show) printf("\n");
            }
            //if (move==0) usleep(1000*1000);
            cgolds=indexgold;
            cmonsters=indexmonster;
            if (move>=10000 || cgolds==0) break;

            int command;
            clock_t time;

            printf("Players think ");
            //0
            if (cplayers>-1) {
                int isGold=0;
                for (i=0;i<maxn;i++) {
                    for (j=0;j<maxm;j++) {
                        if (map[i][j]=='*') {
                            isGold=1;
                            break;
                        }
                    }
                    if (isGold) break;
                }
                if (isGold) {
                    indexplayer=0;
                    time=clock();
                    get_player1(map,maxn,maxm,Player[indexplayer].y,Player[indexplayer].x,&command);
                    time = clock()-time;
                    printf("%3.0f ms ", 1000*(double)time/CLOCKS_PER_SEC);
                    if (1000*(double)time/CLOCKS_PER_SEC<1000) {
                        switch (command) {
                            case 0: break;
                            case 1: if (Player[indexplayer].x>0 && map[Player[indexplayer].y][Player[indexplayer].x-1]!='#') Player[indexplayer].x--; break;
                            case 2: if (Player[indexplayer].y>0 && map[Player[indexplayer].y-1][Player[indexplayer].x]!='#') Player[indexplayer].y--; break;
                            case 3: if (Player[indexplayer].x<maxm-1 && map[Player[indexplayer].y][Player[indexplayer].x+1]!='#') Player[indexplayer].x++; break;
                            case 4: if (Player[indexplayer].y<maxn-1 && map[Player[indexplayer].y+1][Player[indexplayer].x]!='#') Player[indexplayer].y++; break;
                            default : break;
                        }
                    }
                    updatemap(map,maxn,maxm,Monster,cmonsters,Gold,cgolds,Player,cplayers,indexplayer,score);
                }
            }
            //1
            if (cplayers>0) {
                int isGold=0;
                for (i=0;i<maxn;i++) {
                    for (j=0;j<maxm;j++) {
                        if (map[i][j]=='*') {
                            isGold=1;
                            break;
                        }
                    }
                    if (isGold) break;
                }
                if (isGold) {
                    indexplayer=1;
                    time=clock();
                    get_player2(map,maxn,maxm,Player[indexplayer].y,Player[indexplayer].x,&command);
                    time = clock()-time;
                    printf("%3.0f ms ", 1000*(double)time/CLOCKS_PER_SEC);
                    if (1000*(double)time/CLOCKS_PER_SEC<1000) {
                        switch (command) {
                            case 0: break;
                            case 1: if (Player[indexplayer].x>0 && map[Player[indexplayer].y][Player[indexplayer].x-1]!='#') Player[indexplayer].x--; break;
                            case 2: if (Player[indexplayer].y>0 && map[Player[indexplayer].y-1][Player[indexplayer].x]!='#') Player[indexplayer].y--; break;
                            case 3: if (Player[indexplayer].x<maxm-1 && map[Player[indexplayer].y][Player[indexplayer].x+1]!='#') Player[indexplayer].x++; break;
                            case 4: if (Player[indexplayer].y<maxn-1 && map[Player[indexplayer].y+1][Player[indexplayer].x]!='#') Player[indexplayer].y++; break;
                            default : break;
                        }
                    }
                    updatemap(map,maxn,maxm,Monster,cmonsters,Gold,cgolds,Player,cplayers,indexplayer,score);
                }
            }
            /*
            //2
            if (cplayers>0) {
                int isGold=0;
                for (i=0;i<maxn;i++) {
                    for (j=0;j<maxm;j++) {
                        if (map[i][j]=='*') {
                            isGold=1;
                            break;
                        }
                    }
                    if (isGold) break;
                }
                if (isGold) {
                    indexplayer=2;
                    time=clock();
                    get_player3(map,maxn,maxm,Player[indexplayer].y,Player[indexplayer].x,&command);
                    time = clock()-time;
                    printf("%3.0f ms ", 1000*(double)time/CLOCKS_PER_SEC);
                    if (1000*(double)time/CLOCKS_PER_SEC<1000) {
                        switch (command) {
                            case 0: break;
                            case 1: if (Player[indexplayer].x>0 && map[Player[indexplayer].y][Player[indexplayer].x-1]!='#') Player[indexplayer].x--; break;
                            case 2: if (Player[indexplayer].y>0 && map[Player[indexplayer].y-1][Player[indexplayer].x]!='#') Player[indexplayer].y--; break;
                            case 3: if (Player[indexplayer].x<maxm-1 && map[Player[indexplayer].y][Player[indexplayer].x+1]!='#') Player[indexplayer].x++; break;
                            case 4: if (Player[indexplayer].y<maxn-1 && map[Player[indexplayer].y+1][Player[indexplayer].x]!='#') Player[indexplayer].y++; break;
                            default : break;
                        }
                    }
                    updatemap(map,maxn,maxm,Monster,cmonsters,Gold,cgolds,Player,cplayers,indexplayer,score);
                }
            }
            //3
            if (cplayers>1) {
                int isGold=0;
                for (i=0;i<maxn;i++) {
                    for (j=0;j<maxm;j++) {
                        if (map[i][j]=='*') {
                            isGold=1;
                            break;
                        }
                    }
                    if (isGold) break;
                }
                if (isGold) {
                    indexplayer=3;
                    time=clock();
                    get_player4(map,maxn,maxm,Player[indexplayer].y,Player[indexplayer].x,&command);
                    time = clock()-time;
                    printf("%3.0f ms ", 1000*(double)time/CLOCKS_PER_SEC);
                    if (1000*(double)time/CLOCKS_PER_SEC<1000) {
                        switch (command) {
                            case 0: break;
                            case 1: if (Player[indexplayer].x>0 && map[Player[indexplayer].y][Player[indexplayer].x-1]!='#') Player[indexplayer].x--; break;
                            case 2: if (Player[indexplayer].y>0 && map[Player[indexplayer].y-1][Player[indexplayer].x]!='#') Player[indexplayer].y--; break;
                            case 3: if (Player[indexplayer].x<maxm-1 && map[Player[indexplayer].y][Player[indexplayer].x+1]!='#') Player[indexplayer].x++; break;
                            case 4: if (Player[indexplayer].y<maxn-1 && map[Player[indexplayer].y+1][Player[indexplayer].x]!='#') Player[indexplayer].y++; break;
                            default : break;
                        }
                    }
                    updatemap(map,maxn,maxm,Monster,cmonsters,Gold,cgolds,Player,cplayers,indexplayer,score);
                }
            }
            */

            printf("\nScores : ");
            //results
            for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
                printf("%d | ",score[indexplayer]);
            }

            //monsters
            for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
                command = get_monster(map,maxn,maxm,Monster[indexmonster].y,Monster[indexmonster].x);
                switch (command) {
                    case 0: break;
                    case 1: Monster[indexmonster].x--; break;
                    case 2: Monster[indexmonster].y--; break;
                    case 3: Monster[indexmonster].x++; break;
                    case 4: Monster[indexmonster].y++; break;
                    default : break;
                }
                updatemap2(map,maxn,maxm,Monster,cmonsters,Gold, cgolds, Player, cplayers, score);
            }
            printf("move = %d\n",move);
            usleep(timesleep*1000);
            move++;
        }
        for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
            printf("Player %d : scores %d\n",indexplayer,score[indexplayer]);
        }
        printf("move = %d\n",move);
    }
    free(Player);
    free(Gold);
    free(Monster);
    for (i=0;i<maxn;i++) free(map[i]);
    free(map);


    //RATING ELO
    typedef struct profile {
        char name[100];
        int rating;
    } profile;
    profile data[10], *P=(profile *)malloc(cplayers*sizeof(profile));
    int cdata=0;
    //current players
    freopen("../test/cur_game.txt","rt",stdin);
    for (i=0;i<cplayers;i++) {
        scanf("%s\n",P[i].name);
    }
    fclose(stdin);
    freopen("../test/rating.txt","rt",stdin);
    int *ind=(int *)malloc(cplayers*sizeof(int)); //indexs of players in data
    while (1) {
        char st[100];
        scanf("%s\n",st);
        st[(int)strlen(st)]='\0';
        if (!strcmp(st,"0")) break;
        int rate;
        scanf("%d\n",&rate);
        for (i=0;i<(int)strlen(st)+1;i++) data[cdata].name[i]=st[i];
        data[cdata].rating=rate;
        for (i=0;i<cplayers;i++) {
            if (!strcmp(data[cdata].name,P[i].name)) {
                ind[i]=cdata;
            }
        }
        cdata++;
    }
    fclose(stdin);
    printf("BEFORE : \n");
    double *addition=(double *)malloc(cplayers*sizeof(double));
    for (i=0;i<cplayers;i++) {
        addition[i]=0;
    }
    for (i=0;i<cplayers-1;i++) {
        for (j=i+1;j<cplayers;j++) {
            double x= (score[i]>score[j]) ? 1.0 : (score[i]==score[j]) ? 0.5 : 0.0;
            double E1=1.0/(1.0+pow(10.0,(double)(data[ind[j]].rating-data[ind[i]].rating)/400.0)),
                    E2=1.0/(1.0+pow(10.0,(double)(data[ind[i]].rating-data[ind[j]].rating)/400.0));
            addition[i]+=x-E1;
            addition[j]+=1.0-x-E2;
        }
    }

    for (i=0;i<cplayers;i++) {
        data[ind[i]].rating+=(int)(30.0*addition[i]);
    }
    free(P);
    free(addition);


    //HISTORY OF GAMES
    char history[1000][100];
    int indexhistory=0,chistory=0;
    freopen("../test/result_games.txt","rt",stdin);
    while (1) {
        gets(history[indexhistory]);
        if (!strcmp(history[indexhistory],"\0")) break;
        indexhistory++;
    }
    chistory=indexhistory;
    fclose(stdin);
    freopen("../test/result_games.txt","wt",stdout);
    for (indexhistory=0;indexhistory<chistory;indexhistory++) {
        printf("%s\n",history[indexhistory]);
    }
    int indexplayer;
    printf("%s\n",str);
    for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
        printf("%d player = %d\n",indexplayer,score[indexplayer]);
    }
    fclose(stdout);
    free(score);

    printf("AFTER : \n");
    for (i=0;i<cdata;i++) {
        printf("%10s %4d\n",data[i].name,data[i].rating);
    }

    freopen("../test/rating.txt","wt",stdout);
    for (i=0;i<cdata;i++) {
        printf("%s\n%d\n",data[i].name,data[i].rating);
    }
    printf("0");
    fclose(stdout);

    return 0;
}

void updatemap(char **map, int maxn, int maxm, ELEM_POINT *Monster, int cmonsters, ELEM_POINT *Gold, int cgolds, ELEM_POINT *Player, int cplayers, int indexplayer, int *score) {
    int indexgold,indexmonster,i,j;
    //correct
    for (indexgold=0;indexgold<cgolds;indexgold++) {
        if (Gold[indexgold].alive==1 &&
            Player[indexplayer].y==Gold[indexgold].y &&
            Player[indexplayer].x==Gold[indexgold].x) {
                Gold[indexgold].alive=0;
                score[indexplayer]+=1000;
        }
    }

    for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
        if (Monster[indexmonster].alive==1 &&
            Player[indexplayer].y==Monster[indexmonster].y &&
            Player[indexplayer].x==Monster[indexmonster].x) {
                Monster[indexmonster].alive=0;
                score[indexplayer]-=1000;
            }
    }
    char** MAP;
    MAP=(char**)malloc(maxn*sizeof(char*));
    for (i=0;i<maxn;i++) {
        MAP[i]=(char*)malloc(maxm*sizeof(char));
        for (j=0;j<maxm;j++) {
            MAP[i][j]='.';
            if (map[i][j]=='#') MAP[i][j]='#';
        }
    }
    for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
        if (Monster[indexmonster].alive==1) {
            MAP[ Monster[indexmonster].y ][ Monster[indexmonster].x ]='@';
        }
    }
    for (indexgold=0;indexgold<cgolds;indexgold++) {
        if (Gold[indexgold].alive==1) {
            MAP[ Gold[indexgold].y ][ Gold[indexgold].x ]='*';
        }
    }
    for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
        MAP[ Player[indexplayer].y ][ Player[indexplayer].x ]= Player[indexplayer].alive ;
    }

    for (i=0;i<maxn;i++) {
        for (j=0;j<maxm;j++) {
            map[i][j]=MAP[i][j];
        }
        free(MAP[i]);
    }
    free(MAP);
}

void updatemap2(char **map, int maxn, int maxm, ELEM_POINT *Monster, int cmonsters, ELEM_POINT *Gold, int cgolds, ELEM_POINT *Player, int cplayers, int *score) {
    int indexgold,indexmonster,i,j,indexplayer;
    //correct
    for (indexgold=0;indexgold<cgolds;indexgold++) {
        for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
            if (Gold[indexgold].alive==1 &&
                Player[indexplayer].y==Gold[indexgold].y &&
                Player[indexplayer].x==Gold[indexgold].x) {
                    Gold[indexgold].alive=0;
                    score[indexplayer]+=1000;
            }
        }
    }

    for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
        for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
            if (Monster[indexmonster].alive==1 &&
                Player[indexplayer].y==Monster[indexmonster].y &&
                Player[indexplayer].x==Monster[indexmonster].x) {
                    Monster[indexmonster].alive=0;
                    score[indexplayer]-=1000;
                }
        }
    }
    char **MAP;
    MAP=(char**)malloc(maxn*sizeof(char*));
    for (i=0;i<maxn;i++) {
        MAP[i]=(char*)malloc(maxm*sizeof(char));
        for (j=0;j<maxm;j++) {
            MAP[i][j]='.';
            if (map[i][j]=='#') MAP[i][j]='#';
        }
    }
    for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
        if (Monster[indexmonster].alive==1) {
            MAP[ Monster[indexmonster].y ][ Monster[indexmonster].x ]='@';
        }
    }
    for (indexgold=0;indexgold<cgolds;indexgold++) {
        if (Gold[indexgold].alive==1) {
            MAP[ Gold[indexgold].y ][ Gold[indexgold].x ]='*';
        }
    }
    for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
        MAP[ Player[indexplayer].y ][ Player[indexplayer].x ]= Player[indexplayer].alive ;
    }

    for (i=0;i<maxn;i++) {
        for (j=0;j<maxm;j++) {
            map[i][j]=MAP[i][j];
        }
        free(MAP[i]);
    }
    free(MAP);
}
