package pl.edu.zut.wo.wzorce.symulator;

public class Kaczka {
    LatanieInterfejs latanieInterfejs;
    KwakanieInterfejs kwakanieInterfejs;
//    void kwacz() {
//    	System.out.println("Kwaczę kwa kwa kwa.");
//    }
    void pływaj() {
    	System.out.println("Pływam jak kaczka.");
    }
    void wyświetl() {
    	System.out.println("Wyglądam jak kaczka.");
    }
//    void leć() {
//    	System.out.println("Latam jak kaczka.");
//    }
    void wykonajLeć(){
        latanieInterfejs.leć();
    }

    void wykonajKwacz(){
        kwakanieInterfejs.kwacz();
    }

}
