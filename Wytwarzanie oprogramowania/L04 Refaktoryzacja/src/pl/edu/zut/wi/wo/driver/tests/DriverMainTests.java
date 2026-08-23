package pl.edu.zut.wi.wo.driver.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import pl.edu.zut.wi.wo.driver.Driver;
import pl.edu.zut.wi.wo.driver.DriverMain;

public class DriverMainTests {

	@Test
	public void test() {
		DriverMain dm = new DriverMain();
		Driver driver = new Driver("jan kowalski", 6);
		assertEquals(2, dm.rating(driver));
	}

}
