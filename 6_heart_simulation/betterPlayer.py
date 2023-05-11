from Hand import Hand
from Card import Card, Suit
class betterPlayer:
    def __init__(self, name):
        suitRank=[3,0,1,2] #3 is hearts, 0 is clubs, 1 is diamonds, 2 is spades
        rankRank=[14,13,12,11,10,9,8,7,6,5,4,3,2]
#        rankRank=[2,3,4,5,6,7,8,9,10,11,12,13,14]
        self.name=name
        self.hand=Hand()
        self.score=0
        self.tricksWon=[]
        self.passRank=self.makePassRank(suitRank,rankRank)
    
    def addCard(self, card):
        self.hand.addCard(card)


    def play(self, option='play', c=None,auto=False):
        if option is 'pass':
            card=self.getBestPass()       
        else:
            if c is None:
                card=self.hand.getRandomCard()
                return self.hand.containsCard(card.rank.rank,card.suit.iden)
            else:
                return self.hand.playCard(c)
       # card=self.hand.playCardBetter(card)
        

    def trickWon(self, trick):
        self.score += trick.points

    def hasSuit(self, suit):
        return len(self.hand.hand[suit.iden])>0

    def removeCard(self, card):
        self.hand.removeCard(card)

    def discardTricks(self):
        self.tricksWon=[]

    def hasOnlyHearts(self):
        return self.hand.hasOnlyHearts()

    def getBestPass(self):
        for best in self.passRank:
            if self.hand.containsCard(best.rank.rank,best.suit.iden):
                return best

    def makePassRank(self,suitRank,rankRank):
        passRank=[]
        #heart>other
        for r in rankRank:
            for s in suitRank:
                passRank.append(Card(r,s))
        king=passRank[4]
        queen=passRank[8]
        jack=passRank[12]
        ten=passRank[16]

        #king>other
        for i in range(3,0,-1):
            passRank[i+1]=passRank[i]
        passRank[1]=king

        
        #queen>other
        for i in range(7,1,-1):
            passRank[i+1]=passRank[i]
        passRank[2]=queen

        #jack>other
        
        for i in range(11,2,-1):
            passRank[i+1]=passRank[i]
        passRank[3]=jack

        #10>other
        #'''
        for i in range(15,3,-1):
            passRank[i+1]=passRank[i]
        passRank[4]=ten
        #'''

        return passRank
def main():
    player=betterPlayer("me")
    print player.play(option='pass').__str__()
    string=""
    for i in player.passRank:
        string=string+i.__str__()+" "
    print string

if __name__=='__main__':
    main()

