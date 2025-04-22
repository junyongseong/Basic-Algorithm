import javax.swing.*; // Swing GUI
import java.awt.*; // Java의 GUI 라이브러리
import java.awt.event.ActionEvent; // 이벤트 구현
import java.awt.event.ActionListener; // 입력 이벤트
import java.awt.image.BufferedImage; // 이미지
import java.io.*; // 파일 읽기 쓰기
import java.net.Socket; // 소켓 통신
import javax.imageio.ImageIO; // 이미지 읽고 쓰는데

public class UserVendingMachineClient extends JFrame {
    private JTextArea outputArea;
    private JTextArea moneyStatusArea;
    private Socket socket; // 소켓 연결
    private PrintWriter out;
    private BufferedReader in;
    private int insertedMoney = 0; // 투입된 돈
    private JLabel moneyLabel; // 돈 표시
    private JLabel[] itemIndicators;
    private String[] items = {"water", "coffee", "ion_drinks", "high_end_coffee", "soda", "specialized_drinks"}; // 판매하는 음료

    private final int MAX_AMOUNT_SINGLE_COIN = 5000; // 개별 동전 최대 금액
    private final int MAX_AMOUNT_COMBINED_COINS = 7000; // 결합된 동전 최대 금액

    public UserVendingMachineClient() {
        setTitle("Vending Machine User Interface");
        setSize(1000, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        outputArea = new JTextArea(10, 30);
        outputArea.setEditable(false);

        moneyStatusArea = new JTextArea(10, 30);
        moneyStatusArea.setEditable(false);

        JPanel vendingPanel = new JPanel(); // 패널 생성
        vendingPanel.setLayout(new GridLayout(3, 2, 10, 10));
        vendingPanel.setPreferredSize(new Dimension(600, 600)); // 크기 설정

        itemIndicators = new JLabel[items.length];

        // 아이템 추가
        for (int i = 0; i < items.length; i++) {
            String item = items[i];
            JPanel itemPanel = new JPanel();
            itemPanel.setLayout(new BorderLayout(10, 10));

            JLabel imageLabel = new JLabel(); // 이미지 레이블
            try {
                BufferedImage img = ImageIO.read(new File("images/" + item + ".jpg"));
                Image scaledImg = img.getScaledInstance(150, 150, Image.SCALE_SMOOTH); // 이미지 크기
                imageLabel.setIcon(new ImageIcon(scaledImg));
            } catch (IOException e) {
                imageLabel.setText("이미지를 찾을 수 없습니다."); // 이미지 없는 경우
            }
            imageLabel.setHorizontalAlignment(SwingConstants.CENTER);

            JButton button = new JButton(item + " (" + getPrice(item) + " 원)");
            button.setHorizontalAlignment(SwingConstants.CENTER);
            button.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    int price = getPrice(item);
                    if (insertedMoney >= price) {
                        sendCommand("DISPENSE " + item);
                    } else {
                        moneyStatusArea.setText("돈이 부족합니다. " + (price - insertedMoney) + " 원이 더 필요합니다.\n");
                    }
                }
            });

            JLabel indicator = new JLabel("\u25CF");
            indicator.setForeground(Color.RED);
            itemIndicators[i] = indicator;

            JPanel bottomPanel = new JPanel();
            bottomPanel.setLayout(new BorderLayout());
            bottomPanel.add(button, BorderLayout.CENTER);
            bottomPanel.add(indicator, BorderLayout.EAST); 

