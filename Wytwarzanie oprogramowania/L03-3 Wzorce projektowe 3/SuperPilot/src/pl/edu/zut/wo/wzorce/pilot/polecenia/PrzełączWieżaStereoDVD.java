package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.WieżaStereo;

public class PrzełączWieżaStereoDVD implements Polecenie {
    WieżaStereo wieżaStereo;

    public PrzełączWieżaStereoDVD(WieżaStereo wieżaStereo) {
        super();
        this.wieżaStereo = wieżaStereo;
    }

    public void wykonaj() {
        wieżaStereo.ustawDVD();
    }

    public void wycofaj() {
    }
}