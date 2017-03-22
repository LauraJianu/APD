package apd1;
import java.util.Random;
import java.lang.*;


public class MM {
    
    //Creating the matrix
    static int[][] mat = new int[3][3];
    static int[][] mat2 = new int[3][3];
    static int[][] result = new int[3][3];

    public static void main(String [] args){
        
        //Creating the object of random class
        Random rand = new Random();
        
        
        //Filling first matrix with random values
        for (int i = 0; i < mat.length; i++) {
            for (int j = 0; j < mat[i].length; j++) {
                mat[i][j]=rand.nextInt(10);
            }
        }
        
        //Filling second matrix with random values
        for (int i = 0; i < mat2.length; i++) {
            for (int j = 0; j < mat2[i].length; j++) {
                mat2[i][j]=rand.nextInt(10);
            }
        }

        //Printing the first matrix
        System.out.println("This is first matrix:");
        for (int i = 0; i < mat.length; i++) {
            for (int j = 0; j < mat[i].length; j++) {
                System.out.print(mat[i][j]+" ");
            }
            System.out.println();
        }

        //Printing the second matrix
        System.out.println("\nThis is second matrix:");
        for (int i = 0; i < mat.length; i++) {
            for (int j = 0; j < mat[i].length; j++) {
                System.out.print(mat2[i][j]+" ");
            }
            System.out.println();
        }

        try{
            //Object of multiply Class
            Multiply multiply = new Multiply(3,3);
            
            //Threads
            MatrixMultiplier thread1 = new MatrixMultiplier(multiply);
            MatrixMultiplier thread2 = new MatrixMultiplier(multiply);
            MatrixMultiplier thread3 = new MatrixMultiplier(multiply);
            
            //Implementing threads
            Thread th1 = new Thread(thread1);
            Thread th2 = new Thread(thread2);
            Thread th3 = new Thread(thread3);
            
            //Starting threads
            th1.start();
            th2.start();
            th3.start();
            
            th1.join();
            th2.join();
            th3.join();
            
        }catch (Exception e) {
            e.printStackTrace();
        }

        //Printing the result
        System.out.println("\n\nResult:");
        for (int i = 0; i < result.length; i++) {
            for (int j = 0; j < result[i].length; j++) {
                System.out.print(result[i][j]+" ");
            }
            System.out.println();
        }
    }//End main

}//End Class

     