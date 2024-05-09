#ifndef PEDOMETER_H
#define PEDOMETER_H

class Pedometer {
    public:
        Pedometer(void);
        int StepAlgorithm (int x, int y, int z, int stepsCount);
    private:
        int stepsCount_;
};

#endif //PEDOMETER_H