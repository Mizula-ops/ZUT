package pl.edu.zut.wo.wzorce.pogodynka;

import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.IndeksCiepłaWyświetlanie;
import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.PrognozaWyświetlanie;
import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.StatystykaWyświetlanie;
import pl.edu.zut.wo.wzorce.pogodynka.wyświetl.WarunkiBieżąceWyświetlanie;

public class StacjaMeteo {

	public static void main(String[] args) {
		DanePogodowe danePogodowe = new DanePogodowe();
//		danePogodowe.ustawOdczyty(26.6f, 65, 1013.1f);
//		danePogodowe.ustawOdczyty(27.7f, 70, 997.0f);
//		danePogodowe.ustawOdczyty(25.5f, 90, 997.0f);
        WarunkiBieżąceWyświetlanie warunkiBieżące = new WarunkiBieżąceWyświetlanie(danePogodowe);
        StatystykaWyświetlanie statystyka = new StatystykaWyświetlanie(danePogodowe);
        PrognozaWyświetlanie prognoza = new PrognozaWyświetlanie(danePogodowe);
        IndeksCiepłaWyświetlanie indeksCiepła = new IndeksCiepłaWyświetlanie(danePogodowe);

        danePogodowe.ustawOdczyty(26.6f,65,1013.1f);
        System.out.println();
        danePogodowe.ustawOdczyty(27.7f,70,997.0f);
        System.out.println();
        danePogodowe.usuńObserwatora(prognoza);
        danePogodowe.ustawOdczyty(25.5f,90,997.0f);

	}
	
}