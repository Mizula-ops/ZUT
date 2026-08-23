#include <iostream>
#include <cstdlib>
#include <functional>
#include <thread>
#include <chrono>
#include <vector>
#include <ctime>
using namespace std;
using namespace chrono;
int num_threads =1;
const int N =4096;
//int A[N][N], B[N][N], C[N][N], BT[N][N];
float* _at=new float[N*N];
float* _bt=new float[N*N];
float* _bt2=new float[N*N];
float* _ct=new float[N*N];
float** AT= new float*[N];
float** BT= new float*[N];
float** BT2= new float*[N];
float** CT= new float*[N];


void multiplying_matrix(int tid){
    
    int start=(N/num_threads)*tid;
    int stop=start+(N/num_threads);
    for(int i=start;i<stop;i++){
        for(int j=0;j<N;j++){
            CT[i][j]=0.0f;
            for(int k=0;k<N;k++){
                CT[i][j]+=AT[i][k]*BT[k][j];
            }
        }
    }
}
void multiplying_matrix_T(int tid){
    
    int start=(N/num_threads)*tid;
    int stop=start+(N/num_threads);
    for(int i=start;i<stop;i++){
        for(int j=0;j<N;j++){
            CT[i][j]=0.0f;
            for(int k=0;k<N;k++){
                CT[i][j]+=AT[i][k]*BT2[j][k];
            }
        }
    }
}
int main(){
    srand(time(nullptr));
    int i,j;
     for(i=0;i<N;i++){
            AT[i]=_at+N*i;
            BT[i]=_bt+N*i;
            CT[i]=_ct+N*i;
            BT2[i]=_bt2+N*i;
        }
        for (i=0;i<N;i++){
        for(j=0;j<N;j++){
            BT[i][j] = static_cast<float>(rand()) / RAND_MAX * 100.0f; // 0..100
            AT[i][j] = static_cast<float>(rand()) / RAND_MAX * 100.0f;
        }
        }
         for (i=0;i<N;i++){
            for(j=0;j<N;j++){
                BT2[i][j]=BT[j][i];
            }
        }
    
    for( num_threads=8;num_threads<=8;num_threads*=2){
        
        cout<<"Liczba watkow:"<<num_threads<<endl;
       
        thread th[num_threads];
        const auto start{steady_clock::now()};
        for (i = 0; i < num_threads; i++) {
        th[i] = thread(multiplying_matrix, i);
        }
        for (i = 0; i < num_threads; i++) {
        th[i].join();
        } 
        const auto finish{steady_clock::now()};
        const duration<double> elapsed_seconds{finish - start};
        cout << elapsed_seconds.count() << '\n';
    }
    delete[] AT; delete[] BT; delete[] BT2; delete[] CT;
    delete[] _at; delete[] _bt; delete[] _ct; delete[] _bt2;
}