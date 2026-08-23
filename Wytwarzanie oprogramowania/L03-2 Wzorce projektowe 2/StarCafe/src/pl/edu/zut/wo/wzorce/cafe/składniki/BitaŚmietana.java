package pl.edu.zut.wo.wzorce.cafe.składniki;

import pl.edu.zut.wo.wzorce.cafe.napoje.Napój;

import javax.sql.rowset.serial.SerialJavaObject;

public class BitaŚmietana extends SkładnikDekorator {
    Napój napój;
    public BitaŚmietana(Napój n){
        napój = n;
    }
    public String pobierzOpis(){
        return napój.pobierzOpis() + " Bita Śmietana" ;
    }


    public double koszt() {
        return napój.koszt() +0.10;
    }
}
