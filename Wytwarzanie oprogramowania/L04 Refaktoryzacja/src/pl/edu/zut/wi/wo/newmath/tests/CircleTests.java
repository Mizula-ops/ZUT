package pl.edu.zut.wi.wo.newmath.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import pl.edu.zut.wi.wo.newmath.Circle;

public class CircleTests {

	@Test
	public void testCircum() {
		assertEquals(12.56, Circle.circumference(2), 0.01);
	}

}
