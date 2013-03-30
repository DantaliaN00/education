# search.py
# ---------
# Licensing Information: Please do not distribute or publish solutions to this
# project. You are free to use and extend these projects for educational
# purposes. The Pacman AI projects were developed at UC Berkeley, primarily by
# John DeNero (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# For more info, see http://inst.eecs.berkeley.edu/~cs188/sp09/pacman.html

"""
In search.py, you will implement generic search algorithms which are called 
by Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
  """
  This class outlines the structure of a search problem, but doesn't implement
  any of the methods (in object-oriented terminology: an abstract class).
  
  You do not need to change anything in this class, ever.
  """
  
  def getStartState(self):
     """
     Returns the start state for the search problem 
     """
     util.raiseNotDefined()
    
  def isGoalState(self, state):
     """
       state: Search state
    
     Returns True if and only if the state is a valid goal state
     """
     util.raiseNotDefined()

  def getSuccessors(self, state):
     """
       state: Search state
     
     For a given state, this should return a list of triples, 
     (successor, action, stepCost), where 'successor' is a 
     successor to the current state, 'action' is the action
     required to get there, and 'stepCost' is the incremental 
     cost of expanding to that successor
     """
     util.raiseNotDefined()

  def getCostOfActions(self, actions):
     """
      actions: A list of actions to take
 
     This method returns the total cost of a particular sequence of actions.  The sequence must
     be composed of legal moves
     """
     util.raiseNotDefined()
           

def tinyMazeSearch(problem):
  """
  Returns a sequence of moves that solves tinyMaze.  For any other
  maze, the sequence of moves will be incorrect, so only use this for tinyMaze
  """
  from game import Directions
  s = Directions.SOUTH
  w = Directions.WEST
  return  [s,s,w,s,w,w,s,w]

def depthFirstSearch(problem):
  """
  Search the deepest nodes in the search tree first [p 85].
  
  Your search algorithm needs to return a list of actions that reaches
  the goal.  Make sure to implement a graph search algorithm [Fig. 3.7].
  
  To get started, you might want to try some of these simple commands to
  understand the search problem that is being passed in:
  
  print "Start:", problem.getStartState()
  print "Is the start a goal?", problem.isGoalState(problem.getStartState())
  print "Start's successors:", problem.getSuccessors(problem.getStartState())
  """

  # Call recursive helper with problem, start state tuple, and empty explored list.
  # Return the returned list minus the first value. It is a nonsense value to create
  # the first tuple.
  ret = helperDFS(problem, (problem.getStartState(),0), [])[1:]
  return ret

def helperDFS(problem, state, explored):
  """
  DFS recursive helper function.
  """

  # Check if state has been explored.
  if (state[0] in explored):
    return 0
  # Check if state is goal.
  if (problem.isGoalState(state[0])):
    # If so, return a new list with the direction to get to it.
    return [state[1]]

  # Add current state to the explored list.
  explored.append(state[0])

  # Get successprs.
  successors = problem.getSuccessors(state[0])
  # Loop throguh successors.
  for newState in reversed(successors):
    # Recursively check successor.
    ret = helperDFS(problem, newState, explored)
    # If goal has been found...
    if (ret is not 0):
      # Prepend direction to get to current state and return.
      ret.insert(0, state[1])
      return ret

  # No goal found, return 0.
  return 0

def breadthFirstSearch(problem):
  "Search the shallowest nodes in the search tree first. [p 81]"

  # Get start state and check if it's a goal.
  startState = problem.getStartState()
  if problem.isGoalState(startState):
    return []

  # Initialize frontier queue and push start state on.
  frontier = util.Queue()
  frontier.push((startState, []))

  # Initialize empty explored state list.
  explored = [startState]

  # While frontier queue is not empty...
  while (not frontier.isEmpty()):
    # Pop off frontier.
    currState, actionList = frontier.pop()

    # Loop through successors...
    for newState, newAction, newCost in problem.getSuccessors(currState):
      # Check if the node is a goal...
      if problem.isGoalState(newState):
        # Return the list.
        actionList.append(newAction)
        return actionList

      # Copy temporary list.
      tempList = actionList[:]
      # If state hasn't been explored...
      if (newState not in explored):
        # Add state to explored list.
        explored.append(newState)
        # Add move to associated list.
        tempList.append(newAction)
        # Push onto frontier.
        frontier.push((newState, tempList))
      
def uniformCostSearch(problem):
  "Search the node of least total cost first. "

  # Get start state and check if it's a goal.
  startState = problem.getStartState()
  if problem.isGoalState(startState):
    return []

  # Initialize frontier priority queue and push start state on.
  frontier = util.PriorityQueue()
  frontier.push((startState, [], 0), 0)

  # Initialize empty explored state list.
  explored = [startState]

  # While frontier queue is not empty...
  while (not frontier.isEmpty()):
    # Pop off frontier based on lowest cost.
    currState, actionList, totalCost = frontier.pop()

    # Loop through successors...
    for newState, newAction, newCost in problem.getSuccessors(currState):
      # Check if the node is a goal...
      if problem.isGoalState(newState):
        # Return the list.
        actionList.append(newAction)
        return actionList

      # Copy temporary list.
      tempList = actionList[:]
      # If state hasn't been explored...
      if (newState not in explored):
        # Add state to explored list.
        explored.append(newState)
        # Add move to associated list.
        tempList.append(newAction)
        # Push onto frontier.
        frontier.push((newState, tempList, totalCost + newCost), totalCost + newCost)

def nullHeuristic(state, problem=None):
  """
  A heuristic function estimates the cost from the current state to the nearest
  goal in the provided SearchProblem.  This heuristic is trivial.
  """
  return 0

def aStarSearch(problem, heuristic=nullHeuristic):
  "Search the node that has the lowest combined cost and heuristic first."

  # Get start state and check if it's a goal.
  startState = problem.getStartState()
  if problem.isGoalState(startState):
    return []

  # Initialize frontier priority queue and push start state on.
  frontier = util.PriorityQueueWithFunction(lambda (state, actionlist, cost): heuristic(state, problem) + cost)
  frontier.push((startState, [], 0))

  # Initialize empty explored state list.
  explored = [startState]

  # While frontier queue is not empty...
  while (not frontier.isEmpty()):
    # Pop off frontier based on lowest cost.
    currState, actionList, totalCost = frontier.pop()

    # Loop through successors...
    for newState, newAction, newCost in problem.getSuccessors(currState):
      # Check if the node is a goal...
      if problem.isGoalState(newState):
        # Return the list.
        actionList.append(newAction)
        return actionList

      # Copy temporary list.
      tempList = actionList[:]
      # If state hasn't been explored...
      if (newState not in explored):
        # Add state to explored list.
        explored.append(newState)
        # Add move to associated list.
        tempList.append(newAction)
        # Push onto frontier.
        frontier.push((newState, tempList, totalCost + newCost))
    
  
# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch