package pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.eager;


public class Singleton {
	private static Singleton unikalnaInstancja = new Singleton();

	// tutaj umieść inne użyteczne zmienne obiektowe
	private Singleton() {
	}

	public static Singleton pobierzInstancję() {
		return unikalnaInstancja;
	}
	// tutaj umieść inne użyteczne metody
}