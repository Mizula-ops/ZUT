
class OfertaPodwykonawcy:
    licznik = 0

    def __init__(self, dostepnosc, cena,podwykonawca):
        OfertaPodwykonawcy.licznik += 1
        self.id_oferty = OfertaPodwykonawcy.licznik
        self.dostepnosc = dostepnosc
        self.cena = cena
        self.podwykonawca=podwykonawca
   
    def __str__(self):
        return (
            f"ID Oferty: {self.id_oferty}\n"
            f"Dostępność: {self.dostepnosc}\n"
            f"Cena: {self.cena}\n"
            f"Podwykonawca: {self.podwykonawca}"
        )
