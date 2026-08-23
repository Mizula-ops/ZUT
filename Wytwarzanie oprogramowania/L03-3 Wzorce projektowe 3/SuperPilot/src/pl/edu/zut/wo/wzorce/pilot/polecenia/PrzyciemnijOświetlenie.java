package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.Światło;

public class PrzyciemnijOświetlenie implements Polecenie{
    Światło światło;
    public PrzyciemnijOświetlenie(Światło światło){
        super();
        this.światło = światło;
    }

    public void wykonaj(){
        światło.ściemnij(this.światło.odczytajPoziom()/2);
    }

    public void wycofaj(){
        światło.ściemnij(this.światło.odczytajPoziom()*2);
    }
}
