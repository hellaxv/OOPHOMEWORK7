#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum Suits {
    CLUBS = 0,
    DIAMONDS = 1,
    HEARTS = 2,
    SPADES = 3
};

enum Ranks {
    ACE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13
};

enum Numbers {
    DECKSIZE = 52
};

class Card
{
protected:
    Suits suit = CLUBS;
    Ranks rank = ACE;
    bool isFaceUp = true;
public:
    Card() {}
    Card(Ranks inputRank, Suits inputSuit) : suit(inputSuit), rank(inputRank) {}
    void Flip()
    {
        isFaceUp = !isFaceUp;
    }

    Ranks getValue()
    {
        return rank;
    }

    Suits getSuit()
    {
        return suit;
    }

    friend ostream& operator<<(ostream& out, const Card& card);
};

ostream& operator<<(ostream& out, const Card& card)
{
    if (card.isFaceUp)
    {
        out << "\n" << card.rank << " " << card.suit;
    }
    else out << "XX";
    return out;
}

class Hand
{
protected:
    vector<Card*> hand;
public:
    void Add(Card* input)
    {
        hand.push_back(input);
    }

    void Clear()
    {
        hand.clear();
    }

    int GetValue() const
    {
        int sum = 0;
        for (Card* card : hand)
        {
            sum += card->getValue();
        }
        return sum;
    }

    virtual ~Hand()
    {
        Clear();
    }
};

class GenericPlayer : virtual public Hand
{
protected:
    string name;
public:
    GenericPlayer(const std::string& inputName) : name(inputName) {}

    virtual bool IsHitting() const = 0;

    bool IsBoosted() const
    {
        if (GetValue() > 21) return true;
        else return false;
    }

    void Bust()
    {
        cout << "\n" << name << " is Bust!\n";
    }

    friend ostream& operator<<(ostream& out, const GenericPlayer& genericPlayer);
};

ostream& operator<<(ostream& out, const GenericPlayer& genericPlayer)
{
    out << "\n" << genericPlayer.name << " has:\n";
    for (Card* card : genericPlayer.hand)
    {
        out << card->getValue() << " " << card->getSuit() << "\n";
    }
    out << genericPlayer.GetValue();
    return out;
}

class Player : virtual public GenericPlayer
{
public:
    Player(const string& inputName) : GenericPlayer(inputName) {}
    virtual bool IsHitting() const override
    {
        while (true)
        {
            cout << "\nTake another card? (Y/N): ";
            char* input = 0;
            cin >> input;
            if (cin && input == "Y") return true;
            else if (cin && input == "N") return false;
            else
            {
                system("cls");
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Incorrect input!";
            }
        }
    }

    void Win() const
    {
        cout << "\n" << name << " have Won!";
    }

    void Lose() const
    {
        cout << "\n" << name << " have Lost.";
    }

    void Push() const
    {
        cout << "\n" << name << " played a Draw";
    }
};

class House : virtual public GenericPlayer
{
public:
    House() : GenericPlayer("Diler") {}

    virtual bool IsHitting() const override
    {
        if (GetValue() <= 16) return true;
        else return false;
    }

    void FlipFirstCard()
    {
        hand.front()->Flip();
    }
};


/*HomeWork3
—оздать класс Deck, который наследует от класса Hand и представл€ет собой колоду карт.  ласс Deck имеет 4 метода:
1. vold Populate() - —оздает стандартную колоду из 52 карт, вызываетс€ из конструктора.
2. void Shuffle() - ћетод, который тасует карты, можно использовать функцию из алгоритмов STL random_shuffle
3. vold Deal (Hand& aHand) - метод, который раздает в руку одну карту
4. void AddltionalCards (GenericPlayer& aGenerlcPlayer) - раздает игроку дополнительные карты до тех пор, пока он может и хочет их получать
*/

class Deck : virtual public Hand
{
private:
public:
    void Populate()
    {
        for (int suitCount = CLUBS; suitCount <= SPADES; suitCount++)
        {
            for (int rankCount = ACE; rankCount <= KING; rankCount++)
            {
                Add(new Card(static_cast<Ranks>(rankCount), static_cast<Suits>(suitCount)));
            }
        }
    }

    Deck()
    {
        Populate();
    }

    void Shuffle()
    {
        random_shuffle(hand.begin(), hand.end());
    }

    void Deal(Hand& aHand)
    {
        if (!hand.empty())
        {
            aHand.Add(hand.back());
            hand.pop_back();
        }
        else
        {
            cout << "No cards left!";
        }

    }

    void AdditionalCards(GenericPlayer& aGenericPlayer)
    {
        while (!aGenericPlayer.IsBoosted() && aGenericPlayer.IsHitting())
        {
            Deal(aGenericPlayer);
        }
        if (aGenericPlayer.IsBoosted()) aGenericPlayer.Bust();
    }
};


/*HomeWork4
–еализовать класс Game, который представл€ет собой основной процесс игры. ” этого класса будет 3 пол€:
колода карт
рука дилера
вектор игроков.
 онструктор класса принимает в качестве параметра вектор имен игроков и создает объекты самих игроков. 
¬ конструкторе создаетс€ колода карт и затем перемешиваетс€. 
“акже класс имеет один метод play(). 
¬ этом методе раздаютс€ каждому игроку по две стартовые карты, а перва€ карта дилера пр€четс€. 
ƒалее выводитс€ на экран информаци€ о картах каждого игра, в т.ч. и дл€ дилера. 
«атем раздаютс€ игрокам дополнительные карты. ѕотом показываетс€ перва€ карта дилера и дилер набирает карты, если ему надо. 
ѕосле этого выводитс€ сообщение, кто победил, а кто проиграл. ¬ конце руки всех игроков очищаютс€.
*/

class Game
{
private:
    Deck gameDeck;
    House dealerHand;
    vector<Player*> players;
public:
    Game(vector<string> playerNames)
    {
        players.clear();
        players.resize(playerNames.size());
        vector<string>::const_iterator namesIterator = playerNames.begin();
        while (namesIterator != playerNames.end())
        {
            players.push_back(new Player(*namesIterator));
        }
        gameDeck.Shuffle();
    }

    void Play()
    {

    }

}; 
