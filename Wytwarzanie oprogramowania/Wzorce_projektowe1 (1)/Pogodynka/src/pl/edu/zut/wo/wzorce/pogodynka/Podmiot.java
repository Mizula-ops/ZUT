package pl.edu.zut.wo.wzorce.pogodynka;

public interface Podmiot {
    public void zarejestrujObserwatora(Obserwator o);
    public void usuńObserwatora(Obserwator o);
    public void powiadomObserwatorów();
}
