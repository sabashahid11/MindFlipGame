import java.util.*;
public class Customer {
    private int customerId;
    private String customerName;
    private String email;
    private String phoneNumber;
    private List<Order> orderHistory;

    public Customer(String customerName, String email) {
        this.customerName = customerName;
        this.email = email;
        this.orderHistory = new ArrayList<>();
    }

    public void addOrder(Order order) {
        orderHistory.add(order);
    }

    public int getCustomerId() { return customerId; }
    public void setCustomerId(int customerId) { this.customerId = customerId; }
    public String getCustomerName() { return customerName; }
    public String getEmail() { return email; }
    public void setCustomerName(String customerName) { this.customerName = customerName; }
    public List<Order> getOrderHistory() { return orderHistory; }
}

