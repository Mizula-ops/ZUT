class Usluga:
    licznik_id = 0

    def __init__(self, name, typ, opis, cena):
        Usluga.licznik_id += 1
        self.id_uslugi = Usluga.licznik_id
        self.name = name
        self.typ = typ
        self.opis = opis
        self.cena = cena
        self.podwykonawcy = []

    def prize(self):
        return self.cena

    def get_nazwa(self):
        return self.name

    def dodaj_podwykonawce(self, podwykonawca):
        self.podwykonawcy.append(podwykonawca)
        return(f"Dodano podwykonawce do usługi {self.name}")

    def __str__(self):
        return (
            f"Usluga: {self.name}\n"
            f"Dziedzina: {self.typ}\n"
            f"Opis: {self.opis}\n"
            f"Cena: {self.cena}\n"
            f"Podwykonawcy: {[str(p) for p in self.podwykonawcy]}"
        )




