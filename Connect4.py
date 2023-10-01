MaxHeight = 3
toWin = 3
columns = 4
Done = 0
class Situation:
    def __init__(self,L,player):
        global MaxHeight
        self.children = []
        self.result = None
        self.L = L
        self.player = player
        self.play = None
        self.visited = False
    def breed(self):
        global MaxHeight,AllSituations,Done
        self.result = self.reflect()
        if self.result == 1-2*self.player:
            self.visited = True
            Done += 1
            print("Nodes done :",Done)
            return
        full = True
        for i in range(len(self.L)):
            col = self.L[i]
            if len(col)>MaxHeight:
                print("shit")
                print(self.L)
            elif len(col)==MaxHeight:
                continue
            else:
                full = False
                cL = self.L.copy()
                cL[i] = cL[i]+str(self.player)
                g = G_to_g(cL)
                n,s,c = index(g,MaxHeight)
                child = AllSituations[n]
                child = child[s]
                child = child[c]
                child.player = 1-self.player
                if not child.visited:
                    if child.result !=None:
                        input("Well shit..")
                    child.breed()
                self.children.append(child)
                if self.check_children() and self.player==0:
                    break
        if full:
            self.result = self.reflect()    
        self.visited = True
        Done += 1
        print("Nodes done :",Done)        
    def to_list(self):
        if len(self.children)==0:
            return self.L
        CR = []
        for child in self.children:
            CR.append(child.to_list())
        return CR
    def eval_tree(self):
        if len(self.children)==0:
            return (self.result,self.play)
        CR = []
        for child in self.children:
            CR.append(child.eval_tree())
        return [(self.result,self.play)]+CR
    def print_ends(self):
        if len(self.children)==0:
            self.reflect()
            print(self.L,self.result)
            return
        for child in self.children:
            child.print_ends()
    def reflect(self):
        global toWin,MaxHeight
        M = self.L
        #check rows
        for r in range(MaxHeight):
            consecutive = 0
            last = None
            for c in range(len(M)):
                if len(M[c])<r+1:
                    last = None
                    consecutive = 0
                    continue
                if M[c][r]==last:
                    consecutive += 1
                else:
                    consecutive = 0
                last = M[c][r]
                if consecutive == toWin-1:
                    return 1-2*int(last)
        #check columns
        for c in range(columns):
            consecutive = 0
            last = None
            for r in range(len(M[c])):
                if M[c][r]==last:
                    consecutive += 1
                else:
                    consecutive = 0
                last = M[c][r]
                if consecutive == toWin-1:
                    return 1-2*int(last)
        # up diagonals
        for c in range(columns-toWin+1):
            for r in range(MaxHeight-toWin+1):
                if len(M[c])-1 < r:
                    break
                last = M[c][r]
                good = True
                for i in range(1,toWin):
                    if r+i > len(M[c+i])-1 or M[c+i][r+i]!=last:
                        good = False
                        break
                if good:
                    return 1-2*int(last)
        # down diagonals
        for c in range(columns-toWin+1):
            for r in range(toWin-1,MaxHeight):
                if len(M[c])-1 < r:
                    break
                last = M[c][r]
                good = True
                for i in range(1,toWin):
                    if r-i > len(M[c+i])-1 or M[c+i][r-i]!=last:
                        good = False
                        break
                if good:
                    return 1-2*int(last)
        return 0
    def check_children(self):
        self.result = 2*self.player-1
        optimum_result = 1-2*self.player
        self.play = 0
        for i in range(len(self.children)):
            child = self.children[i]
            if self.result == None and child.result*optimum_result>=0:
                self.play = i
                self.result = child.result
            if child.result == None:
                self.result = None
                print("this is bizzare")
            if (child.result - self.result)*optimum_result < 0:
                continue
            elif (child.result - self.result)*optimum_result > 0:
                self.result = child.result
                self.play = i
            #    self.time = child.time + 1
            #elif child.time+1 < self.time:
            #    self.result = child.result
            #    self.play = i
            #    self.time = child.time + 1
        return self.result == optimum_result
    def __repr__(self):
        G = ""
        M = self.L
        for r in range(MaxHeight):
            row = ""
            for c in range(len(M)):
                if len(M[c])<r+1:
                    row += "X "
                else:
                    row += M[c][r] + " "
            G = row + "\n" + G
        G = "\n" + G
        G += "\n It is player "+str(self.player)+"'s turn."
        G += "\n The expected result is :"
        if self.result == None :
            G += " inconclusive." 
        elif self.result == 0:
            G += " a draw."
        elif self.result == 1:
            G += " Player 0 wins"
        elif self.result == -1:
            G += " Player 1 wins"
        else :
            G += " Shit! I haven't calculated this!"
        return G
    def Input(self,c):
        if len(self.L[c-1])==MaxHeight:
            print(f"invalid move! A column can only contain {MaxHeight} coins.")
            return None
        play = -1
        for i in range(c):
            if len(self.L[i])<MaxHeight:
                play += 1
        return self.children[play]
    def children_status(self):
        L = []
        for child in self.children:
            L.append((child.result,child.play,child.result))
        return L

def comb(N,r):
    c = 1
    for i in range(1,r+1):
        c = (c*(N-r+i))//i
    return c
