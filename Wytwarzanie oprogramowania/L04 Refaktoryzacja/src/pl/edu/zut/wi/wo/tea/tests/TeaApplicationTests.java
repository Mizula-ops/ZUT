package pl.edu.zut.wi.wo.tea.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import pl.edu.zut.wi.wo.tea.TeaApplication;


public class TeaApplicationTests {
    @Test
    public void testTransformata() {
        TeaApplication app = new TeaApplication();
        app.transformata();

        assertEquals(125.0, TeaApplication.example1, 0);
        assertEquals(25.0, TeaApplication.example2, 0);
        assertEquals(125.0, TeaApplication.example3, 0);
    }

}
