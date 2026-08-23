package pl.edu.zut.wi.wo.tea_copy.tea;

public class ReadingRecord {
	String customer;
	int quantity;
	int month;
	int year;

    static double calculateBaseCharge(ReadingRecord aReading) {
        return baseRate(aReading.month, aReading.year) * aReading.quantity;
    }

    static double taxThreshold(int year) {
        return 100;
    }

    static double baseRate(int month, int year) {
        return 12.5;
    }
}
