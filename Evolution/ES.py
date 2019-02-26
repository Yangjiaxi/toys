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

DNA_SIZE = 1  # DNA (real number)
DNA_BOUND = [0, 5]  # solution upper and lower bounds
N_GENERATIONS = 150
POP_SIZE = 100  # population size
N_KID = 10  # n kids per generation


def F(x):
    return np.sin(10 * x) * x + np.cos(4 * x) * x
    # return np.sin(0.75*x**3)*x + np.cos(2*x) * x


# find non-zero fitness for selection
def get_fitness(pred):
    return pred.flatten()


def make_kid(pop, n_kid):
    # generate empty kid holder
    kids = {'DNA': np.empty((n_kid, DNA_SIZE))}
    kids['mut_strength'] = np.empty_like(kids['DNA'])
    for kv, ks in zip(kids['DNA'], kids['mut_strength']):
        # crossover (roughly half p1 and half p2)
        p1, p2 = np.random.choice(np.arange(POP_SIZE), size=2, replace=False)
        cp = np.random.randint(
            0, 2, DNA_SIZE, dtype=np.bool)  # crossover points
        kv[cp] = pop['DNA'][p1, cp]
        kv[~cp] = pop['DNA'][p2, ~cp]
        ks[cp] = pop['mut_strength'][p1, cp]
        ks[~cp] = pop['mut_strength'][p2, ~cp]

        # mutate (change DNA based on normal distribution)
        ks[:] = np.maximum(ks + (np.random.rand(*ks.shape) - 0.5),
                           0.)  # must > 0
        kv += ks * np.random.randn(*kv.shape)
        kv[:] = np.clip(kv, *DNA_BOUND)  # clip the mutated value
    return kids


def kill_bad(pop, kids):
    for key in ['DNA', 'mut_strength']:
        pop[key] = np.vstack((pop[key], kids[key]))
    # pop = kids

    fitness = get_fitness(F(pop['DNA']))  # calculate global fitness
    idx = np.arange(pop['DNA'].shape[0])
    # selected by fitness ranking (not value)
    good_idx = idx[fitness.argsort()][-POP_SIZE:]
    for key in ['DNA', 'mut_strength']:
        pop[key] = pop[key][good_idx]
    return pop


pop = dict(
    DNA=5 * np.random.rand(1, DNA_SIZE).repeat(
        POP_SIZE, axis=0),  # initialize the pop DNA values
    mut_strength=np.random.rand(
        POP_SIZE, DNA_SIZE))  # initialize the pop mutation strength values

plt.ion()  # something about plotting
x = np.linspace(*DNA_BOUND, 200)
plt.plot(x, F(x))

for gen in tqdm(range(N_GENERATIONS)):
    # something about plotting
    if 'sca' in globals():
        sca.remove()
    sca = plt.scatter(
        pop['DNA'], F(pop['DNA']), s=50, lw=0, c='green', alpha=0.8)
    plt.pause(0.1)

    # ES part
    kids = make_kid(pop, N_KID)
    pop = kill_bad(pop, kids)  # keep some good parent for elitism

plt.ioff()
plt.show()
