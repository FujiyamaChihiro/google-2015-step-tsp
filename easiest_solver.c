//
//  easiest_solver.c
//  
//
//  Created by 藤山 千紘 on 2015/06/29.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define POINT_MAX 2048

typedef struct coordinate_t{
    int id;
    double x;
    double y;
}coordinate;

coordinate points[POINT_MAX];
int path[POINT_MAX];
int point_num=0;


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

void solve(){
    int current_city,pid;
    double px,py,cx,cy;
    double travel_distance=0.0;
    
    pid=points[point_num-1].id;
    px=points[point_num-1].x;
    py=points[point_num-1].y;
    
    for(int i=0;i<point_num;i++){
        current_city=points[i].id;
        cx=points[i].x;
        cy=points[i].y;
        
        travel_distance+=sqrt(pow(cx-px,2)+pow(cy-py,2));
        path[i]=current_city;
        
        pid=current_city;
        px=cx;
        py=cy;
    }
    printf("total distance : %.15lf\n",travel_distance);
}

int main(int argc,char** argv){
    if(argc!=3){
        printf("usage : ./a.out <input_file_name> <output_file_name>\n");
        exit(1);
    }
    read_file(argv);
    solve();
    write_file(argv);
    
}