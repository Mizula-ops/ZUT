package pl.edu.zut.wo.wzorce.cafe.napoje;

public abstract class Napój {
	protected String opis;

	public Napój() {
		opis = "Nieznany napój";
	}

	public String pobierzOpis() {
		return opis;
	}

	public abstract double koszt();
}