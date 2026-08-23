
package pl.edu.zut.wo.wzorce.fabryka_czekolady;
import pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.double_lock.CzekoladowyKociołDL;
import pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.eager.CzekoladowyKociołEager;
import pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.jbloch.CzekoladowyKociołEnum;
import pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.klasyczny.CzekoladowyKociołKlasyczny;
import pl.edu.zut.wo.wzorce.fabryka_czekolady.singleton.synchro.CzekoladowyKociołSynchro;


public class FabrykaCzekolady {

	public static void main(String... params) {
		CzekoladowyKociołKlasyczny kocioł = CzekoladowyKociołKlasyczny.pobierzInstancję();
		kocioł.napełniaj();
		kocioł.gotuj();
		kocioł.opróżniaj();

        CzekoladowyKociołSynchro kociołSynchro = CzekoladowyKociołSynchro.pobierzInstancję();
        kociołSynchro.napełniaj();
        kociołSynchro.gotuj();
        kociołSynchro.opróżniaj();

        CzekoladowyKociołEager kociołEager = CzekoladowyKociołEager.pobierzInstancję();
        kociołEager.napełniaj();
        kociołEager.gotuj();
        kociołEager.opróżniaj();

        CzekoladowyKociołDL kociołDL = CzekoladowyKociołDL.pobierzInstancję();
        kociołDL.napełniaj();
        kociołDL.gotuj();
        kociołDL.opróżniaj();

        CzekoladowyKociołEnum kociołEnum = CzekoladowyKociołEnum.INSTANCJA;
        kociołEnum.napełniaj();
        kociołEnum.gotuj();
        kociołEnum.opróżniaj();
	}
}
