import sys


class ValidationError(Exception):
    """
    Exception class for when the validation fails.
    """
    pass


def ordinal(n):
    """
    Convert an integer into its ordinal representation.

    Parameters
    ----------
    n : int
      A natural number.

    Returns
    ----------
    res : str
      An ordinal representation of n.
    """
    if 11 <= (n % 100) <= 13:
        suffix = 'th'
    else:
        suffix = ['th', 'st', 'nd', 'rd', 'th'][min(n % 10, 4)]
    res = str(n) + suffix
    return res


def read_file(file_path):
    """
    Read a file line-by-line.

    Parameters
    ----------
    file_path : str
      A path of the target file.

    Returns
    ----------
    lines : list[str]
      A list that contains each line of the file.
    """
    with open(file_path) as f:
        lines = [line.strip() for line in f.readlines()]
        return lines


def read_DIMACS(file_path):
    """
    Read a file that describes an input graph structure in DIMACS format.

    Parameters
    ----------
    file_path : str
      A path of the target file.

    Returns
    ----------
    vnum : int
      A number of nodes in an input graph.
    edges : list
      A list that contains each edge of an input graph.
    """
    lines = read_file(file_path)
    vnum = 0
    edges = []
    for l in lines:
        values = l.split()
        if values[0] == 'p':
            vnum = int(values[1])
        elif values[0] == 'e':
            v1 = int(values[1])
            v2 = int(values[2])
            edges.append((v1, v2))
    return vnum, edges


def read_st(file_path):
    """
    Read a file that describes start/target states.

    Parameters
    ----------
    file_path : str
      A path of the target file.

    Returns
    ----------
    s : list[int]
      A list that contains each node of the start state.
    t : list[int]
      A list that contains each node of the target state.
    """
    lines = read_file(file_path)
    s = []
    t = []
    for l in lines:
        values = l.split()
        if values[0] == 's':
            values.pop(0)
            s = list(map(int, values))
            s.sort()
        elif values[0] == 't':
            values.pop(0)
            t = list(map(int, values))
            t.sort()
    return s, t


def read_answer(file_path):
    """
    Read a file that describes an answer for the ISR problem.

    Parameters
    ----------
    file_path : str
      A path of the target file.

    Returns
    ----------
    bool
      True if some reconfiguration sequence exists, False otherwise.
    answers : list
      A list that contains each state in a reconfiguration sequence.
    """
    lines = read_file(file_path)
    answers = []
    for l in lines:
        values = l.split()
        if values[0] == 'a':
            if values[1] == 'YES':
                continue
            elif values[1] == 'NO':
                return False, answers
            else:
                values.pop(0)
                a = list(map(int, values))
                a.sort()
                answers.append(a)
    return True, answers


def is_independent_set(vnum, edges, state):
    """
    Read a file in DIMACS format.

    Parameters
    ----------
    vnum : int
      A number of nodes.
    edges : list
      A list of edges.
    state : list
      A list of a subset of nodes.

    Returns
    ----------
    bool
      True if ``state`` is an independent set, False otherwise.
    """
    X = [False] * (vnum + 1)
    for s in state:
        X[s] = True
    for e in edges:
        (v1, v2) = e
        if X[v1] and X[v2]:
            return False
    return True


def main():
    args = sys.argv
    vnum, edges = read_DIMACS(args[1])
    s, t = read_st(args[2])
    is_exist, answers = read_answer(args[3])

    # If there is no reconfiguration sequence, then the validation succeeds.
    if not is_exist:
        print('Validation success')
        return

    # Check that the initial state match the start state
    if s != answers[0]:
        raise ValidationError(
            'The initial state must be equal to the start state')

    # Check that the last state match the target state
    if t != answers[-1]:
        raise ValidationError(
            'The last state must be equal to the target state')

    # Check that each state is an independent set.
    for i, a in enumerate(answers):
        if not is_independent_set(vnum, edges, a):
            raise ValidationError('The ' + ordinal(i + 1) +
                                  ' state is not an independent set')

    # Check that the difference between successive states is one
    for i in range(len(answers) - 1):
        diff = set(answers[i]) - set(answers[i + 1])
        if len(diff) != 1:
            raise ValidationError(
                'Each independent set in the sequence results from '
                'the previous one by moving exactly one token to another node')

    # If it passes all the checks, then the validation succeeds.
    print('Validation success')


if __name__ == '__main__':
    main()
