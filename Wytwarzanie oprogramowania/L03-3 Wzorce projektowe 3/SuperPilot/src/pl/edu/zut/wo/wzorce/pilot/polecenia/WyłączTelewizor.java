package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.TV;

public class WyłączTelewizor implements Polecenie{
    TV tv;

    public WyłączTelewizor(TV tv){
        super();
        this.tv = tv;
    }

    public void wykonaj() {
        tv.wyłącz();
    }

    public void wycofaj(){
        tv.włącz();
    }
}
