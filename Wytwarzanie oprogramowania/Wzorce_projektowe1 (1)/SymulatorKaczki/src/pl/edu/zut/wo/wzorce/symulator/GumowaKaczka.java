package pl.edu.zut.wo.wzorce.symulator;


public class GumowaKaczka extends Kaczka {
    public GumowaKaczka() {
        latanieInterfejs = new NieLatam();
        kwakanieInterfejs = new Piszcz();
    }

//	void kwacz() {
//		System.out.println("Piszczenie pi, pi.");
//
//	}

	public void wyświetl(){
		System.out.println("Wygląda jak gumowa kaczka");
	}
}