#include<math.h>
#include<string.h>
//структура содержащая информацию о клетке карты
typedef struct CELL{
    int step;
    int danger;
    int weight;
    int egold[10];
} mapcell;

typedef struct ENEMY{
    int xenemy;
    int yenemy;
    int xenemygold[10000];
    int yenemygold[10000];
    int yenemynumber;
    int enemystep[10000];
} enemies;

void get_player3(char **map, int MAX_Y, int MAX_X, int YY, int XX, int *command) {

    const int zona=0; //определитель ареала опасности

    //мои координаты
    int x=YY, y=XX;
    //размеры карты
    int m=MAX_Y, n=MAX_X;

    //координаты противника и его голды
    enemies enemy[10];
    //характеристики клеток
    mapcell cell[m][n];

    //перевод символьной карты в удобоваримый вид
    int i, j, k, p=0, maxg=0, d, g;
    int mynumber;
    int numberofenemy; //количество игроков
    for(j=0; j<m; j++) //столбец
        for(i=0; i<n; i++) { //строка
        cell[j][i].danger=-1;
        cell[j][i].step=m*n;
        for (k=0; k<10; k++)
            cell[j][i].egold[k]=0;
        if (map[j][i]=='#')
            cell[j][i].weight=2;                                             //стена 2
        if (map[j][i]=='.')
            cell[j][i].weight=1;                                             //пустота 1
            for (k=0; k<10; k++) //работает только до 10 игроков
                if (map[j][i]==(k+'0')){
                    if ((x==j) && (y==i)) break;
                    cell[j][i].weight=-2;                                    //игроки -2
                    enemy[p].xenemy=j;
                    enemy[p].yenemy=i; //координаты х и у других игроков
                    enemy[p].yenemynumber=k;
                    p++;
            }
        if((x==j) && (y==i)){
            for (k=0; k<10; k++)
                if (map[j][i]==(k+'0'))
                    mynumber=k;  //мой номер
        }
        if (map[j][i]=='@')
            cell[j][i].weight=-1;                                            //собака -1
        if (map[j][i]=='*'){                                                 //голда 3
            cell[j][i].weight=3;
            for (k=0; k<10; k++)
                cell[j][i].egold[k]=1;
            maxg++;
            }
        }
    numberofenemy=p; //количество противников
    cell[x][y].weight=0;                                                      //я

    int xgold=0, ygold=0, maxiter=m*n, iter; //координаты сокровищ
    //111111111111111111111111111111111111111111111
    if (maxg>10) maxg=10;
    //11111111111111111111111111111111111111111111111
    //сначала найдем количество шагов и координаты ближайших сокровищ противника
    for(k=0; k<numberofenemy; k++){
    cell[enemy[k].xenemy][enemy[k].yenemy].step=0; //ставим точку старта
    iter=0;
    for(g=0; g<maxg; g++){
    iter--;
    while(iter<maxiter){ //выходит за пределы массива, но это не страшно(???)
        for(j=0; j<m; j++)
            for(i=0; i<n; i++)
                if(cell[j][i].step==iter){
                    if(cell[j+1][i].egold[k]==1){
                        cell[j+1][i].step=(iter+1);
                        xgold=j+1;
                        ygold=i;
                        goto nextenemy;
                    };
                    if(cell[j-1][i].egold[k]==1){
                        cell[j-1][i].step=(iter+1);
                        xgold=j-1;
                        ygold=i;
                        goto nextenemy;
                    };
                    if(cell[j][i+1].egold[k]==1){
                        cell[j][i+1].step=(iter+1);
                        xgold=j;
                        ygold=i+1;
                        goto nextenemy;
                    };
                    if(cell[j][i-1].egold[k]==1){
                        cell[j][i-1].step=(iter+1);
                        xgold=j;
                        ygold=i-1;
                        goto nextenemy;
                    };
                    if((cell[j+1][i].weight!=2) && (cell[j+1][i].step==m*n))
                        cell[j+1][i].step=(iter+1);
                    if((cell[j-1][i].weight!=2) && (cell[j-1][i].step==m*n))
                        cell[j-1][i].step=(iter+1);
                    if((cell[j][i+1].weight!=2) && (cell[j][i+1].step==m*n))
                        cell[j][i+1].step=(iter+1);
                    if((cell[j][i-1].weight!=2) && (cell[j][i-1].step==m*n))
                        cell[j][i-1].step=(iter+1);
                }
        iter++;
    }

nextenemy:   iter++;

    enemy[k].enemystep[g]=iter;
    enemy[k].xenemygold[g]=xgold;
    enemy[k].yenemygold[g]=ygold;
    cell[xgold][ygold].egold[k]=0;
    }
        for (d=0; d<m; d++)
        for (p=0; p<n; p++)
            cell[d][p].step=m*n;
}

    //зададим опасные зоны
    int areal, dareal;
    p=0;
    int gold=0;

backnextgold:
    dareal=0;
    areal=zona;

downareal:
    if(dareal!=0){
        areal=zona-dareal;
        for(j=0; j<m; j++)
            for(i=0; i<n; i++)
                if (cell[j][i].danger>areal)
                    cell[j][i].danger=-1;
                goto down;
}

    if(areal==0)
        goto down;

    for(j=0; j<m; j++)
        for(i=0; i<n; i++)
            if (cell[j][i].weight==-1)
                cell[j][i].danger=areal;  //проставляем на зеркале опасности монстров

    while(areal!=1){
        for(j=0; j<m; j++)
            for(i=0; i<n; i++)
                if(cell[j][i].danger==areal){ //проставляем зоны опасности в ходах
                    if(cell[j+1][i].danger==-1)
                        cell[j+1][i].danger=areal-1;
                    if(cell[j-1][i].danger==-1)
                        cell[j-1][i].danger=areal-1;
                    if(cell[j][i+1].danger==-1)
                        cell[j][i+1].danger=areal-1;
                    if(cell[j][i-1].danger==-1)
                        cell[j][i-1].danger=areal-1;
                }
        areal--;
    }

down: 1;

/*printf("%d \n\n\n", areal);
for (j=0; j<m; j++){
    for (i=0; i<n; i++)
        printf("%d ", cell[j][i].danger);
        printf("\n");
}
Sleep(5000);*/

    //выполним трассировку
    int out, minout;
    iter=0;

        for(j=0; j<m; j++)
            for(i=0; i<n; i++)
                cell[j][i].step=m*n;

    if (gold==maxg){
        *command=minout;
        goto goend;
    }

    cell[x][y].step=0; //ставим точку старта

    while(iter<maxiter){ //выходит за пределы массива, но это не страшно(???)
        for(j=0; j<m; j++)
            for(i=0; i<n; i++)
                if(cell[j][i].step==iter){
                    if(cell[j+1][i].weight==3){
                        cell[j+1][i].step=(iter+1);
                        xgold=j+1;
                        ygold=i;
                        goto next;
                    }
                    if(cell[j-1][i].weight==3){
                        cell[j-1][i].step=(iter+1);
                        xgold=j-1;
                        ygold=i;
                        goto next;
                    }
                    if(cell[j][i+1].weight==3){
                        cell[j][i+1].step=(iter+1);
                        xgold=j;
                        ygold=i+1;
                        goto next;
                    }
                    if(cell[j][i-1].weight==3){
                        cell[j][i-1].step=(iter+1);
                        xgold=j;
                        ygold=i-1;
                        goto next;
                    }

                    if((cell[j+1][i].weight!=2) && (cell[j+1][i].step==m*n) && (cell[j+1][i].danger<0))
                        cell[j+1][i].step=(iter+1);
                    if((cell[j-1][i].weight!=2) && (cell[j-1][i].step==m*n) && (cell[j-1][i].danger<0))
                        cell[j-1][i].step=(iter+1);
                    if((cell[j][i+1].weight!=2) && (cell[j][i+1].step==m*n) && (cell[j][i+1].danger<0))
                        cell[j][i+1].step=(iter+1);
                    if((cell[j][i-1].weight!=2) && (cell[j][i-1].step==m*n) && (cell[j][i-1].danger<0))
                        cell[j][i-1].step=(iter+1);

                    if((cell[j+1][i].danger>0) && (cell[j-1][i].danger>0) && (cell[j][i+1].danger>0) && (cell[j][i-1].danger>0)){ //ход если мы в зоне действия монстра
                        if(cell[j+1][i].danger<=cell[j-1][i].danger)
                            d=cell[j+1][i].danger;
                        else d=cell[j-1][i].danger;
                        if(cell[j][i+1].danger<=d)
                            d=cell[j][i+1].danger;
                        if(cell[j][i-1].danger<=d)
                            d=cell[j][i+1].danger;
                        if((cell[j+1][i].weight!=2) && (cell[j+1][i].step==m*n) && (cell[j+1][i].danger==d))
                            cell[j+1][i].step=(iter+1);
                        if((cell[j-1][i].weight!=2) && (cell[j-1][i].step==m*n) && (cell[j-1][i].danger==d))
                            cell[j-1][i].step=(iter+1);
                        if((cell[j][i+1].weight!=2) && (cell[j][i+1].step==m*n) && (cell[j][i+1].danger==d))
                            cell[j][i+1].step=(iter+1);
                        if((cell[j][i-1].weight!=2) && (cell[j][i-1].step==m*n) && (cell[j][i-1].danger==d))
                            cell[j][i-1].step=(iter+1);
                        }
                    }
        iter++;
    }

next:   iter++;

/*printf("%d \n\n\n", iter);
for (j=0; j<m; j++){
    for (i=0; i<n; i++)
        printf("%d ", cell[j][i].step);
        printf("\n");
}
Sleep(5000);*/

    int xx, yy;
    xx=xgold; //координаты моего фантома который как будто бы уже дошел до сокровища и возвращается домой
    yy=ygold;

    //прокладываем маршрут до сокровища
    int dx, dy, rx, ry;
    int dmin=m*n;

    while(1>0){
        rx=xx;  //запомнили координаты предпоследнего хода
        ry=yy;
        if(cell[xx+1][yy].step<dmin && cell[j][i].danger<0){
            dmin=cell[xx+1][yy].step;
            dx=xx+1;
            dy=yy;
        }
        if(cell[xx-1][yy].step<dmin && cell[j][i].danger<0){
            dmin=cell[xx-1][yy].step;
            dx=xx-1;
            dy=yy;
        }
        if(cell[xx][yy+1].step<dmin && cell[j][i].danger<0){
            dmin=cell[xx][yy+1].step;
            dx=xx;
            dy=yy+1;
        }
        if(cell[xx][yy-1].step<dmin && cell[j][i].danger<0){
            dmin=cell[xx][yy-1].step;
            dx=xx;
            dy=yy-1;
        }
        xx=dx;
        yy=dy;
        if(cell[xx][yy].step==0)
            goto then; //вошли в себя
}

then: 1;

    if (((x+1)==rx) && (y==ry)) out=4;
    if (((x-1)==rx) && (y==ry)) out=2;
    if ((x==rx) && ((y+1)==ry)) out=3;
    if ((x==rx) && ((y-1)==ry)) out=1;
    //2 вверх
    //1 влеово
    //4 вниз
    //3 вправо

    //проверим нет ли игроков, которые к этому сокровищу ближе, чем мы.
    for(k=0; k<numberofenemy; k++)
    for(g=0; g<maxg; g++){
        if ((enemy[k].enemystep[g]<iter) && (xgold==enemy[k].xenemygold[g]) && (ygold==enemy[k].yenemygold[g])){
            if(areal!=0){
                dareal++;
                goto downareal; //уменьшаем радиус обхода собак
            }
            cell[xgold][ygold].weight=1;
            if (p==0){
                minout=out;
                p++;
            }
            gold++;
            goto backnextgold; //идем обратно и ищем другое сокровище
        }
        else{
            if((enemy[k].enemystep[g]==iter) && (xgold==enemy[k].xenemygold[g]) && (ygold==enemy[k].yenemygold[g]) && (enemy[k].yenemynumber<mynumber)){
                if(areal!=0){
                    dareal++;
                    goto downareal; //уменьшаем радиус обхода собак
                }
                cell[xgold][ygold].weight=1;
                if (p==0){
                    minout=out;
                    p++;
                }
                gold++;
                goto backnextgold; //идем обратно и ищем другое сокровищ
            }
        }
    }

    *command=out;

goend: 1;
}
