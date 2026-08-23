#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ctime>

using namespace std;

// 问题参数
constexpr int S = 4;    // 船舶数量
constexpr int B = 2;    // 泊位数量
constexpr int R = 3;    // 堆取料机数量
constexpr int H = 2 * R;    // 储存行数量
constexpr int U = 20;    // 每行的堆位数量
constexpr int A = 3;    // 货物种类

// 船舶数据
struct Ship {
    double arriveTime;    // 到港时间
    double draft;        // 吃水深度
    double length;        // 长度
    double weight;        // 货物重量
    int cargoType;        // 货物种类
    int numBays;        // 所需堆位数
};

// 泊位数据
struct Berth {
    double depth;        // 水深
    double length;        // 长度
    double v[A];        // 卸船速率
};

// 堆取料机数据
struct Reclaimer {
    double g[A];        // 堆料速率
};

// 全局数据
Ship ships[S];
Berth berths[B];
Reclaimer reclaimers[R];
double beltSpeed[B];    // 皮带机带速
double dist[B][R];    // 泊位到堆取料机的距离
double cost[B][H][U];    // 单位货物运输成本

// 遗传算法参数
const int POPULATION_SIZE = 100;
const int MAX_GENERATIONS = 1000;
const double MUTATION_RATE = 0.1;
const double CROSSOVER_RATE = 0.8;
const double ELITE_RATE = 0.1;

// 染色体结构
struct Chromosome {
    int berthAssign[S];        // 船舶泊位分配
    int reclaimerAssign[S];    // 船舶堆取料机分配
    int storageRowAssign[S];    // 船舶储存行分配
    vector<int> bayAssign[S];    // 船舶堆位分配
    int berthOrder[S][S];        // 泊位作业顺序
    int reclaimerOrder[S][S];    // 堆取料机作业顺序
    double fitness;            // 适应度
};

// 随机数生成
random_device rd;
mt19937 gen(rd());

// 初始化数据（与CPLEX代码完全相同的数据）
void initializeData() {
    // 从test.cpp的输出中复制数据
    
    // 船舶数据
    ships[0].arriveTime = 38;    // 到港时间
    ships[0].draft = 16;        // 吃水深度
    ships[0].length = 193;        // 长度
    ships[0].weight = 62437;        // 货物重量
    ships[0].cargoType = 2;        // 货物种类
    ships[0].numBays = 2;        // 所需堆位数
    
    ships[1].arriveTime = 67;
    ships[1].draft = 13;
    ships[1].length = 231;
    ships[1].weight = 92285;
    ships[1].cargoType = 2;
    ships[1].numBays = 2;
    
    ships[2].arriveTime = 141;
    ships[2].draft = 16;
    ships[2].length = 310;
    ships[2].weight = 178100;
    ships[2].cargoType = 0;
    ships[2].numBays = 9;
    
    ships[3].arriveTime = 128;
    ships[3].draft = 21;
    ships[3].length = 330;
    ships[3].weight = 165921;
    ships[3].cargoType = 1;
    ships[3].numBays = 6;
    
    // 泊位数据
    berths[0].depth = 16;        // 水深
    berths[0].length = 255;        // 长度
    berths[0].v[0] = 2942;        // 卸船速率
    berths[0].v[1] = 2089;
    berths[0].v[2] = 3339;
    
    berths[1].depth = 24;
    berths[1].length = 360;
    berths[1].v[0] = 3695;
    berths[1].v[1] = 3064;
    berths[1].v[2] = 4092;
    
    // 堆取料机数据
    reclaimers[0].g[0] = 3932;        // 堆料速率
    reclaimers[0].g[1] = 3366;
    reclaimers[0].g[2] = 4475;
    
    reclaimers[1].g[0] = 4865;
    reclaimers[1].g[1] = 4167;
    reclaimers[1].g[2] = 5352;
    
    reclaimers[2].g[0] = 4534;
    reclaimers[2].g[1] = 4043;
    reclaimers[2].g[2] = 5188;
    
    // 皮带机带速
    beltSpeed[0] = 15000;    // 假设值
    beltSpeed[1] = 15000;
    
    // 泊位到堆取料机的距离
    dist[0][0] = 1038;
    dist[0][1] = 2719;
    dist[0][2] = 2238;
    dist[1][0] = 1437;
    dist[1][1] = 1855;
    dist[1][2] = 2797;
    
    // 单位货物运输成本
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < H; j++) {
            int l = j / 2;
            for (int k = 0; k < U; k++) {
                cost[i][j][k] = dist[i][l] / 1000 + 0.1 * k;
            }
        }
    }
}

