class WrongNumberOfPlayersError < StandardError ; end
class NoSuchStrategyError < StandardError ; end

def rps_result(m1, m2)
  raise NoSuchStrategyError if m1[1].match(/R|P|S/).nil? or m2[1].match(/R|P|S/).nil?
  
  if m1[1] == "R" and m2[1] == "S" then return m1 end
  if m1[1] == "R" and m2[1] == "P" then return m2 end
  
  if m1[1] == "S" and m2[1] == "P" then return m1 end
  if m1[1] == "S" and m2[1] == "R" then return m2 end
  
  if m1[1] == "P" and m2[1] == "R" then return m1 end
  if m1[1] == "P" and m2[1] == "S" then return m2 end
  
  return m1
end

def rps_game_winner(game)
  raise WrongNumberOfPlayersError unless game.length == 2
  rps_result game[0], game[1]
end

def rps_tournament_winner(tournament)
  if tournament[0][0].kind_of?(Array)
	rps_game_winner [rps_tournament_winner(tournament[0]), rps_tournament_winner(tournament[1])]
  else 
    rps_game_winner [tournament[0], tournament[1]]
  end
end