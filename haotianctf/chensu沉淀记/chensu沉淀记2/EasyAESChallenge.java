import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;

public class EasyAESChallenge {
    private static final String KEY = "CTF_is_fun_2025!"; // 16-byte key
    private static final String FLAG = "flag{AES_1s_n0t_h4rd_2025}";
    
    public static void main(String[] args) {
        try {
            // Create AES key
            SecretKeySpec secretKey = new SecretKeySpec(KEY.getBytes(), "AES");
            
            // Encrypt FLAG
            Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            byte[] encryptedFlag = cipher.doFinal(FLAG.getBytes());
            String encodedFlag = Base64.getEncoder().encodeToString(encryptedFlag);
            
            // Print hints and encrypted flag
            System.out.println(secretKey);
            System.out.println("Welcome to Easy AES Challenge!");
            System.out.println("Can you decrypt this message?");
            System.out.println("Encrypted flag: " + encodedFlag);
            System.out.println("\nHint 1: The encryption algorithm is AES in ECB mode");
            System.out.println("Hint 2: Key length is 16 bytes");
            System.out.println("Hint 3: The key is related to CTF and current year");
            
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
//lvLxGWhMhiFoxBYRWqj0kR9EXj969qiY5ezBle58bqE=
//CTF_is_fun_2025!