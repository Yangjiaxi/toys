import numpy as np
import matplotlib.pyplot as plt
import seaborn as sb
sb.set()
sb.set_style("whitegrid")
plt.rcParams['figure.figsize'] = [4, 4]
plt.rcParams['figure.dpi'] = 144
plt.rcParams['font.family'] = "Arial Unicode MS"
plt.rcParams['axes.unicode_minus'] = False

N_CITY = 15
CROSS_RATE = 0.5
MUTATE_RATE = 0.01
POP_SIZE = 1000
N_GENERATION = 500


class Map(object):
    def __init__(self, n_city):
        self.cx = np.random.rand(n_city)
        self.cy = np.random.rand(n_city)

    def __getitem__(self, idx):
        return self.cx[idx], self.cy[idx]

    def get_crood(self, way):
        res = np.zeros((len(way), 2))
        for idx in range(len(way)):
            res[idx] = self[way[idx]]
        # print(way)
        res = res.T
        # print(res.shape)
        return res[0], res[1]

    def dist(self, i, j):
        return np.add((self.cx[i] - self.cx[j]) ** 2,
                      (self.cy[i] - self.cy[j]) ** 2)

    def total_dist(self, way):
        assert len(way) == len(self.cx)
        dis = 0
        for i in range(len(way) - 1):
            dis += self.dist(way[i], way[i + 1])
        return dis


class GA(object):
    def __init__(self, DNA_size, cross_rate, mutation_rate, pop_size, env):
        self.DNA_size = DNA_size
        self.cross_rate = cross_rate
        self.mutation_rate = mutation_rate
        self.pop_size = pop_size
        self.pop = np.vstack([np.random.permutation(DNA_size)
                              for _ in range(pop_size)])
        self.env = env

    def translate_DNA(self, DNA):
        return DNA

    def get_fitness(self):
        res = np.zeros(self.pop_size)
        for idx in range(self.pop_size):
            res[idx] = env.total_dist(self.pop[idx])
        fitness = np.exp(1/res)
        return fitness

    def select(self, fitness):
        idx = np.random.choice(np.arange(
            self.pop_size), size=self.pop_size, replace=True, p=fitness/fitness.sum())
        return self.pop[idx]

    # def cross_over(self, parent, pop):
    #     if np.random.rand() < self.cross_rate:
    #         idx = np.random.randint(0, self.pop_size, size=1)
    #         a = parent.copy()
    #         b = self.pop[idx].copy().reshape((self.DNA_size, ))
    #         # print(a.shape, b.shape)
    #         res = np.zeros_like(a)
    #         idx = np.random.randint(0, 2, size=self.DNA_size).astype(np.bool)
    #         res[idx] = a[idx]
    #         if idx.sum() < self.DNA_size:
    #             res[~idx] = b[np.sort([np.nonzero(b == x)[0][0]
    #                                    for x in a[~idx]])]
    #         parent = res
    #     return parent

    def cross_over(self, parent, pop):
        if np.random.rand() < self.cross_rate:
            # select another individual from pop
            i_ = np.random.randint(0, self.pop_size, size=1)
            cross_points = np.random.randint(0, 2, self.DNA_size).astype(
                np.bool)   # choose crossover points
            # find the city number
            keep_city = parent[~cross_points]
            swap_city = pop[i_, np.isin(
                pop[i_].ravel(), keep_city, invert=True)]
            parent[:] = np.concatenate((keep_city, swap_city))
        return parent

    def mutate(self, child):
        for point in range(self.DNA_size):
            if np.random.rand() < self.mutation_rate:
                idx = np.random.randint(0, self.DNA_size)
                child[point], child[idx] = child[idx], child[point]
        return child

    def evolve(self, fitness):
        pop = self.select(fitness)
        pop_copy = pop.copy()
        for parent in pop:
            child = self.cross_over(parent, pop_copy)
            child = self.mutate(child)
            parent[:] = child
        self.pop = pop

    def show(self, best_DNA, gen):
        plt.clf()
        plt.scatter(env.cx, env.cy)
        lx, ly = self.env.get_crood(best_DNA)
        for (x, y, po) in zip(lx, ly, best_DNA):
            plt.text(x, y, "{}".format(po))
        plt.text(0.03, -0.03,
                 "dist:{:.2f} gen:{}".format(self.env.total_dist(best_DNA), gen))
        plt.plot(lx, ly)
        plt.pause(0.01)


if __name__ == "__main__":
    env = Map(N_CITY)
    ga = GA(DNA_size=N_CITY,
            cross_rate=CROSS_RATE,
            mutation_rate=MUTATE_RATE,
            pop_size=POP_SIZE,
            env=env)

    best = None
    shortest = 999
    for gen in range(N_GENERATION):
        fitness = ga.get_fitness()
        best_way = ga.pop[np.argmax(fitness)]
        dist = env.total_dist(best_way)
        if dist < shortest:
            best = best_way
            shortest = dist
        print("Gen : {} dist:{:.4f}".format(gen, shortest))
        ga.show(best, gen)
        ga.evolve(fitness)
