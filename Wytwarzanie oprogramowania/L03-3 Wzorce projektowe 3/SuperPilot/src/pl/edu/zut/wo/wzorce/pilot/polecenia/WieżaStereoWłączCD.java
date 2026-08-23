package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.WieżaStereo;

public class WieżaStereoWłączCD implements Polecenie {
    WieżaStereo wieżaStereo;

    public WieżaStereoWłączCD(WieżaStereo wieżaStereo){
        super();
        this.wieżaStereo = wieżaStereo;
    }

    public void wykonaj(){
        wieżaStereo.włącz();
        wieżaStereo.ustawCD();
    }

    public void wycofaj(){
        wieżaStereo.wyłącz();
    }
}
