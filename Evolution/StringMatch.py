import numpy as np
from tqdm import tqdm

TARGET_STRING = "War, war never changes."
POP_SIZE = 1000
CROSS_RATE = 0.2
MUTATION_RATE = 0.01
N_GENERATIONS = 1000

DNA_SIZE = len(TARGET_STRING)
TARGET_ASCII = np.fromstring(TARGET_STRING, dtype=np.uint8)
ASCII_BOUND = [32, 126]


class GA(object):
    def __init__(self, DNA_size, DNA_bound, cross_rate, mutation_rate,
                 pop_size):
        self.DNA_size = DNA_size
        DNA_bound[1] += 1
        self.DNA_bound = DNA_bound
        self.cross_rate = cross_rate
        self.mutation_rate = mutation_rate
        self.pop_size = pop_size
        self.pop = np.random.randint(
            *DNA_bound, size=(pop_size, DNA_size)).astype(np.uint8)

    def translate_DNA(self, DNA):
        return DNA.tostring().decode('ascii')

    def get_fitness(self):
        match_count = (self.pop == TARGET_ASCII).sum(axis=1)
        return match_count

    def select(self):
        fitness = self.get_fitness()
        idx = np.random.choice(
            np.arange(self.pop_size),
            size=self.pop_size,
            replace=True,
            p=fitness / fitness.sum())
        return self.pop[idx]

    def crossover(self, parent, pop):
        if np.random.rand() < self.cross_rate:
            idx = np.random.randint(0, self.pop_size, size=1)
            cross_points = np.random.randint(0, 2,
                                             self.DNA_size).astype(np.bool)
            parent[cross_points] = pop[idx, cross_points]
        return parent

    def mutate(self, child):
        for point in range(self.DNA_size):
            if np.random.rand() < self.mutation_rate:
                child[point] = np.random.randint(*self.DNA_bound)
        return child

    def evolve(self):
        pop = self.select()
        pop_copy = pop.copy()
        for parent in pop:
            child = self.crossover(parent, pop_copy)
            child = self.mutate(child)
            parent[:] = child
        self.pop = pop

    def get_best(self):
        best_DNA = self.pop[np.argmax(fitness)]
        best_string = self.translate_DNA(best_DNA)
        return best_DNA, best_string


if __name__ == "__main__":
    ga = GA(
        DNA_size=DNA_SIZE,
        DNA_bound=ASCII_BOUND,
        cross_rate=CROSS_RATE,
        mutation_rate=MUTATION_RATE,
        pop_size=POP_SIZE)
    with open("StringMatch.txt", "w+") as f:
        f.write("Evolution Algorithm for String Match\n")
        f.write("pop size : {}\n".format(POP_SIZE))
        f.write("cross rate : {}\n".format(CROSS_RATE))
        f.write("mutation rate : {}\n".format(MUTATION_RATE))
        f.write("DNA size : {}\n".format(DNA_SIZE))
    for generation in tqdm(range(N_GENERATIONS)):
        fitness = ga.get_fitness()
        best_DNA, best_string = ga.get_best()
        with open("StringMatch.txt", "a+") as f:
            f.write("Gen {:04d} : {}\n".format(generation, best_string))
        if best_string == TARGET_STRING:
            break
        ga.evolve()
