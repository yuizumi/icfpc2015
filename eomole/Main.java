import java.io.FileInputStream;
import java.util.*;

class Main {
    private static final boolean DEBUG = false;
    private static final int RUNNUM = 8;

    public static void main(String... args) throws Exception {
        {
            boolean f = false;
            for (final String s : args)
                if (s.equals("-f"))
                    f = true;
                else if (f) {
                    System.setIn(new FileInputStream(s));
                    f = false;
                }
        }

        List<String> solutions = new ArrayList<>();

        final Scanner sc = new Scanner(System.in);
        while (sc.hasNext()) {
            final int id = sc.nextInt();
            if (id < 0)
                break;

            final Unit[] units = new Unit[sc.nextInt()];
            for (int i = 0; i < units.length; i++) {
                final Cell[] members = new Cell[sc.nextInt()];
                for (int j = 0; j < members.length; j++)
                    members[j] = Cell.convert(sc.nextInt(), sc.nextInt());
                units[i] = new Unit(members, Cell.convert(sc.nextInt(), sc.nextInt()));
            }
            final Board initial = new Board(sc.nextInt(), sc.nextInt());
            final Cell[] filled = new Cell[sc.nextInt()];
            for (int i = 0; i < filled.length; i++) {
                filled[i] = Cell.convert(sc.nextInt(), sc.nextInt());
                initial.map[filled[i].x + initial.h / 2][filled[i].y] = true;
            }
            final int sourceLength = sc.nextInt();
            final int[] sourceSeeds = new int[sc.nextInt()];
            for (int i = 0; i < sourceSeeds.length; i++)
                sourceSeeds[i] = sc.nextInt();

            if (DEBUG) {
                initial.print();
                for (final Unit unit : units)
                    initial.printWithAUnit(initial.appear(unit));
            }

            for (final int seed : sourceSeeds) {
                final PRNG prng = new PRNG(seed);
                final Unit[] unitSeq = new Unit[sourceLength];
                for (int i = 0; i < sourceLength; i++)
                    unitSeq[i] = initial.appear(units[prng.nextInt() % units.length]);
                final String solution = dfs(new Board(initial), unitSeq[0], unitSeq, 1, new HashSet<>(),
                        new boolean[phrases.length], -1, 0);
                solutions.add(String.format("{\"seed\": %d, \"solution\": \"%s\", \"tag\": \"java%d\", \"problemId\": %d}",
                        seed, solution, RUNNUM, id));
                // for yizumi interface
//                {
//                    final StringBuilder sb = new StringBuilder();
//                    for(final char c : solution.toCharArray())
//                        sb.append(Decoder.decode(c).number);
//                    solutions.add(sb.toString());
//                }
                if (DEBUG)
                    System.out.println(solution);
            }
        }

        final StringBuilder sb = new StringBuilder();
        for (final String s : solutions) {
            if (sb.length() > 0)
                sb.append(',');
            sb.append(s);
        }
        System.out.println("[" + sb + "]");

    }

    static String[] phrases = {
            "Ei!",
            "Ia! Ia!",
            "R'lyeh",
            "Yuggoth",
            "Tsathoggua",
            "Necronomicon",
            "Cthulhu fhtagn!",
            "Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn.",
            "In his house at R'lyeh dead Cthulhu waits dreaming.",
            "Planet 10",
    };

