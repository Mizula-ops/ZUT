package pl.edu.zut.wo.wzorce.symulator;

public class MiniSymulatorKaczki {

	public static void main(String[] args) {
		Kaczka dzika = new DzikaKaczka();
		dzika.wyświetl();
		dzika.wykonajKwacz();
		dzika.wykonajLeć();
		Kaczka gumowa = new GumowaKaczka();
		gumowa.wyświetl();
		gumowa.wykonajKwacz();
		gumowa.wykonajLeć();
        ModelKaczki modelKaczki = new ModelKaczki();
        modelKaczki.wyświetl();
        modelKaczki.ustawKwaczInt(new Kwacz());
        modelKaczki.wykonajKwacz();
        modelKaczki.ustawLećInt(new LotZNapędemRakietowym());
        modelKaczki.wykonajLeć();
        modelKaczki.ustawLećInt(new NieLatam());
        modelKaczki.wykonajLeć();
	}
}
