N = 4
board = [-1] * N
count = 0

def print_board():
    for i in range(N):
        row = ""
        for j in range(N):
            row += " Q " if board[i] == j else " . "
        print(row)
    print()

def is_safe(row, col):
    for i in range(row):
        if board[i] == col or abs(board[i] - col) == abs(i - row):
            return False
    return True

def solve(row):
    global count

    if row == N:
        count += 1
        print(f"Solution {count} found:")
        print_board()
        return

    for col in range(N):
        print(f"Trying Queen at row {row}, column {col}")

        if is_safe(row, col):
            board[row] = col
            print("Placement successful:")
            print_board()

            solve(row + 1)

            board[row] = -1
            print(f"Backtracking from row {row}, column {col}")
            print_board()
        else:
            print("Position is unsafe!\n")

solve(0)

if count == 0:
    print("No solution exists.")
else:
    print("Total solutions:", count)
