package pl.edu.zut.wi.wo.order;

public class OrderCalculator {

	public static double price(Order order) {
		// Cena końcowa = cena bazowa – upust + koszt wysyłki.
        final double basedPrice = order.getQuantity() * order.getItemPrice();
        final double discount = Math.max(0, order.getQuantity() - 500) * order.getItemPrice() * 0.05;
        final double deliveryCost = Math.min(order.getQuantity() * order.getItemPrice() * 0.1, 100);
        return basedPrice
				- discount
				+ deliveryCost;
	}
}
