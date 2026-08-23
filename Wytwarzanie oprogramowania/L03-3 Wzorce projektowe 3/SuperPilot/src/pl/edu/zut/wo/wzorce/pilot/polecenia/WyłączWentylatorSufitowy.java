package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.WentylatorSufitowy;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.Światło;

public class WyłączWentylatorSufitowy implements Polecenie {
    WentylatorSufitowy wentylatorSufitowy;
    public WyłączWentylatorSufitowy(WentylatorSufitowy wentylatorSufitowy){
        super();
        this.wentylatorSufitowy = wentylatorSufitowy;
    }

    public void wykonaj(){wentylatorSufitowy.off();}
    public void wycofaj(){wentylatorSufitowy.szybko();}
}
