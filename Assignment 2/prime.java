//To run this program, use the java command, then the name of the class file, along with the max number you want
//Eg. "java prime 35"

public class prime {
    public static void main(String args[]){

        //convert args into num and pass into thread
        int numIn = Integer.valueOf(args[0]);
        PrimeThread primeThread = new PrimeThread(numIn);

        //start thread
        primeThread.start();
    }
}

class PrimeThread extends Thread{
    int numOfTerms;

    public PrimeThread(int numOfTerms){
        //set number of terms to argument
        this.numOfTerms = numOfTerms;
    }

    public void run(){
        //go through all number less than or equal to user input
        for(int i = 2; i <= numOfTerms; i++){

            //assume prime, and test for divisibility
            boolean isPrime = true;

            for(int j = 2; j <= i/2; j++){
                if(i % j == 0){
                    isPrime = false;
                    break;
                }
            }

            //print if prime
            if (isPrime) {
                System.out.println(i);
            }
        }
    }
}
