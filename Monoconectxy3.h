typedef struct list_element {
    short int x;
    short int y;
    struct list_element* next;
} list_element;

list_element* create_new_element(int val, int vall)
{
    list_element* elem =(list_element*)malloc(sizeof(list_element));
    elem->x=val;
    elem->y=vall;
    elem->next=NULL;
    return elem;
}

void insert_into_list(list_element* root, list_element* new_elem)
{
    new_elem->next = root->next;
    root->next = new_elem;
}

list_element* get_by_index(list_element* root, int index)
{
    if(index<0) return NULL;
    list_element* a=root;
    int w;
    for(w=0; w<index; w++)
    {
      if(a==NULL) return NULL; 
      a=a->next;
    }
    return a;
}

void delete_by_index (list_element* root, short int index)
{
    if(index>0)
    {
    list_element* a;
    list_element* b;
    a=get_by_index(root,index-1);
         if(a!=NULL && a->next!=NULL)
         {    
              b=a->next;
              a->next=b->next;
              free(b);
         }    
    }
}
void free_all(list_element* root)
{
     if(root!=NULL)
     {
     if((root->next)!=NULL)  free_all(root->next);
     free(root);}
}
void print_list(list_element* root)
{
    list_element* a;
    a=root;
    while(a!=NULL)
    {
    printf("x=%d y=%d\n", a->x,a->y);
    a=a->next;
    }
}
void normalize(list_element* root1)
{
     list_element* a;
     a=root1;
     while(a->next!=NULL)
     {
          if((a->next)->x==a->x && (a->next)->y==a->y) 
          {
              list_element* b;
              b=a->next;
              a->next=b->next;
              free(b);
          }
          a=a->next;
     }
}
void round1 (int x,int y,int X,int Y, short int map2[][X],list_element* root1,int n)
{
     delete_by_index(root1,n);
     if(x>0 && x<X-1 && y>0 && y<Y-1) 
     {
            if(map2[y-1][x] == -1 || map2[y-1][x]>map2[y][x]+1) {map2[y-1][x]=map2[y][x]+1; insert_into_list(root1, create_new_element(x,y-1));}
            if(map2[y][x-1] == -1 || map2[y][x-1]>map2[y][x]+1) {map2[y][x-1]=map2[y][x]+1; insert_into_list(root1, create_new_element(x-1,y));}
            if(map2[y][x+1] == -1 || map2[y][x+1]>map2[y][x]+1) {map2[y][x+1]=map2[y][x]+1; insert_into_list(root1, create_new_element(x+1,y));}
            if(map2[y+1][x] == -1 || map2[y+1][x]>map2[y][x]+1) {map2[y+1][x]=map2[y][x]+1; insert_into_list(root1, create_new_element(x,y+1));}
     }
}
void runner (short int x,short int y,short int X,short int Y, short int map2[][X],list_element* root1)
{
     short int n;
     n=0;
     list_element* a;
     a=root1;
     round1(x,y,X,Y,map2,root1,n);
     while(a->next!=NULL) {n++; a=a->next;}
     while(get_by_index(root1,n-1)!=NULL)
     {
           round1(get_by_index(root1,n)->x,get_by_index(root1,n)->y,X,Y,map2,root1,n);
           normalize(root1);
           n=0;
           a=root1;
           while(a->next!=NULL) {n++; a=a->next;}
     }
}
short int runner1 (short int* x,short int* y,short int X,short int Y, char** map,list_element* root1)
{
     short int n,i,j;
     short int map3[Y][X];
     for(i=0; i<Y; i++)
    {
         for(j=0; j<X; j++)
         {
                  if(map[i][j]>='0' || map[i][j]<='9') map3[i][j]=-1;
                  if(map[i][j]=='#') map3[i][j]=-2;
                  if(map[i][j]=='.') map3[i][j]=-1;
                  if(map[i][j]=='*') map3[i][j]=-1;
                  if(map[i][j]=='@') map3[i][j]=-1;
         }
    }
     n=0;
     map3[*y][*x]=0;
     list_element* a;
     a=root1;
     round1(*x,*y,X,Y,map3,root1,n);
     while(a->next!=NULL) {n++; a=a->next;}
     while(get_by_index(root1,n-1)!=NULL)
     {
           if (map[get_by_index(root1,n)->y][get_by_index(root1,n)->x]=='*') 
           {
           *x=(get_by_index(root1,n)->x);
           *y=(get_by_index(root1,n)->y);
           return map3[*y][*x];
           }
           round1(get_by_index(root1,n)->x,get_by_index(root1,n)->y,X,Y,map3,root1,n);
           normalize(root1);
           n=0;
           a=root1;
           while(a->next!=NULL) {n++; a=a->next;}
     }
     free_all(root1->next);
     return 10000;
}
