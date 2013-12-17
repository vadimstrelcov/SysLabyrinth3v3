typedef struct Point {
    int T,p;
    char status,protect;
} Point;
typedef struct _point {
    int y,x;
} _point;
typedef struct Item {
    int y,x,L;
    char used;
} Item;


int commivoyager(Point **Map,int MAX_Y,int MAX_X,Item *List,int cgolds,int level,int depth_commivoyager,int *MinLength,int use_players,int *minlGold);

int minimum(int a, int b) {
    return (a<b) ? a : b;
}
int maximum(int a, int b) {
    return (a>b) ? a : b;
}

char showing=0;

void get_player1(char **map, int MAX_Y, int MAX_X, int YY, int XX, int *command) {

    if (showing) usleep(3000*1000);
    int i,j;
    Point **Map;
    Map=(Point **)malloc(MAX_Y*sizeof(Point *));
    for (i=0;i<MAX_Y;i++) {
        Map[i]=(Point *)malloc(MAX_X*sizeof(Point));
    }
    _point *Gold, *Monster, *Player;
    int cgolds=0,cmonsters=0,cplayers=0,indexmonster=0,indexgold=0,indexplayer=0;
    for (i=0;i<MAX_Y;i++) {
        for (j=0;j<MAX_X;j++) {
            if (map[i][j]=='*') cgolds++;
            if (map[i][j]=='@') cmonsters++;
            if (map[i][j]>='0' && map[i][j]<='9') cplayers++;
        }
    }
    Gold=(_point *)malloc(cgolds*sizeof(_point));
    Monster=(_point*)malloc(cmonsters*sizeof(_point));
    Player=(_point *)malloc(cplayers*sizeof(_point));
    for (i=0;i<MAX_Y;i++) {
        for (j=0;j<MAX_X;j++) {
            if (map[i][j]=='#') {
                Map[i][j].status=0;
            }
            if (map[i][j]=='.') {
                Map[i][j].status=1;
            }
            if (map[i][j]>='0' && map[i][j]<='9') {
                Map[i][j].status=2;
                Player[indexplayer].y=i;
                Player[indexplayer].x=j;
                indexplayer++;
            }
            if (map[i][j]=='*') {
                Map[i][j].status=3;
                Gold[indexgold].y=i;
                Gold[indexgold].x=j;
                indexgold++;
            }
            if (map[i][j]=='@') {
                Map[i][j].status=4;
                Monster[indexmonster].y=i;
                Monster[indexmonster].x=j;
                indexmonster++;
            }
        }
    }
    int *MinlGold=(int *)malloc(cgolds*sizeof(int));
    for (indexgold=0;indexgold<cgolds;indexgold++) {
        MinlGold[indexgold]=MAX_Y*MAX_X+1;
    }

    //algo for other players
    for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
        if (!(Player[indexplayer].y==YY && Player[indexplayer].x==XX)) {
            Point **tmpMap=(Point **)malloc(MAX_Y*sizeof(Point *));
            for (i=0;i<MAX_Y;i++) {
                tmpMap[i]=(Point *)malloc(MAX_X*sizeof(Point));
            }
            for (i=0;i<MAX_Y;i++) {
                for (j=0;j<MAX_X;j++) {
                    tmpMap[i][j].p=-1;
                    tmpMap[i][j].T=-1;
                    tmpMap[i][j].status=Map[i][j].status;
                    tmpMap[i][j].protect=0;
                }
            }
            //algo
            int *newList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
            int *oldList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
            int coldlist=0,indexoldlist=0,indexnewlist=0;
            int T=0;
            tmpMap[Player[indexplayer].y][Player[indexplayer].x].T=0;
            oldList[coldlist++]=MAX_X*Player[indexplayer].y+Player[indexplayer].x;
            while (1) {
                if (coldlist==0) break;
                indexnewlist=0;
                for (indexoldlist=0;indexoldlist<coldlist;indexoldlist++) {
                    int y=oldList[indexoldlist]/MAX_X, x=oldList[indexoldlist]%MAX_X;
                    //up
                    if (y>0) {
                        if (tmpMap[y-1][x].status!=0 && tmpMap[y-1][x].T==-1) {
                            newList[indexnewlist++]=MAX_X*(y-1)+x;
                            tmpMap[y-1][x].T=tmpMap[y][x].T+1;
                        }
                    }
                    //right
                    if (x<MAX_X-1) {
                        if (tmpMap[y][x+1].status!=0 && tmpMap[y][x+1].T==-1) {
                            newList[indexnewlist++]=MAX_X*y+x+1;
                            tmpMap[y][x+1].T=tmpMap[y][x].T+1;
                        }
                    }
                    //down
                    if (y<MAX_Y-1) {
                        if (tmpMap[y+1][x].status!=0 && tmpMap[y+1][x].T==-1) {
                            newList[indexnewlist++]=MAX_X*(y+1)+x;
                            tmpMap[y+1][x].T=tmpMap[y][x].T+1;
                        }
                    }
                    //left
                    if (x>0) {
                        if (tmpMap[y][x-1].status!=0 && tmpMap[y][x-1].T==-1) {
                            newList[indexnewlist++]=MAX_X*y+x-1;
                            tmpMap[y][x-1].T=tmpMap[y][x].T+1;
                        }
                    }
                }
                coldlist=0;
                for (i=0;i<indexnewlist;i++) {
                    oldList[coldlist++]=newList[i];
                }
                T++;
            }
            free(newList);
            free(oldList);

            /*
            printf("PLAYER=%d\n",indexplayer);
            for (i=0;i<MAX_Y;i++) {
                for (j=0;j<MAX_X;j++) {
                    printf("%3d ",tmpMap[i][j].T);
                }
                printf("\n");
            }
            usleep(5*1000*1000);
            */

            int v=-1,minimL=-1;
            for (indexgold=0;indexgold<cgolds;indexgold++) {
                if ((tmpMap[Gold[indexgold].y][Gold[indexgold].x].T<minimL &&
                        tmpMap[Gold[indexgold].y][Gold[indexgold].x].T!=-1 && minimL!=-1) || (minimL==-1)) {
                    v=indexgold;
                    minimL=tmpMap[Gold[v].y][Gold[v].x].T;
                }
            }
            if (v!=-1) {
                MinlGold[v]=minimum(MinlGold[v],tmpMap[Gold[v].y][Gold[v].x].T);
            }

            for (i=0;i<MAX_Y;i++) {
                free(tmpMap[i]);
            }
            free(tmpMap);
        }
    }
    /*
    printf("MINLGOLD : \n");
    for (indexgold=0;indexgold<cgolds;indexgold++) {
        printf("%d ",MinlGold[indexgold]);
    }
    printf("\n");
    usleep(10*1000*1000);
    */


    int depth_m=maximum( (int)( 10*pow((double)(MAX_X*MAX_Y)/10000,1.0)),1);
    depth_m=0;
    while (depth_m>=0) {
        //protection
        for (i=0;i<MAX_Y;i++) {
            for (j=0;j<MAX_X;j++) {
                Map[i][j].protect=0;
                Map[i][j].T=-1;
                Map[i][j].p=-1;
            }
        }
        for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
            _point C;
            C.x=Monster[indexmonster].x;
            C.y=Monster[indexmonster].y;
            int rr=2*depth_m+1;
            Point **massive;
            massive=(Point **)malloc(rr*sizeof(Point *));
            for (i=0;i<rr;i++) massive[i]=(Point *)malloc(rr*sizeof(Point));
            for (i=0;i<rr;i++) {
                for (j=0;j<rr;j++) {
                    massive[i][j].T=-1;
                }
            }
            int *newList=(int *)malloc(rr*rr*sizeof(int));
            int *oldList=(int *)malloc(rr*rr*sizeof(int));
            int coldlist=0,indexoldlist=0,indexnewlist=0;
            int T=0;
            massive[depth_m][depth_m].T=0;
            oldList[coldlist++]=rr*depth_m+depth_m;
            while (1) {
                if (coldlist==0 || T>depth_m) break;
                indexnewlist=0;
                for (indexoldlist=0;indexoldlist<coldlist;indexoldlist++) {
                    int y=oldList[indexoldlist]/rr, x=oldList[indexoldlist]%rr;
                    //up
                    if (y>0) {
                        if (C.y-depth_m+y-1>=0 && C.y-depth_m+y-1<=MAX_Y-1 &&
                            C.x-depth_m+x>=0 && C.x-depth_m+x<=MAX_X-1) {
                                if (Map[C.y-depth_m+y-1][C.x-depth_m+x].status!=0 && massive[y-1][x].T==-1) {
                                    newList[indexnewlist++]=rr*(y-1)+x;
                                    massive[y-1][x].T=massive[y][x].T+1;
                                }
                            }
                    }
                    //down
                    if (y<rr-1) {
                        if (C.y-depth_m+y+1>=0 && C.y-depth_m+y+1<=MAX_Y-1 &&
                            C.x-depth_m+x>=0 && C.x-depth_m+x<=MAX_X-1) {
                                //printf("depth_m = %d, C.y=%d, C.x=%d, %d, %d; .T=%d\n",depth_m,C.y,C.x,C.y-depth_m+y,C.x-depth_m+x,massive[y+1][x].T);
                                if (Map[C.y-depth_m+y+1][C.x-depth_m+x].status!=0 && massive[y+1][x].T==-1) {
                                    newList[indexnewlist++]=rr*(y+1)+x;
                                    massive[y+1][x].T=massive[y][x].T+1;
                                }
                            }
                    }
                    //right
                    if (x<rr-1) {
                        if (C.y-depth_m+y>=0 && C.y-depth_m+y<=MAX_Y-1 &&
                            C.x-depth_m+x+1>=0 && C.x-depth_m+x+1<=MAX_X-1) {
                                if (Map[C.y-depth_m+y][C.x-depth_m+x+1].status!=0 && massive[y][x+1].T==-1) {
                                    newList[indexnewlist++]=rr*y+x+1;
                                    massive[y][x+1].T=massive[y][x].T+1;
                                }
                            }
                    }
                    //left
                    if (x>0) {
                        if (C.y-depth_m+y>=0 && C.y-depth_m+y<=MAX_Y-1 &&
                            C.x-depth_m+x-1>=0 && C.x-depth_m+x-1<=MAX_X-1) {
                                if (Map[C.y-depth_m+y][C.x-depth_m+x-1].status!=0 && massive[y][x-1].T==-1) {
                                    newList[indexnewlist++]=rr*y+x-1;
                                    massive[y][x-1].T=massive[y][x].T+1;
                                }
                            }
                    }
                }
                coldlist=0;
                for (i=0;i<indexnewlist;i++) {
                    oldList[coldlist++]=newList[i];
                }
                T++;
            }
            for (i=0;i<rr;i++) {
                for (j=0;j<rr;j++) {
                    if (C.y-depth_m+i>=0 && C.y-depth_m+i<=MAX_Y-1 &&
                        C.x-depth_m+j>=0 && C.x-depth_m+j<=MAX_X-1) {
                            Map[C.y-depth_m+i][C.x-depth_m+j].protect=Map[C.y-depth_m+i][C.x-depth_m+j].protect ||
                            (massive[i][j].T!=-1);
                        }
                }
            }
            Map[C.y][C.x].protect=0;
            free(newList);
            free(oldList);
            for (i=0;i<rr;i++) {
                free(massive[i]);
            }
            free(massive);
        }
        //algo
        int *newList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
        int *oldList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
        int coldlist=0,indexoldlist=0,indexnewlist=0;
        int T=0;
        oldList[coldlist++]=MAX_X*YY+XX;
        Map[YY][XX].T=0;
        while (1) {
            if (coldlist==0) break;
            indexnewlist=0;
            for (indexoldlist=0;indexoldlist<coldlist;indexoldlist++) {
                int y=oldList[indexoldlist]/MAX_X, x=oldList[indexoldlist]%MAX_X;
                //up
                if (y>0) {
                    if (Map[y-1][x].status!=0 && Map[y-1][x].T==-1 && Map[y-1][x].protect==0) {
                        newList[indexnewlist++]=MAX_X*(y-1)+x;
                        Map[y-1][x].T=Map[y][x].T+1;
                        Map[y-1][x].p=MAX_X*y+x;
                    }
                }
                //down
                if (y<MAX_Y-1) {
                    if (Map[y+1][x].status!=0 && Map[y+1][x].T==-1 && Map[y+1][x].protect==0) {
                        newList[indexnewlist++]=MAX_X*(y+1)+x;
                        Map[y+1][x].T=Map[y][x].T+1;
                        Map[y+1][x].p=MAX_X*y+x;
                    }
                }
                //right
                if (x<MAX_X-1) {
                    if (Map[y][x+1].status!=0 && Map[y][x+1].T==-1 && Map[y][x+1].protect==0) {
                        newList[indexnewlist++]=MAX_X*y+x+1;
                        Map[y][x+1].T=Map[y][x].T+1;
                        Map[y][x+1].p=MAX_X*y+x;
                    }
                }
                //left
                if (x>0) {
                    if (Map[y][x-1].status!=0 && Map[y][x-1].T==-1 && Map[y][x-1].protect==0) {
                        newList[indexnewlist++]=MAX_X*y+x-1;
                        Map[y][x-1].T=Map[y][x].T+1;
                        Map[y][x-1].p=MAX_X*y+x;
                    }
                }
            }
            coldlist=0;
            for (i=0;i<indexnewlist;i++) {
                oldList[coldlist++]=newList[i];
            }
            T++;
        }
        free(newList);
        free(oldList);

        if (showing) {
            printf("depth_m = %d\n",depth_m);
            for (i=0;i<MAX_Y;i++) {
                for (j=0;j<MAX_X;j++) {
                    printf("%3d",Map[i][j].T);
                    if (Map[i][j].status==3) printf("!");
                    else printf(" ");
                }
                printf("\n");
            }
            usleep(10*1000*1000);
        }

        Item *List=(Item *)malloc(cgolds*sizeof(Item));
        for (indexgold=0;indexgold<cgolds;indexgold++) {
            List[indexgold].L=Map[Gold[indexgold].y][Gold[indexgold].x].T;
            List[indexgold].y=Gold[indexgold].y;
            List[indexgold].x=Gold[indexgold].x;
            List[indexgold].used=0;
        }

        /*
        printf("List.T = ");
        for (indexgold=0;indexgold<cgolds;indexgold++) {
            printf("%d ",List[indexgold].L);
        }
        printf("\n");
        */

        int depth_c=minimum(3,cgolds),MinLength=MAX_Y*MAX_X+1;

        int v=commivoyager(Map,MAX_Y,MAX_X,List,cgolds,0,depth_c,&MinLength,1,MinlGold);


        if (showing) printf("FIRST : depth_m=%d v_y=%d v_x=%d\n",depth_m,v/MAX_X,v%MAX_X);
        if (v==-1) {
            MinLength=MAX_Y*MAX_X+1;
            for (indexgold=0;indexgold<cgolds;indexgold++) {
                List[indexgold].L=Map[Gold[indexgold].y][Gold[indexgold].x].T;
                List[indexgold].y=Gold[indexgold].y;
                List[indexgold].x=Gold[indexgold].x;
                List[indexgold].used=0;
            }
            if (showing) printf("SECOND :\n");
            v=commivoyager(Map,MAX_Y,MAX_X,List,cgolds,0,depth_c,&MinLength,0,MinlGold);
        }
        if (showing) printf("v_y=%d v_x=%d\n",v/MAX_X,v%MAX_X);
        //usleep(30*1000*1000);
        free(List);


        if (v!=-1) {
            int y=v/MAX_X,x=v%MAX_X;
            int moved;
            while (!(YY==y && XX==x)) {
                if (y==Map[y][x].p/MAX_X) {
                    if (x>Map[y][x].p%MAX_X) moved=3;//left->right
                    else moved=1;//right->left
                } else {
                    if (y>Map[y][x].p/MAX_X) moved=4;//up->down
                    else moved=2;//down->up
                }
                int ty=Map[y][x].p/MAX_X,tx=Map[y][x].p%MAX_X;
                y=ty;
                x=tx;
            }
            for (i=0;i<MAX_Y;i++) {
                free(Map[i]);
            }
            free(Map);
            free(Gold);
            free(Monster);
            free(Player);
            free(MinlGold);
            *command=moved;
            return;
        }
        depth_m--;
    }
    for (i=0;i<MAX_Y;i++) {
        free(Map[i]);
    }
    free(Map);
    free(Gold);
    free(Monster);
    free(Player);
    free(MinlGold);
    *command=0;
}

