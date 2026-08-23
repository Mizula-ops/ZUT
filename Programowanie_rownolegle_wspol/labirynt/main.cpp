#include "labirynt.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <vector>
#include <algorithm>
#include <random>
#include <omp.h>
using namespace std;
using namespace chrono;

 int arr[N][N];
//Zmienne dla watkow
omp_lock_t locks[N][N];

int globalTid=1;

//Zmienne do obrazu
 int scale=20;
const int width = N;
const int height =N;
const int MaxColorComponentValue=255;
void threadStart(int x,int y,int tid){
   
    mt19937 los(random_device{}());
    while(true){
        omp_set_lock(&locks[x][y]);
        if (arr[x][y] == 0){
            arr[x][y] = tid;

            bool oldWant = true;
            int nextX = x;
            int nextY = y;

            vector<int> dirs = {0,1,2,3};
            shuffle(dirs.begin(), dirs.end(), los);

            for (int d : dirs) {
                int nx = x, ny = y;

                if (d == 0) nx = x - 1;      
                else if (d == 1) nx = x + 1;   
                else if (d == 2) ny = y - 1;   
                else if (d == 3) ny = y + 1;   

                if (nx < 0 || nx >= N || ny < 0 || ny >= N)
                    continue;

                if (arr[nx][ny] == 0) {
                    if (oldWant) {
                        oldWant = false;
                        nextX = nx;
                        nextY = ny;
                    } else {
                        int childTid;
                        #pragma omp critical
                        {
                        childTid= ++globalTid;
                        }
                        #pragma omp task 
                        {
                            threadStart(nx, ny, childTid);
                        }
                            
                    }
                }
            }
            int _x = x, _y = y;
            x = nextX;
            y = nextY;
            omp_unset_lock(&locks[_x][_y]);

        } else {
            omp_unset_lock(&locks[x][y]);
            break;
        }
    
    }
    
}

void printArray(int arr[N][N]){
    for(int i=0;i<N;i++){
        cout<<endl;
        for(int j=0;j<N;j++){
            if(arr[i][j]==-1)
                cout<<"|"<<" ";
                else
                cout<<arr[i][j]<<" ";
        }
    }
}
void numberToRGB(int number, unsigned char &r, unsigned char &g, unsigned char &b){
    if (number ==-1){
        r=g=b=0;
        return;
    }
    r = (number * 97) % 256;
    g = (number * 57) % 256;
    b = (number * 157) % 256;
    if (r < 30) r += 30;
    if (g < 30) g += 30;
    if (b < 30) b += 30;
}
void saveLabyrinthPPM(char *filename) {
    FILE *fp = fopen(filename, "wb");
    char *comment="# ";
    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,height*scale,width*scale,MaxColorComponentValue);
    
    for (int i = 0; i < height; i++) {
        for (int sy = 0; sy < scale; ++sy) { 
            for (int j = 0; j < width; j++) {
                unsigned char r,g,b;
                numberToRGB(arr[i][j], r, g, b);
                for (int sx = 0; sx < scale; ++sx) { 
                    unsigned char px[3] = { r, g, b };
                    fwrite(px, 1, 3, fp);
                }
                
            }
        }
    }

    fclose(fp);
}
int main()
{
   
    srand(time(nullptr)); 
    Labirynt* labirynt = new Labirynt(); 
    
    labirynt->generateProfessionalBoard(0.5,false);
    labirynt->normalArray(arr);
    cout<<"Wygenerowano labirynt"<<endl;
    #pragma omp parallel
    {
        #pragma omp single
        {
          
            #pragma omp task 
            {
                threadStart(0, 0, globalTid);
            }
            #pragma omp taskwait
        }
    }
   
    cout<<"Zakonczono symulacje poruszania sie watkow po labiryncie"<<endl;
    saveLabyrinthPPM("labirynt2.ppm");
    cout<<"Zapisano jako labirynt1.ppm"<<endl;
     return 0;
        
      
}