    static String dfs(Board b, Unit u, Unit[] unitSeq, int idx, HashSet<Unit> visited, boolean[] used, int pidx, int cidx) {

        if (DEBUG) {
            System.out.println("depth = " + idx);
            System.out.println(u);
            b.printWithAUnit(u);
        }

        // try new phrase
        if (pidx < 0)
            for (int i = 0; i < phrases.length; i++)
                if (!used[i]) {
                    used[i] = true;
                    final String s = dfs(b, u, unitSeq, idx, visited, used, i, 0);
                    if (s != null && s.startsWith(phrases[i])) {
                        if (DEBUG)
                            System.out.println("try " + phrases[i] + ":\t" + s);
                        return s;
                    }
                    used[i] = false;
                }

        final Command[] commands = pidx < 0 ?
                Command.values() :
                new Command[]{Decoder.decode(phrases[pidx].charAt(cidx))};

        // TODO: unsafe
        visited.add(u);

        final boolean flag = pidx < 0 || cidx >= phrases[pidx].length() - 1;


        for (Command c : commands) {
            final Unit n = b.doCommand(c, u);
            if (visited.contains(n))
                continue;
            if (b.isLocked(n)) {
                if (idx >= unitSeq.length) {
                    if (DEBUG) {
                        System.out.println("depth (a) = " + idx);
                        b.printWithAUnit(u);
                    }
                    visited.remove(u);
                    return "" + (pidx < 0 ? c.c : phrases[pidx].charAt(cidx));
                }
                final Board newBoard = new Board(b).lock(u).remove();
                if (newBoard.isLocked(unitSeq[idx])) {
                    if (DEBUG) {
                        System.out.println("depth (b) = " + idx);
                        b.printWithAUnit(u);
                    }
                    visited.remove(u);
                    return "" + (pidx < 0 ? c.c : phrases[pidx].charAt(cidx));
                }
                final String s = dfs(newBoard, unitSeq[idx], unitSeq, idx + 1, new HashSet<>(), used,
                        flag ? -1 : pidx, flag ? 0 : cidx + 1);
                if (s != null) {
                    if (DEBUG) {
                        System.out.println("depth (c) = " + idx + " (" + s.length() + ")");
                        b.printWithAUnit(u);
                    }
                    visited.remove(u);
                    return (pidx < 0 ? c.c : phrases[pidx].charAt(cidx)) + s;
                }
                continue;
            }
            final String s = dfs(b, n, unitSeq, idx, visited, used,
                    flag ? -1 : pidx, flag ? 0 : cidx + 1);
            if (s != null) {
                if (DEBUG) {
                    System.out.println("depth (d) = " + idx + " (" + s.length() + ")");
                    b.printWithAUnit(u);
                }
                visited.remove(u);
                return (pidx < 0 ? c.c : phrases[pidx].charAt(cidx)) + s;
            }
        }
        visited.remove(u);
        return null;
    }


    static enum Command {
        MoveSW('4', 4), MoveSE('5', 3), MoveW('3', 2), MoveE('2', 1), TurnCounterClockwise('x', 5), TurnClockwise('1', 6),;

        final char c;
        final int number;

        Command(char c, int number) {
            this.c = c;
            this.number = number;
        }
    }

    static class Board {
        final int w, h;
        final boolean[][] map;

        public Board(int w, int h) {
            this.h = h;
            this.w = w;
            this.map = new boolean[w + h / 2][h];
        }

        public boolean isLocked(Unit u) {
            for (Cell c : u.members)
                if (c.x < -c.y / 2 || c.x >= w - c.y / 2 || c.y < 0 || c.y >= h || map[c.x + h / 2][c.y])
                    return true;
            return false;
        }

