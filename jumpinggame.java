import java.util.*;

class perm {

    public static int total = 0;

    public static int print(int[] perm, int len, int u, int d) {
        int count = 0;
        for (int i = 0; i < perm.length - 1; i++) {
            if ((perm[i] - perm[i+1] <= d && perm[i] - perm[i+1] >= 0) || (perm[i] - perm[i+1] >= -u && perm[i] - perm[i+1] <= 0)) {
                count++;
            } else {
                break;
            }
            if (count >= perm.length - 1) {
                return 1;
            }
        }
        return 0;
    }

    public static int printperms(int[] perm, int[] nums, boolean[] used, int k, int u, int d) {
        if (k == perm.length)
            total += print(perm, perm.length, u, d);

        for (int i = 0; i < perm.length; i++) {
            if (!used[i]) {
                used[i] = true;
                perm[k] = nums[i];
                printperms(perm, nums, used, k + 1, u, d);
                used[i] = false;
            }
        }
        return total;

    }

    public static void resetTotal() {
        total = 0;
    }
}

public class jumpinggame {
    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();

        for (int i = 0; i < n; i++) {

            int length = sc.nextInt();
            int[] nums = new int[length];
            int[] perms = new int[length];
            for (int x = 0; x < length; x++) {
                nums[x] = sc.nextInt();
            }
            int up = sc.nextInt();
            int down = sc.nextInt();

            boolean[] used = new boolean[length];
            Arrays.fill(used, false);

            System.out.println(perm.printperms(perms, nums, used, 0, up, down));
            perm.resetTotal();
        }

        sc.close();
    }
}
