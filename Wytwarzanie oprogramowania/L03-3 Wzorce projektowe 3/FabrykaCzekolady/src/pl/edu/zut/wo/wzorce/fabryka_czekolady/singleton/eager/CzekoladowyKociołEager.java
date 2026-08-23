package pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.eager;



public class CzekoladowyKociołEager {
    private boolean pusty;
    private boolean ugotowany;
    private static CzekoladowyKociołEager unikalnaInstancja = new CzekoladowyKociołEager();

    // tutaj umieść inne użyteczne zmienne obiektowe
    private CzekoladowyKociołEager() {
        pusty = true;
        ugotowany = false;
        System.out.println("Utworzenie instancji Czekoladowego Kotła: " + this);
    }

    public static CzekoladowyKociołEager pobierzInstancję() {
        return unikalnaInstancja;
    }
    // tutaj umieść inne użyteczne metody


    public void napełniaj() {
        if (jestPusty()) {
            pusty = false;
            ugotowany = false;
            // napełniaj bojler mieszanką mleka i czekolady
            System.out.println("Napełnianie bojlera mieszanką czekolady i mleka");
        }
    }

    public void opróżniaj() {
        if (!jestPusty() && jestUgotowany()) {
            // opróżniaj bojler z ugotowanej mieszanki mleka i czekolady
            System.out.println("Opróżnianie bojlera z ugotowanej mieszanki czekolady i mleka");
            pusty = true;
        }
    }

    public void gotuj() {
        if (!jestPusty() && !jestUgotowany()) {
            // gotuj zawartość kotła
            System.out.println("Gotowanie mieszanki czekolady i mleka");
            ugotowany = true;
        }
    }

    public boolean jestPusty() {
        return pusty;
    }

    public boolean jestUgotowany() {
        return ugotowany;
    }
}