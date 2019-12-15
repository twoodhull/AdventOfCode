import java.io.File;
import java.io.FileReader;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class part2 {
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
        ArrayList<Pair<String, Long>> inputs = new ArrayList<>();
        String outputName;
        long outputQuantity;
    }

    static long getOreCount(HashMap<String, Reaction> reactionMap, long fuel) {
        long oreCount = 0;
        HashMap<String, Long> excess = new HashMap<>();
        ArrayDeque<Pair<String, Long>> q = new ArrayDeque<>();
        q.add(new Pair<>("FUEL", fuel));

        while (!q.isEmpty()) {
            Pair<String, Long> cur = q.poll();

            if (cur.first.equals("ORE")) {
                oreCount += cur.second;
                continue;
            }

            long amountNeeded = cur.second;

            if (excess.containsKey(cur.first)) {
                long excessAmount = excess.get(cur.first);

                if (excessAmount > amountNeeded) {
                    excess.put(cur.first, excessAmount - amountNeeded);
                    amountNeeded = 0;
                } else {
                    amountNeeded -= excessAmount;
                    excess.remove(cur.first);
                }
            }

            if (amountNeeded == 0L) {
                continue;
            }

            Reaction r = reactionMap.get(cur.first);
            long reactionCount = amountNeeded / r.outputQuantity;

            if (amountNeeded % r.outputQuantity != 0L) {
                reactionCount++;
            }

            for (Pair<String, Long> input : r.inputs) {
                q.offer(new Pair<>(input.first, input.second * reactionCount));
            }

            long produced = r.outputQuantity * reactionCount;
            long leftOver = produced - amountNeeded;

            if (leftOver > 0) {
                if (excess.containsKey(cur.first)) {
                    excess.put(cur.first, excess.get(cur.first) + leftOver);
                } else {
                    excess.put(cur.first, leftOver);
                }
            }
        }

        return oreCount;
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
            r.outputQuantity = Long.parseLong(allMatches.get(allMatches.size() - 2));

            for (int i = 0; i < allMatches.size() - 2; i += 2) {
                r.inputs.add(new Pair<>(allMatches.get(i + 1), Long.parseLong(allMatches.get(i))));
            }

            reactionMap.put(r.outputName, r);
        }

        long maxOre = 1000000000000L;
        long lo = 1;
        long hi = maxOre;

        while (hi > lo) {
            long mid = (lo + hi) / 2;
            long oreCount = getOreCount(reactionMap, mid);

            if (oreCount > maxOre) {
                hi = mid;
            } else if (oreCount < maxOre) {
                lo = mid + 1;
            } else {
                lo = hi = mid;
            }
        }

        while (getOreCount(reactionMap, lo) > maxOre) {
            lo--;
        }
        while (getOreCount(reactionMap, lo + 1) < maxOre) {
            lo++;
        }

        System.out.println(lo + " needs " + getOreCount(reactionMap, lo) + " ORE");

        in.close();
    }
}
