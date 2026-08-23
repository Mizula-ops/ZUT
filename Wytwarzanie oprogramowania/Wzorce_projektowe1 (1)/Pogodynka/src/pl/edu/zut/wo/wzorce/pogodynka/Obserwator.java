package pl.edu.zut.wo.wzorce.pogodynka;

public interface Obserwator{
    public void aktualizacja(float temperatura, float wilgotność, float ciśnienie);
}
