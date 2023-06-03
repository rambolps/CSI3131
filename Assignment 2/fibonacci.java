import java.util.Scanner;

//main thread of execution. 
public class fibonacci {
    public static void main(String args[]) {

        int n = 0; //Number of fibonacci terms to calculate.
        long[] result; //Fibonacci terms returned from thread.

        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter in the number of fibonacci numbers to generate:"); //Note that a '1' input denotes fibonacci term '0', so we want to get up to fibonacci term n-1.
        
        try {
            n = scanner.nextInt();

            while (n <= 0) {
                System.out.println("Please input a number greater than 0.");
                System.out.println("Enter in the number of fibonacci numbers to generate:");
                n = scanner.nextInt();
            }

        } catch (Exception e) {
    
            System.out.println("Invalid input. Please input an integer.");
        }

        scanner.close();
    
        FibonacciThread calculateTerms = new FibonacciThread(n);
        
        calculateTerms.start(); //start the thread. 

        try {
            calculateTerms.join(); // Wait for the child thread.
        } catch (Exception e) {
            System.out.println("Failed to join thread.");
        }

        result = calculateTerms.getTerms(); //Get the fibonacci terms. 

        //Print the fibonacci terms. 
        System.out.println("Fibonacci terms: ");
        for (int i = 0; i < result.length; i++){
            System.out.print(result[i] + " ");

        }
        System.out.println();

    } 
    
}

//child thread. 
class FibonacciThread extends Thread {
    int n; 
    long[] fibTerms; 

    public FibonacciThread(int terms) {
        n = terms;

    }

    public long[] getTerms() {
        return fibTerms; 
    }
    
    public void run() {
        fibTerms = new long[n];

        if (n == 1) {
            fibTerms[0] = 0;
        }

        else { 
            fibTerms[0] = 0;
            fibTerms[1] = 1;

            for (int i = 2; i < n; i++) {
                fibTerms[i] = fibTerms[i - 1] + fibTerms[i - 2];
            }
        }
    }


}




