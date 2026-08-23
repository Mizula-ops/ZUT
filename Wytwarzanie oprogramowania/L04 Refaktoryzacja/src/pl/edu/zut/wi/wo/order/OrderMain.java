package pl.edu.zut.wi.wo.order;

public class OrderMain {

	public static void main(String[] args) {
		Order order = new Order(10, 100);
		System.out.println(String.format("Cena koñcowa zamówienia: %.2f", OrderCalculator.price(order)));
	}

}
