#ifndef PEDOMETER_H
#define PEDOMETER_H


class Pedometer {
    public:
        Pedometer(void);
        int StepAlgorithm (int x, int y, int z, int stepsCount);
};

#endif //PEDOMETER_H