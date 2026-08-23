#include <iostream>
#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;
using namespace chrono;

const int iXmax = 10240; 
const int iYmax = 10240;

const double CxMin=-2.5;
const double CxMax=1.5;
const double CyMin=-2.0;
const double CyMax=2.0;

double PixelWidth=(CxMax-CxMin)/iXmax;
double PixelHeight=(CyMax-CyMin)/iYmax;

unsigned char color[iYmax][iXmax][3];
const int IterationMax=200;

const double EscapeRadius=2;
double ER2=EscapeRadius*EscapeRadius;
int counter=0;
const int MAX_THREADS =512;
long int sum [MAX_THREADS] = {0};
int num_threads=0;
mutex mtx;

void mandelbrot(int tid){
         
        int myiy = 0;
        while(myiy<iYmax){
            mtx.lock();
            myiy = counter++;
            mtx.unlock();
            int iY=myiy;

            if(iY < iYmax){
                double Cy=CyMin + iY*PixelHeight;
                if (fabs(Cy)< PixelHeight/2) Cy=0.0;
                int Iteration=0;
                
                for(int iX=0;iX<iXmax;iX++)
                {
                    double Cx=CxMin + iX*PixelWidth;
                    double Zx=0.0;
                    double Zy=0.0;
                    double Zx2=Zx*Zx;
                    double Zy2=Zy*Zy;
                    for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
                    {
                        
                        Zy=2*Zx*Zy + Cy;
                        Zx=Zx2-Zy2 +Cx;
                        Zx2=Zx*Zx;
                        Zy2=Zy*Zy;
                    };
                    sum[tid]+=Iteration;
                    if (Iteration==IterationMax)
                    { 
                                color[iY][iX][0]=255;
                                color[iY][iX][1]=255;
                                color[iY][iX][2]=255;                           
                    }
                    else
                    { 
                                int base = (tid * 40) % 256;
                                color[iY][iX][0]=0;
                                color[iY][iX][1]=base/2;
                                color[iY][iX][2]=100+base;                           
                    }
                }
            }
        }
    
}

int main() {
    
    int i, j;
    for (num_threads=1;num_threads<=512; num_threads*=2)
    {
        counter = 0;
        thread th[num_threads];
        const auto start{steady_clock::now()};
        for (i = 0; i < num_threads; ++i) {
            th[i] = thread(mandelbrot, i);
        }
        for (i = 0; i < num_threads; ++i) {
            th[i].join();
        }
        const auto finish{steady_clock::now()};
        const duration<double> elapsed_seconds{finish - start};
        cout << "Dla "<<num_threads<<endl;
        for(i=0;i<num_threads;i++){
            cout<<"Dla wątka "<<i+1<<" Suma iteracji:"<< sum[i]<<endl;
            sum[i]=0;
        }
        if(num_threads==4){
             FILE * fp;
        char *filename="new2.ppm";
        char *comment="# ";
        const int MaxColorComponentValue = 255;
        
        fp= fopen(filename,"wb"); 
        
        
        fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
        
        size_t total_bytes = static_cast<size_t>(iXmax) * iYmax * 3;
        fwrite(&color[0][0][0], 1, total_bytes, fp);
        fclose(fp);
        }
        cout << elapsed_seconds.count() << '\n'<<endl;
        
    }
    
    

    return 0;
}