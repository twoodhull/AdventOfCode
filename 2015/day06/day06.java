import java.util.Scanner;

public class day06 {
    static class Point {
        int x, y;

        public Point(int _x, int _y) {
            x = _x;
            y = _y;
        }

        public Point(String rep) {
            String[] vals = rep.split(",");
            y = Integer.parseInt(vals[0]);
            x = Integer.parseInt(vals[1]);
        }
    }

    static interface LightChanger {
        public boolean getNewValue(boolean value);
        public int getNewValue(int value);
    }

    static class Toggle implements LightChanger {
        public boolean getNewValue(boolean value) { return !value; }
        public int getNewValue(int value) { return value + 2; }
    }

    static class TurnOn implements LightChanger {
        public boolean getNewValue(boolean value) { return true; }
        public int getNewValue(int value) { return value + 1; }
    }

    static class TurnOff implements LightChanger {
        public boolean getNewValue(boolean value) { return false; }
        public int getNewValue(int value) { return Math.max(0, value - 1); }
    }

    static LightChanger toggle = new Toggle();
    static LightChanger turnOn = new TurnOn();
    static LightChanger turnOff = new TurnOff();

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);

        boolean[][] lights = new boolean[1000][1000];
        int[][] brightness = new int[1000][1000];

        while (in.hasNextLine()) {
            String line = in.nextLine();
            String[] split = line.split(" ");

            if (line.startsWith("toggle")) {
                changeLights(lights, brightness, new Point(split[1]), new Point(split[3]), toggle);
            } else if (line.startsWith("turn on")) {
                changeLights(lights, brightness, new Point(split[2]), new Point(split[4]), turnOn);
            } else if (line.startsWith("turn off")) {
                changeLights(lights, brightness, new Point(split[2]), new Point(split[4]), turnOff);
            }
        }

        in.close();

        int count = 0;
        int totalBrightness = 0;
        for (int i = 0; i < lights.length; i++) {
            for (int j = 0; j < lights[i].length; j++) {
                count += lights[i][j] ? 1 : 0;
                totalBrightness += brightness[i][j];
            }
        }

        System.out.println("Part 1 Count: " + count);
        System.out.println("Part 2 Brightness: " + totalBrightness);
    }

    static void changeLights(boolean[][] lights, int[][] brightness, Point topLeft, Point bottomRight, LightChanger changer) {
        for (int y = topLeft.y; y <= bottomRight.y; y++) {
            for (int x = topLeft.x; x <= bottomRight.x; x++) {
                lights[y][x] = changer.getNewValue(lights[y][x]);
                brightness[y][x] = changer.getNewValue(brightness[y][x]);
            }
        }
    }
}
