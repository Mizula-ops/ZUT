package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.TV;

public class PrzełączTVDVD implements Polecenie{
    TV tv;
    public PrzełączTVDVD(TV tv) {
        super();
        this.tv = tv;
    }

    public void wykonaj(){
        tv.ustawDVD();
    }

    public void wycofaj(){
    }
}
