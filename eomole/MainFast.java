import java.io.FileInputStream;
import java.util.*;

class MainFast {
    private static final boolean DEBUG = false;
    private static final int RUNNUM = 10;
    private static final boolean YIZUMI_OUTPUT = true;
    private static final int BEAM_BOUND = 10;

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
            if(id < 0)
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
                final TSG[] tsgs = new TSG[sourceLength + 1];
                for (int i = 0; i < tsgs.length; i++)
                    tsgs[i] = new TSG();
                tsgs[0].add(new Game(new Board(initial), unitSeq[0], unitSeq, 0, 0, 0, "", new HashSet<>()));
                FinalScore best = new FinalScore(0, "");

                // search
                for (final TSG tsg : tsgs)
                    while (!tsg.isEmpty()) {
                        final Game g = tsg.pollFirst();

                        if (DEBUG)
                            if (g.cunit != null && !g.b.isLocked(g.cunit)) {
                                System.out.println("Num:\t" + g.uidx);
                                System.out.println("Score:\t" + g.moveScore + ", " + g.phraseBonus);
                                g.b.printWithAUnit(g.cunit);
                            }

                        final FinalScore r = g.finish();

                        if (best.compareTo(r) > 0)
                            best = r;
                        final ArrayList<Game> nexts = g.generateNexts();
                        for (final Game ng : nexts) {
                            tsgs[ng.uidx].add(ng);
                            while (tsgs[ng.uidx].size() > BEAM_BOUND)
                                tsgs[ng.uidx].pollLast();
                        }
                    }
                validate(initial, best.solution, unitSeq);

                System.err.println(id + "\t" + seed + "\t" + best.score + "pt\n\t" + best.solution);

