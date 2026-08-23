package pl.edu.zut.wo.wzorce.pizzeria;

import pl.edu.zut.wo.wzorce.pizzeria.metoda_fabrykująca.AmerykańskaPizzeria;
import pl.edu.zut.wo.wzorce.pizzeria.metoda_fabrykująca.WłoskaPizzeria;
import pl.edu.zut.wo.wzorce.pizzeria.pizza.Pizza;

public class TestPizzerii {

	public static void main(String[] args) {
		Pizzeria1 pizzeria = new Pizzeria1();
		Pizza pizza = pizzeria.zamówPizza("serowa");
		System.out.println(pizzeria.getClass().getSimpleName() + " wyprodukowała " + pizza.getClass().getSimpleName());
        WłoskaPizzeria pizzeria2 = new WłoskaPizzeria();
        Pizza pizza2 = pizzeria2.zamówPizza("serowa");
        System.out.println(pizzeria2.getClass().getSimpleName() + " wyprodukowała " + pizza2.getClass().getSimpleName());
        AmerykańskaPizzeria pizzeria3 = new AmerykańskaPizzeria();
        Pizza pizza3 = pizzeria3.zamówPizza("serowa");
        System.out.println(pizzeria3.getClass().getSimpleName() + " wyprodukowała " + pizza3.getClass().getSimpleName());

    }
}
