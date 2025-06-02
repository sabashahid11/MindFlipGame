public class Product {
    private int productId;
    private String productName;
    private double price;
    private int stockQuantity;

    public Product(String productName, double price, int stockQuantity) {
        this.productName = productName;
        this.price = price;
        this.stockQuantity = stockQuantity;
    }

    public void updateStock(int quantity) {
        if (checkAvailability(quantity)) {
            this.stockQuantity -= quantity;
        }
    }

    public void restoreStock(int quantity) {
        this.stockQuantity += quantity;
    }

    public boolean checkAvailability(int quantity) {
        return this.stockQuantity >= quantity;
    }

    public int getProductId() { return productId; }
    public void setProductId(int productId) { this.productId = productId; }
    public String getProductName() { return productName; }
    public void setProductName(String productName) { this.productName = productName; }
    public double getPrice() { return price; }
    public void setPrice(double price) { this.price = price; }
    public int getStockQuantity() { return stockQuantity; }

    public void setStockQuantity(int stockQuantity) {
        this.stockQuantity = stockQuantity;
    }
}