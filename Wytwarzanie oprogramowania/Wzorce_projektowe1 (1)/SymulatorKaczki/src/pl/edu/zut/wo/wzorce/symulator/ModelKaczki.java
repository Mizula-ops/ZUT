package pl.edu.zut.wo.wzorce.symulator;

public class ModelKaczki extends Kaczka {
    void ustawKwaczInt(KwakanieInterfejs klasaImplementująca){
        kwakanieInterfejs =  klasaImplementująca;
    }

    void ustawLećInt(LatanieInterfejs klasaImplementująca){
        latanieInterfejs = klasaImplementująca;
    }

    void wyświetl(){
        System.out.println("Jestem modelem kaczki");
    }
}