// 初始化染色体
Chromosome initializeChromosome() {
    Chromosome chrom;
    
    // 初始化泊位分配
    for (int i = 0; i < S; i++) {
        // 选择合适的泊位（满足水深和长度要求）
        vector<int> validBerths;
        for (int j = 0; j < B; j++) {
            if (berths[j].depth >= ships[i].draft && berths[j].length >= ships[i].length) {
                validBerths.push_back(j);
            }
        }
        if (!validBerths.empty()) {
            chrom.berthAssign[i] = validBerths[rand() % validBerths.size()];
        } else {
            chrom.berthAssign[i] = rand() % B;
        }
    }
    
    // 初始化堆取料机分配
    for (int i = 0; i < S; i++) {
        chrom.reclaimerAssign[i] = rand() % R;
    }
    
    // 初始化储存行分配
    for (int i = 0; i < S; i++) {
        int r = chrom.reclaimerAssign[i];
        int row = 2 * r + (rand() % 2);
        chrom.storageRowAssign[i] = row;
    }
    
    // 初始化堆位分配
    for (int i = 0; i < S; i++) {
        int h = chrom.storageRowAssign[i];
        vector<int> availableBays;
        for (int j = 0; j < U; j++) {
            availableBays.push_back(j);
        }
        random_shuffle(availableBays.begin(), availableBays.end());
        for (int j = 0; j < ships[i].numBays && j < availableBays.size(); j++) {
            chrom.bayAssign[i].push_back(availableBays[j]);
        }
    }
    
    // 初始化作业顺序
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < S; j++) {
            chrom.berthOrder[i][j] = 0;
            chrom.reclaimerOrder[i][j] = 0;
        }
    }
    
    return chrom;
}

// 计算适应度
void calculateFitness(Chromosome& chrom) {
    // 计算转运成本
    double transportCost = 0;
    for (int i = 0; i < S; i++) {
        int b = chrom.berthAssign[i];
        int h = chrom.storageRowAssign[i];
        for (int bay : chrom.bayAssign[i]) {
            transportCost += cost[b][h][bay] * ships[i].weight / ships[i].numBays;
        }
    }
    
    // 计算作业时间和总在港时间
    double totalTime = 0;
    double startTime[S] = {0};
    double endTime[S] = {0};
    double startTimeSR[S] = {0};
    double endTimeSR[S] = {0};
    
    // 计算每艘船的开始和结束时间
    for (int i = 0; i < S; i++) {
        int b = chrom.berthAssign[i];
        int r = chrom.reclaimerAssign[i];
        int cargoType = ships[i].cargoType;
        
        // 计算卸船时间
        double unloadRate = min(berths[b].v[cargoType], reclaimers[r].g[cargoType]);
        double unloadTime = ships[i].weight / unloadRate;
        
        // 开始时间不能早于到港时间
        startTime[i] = ships[i].arriveTime;
        
        // 检查同泊位的其他船舶
        for (int j = 0; j < S; j++) {
            if (i != j && chrom.berthAssign[j] == b) {
                if (chrom.berthOrder[j][i]) {
                    startTime[i] = max(startTime[i], endTime[j]);
                }
            }
        }
        
        endTime[i] = startTime[i] + unloadTime;
        
        // 计算堆料时间
        double transportTime = dist[b][r] / beltSpeed[b];
        startTimeSR[i] = startTime[i] + transportTime;
        endTimeSR[i] = endTime[i] + transportTime;
        
        // 检查同堆取料机的其他船舶
        for (int j = 0; j < S; j++) {
            if (i != j && chrom.reclaimerAssign[j] == r) {
                if (chrom.reclaimerOrder[j][i]) {
                    startTimeSR[i] = max(startTimeSR[i], endTimeSR[j]);
                }
            }
        }
        
        totalTime += endTime[i] - ships[i].arriveTime;
    }
    
    // 计算总适应度（目标函数值）
    double objective = 30000 * totalTime + transportCost;
    chrom.fitness = 1.0 / (1.0 + objective);    // 适应度越大越好
}

