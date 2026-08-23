package pl.edu.zut.wo.wzorce.pizzeria;

import pl.edu.zut.wo.wzorce.pizzeria.pizza.Pizza;

public class TestPizzerii2 {

	public static void main(String[] args) {
		WłoskaPizzeria pizzeria = new WłoskaPizzeria();
		Pizza pizza = pizzeria.zamówPizza("owoce morza");
		System.out.println(pizza);
	}
}
