import numpy as np
import matplotlib.pyplot as plt
import seaborn as sb
from tqdm import tqdm

sb.set()
sb.set_style("whitegrid")
plt.rcParams['figure.figsize'] = [4, 4]
plt.rcParams['figure.dpi'] = 144
plt.rcParams['font.family'] = "Arial Unicode MS"
plt.rcParams['axes.unicode_minus'] = False

DNA_SIZE = 10  # DNA长度
POP_SIZE = 20  # 种群数量
CROSS_RATE = 0.8
MUTATION_RATE = 0.003
N_GENERATIONS = 50
X_BOUND = [0, 5]


def F(x):
    return np.sin(10*x)*x + np.cos(2*x)*x
    # return np.sin(0.75*x**3)*x + np.cos(2*x) * x


def get_fitness(pred):  # 获得个体适应程度
    return pred + 1e-3 - np.min(pred)


def translateDNA(pop):  # DNA解码为程序的数据
    return pop.dot(2 ** np.arange(DNA_SIZE)
                   [::-1]) / float(2**DNA_SIZE-1) * X_BOUND[1]


def select(pop, fitness):  # 根据
    idx = np.random.choice(np.arange(POP_SIZE), size=POP_SIZE,
                           replace=True, p=fitness/fitness.sum())
    return pop[idx]


def crossover(parent, pop):     # mating process (genes crossover)
    if np.random.rand() < CROSS_RATE:
        # select another individual from pop
        i_ = np.random.randint(0, POP_SIZE, size=1)
        cross_points = np.random.randint(0, 2, size=DNA_SIZE).astype(np.bool)
        parent[cross_points] = pop[i_, cross_points]
    return parent


def mutate(child):
    for point in range(DNA_SIZE):
        if np.random.rand() < MUTATION_RATE:
            child[point] = 1 if child[point] == 0 else 0
    return child


if __name__ == "__main__":
    # initialize the pop DNA
    pop = np.random.randint(2, size=(POP_SIZE, DNA_SIZE))

    plt.ion()
    x = np.linspace(*X_BOUND, 200)
    plt.plot(x, F(x))

    for _ in tqdm(range(N_GENERATIONS)):
        # compute function value by extracting DNA
        F_values = F(translateDNA(pop))

        if 'sca' in globals():
            sca.remove()
        sca = plt.scatter(translateDNA(pop), F_values,
                          s=50, lw=0, c='green', alpha=0.8)
        plt.pause(0.1)

    # GA part (evolution)
        fitness = get_fitness(F_values)
        # print("Most fitted DNA: ", pop[np.argmax(fitness), :])
        pop = select(pop, fitness)
        pop_copy = pop.copy()
        for parent in pop:
            child = crossover(parent, pop_copy)
            child = mutate(child)
            parent[:] = child       # parent is replaced by its child

    plt.ioff()
    plt.show()
