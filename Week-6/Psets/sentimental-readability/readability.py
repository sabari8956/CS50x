# TODO


def main():


    sentence = input("enter :")

    l , w , s = count(sentence)
    l = float(l)
    s = float(s)
    w += 1
    l = (l / w )* 100
    s = (s /  w) *100



    index = round(0.0588 * l - 0.296 * s - 15.8)

    if (index < 1):
        print("Before Grade 1")
    elif (index > 16):
        print("Grade 16+")
    else:
        print(f"Grade {index}")



def count(sen):
    alphabets = words = sentences = 0
    for letter in sen:

        if (letter.isalpha()):
            alphabets +=1
        elif (letter == ' '):
            words +=1
        elif (letter in ['.', '!', '?']):
            sentences += 1

    return (alphabets, words, sentences)



main()
