2048 PROGRAM using C
===

TERM
---
2017.06.06 ~ 2017.06.14


FLOW CHART
---
![image](https://user-images.githubusercontent.com/30820487/50943356-6feb7b00-14cf-11e9-8e8a-d38438d32b29.png)


FUNCTION: Role
---
++*init()++*:

set values for starting game.

*rotate(int board[4][4])*:

rotate board 90 degrees to the right.

*set_board(int dir, int board[4][4])*:

move blocks to the entered direction.

And, return 1 if it is possible to move at least one block as direction.

*is_game_ovver()*:

return 0 if it is impossible to move any block to any direction.

*draw_board(int total_sum, int command)*:

print board to screen.

store board, grade in output.txt.

confirm that sum of numbers in board is same as input total_sum.

*make_two_or_four()*:

generate 2 or 4 number in the blank of the board randomly.

*getch()*:

return key-board that user enterd.

*GetCommand()*:

return 0 if entered key is 'D' or 'd'.

return 1 if entered key is 'W' or 'w'.

return 2 if entered key is 'A' or 'a'.

return 3 if entered key is 'S' or 's'.


FUNCTION: Description
---
