import java.io.File;
import java.util.Random;
import java.util.Scanner;

public class JavaCommands {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.print("basic240-shell> ");
            String input = scanner.nextLine();

            if (input.equals("exit")) {
                break;
            } else if (input.equals("ls")) {
                listFiles();
            } else {
                String[] command = input.split(" ");
                if (command[0].equals("generate")) {
                    if (command.length != 1) {
                        System.out.println("Invalid arguments for generate command.");
                        continue;
                    }
                    int lowerBound = 0;
                    int upperBound = 10;
                    int randomNumber1 = generateRandom(lowerBound, upperBound);
                    int randomNumber2 = generateRandom(lowerBound, upperBound);
                    System.out.println("Generated random numbers: " + randomNumber1 + " " + randomNumber2);
                } else if (command[0].equals("sum")) {
                    if (command.length != 3) {
                        System.out.println("Invalid arguments for sum command.");
                        continue;
                    }
                    int num1 = Integer.parseInt(command[1]);
                    int num2 = Integer.parseInt(command[2]);
                    int sum = sumNumbers(num1, num2);
                    System.out.println(num1 + "+" + num2 + "=" + sum);
                } else if (command[0].equals("subtract")) {
                    if (command.length != 3) {
                        System.out.println("Invalid arguments for subtract command.");
                        continue;
                    }
                    int num1 = Integer.parseInt(command[1]);
                    int num2 = Integer.parseInt(command[2]);
                    int difference = subtractNumbers(num1, num2);
                    System.out.println(num1 + "-" + num2 + "=" + difference);
                } else {
                    System.out.println("Invalid command: " + command[0]);
                }
            }
        }
    }

    private static int generateRandom(int lowerBound, int upperBound) {
        Random random = new Random();
        return random.nextInt(upperBound - lowerBound + 1) + lowerBound;
    }

    private static int sumNumbers(int num1, int num2) {
        return num1 + num2;
    }

    private static int subtractNumbers(int num1, int num2) {
        return num1 - num2;
    }

    private static void listFiles() {
        File currentDir = new File(".");
        File[] files = currentDir.listFiles();
        for (File file : files) {
            System.out.println(file.getName());
        }
    }
}
