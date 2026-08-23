package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.Jacuzzi;

public class PrzygotujJacuzzi implements Polecenie {
    Jacuzzi jacuzzi;
    public PrzygotujJacuzzi(Jacuzzi jacuzzi){
        super();
        this.jacuzzi = jacuzzi;
    }

    public void wykonaj(){
        jacuzzi.włącz();
        jacuzzi.ustawTemperaturę(30);
        jacuzzi.cyrkulacja();
        jacuzzi.włączDysze();
    }

    public void wycofaj(){
        jacuzzi.wyłącz();
    }
}
