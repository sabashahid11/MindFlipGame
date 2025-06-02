import java.util.*;

public class Order {
    private int orderId;
    private Customer customer;
    private List<OrderItem> orderItems;
    private double totalPrice;
    private Date orderDate;
    private OrderStatus status;

    public enum OrderStatus {
        PLACED, PROCESSING, COMPLETED, CANCELLED
    }
    public void setStatus(OrderStatus status) {
        this.status = status;
    }

    public Order(Customer customer) {
        this.customer = customer;
        this.orderItems = new ArrayList<>();
        this.orderDate = new Date();
        this.status = OrderStatus.PLACED;
    }

    public void placeOrder() {

        if (validateOrder()) {

            for (OrderItem item : orderItems) {
                item.getProduct().updateStock(item.getQuantity());
            }
            this.status = OrderStatus.PROCESSING;
            generateInvoice();
        }
    }

    public void cancelOrder() {
        if (this.status != OrderStatus.COMPLETED) {

            for (OrderItem item : orderItems) {
                item.getProduct().restoreStock(item.getQuantity());
            }
            this.status = OrderStatus.CANCELLED;
        }
    }

    public void calculateTotalPrice() {
        this.totalPrice = orderItems.stream()
                .mapToDouble(item -> item.getProduct().getPrice() * item.getQuantity())
                .sum();
    }

    public void generateInvoice() {
        // Logic to generate and potentially print/save invoice
        System.out.println("Invoice generated for Order #" + orderId);
    }

    public void addProduct(Product product, int quantity) {
        if (product.checkAvailability(quantity)) {
            orderItems.add(new OrderItem(product, quantity));
            calculateTotalPrice();
        }
    }

    private boolean validateOrder() {
        return !orderItems.isEmpty() &&
                orderItems.stream().allMatch(item ->
                        item.getProduct().checkAvailability(item.getQuantity()));
    }

    public int getOrderId() { return orderId; }
    public void setOrderId(int orderId) { this.orderId = orderId; }
    public Customer getCustomer() { return customer; }
    public List<OrderItem> getOrderItems() { return orderItems; }
    public double getTotalPrice() { return totalPrice; }
    public OrderStatus getStatus() { return status; }
}


