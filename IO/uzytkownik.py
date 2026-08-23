class User:
    def __init__(self, name,nazwisko, email, password, role):
        self.name = name
        self.nazwisko=nazwisko
        self.email = email
        self.password = password
        self.role = role

    def zaloguj_sie(self, email, password):
        if self.password == password and self.email == email:
            print(f"Witaj z powrotem: {self.name}")
            return 1
        else:
            print("Nieprawidłowe dane logowania")
            return 0
    def get_imie(self):
        return self.name
    def get_email(self):
        return self.email
    def __str__(self):
        return (
            f"Imie: {self.name}\n"
            f"Nazwisko: {self.nazwisko}\n"
            f"Email: {self.email}\n"
            f"Rola: {self.role}"
        )

