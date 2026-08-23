package pl.edu.zut.wo.wzorce.pogodynka;

import java.io.BufferedWriter;
import java.util.ArrayList;

import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.PrognozaWyświetlanie;
import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.StatystykaWyświetlanie;
import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.WarunkiBieżąceWyświetlanie;

public class DanePogodowe implements Podmiot{
	private float temperatura;
	private float wilgotność;
	private float ciśnienie;
	
//	private PrognozaWyświetlanie prognozaWyświetl = new PrognozaWyświetlanie();
//	private WarunkiBieżąceWyświetlanie warunkiBieżąceWyświetl = new WarunkiBieżąceWyświetlanie();
//	private StatystykaWyświetlanie statystykaWyświetl = new StatystykaWyświetlanie();

    private ArrayList<Obserwator> obserwatorzy = new ArrayList<>();

	public void odczytyZmiana(){
//		float temp = pobierzTemperaturę();
//		float wilgotność = pobierzWilgotność();
//		float ciśnienie = pobierzCiśnienie();
//		warunkiBieżąceWyświetl.aktualizacja(temp, wilgotność, ciśnienie);
//		statystykaWyświetl.aktualizacja(temp, wilgotność, ciśnienie);
//		prognozaWyświetl.aktualizacja(temp, wilgotność, ciśnienie);
        powiadomObserwatorów();
	}

	private float pobierzTemperaturę(){
		return temperatura;
	}

	private float pobierzWilgotność(){
		return wilgotność;
	}

	private float pobierzCiśnienie(){
		return ciśnienie;
	}
	
	public void ustawOdczyty(float temperatura, float wilgotność, float ciśnienie) {
		this.temperatura = temperatura;
		this.wilgotność = wilgotność;
		this.ciśnienie = ciśnienie;
		odczytyZmiana();
	}

    public void zarejestrujObserwatora(Obserwator o){
        obserwatorzy.add(o);
    }
    public void usuńObserwatora(Obserwator o){
        obserwatorzy.remove(o);
    }
    public void powiadomObserwatorów(){
        for (Obserwator o : obserwatorzy){
            o.aktualizacja(temperatura,wilgotność,ciśnienie);
        }
    }
}
