# multiAgents.py
# --------------
# Licensing Information: Please do not distribute or publish solutions to this
# project. You are free to use and extend these projects for educational
# purposes. The Pacman AI projects were developed at UC Berkeley, primarily by
# John DeNero (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and Pieter 
# Abbeel in Spring 2013.
# For more info, see http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html

from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]
        
        "*** YOUR CODE HERE ***"
        # return successorGameState.getScore()
        
        newFoodDistances = []
        x = 0
        for row in newFood:
            x += 1
            y = 0
            for column in row:
                if column == True:
                    distanceToFood = manhattanDistance(newPos, [x,y])
                    if distanceToFood > 0:
                        newFoodDistances.append(1.0/distanceToFood)
                    else:
                        newFoodDistances.append(0)
                y += 1
                
        if len(newFoodDistances) == 0:
            newFoodDistances.append(1)
        
        actionModifier = 0
        if action == Directions.STOP:
            actionModifier -= 1
        else:
            actionModifier += 1
                
        return successorGameState.getScore() + max(newFoodDistances) + actionModifier


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        return self.minimaxValue(gameState, 0, 0)[1]
        
    def minimaxValue(self, gameState, depth, agentIndex):
        # First check if we are moving onto the next level, i.e. back to Pacman's turn
        if agentIndex >= gameState.getNumAgents():
            depth += 1
            agentIndex = 0
            
        actions = gameState.getLegalActions(agentIndex)
        if Directions.STOP in actions:
            actions.remove(Directions.STOP)
        
        if depth >= self.depth or len(actions) == 0:
            return (self.evaluationFunction(gameState), None)
        
        random.shuffle(actions)
        minimaxValues = []
        for action in actions:
            minimaxValues.append(self.minimaxValue(gameState.generateSuccessor(agentIndex, action), depth, agentIndex+1)[0])
        
        if agentIndex == 0:
            selectedValue = max(minimaxValues)
        else:
            selectedValue = min(minimaxValues)

        for index in range(len(minimaxValues)):
            if selectedValue == minimaxValues[index]:
                return (selectedValue, actions[index])

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        """
        return self.alphabeta(gameState, 0, 0, -99999999, 999999999)[1]
            
        def alphabeta(self, gameState, depth, agentIndex, alpha, beta):
            # First check if we are moving onto the next level, i.e. back to Pacman's turn
            if agentIndex >= gameState.getNumAgents():
                depth += 1
                agentIndex = 0

            actions = gameState.getLegalActions(agentIndex)

            if Directions.STOP in actions:
              actions.remove(Directions.STOP)

            if depth >= self.depth or len(actions) == 0:
              return (self.evaluationFunction(gameState), None)

            theAction = None
            if agentIndex == 0:
              for action in actions:
                value = self.alphabeta(gameState.generateSuccessor(agentIndex, action), depth, agentIndex+1, alpha, beta)[0]

                if value > alpha:
                  alpha = value
                  theAction = action
                
                if beta <= alpha:
                  break

              return (alpha, theAction)
            else:
              for action in actions:
                value = self.alphabeta(gameState.generateSuccessor(agentIndex, action), depth, agentIndex+1, alpha, beta)[0]

                if value < beta:
                  beta = value
                  theAction = action

                if beta <= alpha:
                  break

              return (beta, theAction)
        """
            
        alpha = float('-infinity')
        beta = float('infinity')
        score = float('-infinity')
        actions = gameState.getLegalActions(0)
        if Directions.STOP in actions:
            actions.remove(Directions.STOP)
        bestAction = actions[0]

        for action in actions:
          successor = gameState.generateSuccessor(0, action)
          res = self.minAgent(0, successor, 1, alpha, beta)
          
          if(res > score):
            score = res
            bestAction = action
          
          if(res > beta):
            return bestAction
          
          alpha = max(alpha, res)

        return bestAction  
        
    def minAgent(self, depth, gameState, ghostIndex, alpha, beta):
    
        if(self.depth == depth or gameState.isLose() or gameState.isWin()):
            return self.evaluationFunction(gameState)

        actions = gameState.getLegalActions(ghostIndex)
        score = float('infinity')

        for action in actions:
          successor = gameState.generateSuccessor(ghostIndex, action)
          
          if ghostIndex < gameState.getNumAgents() - 1:
            score = min(score, self.minAgent(depth, successor, ghostIndex + 1, alpha, beta))
          
          elif ghostIndex == gameState.getNumAgents() - 1:
            score = min(score, self.maxAgent(depth + 1, successor, alpha, beta))
          
          if(score < alpha):
            return score
          
          beta = min(score, beta)

        return score

    def maxAgent(self, depth, gameState, alpha, beta):

        if (self.depth == depth or gameState.isLose() or gameState.isWin()):
          return self.evaluationFunction(gameState)

        actions = gameState.getLegalActions(0)
        if Directions.STOP in actions:
            actions.remove(Directions.STOP)
        score = float('-infinity')

        for action in actions:
          successor = gameState.generateSuccessor(0, action)
          score = max(score, self.minAgent(depth, successor, 1, alpha, beta))
          
          if(score > beta):
            return score
          
          alpha = max(alpha, score)

        return score

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        return self.expectimaxValue(gameState, 0, 0)[1]
        
    def expectimaxValue(self, gameState, depth, agentIndex):
        # First check if we are moving onto the next level, i.e. back to Pacman's turn
        if agentIndex >= gameState.getNumAgents():
            depth += 1
            agentIndex = 0
            
        actions = gameState.getLegalActions(agentIndex)
        if Directions.STOP in actions:
            actions.remove(Directions.STOP)
        
        if depth >= self.depth or len(actions) == 0:
            return (self.evaluationFunction(gameState), None)
        
        random.shuffle(actions)
        minimaxValues = []
        for action in actions:
            minimaxValues.append(self.expectimaxValue(gameState.generateSuccessor(agentIndex, action), depth, agentIndex+1)[0])
        
        if agentIndex == 0:
            selectedValue = max(minimaxValues)
        else:
            selectedValue = sum(minimaxValues) / len(minimaxValues)

        for index in range(len(minimaxValues)):
            if selectedValue == minimaxValues[index]:
                return (selectedValue, actions[index])

        return (selectedValue, None)

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    #util.raiseNotDefined()
    #return currentGameState.getScore()
    
    newPos = currentGameState.getPacmanPosition()
    newFood = currentGameState.getFood()
    foodList = newFood.asList()

    score = 0

    ghostStates = currentGameState.getGhostStates()
    ghostPositions = currentGameState.getGhostPositions()
    distTable1 = dict()

    scaredTimes = [ghostState.scaredTimer for ghostState in ghostStates]
    '''
    scaredTimes = []
    minTime = 0
    for ghost in ghostStates:
        if ghost.scaredTimer > minTime:
            minTime = ghost.scaredTimer
            minGhost = ghost

        scaredTimes.append(ghost.scaredTimer)

    '''
    ghostDist = []
    for ghostPos in ghostPositions:
      temp = manhattanDistance(ghostPos, newPos)
      ghostDist.append(temp)
    '''
    if (position,food) not in distTable.keys():
        dist = mazeDistance(food,position,problem.startingGameState)
        distTable[(position,food)] = dist
    foodmap.append(distTable[(position,food)])
    '''

    foodDist = []
    for food in foodList: # for every pebble closeby add 1
      foodDist.append(manhattanDistance(food, newPos))

    if len(foodDist) == 0: 
      closestFood = 1.0
      amountFood = 1.0
      score = 1.0/closestFood + 1.0/amountFood
      closestGhost = min(ghostDist)

    else:
      amountFood = len(foodList)
      closestFood = min(foodDist)
      closestGhost = min(ghostDist)

    #score = 10.0/closestFood - 10.0*amountFood + (min(scaredTimes) - 20)*closestGhost 
    
    if min(scaredTimes) < 7: # SCARED TIMER < 6
        if min(scaredTimes) == 0 and closestGhost <= 3:
            return -100000

        score = 10.0/closestFood - 10.0*amountFood - closestGhost/3.0 + 2*currentGameState.getScore() #there is no tiebreaker other than closestGhost so it waits for it to come back
        # TO FOOD, FROM GHOST
        # TO FOOD, TO GHOST
        # FROM FOOD, TO GHOST

    elif min(scaredTimes) >= 7 and min(scaredTimes) < 28: #SCARED TIMER > 6 until 39
        score = closestGhost + 10.0/closestFood - 10.0*amountFood + 3*currentGameState.getScore()
        # TO FOOD, TO GHOST
        # TO FOOD, FROM GHOST
        # FROM FOOD, FROM GHOST

    elif min(scaredTimes) >= 28:
        score = 5*closestGhost + 10.0/closestFood - 10.0*amountFood + 4*currentGameState.getScore()#+ min(scaredTimes) 
        # TO FOOD, TO GHOST

    #print score
    return score

