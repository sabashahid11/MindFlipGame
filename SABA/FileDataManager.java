import java.io.*;
import java.util.ArrayList;
import java.util.List;


public class FileDataManager {

    private static final String CUSTOMERS_FILE = "customers.txt";
    private static final String PRODUCTS_FILE = "products.txt";
    private static final String ORDERS_FILE = "orders.txt";

    public void saveCustomer(Customer customer) {
        try (PrintWriter writer = new PrintWriter(new FileWriter(CUSTOMERS_FILE, true))) {
            writer.println(
                    customer.getCustomerId() + "," +
                            customer.getCustomerName() + " , " + customer.getEmail()
            );
        } catch (IOException e) {
            System.out.println("Error saving customer: " + e.getMessage());
        }
    }

    public void saveProduct(Product product) {
        try (PrintWriter writer = new PrintWriter(new FileWriter(PRODUCTS_FILE, true))) {
            writer.println(
                    product.getProductId() + "," +
                            product.getProductName() + "," +
                            product.getPrice() + "," +
                            product.getStockQuantity()
            );
        } catch (IOException e) {
            System.out.println("Error saving product: " + e.getMessage());
        }
    }

    public void saveOrder(Order order) {
        try (PrintWriter writer = new PrintWriter(new FileWriter(ORDERS_FILE, true))) {
            // Write order header
            writer.println(     //println writes everything on new line
                    order.getOrderId() + "," +
                            order.getCustomer().getCustomerId() + "," +
                            order.getTotalPrice() + "," +
                            order.getStatus()
            );

            for (OrderItem item : order.getOrderItems()) {
                writer.println(
                        order.getOrderId() + "," +
                                item.getProduct().getProductId() + "," +
                                item.getQuantity()
                );
            }
        } catch (IOException e) {
            System.out.println("Error saving order: " + e.getMessage());
        }
    }

    public List<Customer> readCustomers() {
        List<Customer> customers = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(CUSTOMERS_FILE))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                Customer customer = new Customer(parts[1], "email_placeholder");
                customer.setCustomerId(Integer.parseInt(parts[0]));
                customers.add(customer);
            }
        } catch (IOException e) {
            System.out.println("Error reading customers: " + e.getMessage());
        }
        return customers;
    }

    public List<Product> readProducts() {
        List<Product> products = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(PRODUCTS_FILE))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                Product product = new Product(
                        parts[1],
                        Double.parseDouble(parts[2]),
                        Integer.parseInt(parts[3])
                );
                product.setProductId(Integer.parseInt(parts[0]));
                products.add(product);
            }
        } catch (IOException e) {
            System.out.println("Error reading products: " + e.getMessage());
        }
        return products;
    }

    public static void main(String[] args) {
        FileDataManager dataManager = new FileDataManager();

        Customer customer = new Customer("John Doe", "john@example.com");
        customer.setCustomerId(1);
        dataManager.saveCustomer(customer);

        Product product = new Product("Laptop", 999.99, 50);
        product.setProductId(1);
        dataManager.saveProduct(product);

        Order order = new Order(customer);
        order.setOrderId(1);
        order.addProduct(product, 1);
        dataManager.saveOrder(order);

        List<Customer> customers = dataManager.readCustomers();
        List<Product> products = dataManager.readProducts();
    }

    public int getNextCustomerId() {
        List<Customer> customers = readCustomers();
        return customers.isEmpty() ? 1 :
                customers.stream().mapToInt(Customer::getCustomerId).max().getAsInt() + 1;
    }

    public int getNextOrderId() {
        List<Order> orders = readAllOrders();
        return orders.isEmpty() ? 1 :
                orders.stream().mapToInt(Order::getOrderId).max().getAsInt() + 1;
    }

    public Product findProductById(int productId) {
        List<Product> products = readProducts();
        return products.stream()
                .filter(p -> p.getProductId() == productId)
                .findFirst()
                .orElse(null);
    }

    public void updateProduct(Product product) {
        List<Product> products = readProducts();

        products.removeIf(p -> p.getProductId() == product.getProductId());
        products.add(product);

        try (PrintWriter writer = new PrintWriter(new FileWriter(PRODUCTS_FILE))) {
            for (Product p : products) {
                writer.println(
                        p.getProductId() + "," +
                                p.getProductName() + "," +
                                p.getPrice() + "," +
                                p.getStockQuantity()
                );
            }
        } catch (IOException e) {
            System.out.println("Error updating product: " + e.getMessage());
        }
    }

    public List<Order> readAllOrders() {
        List<Order> orders = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(ORDERS_FILE))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");

                int orderId = Integer.parseInt(parts[0]);
                int customerId = Integer.parseInt(parts[1]);
                double totalPrice = Double.parseDouble(parts[2]);
                Order.OrderStatus status = Order.OrderStatus.valueOf(parts[3]);

                Customer customer = readCustomers().stream()
                        .filter(c -> c.getCustomerId() == customerId)
                        .findFirst()
                        .orElse(null);

                if (customer != null) {
                    Order order = new Order(customer);
                    order.setOrderId(orderId);
                    order.calculateTotalPrice(); // This will set the total price
                    orders.add(order);
                }
            }
        } catch (IOException e) {
            System.out.println("Error reading orders: " + e.getMessage());
        }
        return orders;
    }

    public List<Order> findOrdersByCustomerName(String customerName) {
        List<Order> allOrders = readAllOrders();
        return allOrders.stream()
                .filter(order -> order.getCustomer().getCustomerName().equalsIgnoreCase(customerName))
                .toList();
    }

    public Order findOrderById(int orderId) {
        List<Order> allOrders = readAllOrders();
        return allOrders.stream()
                .filter(order -> order.getOrderId() == orderId)
                .findFirst()
                .orElse(null);
    }
}
