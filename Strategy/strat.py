import time
import math
RUN_TIME = 300
SIMON_TIME = 15
ETCH_TIME = 5
RUBIK_TIME = 5
CARD_TIME = 10
FOLLOW_TIME = 10


def initializePics():
    print("Initializing PIC communication")
    return


def waitForStart():
    print("Waiting for start signal")
    print("Starting")
    return


def followToGame():
    startFollowTime = time.time()
    print("Following to game")
    time.sleep(FOLLOW_TIME)
    success = True
    return time.time() - startFollowTime


def playSimon():
    startSimonTime = time.time()
    print("Playing Simon")
    time.sleep(SIMON_TIME)
    success = True
    return success, time.time() - startSimonTime


def playEtchASketch():
    startEtchTime = time.time()
    print("Playing Etch-a-Sketch")
    time.sleep(ETCH_TIME)
    success = True
    return success, time.time() - startEtchTime


def playRubik():
    startRubikTime = time.time()
    print("Playing Rubik's Cube")
    time.sleep(RUBIK_TIME)
    success = True
    return success, time.time() - startRubikTime


def playCards():
    startCardsTime = time.time()
    print("Playing Cards")
    time.sleep(CARD_TIME)
    success = True
    return success, time.time() - startCardsTime


def goToFinish():
    startTime = time.time()
    print("Bringing it home")
    time.sleep(FOLLOW_TIME)
    return time.time() - startTime


if __name__ == "__main__":
    elapsedTime = 0
    totalFollowTime = 0

    # Initialize pics
    initializePics()

    # Wait for start
    waitForStart()

    startTime = time.time()

    # Follow line to game
    totalFollowTime += followToGame()

    # Play Simon
    simonSuccess, simonTime = playSimon()

    # Follow line to game
    totalFollowTime += followToGame()

    # Play Etch-a-Sketch
    etchSuccess, etchTime = playEtchASketch()

    # Follow line to game
    totalFollowTime += followToGame()

    # Play Rubik's Cube
    rubikSuccess, rubikTime = playRubik()

    # Follow line to game
    totalFollowTime += followToGame()

    # Play Cards
    cardSuccess, cardTime = playCards()

    # Follow line to finish
    goToFinishTime = goToFinish()

    elapsedTime = totalFollowTime + goToFinishTime + simonTime + etchTime + rubikTime + cardTime

    totalPoints = RUN_TIME - elapsedTime

    if(simonSuccess):
        totalPoints += 15
    if(etchSuccess):
        totalPoints += 20
    if(rubikSuccess):
        totalPoints += 5
    if(cardSuccess):
        totalPoints += 5

    print("Total run time: " + str(elapsedTime))
    print("Total points: " + str(math.trunc(totalPoints)))
