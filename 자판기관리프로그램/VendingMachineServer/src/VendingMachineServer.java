import java.io.*;
import java.net.*;
import java.util.HashMap;

public class VendingMachineServer {

    private static HashMap<String, Integer> inventory = new HashMap<>();
    private static HashMap<String, Integer> prices = new HashMap<>();
    private static boolean running = true;

    static {
        inventory.put("water", 10);
        inventory.put("coffee", 10);
        inventory.put("ion_drinks", 10);
        inventory.put("high_end_coffee", 10);
        inventory.put("soda", 10);
        inventory.put("specialized_drinks", 10);

        prices.put("water", 450);
        prices.put("coffee", 500);
        prices.put("ion_drinks", 550);
        prices.put("high_end_coffee", 700);
        prices.put("soda", 750);
        prices.put("specialized_drinks", 800);
    }

    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(9999);
            System.out.println("Server started on port 9999");

            while (running) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Accepted connection from " + clientSocket.getInetAddress());
                new Thread(new ClientHandler(clientSocket)).start();
            }
        } catch (IOException e) {
            System.out.println("Server error: " + e.getMessage());
        } finally {
            if (serverSocket != null && !serverSocket.isClosed()) {
                try {
                    serverSocket.close();
                } catch (IOException e) {
                    System.out.println("Error closing server socket: " + e.getMessage());
                }
            }
        }
    }

    private static class ClientHandler implements Runnable {
        private Socket clientSocket;

        public ClientHandler(Socket socket) {
            this.clientSocket = socket;
        }

        public void run() {
            try (
                BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            ) {
                String request;
                while ((request = in.readLine()) != null) {
                    String[] parts = request.split(" ");
                    String command = parts[0];
                    String item = parts[1];
                    int quantity = 0;
                    if (parts.length > 2) {
                        try {
                            quantity = Integer.parseInt(parts[2]);
                        } catch (NumberFormatException e) {
                            out.println("Invalid quantity format.");
                            continue;
                        }
                    }

                    String response;
                    switch (command) {
                        case "DISPENSE":
                            response = dispenseItem(item);
                            break;
                        case "CHECK":
                            response = checkItem(item);
                            break;
                        case "REFILL":
                            response = refillItem(item, quantity);
                            break;
                        default:
                            response = "Invalid command.";
                            break;
                    }
                    out.println(response);
                }
            } catch (IOException e) {
                System.out.println("Error handling client: " + e.getMessage());
            } finally {
                if (clientSocket != null && !clientSocket.isClosed()) {
                    try {
                        clientSocket.close();
                    } catch (IOException e) {
                        System.out.println("Error closing client socket: " + e.getMessage());
                    }
                }
            }
        }

        private String dispenseItem(String item) {
            if (inventory.containsKey(item) && inventory.get(item) > 0) {
                inventory.put(item, inventory.get(item) - 1);
                if (inventory.get(item) == 0) {
                    return item + " is out of stock.";
                }
                return item + " dispensed for " + prices.get(item) + " won. Remaining: " + inventory.get(item);
            } else {
                return item + " out of stock.";
            }
        }

        private String checkItem(String item) {
            return item + ": " + inventory.getOrDefault(item, 0) + " left";
        }

        private String refillItem(String item, int quantity) {
            if (quantity <= 0) {
                return "Invalid refill quantity.";
            }
            inventory.put(item, inventory.get(item) + quantity);
            return item + " refilled to " + inventory.get(item) + ".";
        }
    }
}