# 8 Puzzle using BFS

def show(state):
    print(state[0], state[1], state[2])
    print(state[3], state[4], state[5])
    print(state[6], state[7], state[8])
    print()


start = list(map(int, input("Enter initial state (9 numbers, 0 = blank): ").split()))
goal = list(map(int, input("Enter goal state (9 numbers, 0 = blank): ").split()))

# Each move: (change in row, change in column, name)
# The name says which way the blank (0) moves
directions = [
    (-1, 0, "UP"),
    (1, 0, "DOWN"),
    (0, -1, "LEFT"),
    (0, 1, "RIGHT")
]

# Queue stores (state, path of states, list of moves)
queue = [(start, [start], [])]

# visited is a set because checking a set is much faster than a list
# Lists can't be stored in a set, so each state is stored as a tuple
visited = set()
visited.add(tuple(start))

found = False

while len(queue) > 0:
    state, path, moves = queue.pop(0)

    if state == goal:
        found = True
        break

    # Find the blank and its row and column
    zero = state.index(0)
    row = zero // 3
    col = zero % 3

    for dr, dc, move in directions:
        new_row = row + dr
        new_col = col + dc

        # Check that the move stays inside the board
        if 0 <= new_row < 3 and 0 <= new_col < 3:
            new_zero = new_row * 3 + new_col

            # Swap the blank with its neighbour
            new_state = state.copy()
            new_state[zero] = new_state[new_zero]
            new_state[new_zero] = 0

            if tuple(new_state) not in visited:
                visited.add(tuple(new_state))
                queue.append((new_state, path + [new_state], moves + [move]))

if found:
    print("Goal reached in", len(moves), "moves\n")

    print("Initial state:")
    show(path[0])

    for i in range(len(moves)):
        print("Move", i + 1, ": blank moves", moves[i])
        show(path[i + 1])
else:
    print("Goal cannot be reached!")