# Abbreviation
better = betterEvaluationFunction
    
# Added previous search and searchAgents for access to A* and AnyFoodSearchProblem.
import search, searchAgents

class ContestAgent(MultiAgentSearchAgent):
    """
      Your agent for the mini-contest
    """
    
    def getAction(self, gameState):
        """
          Returns an action.  You can use any method you want and search to any depth you want.
          Just remember that the mini-contest is timed, so you have to trade off speed and computation.

          Ghosts don't behave randomly anymore, but they aren't perfect either -- they'll usually
          just make a beeline straight towards Pacman (or away from him if they're scared!)
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        
        agent = AlphaBetaAgent()
        agent.evaluationFunction = self.contestEvaluationFunction

        # Call alpha-beta agent for action.
        return agent.getAction(gameState)
        
    def contestEvaluationFunction(self, currentGameState):
      #
      #  Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      #  evaluation function (question 5).
      #
      #  DESCRIPTION:
      #
      #  This is very similar to the betterEvaluationFunction. For the most part, the function
      #  works about the same, except that weights were adjusted to better reflect actual score
      #  gain possible from a state, but there are also a few other key differences.
      #
      #  First, ghost positions while scared are true distances (calculated with an A* search).
      #  This gives a more accurate representation of how far Pacman would need to move to eat
      #  the scared ghost. The weight for eating a scared ghost was changed from just the
      #  scaredTimer to the scaredTimer multiplied by 8. This was an effort to mirror the actual
      #  score that might be gained by chasing and eating the ghost.
      #
      #  Second, a section was added for when the ghosts aren't scared. This will keep Pacman a
      #  little further away from ghosts and out of hairy situations. This is done by checking
      #  the manhattan distance (this is done for speed, since non-scared states are more common
      #  than scared states) and if he was too close to that ghost, the ghost total would be
      #  lowered by 500 to reflect the risk of being so close to a non-scared ghost.
      #
      
      # Create a food search problem to find nearest food.
      problem = searchAgents.AnyFoodSearchProblem(currentGameState)
      # A* search for nearest food.
      searched = search.aStarSearch(problem)
      # Get the real distance to the nearest food.
      dist = len(searched) if searched != None else 1
      # Get the current number of pellets left.
      fc = currentGameState.getFood().count() if currentGameState.getFood().count() > 0 else 1

      # Ghost score total.
      gt = 0
      # Current position of pacman.
      pos = currentGameState.getPacmanPosition()
      # For each ghost...
      for ghost in currentGameState.getGhostStates():
        # If the ghost is scared...
        if ghost.scaredTimer:
          # Get the ghost's position.
          gpos = ghost.getPosition()
          # Create ghost serach problem.
          gp = searchAgents.PositionSearchProblem(currentGameState, lambda x: 1, (int(gpos[0]), int(gpos[1])), pos, False)
          # A* search for current ghost.
          gs = search.aStarSearch(gp, searchAgents.manhattanHeuristic)
          # Get real distance of ghost.
          gd = len(gs) if gs != None else 1
          # At max ghost.scaredTimer * 8. is 240. Actual gain from eating scared ghost is 250.
          # This is decayed by the real distance of to the ghost.
          gt += (ghost.scaredTimer * 8.)/gd
        # If ghost is not scared...
        else:
          # Get manhattan distance of ghost.
          gd = util.manhattanDistance(ghost.getPosition(), pos)
          # If the distance is less than one, reduce ghost total by 500.
          # Basically pacman could die here, so lower the possible score here.
          if gd <= 1:
            gt -= 500

      # Return the current score
      # plus the inverse of the real distance to the nearest food (lower distance for higher score.)
      # plus the inverse of the number of pellets left (lower number of pellets left for higher score.)
      # plus the ghost total.
      return currentGameState.getScore() + 1./dist + 500./fc + gt
