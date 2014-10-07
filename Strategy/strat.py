import time
import math

RUN_TIME = 300
SIMON_TIME = 15
ETCH_TIME = 30
RUBIK_TIME = 7
CARD_TIME = 3
FOLLOW_TIME = 10

START_POINTS = 100
TO_GAME_POINTS = 10
SIMON_POINTS = 150
ETCH_POINTS = 200
RUBIKS_POINTS = 50
CARD_POINTS = 50

elapsedTime = 0
totalFollowTime = 0
totalPoints = 0


def initializePics():
    print("Initializing PIC communication")
    return


def waitForStart():
    global totalPoints

    print("Waiting for start signal")
    print("Starting")

    totalPoints += START_POINTS
    return


def followToGame():
    global totalPoints

    startFollowTime = time.time()
    print("Following to game")
    time.sleep(FOLLOW_TIME)

    success = True
    totalPoints += TO_GAME_POINTS
    return time.time() - startFollowTime


def playSimon():
    global totalPoints

    startSimonTime = time.time()
    print("Playing Simon")
    time.sleep(SIMON_TIME)

    success = True
    totalPoints += SIMON_POINTS
    return success, time.time() - startSimonTime


def playEtchASketch():
    global totalPoints

    startEtchTime = time.time()
    print("Playing Etch-a-Sketch")
    time.sleep(ETCH_TIME)

    success = True
    totalPoints += ETCH_POINTS
    return success, time.time() - startEtchTime


def playRubik():
    global totalPoints

    startRubikTime = time.time()
    print("Playing Rubik's Cube")
    time.sleep(RUBIK_TIME)

    success = True
    totalPoints += RUBIKS_POINTS
    return success, time.time() - startRubikTime


def playCards():
    global totalPoints

    startCardsTime = time.time()
    print("Playing Cards")
    time.sleep(CARD_TIME)

    totalPoints += CARD_POINTS
    success = True
    return success, time.time() - startCardsTime


def goToFinish():
    global totalPoints

    startTime = time.time()
    print("Bringing it home")

    time.sleep(FOLLOW_TIME)
    return time.time() - startTime


if __name__ == "__main__":
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

    # Time points
    elapsedTime = totalFollowTime + goToFinishTime + simonTime + etchTime + rubikTime + cardTime
    totalPoints += RUN_TIME - elapsedTime

    print("Total run time: " + str(elapsedTime))
    print("Total points: " + str(math.trunc(totalPoints)))
