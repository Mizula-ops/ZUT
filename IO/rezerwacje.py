class Rezerwacja:
    def __init__ (self,imie,email,status,data_rezerwacji,koszt,nazwa_uslugi):
        self.imie = imie
        self.data = data_rezerwacji
        self.status=status
        self.koszt=koszt
        self.nazwa_uslugi=nazwa_uslugi
    def anuluj_rezerwacje(self):
        self.status="Anulowana"
        self.data=None
        print(f"Rezerwacja dla {self.imie} zostala anulowana")
    def __str__(self):
        return(f"Rezerawacja dla {self.imie} jest {self.status}\n"
        f"Dzien wykonania {self.data}.\n"
        f"Całkowitykoszt: {self.koszt}.\n")


