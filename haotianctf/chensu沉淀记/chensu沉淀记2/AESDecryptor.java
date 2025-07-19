import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;
import java.util.Scanner;

public class AESDecryptor {
    public static void main(String[] args) {
        try {
            Scanner scanner = new Scanner(System.in);
            
            // Get user input
            System.out.println("Enter the encrypted flag (Base64 format):");
            String encryptedFlag = scanner.nextLine();
            
            System.out.println("Enter the key (16 bytes):");
            String key = scanner.nextLine();
            
            // Create AES key
            SecretKeySpec secretKey = new SecretKeySpec(key.getBytes(), "AES");
            
            // Decrypt process
            Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, secretKey);
            
            // Base64 decode and decrypt
            byte[] decryptedBytes = cipher.doFinal(Base64.getDecoder().decode(encryptedFlag));
            String decryptedFlag = new String(decryptedBytes);
            
            System.out.println("\nDecrypted flag: " + decryptedFlag);
            
            // 验证flag格式
            if (decryptedFlag.startsWith("flag{") && decryptedFlag.endsWith("}")) {
                System.out.println("Valid flag format!");
            } else {
                System.out.println("Warning: This doesn't look like a valid flag format.");
            }
            
        } catch (IllegalArgumentException e) {
            System.out.println("Error: Invalid Base64 input!");
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
            System.out.println("Possible causes:");
            System.out.println("1. Incorrect key");
            System.out.println("2. Invalid encrypted text");
            System.out.println("3. Wrong encryption parameters");
        }
    }
}
