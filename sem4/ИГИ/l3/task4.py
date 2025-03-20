

TEXT = "So she was considering in her own mind, as well as she could, for the hot day made her feel very sleepy and stupid, whether the pleasure of making a daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly a White Rabbit with pink eyes ran close by her."


def count_words(s : str):
    """Count words in @s"""
    return len(s.split(' '))

def even_words(s : str):
    """Return list of words with even length"""
    return [w for w in s.split(' ') if len(w)%2 == 0]

def shortest_starts_with_a(s : str):
    """Return the shortest word in @s that starts with `a`"""
    return  [w for w in sorted(s.split(' '), key = lambda k : len(k)) if w[0] == 'a'][0]

def repeated_words(s : str):
    """Return list of repeated words in @s"""
    count = {}
    for w in s.split(' '):
        if w in count:
            count[w] += 1
        else:
            count[w] = 1
    return  [x[0] for x in count.items() if x[1] > 1]


def run():
    """Run task 4"""
    print("TEXT:\n```")
    print(TEXT)
    print("```\n")


    print("Number of words : ", count_words(TEXT))
    print("List of words with even length : \n", even_words(TEXT))
    print("Shortest word that starts with `a` : ", shortest_starts_with_a(TEXT.lower()))
    print("Repeated words : ", repeated_words(TEXT.lower()))