        public Unit doCommand(Command c, Unit u) {
            switch (c) {
                case MoveW: {
                    final Cell newPivot = new Cell(u.pivot.x - 1, u.pivot.y);
                    final Cell[] newMembers = new Cell[u.members.length];
                    for (int i = 0; i < newMembers.length; i++)
                        newMembers[i] = new Cell(u.members[i].x - 1, u.members[i].y);
                    return new Unit(newMembers, newPivot);
                }
                case MoveE: {
                    final Cell newPivot = new Cell(u.pivot.x + 1, u.pivot.y);
                    final Cell[] newMembers = new Cell[u.members.length];
                    for (int i = 0; i < newMembers.length; i++)
                        newMembers[i] = new Cell(u.members[i].x + 1, u.members[i].y);
                    return new Unit(newMembers, newPivot);
                }
                case MoveSW: {
                    final Cell newPivot = new Cell(u.pivot.x - 1, u.pivot.y + 1);
                    final Cell[] newMembers = new Cell[u.members.length];
                    for (int i = 0; i < newMembers.length; i++)
                        newMembers[i] = new Cell(u.members[i].x - 1, u.members[i].y + 1);
                    return new Unit(newMembers, newPivot);
                }
                case MoveSE: {
                    final Cell newPivot = new Cell(u.pivot.x, u.pivot.y + 1);
                    final Cell[] newMembers = new Cell[u.members.length];
                    for (int i = 0; i < newMembers.length; i++)
                        newMembers[i] = new Cell(u.members[i].x, u.members[i].y + 1);
                    return new Unit(newMembers, newPivot);
                }
                case TurnClockwise: {
                    final Cell[] newMembers = new Cell[u.members.length];
                    for (int i = 0; i < newMembers.length; i++)
                        newMembers[i] = u.members[i].rotate(u.pivot, true);
                    return new Unit(newMembers, u.pivot);
                }
                case TurnCounterClockwise: {
                    final Cell[] newMembers = new Cell[u.members.length];
                    for (int i = 0; i < newMembers.length; i++)
                        newMembers[i] = u.members[i].rotate(u.pivot, false);
                    return new Unit(newMembers, u.pivot);
                }
            }
            throw null;
        }

        public void print() {
            System.out.println(w + " * " + h);
            for (int i = 0; i < h; i++) {
                final StringBuilder sb = new StringBuilder();
                if (i % 2 == 1)
                    sb.append(' ');
                for (int j = 0; j < w; j++)
                    sb.append(' ').append(map[j - i / 2 + h / 2][i] ? '*' : '.');
                System.out.println(sb);
            }
            System.out.println();
        }

        public void printWithAUnit(Unit u) {
            final Board b = new Board(w, h).lock(u);
            for (int i = 0; i < h; i++) {
                final StringBuilder sb = new StringBuilder();
                if (i % 2 == 1)
                    sb.append(' ');
                for (int j = 0; j < w; j++)
                    sb.append(' ')
                            .append(b.map[j - i / 2 + h / 2][i] ?
                                    map[j - i / 2 + h / 2][i] ? '^' : '@' :
                                    map[j - i / 2 + h / 2][i] ? '*' : '.');
                System.out.println(sb);
            }
            System.out.println();
        }

        public Board(Board b) {
            this.w = b.w;
            this.h = b.h;
            this.map = new boolean[b.map.length][b.map[0].length];
            for (int i = 0; i < map.length; i++)
                this.map[i] = Arrays.copyOf(b.map[i], b.map[i].length);
        }

        public Board lock(Unit u) {
            for (final Cell c : u.members)
                map[c.x + h / 2][c.y] = true;
            return this;
        }


        public Board remove() {
            // remove
            for (int i = h - 1, d = 0; i >= 0; i--) {
                boolean check = true;
                for (int j = 0; j < w; j++)
                    check &= map[j - i / 2 + h / 2][i];
                if (check) {
                    d++;
                    for (int j = 0; j < w; j++)
                        map[j - i / 2 + h / 2][i] = false;
                } else {
                    for (int j = 0; j < w; j++)
                        if (d > 0) {
                            map[j - (i + d) / 2 + h / 2][i + d] = map[j - i / 2 + h / 2][i];
                            map[j - i / 2 + h / 2][i] = false;
                        }
                }
            }
            return this;
        }

        public Unit appear(Unit next) {
            int minx = Integer.MAX_VALUE, maxx = Integer.MIN_VALUE;
            for (final Cell c : next.members) {
                minx = Math.min(minx, c.x + c.y / 2);
                maxx = Math.max(maxx, c.x + c.y / 2);
            }
            final int dy = 0; // unsafe
            final int dx = (w - maxx + minx - 1) / 2 - minx;
            final Cell newPivot = new Cell(next.pivot.x + dx, next.pivot.y + dy);
            final Cell[] newMembers = new Cell[next.members.length];
            for (int i = 0; i < newMembers.length; i++)
                newMembers[i] = new Cell(next.members[i].x + dx, next.members[i].y + dy);

            return new Unit(newMembers, newPivot);
        }
    }

