#include "Monoconnectxy3.h"
int get_player2(char **map, int MAX_Y, int MAX_X, int y1, int x1, int *command) {
    short int map2[MAX_Y][MAX_X];
    short int i,j;
    for(i=0; i<MAX_Y; i++)
    {
         for(j=0; j<MAX_X; j++)
         {
                  if(map[i][j]>='0' || map[i][j]<='9') map2[i][j]=-1;
                  if(map[i][j]=='#') map2[i][j]=-2;
                  if(map[i][j]=='.') map2[i][j]=-1;
                  if(map[i][j]=='*') map2[i][j]=-1;
                  if(map[i][j]=='@') map2[i][j]=-1;
         }
    }
    short int L,x2,y2,x3,y3,N;
    list_element* root;
    list_element* root1;
    list_element* root2;
    root=create_new_element(0,0);
    root1=create_new_element(-1,-1);
    root2=create_new_element(0,0);
    map2[y1][x1]=0;
    runner(x1,y1,MAX_X,MAX_Y,map2,root1);
    free_all(root1->next);

    L=30000;
    N=0;
    for(i=0; i<MAX_Y; i++)
    {
         for(j=0; j<MAX_X; j++)
         {
              if(map[i][j]=='*' && map2[i][j]>0)
              {
                  N++;
                  insert_into_list(root2, create_new_element(j,i));
              }
         }
    }
    x3=10000;
    i=0;
    int m[3][2];
    for(i=0; i<N; i++) if(map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x]<=x3)
    x3=map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x];
    for(i=0; i<N; i++) if(map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x]==x3) break;
    m[0][0]=get_by_index(root2, i+1)->y;
    m[0][1]=get_by_index(root2, i+1)->x;
    if(N>1)
    {
        delete_by_index(root2, i+1);
        x3=10000;
        for(i=0; i<N-1; i++) if(map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x]<=x3)
        x3=map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x];
        for(i=0; i<N-1; i++) if(map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x]==x3) break;

        m[1][0]=get_by_index(root2, i+1)->y;
        m[1][1]=get_by_index(root2, i+1)->x;
        if(N>2)
        {
            delete_by_index(root2, i+1);
            x3=10000;
            for(i=0; i<N-2; i++) if(map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x]<=x3)
            x3=map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x];
            for(i=0; i<N-2; i++) if(map2[get_by_index(root2, i+1)->y][get_by_index(root2, i+1)->x]==x3) break;
            m[2][0]=get_by_index(root2, i+1)->y;
            m[2][1]=get_by_index(root2, i+1)->x;
        }
    }
    free_all(root2->next);
    root2->next=NULL;
    if(N>2) {insert_into_list(root2, create_new_element(m[2][1],m[2][0])); N=3;}
    else N=2;
    if(N>1) {insert_into_list(root2, create_new_element(m[1][1],m[1][0]));}
    insert_into_list(root2, create_new_element(m[0][1],m[0][0]));
    short int minimum;
    short int sokr[N];
    if(N>2)
    {
           for(i=0; i<N; i++)
           {
             sokr[i]=0;
             x3=get_by_index(root2,i+1)->x;
             y3=get_by_index(root2,i+1)->y;
             x2=x3;
             y2=y3;
             map[y2][x2]='.';
             sokr[i]+=map2[y2][x2];
             free_all(root1->next);
             root1->next=NULL;
             sokr[i]+=runner1(&x3,&y3,MAX_X,MAX_Y,map,root1);
             free_all(root1->next);
             root1->next=NULL;
             sokr[i]+=runner1(&x3,&y3,MAX_X,MAX_Y,map,root1);
             free_all(root1->next);
             root1->next=NULL;
             map[y2][x2]='*';
           }
    }
    else {sokr[0]=map2[get_by_index(root2,1)->y][get_by_index(root2,1)->x];N=1; i=0;}
    minimum=30000;
    for(i=0; i<N; i++)
    {
             if(sokr[i]<minimum) minimum=sokr[i];
    }
    for(i=0; i<N; i++)
    {
             if(sokr[i]==minimum) break;
    }
    x3=get_by_index(root2,i+1)->x;
    y3=get_by_index(root2,i+1)->y;
    L=map2[y3][x3];
    for(i=L-1;i>=0;i--)
    {
        insert_into_list(root,create_new_element(x3,y3));
        if(map2[y3-1][x3]==i) y3-=1;
        else{
            if(map2[y3+1][x3]==i) y3+=1;
            else{
                if(map2[y3][x3-1]==i) x3-=1;
                else{
                        if(map2[y3][x3+1]==i) x3+=1;
                    }
                }
            }
    }
    short int a,b;
    a=root->next->y;
    b=root->next->x;
    free_all(root);
    free_all(root1);
    free_all(root2);
    if(y1-a==-1) *command=4;//3
    if(y1-a==1) *command=2;//1
    if(x1-b==-1) *command=3;//2
    if(x1-b==1) *command=1;//4
}
/*
1 left
2 up
3 right
4 down
*/
