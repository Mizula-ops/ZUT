#ifndef LABIRYNT_H
#define LABIRYNT_H
#include "lista.h"
#include <iostream>
#include <cstdio>
#include <random>
using namespace std;
const int N=16;

struct Elementlabirynt{
    int data;
    int number;
    int x;
    int y;
    public:
        Elementlabirynt(int _x, int _y) :x(_x), y(_y), data(-1), number(-1){}
};

class Labirynt{
   
    Elementlabirynt* array[N][N];
    int corridorsLenght=0;
public:
  
    Labirynt(){
        for (int x=0; x< N;x++){
            for(int y=0;y<N;y++){
                array[x][y]= new Elementlabirynt(x,y);
            }
        }
    }
    ~Labirynt(){clear();}
    void generate_corridors(){
        int i,j;
        Elementlabirynt *el =array[0][0];
        el->number=2;
        el->data=0;
        int y,x=0;
        LinkedList < Elementlabirynt* >* wiadukt = new LinkedList < Elementlabirynt* >();
        wiadukt->frontpush(el);
        while (wiadukt->isNoEmpty()){
            Elementlabirynt *p= wiadukt -> headReturn();
            corridorsLenght++;
            wiadukt->frontpop();
            Elementlabirynt *up, *down, *left, *right;
            down  = (p->x + 1 < N) ? array[p->x + 1][p->y] : nullptr;
            up = (p->x - 1 >= 0) ? array[p->x - 1][p->y] : nullptr;
            right = (p->y + 1 < N) ? array[p->x][p->y + 1] : nullptr;
            left  = (p->y - 1 >= 0) ? array[p->x][p->y - 1] : nullptr;
            if (p->number!=2){

                if ( up && down && up->data==0 && down->data==-1 ){
                    down->data=0;
                    down->number=p->number+1;
                    wiadukt->backpush(down);
                }
                else if (up && down && up->data==-1 && down->data==0 ){
                    up->data=0;
                    up->number=p->number+1;
                    wiadukt->backpush(up);
                }
                else if (right && left && right->data==0 && left->data==-1){
                        left->data=0;
                        left->number=p->number+1;
                        wiadukt->backpush(left);
                }
                else if (right && left && right->data==-1 && left->data==0 ){
                        right->data=0;
                        right->number=p->number+1;
                        wiadukt->backpush(right);
                }    
           
            }
            else if(p->number==2){
               
                  if (up && up->data==-1){
                    int which= rand()%3+1;
                    if(which==1 || which == 2){
                        up->data=0;
                        up->number=0;
                        wiadukt->backpush(up);
                      
                    }
                    
                }
                if (down && down->data==-1){
                    int which= rand()%3+1;
                    if(which==1 || which == 2){
                        down->data=0;
                        down->number=0;
                        wiadukt->backpush(down);
                    }
                    
                }
                if (left && left->data==-1){
                    int which= rand()%3+1;
                    if(which==1 || which ==2){
                        left->data=0;
                        left->number=0;
                        wiadukt->backpush(left);
                    }
                    
                }
                if (right && right->data==-1){
                    int which= rand()%3+1;
                    if(which==1 || which ==2){
                        right->data=0;
                        right->number=0;
                        wiadukt->backpush(right);
                    }
                    
                }
            }
              
        }
             
        delete wiadukt;
    }
    void print(){
        for( int i=0; i<N;i++){
            cout<<endl;
            for(int j=0;j<N;j++){
                if(array[i][j]->data==-1)
                cout<<"|"<<" ";
                else
                cout<<array[i][j]->number<<" ";
            }
        }
    }
    int corridorsSize(){
        return corridorsLenght;
    }
    void clear(){
        for (int x=0; x< N;x++){
            for(int y=0;y<N;y++){
                array[x][y]->data = -1;
                array[x][y]->number = -1;
            }
        }
        corridorsLenght=0;
        
    }
    void generateProfessionalBoard(double procent,bool finishPoint){
        
        while(true){
            clear();
            generate_corridors();
            int corridors = corridorsLenght;
            double percent = (double)corridors / (N*N);
            if(percent>=procent){
                if(finishPoint){
                    if( array[N-1][N-1]->data==0){
                    break;
                }
                }
                else if (!finishPoint){
                    break;

                }
            }
            
        }
    }
    void normalArray(int out[N][N]){
        int normalArray[N][N];
        for (int x=0; x< N;x++){
            for(int y=0;y<N;y++){
                out[x][y]=array[x][y]->data;
            }
        }
        
    }
};
#endif