package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.WentylatorSufitowy;

public class WentylatorSufitowyWysokieObroty implements Polecenie{
    WentylatorSufitowy wentylatorSufitowy;

    public WentylatorSufitowyWysokieObroty(WentylatorSufitowy wentylatorSufitowy) {
        super();
        this.wentylatorSufitowy = wentylatorSufitowy;
    }

    public void wykonaj(){wentylatorSufitowy.szybko();}
    public void wycofaj(){wentylatorSufitowy.off();}
}
