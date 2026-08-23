package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.Jacuzzi;

public class WyłączJacuzzi implements Polecenie{
    Jacuzzi jacuzzi;
    public WyłączJacuzzi(Jacuzzi jacuzzi) {
        super();
        this.jacuzzi = jacuzzi;
    }

    public void wykonaj(){
        jacuzzi.wyłącz();
    }

    public void wycofaj(){
        jacuzzi.włącz();
        jacuzzi.ustawTemperaturę(30);
        jacuzzi.cyrkulacja();
        jacuzzi.włączDysze();
    }
}
