package pl.edu.zut.wi.wo.order.tests;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import pl.edu.zut.wi.wo.order.Order;
import pl.edu.zut.wi.wo.order.OrderCalculator;

public class OrderTests {

	@Test
	public void testPrice() {
		Order order = new Order(10, 100); 
		assertEquals("Price should be 1100.00", OrderCalculator.price(order), 1100, 0);
	}

}
