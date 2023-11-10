# TODO


def main():
    # gets card number and strips the spaces in both sides
    cardNumber = input("Card Number: ").strip()

    # checks if its a number
    try:
        int(cardNumber)
    except:
        return

    # algorthm
    # card Validater algorithm
    if (lunaAlgorithm(cardNumber)):
        cardChecker(cardNumber)

    else:
        print("INVALID")
        return


def lunaAlgorithm(cardNumber):
    value = 0
    boo = False
    #  goes from last to first
    for i in cardNumber[::-1]:
        if boo:
            n = int(i) * 2
            if n > 9:
                n = (n % 10) + 1
            value += n
            boo = False
        else:
            value += int(i)
            boo = True

    #  if the card number is valid
    if value % 10 == 0:
        return True
    
    # if it isnt valid
    else:
        return False


def cardChecker(cardNumber):
    len_card = len(cardNumber)

    if len_card in [13, 15, 16]:
        if len_card == 15 and cardNumber[0] == '3' and cardNumber[1] == '4' or cardNumber[1] == '7':
            print("AMEX")
        elif len_card == 13 and cardNumber[0] == '4':
            print("VISA")
        else:
            if cardNumber[0] == '4':
                print("VISA")
            elif int(cardNumber[0:2]) > 50 and int(cardNumber[0:2]) < 56:
                print("MASTERCARD")
            else:
                print("INVALID")
    else:
        print("INVALID")


main()
