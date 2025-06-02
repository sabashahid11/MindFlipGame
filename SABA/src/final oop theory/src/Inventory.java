import java.io.*;
import java.util.*;

public class Inventory {
    private static final String INVENTORY_FILE = "inventory.txt";
    private static final String DEFAULT_INVENTORY = "bag,50\nshoes,30\nlipstick,20";

    static {
        try {
            File file = new File(INVENTORY_FILE);
            if (file.createNewFile()) {
                System.out.println("File created: " + INVENTORY_FILE);

                try (BufferedWriter writer = new BufferedWriter(new FileWriter(INVENTORY_FILE))) {
                    writer.write(DEFAULT_INVENTORY);
                    writer.newLine();
                }
            }
        } catch (IOException e) {
            System.out.println("Error initializing inventory file: " + e.getMessage());
        }
    }


    public void displayInventory() {
        try (BufferedReader br = new BufferedReader(new FileReader(INVENTORY_FILE))) {
            String line;
            System.out.println("Current Inventory:");
            System.out.println("-----------------");
            while ((line = br.readLine()) != null) {
                System.out.println(line); // Print each item in the inventory
            }
        } catch (IOException e) {
            System.out.println("Error reading inventory file: " + e.getMessage());
        }
    }


    public int getAvailableStock(String item) {
        try (BufferedReader br = new BufferedReader(new FileReader(INVENTORY_FILE))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 2) {
                    String itemName = parts[0].trim();
                    int stock = Integer.parseInt(parts[1].trim());
                    if (itemName.equalsIgnoreCase(item)) {
                        return stock; // Return the stock for the specified item
                    }
                }
            }
        } catch (IOException | NumberFormatException e) {
            System.out.println("Error retrieving stock: " + e.getMessage());
        }
        return -1; // Return -1 if item is not found
    }

    public void updateInventoryStock(String item, int quantity) {
        Map<String, Integer> inventory = new LinkedHashMap<>();
        boolean itemFound = false;

        // Read the current inventory into a map
        try (BufferedReader br = new BufferedReader(new FileReader(INVENTORY_FILE))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 2) {
                    String itemName = parts[0].trim();
                    int stock = Integer.parseInt(parts[1].trim());
                    if (itemName.equalsIgnoreCase(item)) {
                        if (stock >= quantity) {
                            stock -= quantity; // Deduct the quantity
                            itemFound = true;
                        } else {
                            System.out.println("Insufficient stock for " + item + ". Available: " + stock);
                            return;
                        }
                    }
                    inventory.put(itemName, stock);
                }
            }
        } catch (IOException | NumberFormatException e) {
            System.out.println("Error reading inventory for update: " + e.getMessage());
            return;
        }


        if (!itemFound) {
            System.out.println("Item " + item + " not found in inventory.");
            return;
        }


        try (BufferedWriter bw = new BufferedWriter(new FileWriter(INVENTORY_FILE))) {
            for (Map.Entry<String, Integer> entry : inventory.entrySet()) {
                bw.write(entry.getKey() + "," + entry.getValue());
                bw.newLine();
            }
            System.out.println("Inventory updated successfully for item: " + item);
        } catch (IOException e) {
            System.out.println("Error updating inventory file: " + e.getMessage());
        }
    }
}
