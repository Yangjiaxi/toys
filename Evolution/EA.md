# 进化算法 Evolutionary Algorithms, EAs

## 一、生物的进化

## 二、遗传算法 Genetic Algorithm, GA

### 0.名词

1. 种群 population
2. 亲本
3. 子代
4. 基因 gene/DNA
5. 交叉配对 crossover
6. 变异 mutate
7. 进化 evolve

### 1. 遗传算法

> 遗传算法（Genetic Algorithm, GA）起源于对生物系统所进行的计算机模拟研究。它是模仿自然界生物进化机制发展起来的随机全局搜索和优化方法，借鉴了达尔文的进化论和孟德尔的遗传学说。其本质是一种高效、并行、全局搜索的方法，能在搜索过程中自动获取和积累有关搜索空间的知识，并自适应地控制搜索过程以求得最佳解。

### 2. 基本操作

- 初始化：随机产生种群
- 物竞：根据策略、目标等度量，判断个体的适应程度(fitness)
- 天择：依据适应程度(fitness)选择个体成为亲本个体，适应程度越高的个体被选中的概率应当越高
- 用亲本的染色体按照一定的规则进行交叉(crossover)，产生子代，替换亲本
- 子代产生变异(mutate)
- 形成新种群(evolve)
- ![GA](/assert/ES3.png)

### 3. GA crossover

1. 折半
2. 随机选择
    - 完全随机
    - 概率随机

### 4. GA mutate :  反转

### 存在的问题：

### 5. 精英父母流失

- Microbial Genetic Algorithm, MGA
- 要点：保留好的精英父母(Elitism, 精英主义)
- 选择步（天择）：
  - 从种群中随机挑选两个个体，进行比较
  - winner不改变
  - 用winner改变loser，生成loser_plus
  - loser_plus变异
  - 杀死原有loser，把winner和loser_plus放回种群
- 效果：相对差的个体得到相对好的个体的基因，相对好的个体保留，避免了精英个体流失问题

### 6. 后期搜索太慢

- GA算法的DNA(遗传信息)一般使用二进制序列表示
- 二进制非1即0，变化幅度太大
- 即使已经搜索到最优解附近，想要达到最优解却还要很久
- 进化策略

## 三、进化策略 Evolution Strategy, ES

### 1. DNA：

- GA的DNA：一个二进制向量
  - DNA = 11010010
- ES的DNA：两个(或一个)实数向量
  - 遗传信息（编码性状的信息）
  - 变异强度（决定每一位的变异强度）（可选）
    ```Python
    DNA1=[1.23, -0.13,  2.35, 112.5]
    DNA2=[ 0.1,  2.44, 5.112, 2.144]
    ```
- ES的crossover与mutate:
  - ![ES](/assert/ES4.png)
  - crossover
  - mutate (以高斯变异为例)
    - 按照[变异强度]为标准差，[原有基因]为均值，生成新的基因
    ```Py
        GaussRandom(mean=8.8, std=1) -> 8.7
    ```
    - 在当前基因上加上以0为均值，[变异强度]为标准差的高斯分布
    ```Py
        8.8 + GaussRandom(0, 1) -> 8.7
    ```
- 在ES中有两套信息被遗传给了子代
  - 均值(DNA信息)
  - 方差(变异强度)

### 2. ES步骤

- 初始化$\mu$个个体作为起始种群
- 在种群中随机挑选两个个体作为亲本->crossover->产生一个子代->mutate
- 重复上一步$\lambda$次，生成$\lambda$个子代
- 挑选出$\mu$个个体作为下一代种群

### 3. ES crossover

- 离散重组
  - 折半
  - 随机选择
- 中值重组：对应位置的均值作为子代的值
- 混杂重组
  - 先确定一个亲本
  - 对于这个亲本的每一个DNA位，都**有放回**地从种群中挑选一个个体，进行重组

### 4. ES mutate

- 高斯变异
- 柯西变异
- 变异强度也可以变异，这样变异强度会在快收敛时自动缩小，加速收敛(learning rate decay)
- 高斯变异具有很强的局部搜索能力，但引导个体跳出局部最优解的能力较弱，柯西变异稍好

### 5.选择

- $(\mu+\lambda)-ES$：将亲本和子代放在一起，通过fitness选择$\mu$个个体作为下一代的种群
- $(\mu, \lambda)-ES$ (前提$\lambda \gt \mu$)：舍弃原有亲本，在子代中选择$\mu$个个体作为下一代的种群
  - 该方法通常将变异强度也变异
- $(1+1)-ES$
  - 从种群中随机挑选一个亲本
  - 复制这个亲本作为子代
  - 子代mutate
  - 子代与亲本比较，保留好的
  - 新的种群产生后，进行`MUT_STRENGTH`的更新
    - 还没快收敛的时候增大`MUT_STRENGTH`
    - 快收敛的时候减小`MUT_STRENGTH`
    - 使用$\frac{1}{5}$原则判断是否收敛
      - 如果有$\frac{1}{5}$的变异比原始的亲本好，就是快收敛了
      - 否则就还没收敛
      - > Rechenberg, I. 1973. Evolutionsstrategie – Optimierung technischer Systeme nach Prinzipien der biologischen Evolution, Frommann-Holzboog.
      - ![1+1](/assert/1+1.png)

## 四、神经网络进化 Neuro Evolution, NS

### 1. 固定神经网络的形态，改变参数

- 通过不断尝试变异，修改神经元之间的连接权重，改变神经网络的预测结果，保留预测准的，淘汰不准的。

- https://blog.openai.com/evolution-strategies/

### 2. 修改参数和形态

- NeuroEvolution of Augmenting Topologies, NEAT
- http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf
- http://nn.cs.utexas.edu/downloads/papers/stanley.cec02.pdf

### 3. NEAT

- 对神经元和连接使用innovation ID进行direct coding
- 利用innovation ID对齐亲本网络，进行crossover
- 对神经元node与连接link进行mutate
- 适当保留表现较差的网络(生物多样性)
- 从[input -> output]的最简网络开始进化
- Genome -> network

  ![neat1](/assert/neat1.png)
- mutate node & link

  ![neat2](/assert/neat2.png)
- crossover

  ![neat3](/assert/neat3.png)
