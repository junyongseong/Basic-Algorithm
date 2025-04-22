import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;

public class AdminVendingMachineClient extends JFrame {
    private JTextArea outputArea;
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    public AdminVendingMachineClient() {
        setTitle("Vending Machine Admin Interface");
        setSize(600, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  // 기본 종료 동작 설정

        outputArea = new JTextArea(15, 40);
        outputArea.setEditable(false);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(3, 2));

        String[] items = {"water", "coffee", "ion_drinks", "high_end_coffee", "soda", "specialized_drinks"};
        for (String item : items) {
            JButton checkButton = new JButton("Check " + item);
            checkButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    sendCommand("CHECK " + item);
                }
            });

            JButton refillButton = new JButton("Refill " + item);
            refillButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    String quantity = JOptionPane.showInputDialog("Enter quantity to refill " + item + ":");
                    try {
                        int qty = Integer.parseInt(quantity);
                        sendCommand("REFILL " + item + " " + qty);
                    } catch (NumberFormatException ex) {
                        outputArea.append("Invalid quantity\n");
                    }
                }
            });

            panel.add(checkButton);
            panel.add(refillButton);
        }

        Container contentPane = getContentPane();
        contentPane.add(panel, BorderLayout.NORTH);
        contentPane.add(new JScrollPane(outputArea), BorderLayout.CENTER);

        try {
            socket = new Socket("localhost", 9999);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            outputArea.append("Error connecting to server\n");
        }

        new Thread(new Runnable() {
            public void run() {
                try {
                    String response;
                    while ((response = in.readLine()) != null) {
                        outputArea.append(response + "\n");
                    }
                } catch (IOException e) {
                    outputArea.append("Connection closed\n");
                }
            }
        }).start();
    }

    private void sendCommand(String command) {
        if (out != null) {
            out.println(command);
        } else {
            outputArea.append("Not connected to server\n");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new AdminVendingMachineClient().setVisible(true);
            }
        });
    }
}