    static class Unit {
        final Cell[] members;
        final Cell pivot;

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;

            Unit unit = (Unit) o;

            if (!Arrays.equals(members, unit.members)) return false;
            if (pivot != null ? !pivot.equals(unit.pivot) : unit.pivot != null) return false;

            return true;
        }

        @Override
        public int hashCode() {
            int result = members != null ? Arrays.hashCode(members) : 0;
            result = 31 * result + (pivot != null ? pivot.hashCode() : 0);
            return result;
        }

        public Unit(Cell[] members, Cell pivot) {
            this.members = members;
            Arrays.sort(members);
            this.pivot = pivot;
        }

        @Override
        public String toString() {
            final StringBuilder sb = new StringBuilder("Unit{");
            sb.append("members=").append(Arrays.toString(members));
            sb.append(", pivot=").append(pivot);
            sb.append('}');
            return sb.toString();
        }
    }

    static class Cell implements Comparable<Cell> {
        final int x, y;

        public Cell(int x, int y) {
            this.x = x;
            this.y = y;
        }

        static Cell convert(int x2, int y) {
            return new Cell(x2 - y / 2, y);
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;

            Cell cell = (Cell) o;

            if (x != cell.x) return false;
            if (y != cell.y) return false;

            return true;
        }

        @Override
        public int hashCode() {
            int result = x;
            result = 31 * result + y;
            return result;
        }

        private static double kSqrt3 = 1.7320508075688772;

        public Cell rotate(Cell pivot, boolean clockwise) {
            final int x0 = x - pivot.x;
            final int y0 = y - pivot.y;
            final double ex0 = 2.0 * x0 + y0;
            final double ey0 = y0 * kSqrt3;
            final double ex1 = (ex0 + (clockwise ? -ey0 : ey0) * kSqrt3) * .5;
            final double ey1 = ((clockwise ? ex0 : -ex0) * kSqrt3 + ey0) * .5;
            final int y1 = (int) (Math.round(ey1 / kSqrt3));
            final int x1 = (int) (Math.round((ex1 - y1) / 2.0));
            return new Cell(x1 + pivot.x, y1 + pivot.y);
        }

        @Override
        public String toString() {
            final StringBuilder sb = new StringBuilder("Cell{");
            sb.append("x=").append(x);
            sb.append(", y=").append(y);
            sb.append('}');
            return sb.toString();
        }

        @Override
        public int compareTo(Cell o) {
            return x == o.x ? y - o.y : x - o.x;
        }
    }

    static class PRNG {
        int stat;

        public PRNG(int seed) {
            this.stat = seed;
        }

        public int nextInt() {
            int ret = (stat >> 16) & 0x7FFF;
            stat = stat * 1103515245 + 12345;
            return ret;
        }
    }

    static class Decoder {
        static Map<Character, Command> map = new HashMap<>();

        static {
            for (final char c : "p'!.03".toCharArray())
                map.put(c, Command.MoveW);
            for (final char c : "bcefy2".toCharArray())
                map.put(c, Command.MoveE);
            for (final char c : "aghij4".toCharArray())
                map.put(c, Command.MoveSW);
            for (final char c : "lmno 5".toCharArray())
                map.put(c, Command.MoveSE);
            for (final char c : "dqrvz1".toCharArray())
                map.put(c, Command.TurnClockwise);
            for (final char c : "kstuwx".toCharArray())
                map.put(c, Command.TurnCounterClockwise);
        }

        static Command decode(char c) {
            c = Character.toLowerCase(c);
            if (!map.containsKey(c))
                throw new RuntimeException("" + c);
            return map.get(c);
        }
    }
}
