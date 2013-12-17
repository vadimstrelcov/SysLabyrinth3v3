typedef struct graph_peak{
    int val;
    int x;
    int y;
    struct graph_peak* left;
    struct graph_peak* up;
    struct graph_peak* right;
    struct graph_peak* down;
} graph_peak;

graph_peak *create_new_peak(int val, int x, int y){
    graph_peak *new_peak = (graph_peak *)malloc(sizeof(graph_peak));
    new_peak->val = val;
    new_peak->x = x;
    new_peak->y = y;
    new_peak->left = NULL;
    new_peak->up = NULL;
    new_peak->right = NULL;
    new_peak->down = NULL;
    return new_peak;
}

int in_field(int p, int max_p){
    return((p >= 0)&&(p < max_p));
}

int burn(graph_peak *peak, graph_peak **new_peaks, int *new_peaks_length, char **map, int **aux_array, int MAX_X, int MAX_Y){
    int wave_number = peak->val + 1;
    int lx,rx,uy,dy;
    lx = peak->x - 1;
    rx = peak->x + 1;
    uy = peak->y - 1;
    dy = peak->y + 1;

    int status = 1;
    graph_peak* new_peak;
    if(in_field(lx, MAX_X)){
        if((map[peak->y][lx] != '#')&&(aux_array[peak->y][lx] == -1)){
            new_peak = create_new_peak(wave_number, lx, peak->y);
            new_peak->right = peak;
            peak->left = new_peak;

            aux_array[peak->y][lx] = wave_number;
            new_peaks[*new_peaks_length] = new_peak;
            (*new_peaks_length)++;

            if(map[peak->y][lx] == '*'){
                return 7;
            }
        }
    }
    if(in_field(uy, MAX_Y)){
        if((map[uy][peak->x] != '#')&&(aux_array[uy][peak->x] == -1)){
            new_peak = create_new_peak(wave_number, peak->x, uy);
            new_peak->down = peak;
            peak->up = new_peak;

            aux_array[uy][peak->x] = wave_number;
            new_peaks[*new_peaks_length] = new_peak;
            (*new_peaks_length)++;

            if(map[uy][peak->x] == '*'){
                return 7;
            }
        }
    }
    if(in_field(rx, MAX_X)){
        if((map[peak->y][rx] != '#')&&(aux_array[peak->y][rx] == -1)){
            new_peak = create_new_peak(wave_number, rx, peak->y);
            new_peak->left = peak;
            peak->right = new_peak;

            aux_array[peak->y][rx] = wave_number;
            new_peaks[*new_peaks_length] = new_peak;
            (*new_peaks_length)++;

            if(map[peak->y][rx] == '*'){
                return 7;
            }
        }
    }
    if(in_field(dy, MAX_Y)){
        if((map[dy][peak->x] != '#')&&(aux_array[dy][peak->x] == -1)){
            new_peak = create_new_peak(wave_number, peak->x, dy);
            new_peak->up = peak;
            peak->down = new_peak;

            aux_array[dy][peak->x] = wave_number;
            new_peaks[*new_peaks_length] = new_peak;
            (*new_peaks_length)++;

            if(map[dy][peak->x] == '*'){
                return 7;
            }
        }
    }
    return status;
}

graph_peak* step_back(graph_peak* peak, int* step_direction){


    if((peak->left != NULL)&&(peak->left->val < peak->val)){
        *step_direction = 3;
        return(peak->left);
    }
    if((peak->up != NULL)&&(peak->up->val < peak->val)){
        *step_direction = 4;
        return(peak->up);
    }
    if((peak->right != NULL)&&(peak->right->val < peak->val)){
        *step_direction = 1;
        return(peak->right);
    }
    if((peak->down != NULL)&&(peak->down->val < peak->val)){
        *step_direction = 2;
        return(peak->down);
    }
    *step_direction = 0;
    return NULL;
}

void get_player4(char **map, int MAX_Y, int MAX_X, int YY, int XX, int *command){
    int x,y;
    int **aux_array = (int **)malloc(MAX_Y*sizeof(int *));
    for(y=0; y<MAX_Y; y++){
        aux_array[y] = (int *)malloc(MAX_X*sizeof(int));
        for(x=0; x<MAX_X; x++){
            aux_array[y][x] = -1;
        }
    }
    aux_array[YY][XX] = 0;

    int max_dim = (MAX_X > MAX_Y) ? MAX_X : MAX_Y;
    graph_peak **current_peaks = (graph_peak **)malloc(max_dim*2*sizeof(graph_peak *));
    graph_peak **new_peaks = (graph_peak **)malloc(max_dim*2*sizeof(graph_peak *));
    current_peaks[0] = create_new_peak(0, XX, YY);
    int current_peaks_length = 1;
    int new_peaks_length = 0;

    int i;
    int status = 1;
    while(status == 1){
        i = 0;
        while((i < current_peaks_length)&&(status == 1)){
            status = burn(current_peaks[i], new_peaks, &new_peaks_length, map, aux_array, MAX_X, MAX_Y);
            i++;
        }
        for(i=0; i<new_peaks_length; i++){
            current_peaks[i] = new_peaks[i];
        }
        current_peaks_length = new_peaks_length;
        new_peaks_length = 0;
    }

    graph_peak *element = current_peaks[current_peaks_length-1];
    while(element->val > 0){
        element = step_back(element, command);
    }

    free(current_peaks);
    free(new_peaks);
    for(y=0; y<MAX_Y; y++) {
        free(aux_array[y]);
    }
    free(aux_array);
}
