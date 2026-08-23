package pl.edu.zut.wi.wo.tea_copy.tea.tests;

import org.junit.Test;
import pl.edu.zut.wi.wo.tea_copy.tea.TeaApplication;

import static org.junit.Assert.assertEquals;


public class TeaApplicationTests {

	@Test
	public void testFirstMethod() {
		assertEquals(125.0, TeaApplication.firstMethod(), 0);
	}

	@Test
	public void testSecondMethod() {
		assertEquals(25.0, TeaApplication.secondMethod(), 0);
	}

	@Test
	public void testThirdMethod() {
		assertEquals(125.0, TeaApplication.thirdMethod(), 0);
	}

}