// 选择操作（轮盘赌选择）
vector<Chromosome> selection(const vector<Chromosome>& population) {
    vector<Chromosome> selected;
    double totalFitness = 0;
    
    for (const auto& chrom : population) {
        totalFitness += chrom.fitness;
    }
    
    for (int i = 0; i < population.size(); i++) {
        double r = ((double)rand() / RAND_MAX) * totalFitness;
        double sum = 0;
        for (const auto& chrom : population) {
            sum += chrom.fitness;
            if (sum >= r) {
                selected.push_back(chrom);
                break;
            }
        }
    }
    
    return selected;
}

// 交叉操作
Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2) {
    Chromosome child;
    
    // 单点交叉
    int crossPoint = rand() % S;
    
    // 交叉泊位分配
    for (int i = 0; i < crossPoint; i++) {
        child.berthAssign[i] = parent1.berthAssign[i];
    }
    for (int i = crossPoint; i < S; i++) {
        child.berthAssign[i] = parent2.berthAssign[i];
    }
    
    // 交叉堆取料机分配
    for (int i = 0; i < crossPoint; i++) {
        child.reclaimerAssign[i] = parent1.reclaimerAssign[i];
    }
    for (int i = crossPoint; i < S; i++) {
        child.reclaimerAssign[i] = parent2.reclaimerAssign[i];
    }
    
    // 交叉储存行分配
    for (int i = 0; i < crossPoint; i++) {
        child.storageRowAssign[i] = parent1.storageRowAssign[i];
    }
    for (int i = crossPoint; i < S; i++) {
        child.storageRowAssign[i] = parent2.storageRowAssign[i];
    }
    
    // 交叉堆位分配
    for (int i = 0; i < S; i++) {
        if (i < crossPoint) {
            child.bayAssign[i] = parent1.bayAssign[i];
        } else {
            child.bayAssign[i] = parent2.bayAssign[i];
        }
    }
    
    return child;
}

// 变异操作
void mutate(Chromosome& chrom) {
    // 变异泊位分配
    if (rand() % 100 < MUTATION_RATE * 100) {
        int i = rand() % S;
        vector<int> validBerths;
        for (int j = 0; j < B; j++) {
            if (berths[j].depth >= ships[i].draft && berths[j].length >= ships[i].length) {
                validBerths.push_back(j);
            }
        }
        if (!validBerths.empty()) {
            chrom.berthAssign[i] = validBerths[rand() % validBerths.size()];
        }
    }
    
    // 变异堆取料机分配
    if (rand() % 100 < MUTATION_RATE * 100) {
        int i = rand() % S;
        chrom.reclaimerAssign[i] = rand() % R;
        // 更新储存行分配
        int r = chrom.reclaimerAssign[i];
        int row = 2 * r + (rand() % 2);
        chrom.storageRowAssign[i] = row;
    }
    
    // 变异储存行分配
    if (rand() % 100 < MUTATION_RATE * 100) {
        int i = rand() % S;
        int r = chrom.reclaimerAssign[i];
        int row = 2 * r + (rand() % 2);
        chrom.storageRowAssign[i] = row;
    }
    
    // 变异堆位分配
    if (rand() % 100 < MUTATION_RATE * 100) {
        int i = rand() % S;
        int h = chrom.storageRowAssign[i];
        vector<int> availableBays;
        for (int j = 0; j < U; j++) {
            availableBays.push_back(j);
        }
        random_shuffle(availableBays.begin(), availableBays.end());
        chrom.bayAssign[i].clear();
        for (int j = 0; j < ships[i].numBays && j < availableBays.size(); j++) {
            chrom.bayAssign[i].push_back(availableBays[j]);
        }
    }
}