                if (YIZUMI_OUTPUT)
                    solutions.add(id + " " + seed + "\n" + best.solution);
                else
                    solutions.add(
                            String.format("{\"seed\": %d, \"solution\": \"%s\", \"tag\": \"java%d\", \"problemId\": %d}",
                                    seed, best.solution, RUNNUM, id));

            }
        }

        if (YIZUMI_OUTPUT) {
            final StringBuilder sb = new StringBuilder().append(solutions.size());
            for (final String s : solutions)
                sb.append(s).append('\n');
            System.out.println(sb.toString());
        } else {
            final StringBuilder sb = new StringBuilder();
            for (final String s : solutions) {
                if (sb.length() > 0)
                    sb.append(',');
                sb.append(s);
            }
            System.out.println("[" + sb + "]");
        }

    }

    static String[] phrases = {
            "ei!",
            "ia! ia!",
            "r'lyeh",
            "yuggoth",
            "tsathoggua",
            "necronomicon",
            "cthulhu fhtagn!",
            "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn.",
            "in his house at r'lyeh dead cthulhu waits dreaming.",
            "planet 10",
    };

    static void validate(Board b, String commands, Unit[] units) {
        b = new Board(b);
        final HashSet<Unit> visited = new HashSet<>();
        Unit u = units[0];
        int nxt = 1;
        boolean closed = false;
        final char[] cs = commands.toCharArray();
        for (int i = 0; i < cs.length; i++) {
            if (closed)
                throw new RuntimeException("already closed at " + i + " in\n\t" + commands);

            visited.add(u);
            final Command c = Decoder.decode(cs[i]);
            final Unit nu = b.doCommand(c, u);
            if (visited.contains(nu))
                throw new RuntimeException("same position at " + i + " in\n\t" + commands);

            if (b.isLocked(nu)) {
                b.lock(u);
                b.remove();
                visited.clear();

                if (nxt < units.length) {
                    u = units[nxt++];
                    if(b.isLocked(u)) {
                        u = null;
                        closed = true;
                    }
                } else {
                    u = null;
                    closed = true;
                }
            } else {
                u = nu;
            }
        }
    }

    static class TSG extends TreeSet<Game> {
    }

    static class Game implements Comparable<Game> {
        static int nid = 0;

        public Game(Board b, Unit cunit,
                    Unit[] units, int uidx, int moveScore, int ls_old_m1, String commands, HashSet<Unit> visited) {
            this.visited = visited;
            this.b = b;
            this.units = units;
            this.cunit = cunit;
            this.uidx = uidx;
            this.moveScore = moveScore;
            this.ls_old_m1 = ls_old_m1;
            this.commands = commands;

            int pb = 0;
            int mask = 0;
            for (int i = 0; i < phrases.length; i++) {
                int cnt = 0;
                for (int j = commands.indexOf(phrases[i]); j >= 0; j = commands.indexOf(phrases[i], j + 1))
                    cnt++;
                if (cnt > 0) {
                    mask |= 1 << i;
                    pb += 2 * phrases[i].length() * cnt + 300;
                }
            }
            phraseBonus = pb;
            usedPhraseMask = mask;
            h2 = b.h / 2;
            visited.add(cunit);
        }

        public Game copy() {
            return new Game(b, cunit, units, uidx, moveScore, ls_old_m1, commands, new HashSet<>(visited));
        }

        public Game fork(Unit u, String cs) {
            return new Game(b, u, units, uidx, moveScore, ls_old_m1, cs, new HashSet<>(visited));
        }

        final Board b;
        final Unit cunit;
        final Unit[] units;
        final int uidx, moveScore, ls_old_m1;
        final String commands;
        final HashSet<Unit> visited;

        // computed in constructor
        final int phraseBonus;
        final int usedPhraseMask;
        final int id = nid++;
        final int h2;

        static final int[] di = {-1, 0, 1, 1};
        static final int[] dj = {0, -1, -1, 0};
        static final Command[] moveCommands = {Command.MoveE, Command.MoveW, Command.MoveSE, Command.MoveSW};
        static final Command[] turnCommands = {Command.TurnClockwise, Command.TurnCounterClockwise};

        int[][] makeDistanceMap() {
            // find most distant cell
            final int[][] distMap = new int[b.map.length][b.map[0].length];
            for (final int[] is : distMap)
                Arrays.fill(is, Integer.MAX_VALUE);

            final HashSet<Unit> vv = new HashSet<>(visited);
            final ArrayDeque<Unit> que = new ArrayDeque<>();
            que.offer(cunit);
            vv.add(cunit);
            distMap[cunit.pivot.x + h2][cunit.pivot.y] = 0;
            while (!que.isEmpty()) {
                final Unit u = que.pollFirst();
                for (final Command c : Command.values()) {
                    final Unit nu = b.doCommand(c, u);
                    // TODO: full-support pivots outside of the board
                    if (!vv.contains(nu) && !b.isLocked(nu) && b.isIn(nu.pivot)) {
                        if (distMap[nu.pivot.x + h2][nu.pivot.y] > distMap[u.pivot.x + h2][u.pivot.y] + 1) {
                            que.offer(nu);
                            vv.add(nu);
                            distMap[nu.pivot.x + h2][nu.pivot.y] = distMap[u.pivot.x + h2][u.pivot.y] + 1;
                        }
                    }
                }
            }

            return distMap;
        }

        int getFurthestDistance(int[][] distMap) {
            int max = 0;
            for (int i = 0; i < distMap.length; i++)
                for (int j = 0; j < distMap[i].length; j++)
                    if (distMap[i][j] < Integer.MAX_VALUE && distMap[i][j] > max) {
                        max = distMap[i][j];
                    }
            return max;
        }

        ArrayList<Cell> generateCandidates(int[][] distMap, int max) {
            final ArrayList<Cell> ret = new ArrayList<>();
            for (int i = 0; i < distMap.length; i++)
                for (int j = 0; j < distMap[i].length; j++)
                    if (distMap[i][j] < Integer.MAX_VALUE && distMap[i][j] == max)
                        ret.add(new Cell(i - h2, j));
            return ret;
        }

        Cell restorePath(int[][] distMap, Cell c) {
            for (int d = 0; d < 4; d++) {
                final Cell nc = new Cell(c.x + di[d], c.y + dj[d]);
                if (b.isIn(nc))
                    if (distMap[nc.x + h2][nc.y] == distMap[c.x + h2][c.y] - 1)
                        return nc;
            }
            throw null;
        }

        Unit planCommands(Cell next, Unit u, ArrayList<Command> plan, HashSet<Unit> visited) {
            // decide move operation
            Command move = null;
            for (final Command c : moveCommands) {
                final Unit nu = b.doCommand(c, u);
                if (nu.pivot.equals(next)) {
                    if (!b.isLocked(nu) && !visited.contains(nu)) {
                        plan.add(c);
                        visited.add(nu);
                        return nu;
                    }
                    move = c;
                    break;
                }
            }
            if (move == null)
                throw null;

            // try turn operation
            for (final Command c : turnCommands) {
                Unit now = u;
                for (int i = 1; i <= 4; i++) {
                    final Unit turned = b.doCommand(c, now);
                    if (b.isLocked(turned) || visited.contains(turned))
                        break;
                    final Unit nu = b.doCommand(move, turned);
                    if (!b.isLocked(nu) && !visited.contains(nu)) {
                        for (int j = 0; j < i; j++)
                            plan.add(c);
                        visited.add(nu);
                        return nu;
                    }
                    now = turned;
                }
            }
            b.printWithAUnit(u);
            throw null;
        }

        Command tryToLock(Unit u) {
            for (final Command c : Command.values()) {
                final Unit last = b.doCommand(c, u);
                if (b.isLocked(last))
                    return c;
            }
            return null;
        }

        String encode(ArrayList<Command> plan) {
            final StringBuilder sb = new StringBuilder();
            for (final Command c : plan)
                sb.append(c.c);
            return sb.toString();
        }

        Game advanceToNewUnit(Unit u, String commands) {
            final Board newBoard = new Board(b);
            final int ls = newBoard.lock(u).remove();
            final int point = u.members.length + 100 * ls * (ls + 1) / 2;
            final int newMoveScore = moveScore + point + ls_old_m1 * point / 10;

            return new Game(newBoard, uidx + 1 < units.length ? units[uidx + 1] : null, units, uidx + 1,
                    newMoveScore, ls > 0 ? ls - 1 : 0, commands, new HashSet<>());

        }

        Game tryToEmbedPhrase(String phrase, String commands) {
            Unit u = cunit;
            final StringBuilder sb = new StringBuilder(commands);
            final char[] cs = phrase.toCharArray();
            for (int i = 0; i < cs.length; i++) {
                if (u == null || b.isLocked(u))
                    return null;

                final Command c = Decoder.decode(cs[i]);
                sb.append(cs[i]);
                final Unit nu = b.doCommand(c, u);
                if (b.isLocked(nu)) {
                    return advanceToNewUnit(u, sb.toString()).
                            tryToEmbedPhrase(phrase.substring(i + 1), sb.toString());
                } else {
                    if (visited.contains(nu))
                        return null;

                    visited.add(u = nu);
                }
            }
            return new Game(b, u, units, uidx, moveScore, ls_old_m1, sb.toString(), visited);
        }

        void tryDest(Cell dest, int[][] distMap, ArrayList<Game> nexts) {
            final ArrayDeque<Cell> path = new ArrayDeque<>();
            for (Cell c = dest; !c.equals(cunit.pivot); c = restorePath(distMap, c))
                path.offerFirst(c);

            final ArrayList<Command> plan = new ArrayList<>();
            final HashSet<Unit> v = new HashSet<>(visited);
            Unit u = cunit;
            while (!path.isEmpty())
                u = planCommands(path.pollFirst(), u, plan, v);

            final Command lock = tryToLock(u);
            if (lock != null) {
                plan.add(lock);
                nexts.add(advanceToNewUnit(u, commands + encode(plan)));
            }

            Unit uu = cunit;
            final StringBuilder sb = new StringBuilder(commands);
            int mask = 0;
            for (final Command c : plan) {
                for (int i = 0; i < phrases.length; i++) {
                    final String s = sb.toString();
                    if (((mask | usedPhraseMask) & 1 << i) == 0) {
                        final Game emb = fork(uu, sb.toString()).tryToEmbedPhrase(phrases[i], s);
                        if(emb != null) {
                            nexts.add(emb);
                            mask |= 1 << i;
                        }
                    }
                }
                sb.append(c.c);
                uu = b.doCommand(c, uu);
                visited.add(uu);
            }
        }

        public ArrayList<Game> generateNexts() {
            if (cunit == null || b.isLocked(cunit) || !b.isIn(cunit.pivot))
                return new ArrayList<>();

            final ArrayList<Game> nexts = new ArrayList<>();

            final int[][] distMap = makeDistanceMap();
            final int max = getFurthestDistance(distMap);

            for (final Cell cand : generateCandidates(distMap, max))
                copy().tryDest(cand, distMap, nexts);

            return nexts;
        }

        FinalScore finish() {
            return new FinalScore(moveScore + phraseBonus, commands);
        }

        @Override
        public int compareTo(Game o) {
            final int d_score = o.moveScore + o.phraseBonus - moveScore - phraseBonus;
            if (d_score != 0)
                return d_score;
            final int d_uidx = uidx - o.uidx;
            if (d_uidx != 0)
                return d_uidx;
            final int d_pnum = Integer.bitCount(phraseBonus) - Integer.bitCount(o.phraseBonus);
            if (d_pnum != 0)
                return d_pnum;
            final int d_vnum = visited.size() - o.visited.size();
            if (d_vnum != 0)
                return d_vnum;
            // TODO:
            return id - o.id;
        }
    }

    static class FinalScore implements Comparable<FinalScore> {
        final int score;
        final String solution;

        public FinalScore(int score, String solution) {
            this.score = score;
            this.solution = solution;
        }

        @Override
        public int compareTo(FinalScore o) {
            return score == o.score ? solution.compareTo(o.solution) : o.score - score;
        }
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
                if (c.x < -c.y / 2 || c.x >= w - c.y / 2 || c.y < 0 || h <= c.y || map[c.x + h / 2][c.y])
                    return true;
            return false;
        }

        public boolean isIn(Cell c) {
            return c.x >= -h / 2 && c.x < w && 0 <= c.y && c.y < h;
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


        public int remove() {
            // remove

            int d = 0;
            for (int i = h - 1; i >= 0; i--) {
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

            return d;
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

    static String yizumiCode(String s) {
        final StringBuilder sb = new StringBuilder();
        for (final char c : s.toCharArray())
            sb.append(Decoder.decode(c).number);
        return sb.toString();
    }
}

