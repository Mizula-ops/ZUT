
class Faktura:
    def __init__(self, numer, opis):
        self.numer_faktury = numer
        self.plik = opis

    def __str__(self):
        return (
            f"Numer Faktury: {self.numer_faktury}\n"
            f"Opis: {self.plik}"
        )


