import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class day07 {
    static class Wire {
        int value = -1;
        ArrayList<Gate> outputs = new ArrayList<>();

        public String toString() {
            return Integer.toString(value);
        }
    }

    static class Gate {
        static int mask = ((1 << 16) - 1);

        Wire output = null;
        String op = "";
        ArrayList<Wire> inputs = new ArrayList<>();
        int resolved = 0;

        void writeValue() {
            if (output.value != -1) {
                return;
            }

            if (op.equals("AND")) {
                output.value = (inputs.get(0).value & inputs.get(1).value) & mask;
            } else if (op.equals("OR")) {
                output.value = (inputs.get(0).value | inputs.get(1).value) & mask;
            } else if (op.equals("LSHIFT")) {
                output.value = (inputs.get(0).value << inputs.get(1).value) & mask;
            } else if (op.equals("RSHIFT")) {
                output.value = (inputs.get(0).value >> inputs.get(1).value) & mask;
            } else if (op.equals("NOT")) {
                output.value = (~inputs.get(0).value) & mask;
            } else if (op.equals("WRITE")) {
                output.value = inputs.get(0).value;
            }
        }
    }

    static boolean isNumber(String str) {
        for (char c : str.toCharArray()) {
            if (!Character.isDigit(c)) {
                return false;
            }
        }

        return true;
    }

    static HashMap<String, Wire> parseInput(ArrayList<String> inputLines) {
        HashMap<String, Wire> wires = new HashMap<>();

        for (String line : inputLines) {
            String[] splitLine = line.split(" ");

            if (line.contains("AND") || line.contains("OR") || line.contains("LSHIFT") || line.contains("RSHIFT")) {
                wires.putIfAbsent(splitLine[0], new Wire());
                wires.putIfAbsent(splitLine[2], new Wire());
                wires.putIfAbsent(splitLine[4], new Wire());

                Wire input1 = wires.get(splitLine[0]);
                Wire input2 = wires.get(splitLine[2]);
                Wire output = wires.get(splitLine[4]);

                Gate gate = new Gate();

                input1.outputs.add(gate);
                input2.outputs.add(gate);

                gate.op = splitLine[1];
                gate.inputs.add(input1);
                gate.inputs.add(input2);
                gate.output = output;
            } else if (line.contains("NOT")) {
                wires.putIfAbsent(splitLine[1], new Wire());
                wires.putIfAbsent(splitLine[3], new Wire());

                Wire input = wires.get(splitLine[1]);
                Wire output = wires.get(splitLine[3]);

                Gate gate = new Gate();

                input.outputs.add(gate);

                gate.op = "NOT";
                gate.inputs.add(input);
                gate.output = output;
            } else {
                wires.putIfAbsent(splitLine[0], new Wire());
                wires.putIfAbsent(splitLine[2], new Wire());

                Wire input = wires.get(splitLine[0]);
                Wire output = wires.get(splitLine[2]);

                Gate gate = new Gate();

                input.outputs.add(gate);

                gate.op = "WRITE";
                gate.inputs.add(input);
                gate.output = output;
            }
        }

        return wires;
    }

    static int getWireValue(HashMap<String, Wire> wires, String wireName) {
        ArrayDeque<Wire> resolved = new ArrayDeque<>();

        for (String key : wires.keySet()) {
            if (isNumber(key)) {
                Wire wire = wires.get(key);
                wires.get(key).value = Integer.parseInt(key);
                resolved.add(wire);
            }
        }

        while (!resolved.isEmpty()) {
            Wire cur = resolved.poll();

            for (Gate g : cur.outputs) {
                g.resolved++;

                if (g.resolved == g.inputs.size()) {
                    g.writeValue();
                    resolved.add(g.output);
                }
            }
        }

        return wires.get(wireName).value;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);

        ArrayList<String> inputLines = new ArrayList<>();
        while (in.hasNextLine()) {
            inputLines.add(in.nextLine());
        }

        HashMap<String, Wire> wires = parseInput(inputLines);
        int aValue = getWireValue(wires, "a");
        System.out.println("Part 1: " + aValue);

        wires = parseInput(inputLines);
        wires.get("b").value = aValue;
        aValue = getWireValue(wires, "a");
        System.out.println("Part 2: " + aValue);

        in.close();
    }
}