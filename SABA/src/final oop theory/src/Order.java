import java.io.*;

public class Order {
    private static final String ORDERS_FILE = "orders.txt";
    static {
        try {
            File file = new File(ORDERS_FILE);
            if (file.createNewFile()) {
                System.out.println("File created: " + ORDERS_FILE);
            }
        } catch (IOException e) {
            System.out.println("Error initializing orders file: " + e.getMessage());
        }
    }


    public void placeOrder(String customerName, String item, int quantity) {
        Inventory inventoryHandler = new Inventory();

        // Check if enough stock is available
        if (inventoryHandler.getAvailableStock(item) < quantity) {
            System.out.println("Not enough stock available. Try a lower quantity.");
            return;
        }

        // Get the item price
        int price = getItemPrice(item);
        if (price == -1) {
            System.out.println("Item not found in inventory.");
            return;
        }

        // Calculate total price
        int totalPrice = price * quantity;

        // Save the order, update inventory, and generate a receipt
        saveOrder(customerName, item, quantity, totalPrice);
        inventoryHandler.updateInventoryStock(item, quantity);
        generateReceipt(customerName, item, quantity, totalPrice);
    }

    private int getItemPrice(String item) {
        // Mocked item prices for demonstration
        if (item.equalsIgnoreCase("item1")) return 50;
        if (item.equalsIgnoreCase("item2")) return 30;
        return -1; // Item not found
    }

    private void saveOrder(String customerName, String item, int quantity, int totalPrice) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ORDERS_FILE, true))) {
            // Write order details to the 'orders.txt' file
            bw.write("Customer: " + customerName + ", Item: " + item + ", Quantity: " + quantity + ", Total Price: " + totalPrice);
            bw.newLine();
        } catch (IOException e) {
            System.out.println("Error saving the order: " + e.getMessage());
        }
    }

    private void generateReceipt(String customerName, String item, int quantity, int totalPrice) {
        // Display the receipt
        System.out.println("Receipt:");
        System.out.println("Customer Name: " + customerName);
        System.out.println("Item: " + item);
        System.out.println("Quantity: " + quantity);
        System.out.println("Total Price: $" + totalPrice);
        System.out.println("Thank you for your order!");
    }
}