            itemPanel.add(imageLabel, BorderLayout.CENTER); // 이미지 추가
            itemPanel.add(bottomPanel, BorderLayout.SOUTH); // 바닥 패널
            vendingPanel.add(itemPanel); // 아이템 패널 추가
        }

        JPanel moneyPanel = new JPanel(); // 돈 패널
        moneyPanel.setLayout(new GridLayout(4, 2, 10, 10));

        String[] denominations = {"10원", "50원", "100원", "500원", "1000원"}; // 동전
        int[] values = {10, 50, 100, 500, 1000};
        for (int i = 0; i < denominations.length; i++) {
            JButton moneyButton = new JButton(denominations[i]);
            int value = values[i];
            moneyButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if ((value == 10 || value == 50 || value == 100 || value == 500) && insertedMoney + value > MAX_AMOUNT_SINGLE_COIN) {
                        moneyStatusArea.setText("동전의 사용은 5000원을 넘을 수 없습니다..\n");
                    } else if (value == 1000 && insertedMoney + value > MAX_AMOUNT_COMBINED_COINS) {
                        moneyStatusArea.setText("돈의 한계는 7000원을 넘어 설 수 없습니다..\n");
                    } else {
                        insertedMoney += value;
                        moneyLabel.setText("입금된 돈: " + insertedMoney + " 원");
                        moneyStatusArea.setText("");
                        updateItemIndicators();
                        sendCommand("INSERT_MONEY " + value); // 서버로 돈을 삽입하는 명령 전송
                    }
                }
            });
            moneyPanel.add(moneyButton);
        }

        moneyLabel = new JLabel("입금된 돈: 0 원", SwingConstants.CENTER);
        moneyPanel.add(moneyLabel);

        JButton returnButton = new JButton("돈 반환");
        returnButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                int response = JOptionPane.showConfirmDialog(null, "돈을 반환하시겠습니까?", "돈 반환", JOptionPane.YES_NO_OPTION);
                if (response == JOptionPane.YES_OPTION) {
                    sendCommand("RETURN_MONEY");
                } else {
                    moneyStatusArea.setText("");
                    dispose();
                }
            }
        });
        moneyPanel.add(returnButton);

        Container contentPane = getContentPane();
        contentPane.add(moneyPanel, BorderLayout.WEST);
        contentPane.add(vendingPanel, BorderLayout.CENTER);

        JPanel statusPanel = new JPanel();
        statusPanel.setLayout(new BorderLayout());
        statusPanel.add(new JScrollPane(outputArea), BorderLayout.CENTER);
        statusPanel.add(new JScrollPane(moneyStatusArea), BorderLayout.SOUTH);
        contentPane.add(statusPanel, BorderLayout.EAST);

        try {
            socket = new Socket("localhost", 9999);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            outputArea.append("서버에 연결하는 중 오류 발생\n");
        }

        new Thread(new Runnable() {
            public void run() {
                try {
                    String response;
                    while ((response = in.readLine()) != null) {
                        if (response.startsWith("DISPENSED")) {
                            String[] parts = response.split(" ");
                            String item = parts[1];
                            int remainingMoney = Integer.parseInt(parts[2]);
                            insertedMoney = remainingMoney;
                            moneyLabel.setText("입금된 돈: " + insertedMoney + " 원");
                            updateItemIndicators();
                            outputArea.append(item + " 음료가 나왔습니다.\n");
                        } else if (response.startsWith("Money returned")) {
                            String[] parts = response.split(": ");
                            int returnedMoney = Integer.parseInt(parts[1].trim());
                            JOptionPane.showMessageDialog(null, "Returned money: " + returnedMoney + " 원");
                            insertedMoney = 0;
                            moneyLabel.setText("입금된 돈: 0 원");
                            updateItemIndicators();
                        } else if (response.equals("품절")) {
                            outputArea.append("선택하신 음료가 품절되었습니다.\n");
                        } else if (response.equals("잔액이 부족합니다.")) {
                            outputArea.append("잔액이 부족합니다.\n");
                        } else {
                            outputArea.append(response + "\n");
                        }
                    }
                } catch (IOException e) {
                    outputArea.append("연결 종료\n");
                }
            }
        }).start();
    }

    // 아이템 가격 메서드
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
        }
        return 0;
    }

    // 최신 돈 상태를 보여줌
    private void updateItemIndicators() {
        for (int i = 0; i < items.length; i++) {
            if (insertedMoney >= getPrice(items[i])) {
                itemIndicators[i].setForeground(Color.GREEN);
            } else {
                itemIndicators[i].setForeground(Color.RED);
            }
        }
    }

    // 서버로 명령을 보냄
    private void sendCommand(String command) {
        if (out != null) {
            out.println(command);
        } else {
            outputArea.append("서버에 연결되지 않았습니다.\n");
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
