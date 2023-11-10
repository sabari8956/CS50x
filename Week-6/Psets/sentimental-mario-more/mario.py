# TODO
import sys

def main():
    height = get_height()
    n = height - 1
    h = 1
    while (n  > -1 ):
        print(' ' * n, end="")
        print('#' * h, end="")


        print("  ", end="")

        print('#' * h)

        n -= 1
        h += 1
def get_height():
    while True:
        try:
            h = int(input("Height: "))

        except:
            pass

        else:
            if (h > 0 and h<9):
                return h


main()