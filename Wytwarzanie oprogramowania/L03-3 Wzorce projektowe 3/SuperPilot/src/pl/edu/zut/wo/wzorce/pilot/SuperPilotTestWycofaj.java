package pl.edu.zut.wo.wzorce.pilot;

import pl.edu.zut.wo.wzorce.pilot.polecenia.*;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.DrzwiGarażu;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.WentylatorSufitowy;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.WieżaStereo;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.Światło;

public class SuperPilotTestWycofaj {
    public static void main(String[] args) {
        SuperPilot superPilot = new SuperPilot();

        Światło salonŚwiatło = new Światło("Salon");
        Światło kuchniaŚwiatło = new Światło("Kuchnia");
        WentylatorSufitowy wentylatorSufitowy = new WentylatorSufitowy("Salon");
        DrzwiGarażu drzwiGarażu = new DrzwiGarażu("Garaż");
        WieżaStereo wieżaStereo = new WieżaStereo("Salon");

        Polecenie salonŚwiatłoWłącz = new PolecenieWłączŚwiatło(salonŚwiatło);
        Polecenie salonŚwiatłoWyłącz = new PolecenieWyłączŚwiatło(salonŚwiatło);

        Polecenie kuchniaŚwiatłoWłącz = new PolecenieWłączŚwiatło(kuchniaŚwiatło);
        Polecenie kuchniaŚwiatłoWyłącz = new PolecenieWyłączŚwiatło(kuchniaŚwiatło);

        Polecenie wentylatorSufitowyWłącz = new WentylatorSufitowyWysokieObroty(wentylatorSufitowy);
        Polecenie wentylatofSufitowyWyłącz = new WyłączWentylatorSufitowy(wentylatorSufitowy);

        Polecenie drzwiGarażuOtwórz = new OtwórzDrzwiGarażowe(drzwiGarażu);
        Polecenie drzwiGarażuZamknij = new ZamknijDrzwiGarażowe(drzwiGarażu);

        Polecenie wieżaStereoWłączCD = new WieżaStereoWłączCD(wieżaStereo);
        Polecenie wyłączWieżaStereo = new WyłączWieżaStereo(wieżaStereo);

        superPilot.ustawPolecenie(0, salonŚwiatłoWłącz, salonŚwiatłoWyłącz);
        superPilot.ustawPolecenie(1,kuchniaŚwiatłoWłącz,kuchniaŚwiatłoWyłącz);
        superPilot.ustawPolecenie(2,wentylatorSufitowyWłącz,wentylatofSufitowyWyłącz);
        superPilot.ustawPolecenie(3,drzwiGarażuOtwórz,drzwiGarażuZamknij);
        superPilot.ustawPolecenie(4,wieżaStereoWłączCD,wyłączWieżaStereo);

        System.out.println(superPilot);

        superPilot.wciśniętoPrzyciskWłącz(0);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWyłącz(0);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWłącz(1);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWyłącz(1);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWłącz(2);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWyłącz(2);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWłącz(3);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWyłącz(3);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWłącz(4);
        superPilot.wciśniętoPrzyciskWycofaj();
        superPilot.wciśniętoPrzyciskWyłącz(4);
        superPilot.wciśniętoPrzyciskWycofaj();

    }
}
