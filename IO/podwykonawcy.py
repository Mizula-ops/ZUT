
class Podwykonawcy:
    def __init__ (self,imie,nazwa_firmy,email,telefon):
        self.imie=imie
        self.nazwa_firmy=nazwa_firmy
        self.email=email
        self.telefon=telefon
    def get_imie(self):
        return self.imie

    def __str__(self):
        return (
            f"Imię: {self.imie}\n"
            f"Nazwa firmy: {self.nazwa_firmy}\n"
            f"Email: {self.email}\n"
            f"Telefon: {self.telefon}"
        )