int commivoyager(Point **Map,int MAX_Y,int MAX_X,Item *List,int cgolds,int level,int depth_commivoyager,int *MinLength,int use_players,int *minlGold) {
    int indexgold,flagin;

    if (showing) {
        printf("MinLength=%d | level=%d : ",*MinLength,level);
        for (indexgold=0;indexgold<cgolds;indexgold++) {
            printf("%d ",List[indexgold].used);
        }
        printf(" | Length : ");
        for (indexgold=0;indexgold<cgolds;indexgold++) {
            printf("%d ",List[indexgold].L);
        }
        printf("\n");
    }

    if (level!=depth_commivoyager) {
        flagin=-1;
        for (indexgold=0;indexgold<cgolds;indexgold++) {

            int i,j,indexgold_;

            if (showing) {
                printf("CURRENT MinLength=%d | level=%d : ",*MinLength,level);
                for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                    printf("%d ",List[indexgold_].used);
                }
                printf(" | Length : ");
                for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                    printf("%d ",List[indexgold_].L);
                }
                printf("\n");
            }
            if (use_players && level==0) {
                for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                    if (minlGold[indexgold_]<List[indexgold_].L) {
                        List[indexgold_].L=-1;
                    }
                }
                if (showing) {
                    printf("\nUSE PLAYERS : \n");
                    for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                        printf("%d ",List[indexgold_].L);
                    }
                    printf("\n");
                }
            }

            if (List[indexgold].L<*MinLength && List[indexgold].L!=-1 && !List[indexgold].used) {
                Point **tmpMAP;
                tmpMAP=(Point **)malloc(MAX_Y*sizeof(Point *));
                for (i=0;i<MAX_Y;i++) {
                    tmpMAP[i]=(Point *)malloc(MAX_X*sizeof(Point));
                }
                for (i=0;i<MAX_Y;i++) {
                    for (j=0;j<MAX_X;j++) {
                        tmpMAP[i][j].p=-1;
                        tmpMAP[i][j].T=-1;
                        tmpMAP[i][j].status=Map[i][j].status;
                        tmpMAP[i][j].protect=Map[i][j].protect;
                    }
                }
                List[indexgold].used=1;

                int *newList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
                int *oldList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
                int coldlist=0,indexoldlist=0,indexnewlist=0;
                int T=List[indexgold].L;
                tmpMAP[List[indexgold].y][List[indexgold].x].T=T;
                oldList[coldlist++]=MAX_X*List[indexgold].y+List[indexgold].x;
                while (1) {
                    if (coldlist==0) break;
                    indexnewlist=0;
                    for (indexoldlist=0;indexoldlist<coldlist;indexoldlist++) {
                        int y=oldList[indexoldlist]/MAX_X, x=oldList[indexoldlist]%MAX_X;
                        //up
                        if (y>0) {
                            if (tmpMAP[y-1][x].status!=0 && tmpMAP[y-1][x].T==-1 && tmpMAP[y-1][x].protect==0) {
                                newList[indexnewlist++]=MAX_X*(y-1)+x;
                                tmpMAP[y-1][x].T=tmpMAP[y][x].T+1;
                            }
                        }
                        //down
                        if (y<MAX_Y-1) {
                            if (tmpMAP[y+1][x].status!=0 && tmpMAP[y+1][x].T==-1 && tmpMAP[y+1][x].protect==0) {
                                newList[indexnewlist++]=MAX_X*(y+1)+x;
                                tmpMAP[y+1][x].T=tmpMAP[y][x].T+1;
                            }
                        }
                        //right
                        if (x<MAX_X-1) {
                            if (tmpMAP[y][x+1].status!=0 && tmpMAP[y][x+1].T==-1 && tmpMAP[y][x+1].protect==0) {
                                newList[indexnewlist++]=MAX_X*y+x+1;
                                tmpMAP[y][x+1].T=tmpMAP[y][x].T+1;
                            }
                        }
                        //left
                        if (x>0) {
                            if (tmpMAP[y][x-1].status!=0 && tmpMAP[y][x-1].T==-1 && tmpMAP[y][x-1].protect==0) {
                                newList[indexnewlist++]=MAX_X*y+x-1;
                                tmpMAP[y][x-1].T=tmpMAP[y][x].T+1;
                                tmpMAP[y][x-1].p=MAX_X*y+x;
                            }
                        }
                    }
                    coldlist=0;
                    for (i=0;i<indexnewlist;i++) {
                        oldList[coldlist++]=newList[i];
                    }
                    T++;
                }
                free(newList);
                free(oldList);

                if (use_players) {
                    for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                        if (tmpMAP[List[indexgold_].y][List[indexgold_].x].T>minlGold[indexgold_]) {
                            tmpMAP[List[indexgold_].y][List[indexgold_].x].T=-1;
                        }
                    }
                }

                if (showing) {
                    printf("SOME PRINT\n");
                    for (i=0;i<MAX_Y;i++) {
                        for (j=0;j<MAX_X;j++) {
                            printf("%3d",tmpMAP[i][j].T);
                            if (tmpMAP[i][j].status==3) printf("!");
                            else printf(" ");
                        }
                        printf("\n");
                    }
                }

                Item *new_List=(Item *)malloc(cgolds*sizeof(Item));
                for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                    if (List[indexgold_].used) {
                        new_List[indexgold_].L=List[indexgold_].L;
                    } else {
                        new_List[indexgold_].L=tmpMAP[List[indexgold_].y][List[indexgold_].x].T;
                    }
                    new_List[indexgold_].used=List[indexgold_].used;
                    new_List[indexgold_].x=List[indexgold_].x;
                    new_List[indexgold_].y=List[indexgold_].y;
                }

                if (showing) {
                    printf("NEW_DATA : level=%d : ",level);
                    for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                        printf("%d ",new_List[indexgold_].used);
                    }
                    printf(" | Length : ");
                    for (indexgold_=0;indexgold_<cgolds;indexgold_++) {
                        printf("%d ",new_List[indexgold_].L);
                    }
                    printf("\n");
                    usleep(10*1000*1000);
                }

                int x=commivoyager(Map,MAX_Y,MAX_X,new_List,cgolds,level+1,depth_commivoyager,MinLength,use_players,minlGold);
                if ((x!=-1) && ( (tmpMAP[flagin/MAX_X][flagin%MAX_X].T>=tmpMAP[x/MAX_X][x%MAX_X].T && flagin!=-1) || (flagin==-1) ) ) {
                    flagin=List[indexgold].y*MAX_X+List[indexgold].x;
                }

                List[indexgold].used=0;
                free(new_List);
                for (i=0;i<MAX_Y;i++) {
                    free(tmpMAP[i]);
                }
                free(tmpMAP);
            }
        }
        return flagin;
    } else {
        flagin=-1;
        if (level==cgolds) {
            for (indexgold=0;indexgold<cgolds;indexgold++) {
                if (List[indexgold].L==-1) return -1;
            }
        }
        int maxL=-1;
        for (indexgold=0;indexgold<cgolds;indexgold++) {
            if (List[indexgold].L>maxL && List[indexgold].used) {
                flagin=indexgold;
                maxL=List[indexgold].L;
            }
        }
        if (flagin!=-1 && *MinLength>maxL) {
            *MinLength=maxL;
            flagin=MAX_X*List[flagin].y+List[flagin].x;
        }
        //printf("FLAGIN_y=%d FLAGIN_x=%d\n",flagin/MAX_X,flagin%MAX_X);
        return flagin;
    }
    return -1;
}
