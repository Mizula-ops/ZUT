package pl.edu.zut.wo.wzorce.symulator;

public class WabikKaczka extends Kaczka{
    public WabikKaczka() {
        latanieInterfejs = new NieLatam();
        kwakanieInterfejs = new NieKwacz();
    }
    void wyświetl(){
        System.out.println("Wyglądam jak wabik");
    }
}
