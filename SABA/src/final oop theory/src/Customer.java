import java.io.*;
import java.util.Scanner;

public class Customer {

    public void handleCustomer(Scanner scanner) {
        String fileName = "customer.txt";
        try {
            File file = new File(fileName);
            if (file.createNewFile()) {
                System.out.println("File created: " + fileName);
            }
        } catch (IOException e) {
            System.out.println("Error creating file: " + e.getMessage());
        }

        Inventory inventoryHandler = new Inventory();
        Order orderHandler = new Order();

        System.out.println("Available items:");
        inventoryHandler.displayInventory();

        System.out.print("Enter your name: ");
        String customerName = scanner.nextLine();

        System.out.print("Enter the item you want to order: ");
        String item = scanner.nextLine();

        System.out.print("Enter the quantity: ");
        try {
            int quantity = Integer.parseInt(scanner.nextLine());
            orderHandler.placeOrder(customerName, item, quantity);


            saveCustomerDetailsToFile(customerName, item, quantity);
        } catch (NumberFormatException e) {
            System.out.println("Invalid quantity. Please enter a valid number.");
        }
    }


    private void saveCustomerDetailsToFile(String customerName, String item, int quantity) {
        String fileName = "customer.txt";
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, true))) { // Append mode
            writer.write("Customer Name: " + customerName);
            writer.newLine();
            writer.write("Item: " + item);
            writer.newLine();
            writer.write("Quantity: " + quantity);
            writer.newLine();
            writer.write("---------------");
            writer.newLine();
            System.out.println("Customer details saved to " + fileName);
        } catch (IOException e) {
            System.out.println("Error writing to file: " + e.getMessage());
        }
    }
}

