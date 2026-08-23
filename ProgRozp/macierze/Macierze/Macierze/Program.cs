using System.Threading.Tasks;
using System;

namespace Macierze
{
    internal class Program
    {
        public static int[][] GenerateMatrix(int a1, int a2)
        {
            int[][] A = new int[a1][];
            Random rand = new Random();
            for (int i = 0; i < a1; i++)
            {
                A[i] = new int[a2];
                for (int j = 0; j < a2; j++) 
                    A[i][j] = rand.Next(0, 10);
                
            }

            return A;
        }
        public static int[][] Multiply(int[][] A, int[][] B)
        {
            int a1 = A.Length;
            int a2 = A[0].Length;
            int b1 = B.Length;
            int b2 = B[0].Length;

            if (a2 != b1)
            {
                Console.WriteLine("Błąd wymiarów");
                return null;
            }

            int[][] C = new int[a1][];
            for (int i = 0; i < a1; i++)
                C[i] = new int[b2];
            
            for (int i = 0; i < a1; i++)
                for (int j = 0; j < b2; j++)
                    for (int k = 0; k < a2; k++)
                        C[i][j] += A[i][k] * B[k][j];
            return C;
        }
        public static int[][] MultiplyParallel(int[][] A, int[][] B, int threads)
        {
            int a1 = A.Length;
            int a2 = A[0].Length;
            int b1 = B.Length;
            int b2 = B[0].Length;

            if (a2 != b1)
            {
                Console.WriteLine("Błąd wymiarów");
                return null;
            }

            int[][] C = new int[a1][];
            for (int i = 0; i < a1; i++)
                C[i] = new int[b2];

            var options = new ParallelOptions
            {
                MaxDegreeOfParallelism = threads
            };

            Parallel.For(0, a1, options, i =>
            {
                for (int j = 0; j < b2; j++)
                    for (int k = 0; k < a2; k++)
                        C[i][j] += A[i][k] * B[k][j];
            });

            return C;
        }
        public static void PrintMatrix(int[][] M)
        {
            for (int i = 0; i < M.Length; i++)
            {
                for (int j = 0; j < M[i].Length; j++)
                {
                    Console.Write(M[i][j] + " ");
                }
                Console.WriteLine();
            }
        }

        public static void Main(string[] args)
        {
            Console.Write("Podaj rozmiar macierzy (n dla n x n): ");
            int n = int.Parse(Console.ReadLine());

            int[][] A = GenerateMatrix(n, n);
            int[][] B = GenerateMatrix(n, n);

            Console.WriteLine("\nMacierz A:");
            PrintMatrix(A);

            Console.WriteLine("\nMacierz B:");
            PrintMatrix(B);

            int[][] C = Multiply(A, B);

            Console.WriteLine("\nWynik mnożenia (sekwencyjnie):");
            PrintMatrix(C);

            int threads = 4; // możesz zmienić
            int[][] Cp = MultiplyParallel(A, B, threads);

            Console.WriteLine($"\nWynik mnożenia (równolegle, {threads} wątki):");
            PrintMatrix(Cp);
            
            Mandelbrot m = new Mandelbrot();
            
            m.RunParallel(threads);
            m.SaveToPpm("mandelbrot.ppm");
        }
    }
}