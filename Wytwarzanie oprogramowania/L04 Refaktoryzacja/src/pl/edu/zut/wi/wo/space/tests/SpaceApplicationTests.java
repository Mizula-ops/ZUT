package pl.edu.zut.wi.wo.space.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import pl.edu.zut.wi.wo.space.Person;
import pl.edu.zut.wi.wo.space.Ship;
import pl.edu.zut.wi.wo.space.Space;

public class SpaceApplicationTests {

	@Test
	public void test() {
		Space space = new Space();
		space.setDefaultOwner(new Person("Martin", "Fowler"));
		Ship spaceShip = new Ship(space.getDefaultOwner());
		assertEquals("Martin", spaceShip.getOwner().getFirstName());
		assertEquals("Fowler", spaceShip.getOwner().getLastName());
	}

}
