package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.WieżaStereo;

public class WłączWieżaStereo implements Polecenie{
    WieżaStereo wieżaStereo;
    public WłączWieżaStereo(WieżaStereo wieżaStereo){
        super();
        this.wieżaStereo = wieżaStereo;
    }
    public void wykonaj(){
        wieżaStereo.włącz();
    }

    public void wycofaj(){
        wieżaStereo.wyłącz();
    }
}
