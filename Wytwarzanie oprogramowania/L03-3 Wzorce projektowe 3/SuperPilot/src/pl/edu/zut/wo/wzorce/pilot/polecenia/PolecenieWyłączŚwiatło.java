package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.Światło;

public class PolecenieWyłączŚwiatło implements Polecenie {
    Światło światło;

    public PolecenieWyłączŚwiatło(Światło światło){
        super();
        this.światło = światło;
    }

    public void wykonaj(){światło.wyłącz();}
    public void wycofaj(){światło.włącz();}
}
