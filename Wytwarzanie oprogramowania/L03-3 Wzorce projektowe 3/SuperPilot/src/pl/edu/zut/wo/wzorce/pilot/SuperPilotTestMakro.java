package pl.edu.zut.wo.wzorce.pilot;

import pl.edu.zut.wo.wzorce.pilot.polecenia.*;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.Jacuzzi;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.TV;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.WieżaStereo;
import pl.edu.zut.wo.wzorce.pilot.sterowniki.Światło;

public class SuperPilotTestMakro {
    public static void main(String[] args) {
        SuperPilot superPilot = new SuperPilot();
        Światło salonŚwiatło = new Światło("Salon");
        Światło kuchniaŚwiatło = new Światło("Kuchnia");
        WieżaStereo wieżaStereo = new WieżaStereo("Salon");
        TV tv = new TV("Salon");
        Jacuzzi jacuzzi = new Jacuzzi();

        Polecenie salonŚwiatłoWłącz = new PolecenieWłączŚwiatło(salonŚwiatło);
        Polecenie salonŚwiatłoWyłącz = new PolecenieWyłączŚwiatło(kuchniaŚwiatło);

        Polecenie kuchniaŚwiatłoWłącz = new PolecenieWłączŚwiatło(kuchniaŚwiatło);
        Polecenie kuchniaŚwiatłoWyłącz = new PolecenieWyłączŚwiatło(kuchniaŚwiatło);

        Polecenie[] makroWłączŚwiatła = {salonŚwiatłoWłącz,kuchniaŚwiatłoWłącz};
        MakroPolecenie włączWszystkieŚwiatła = new MakroPolecenie(makroWłączŚwiatła);
        Polecenie[] makroWyłączŚwiatła = {salonŚwiatłoWyłącz, kuchniaŚwiatłoWyłącz};
        MakroPolecenie wyłaczWszystkieŚwiatła = new MakroPolecenie(makroWyłączŚwiatła);
//        System.out.println(superPilot);

        Polecenie przyciemnijOświetlenieSalon = new PrzyciemnijOświetlenie(salonŚwiatło);
        Polecenie przyciemnijOświetlenieKuchnia = new PrzyciemnijOświetlenie(kuchniaŚwiatło);
        Polecenie włączWieżaStereo = new WłączWieżaStereo(wieżaStereo);
        Polecenie włączTelewizor = new WłączTelewizor(tv);
        Polecenie przełączWieżaStereoDVD = new PrzełączWieżaStereoDVD(wieżaStereo);
        Polecenie przełączTVDVD = new PrzełączTVDVD(tv);
        Polecenie przygotujJacuzzi = new PrzygotujJacuzzi(jacuzzi);
        Polecenie[] przygotujImpreze = {przyciemnijOświetlenieSalon, przyciemnijOświetlenieKuchnia,włączWieżaStereo,włączTelewizor,przełączWieżaStereoDVD,przełączTVDVD,przygotujJacuzzi};
        MakroPolecenie makroImprezaStart = new MakroPolecenie(przygotujImpreze);

        Polecenie wyłączJacuzzi = new WyłączJacuzzi(jacuzzi);
        Polecenie wyłączTV = new WyłączTelewizor(tv);
        Polecenie wyłączWieżaStereo = new WyłączWieżaStereo(wieżaStereo);
        Polecenie rozjaśnijOświetlenieSalon = new RozjaśnijOświetlenie(salonŚwiatło);
        Polecenie rozjaśnijOświetlenieKuchnia = new RozjaśnijOświetlenie(kuchniaŚwiatło);
        Polecenie[] makroImprezaZakończ = {wyłączJacuzzi,wyłączTV,wyłączWieżaStereo,rozjaśnijOświetlenieSalon,rozjaśnijOświetlenieKuchnia};
        MakroPolecenie makroImprezaStop = new MakroPolecenie(makroImprezaZakończ);

        superPilot.ustawPolecenie(0, włączWszystkieŚwiatła, wyłaczWszystkieŚwiatła);
        superPilot.ustawPolecenie(1,makroImprezaStart,makroImprezaStop);
        superPilot.wciśniętoPrzyciskWłącz(0);
        System.out.println();
        superPilot.wciśniętoPrzyciskWłącz(1);
        System.out.println();
        superPilot.wciśniętoPrzyciskWyłącz(1);
        System.out.println();
        superPilot.wciśniętoPrzyciskWyłącz(0);

    }
}
