package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.WieżaStereo;

public class WyłączWieżaStereo implements Polecenie{
    WieżaStereo wieżaStereo;

    public WyłączWieżaStereo(WieżaStereo wieżaStereo){
        super();
        this.wieżaStereo = wieżaStereo;
    }

    public void wykonaj() {
        wieżaStereo.wyłącz();
    }

    public void wycofaj() {
        wieżaStereo.włącz();
        wieżaStereo.ustawCD();
    }
}
