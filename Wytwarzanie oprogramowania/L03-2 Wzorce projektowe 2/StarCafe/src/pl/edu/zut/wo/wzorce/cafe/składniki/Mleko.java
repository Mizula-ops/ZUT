package pl.edu.zut.wo.wzorce.cafe.składniki;

import pl.edu.zut.wo.wzorce.cafe.napoje.Napój;

public class Mleko extends SkładnikDekorator{
    Napój napój;
    public Mleko(Napój n){
        napój = n;
    }
    public String pobierzOpis(){
        return napój.pobierzOpis() + " Mleko" ;
    }


    public double koszt() {
        return napój.koszt() +0.1;
    }
}
