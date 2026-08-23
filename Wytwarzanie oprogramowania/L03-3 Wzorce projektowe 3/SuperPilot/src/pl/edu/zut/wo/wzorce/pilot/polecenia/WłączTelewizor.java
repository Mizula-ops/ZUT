package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.TV;

public class WłączTelewizor implements Polecenie {
    TV tv;
    public WłączTelewizor(TV tv) {
        super();
        this.tv = tv;
    }

    public void wykonaj(){
        tv.włącz();
    }

    public void wycofaj(){
        tv.wyłącz();
    }
}
