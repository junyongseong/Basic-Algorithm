import java.io.*;
import java.net.*;
import java.util.*;

public class VendingMachineServer {
    private static Map<String, Integer> stock = new HashMap<>(); // 음료 재고를 관리
    private static Map<String, Integer> prices = new HashMap<>(); // 음료 가격을 관리
    private static String adminPassword = "yj@20194057"; // 관리자 비밀번호

    static {
        // 초기 재고 설정
        stock.put("water", 10);
        stock.put("coffee", 10);
        stock.put("ion_drinks", 10);
        stock.put("high_end_coffee", 10);
        stock.put("soda", 10);
        stock.put("specialized_drinks", 10);

        // 초기 가격 설정
        prices.put("water", 450);
        prices.put("coffee", 500);
        prices.put("ion_drinks", 550);
        prices.put("high_end_coffee", 700);
        prices.put("soda", 750);
        prices.put("specialized_drinks", 800);
    }

    public static void main(String[] args) {
        System.out.println("Vending Machine Server is running...");
        try (ServerSocket serverSocket = new ServerSocket(9999)) {
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("New client connected: " + clientSocket);

                // 클라이언트와의 통신을 담당할 새 스레드 생성
                Thread thread = new Thread(new ClientHandler(clientSocket));
                thread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // 클라이언트 요청을 처리하는 내부 클래스
    private static class ClientHandler implements Runnable {
        private Socket clientSocket;
        private BufferedReader in;
        private PrintWriter out;
        private int insertedMoney = 0; // 클라이언트별 투입된 돈 관리

        public ClientHandler(Socket socket) {
            this.clientSocket = socket;
            try {
                in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                out = new PrintWriter(clientSocket.getOutputStream(), true);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            try {
                String command;
                while ((command = in.readLine()) != null) {
                    System.out.println("Received command from client: " + command);
                    String[] parts = command.split(" ");
                    String operation = parts[0];
                    
                    switch (operation) {
                        case "DISPENSE":
                            String item = parts[1];
                            dispenseItem(item);
                            break;
                        case "INSERT_MONEY":
                            int moneyInserted = Integer.parseInt(parts[1]);
                            insertMoney(moneyInserted);
                            break;
                        case "RETURN_MONEY":
                            returnMoney();
                            break;
                        default:
                            out.println("Invalid command: " + command); // 잘못된 명령 처리임
                            break;
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    clientSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        // 음료 판매 메서드
        private void dispenseItem(String item) {
            int stockCount = stock.getOrDefault(item, 0);
            if (stockCount > 0) {
                int price = prices.getOrDefault(item, 0);
                if (insertedMoney >= price) {
                    stock.put(item, stockCount - 1); // 재고 감소
                    insertedMoney -= price; // 투입된 돈에서 금액 차감
                    out.println("DISPENSED " + item + " " + insertedMoney); // 클라이언트에게 판매 완료 메시지 전송
                    System.out.println(item + " 음료가 나왔습니다.");
                } else {
                    out.println("잔액이 부족합니다."); // 잔액 부족 메시지 전송
                }
            } else {
                out.println("품절"); // 품절 메시지 전송
            }
        }

        // 돈 투입 메서드
        private void insertMoney(int moneyInserted) {
            insertedMoney += moneyInserted; // 클라이언트별 투입된 돈 추가
            out.println("현재 총 투입 금액: " + insertedMoney + " 원"); // 현재 총 투입 금액 전송
            System.out.println(moneyInserted + " 원을 넣었습니다.");
        }

        // 돈 반환 메서드
        private void returnMoney() {
            out.println("Money returned: " + insertedMoney); // 반환할 금액 전송
            System.out.println(insertedMoney + " 원을 반환합니다.");
            insertedMoney = 0; // 클라이언트별 투입된 돈 초기화
        }
    }
}
