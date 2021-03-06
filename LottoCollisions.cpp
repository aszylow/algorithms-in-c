/*  High quality collision tester for lottery draws.

    Will output number of draws taken to reach a collision.
    Might take some time.

    This program selects shuffled numbers from highly random positions
    to simulate drawn objects from a mechanical device.

    Testing will demonstrate that 1,2,3,4,5,6 or any other pattern-based
    draw is as likely to occur as any other combination of numbers.

    In a lottery, distinct, abstract objects are chosen, not numbers.
    A lottery number is an object's identification. Therefore, examining
    order, intervals, primality, evenness, oddness, or any other numeric
    patterns and properties is a waste of time.

    Default 6-49 draw, but can be modified to many other lotteries.
*/

#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>

#define DRAWSIZE 49
#define DRAWS 1
#define DRAWNUMS 6
#define SHUFFLES 1

using namespace std;

int getRandomIndex(int endIndex) {

    mt19937 mt(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, endIndex);

    return dist(mt);
}

void shuffleVector(vector<int> &drawNums) {

    for(int i = 1; i <= SHUFFLES; i++) {
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
        shuffle(drawNums.begin(), drawNums.end(), default_random_engine(seed));
    }
}

int testCollision(int *drawTest, int *drawResult) {

    int match = 0;

    for(int i = 0; i < DRAWNUMS; i++) {
        if(drawResult[i] == drawTest[i])
            match++;
    }

    if(match == DRAWNUMS)
        return 1;
    else
        return 0;
}

int main() {

    int drawTest[] {1, 2, 3, 4, 5, 6};
    unsigned collisionTries = 0;
    int collisionFound = 0;

    while(collisionFound != 1) {

        for(int i = 1; i <= DRAWS; i++) {

            int endIndex = DRAWSIZE - 1;
            vector<int> drawNums;
            int drawResult[DRAWNUMS];

            for(int i = 1; i <= DRAWSIZE; i++)
                drawNums.push_back(i);

            for(int i = 1; i <= DRAWNUMS; i++) {

                shuffleVector(drawNums);

                int randIndex = getRandomIndex(endIndex);
                int valAtIndex = drawNums.at(randIndex);
                drawResult[i - 1] = valAtIndex;
                drawNums.erase(drawNums.begin() + randIndex);
                endIndex--;
            }

            sort(drawResult, drawResult + DRAWNUMS);
            collisionFound = testCollision(drawTest, drawResult);

            if(collisionFound == 1) {
                for (int i = 0; i < DRAWNUMS; i++)
                    cout << drawResult[i] << " ";
                cout << "at " << collisionTries << " draws." << endl;
            }

            collisionTries++;
        }
    }

    return 0;
}
