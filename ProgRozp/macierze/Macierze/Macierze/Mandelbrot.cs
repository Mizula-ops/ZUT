using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;
namespace Macierze
{
    public class Mandelbrot
    {
        const int iXmax = 10240;
        const int iYmax = 10240;

        const double CxMin=-2.5;
        const double CxMax=1.5;
        const double CyMin=-2.0;
        const double CyMax=2.0;
        double PixelWidth = (CxMax - CxMin) / iXmax;
        double PixelHeight = (CyMax - CyMin) / iYmax;
        
        char [,,] color = new char[iYmax,iXmax,3];
        const int IterationMax=200;

        const double EscapeRadius=2;
        double ER2=EscapeRadius*EscapeRadius;
        int num_threads=1;
        

        public void RunParallel(int threads)
        {
            var options = new ParallelOptions
            {
                MaxDegreeOfParallelism = threads
            };
            var start = DateTime.Now;
            Parallel.For(0, iYmax, options, iY =>
            {
                double Cy = CyMin + iY * PixelHeight;
                for (int iX = 0; iX < iXmax; iX++)
                { double Cx = CxMin + iX * PixelWidth;
                    double Zx = 0, Zy = 0;
                    double Zx2 = 0, Zy2 = 0;
                    int Iteration;
                    for (Iteration = 0; Iteration < IterationMax && (Zx2 + Zy2) < ER2; Iteration++)
                    {
                        Zy = 2 * Zx * Zy + Cy;
                        Zx = Zx2 - Zy2 + Cx;
                        Zx2 = Zx * Zx;
                        Zy2 = Zy * Zy;
                    }

                    if (Iteration == IterationMax)
                    {
                        color[iY, iX, 0] = (char)255;
                        color[iY, iX, 1] = (char)255;
                        color[iY, iX, 2] = (char)255;
                    }
                    else
                    {
                        color[iY, iX, 0] = (char)0;
                        color[iY, iX, 1] = (char)0;
                        color[iY, iX, 2] = (char)0;
                    }
                }
            });

            var end = DateTime.Now;

            Console.WriteLine($"Parallel Mandelbrot ({threads} wątków)");
            Console.WriteLine($"Czas: {(end - start).TotalSeconds:F4}s\n");
        }
        public void SaveToPpm(string filename)
        {
            using (FileStream fs = new FileStream(filename, FileMode.Create))
            using (BinaryWriter bw = new BinaryWriter(fs))
            {
                string header = $"P6\n# Mandelbrot\n{iXmax} {iYmax}\n255\n";
                bw.Write(Encoding.ASCII.GetBytes(header));

                for (int y = 0; y < iYmax; y++)
                {
                    for (int x = 0; x < iXmax; x++)
                    {
                        bw.Write((byte)color[y, x, 0]);
                        bw.Write((byte)color[y, x, 1]);
                        bw.Write((byte)color[y, x, 2]);
                    }
                }
            }
        }
        
    }
}
        
    
