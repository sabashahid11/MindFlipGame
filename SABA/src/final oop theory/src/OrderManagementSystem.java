import java.util.Scanner;

public class OrderManagementSystem {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Staff staffHandler = new Staff();
        Customer customerHandler = new Customer();

        while (true) {
            System.out.println("Choose what you want to be:");
            System.out.println("1. Customer");
            System.out.println("2. Staff");
            System.out.println("3. Exit");
            System.out.println("(Enter 'staff', 'customer', or 'exit')");
            String role = scanner.nextLine();

            if (role.equals("exit")) {
                System.out.println("Exiting the system. Goodbye!");
                break;
            } else if (role.equals("staff")) {
                staffHandler.handleStaff(scanner);
            } else if (role.equals("customer")) {
                customerHandler.handleCustomer(scanner);
            } else {
                System.out.println("Invalid input. Please enter 'staff', 'customer', or 'exit'.");
            }
        }
        scanner.close();
    }
}













