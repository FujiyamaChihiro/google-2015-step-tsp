#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define POINT_MAX 2048

typedef struct coordinate_t{
    int id; //都市の名前
    double x; //x座標
    double y; //y座標
    int fl; //訪れたら1,未訪問なら0
}coordinate;

coordinate points[POINT_MAX];
int path[POINT_MAX]; //訪れる順番
int point_num=0; //都市数
int visited_point_num=0; //すでに訪問した都市の数

void insert_point(double x,double y){
    if(point_num<POINT_MAX){
        points[point_num].id=point_num;
        points[point_num].x=x;
        points[point_num].y=y;
        point_num++;
    }else{
        printf("The table is full.\n");
        exit(1);
    }
}

void read_file(char **argv){
    FILE* file=fopen(argv[1],"r");
    if(file==NULL){
        printf("Not found.\n");
        exit(1);
    }
    double x,y;
    int tmp=fscanf(file,"%*s%lf,%lf\n",&x,&y); //1行目は読み飛ばす
    insert_point(x,y);
    while(1){
        tmp=fscanf(file,"%lf,%lf\n",&x,&y);
        if(tmp==EOF){
            fclose(file);
            return;
        }else{
            insert_point(x,y);
        }
    }
}

void write_file(char **argv){
    FILE* file=fopen(argv[2],"w");
    if(file==NULL){
        printf("Not Found.\n");
        exit(1);
    }
    fputs("index\n",file);
    for(int i=0;i<point_num;i++){
        fprintf(file,"%d\n",path[i]);
    }
    fclose(file);
}

void initialize_fl(){
    for(int i=0;i<point_num;i++){
        points[i].fl=0;
    }
}

double distance(coordinate *point1,coordinate *point2){
    double distance=sqrt(pow(point1->x-point2->x,2)+pow(point1->y-point2->y,2));
    return distance;
}

double solve(coordinate *point){
    double *dist;
    dist=(double *)malloc(sizeof(double)*point_num);
    double travel_distance;
    path[visited_point_num]=point->id; //スタート地点を更新
    points[point->id].fl=1;
    visited_point_num++;
    if(visited_point_num==point_num){
        return distance(&points[point->id],&points[path[0]]);
    }
    for(int i=0;i<point_num;i++){
        dist[i]=distance(&points[point->id],&points[i]);
    }
    int nearest_point;
    int std;
    for(int i=0;i<point_num;i++){
        if(dist[i]!=0&&points[i].fl==0){
            std=dist[i];
            nearest_point=i;
            break;
        }
    }
    /*距離が0ではない最も近い点を見つける*/
    for(int i=0;i<point_num;i++){
        if(std>=dist[i]&&dist[i]!=0&&points[i].fl==0){
            std=dist[i];
            nearest_point=i;
        }
    }
    travel_distance=dist[nearest_point];
    free(dist);
    return travel_distance+solve(&points[nearest_point]);
}

int main(int argc,char **argv){
    int start=0;
    if(argc!=3){
        printf("usage : ./a.out <input_file_name> <output_file_name>\n");
        exit(1);
    }
    read_file(argv);
    initialize_fl();
    double total_distance_for_cmp1=solve(&points[0]);
    for(int i=1;i<point_num;i++){
        visited_point_num=0;
        initialize_fl();
        double total_distance_for_cmp2=solve(&points[i]);
        if(total_distance_for_cmp2<total_distance_for_cmp1){
            total_distance_for_cmp1=total_distance_for_cmp2;
            start=i;
        }
    }
    printf("start is %d\n",start);
    visited_point_num=0;
    initialize_fl();
    double total_distance=solve(&points[start]);
    printf("total distance : %.15lf\n",total_distance);
    write_file(argv);
}

