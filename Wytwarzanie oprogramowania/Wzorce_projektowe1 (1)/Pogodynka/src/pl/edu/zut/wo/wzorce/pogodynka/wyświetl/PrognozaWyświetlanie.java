package pl.edu.zut.wo.wzorce.pogodynka.wyświetl;

import pl.edu.zut.wo.wzorce.pogodynka.Obserwator;
import pl.edu.zut.wo.wzorce.pogodynka.Podmiot;
import pl.edu.zut.wo.wzorce.pogodynka.WyświetlElement;

public class PrognozaWyświetlanie implements Obserwator, WyświetlElement {

	private float temperatura;
	private float wilgotność;
	private float ciśnienie;

    private Podmiot danePogodowe;

    public PrognozaWyświetlanie(Podmiot danePogodowe) {
        this.danePogodowe = danePogodowe;
        danePogodowe.zarejestrujObserwatora(this);
    }

	public void aktualizacja(float temperatura, float wilgotność, float ciśnienie) {
		this.temperatura = temperatura;
		this.wilgotność = wilgotność;
		this.ciśnienie = ciśnienie;
		wyświetl();
	}
	
	public void wyświetl() {
		System.out.println("Prognoza " + temperatura + " stopni C oraz " + wilgotność + "% wilgotność");
	}

}