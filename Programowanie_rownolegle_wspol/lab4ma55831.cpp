
#include <iostream>
#include <omp.h>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <mutex>
#include <vector>
#include <fstream>
using namespace std;


const int iXmax = 2048; 
const int iYmax = 2048;

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
const int MAX_THREADS =4;
long int sum [MAX_THREADS] = {512};
int num_threads=0;
mutex mtx;
 double s,f;
double thread_time[MAX_THREADS]={0};
void mandelbrot(int num_thread){
        
        int tid;
        s=omp_get_wtime();
        #pragma omp parallel num_threads(num_thread) private(tid)
        {
            tid=omp_get_thread_num();
            double t_start = omp_get_wtime();
            #pragma omp for  nowait
            for(int iY=0;iY<iYmax;iY++){
                double Cy=CyMin + iY*PixelHeight;
                if (fabs(Cy)< PixelHeight/2) Cy=0.0;
                int Iteration;          
                                
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
            double t_end = omp_get_wtime();
            thread_time[tid] = t_end - t_start;
        }
        f=omp_get_wtime();
            
                
    
}

int main() {
    
    int i, j;
    ofstream csv("czas.csv",ios::app);
    vector<int>thread_list={1,2,4,8,16,32,64,128,256,512};
    vector<double> time_list;
        for (num_threads=4;num_threads<=4; num_threads*=2)
        {
        
            mandelbrot(num_threads);
            double elapsed = f - s;
            cout << "Dla " << num_threads << " watkow. Czas to: "
                << fixed << setprecision(6) << elapsed << " s" << endl;
            cout<<endl;
            
            time_list.push_back(elapsed);
            
            for(i=0;i<num_threads;i++){
                cout<<"Dla wątka "<<i+1<<" Suma iteracji:"<< sum[i]<< "czas to:" <<thread_time[i]<<fixed<<setprecision(6)<<elapsed<< "s"<<endl;
                
                sum[i]=0;
            
            }
            

        

            if(num_threads==4 && iXmax ==512 && iYmax == 512){
                FILE * fp;
            char *filename="new4.ppm";
            char *comment="# ";
            const int MaxColorComponentValue = 255;
            
            fp= fopen(filename,"wb"); 
            
            
            fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
            
            size_t total_bytes = static_cast<size_t>(iXmax) * iYmax * 3;
            fwrite(&color[0][0][0], 1, total_bytes, fp);
            fclose(fp);
            }
            

        
            
        }
        
         csv << "ilosc watkow;";
        for (int t : thread_list)
        csv << t << ";";
        csv << "\n";
        
        csv << "czas dla obrazu "<<iYmax <<"X"<<iXmax<<";";
        csv << fixed << setprecision(6);
        for (double tt : time_list)
            csv << tt << ";";
        csv << "\n";
    
   
     csv.close();

    return 0;
}