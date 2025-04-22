import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class VendingMachineAdminGUI extends JFrame {
    private JTextArea outputArea;  // 출력을 담당할 JTextArea
    private JPasswordField passwordField;  // 관리자 패스워드 입력을 받음
    private JPasswordField newPasswordField;  // 새로운 패스워드 입력을 받음
    private JPasswordField confirmPasswordField;  // 새로운 패스워드 확인받음
    private Socket socket;  // 서버와의 소켓연결
    private PrintWriter out;  // 서버로 출력할 객체
    private BufferedReader in;  // 서버로부터 입력을 받는 객체
    private String currentPassword = "yj@20194057";  // 초기 관리자 패스워드
    private boolean isAdminLoggedIn = false;

    private Map<String, Map<String, Integer>> vendingMachines = new HashMap<>();
    {
        //기본 재고
        Map<String, Integer> stock1 = new HashMap<>();
        stock1.put("Water", 10);
        stock1.put("Coffee", 10);
        stock1.put("Tea", 10);
        stock1.put("Cola", 10);
        stock1.put("Juice", 10);
        stock1.put("Soda", 10);

        //기본 재고
        Map<String, Integer> stock2 = new HashMap<>();
        stock2.put("Water", 10);
        stock2.put("Coffee", 10);
        stock2.put("Tea", 10);
        stock2.put("Cola", 10);
        stock2.put("Juice", 10);
        stock2.put("Soda", 10);

        vendingMachines.put("Machine 1", stock1);
        vendingMachines.put("Machine 2", stock2);
    }

    public VendingMachineAdminGUI() {
        setTitle("Vending Machine Admin Interface"); // 제목
        setSize(600, 600);// 프레임 크기설정
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //프로그램 종료

        outputArea = new JTextArea(10, 30);//출력 영역 생성과 크기
        outputArea.setEditable(false);//편집 불가능

        JPanel loginPanel = new JPanel();//로그인 패널생성
        loginPanel.setLayout(new GridLayout(4, 2, 10, 10));

        loginPanel.add(new JLabel("비밀번호 입력:"));//비밀번호 입력
        passwordField = new JPasswordField(10);
        loginPanel.add(passwordField);

        JButton loginButton = new JButton("로그인"); //로그인
        loginButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String enteredPassword = new String(passwordField.getPassword());
                if (enteredPassword.equals(currentPassword)) {
                    isAdminLoggedIn = true;
                    outputArea.append("로그인 성공\n");
                    openAdminMenu(); //관리자 메뉴 열기
                } else {
                    outputArea.append("로그인 실패\n");
                }
            }
        });
        loginPanel.add(loginButton);

        loginPanel.add(new JLabel("새로운 비밀번호 입력:")); //새로운 비밀번호 입력 받기
        newPasswordField = new JPasswordField(10);
        loginPanel.add(newPasswordField);

        loginPanel.add(new JLabel("비밀번호 확인:")); //비밀번호 확인하기
        confirmPasswordField = new JPasswordField(10);
        loginPanel.add(confirmPasswordField);

        JButton changePasswordButton = new JButton("비밀번호 변경");//비밀번호 변경
        changePasswordButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String currentPassword = new String(passwordField.getPassword());
                String newPassword = new String(newPasswordField.getPassword());
                String confirmPassword = new String(confirmPasswordField.getPassword());

                if (!currentPassword.equals(VendingMachineAdminGUI.this.currentPassword)) {
                    outputArea.append("비밀번호가 틀렸습니다.\n");
                } else if (!isValidPassword(newPassword)) {
                    outputArea.append("비밀번호는 반드시 !,@,#,$,%,^,&,* 특수문자를 포함하고 8자 이상이어야합니다.\n");
                } else if (!newPassword.equals(confirmPassword)) {
                    outputArea.append("비밀번호가 일치하지 않습니다.\n");
                } else {
                    VendingMachineAdminGUI.this.currentPassword = newPassword;
                    outputArea.append("비밀번호 변경 성공\n");
                    clearPasswordField(newPasswordField);
                    clearPasswordField(confirmPasswordField);
                }
            }
        });
        loginPanel.add(changePasswordButton);

        Container contentPane = getContentPane();
        contentPane.add(new JScrollPane(outputArea), BorderLayout.CENTER);
        contentPane.add(loginPanel, BorderLayout.NORTH);

        try {
            socket = new Socket("localhost", 9999);//서버에 소켓 연결
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

    private boolean isValidPassword(String password) {
        return password.matches("^(?=.*[!@#$%^&*]).{8,}$");
    }

    private void clearPasswordField(JPasswordField passwordField) {
        passwordField.setText("");
    }

    private void openAdminMenu() {
        JFrame adminFrame = new JFrame("Admin Menu");
        adminFrame.setSize(400, 300);
        adminFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JPanel adminPanel = new JPanel();
        adminPanel.setLayout(new GridLayout(4, 1, 10, 10));

        JButton stockStatusButton = new JButton("재고 상태");
        stockStatusButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (isAdminLoggedIn) {
                    displayStockStatus();
                } else {
                    outputArea.append("Please log in first\n");
                }
            }
        });
        adminPanel.add(stockStatusButton);

        JButton salesStatusButton = new JButton("판매 현황");
        salesStatusButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (isAdminLoggedIn) {
                    sendCommand("SALES_STATUS");
                } else {
                    outputArea.append("Please log in first\n");
                }
            }
        });
        adminPanel.add(salesStatusButton);

        JButton collectButton = new JButton("Collect Money");
        collectButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (isAdminLoggedIn) {
                    sendCommand("COLLECT_MONEY");
                } else {
                    outputArea.append("Please log in first\n");
                }
            }
        });
        adminPanel.add(collectButton);

        JButton returnButton = new JButton("Return to Login");
        returnButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                isAdminLoggedIn = false;
                adminFrame.dispose();
                outputArea.setText("");
                clearPasswordField(passwordField);
                currentPassword = "yj@20194057"; //기본 비밀번호
            }
        });
        adminPanel.add(returnButton); //다시 로그인 화면

        adminFrame.add(adminPanel);
        adminFrame.setVisible(true);
    }

    private void displayStockStatus() {
        JFrame stockStatusFrame = new JFrame("재고 상태");
        stockStatusFrame.setSize(400, 300);
        stockStatusFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JPanel stockStatusPanel = new JPanel(new BorderLayout());
        stockStatusPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JTextArea stockStatusTextArea = new JTextArea(10, 30);
        stockStatusTextArea.setEditable(false);
        stockStatusPanel.add(new JScrollPane(stockStatusTextArea), BorderLayout.CENTER);

        stockStatusTextArea.append("현재 재고상태:\n\n"); //현재 재고상태

        for (Map.Entry<String, Map<String, Integer>> entry : vendingMachines.entrySet()) {
            String machineName = entry.getKey();
            Map<String, Integer> stock = entry.getValue();

            stockStatusTextArea.append("Machine: " + machineName + "\n");

            for (Map.Entry<String, Integer> item : stock.entrySet()) {
                stockStatusTextArea.append(item.getKey() + ": " + item.getValue() + "\n");
            }

            stockStatusTextArea.append("\n");
        }

        stockStatusFrame.add(stockStatusPanel); //재고상태 패널 추가
        stockStatusFrame.setVisible(true); 
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
                new VendingMachineAdminGUI().setVisible(true); //GUI 실행
            }
        });
    }
}