def Comb(N,r):
    if N==r:
        s=""
        for _ in range(N):
            s += "1"
        return [s]
    if r==0:
        s=""
        for _ in range(N):
            s += "0"
        return [s]
    withoutMe = Comb(N-1,r)
    withMe = Comb(N-1,r-1)
    for i in range(len(withoutMe)):
        withoutMe[i] = "0"+withoutMe[i]
    for i in range(len(withMe)):
        withMe[i] = "1"+withMe[i]
    return withoutMe+withMe
def indexC(s):
    N = 0
    r = 0
    S = 0
    C = 0
    for c in s[::-1] :
        if c == '1':
            r += 1
        if N==r:
            C = 1
        elif N<r:
            C = 0
        elif r==0:
            C = 1
        else :
            C = C*N
            if c == '1':
                C = C//r
            else:
                C = C//(N-r)
        if c=='1':
            S += C
        N += 1
    return S
def indexS(t,Max):
    n = 0
    p = 0
    for x in t:
        n += x
        p += 1
    S = 0
    m = n
    for x in t:
        p -= 1
        for i in range(x):
            S += splits(m-i,p,Max)
        m -= x
    return S
def iSplits(n,parts,Max):
    L =[]
    if parts < 1:
        return []
    if parts==1:
        if n<=Max:
            return [(n,)]
        else:
            return []
    if parts == 2:
        for d in range(Max+1):
                    if n < 2*d:
                        break
                    if n-d > Max:
                        continue
                    L.append((d,n-d))
        return L
    for a in range(Max+1):
        if n<parts*a:
            break
        S = iSplits(n-parts*a,parts-1,Max-a)
        for s in S:
            t = [a]
            for x in s:
                t.append(x+a)
            t = tuple(t)
            L.append(t)
    return L
def Splits(n,parts,Max):
    L =[]
    if parts < 1:
        return []
    if parts==1:
        if n<=Max:
            return [(n,)]
        else:
            return []
    if parts == 2:
        for d in range(Max+1):
                if n>=d and n-d <= Max:
                    L.append((d,n-d))
        return L
    for a in range(Max+1):
        if n<a:
            break
        S = Splits(n-a,parts-1,Max)
        for s in S:
            t = [a]
            for x in s:
                t.append(x)
            t = tuple(t)
            L.append(t)
    return L
def distinct(t):
    L = []
    x0 = t[0]
    count = 0
    for x in t:
        if x!=x0:
            L.append(count)
            x0 = x
            count = 1
        else:
            count += 1
    L.append(count)
    return L
def multin(L):
    C = 1
    N = 0
    for x in L:
        for i in range(1,x+1):
            N += 1
            C = (C*N)//i
    return C
def splits(n,parts,Max):
    S = 0
    for s in iSplits(n,parts,Max):
        S += multin(distinct(s))
    return(S)
def splitAndComb(n,parts,Max,firstPlayer):
    S = Splits(n,parts,Max)
    if firstPlayer==0:
        C = Comb(n,n//2)
    else:
        C = Comb(n,n-n//2)
    M = [[(s,c) for c in C] for s in S]
    return M
def index(g,Max):
    n = 0
    for x in g[0]:
        n += x
    return (n,indexS(g[0],Max),indexC(g[1]))
def G_to_g(G):
    s = []
    c = ""
    for col in G:
        s.append(len(col))
        c += col
    return (tuple(s),c)
def g_to_G(g):
    s = g[0]
    c = g[1]
    M = []
    x0 = 0
    for x in s:
        M.append(c[x0:x0+x])
        x0 +=x
    return M
def Play():
    first_player = input("Do you want to go first, or let the computer play the first turn ? Type 'me' to take the first turn \n")
    if first_player=='me':
        first_player = 1
    else:
        first_player = 0
    GenBred(first_player)
    Game = AllSituations[0][0][0]
    if first_player == 0:
        Game = Game.children[Game.play]
    while True:
        print(Game)
        if (Game.play == None) or (Game.reflect() !=0):
            print("The game is done.")
            break
        move = input("Type the number of column you want to put a coin in and press enter. If you want to quit, type 'X' \n")
        if move == 'X':
            break
        if move not in [str(1+x) for x in range(columns)]:
            print("Invalid move! Try again..")
            continue
        move = int(move)
        Game = Game.Input(move)
        print("Ok")
        print(Game)
        if Game.play == None or Game.reflect()!=0:
            print("The game is done.")
            break
        Game = Game.children[Game.play]
AllSituations = []
def GenBred(first):
    global AllSituations,MaxHeight,toWin,columns
    countOfAllSit = 0
    for n in range(columns*MaxHeight+1):
        SC = splitAndComb(n,columns,MaxHeight,first)
        Sitt = []
        for S in SC:
            Sit = []
            for g in S:
                Sit.append(Situation(g_to_G(g),(first +n)%2))
                countOfAllSit += 1
            Sitt.append(Sit)
        AllSituations.append(Sitt)
    print("Done generating. Number of nodes present is : ",countOfAllSit)
    start = AllSituations[0][0][0]
    start.breed()
    print("Done breeding.\n")
#G = Situation(["00","001","11","1"],0)
#print(G.reflect())
Play()