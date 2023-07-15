import java.util.Random;
import java.util.Scanner;

public class q2 {
    public static void main(String args[])  {
        
        Scanner scanner = new Scanner(System.in);

        System.out.println("How many reference strings do you want to generate?");
        int refSize = scanner.nextInt();
       
        scanner.close();

        Random rand1 = new Random(); 
        int pageFrameSize = rand1.nextInt(7) + 1; //1 to 7 inclusive. 

        int pageReference[] = new int[refSize];
        int pageFrames[] = new int[pageFrameSize];
        
        for (int i = 0; i < refSize; i++) {
            Random rand2 = new Random();

            pageReference[i] = rand2.nextInt(10); //0 to 9 inclusive. 
        }

        for (int a = 0; a < pageFrameSize; a++) {
            pageFrames[a] = -1;
        }

        System.out.println("Page reference string: " + arrayToString(pageReference));

        int fifoPageFaults = fifo(pageReference, pageFrames, refSize, pageFrameSize);

        for (int a = 0; a < pageFrameSize; a++) {
            pageFrames[a] = -1;
        }

        int lruPageFaults = lru(pageReference, pageFrames, refSize, pageFrameSize);

        System.out.println("The FIFO algorithm had " + fifoPageFaults + " page faults.");
        System.out.println("The LRU algorithm had " + lruPageFaults + " page faults.");
    }

    public static int fifo(int[] refString, int[] frames, int refSize, int frameSize) {
        int pageFaults = 0;
        int pointer = 0;

        for (int i = 0; i < refSize; i++) {
            boolean isFault = true; 

            for (int j = 0; j < frameSize; j++){

                if (frames[j] == refString[i]) {
                    isFault = false; 
                    break; 
                }
            }


            if (isFault) {
                frames[pointer] = refString[i];
                pointer = (pointer + 1) % frameSize; 
                pageFaults++;

            }

        }

        System.out.println("FIFO page frame contents: " + arrayToString(frames));

        return pageFaults; 
    }


    public static int lru(int[] refString, int[] frames, int refSize, int frameSize) {
        int pageFaults = 0;
        int age[] = new int[frameSize]; //Keeps track of when a page is referenced in the page frames. Saves the index of the page from the reference string as this guarantees how recent a page was referenced. Higher index meaning more recent. 

        for (int i = 0; i < refSize; i++) {

            boolean isFault = true; 

            for (int j = 0; j < frameSize; j++) {

                if (frames[j] == refString[i]) {
                    isFault = false; 
                    age[j] = i; //Save the index of the reference string page. 
                    break; 
                }    

            }

            if (isFault) {
                int smallestIndex = 0;

                for (int k = 0; k < frameSize; k++) {

                    //Search for the smallest index value. This indicates the oldest as a smaller reference string index means it was referenced earlier. 
                    //In the case that the frames array has not been filled, set smallest index to the first -1 value found so that it is replaced. 
                    if (age[k] < age[smallestIndex] || frames[k] == -1) {
                        smallestIndex = k; 

                        if (frames[k] == -1) {
                            break;
                        }
                    }

                }

                frames[smallestIndex] = refString[i]; //Replace the oldest page with a new page. 
                // Set the index of the page from the reference string as its "age" value. 
                age[smallestIndex] = i; 
                pageFaults++;
            }

        }

        System.out.println("LRU page frame contents: " + arrayToString(frames));

        return pageFaults; 
    }

    public static String arrayToString(int[] arr) {
        String out = "";
        for (int i = 0; i < arr.length ; i++){
            out += arr[i] + " ";
        }

        return out; 
    }


}