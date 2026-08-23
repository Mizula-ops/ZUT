package pl.edu.zut.wo.wzorce.cafe.składniki;

import pl.edu.zut.wo.wzorce.cafe.napoje.Napój;

public class MleczkoSojowe extends SkładnikDekorator{
    Napój napój;
    public MleczkoSojowe(Napój n){
        napój = n;
    }
    public String pobierzOpis(){
        return napój.pobierzOpis() + " Mleczko Sojowe" ;
    }


    public double koszt() {
        return napój.koszt() +0.15;
    }
}
