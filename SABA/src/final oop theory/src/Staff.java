import java.io.*;
import java.util.Scanner;

public class Staff {

    private static final String STAFF_FILE = "staff.txt";
    private static final String DEFAULT_CREDENTIALS = "admin, password";

    static {
        try {
            File file = new File(STAFF_FILE);
            if (file.createNewFile()) {
                System.out.println("File created: " + STAFF_FILE);
                // Adding default credentials on first file creation
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(STAFF_FILE))) {
                    writer.write(DEFAULT_CREDENTIALS);
                    writer.newLine();
                }
            }
        } catch (IOException e) {
            System.out.println("Error initializing staff file: " + e.getMessage());
        }
    }


    public void handleStaff(Scanner scanner) {
        System.out.print("Enter your name: ");
        String name = scanner.nextLine();
        System.out.print("Enter your password: ");
        String password = scanner.nextLine();

        if (authenticateStaff(name, password)) {
            System.out.println("Authentication successful. Welcome, Staff!");
            manageInventory(scanner);

            System.out.print("Do you want to add new staff credentials? (yes/no): ");
            String addChoice = scanner.nextLine();
            if (addChoice.equalsIgnoreCase("yes")) {
                System.out.print("Enter new staff name: ");
                String newName = scanner.nextLine();
                System.out.print("Enter new staff password: ");
                String newPassword = scanner.nextLine();

                try (BufferedWriter writer = new BufferedWriter(new FileWriter(STAFF_FILE, true))) {
                    writer.write(newName + "," + newPassword);
                    writer.newLine();
                    System.out.println("New staff credentials added");
                } catch (IOException e) {
                    System.out.println("Error writing to the staff file: " + e.getMessage());
                }
            }

        } else {
            System.out.println("Invalid credentials. Access denied.");
        }
    }

    private boolean authenticateStaff(String name, String password) {
        try (BufferedReader br = new BufferedReader(new FileReader(STAFF_FILE))) {
            String line;
            while ((line = br.readLine()) != null) {

                String[] credentials = line.split(",");
                if (credentials.length == 2) {
                    String storedName = credentials[0].trim();   // Name in the file
                    String storedPassword = credentials[1].trim(); // Password in the file

                    // Check if input matches stored credentials
                    if (storedName.equals(name) && storedPassword.equals(password)) {
                        return true;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Error reading staff credentials from " + STAFF_FILE + ": " + e.getMessage());
        }
        return false; // Return false if no match is found
    }

    private void manageInventory(Scanner scanner) {
        Inventory inventoryHandler = new Inventory(); // Assuming Inventory class exists
        while (true) {
            System.out.println("Choose an option:");
            System.out.println("1. Manage Inventory");
            System.out.println("2. Update Customer Details");
            System.out.println("3. Exit");
            String choice = scanner.nextLine();

            if (choice.equals("1")) {
                inventoryHandler.displayInventory(); // Display inventory
            } else if (choice.equals("2")) {
                System.out.println("Updating customer details is not implemented yet.");
            } else if (choice.equals("3")) {
                break; // Exit the loop
            } else {
                System.out.println("Invalid choice. Please try again.");
            }
        }
    }
}
