import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;

public class UserVendingMachineClient extends JFrame {
    private JTextArea outputArea;
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    public UserVendingMachineClient() {
        setTitle("Vending Machine User Interface");
        setSize(600, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  // 기본 종료 동작 설정

        outputArea = new JTextArea(15, 40);
        outputArea.setEditable(false);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(3, 2));

        String[] items = {"water", "coffee", "ion_drinks", "high_end_coffee", "soda", "specialized_drinks"};
        for (String item : items) {
            JPanel itemPanel = new JPanel();
            itemPanel.setLayout(new BorderLayout());

            JLabel imageLabel = new JLabel();
            imageLabel.setIcon(new ImageIcon("images/" + item + ".jpg"));

            JButton button = new JButton(item + " (" + getPrice(item) + " won)");
            button.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    sendCommand("DISPENSE " + item);
                }
            });

            itemPanel.add(imageLabel, BorderLayout.CENTER);
            itemPanel.add(button, BorderLayout.SOUTH);
            panel.add(itemPanel);
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

    private int getPrice(String item) {
        switch (item) {
            case "water":
                return 450;
            case "coffee":
                return 500;
            case "ion_drinks":
                return 550;
            case "high_end_coffee":
                return 700;
            case "soda":
                return 750;
            case "specialized_drinks":
                return 800;
            default:
                return 0;
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new UserVendingMachineClient().setVisible(true);
            }
        });
    }
}