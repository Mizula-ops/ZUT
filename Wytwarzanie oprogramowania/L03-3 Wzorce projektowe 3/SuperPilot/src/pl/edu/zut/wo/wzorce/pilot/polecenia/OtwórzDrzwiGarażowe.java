package pl.edu.zut.wo.wzorce.pilot.polecenia;

import pl.edu.zut.wo.wzorce.pilot.sterowniki.DrzwiGarażu;

public class OtwórzDrzwiGarażowe implements Polecenie {
    DrzwiGarażu drzwiGarażu;

    public OtwórzDrzwiGarażowe(DrzwiGarażu drzwiGarażu) {
        super();
        this.drzwiGarażu = drzwiGarażu;
    }

    public void wykonaj() {
        drzwiGarażu.podnieś();
    }

    public void wycofaj() {
        drzwiGarażu.opuść();
    }
}
