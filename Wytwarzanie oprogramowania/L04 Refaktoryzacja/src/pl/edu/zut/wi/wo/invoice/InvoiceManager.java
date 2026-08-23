package pl.edu.zut.wi.wo.invoice;

import java.time.LocalDate;

public class InvoiceManager {

	public static void printOwing(Invoice invoice) {
		double outstanding = 0;
        printHeader();
        outstanding = calculateOwning(invoice, outstanding);
        setDate(invoice);

        printDetails(invoice, outstanding);
    }

    private static void printDetails(Invoice invoice, double outstanding) {
        System.out.println(String.format("nazwa: %s", invoice.getCustomer()));
        System.out.println(String.format("kwota: %.2f", outstanding));
        System.out.println(String.format("płatność do: %s", invoice.getDueDate()));
    }

    private static void setDate(Invoice invoice) {
        LocalDate today = LocalDate.now();
        invoice.setDueDate(today.plusDays(30));
    }

    private static double calculateOwning(Invoice invoice, double outstanding) {
        for (Order o : invoice.getOrders()) {
            outstanding += o.getAmount();
        }
        return outstanding;
    }

    private static void printHeader() {
        System.out.println("************************");
        System.out.println("* Rachunek dla klienta *");
        System.out.println("************************");
    }

}
