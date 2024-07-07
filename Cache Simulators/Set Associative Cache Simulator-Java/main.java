import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class main {
    public static List<Long> storeAddresses(String filePath) {
        //an array list to store the decimal numbers after conting the hexadecimals
        List<Long> decimals = new ArrayList<>();

        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] lineWords = line.split("\\s+");

                for (String word : lineWords) {
                    if (word.length() != 0) {
                        word = word.trim();  
                        long decimal = Long.parseLong(word.substring(2), 16);//removing 0x
                        decimals.add(decimal);
                        
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return decimals;
    }

    public static void main(String[] args) {
        String filePath = args[0];
        List<Long> decimals = storeAddresses(filePath);

        int cacheSize = Integer.parseInt(args[1]);
        int associativity = Integer.parseInt(args[2]);
        int blockSize = 64; // Block size 64Bytes

        int totalSets = (cacheSize * 1024) / (associativity * blockSize);
        int setIndexBits = (int) (Math.log(totalSets) / Math.log(2));
        int blockOffsetBits = (int) (Math.log(blockSize) / Math.log(2));

        List<List<Long>> cache = new ArrayList<>();
        for (int i = 0; i < totalSets; i++) {
            cache.add(new ArrayList<>());
        }

        int totalMisses = 0;
        int totalHits = 0;
        int[] setMisses = new int[totalSets];
        int[] setHits = new int[totalSets];

        // Iterate over the decimal addresses
        for (long address : decimals) {
            int setIndex = (int) ((address >> blockOffsetBits) & ((1L << setIndexBits) - 1L));
            long tag = address >> (blockOffsetBits + setIndexBits);

            List<Long> set = cache.get(setIndex);
            boolean isHit = false;

            // Check for cache hits
            for (int i = 0; i < set.size(); i++) {
                if (set.get(i) == tag) {
                    isHit = true;
                    totalHits++;
                    setHits[setIndex]++;

                    // Move the hit tag to the end of the set (recently accessed)
                    set.remove(i);
                    set.add(tag);

                    break;
                }
            }

            // Cache miss
            if (!isHit) {
                    totalMisses++;
                    setMisses[setIndex]++;

                if (set.size() < associativity) {
                    set.add(tag);
                } else {
                    // Replacing the least recently used block (LRU)
                    set.remove(0);
                    set.add(tag);
                }
            }
        }

        System.out.println("Total Misses: " + totalMisses);
        System.out.println("Total Hits: " + totalHits);
        System.out.println("Set-wise Misses:");
        for (int i = 0; i < totalSets; i++) {
            System.out.println("Set " + i + ": " + setMisses[i]);
        }
        System.out.println("Set-wise Hits:");
        for (int i = 0; i < totalSets; i++) {
            System.out.println("Set " + i + ": " + setHits[i]);
        }
    }
}