// 遗传算法主函数
double geneticAlgorithm() {
    // 初始化种群
    vector<Chromosome> population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Chromosome chrom = initializeChromosome();
        calculateFitness(chrom);
        population.push_back(chrom);
    }
    
    double bestFitness = 0;
    Chromosome bestChrom;
    
    // 进化过程
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        // 选择
        vector<Chromosome> selected = selection(population);
        
        // 交叉和变异
        vector<Chromosome> newPopulation;
        
        // 保留精英
        int eliteSize = ELITE_RATE * POPULATION_SIZE;
        sort(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) {
            return a.fitness > b.fitness;
        });
        for (int i = 0; i < eliteSize; i++) {
            newPopulation.push_back(population[i]);
        }
        
        // 生成新个体
        while (newPopulation.size() < POPULATION_SIZE) {
            if (rand() % 100 < CROSSOVER_RATE * 100) {
                // 交叉
                int parent1Index = rand() % selected.size();
                int parent2Index = rand() % selected.size();
                Chromosome child = crossover(selected[parent1Index], selected[parent2Index]);
                mutate(child);
                calculateFitness(child);
                newPopulation.push_back(child);
            } else {
                // 变异
                int index = rand() % selected.size();
                Chromosome child = selected[index];
                mutate(child);
                calculateFitness(child);
                newPopulation.push_back(child);
            }
        }
        
        population = newPopulation;
        
        // 更新最佳个体
        for (const auto& chrom : population) {
            if (chrom.fitness > bestFitness) {
                bestFitness = chrom.fitness;
                bestChrom = chrom;
            }
        }
        
        // 输出每代的最佳适应度
        if (generation % 100 == 0) {
            cout << "Generation " << generation << ", Best Fitness: " << bestFitness << endl;
        }
    }
    
    // 计算最佳解的目标函数值
    double transportCost = 0;
    double totalTime = 0;
    double startTime[S] = {0};
    double endTime[S] = {0};
    
    for (int i = 0; i < S; i++) {
        int b = bestChrom.berthAssign[i];
        int h = bestChrom.storageRowAssign[i];
        for (int bay : bestChrom.bayAssign[i]) {
            transportCost += cost[b][h][bay] * ships[i].weight / ships[i].numBays;
        }
        
        int r = bestChrom.reclaimerAssign[i];
        int cargoType = ships[i].cargoType;
        double unloadRate = min(berths[b].v[cargoType], reclaimers[r].g[cargoType]);
        double unloadTime = ships[i].weight / unloadRate;
        
        startTime[i] = ships[i].arriveTime;
        for (int j = 0; j < S; j++) {
            if (i != j && bestChrom.berthAssign[j] == b) {
                if (bestChrom.berthOrder[j][i]) {
                    startTime[i] = max(startTime[i], endTime[j]);
                }
            }
        }
        endTime[i] = startTime[i] + unloadTime;
        totalTime += endTime[i] - ships[i].arriveTime;
    }
    
    double objective = 30000 * totalTime + transportCost;
    cout << "Best Objective Value: " << objective << endl;
    cout << "Transport Cost: " << transportCost << endl;
    cout << "Total Time: " << totalTime << endl;
    
    return objective;
}

int main() {
    // 使用与CPLEX相同的随机种子
    srand(0);
    
    // 初始化数据
    initializeData();
    
    // 输出问题数据
    cout << "Ship Data:" << endl;
    for (int i = 0; i < S; i++) {
        cout << "Ship " << i << ": Arrive Time=" << ships[i].arriveTime 
             << ", Draft=" << ships[i].draft << ", Length=" << ships[i].length 
             << ", Weight=" << ships[i].weight << ", Cargo Type=" << ships[i].cargoType 
             << ", Required Bays=" << ships[i].numBays << endl;
    }
    
    cout << "\nBerth Data:" << endl;
    for (int i = 0; i < B; i++) {
        cout << "Berth " << i << ": Depth=" << berths[i].depth << ", Length=" << berths[i].length << endl;
    }
    
    // 运行遗传算法
    cout << "\nRunning Genetic Algorithm..." << endl;
    double gaResult = geneticAlgorithm();
    
    cout << "\nGenetic Algorithm Result:" << endl;
    cout << "Best Objective Value: " << gaResult << endl;
    
    return 0;
}
