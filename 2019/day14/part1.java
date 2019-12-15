import java.io.File;
import java.io.FileReader;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class part1 {
    static class Pair<T1, T2> {
        T1 first;
        T2 second;

        Pair(T1 first, T2 second) {
            this.first = first;
            this.second = second;
        }

        public String toString() {
            return "(" + first + ", " + second + ")";
        }
    }

    static class Reaction {
        ArrayList<Pair<String, Integer>> inputs = new ArrayList<>();
        String outputName;
        int outputQuantity;
    }

    public static void main(String[] args) throws Exception {
        Scanner in = new Scanner(System.in);

        HashMap<String, Reaction> reactionMap = new HashMap<>();
        Pattern reactionPattern = Pattern.compile("\\d+|[A-Z]+");
        while (in.hasNextLine()) {
            String input = in.nextLine();

            ArrayList<String> allMatches = new ArrayList<>();
            Matcher m = reactionPattern.matcher(input);

            while (m.find()) {
                allMatches.add(m.group());
            }

            Reaction r = new Reaction();
            r.outputName = allMatches.get(allMatches.size() - 1);
            r.outputQuantity = Integer.parseInt(allMatches.get(allMatches.size() - 2));

            for (int i = 0; i < allMatches.size() - 2; i += 2) {
                r.inputs.add(new Pair<>(allMatches.get(i + 1), Integer.parseInt(allMatches.get(i))));
            }

            reactionMap.put(r.outputName, r);
        }

        int oreCount = 0;
        HashMap<String, Integer> excess = new HashMap<>();
        ArrayDeque<Pair<String, Integer>> q = new ArrayDeque<>();
        q.add(new Pair<>("FUEL", 1));

        while (!q.isEmpty()) {
            Pair<String, Integer> cur = q.poll();

            if (cur.first.equals("ORE")) {
                oreCount += cur.second;
                continue;
            }

            int amountNeeded = cur.second;

            if (excess.containsKey(cur.first)) {
                int excessAmount = excess.get(cur.first);

                if (excessAmount > amountNeeded) {
                    excess.put(cur.first, excessAmount - amountNeeded);
                    amountNeeded = 0;
                } else {
                    amountNeeded -= excessAmount;
                    excess.remove(cur.first);
                }
            }

            if (amountNeeded == 0) {
                continue;
            }

            Reaction r = reactionMap.get(cur.first);
            int reactionCount = amountNeeded / r.outputQuantity;

            if (amountNeeded % r.outputQuantity != 0) {
                reactionCount++;
            }

            for (Pair<String, Integer> input : r.inputs) {
                q.offer(new Pair<>(input.first, input.second * reactionCount));
            }

            int produced = r.outputQuantity * reactionCount;
            int leftOver = produced - amountNeeded;

            if (leftOver > 0) {
                if (excess.containsKey(cur.first)) {
                    excess.put(cur.first, excess.get(cur.first) + leftOver);
                } else {
                    excess.put(cur.first, leftOver);
                }
            }
        }

        System.out.println("Need " + oreCount + " ORE");

        in.close();
    }
}
